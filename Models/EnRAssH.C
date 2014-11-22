///////////////////////////////////////////////////////////////////////////////
//
// Module name:		EnRAssH.C
// Contents:		Member functions of the classes EnRAssH
//
// Author:		Marc Mueller
// Last modified:	
// By:			Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////

#include "../error.h"
#include "EnRAssH.h"

///////////////////////////////////////////////////////////////////////////////
// Class name:		EnRAssH
// Member function:	EnRAssH
// Purpose:		constructor
//
// Author:		Marc Mueller
// Last modified:	98/10/15
// By:			Marc Mueller
///////////////////////////////////////////////////////////////////////////////
EnRAssH::EnRAssH() : baseModel(1)
{
	zvar = NULL;

	thetaPtr = NULL;
	dPtr = NULL;
	pPtr = NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Class name:		EnRAssH
// Member function:	EnRAssH
// Purpose:		destructor
//
// Author:		Marc Mueller
// Last modified:	98/10/15
// By:			Marc Mueller
///////////////////////////////////////////////////////////////////////////////
EnRAssH::~EnRAssH()
{
	if( zvar ) delete zvar;

	if ( pPtr ) delete [] pPtr;
}

///////////////////////////////////////////////////////////////////////////////
// Class name:		EnRAssH
// Member function:	setLabels
// Purpose:		return a pointer to a variable or a parameter specified
//
// Author:		Marc Mueller
// Last modified:	98/10/15
// By:			Marc Mueller
///////////////////////////////////////////////////////////////////////////////
qreal* EnRAssH::setLabels(char* label)
{
    if( !strcmp(label,"N") ) return( (qreal*)(&N) );
    if( !strcmp(label,"e") ) return(&e);
    if( !strcmp(label,"R") ) return(&R);
    if( !strcmp(label,"x_") ) return( (qreal*)(&x_) );
    if( !strcmp(label,"c_a") ) return(&c_a);
    if( !strcmp(label,"alpha") ) return(&alpha);
    if( !strcmp(label,"alphamin") ) return(&alphamin);
    if( !strcmp(label,"theta0") ) return(&theta0);
    if( !strcmp(label,"theta") ) return(&theta);
    if( !strcmp(label,"thetaE") ) return(&thetaE);
    if( !strcmp(label,"thetaDiff") ) return(&thetaDiff);
    if( !strcmp(label,"d") ) return(&d);
    if( !strcmp(label,"dmid") ) return(&dmid);
    if( !strcmp(label,"dmax") ) return(&dmax);
    if( !strcmp(label,"p0") ) return(&p0);
    if( !strcmp(label,"pmax") ) return(&pmax);
    if( !strcmp(label,"p") ) return(&p);
    if( !strcmp(label,"pOld") ) return(&pOld);
    if( !strcmp(label,"pmid") ) return(&pmid);
    if( !strcmp(label,"MEP") ) return(&MEP);
    if( !strcmp(label,"eps") ) return(&eps);
    if( !strcmp(label,"epsmid") ) return(&epsmid);
    if( !strcmp(label,"avEps") ) return(&avEps);
    if( !strcmp(label,"relEps") ) return(&relEps);
    if( !strcmp(label,"avRelEps") ) return(&avRelEps);
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Class name:		EnRAssH
// Member function:	get_new_d
// Purpose:		get new d
//
// Author:		Marc Mueller
// Last modified:	98/10/15
// By:			Marc Mueller
///////////////////////////////////////////////////////////////////////////////
qreal EnRAssH::get_new_d()
{
	return  (zvar->dice() * dmax);
}

///////////////////////////////////////////////////////////////////////////////
// Class name:		EnRAssH
// Member function:	learn
// Purpose:		update rule
//
// Author:		Marc Mueller
// Last modified:	98/10/15
// By:			Marc Mueller
///////////////////////////////////////////////////////////////////////////////
static qreal last_k(qreal *y, qreal *, const long &k, const long&) {
	return ( y[k-1] );
}
static qreal diff_k(qreal *y, qreal *,const long &k, const long&) {
	return ( y[0]+(y[0]-y[k-1]) );
}
static qreal ma_k(qreal *y, qreal *,const long &k, const long&) {
	qreal	value=0;
	for (int count=0;count<k;count++) value+=y[count];
	return ( value/k );
}
static qreal ols_k(qreal *y, qreal *,const long &k, const long&) {
	qreal	a,b;
	qreal	x_=0;
	qreal	x2_=0;
	qreal	xy_=0;
	qreal	y_=0;
	qreal	term;
    long count;
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
//Log::log() << "\ny_=" << y_ << " x_=" << x_ << " x2_=" << x2_;
//Log::log() << " xy_=" << xy_ << " term=" << term;
	if (term!=0) b=(xy_-x_*y_)/term;
		else b=0;
	a=y_-b*x_;
//Log::log() << " a=" << a << " b=" << b << " count=" << count << endl;

    return ( a+b*count );
}
static qreal ols(qreal *y, qreal *,const long&, const long& t) {

//Log::log() << "t=" << t << " " << ols_k(y,NULL,t,0);
	if(t<2) return(y[0]);
	 else return ( ols_k(y,NULL,t,0) );
}

qreal  EnRAssH::learnAgols(qreal *y,qreal *w,const long &i,const long &t) {
	return ( _learnAgols(y,w,i,t) );
}
qreal  EnRAssH::seroError(qreal *, qreal *,const long&, const long&) {
	return (thetaE);
}
qreal  EnRAssH::constTheta(qreal *, qreal *,const long&,const long&) {
	return (theta0);
}
qreal  EnRAssH::rational(qreal *, qreal *,const long&,const long&) {
	return ((R*thetaOld-dmid)/(1-((alpha*x_/N)*(R*thetaOld-dmid))));
}
//	theta = pOld + 0.5 * (pOld-thetaOld) ;			// lernen

void EnRAssH::learn_init () {

    if (strcmp(learntype,"seroError")==0) learn=&EnRAssH::seroError;
    else if (strcmp(learntype,"constTheta")==0) learn=&EnRAssH::constTheta;
    else if (strcmp(learntype,"rational")==0) learn=&EnRAssH::rational;

    else if (strcmp(learntype,"last_k")==0) {learn=&EnRAssH::learnAgols;_learnAgols=last_k;}
    else if (strcmp(learntype,"diff_k")==0) {learn=&EnRAssH::learnAgols;_learnAgols=diff_k;}
    else if (strcmp(learntype,"ma_k")==0) {learn=&EnRAssH::learnAgols;_learnAgols=ma_k;}
    else if (strcmp(learntype,"ols_k")==0) {learn=&EnRAssH::learnAgols;_learnAgols=ols_k;}
    else if (strcmp(learntype,"ols")==0) {learn=&EnRAssH::learnAgols;_learnAgols=ols;}

    else error("macrodyn::EnRAssH::learn_init: no learn type %s", learntype);
}

///////////////////////////////////////////////////////////////////////////////
// Class name:		EnRAssH
// Member function:	initialize
// Purpose:		initialize the model, define the systems initial state
//
// Author:		Marc Mueller
// Last modified:	98/10/15
// By:			Marc Mueller
///////////////////////////////////////////////////////////////////////////////
void EnRAssH::initialize()
{
	int count;
	if (mem<1) mem=1;

	if ( zvar != NULL ) delete zvar;
	zvar = new rand_var( this, "ranf", "1[0,1];" );
        if( !(zvar) )
	   fatalError("macrodyn::EnRAssH::initialize",
	              "can't create new rand_var");

	if ( pPtr != NULL ) delete [] pPtr;
	pPtr = new qreal[mem+1];
        if( !(pPtr) )
	   fatalError("macrodyn::EnRAssH::initialize",
	              "can't create pPtr");


	for (count=0;count<=mem;count++) pPtr[count]=p0;

	learn_init ();

	alphamin = ( 1 / ( R*e - ( x_ / N ) ) );

	dmid = dmax/2 ;
	dOld = get_new_d();

	thetaOld = theta0;
	pOld = p0;
	pmax = N*e/x_;
}

///////////////////////////////////////////////////////////////////////////////
// Class name:		EnRAssH
// Member function:	loadParamset
// Purpose:		load a parameterset from a specified input file
//
// Author:		Marc Mueller
// Last modified:	98/10/15
// By:			Marc Mueller
///////////////////////////////////////////////////////////////////////////////
void EnRAssH::loadParamset(ifstream& inFile)
{
	int i;
//	inFile >> ni;
//	for (int i=0;i<ni;i++) inFile >> alpha[i];
	inFile >> x_;
	inFile >> N;
	inFile >> e;
	inFile >> R;
	inFile >> dmax;
	inFile >> theta0;    
	inFile >> p0;
	inFile >> learntype;
	inFile >> mem;
	inFile >> length;

	initialize();
}

///////////////////////////////////////////////////////////////////////////////
// Class name:		EnRAssH
// Member function:     saveParamsetWithNames
// Purpose:             add  parameterset to printfile
//
// Author:		Marc Mueller
// Last modified:	98/10/15
// By:			Marc Mueller
///////////////////////////////////////////////////////////////////////////////
void EnRAssH::saveParamsetWithNames(ofstream& outputFile)
{
    outputFile << "\nModel EnRAssH\n";
    outputFile << "N = " << N << "\tx_ = " << x_ << "\te = " << e << endl;
    outputFile << "R = " << R << "\tdmax = " << dmax << endl;
    outputFile << "alpha = " << alpha << endl;
    outputFile << "theta0 = " << theta0 << "\tp0 = " << p0  << endl;
    outputFile << "learn type = " << learntype << "\tmemory = " << mem << endl;
    outputFile << "length = "<< length << "\n";
}

///////////////////////////////////////////////////////////////////////////////
// Class name:		EnRAssH
// Member function:	sendStateSpace
// Purpose:		return pointers to the state variables
//			98/10/15
// Author:		Marc Mueller
// Last modified:	
// By:			Marc Mueller
///////////////////////////////////////////////////////////////////////////////
void EnRAssH::sendStateSpace(int &quantity,const qreal*** stateSpace)
{
    if( stateSpace )
	delete stateSpace;
    *stateSpace= new const qreal* [dimension];
    if( !(*stateSpace) )
	fatalError("EnRAssH::sendStateSpace",
		   "Can't create state space vector");
    quantity=dimension;
    (*stateSpace)[0]=&p;
};

///////////////////////////////////////////////////////////////////////////////
// Class name:		EnRAssH
// Member function:	iteration
// Purpose:		perform one iteration of the system
//
// Author:		Marc Mueller
// Last modified:	98/10/15
// By:			Marc Mueller
///////////////////////////////////////////////////////////////////////////////
void EnRAssH::iteration(const long& t)
{ 
	int count;

	thetaOld = theta;
	dOld = d;
	pOld = p;

	if (alpha<alphamin) {
		alpha=alphamin;
		Log::log() << "alpha=alphamin=" << alphamin << endl;
	}

	qreal dmaxAbs =(  (N/x_) * ( (R*e) - (1/alpha) )  );
	if ( dmax > dmaxAbs )  {
	  Log::log() << "\n time=" << t << " dmax=" << dmax << " > ";
	  Log::log() << dmaxAbs << "=N/x_ * (R*e - 1/alpha) \n";
	  exit (-1);
	  }

	qreal	alphax = alpha*x_;
	qreal	Ralphax = R*alphax;
	
	d = get_new_d();
	qreal	term = (N*(R-1))/(2*Ralphax) - (dmid/(2*R));
	qreal	term2 = term*term;
	thetaE = sqrt( (N*dmid)/Ralphax + term2 ) - term;

    theta = (this->*learn)(pPtr,thetaPtr,mem,t);
	
//Log::log()  << "\n time=" << t << " thetaE="<< thetaE << " theta="<< theta;
	if (theta < 0) theta=0;	// billiger als geschenkt geht nicht

	p = (N*theta)/(R*(alphax*theta+N)) + (d/R);
	qreal	min = N*e/x_ ;
	if (p > min) {
		p = min;
		Log::log() << " min=" << p;
		}
//		else Log::log() << " p=" << p;

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
