///////////////////////////////////////////////////////////////////////////
//																		 //
// Name: finanzmarkt_wage.C
//       lineares Finanzmarktmodell mit Erwartungsr"uckkoppelung	
//		 sowie zwei heterogenen Ha"\n"ergruppen
//	 K=2
//
// Code: Andreas Tonn 
//
///////////////////////////////////////////////////////////////////////////////

#include "../error.h"
#include "finanzmarkt_wage.h"


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
static qreal pf_ces ( qreal k, qreal a, qreal b, qreal c, qreal )
{ return ( exp ( log ( a + b * exp(log(k)*c) ) * 1/c) );
  //
  // expression:	[ a + b * k^c ] ^ {1/c}
  // constraints:	c <= 1, c # 0	
}

static qreal pf_ces_prime ( qreal k, qreal a, qreal b, qreal c, qreal )
{ return ( (1/c) * exp ( log ( a + b * exp(log(k)*c) ) * (1/c - 1) ) * \
                             b * c * exp(log(k)*(c-1)) );
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
static qreal pf_ces2 ( qreal k, qreal a, qreal b, qreal c, qreal )
{ return ( c==0 ? a * exp(log(k)*b) : exp ( log ( a * ( (1-b) + b * exp(log(k)*c) ) ) * 1/c) );
  //
  // expression:	a * [ (1-b) + b * k^c ] ^ {1/c}		c != 0
  //			a * k ^ b				c == 0
  // constraints:	c <= 1	
}

static qreal pf_ces2_prime ( qreal k, qreal a, qreal b, qreal c, qreal )
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
static qreal pf_leo ( qreal k, qreal a, qreal b, qreal , qreal d )
{ return ( a / b * ( 1 - exp (-b * k) +d ) );
  //
  // expression:	a/b * ( 1 - e^(-b * k) )
  // constraints:	a,b >= 0
}

static qreal pf_leo_prime ( qreal k, qreal a, qreal b, qreal , qreal )
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
static qreal pf_cd ( qreal k, qreal a, qreal b, qreal , qreal )
{ return ( a/b * exp( b * log(k) ) );
  //
  // expression:	a/b * e^(b * log(k) )
  // constraints:	a > 0, 0<b<=1
}

static qreal pf_cd_prime ( qreal k, qreal a, qreal b, qreal , qreal )
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
static qreal pf_leontiev ( qreal k, qreal a, qreal b, qreal , qreal d)
{ return ( a*k<b ? a*k+d : b+d );
  //
  // expression:	min{a*k,b}+c
}

static qreal pf_leontiev_prime ( qreal k, qreal a, qreal b, qreal , qreal )
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
static qreal pf_app ( qreal k, qreal a, qreal b, qreal c, qreal d)
{
 return (a*k+c*a*(log((1+exp(-b/(c*a)))/(1+exp((1/c)*(k-b/a)))))+d);
}

static qreal pf_app_prime ( qreal k, qreal a, qreal b, qreal c, qreal )
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
static qreal pf_quad ( qreal k, qreal a, qreal b, qreal c, qreal d)
{
 return ((k<=exp(log(1/a)*(1/(b-1)))) ? c*(k-(a/b*exp(log(k)*b)))+d : 
 
                        c*(exp(log(1/a)*(1/(b-1)))-(a/b*exp(log(exp(log(1/a)*(1/(b-1))))*b)))+d);
}

static qreal pf_quad_prime ( qreal k, qreal a, qreal b, qreal c , qreal  )
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
static qreal pf_log ( qreal k, qreal a, qreal b, qreal , qreal d)
{
 return a/b*log(exp(log(k)*b)+1+d);                        
}

static qreal pf_log_prime ( qreal k, qreal a, qreal b, qreal , qreal d  )
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
static qreal pf_gcd ( qreal k, qreal a, qreal b, qreal c, qreal d)
{
 return a/b*(exp(log(k+exp(log(c)*(1/b)))*b)-c)+d;                        
}

static qreal pf_gcd_prime ( qreal k, qreal a, qreal b, qreal c, qreal )
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
// Class name:		finanzmarkt_wage
// Member function:	pf_init
// Purpose:			maps normalized (A,B,C,D) -> (a,b,c,alpha) and sets pf, pf_prime
//
// Author:			Uli Middelberg
// Last modified:	Mon May 11 12:44:16 METDST 1998
// By:				Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////
void finanzmarkt_wage::pf_init ( void )
{
  a=A_pf; b=B_pf; c=C_pf; d=D_pf;
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
      error("macrodyn::finanzmarkt_wage::pf_init: pf type %d not yet implemented : ", QString::number(pf_type));
  }
}




