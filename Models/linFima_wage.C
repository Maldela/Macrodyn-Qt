///////////////////////////////////////////////////////////////////////////////
//
// Name: linFima_wage.C
//       lineares Finanzmarktmodell mit Erwartungsr"uckkoppelung und Arbeitsmarkt
//	 	K=1
//
// Code: Michael Meyer 
//
///////////////////////////////////////////////////////////////////////////////

#include "../error.h"
#include "linFima_wage.h"

///////////////////////////////////////////////////////////////////////////////
//
//
// Functions:		pf_ces, pf_ces_prime
// Purpose:			production function and its derivative: CES
//
// Author:			Uli Middelberg
// Last modified:	Mon Mar 10 14:44:08 MET 1997
// By:				Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////
static real pf_ces ( real k, real a, real b, real c, real )
{ return ( exp ( log ( a + b * pow(k,c) ) ) * (1/c) );
  //
  // expression:	[ a + b * k^c ] ^ {1/c}
  // constraints:	c <= 1, c # 0	
}

static real pf_ces_prime ( real k, real a, real b, real c, real )
{ return ( (1/c) * exp ( log ( a + b * pow(k,c) ) * (1/c - 1) ) * \
                             b * c * pow(k,(c-1)) );
  //
  // expression:	(1/c) [ a + b * k^c ] ^ (1/c-1) * [ b * c * k^(c-1) ]
  // constraints:	c <= 1, c # 0
}
///////////////////////////////////////////////////////////////////////////////
//
//
// Functions:		pf_ces2, pf_ces2_prime
// Purpose:		production function and its derivative: CES
//
// Author:		Uli Middelberg
// Last modified:	Fri Nov 14 15:01:36 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////
static real pf_ces2 ( real k, real a, real b, real c, real )
{ return ( c==0 ? a * exp(log(k)*b) : exp ( log ( a * ( (1-b) + b * exp(log(k)*c) ) ) * 1/c) );
  //
  // expression:	a * [ (1-b) + b * k^c ] ^ {1/c}		c != 0
  //			a * k ^ b				c == 0
  // constraints:	c <= 1	
}

