///////////////////////////////////////////////////////////////////////////////
//
// Module name:		EnRAss.C
// Contents:		Member functions of the classes EnRAss
//
// Author:		Marc Mueller
// Last modified:	99/05/15
// By:			Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////

#include "../error.h"
#include "EnRAss.h"

///////////////////////////////////////////////////////////////////////////////
// Class name:		EnRAss
// Member function:	EnRAss
// Purpose:		constructor
//
// Author:		Marc Mueller
// Last modified:	98/10/15
// By:			Marc Mueller
///////////////////////////////////////////////////////////////////////////////
EnRAss::EnRAss() : baseModel(1)
{
	zvar = NULL;

	thetaPtr = NULL;
	dPtr = NULL;
	pPtr = NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Class name:		EnRAss
// Member function:	EnRAss
// Purpose:		destructor
//
// Author:		Marc Mueller
// Last modified:	98/10/15
// By:			Marc Mueller
///////////////////////////////////////////////////////////////////////////////
EnRAss::~EnRAss()
{
	if( zvar ) delete zvar;

	if ( pPtr ) delete [] pPtr;
}

///////////////////////////////////////////////////////////////////////////////
// Class name:		EnRAss
// Member function:	setLabels
// Purpose:		return a pointer to a variable or a parameter specified
//
// Author:		Marc Mueller
// Last modified:	98/10/15
// By:			Marc Mueller
///////////////////////////////////////////////////////////////////////////////
qreal* EnRAss::setLabels(const QString& label)
{
    if (label == "N") return( (qreal*)(&N) );
    if (label == "e") return(&e);
    if (label == "R") return(&R);
    if (label == "x_") return( (qreal*)(&x_) );
    if (label == "c_a") return(&c_a);
    if (label == "alpha") return(&alpha);
    if (label == "alphaqMin") return(&alphaqMin);
    if (label == "theta0") return(&theta0);
    if (label == "theta") return(&theta);
    if (label == "thetaE") return(&thetaE);
    if (label == "thetaDiff") return(&thetaDiff);
    if (label == "d") return(&d);
    if (label == "dmid") return(&dmid);
    if (label == "dmax") return(&dmax);
    if (label == "p0") return(&p0);
    if (label == "pqMax") return(&pqMax);
    if (label == "p") return(&p);
    if (label == "pOld") return(&pOld);
    if (label == "pmid") return(&pmid);
    if (label == "MEP") return(&MEP);
    if (label == "eps") return(&eps);
    if (label == "epsmid") return(&epsmid);
    if (label == "avEps") return(&avEps);
    if (label == "relEps") return(&relEps);
    if (label == "avRelEps") return(&avRelEps);
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Class name:		EnRAss
// Member function:	get_new_d
// Purpose:		get new d
//
// Author:		Marc Mueller
// Last modified:	98/10/15
// By:			Marc Mueller
///////////////////////////////////////////////////////////////////////////////
qreal EnRAss::get_new_d()
{
    return  (zvar->dice() * dmax);
}

///////////////////////////////////////////////////////////////////////////////
// Class name:		EnRAss
// Member function:	learn
// Purpose:		update rule
//
// Author:		Marc Mueller
// Last modified:	98/10/15
// By:			Marc Mueller
///////////////////////////////////////////////////////////////////////////////
static qreal last_k(qreal *y, qreal *,const qint64 &k,const qint64&) {
	return ( y[k-1] );
}
static qreal diff_k(qreal *y, qreal *,const qint64 &k,const qint64&) {
	return ( y[0]+(y[0]-y[k-1]) );
}
static qreal ma_k(qreal *y, qreal *,const qint64 &k,const qint64&) {
    qreal	value=0;
    for (qint64 count=0;count<k;count++) value+=y[count];
	return ( value/k );
}
static qreal ols_k(qreal *y, qreal *,const qint64 &k, const qint64&) {
    qreal	a,b;
    qreal	x_=0;
    qreal	x2_=0;
    qreal	xy_=0;
    qreal	y_=0;
    qreal	term;
    qint64 count;
    for (count=0;count<k;count++) {
		y_+=y[count];
		x_+=count;
		x2_+=(count*count);
		xy_+=(y[count]*count);
	}
	y_/=k;
	x_/=k;
	x2_/=k;
	xy_/=k;
	term=(x2_-(x_*x_));
//log() << "\ny_=" << y_ << " x_=" << x_ << " x2_=" << x2_;
//log() << " xy_=" << xy_ << " term=" << term;
	if (term!=0) b=(xy_-x_*y_)/term;
		else b=0;
	a=y_-b*x_;
//log() << " a=" << a << " b=" << b << " count=" << count 
    return ( a+b*count );
}
static qreal ols(qreal *y, qreal *,const qint64 &i,const qint64 &t) {

//log() << "t=" << t << " " << ols_k(y,NULL,t,0);
	if(t<2) return(y[0]);
    //Guess
     //else return ( ols_k(y,NULL,t,0) );
    else return ( ols_k(y,NULL,t,i) );
}

qreal  EnRAss::learnAgols(qreal *y,qreal *w,const qint64 &i,const qint64 &t) {
    return ( _learnAgols(y,w,i,t) );
}
qreal  EnRAss::seroError(qreal *, qreal *,const qint64 &,const qint64&) {
	return (thetaE);
}
qreal  EnRAss::constTheta(qreal *, qreal *,const qint64 &,const qint64&) {
	return (theta0);
}
qreal  EnRAss::rational(qreal *, qreal *,const qint64 &,const qint64&) {
	return ((R*thetaOld-dmid)/(1-((alpha*x_/N)*(R*thetaOld-dmid))));
}
//	theta = pOld + 0.5 * (pOld-thetaOld) ;			// lernen

void EnRAss::learn_init () {

    if (learntype == "seroError") learn=&EnRAss::seroError;
    else if (learntype == "constTheta") learn=&EnRAss::constTheta;
    else if (learntype == "rational") learn=&EnRAss::rational;

    else if (learntype == "last_k") {learn=&EnRAss::learnAgols;_learnAgols=last_k;}
    else if (learntype == "diff_k") {learn=&EnRAss::learnAgols;_learnAgols=diff_k;}
    else if (learntype == "ma_k") {learn=&EnRAss::learnAgols;_learnAgols=ma_k;}
    else if (learntype == "ols_k") {learn=&EnRAss::learnAgols;_learnAgols=ols_k;}
    else if (learntype == "ols") {learn=&EnRAss::learnAgols;_learnAgols=ols;}

    else error("macrodyn::EnRAss::learn_init: no learn type %s", learntype);
}

///////////////////////////////////////////////////////////////////////////////
// Class name:		EnRAss
// Member function:	initialize
// Purpose:		initialize the model, define the systems initial state
//
// Author:		Marc Mueller
// Last modified:	98/10/15
// By:			Marc Mueller
///////////////////////////////////////////////////////////////////////////////
void EnRAss::initialize()
{
	int count;
	if (mem<1) mem=1;

	if ( zvar != NULL ) delete zvar;
	zvar = new rand_var( this, "ranf", "1[0,1];" );
        if( !(zvar) )
	   fatalError("macrodyn::EnRAss::initialize",
	              "can't create new rand_var");

	if ( pPtr != NULL ) delete [] pPtr;
    pPtr = new qreal[mem+1];
        if( !(pPtr) )
	   fatalError("macrodyn::EnRAss::initialize",
	              "can't create pPtr");


	for (count=0;count<=mem;count++) pPtr[count]=p0;

	learn_init ();

    alphaqMin = ( 1 / ( R*e - ( x_ / N ) ) );

    dmid = dmax/2 ;
	dOld = get_new_d();

	thetaOld = theta0;
	pOld = p0;
    pqMax = N*e/x_;
}

///////////////////////////////////////////////////////////////////////////////
// Class name:		EnRAss
// Member function:	loadParamset
// Purpose:		load a parameterset from a specified input file
//
// Author:		Marc Mueller
// Last modified:	98/10/15
// By:			Marc Mueller
///////////////////////////////////////////////////////////////////////////////
void EnRAss::loadParamset(QTextStream& inFile)
{
	inFile >> N;
	inFile >> x_;
	inFile >> e;
	inFile >> R;
    inFile >> dmax;
	inFile >> alpha;
	inFile >> theta0;    
	inFile >> p0;
	inFile >> learntype;
	inFile >> mem;
	inFile >> length;

	initialize();
}

///////////////////////////////////////////////////////////////////////////////
// Class name:		EnRAss
// Member function:     saveParamsetWithNames
// Purpose:             add  parameterset to printfile
//
// Author:		Marc Mueller
// Last modified:	98/10/15
// By:			Marc Mueller
///////////////////////////////////////////////////////////////////////////////
void EnRAss::saveParamsetWithNames(QTextStream& outputFile)
{
    outputFile << "\nModel EnRAss\n";
    outputFile << "N = " << N << "\tx_ = " << x_ << "\te = " << e;
    outputFile << "R = " << R << "\tdmax = " << dmax;
    outputFile << "alpha = " << alpha;
    outputFile << "theta0 = " << theta0 << "\tp0 = " << p0;
    outputFile << "learn type = " << learntype << "\tmemory = " << mem;
    outputFile << "length = "<< length;
}

///////////////////////////////////////////////////////////////////////////////
// Class name:		EnRAss
// Member function:	sendStateSpace
// Purpose:		return pointers to the state variables
//			98/10/15
// Author:		Marc Mueller
// Last modified:	
// By:			Marc Mueller
///////////////////////////////////////////////////////////////////////////////
void EnRAss::sendStateSpace(int &quantity,const qreal*** stateSpace)
{
    if( stateSpace )
	delete stateSpace;
    *stateSpace= new const qreal* [dimension];
    if( !(*stateSpace) )
	fatalError("EnRAss::sendStateSpace",
		   "Can't create state space vector");
    quantity=dimension;
    (*stateSpace)[0]=&p;
};

///////////////////////////////////////////////////////////////////////////////
// Class name:		EnRAss
// Member function:	iteration
// Purpose:		perform one iteration of the system
//
// Author:		Marc Mueller
// Last modified:	98/10/15
// By:			Marc Mueller
///////////////////////////////////////////////////////////////////////////////
void EnRAss::iteration(const qint64& t)
{ 
	int count;

	thetaOld = theta;
	dOld = d;
	pOld = p;

    if (alpha<alphaqMin) {
        alpha=alphaqMin;
        log() << "alpha=alphaqMin=" << alphaqMin;
	}

    qreal dmaxAbs =(  (N/x_) * ( (R*e) - (1/alpha) )  );
    if ( dmax > dmaxAbs )  {
      log() << "\n time=" << t << " dmax=" << dmax << " > ";
      log() << dmaxAbs << "=N/x_ * (R*e - 1/alpha) \n";
	  exit (-1);
	  }

    qreal	alphax = alpha*x_;
    qreal	Ralphax = R*alphax;
	
	d = get_new_d();
    qreal	term = (N*(R-1))/(2*Ralphax) - (dmid/(2*R));
    qreal	term2 = term*term;
	thetaE = sqrt( (N*dmid)/Ralphax + term2 ) - term;

    theta = (this->*learn)(pPtr,thetaPtr,mem,t);
	
//log()  << "\n time=" << t << " thetaE="<< thetaE << " theta="<< theta;
	if (theta < 0) theta=0;	// billiger als geschenkt geht nicht

	p = (N*theta)/(R*(alphax*theta+N)) + (d/R);
    qreal	qMin = N*e/x_ ;
    if (p > qMin) {
        p = qMin;
        log() << " qMin=" << p;
		}
//		else log() << " p=" << p;

//	MEP = ( (alphax* thetaOld + N) * ( d + (R-1) * dOld ) ) / ( N*thetaOld + (alphax*thetaOld+N) * dOld ) +1 - R ;

	pmid = (1/R) * ((theta/(1+(alphax*theta/N)))+dmid);
	epsmid = pmid - thetaOld;	//middle error 
	eps = p - thetaOld; 		//error 
	relEps = eps/p;
	if(t>0) avEps = (avEps*(t-1)+eps)/t;
	if(t>0) avRelEps = (avRelEps*(t-1)+relEps)/t;

	thetaDiff = thetaE - theta;


	
	for (count=mem;count>0;count--) {pPtr[count]=pPtr[count-1];}
	pPtr[0]=p;
	
}