///////////////////////////////////////////////////////////////////////////////
// Funktionsname:	finanzmarkt_wage
// Beschreibung:	constructor
///////////////////////////////////////////////////////////////////////////////
finanzmarkt_wage::finanzmarkt_wage() : baseModel(2) {
	
	zvar = NULL;			
}		
///////////////////////////////////////////////////////////////////////////////
// Funktionsname:	finanzmarkt_wage
// Beschreibung:	destructor
///////////////////////////////////////////////////////////////////////////////
finanzmarkt_wage::~finanzmarkt_wage() {
	if( zvar ) delete zvar;
}

///////////////////////////////////////////////////////////////////////////////
// Funktionsname:	setLabels
// Beschreibung:	return a pointer to a variable or a parameter specified
///////////////////////////////////////////////////////////////////////////////
qreal* finanzmarkt_wage::setLabels(const QString& label)
{
    if (label == "R") return(&R);
	
    if (label == "xAll") return(&xAll);
    if (label == "xF1") return(&xF1);
    if (label == "xF_old") return(&xF_old);
    if (label == "xF11") return(&xF11);
	
    if (label == "xf1") return(&xf1);
    if (label == "xn1") return(&xn1);
    if (label == "rho1") return(&rho1);

    if (label == "Erho1") return(&Erho1);

    if (label == "pF01") return(&pF01);
    if (label == "p01") return(&p01);
    if (label == "p1") return(&p1);
    if (label == "pF1") return(&pF1);
    if (label == "pR1") return(&pR1);
    if (label == "Ed") return(&Ed);
    if (label == "var") return(&var);
    if (label == "div") return(&div);
    if (label == "gamma") return(&gamma);
    if (label == "Haben_F") return(&Haben_F);
    if (label == "Haben_f") return(&Haben_f);
    if (label == "return_F") return(&return_F);
    if (label == "return_f") return(&return_f);
    if (label == "rend_F") return(&rend_F);
    if (label == "rend_f") return(&rend_f);
    if (label == "ErendF_p") return(&ErendF_p);
    if (label == "ErendF_ges") return(&ErendF_ges);
    if (label == "rendf_neu") return(&rendf_neu);

    if (label == "RPF") return(&RPF);
    if (label == "sdRF") return(&sdRF);
    if (label == "Eff_F") return(&Eff_F);
    if (label == "delta_Eff") return(&delta_Eff);

    if (label == "mRF") return(&mRF);
    if (label == "sRF") return(&sRF);
    if (label == "vRF") return(&vRF);
    if (label == "SIZF") return(&SIZF);
    if (label == "SIF") return(&SIF);
    if (label == "delta_SI") return(&delta_SI);

    if (label == "a1") return(&a1);
    if (label == "b1") return(&b1);
    if (label == "c1") return(&c1);
    if (label == "a2") return(&a2);
    if (label == "b2") return(&b2);
    if (label == "c2") return(&c2);
    if (label == "mu0") return(&mu0);
    if (label == "mu") return(&mu);
	
    if (label == "err_div") return(&err_div);

    if (label == "delta_p") return(&delta_p);
    if (label == "delta_div") return(&delta_div);
    if (label == "delta_q") return(&delta_q);
	
    if (label == "xBundle") return(&xBundle);
		

  return NULL;
}


//////////////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		
// Member function:	loadParamset
// Purpose:		load a parameterset from a specified input file
//
// Author:		
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////////////
void finanzmarkt_wage::loadParamset(QDataStream& inFile)
{
    inFile >> pf_type;
    inFile >> A_pf;
    inFile >> B_pf;
    inFile >> C_pf;
    inFile >> D_pf;

	inFile >> n_F;
	inFile >> tau_F;
	inFile >> k_0;

    inFile >> alpha;	//Risikoaversionsparameter eines Fundamentalisten 
    inFile >> ef;
	
    inFile >> xAll;		//Gesamtzahl an Anteilsscheinen der Firma 1
    inFile >> p_0;		//Startwert fuer den Preis eines Anteilscheines der Firma 1
    inFile >> zetamin >> zetamax >> gamma; //Parameterwerte fuer die 
    
    inFile >> var1err;		//Fehlerwerte, die die Noise--Trader auf die 
    inFile >> var2err;		//Werte der Kovarianzmatrix addieren, der 
    inFile >> coerr;		//Fundamentalisten addieren

    inFile >>  b1 >> c1;	//Parameter fuer die Dreieicksverteilung,
    inFile >>  b2 >> c2;	//mit welcher die Fehleinschaetzung der
   							//Noise--Trader modelliert wird	(in der Initialisierung wird
							//der untere Rand auf -b1 bzw.-b2 gestetzt)
    
    inFile >>length;		//Iterationslaenge
	
    initialize();  
}

