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
real* EnRAssH::setLabels(char* label)
{
    if( !strcmp(label,"N") ) return( (real*)(&N) );
    if( !strcmp(label,"e") ) return(&e);
    if( !strcmp(label,"R") ) return(&R);
    if( !strcmp(label,"x_") ) return( (real*)(&x_) );
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
real EnRAssH::get_new_d()
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
static real last_k(real *y, real *,int &k,long&) {
	return ( y[k-1] );
}
static real diff_k(real *y, real *,int &k,long&) {
	return ( y[0]+(y[0]-y[k-1]) );
}
static real ma_k(real *y, real *,int &k,long&) {
	real	value=0;
	for (int count=0;count<k;count++) value+=y[count];
	return ( value/k );
}
static real ols_k(real *y, real *,int &k,long&) {
	real	a,b;
	real	x_=0;
	real	x2_=0;
	real	xy_=0;
	real	y_=0;
	real	term;
	for (int count=0;count<k;count++) {
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
//cout << "\ny_=" << y_ << " x_=" << x_ << " x2_=" << x2_;
//cout << " xy_=" << xy_ << " term=" << term;
	if (term!=0) b=(xy_-x_*y_)/term;
		else b=0;
	a=y_-b*x_;
//cout << " a=" << a << " b=" << b << " count=" << count << endl;
	return ( a+b*count );
}
static real ols(real *y, real *,int &,long &t) {

//cout << "t=" << t << " " << ols_k(y,NULL,t,0);
	if(t<2) return(y[0]);
	 else return ( ols_k(y,NULL,t,0) );
}

real  EnRAssH::learnAgols(real *y,real *w,int &i,long &t) {
	return ( _learnAgols(y,w,i,t) );
}
real  EnRAssH::seroError(real *, real *,int &,long&) {
	return (thetaE);
}
real  EnRAssH::constTheta(real *, real *,int &,long&) {
	return (theta0);
}
real  EnRAssH::rational(real *, real *,int &,long&) {
	return ((R*thetaOld-dmid)/(1-((alpha*x_/N)*(R*thetaOld-dmid))));
}
//	theta = pOld + 0.5 * (pOld-thetaOld) ;			// lernen

void EnRAssH::learn_init () {

	if (strcmp(learntype,"seroError")==0) learn=seroError;	
	else if (strcmp(learntype,"constTheta")==0) learn=constTheta;	
	else if (strcmp(learntype,"rational")==0) learn=rational;	

	else if (strcmp(learntype,"last_k")==0) {learn=learnAgols;_learnAgols=last_k;}
	else if (strcmp(learntype,"diff_k")==0) {learn=learnAgols;_learnAgols=diff_k;}
	else if (strcmp(learntype,"ma_k")==0) {learn=learnAgols;_learnAgols=ma_k;}
	else if (strcmp(learntype,"ols_k")==0) {learn=learnAgols;_learnAgols=ols_k;}
	else if (strcmp(learntype,"ols")==0) {learn=learnAgols;_learnAgols=ols;}

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
	pPtr = new real[mem+1];
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
//	for(i=0;i<ni;i++) inFile >> alpha[i];
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
void EnRAssH::sendStateSpace(int &quantity,const real*** stateSpace)
{
    if( stateSpace )
	delete stateSpace;
    *stateSpace= new const real* [dimension];
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
		cout << "alpha=alphamin=" << alphamin << endl;
	}

	real dmaxAbs =(  (N/x_) * ( (R*e) - (1/alpha) )  );
	if ( dmax > dmaxAbs )  {
	  cout << "\n time=" << t << " dmax=" << dmax << " > ";
	  cout << dmaxAbs << "=N/x_ * (R*e - 1/alpha) \n";
	  exit (-1);
	  }

	real	alphax = alpha*x_;
	real	Ralphax = R*alphax;
	
	d = get_new_d();
	real	term = (N*(R-1))/(2*Ralphax) - (dmid/(2*R));
	real	term2 = term*term;
	thetaE = sqrt( (N*dmid)/Ralphax + term2 ) - term;

	theta = learn(pPtr,thetaPtr,mem,t);
	
//cout  << "\n time=" << t << " thetaE="<< thetaE << " theta="<< theta;
	if (theta < 0) theta=0;	// billiger als geschenkt geht nicht

	p = (N*theta)/(R*(alphax*theta+N)) + (d/R);
	real	min = N*e/x_ ;
	if (p > min) {
		p = min;
		cout << " min=" << p;
		}
//		else cout << " p=" << p;

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