static real pf_ces2_prime ( real k, real a, real b, real c, real )
{ return ( c==0 ? b * a * exp(log(k)*(b-1)) : \
                (1/c) * exp(log( a * ((1-b) + b * exp(log(k)*c)) ) * (1/c - 1) ) * \
                      a * b * c * exp(log(k)*(c-1)) );
  //
  // expression:	(1/c) * [ a * [ (1-b) + b * k^c ] ] ^ (1/c-1) * [ a * b * c * k^(c-1) ] 	c!=0
  // 			b * a * k ^ (b-1)			c == 0
  // constraints:	c <= 1
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Functions:		pf_leo, pf_leo_prime
// Purpose:		production function and its derivative: leo
//
// Author:		Uli Middelberg
// Last modified:	Mon Mar 10 14:44:08 MET 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////
static real pf_leo ( real k, real a, real b, real , real d )
{ return ( a / b * ( 1 - exp (-b * k) +d ) );
  //
  // expression:	a/b * ( 1 - e^(-b * k) )
  // constraints:	a,b >= 0
}

static real pf_leo_prime ( real k, real a, real b, real , real )
{ return ( a * exp (-b * k) );
  //
  // expression:	a * e^(-b * k)
  // constraints:	a,b >= 0
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Functions:		pf_cd, pf_cd_prime
// Purpose:		production function and its derivative: cobb-douglas
//
// Author:		Uli Middelberg
// Last modified:	Fri Nov 14 14:58:49 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////
static real pf_cd ( real k, real a, real b, real , real )
{ return ( a/b * exp( b * log(k) ) );
  //
  // expression:	a/b * e^(b * log(k) )
  // constraints:	a > 0, 0<b<=1
}

static real pf_cd_prime ( real k, real a, real b, real , real )
{ return ( a/k * exp( b * log(k) ) );
  //
  // expression:	a/k * e^(b * log(k) )
  // constraints:	a > 0, 0<b<=1
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Member functions:	pf_leontiev, pf_leontiev_prime
// Purpose:		production function and its derivative: fraction
//
// Author:		Uli Middelberg
// Last modified:	Mon May 11 12:26:37 METDST 1998
// By:			Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////
static real pf_leontiev ( real k, real a, real b, real , real d)
{ return ( a*k<b ? a*k+d : b+d );
  //
  // expression:	min{a*k,b}+c
}

static real pf_leontiev_prime ( real k, real a, real b, real , real )
{ return ( k<=b/a ? a : 0);
  //
  // expression:	a	k<=b/a
  //			0	k> b/a
}
///////////////////////////////////////////////////////////////////////////////
//
//
// Member functions:	pf_app, pf_app_prime
// Purpose:		production function and its derivative: fraction
//
// Author:		Leo Kaas
// Last modified:	Mon May 11 12:44:43 METDST 1998
// By:			Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////
static real pf_app ( real k, real a, real b, real c, real d)
{
 return (a*k+c*a*(log((1+exp(-b/(c*a)))/(1+exp((1/c)*(k-b/a)))))+d);
}

static real pf_app_prime ( real k, real a, real b, real c, real )
{
 return ( a/( 1+exp( (1/c)*(k-b/a) ) ) );
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Member functions:	pf_quad, pf_quad_prime
// Purpose:		production function and its derivative: fraction
//
// Author:		Michael Meyer
// Last modified:	Wed Mar 15 08:50:08 MET 2000
// By:			Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////
static real pf_quad ( real k, real a, real b, real c, real d)
{
 return ((k<=exp(log(1/a)*(1/(b-1)))) ? c*(k-(a/b*exp(log(k)*b)))+d : 
 
                        c*(exp(log(1/a)*(1/(b-1)))-(a/b*exp(log(exp(log(1/a)*(1/(b-1))))*b)))+d);
}

static real pf_quad_prime ( real k, real a, real b, real c , real  )
{
 return ((k<=exp(log(1/a)*(1/(b-1)))) ? c*(1-(a*exp(log(k)*(b-1)))) : 0);
}
///////////////////////////////////////////////////////////////////////////////
//
//
// Member functions:	pf_log, pf_log_prime
// Purpose:		production function and its derivative: fraction
//
// Author:		Michael Meyer
// Last modified:	Wed Mar 15 08:50:08 MET 2000
// By:			Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////
static real pf_log ( real k, real a, real b, real , real d)
{
 return a/b*log(exp(log(k)*b)+1+d);                        
}

static real pf_log_prime ( real k, real a, real b, real , real d  )
{
 return a*exp(log(k)*(b-1))/(exp(log(k)*b)+1+d);
} 
///////////////////////////////////////////////////////////////////////////////
//
//
// Member functions:	pf_gcd, pf_gcd_prime
// Purpose:		production function and its derivative: fraction
//
// Author:		Michael Meyer
// Last modified:	Wed Mar 15 08:50:08 MET 2000
// By:			Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////
static real pf_gcd ( real k, real a, real b, real c, real d)
{
 return a/b*(exp(log(k+exp(log(c)*(1/b)))*b)-c)+d;                        
}

static real pf_gcd_prime ( real k, real a, real b, real c, real )
{
 return a/b*exp(log(k+exp(log(c)*(1/c)))*(b-1));
}


//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
//****                                                                     ****
//****                    FINANZMARKT_WAGE		                           ****
//****                                                                     ****
//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
//*****************************************************************************

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		linFima_wage
// Member function:	pf_init
// Purpose:			maps normalized (A,B,C,D) -> (a,b,c,alpha) and sets pf, pf_prime
//
// Author:			Uli Middelberg
// Last modified:	Mon May 11 12:44:16 METDST 1998
// By:				Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////
void linFima_wage::pf_init ( void )
{
  a=A; b=B; c=C; d=D;
  switch ( int(pf_type)) {
    case 0 :
      pf=pf_ces; pf_prime=pf_ces_prime; 
    break;
    case 1 :
      pf=pf_leo; pf_prime=pf_leo_prime; 
    break;
    case 2 :
      pf=pf_cd; pf_prime=pf_cd_prime;
    break;
    case 3 :
      pf=pf_leontiev; pf_prime=pf_leontiev_prime;
    break;
    case 4 :
      pf=pf_ces2; pf_prime=pf_ces2_prime;
    break;
    case 5 :
      pf=pf_app; pf_prime=pf_app_prime;
    break;
    case 6 :
      pf=pf_quad; pf_prime=pf_quad_prime;
    break;
    case 7 :
      pf=pf_log; pf_prime=pf_log_prime;
    break;
    case 8 :
      pf=pf_gcd; pf_prime=pf_gcd_prime;
    break;

    default :
      error("macrodyn::finanzmarkt_wage::pf_init: pf type %d not yet implemented : ", pf_type);
  }
}

///////////////////////////////////////////////////////////////////////////////
// Funktionsname:	linFima_wage
// Beschreibung:	constructor
///////////////////////////////////////////////////////////////////////////////
linFima_wage::linFima_wage() : baseModel(1) {
	zvar = NULL;	
}

///////////////////////////////////////////////////////////////////////////////
// Funktionsname:	linFima_wage
// Beschreibung:	destructor
///////////////////////////////////////////////////////////////////////////////
linFima_wage::~linFima_wage() {
if(zvar) delete zvar;
}

///////////////////////////////////////////////////////////////////////////////
// Class name:		linFima_wage
// Funktionsname:	loadParamset
// Beschreibung:	Einlesen der Parameter aus der .sim Datei 
///////////////////////////////////////////////////////////////////////////////
void linFima_wage::loadParamset(ifstream& inFile){

	inFile >> pf_type;

	inFile >> A;
 	inFile >> B;
 	inFile >> C;
 	inFile >> D;

	inFile >> nF;
	inFile >> tauF;
	inFile >> k_0;
	inFile >> delta;

	inFile >> rF; // Risikotoleranz

	inFile >> eF; // Anfangsausstattung

	inFile >> xAll;

	inFile >> p0;

    inFile >> zetamin >> zetamax >> gamma; //Parameterwerte fuer die Dividende

	inFile >> length;

	initialize();
}
///////////////////////////////////////////////////////////////////////////////
// Funktionsname:	initialize
// Beschreibung:	initialize the model, define the systems initial state
///////////////////////////////////////////////////////////////////////////////
void linFima_wage::initialize()
{

	zvar = new rand_var( "ranf",1,zetamin,zetamax );	
	if( !(zvar) )
		fatalError("rand_var::initialize stoch_ar","can't create rand_var");

	div=((zetamax-zetamin)/2)/(1-gamma);
	varDiv=((zetamax-zetamin)*(zetamax-zetamin))/12; // ist konstant !

  	pf_init();
	wagerate_t= (*pf)(k_0,a,b,c,d)-k_0*(*pf_prime)(k_0,a,b,c,d);

	p=p0;
	pOld=p0;
	pPerf=p0;
	pPerfOld=p0;

	pF0=p0;
	pF=pF0;
	pFShift=pF0;
	pFOld=pF0;
	pFperf=pF0;

	VF=varDiv;

	errvar=0;
	dOld=div;
	divvar=0;
}

///////////////////////////////////////////////////////////////////////////////
// Funktionsname:	Wagebill
// Beschreibung:	
///////////////////////////////////////////////////////////////////////////////
void linFima_wage::Wagebill()
{
	Wagebill_t=nF*wagerate_t;
cout << "Wagebill=" << Wagebill_t << endl;
}

///////////////////////////////////////////////////////////////////////////////
// Funktionsname:	Savings
// Beschreibung:	
///////////////////////////////////////////////////////////////////////////////
void linFima_wage::Savings()
{
	Savings_t=Wagebill_t-(pF-Ed)*xAll;
cout << "pF=" << pF << endl;
cout << "Ed=" << Ed << endl;
cout << "Savings=" << Savings_t << endl;
}

///////////////////////////////////////////////////////////////////////////////
// Funktionsname:	wagerate
// Beschreibung:	
///////////////////////////////////////////////////////////////////////////////
void linFima_wage::wagerate()
{
	real wagerate_max = MAX(0,Savings_t);
	
cout << "max=" << wagerate_max << endl;

	wagerate_t= (*pf)(wagerate_max,a,b,c,d)-wagerate_max*(*pf_prime)(wagerate_max,a,b,c,d);
cout << "wagerate_dyn=" << wagerate_t << endl;
}

///////////////////////////////////////////////////////////////////////////////
// Funktionsname:	EndogenR
// Beschreibung:	
///////////////////////////////////////////////////////////////////////////////


void linFima_wage::EndogenR()
{
real dummy=MAX(0,Savings_t);
R=(1-delta)+(*pf_prime)(dummy,a,b,c,d);
cout << "R=" << R << endl;
}


///////////////////////////////////////////////////////////////////////////////
// Funktionsname:	iteration
// Beschreibung:	Durchlauf einer Periode
///////////////////////////////////////////////////////////////////////////////
void linFima_wage::iteration(const long& t)
{
// Die Prognosen der Chartisten und Fundamentalisten ist extern vorgeschaltet !

// Dividendenprozess (zufällig)
	div=gamma * div + zvar->dice();
	Ed= gamma * div + ((zetamax-zetamin)/2) ;


	Wagebill();
	Savings();
	wagerate();
	EndogenR();

	real pOld2,pPerfOld2;

// Perfekte Prognose
	if(t>1)
		{ 			// in Periode 1 werden die Startwerte genommen
					// wenn Fundis perfekte Prognosen machen sollen
		if(perfectPredictor) 
			Fi=1/(nF*tauF)*xAll;
 			pF=R*(pF-Ed)+Fi;
cout << "pF=" << pF << endl;
//Mark			pF=((R*pF-r*pC-Ed)/(1-r))+(1/rF)*VF*xAll;
		
		 // falls Fundis extern vorgeschaltet sind, dann wird
		 // ihre Prognose mit der perfekten Prognose "uberschrieben
		
		 	pFperf=((R*pFperf-r*pCperf-Ed)/(1-r))+(1/rF)*VF*xAll; 
		
		 // zum Vergleich !
		 // kann in der gleichen Zeitreihe mit p dargestellt werden
		 // qCperf muss dazu zus"atzlich vorgeschaltet werden und in
		 // der gleichen Weise wie qC gebildet werden !!!
		}

// Dynamik
	pPerfOld2=pPerfOld;
	pPerfOld=pPerf;

	pOld2=pOld;
	pOld=p;
	xFold=xF;
	xCold=xC;
	pFOld=pFShift; 	// da die Prognosen vorgeschaltet sind 
	pFShift=pF; 	// kann nicht direkt pFOld=pF gesetzt werden
	pCOld=pCShift;
	pCShift=pC;

// Preisprozess

	// r,b siehe oben
//Mark	p=(r*pC+(1-r)*pF+de-b_preis)/R; 
	p=1/R*(pF-Fi);	

	pPerf=(r*pCperf+(1-r)*pFperf+de-b_preis)/R; // nur zum Vergleich !!!

// Portfolio
	xF=rF*(pF+Ed-R*p)/Vd; // tatsaechliches Portfolio der Jungen (varianzminimal)
	xC=rC*(pC+Ed-R*p)/Vd;
	xFrel=xF/xAll;	// rel. Portfolio bezug zur Aktienanzahl
	xCrel=xC/xAll;

// Auswertung
	wF=eF+(p+div-R*pOld)*xFold; // tatsaechliches Vermoegen der Alten
	wC=eC+(p+div-R*pOld)*xCold;

	RF=(wF-eF)/eF; // Rendite der Alten
	RC=(wC-eC)/eC;
	RpF=RF-(R-1); // tatsaechliche Risikopraemie der Alten
	RpC=RC-(R-1);

	ferrF=(p-pFOld)/p;
	ferrC=(p-pCOld)/p;

	deltap=p-pOld;

	ertrag=(deltap+div)/p;

	deltapF = pF-pFOld;

	errvar=(errvar*(t-1)+((p+div-pFOld-Ed)*(p+div-pFOld-Ed)))/t;// Schaetzer Fehlervarianz

	divvar=(divvar*(t-1)+((div-dOld)*(div-dOld)))/t;// Schaetzer Dividendenvarianz
	dOld=div;

	pDiff=(pPerf-p)/p;

}


///////////////////////////////////////////////////////////////////////////////
// Funktionsname:	setLabels
// Beschreibung:	return a pointer to a variable or a parameter specified
///////////////////////////////////////////////////////////////////////////////
real* linFima_wage::setLabels(char* label)
{
//	if( !strcmp(label,"intVarName") ) return( (real*)(&intVarName) );
	if( !strcmp(label,"R") ) return(&R);
	if( !strcmp(label,"x") ) return(&xAll);

	if( !strcmp(label,"p") ) return(&p);
	if( !strcmp(label,"p0") ) return(&p0);
  	if( !strcmp(label,"pOld") ) return(&pOld);
  	if( !strcmp(label,"pPerf") ) return(&pPerf);
  	if( !strcmp(label,"pDiff") ) return(&pDiff);

	if( !strcmp(label,"rF") ) return(&rF);
	if( !strcmp(label,"eF") ) return(&eF);
	if( !strcmp(label,"pF0") ) return(&pF0);
	if( !strcmp(label,"pF") ) return(&pF);
	if( !strcmp(label,"pFOld") ) return(&pFOld);
	if( !strcmp(label,"qOld") ) return(&pFOld);
  	if( !strcmp(label,"pFperf") ) return(&pFperf);
	if( !strcmp(label,"xF") ) return(&xF);
	if( !strcmp(label,"xFold") ) return(&xFold);
	if( !strcmp(label,"xFrel") ) return(&xFrel);
	if( !strcmp(label,"wF") ) return(&wF);
	if( !strcmp(label,"RF") ) return(&RF);
	if( !strcmp(label,"RpF") ) return(&RpF);

	if( !strcmp(label,"rC") ) return(&rC);
	if( !strcmp(label,"eC") ) return(&eC);
	if( !strcmp(label,"pC0") ) return(&pC0);
	if( !strcmp(label,"pC") ) return(&pC);
  	if( !strcmp(label,"pCperf") ) return(&pCperf);
	if( !strcmp(label,"xC") ) return(&xC);
	if( !strcmp(label,"xCold") ) return(&xCold);
	if( !strcmp(label,"xCrel") ) return(&xCrel);
	if( !strcmp(label,"wC") ) return(&wC);
	if( !strcmp(label,"RC") ) return(&RC);
	if( !strcmp(label,"RpC") ) return(&RpC);

	if( !strcmp(label,"de") ) return(&de);
	if( !strcmp(label,"Ed") ) return(&Ed);
	if( !strcmp(label,"Vd") ) return(&Vd);
	if( !strcmp(label,"div") ) return(&div);

	if( !strcmp(label,"ferr") ) return(&ferrF);
	if( !strcmp(label,"ferrF") ) return(&ferrF);
	if( !strcmp(label,"ferrC") ) return(&ferrC);
	if( !strcmp(label,"deltap") ) return(&deltap);
	if( !strcmp(label,"deltaq") ) return(&deltapF);
	if( !strcmp(label,"deltapF") ) return(&deltapF);
	if( !strcmp(label,"ertrag") ) return(&ertrag);
	if( !strcmp(label,"return") ) return(&ertrag);

	if( !strcmp(label,"errvar") ) return(&errvar);
	if( !strcmp(label,"divvar") ) return(&divvar);
	if( !strcmp(label,"Savings_t") ) return(&Savings_t);


  return NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Funktionsname: saveParamsetWithNames
// Beschreibung:         add  parameterset to printfile
///////////////////////////////////////////////////////////////////////////////
void linFima_wage::saveParamsetWithNames(ofstream& outputFile)
{
    outputFile << "\nModel linFima_wage\n";
    if(perfectPredictor)
	outputFile << "perfectPredictor" << endl;
    outputFile << "R = " << R << endl;
    outputFile << "xAll= " << xAll << " ";
    outputFile << "p0= " << p0 << " ";
    outputFile << endl;
    outputFile << "length = "<< length << endl;
}

///////////////////////////////////////////////////////////////////////////////
// Funktionsname:	sendStateSpace
// Beschreibung:		return pointers to the state variables
///////////////////////////////////////////////////////////////////////////////
void linFima_wage::sendStateSpace(int &quantity,const real*** stateSpace)
{
    if( stateSpace )
	delete stateSpace;
    *stateSpace= new const real* [1]; // 1 asset in Model linFima_wage
    if( !(*stateSpace) )
	fatalError("linFima_wage::sendStateSpace","Speicherfehler");
    quantity=1; // 1 asset in Model linFima_wage
   (*stateSpace)[0]=&p;
};