///////////////////////////////////////////////////////////////////////////////
// Funktionsname:	initialize
// Beschreibung:	initialize the model, define the systems initial state
///////////////////////////////////////////////////////////////////////////////
void finanzmarkt_wage::initialize()
{
//	epsilon=1e-16;

  	pf_init();

	wagerate_t= (*pf)(k_0,a,b,c,d)-k_0*(*pf_prime)(k_0,a,b,c,d);
	Fi=1/(n_F*tau_F)*xAll;
		
	p1=p_0;			//Erstellen des Startvektors der Wertpapierpreise	
	p1_old=p_0;		//Erstellen des Startvektors der alten Wertpapierpreise
	pF01=p1;		//Erstellen des Startvektors fuer die Preisvorhersage
	
	zvar = new rand_var( "ranf",1,zetamin,zetamax);	
	if( !(zvar) )
		fatalError("rand_var::initialize stoch_ar","can't create rand_var");
	
	div=((zetamax-zetamin)/2)/(1-gamma);
	var=((zetamax-zetamin)*(zetamax-zetamin))/12; // ist konstant !	
	VF=var;	//Erstellen der Kovarianzmatrix der Fundamentalisten

//b2=b1;
a1=-b1;
a2=-b2;
z1=b1-a1;				//Erechnen des Wertes von c der Verteilungsfunktion
if (z1==0){				//der Dreiecksverteilungen, denen die Fehlein-
	F_c1=0;				//shaetzungen der Noise--Trader folgen
	}
else {
	F_c1=(c1-a1)/(b1-a1);		
		}			
c2=c1;
z2=b2-a2;
if (z2==0){
	F_c2=0;	
	}
else {
	F_c2=(c2-a2)/(b2-a2);	
	}
}


///////////////////////////////////////////////////////////////////////////////
// Funktionsname:	Wagebill
// Beschreibung:	
///////////////////////////////////////////////////////////////////////////////
void finanzmarkt_wage::Wagebill()
{
	Wagebill_t=n_F*wagerate_t;
}

///////////////////////////////////////////////////////////////////////////////
// Funktionsname:	Savings
// Beschreibung:	
///////////////////////////////////////////////////////////////////////////////
void finanzmarkt_wage::Savings()
{
	Savings_t = Wagebill_t-((PN-ED)*xAll);
}

///////////////////////////////////////////////////////////////////////////////
// Funktionsname:	wagerate
// Beschreibung:	
///////////////////////////////////////////////////////////////////////////////
void finanzmarkt_wage::wagerate()
{
qreal wagerate_max = MAX(0,Savings_t);

	wagerate_t= (*pf)(wagerate_max,a,b,c,d)-wagerate_t*(*pf_prime)(wagerate_max,a,b,c,d);
}

///////////////////////////////////////////////////////////////////////////////
// Funktionsname:	EndogenR
// Beschreibung:	
///////////////////////////////////////////////////////////////////////////////


void finanzmarkt_wage::EndogenR()
{
qreal dummy=MAX(0,Savings_t);

	R=(1-delta)+(*pf_prime)(dummy,a,b,c,d);
}

///////////////////////////////////////////////////////////////////////////////
// Funktionsname:	Save Values
// Beschreibung:	
///////////////////////////////////////////////////////////////////////////////
void finanzmarkt_wage::SaveValue()
{
//Abspeichern der Werte der Vorperiode,
//fuer spaetere Berechnungen und Auswertungen:

	RHO_old= RHO;

//Speichern der Vorhersagen fuer die Berechung der Vorhersagefehler
	pN_old = PN;
    Ed_old = Ed;		
	P_old= P;	
	p1_old = P_old;	
	d1_old = div;		
	XF_old = XF;
	xF_old = XF_old;
}
///////////////////////////////////////////////////////////////////////////////
// Funktionsname:	Dividendenberechnung
// Beschreibung:	Zufallsdividende fuer die laufende Periode:
///////////////////////////////////////////////////////////////////////////////
void finanzmarkt_wage::Dividende()
{
    Ed= gamma*gamma*div+(1+gamma)*((zetamax-zetamin)/2);	
	div=gamma * d + zvar->dice();
	delta_d1=div-Ed;	           //fuer alternative Renditenberchnung
	Ed= gamma * div + ((zetamax-zetamin)/2) ;
	
	D = div;

	deltaD = delta_d1;         //fuer alternative Renditenberchnung

	ED = Ed; 				//die Erwartungswerte ueber die Dividende werden
}


///////////////////////////////////////////////////////////////////////////////
// Funktionsname:	iteration
// Beschreibung:	Durchlauf einer Periode
///////////////////////////////////////////////////////////////////////////////
void finanzmarkt_wage::iteration(const qint64& t)
{

Wagebill();
Savings();
wagerate();
SaveValue();
Dividende();
		
////////////////////////////////////////////////////////////////////////////////////
//Heutige Fehleinschaetzungen der Noise--Trader ueber die Preise der kommenden Periode 
//folgen jeweils einer Dreiecksverteilung zuwischen a und b und dem Modalwert c
////////////////////////////////////////////////////////////////////////////////////

if(F_c1==0){
	rho1=0;		
	}
else  {
	rho1=zvar->dice();
		if(rho1<=F_c1){
		 	rho1=rho1*(b1-a1)*(c1-a1);
			rho1=a1+sqrt(rho1);
			}
		else{
			rho1=(1-rho1)*(b1-a1)*(b1-c1);
			rho1=b1-sqrt(rho1);	
			}	
	}

if(F_c2==0){
	rho2=0;		
	}	
else  {
	rho2=zvar->dice();
		if(rho2<=F_c2){
			rho2=rho2*(b2-a2)*(c2-a2);
			rho2=a2+sqrt(rho2);
			}
		else{
			rho2=(1-rho2)*(b2-a2)*(b2-c2);
			rho2=b2-sqrt(rho2);	
			}	
	}

	RHO = rho1; 

/////////////////////////////////////////////////////////////////////////////
//Preisprozess bei homogenen Erwartungen (Stand: 07.03.2001)
//Achtung!!!! Gilt nur wenn im *.sim-File mu0=0 ist!
////////////////////////////////////////////////////////////////////////////
//Prognoseregel der ,,Noise--Trader`` (alle sind gleich perfekt oder falsch)

// ANDREAS:PN=R*(PN+RHO)-ED+C		      (da mu:=0 ist C:=Xi_N*VN*X)
/*	if(noise==1){
	point->add(PN,RHO,Z2);
	point->multiplyScalar(R,Z2,Z3);
	point->subtract(Z3,ED2,Z2);
	point->add(Z2,C,PN);
	}
    else{
	PN->m[0][0]= pR1;
	PN->m[1][0]= 1;
	}
*/
// NEU: PN=R*(PN+RHO-ED)+FI					FI=1/n_f*tau_F *X

	PN=R*(PN+RHO-ED)+Fi;
		
//Marktraeumender Preis bei homogenen Erwartungen
//P=(PN+ED-C)/R
	
	P=(PN+ED-C)/R;
	p1=P;

/////////////////////////////////////////////////////////////////////////////
//Preisprozess bei heterogenen Erwartungen (Wissensstand: 23.03.2001)
//
/////////////////////////////////////////////////////////////////////////////

//Vorhersage der Fundamentalisten:
//PF=(R*PF)-ED+(A*RHO)+C
//

	PF=R*PF-ED+(A*RHO)+C;
	
//Marktraeumender Preis bei heterogenen Erwartungen:
//P=(A*PN+B*PF+ED-C)/R

	P=(A*PN+B*PF+ED)/R;
	
	p1=P;

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//        Berechung der Vorhersagefehler aus
//	  den Prognosen der Vorperiode und den heutigen Preisen
//	(!!!! nur fuer N--Trader, da die F--Trader immer perfekte ex-dividend Erwartungen liefern
/////////////////////////////////////////////////////////////////////////////////////////////////////

//Prognosefehler fuer Dividenden:

        err_div = d-Ed_old;

//ex--dividend--Prognosefehler:

		err_pN = p1-pN_old;

//cum--dividend--Prognosefehler:

		err_qN = err_div-err_pN;

/////////////////////////////////////////////////////////////////////////////////////////////////
//
//			Berechunung von ,,delta_p'' und ,,delta_q''
//
/////////////////////////////////////////////////////////////////////////////////////////////////

//delta_p:

        delta_p = p1-p1_old;

//delta_div:
		
		delta_div = d - d1_old;

//delta_q1:

        delta_q = delta_p + delta_div;




////////////////////////////////////////////////////////////////////////////////
//Errechung der Portfolios anhand der Funktionen der aggregierten Nachfragen
////////////////////////////////////////////////////////////////////////////////

//Wertpapierbestand der Fundamentalisten (aggregiert und einzeln)

if(mu<epsilon) {
	XF=0;
	
	xF1=XF;
	xf1=0;
	xF11=xF1/100;		//andere Skalierung fuer die graphische
				        //der Bildaufloesung	
	}
else {	
	//XF=(Xi_F*VF)^(-1)*[PF+ED-R*P]

	Xi_F=alpha/mu;

	XF=pow(Xi_F*VF,-1)*(PF+ED-R*P);

	
	xF1=XF;
	xf1=xF1/mu;
	
	xF11=xF1/100;		//andere Skalierung fuer die graphische
						//Ausgabe (sonst bekomme ich Probleme mit
						//der Bildaufloesung	

}



///////////////////////////////////////////////////////////////////////////
//Habenstand:  Anfangsaustattung minus Kosten der Risikoanlage
//			   Haben=e-px
///////////////////////////////////////////////////////////////////////////

//Fundamentalisten
if(mu<epsilon) {
	Haben_F=0;
	Haben_f=0;
	}
else {
	PXF=P*XF;
	pxF=PXF;
	Haben_F=eF-pxF;
	Haben_f=Haben_F/mu;
	}

///////////////////////////////////////////////////////////////////////////////
//Subjektiv erwartete Rendite des Portfolios aus Aktien
//Subjektiv erwartete Rendite des Gesamtportfolios
//////////////////////////////////////////////////////////////////////////////

//Erwartete Rendite des Portfolios aus Aktien (Fundamentaliste)
if(mu<epsilon) {
	ErendF_p=0;
	}
else {	//(PF+ED-P)*XF)/(P*XF)

ErendF_p=(PF+ED-P)*XF/(P*XF);
}

//Erwartete Rendite des Gesamtportfolios (Fundamentalisten)
if(mu<epsilon) {
	ErendF_ges=0;
	}
else {	//((PF+ED-R*P)*XF/mu*ef)+(R-1)


	ErendF_ges=((PF+ED-R*P)*XF/mu*ef)+(R-1);				//((PF+ED-R*P)*XF/mu*ef)+(R-1)
}

//////////////////////////////////////////////////////////////////////////////////
//   Objektive, erwartete Risikopraemien, Renditestandardabweichungen und
//   Steigungen der Effizienzlinien von Fundamentalisten und Noise--Traders
//
//   Es gilt p^F_t=\E_t(p_{t+1})
/////////////////////////////////////////////////////////////////////////////////

//Fundamentalisten:

if(mu<epsilon) {
	RPF=0;
	sdRF=0;
	Eff_F=0;
	}
else {  //Erwartete Risikopraemie der Fundamentalisten:
        //1/e^F((p^F_t+Ed+R*p)*x^F_t)
	
 	RPF=(PF+ED-R*P)*XF/mu*ef;                     //(PF+ED-R*P)*XF/mu*ef)

        //Bedingte Varianz des Fundamentalisten--Rendite:
        //1/e^F (sqrt ((x^F_t)*(V^F)*(x^F_t) ) )
	    	
	sdRF=sqrt(VF*XF*XF)/eF;//sqrt ((x^F_t)*(V^F)*(x^F_t) )

        //Effizienzliniensteigung der Fundamentalisten:

	Eff_F=RPF/sdRF;
}

//////////////////////////////////////////////////////////////////////////////////////
//Portfoliorenditen: ((Konsum in t+1)-Anfangsaustattung)/Anfangsaustattung
//		rend=((c_{t+1})-e)/e
//////////////////////////////////////////////////////////////////////////////////////

//Renditen der "alten" Fundamentalisten	
if(mu<epsilon) {
	return_F=0;
	return_f=0;
	rend_F=0;
	rend_f=0;
	}
else {	//return_F=(P+D-R*P_old)^(T)*XF_old

	Return_F=(P+D-R*P_old)*XF_old;

	return_F=Return_F;
	return_f=return_F/mu;
	rend_F=(return_F/eF)+(R-1);
	rend_f=(return_f/ef)+(R-1);

	}
}

///////////////////////////////////////////////////////////////////////////////
// Funktionsname:	sendStateSpace
// Beschreibung:	return pointers to the state variables
///////////////////////////////////////////////////////////////////////////////
void finanzmarkt_wage::sendStateSpace(int &quantity,const qreal*** stateSpace)
{
    if( stateSpace )
	delete stateSpace;
    *stateSpace= new const qreal* [1]; // 2 asset in Model finanzmarkt_wage
    if( !(*stateSpace) )
	fatalError("finanzmarkt_wage::sendStateSpace","Speicherfehler");
    quantity=1; // 2 asset in Model finanzmarkt_wage
   (*stateSpace)[0]=&p1;
};


