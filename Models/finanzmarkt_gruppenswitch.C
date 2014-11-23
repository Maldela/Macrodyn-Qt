///////////////////////////////////////////////////////////////////////////
//																		 //
// Name: finanzmarkt_gruppenswitch.C
//       lineares Finanzmarktmodell mit Erwartungsr"uckkoppelung	
//		 sowie zwei heterogenen Händlergruppen
//	 K=2
//
// Code: Andreas Tonn 
//
///////////////////////////////////////////////////////////////////////////////

#include "../error.h"
#include "finanzmarkt_gruppenswitch.h"

///////////////////////////////////////////////////////////////////////////////
// Funktionsname:	finanzmarkt_gruppenswitch
// Beschreibung:	constructor
///////////////////////////////////////////////////////////////////////////////
finanzmarkt_gruppenswitch::finanzmarkt_gruppenswitch() : baseModel(2), epsilon(1e-25)
{
	A = new matrixDef(2,2);
	if( !A )
		fatalError("matrixDef::matrixDef","Can't allocate memory");
	B = new matrixDef(2,2);
	if( !B )
		fatalError("matrixDef::matrixDef","Can't allocate memory");
	C = new matrixDef(2,1);
	if( !C )
		fatalError("matrixDef::matrixDef","Can't allocate memory");
	Id = new matrixDef(2,2);
	if( !Id )
		fatalError("matrixDef::matrixDef","Can't allocate memory");
	D = new matrixDef(2,1);
	if( !D )
		fatalError("matrixDef::matrixDef","Can't allocate memory");
	ED = new matrixDef(2,1);
	if( !ED )
		fatalError("matrixDef::matrixDef","Can't allocate memory");
	P = new matrixDef(2,1);
	if( !P )
		fatalError("matrixDef::matrixDef","Can't allocate memory");
	P_old = new matrixDef(2,1);
	if( !P_old )
		fatalError("matrixDef::matrixDef","Can't allocate memory");
	PF = new matrixDef(2,1);
	if( !PF )
		fatalError("matrixDef::matrixDef","Can't allocate memory");
	PN = new matrixDef(2,1);
	if( !PN )
		fatalError("matrixDef::matrixDef","Can't allocate memory");
	RHO = new matrixDef(2,1);
	if( !RHO )
		fatalError("matrixDef::matrixDef","Can't allocate memory");
	VF = new matrixDef(2,2);
	if( !VF )
		fatalError("matrixDef::matrixDef","Can't allocate memory");
	VN = new matrixDef(2,2);
	if( !VN )
		fatalError("matrixDef::matrixDef","Can't allocate memory");
	X = new matrixDef(2,1);
	if( !X )
		fatalError("matrixDef::matrixDef","Can't allocate memory");
	XF = new matrixDef(2,1);
	if( !XF )
		fatalError("matrixDef::matrixDef","Can't allocate memory");
	XN = new matrixDef(2,1);
	if( !XN )
		fatalError("matrixDef::matrixDef","Can't allocate memory");
	XF_old = new matrixDef(2,1);
	if( !XF_old )
		fatalError("matrixDef::matrixDef","Can't allocate memory");
	XN_old = new matrixDef(2,1);
	if( !XN_old )
		fatalError("matrixDef::matrixDef","Can't allocate memory");
	Return_F = new matrixDef(1,1);
	if( !Return_F )
		fatalError("matrixDef::matrixDef","Can't allocate memory");
	Return_N = new matrixDef(1,1);
	if( !Return_N )
		fatalError("matrixDef::matrixDef","Can't allocate memory");
	Z1 = new matrixDef(2,2);
	if( !Z1 )
		fatalError("matrixDef::matrixDef","Can't allocate memory");
	Z2 = new matrixDef(2,1);
	if( !Z2 )
		fatalError("matrixDef::matrixDef","Can't allocate memory");
	Z3 = new matrixDef(2,1);
	if( !Z3 )
		fatalError("matrixDef::matrixDef","Can't allocate memory");
	Z4 = new matrixDef(2,2);
	if( !Z4 )
		fatalError("matrixDef::matrixDef","Can't allocate memory");
	Z5 = new matrixDef(2,2);
	if( !Z5 )
		fatalError("matrixDef::matrixDef","Can't allocate memory");
	Z6 = new matrixDef(2,1);
	if( !Z6 )
		fatalError("matrixDef::matrixDef","Can't allocate memory");
	point = new matrixFn();
	if( !point )
		fatalError("matrixDef::matrixDef","Can't allocate memory");
	
	zvar1 = NULL;	
	
	zvar2 = NULL;	
	
	zvar3 = NULL;	
	
	zvar4 = NULL;	


}		


///////////////////////////////////////////////////////////////////////////////
// Funktionsname:	finanzmarkt_gruppenswitch
// Beschreibung:	destructor
///////////////////////////////////////////////////////////////////////////////
finanzmarkt_gruppenswitch::~finanzmarkt_gruppenswitch() {

	if(A)	delete A;
	if(B)	delete B;
	if(C)	delete C;
	if(Id)	delete Id;
	if(D)	delete D;
	if(ED)	delete ED;	
	if(P)	delete P;
	if(P_old)	delete P_old;
	if(PF)	delete PF;
	if(PN)	delete PN;
	if(RHO)	delete RHO;
	if(VF)	delete VF;	
	if(VN)	delete VN;	
	if(X)	delete X;
	if(XF)	delete XF;
	if(XN)	delete XN;		
	if(XF_old)	delete XF_old;
	if(XN_old)	delete XN_old;		
	if(Return_F)	delete Return_F;
	if(Return_N)	delete Return_N;
	if(Z1)	delete Z1;
	if(Z2)	delete Z2;
	if(Z3)	delete Z3;
	if(Z4)	delete Z4;
	if(Z5)	delete Z5;
	if(Z6)	delete Z6;
	if(point)	delete point;
	if( zvar1 ) delete zvar1;
	if( zvar2 ) delete zvar2;
	if( zvar3 ) delete zvar3;
	if( zvar4 ) delete zvar4;
}

///////////////////////////////////////////////////////////////////////////////
// Funktionsname:	setLabels
// Beschreibung:	return a pointer to a variable or a parameter specified
///////////////////////////////////////////////////////////////////////////////
qreal* finanzmarkt_gruppenswitch::setLabels(char* label)
{
	if( !strcmp(label,"R") ) return(&R);
	
	if( !strcmp(label,"xAll1") ) return(&xAll1);
	if( !strcmp(label,"xAll2") ) return(&xAll2);
	if( !strcmp(label,"xF1") ) return(&xF1);
	if( !strcmp(label,"xF2") ) return(&xF2);
  	if( !strcmp(label,"xN1") ) return(&xN1);
	if( !strcmp(label,"xN2") ) return(&xN2);
	if( !strcmp(label,"xf1") ) return(&xf1);
	if( !strcmp(label,"xf2") ) return(&xf2);
  	if( !strcmp(label,"xn1") ) return(&xn1);
	if( !strcmp(label,"xn2") ) return(&xn2);
	if( !strcmp(label,"rho1") ) return(&rho1);
	if( !strcmp(label,"rho2") ) return(&rho2);

	if( !strcmp(label,"pF01") ) return(&pF01);
	if( !strcmp(label,"pF02") ) return(&pF02);
	if( !strcmp(label,"pN01") ) return(&pN01);
	if( !strcmp(label,"pN02") ) return(&pN02);
	if( !strcmp(label,"p01") ) return(&p01);
  	if( !strcmp(label,"p02") ) return(&p02);
  	if( !strcmp(label,"p1") ) return(&p1);
	if( !strcmp(label,"p2") ) return(&p2);
  	if( !strcmp(label,"pF1") ) return(&pF1);
	if( !strcmp(label,"pF2") ) return(&pF2);
  	if( !strcmp(label,"pN1") ) return(&pN1);
	if( !strcmp(label,"pN2") ) return(&pN2);
	if( !strcmp(label,"Ed1") ) return(&Ed1);
	if( !strcmp(label,"Ed2") ) return(&Ed2);
	if( !strcmp(label,"var1") ) return(&var1);
	if( !strcmp(label,"var2") ) return(&var2);
	if( !strcmp(label,"d1") ) return(&d1);
	if( !strcmp(label,"d2") ) return(&d2);
	if( !strcmp(label,"return_F") ) return(&return_F);
	if( !strcmp(label,"return_N") ) return(&return_N);
	if( !strcmp(label,"return_f") ) return(&return_f);
	if( !strcmp(label,"return_n") ) return(&return_n);
	if( !strcmp(label,"mean_f") ) return(&mean_f);
	if( !strcmp(label,"mean_n") ) return(&mean_n);
	if( !strcmp(label,"variance_f") ) return(&variance_f);
	if( !strcmp(label,"variance_n") ) return(&variance_n);
	if( !strcmp(label,"U_f") ) return(&U_f);
	if( !strcmp(label,"U_n") ) return(&U_n);
	if( !strcmp(label,"mu") ) return(&mu);
	
  return NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Funktionsname:	initialize
// Beschreibung:	initialize the model, define the systems initial state
///////////////////////////////////////////////////////////////////////////////
void finanzmarkt_gruppenswitch::initialize()
{
	
	
	
	p1=p01;			//Erstellen des Startvektors der Wertpapierpreise
	p2=p02;
	P->m[0][0]= p1;
	P->m[1][0]= p2;
	
	p1_old=p01;		//Erstellen des Startvektors der alten Wertpapierpreise
	p2_old=p02;
	P_old->m[0][0]= p1_old;
	P_old->m[1][0]= p2_old;
			

	pF01=p01;		//Erstellen des Startvektors fuer die Preisvorhersage
	pF02=p02;		//der Fundamentalisten
	PF->m[0][0]= pF01;
	PF->m[1][0]= pF02;


	pN01=p01;		//Erstellen des Startvektors fuer die Preisvorhersage
	pN02=p01;		//der Noise--Trader
	PN->m[0][0]= pN01;
	PN->m[1][0]= pN02;


	mu=mu0;			//Startwert der Gruppenanteile
//	Xi_F=alphaf/mu;		//Startwert fuer Xi_F
//	Xi_N=alphan/(1-mu);	//Startwert fuer Xi_N


	X->m[0][0]= xAll1;	//Erstellen des Vektors der gesamten Wertpapiere
	X->m[1][0]= xAll2;	
	
	XF->m[0][0] = 0;
	XF->m[1][0] = 0;
	XN->m[0][0] = 0;
	XN->m[1][0] = 0;

	mean_f=0;
	mean_n=0;
	variance_f=0;
	variance_n=0;

	
	Id->m[0][0]= 1;		//Erstellen der Identitaetsmatrix
	Id->m[1][0]= 0;
	Id->m[0][1]= 0;	
	Id->m[1][1]= 1;
	
	zvar1 = new rand_var( "ranf",1,zetamin1,zetamax1 );	
	if( !(zvar1) )
		fatalError("rand_var::initialize stoch_ar","can't create rand_var");
	zvar2 = new rand_var( "ranf",1,zetamin2,zetamax2 );	
	if( !(zvar2) )
		fatalError("rand_var::initialize stoch_ar","can't create rand_var");
	zvar3 = new rand_var( "ranf",1,zetamin3,zetamax3 );	
	if( !(zvar3) )
		fatalError("rand_var::initialize stoch_ar","can't create rand_var");
	zvar4 = new rand_var( "ranf",1,zetamin4,zetamax4 );	
	if( !(zvar4) )
		fatalError("rand_var::initialize stoch_ar","can't create rand_var");
	
	
	d1=((zetamax1-zetamin1)/2)/(1-gamma1);
	var1=((zetamax1-zetamin1)*(zetamax1-zetamin1))/12; // ist konstant !
	d1=gamma1*d1 + zvar1->dice();
	Ed1= gamma1*d1 + ((zetamax1-zetamin1)/2);

	d2=((zetamax2-zetamin2)/2)/(1-gamma2);
	var2=((zetamax2-zetamin2)*(zetamax2-zetamin2))/12; // ist konstant !
	d2=gamma2*d2 + zvar2->dice();
	Ed2= gamma2*d2 + ((zetamax2-zetamin2)/2);


//	rho1=((zetamax3-zetamin3)/2)/(1-gamma3);
//	rho1=gamma3 * rho1 + zvar3->dice();
//	rho2=((zetamax4-zetamin4)/2)/(1-gamma4);
//	rho2=gamma4 * rho2 + zvar4->dice();	

	rho1=zvar3->dice();
	rho2=zvar4->dice();
	
	VF->m[0][0]= var1;	//Erstellen der Kovarianzmatrix der Fundamentalisten
	VF->m[1][0]= 0;
	VF->m[0][1]= 0;	
	VF->m[1][1]= var2;

	VN->m[0][0]= var1+var1err;	//Erstellen der Kovarianzmatrix der Noise--Trader
	VN->m[1][0]= 0+coerr;
	VN->m[0][1]= 0+coerr;	
	VN->m[1][1]= var2+var2err;


	D->m[0][0]= d1;
	D->m[1][0]= d2;
	
	ED->m[0][0]= Ed1;
	ED->m[1][0]= Ed2;
	
	RHO->m[0][0]= rho1;
	RHO->m[1][0]= rho2;


	
}


///////////////////////////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////////////////////////////
void finanzmarkt_gruppenswitch::loadParamset(ifstream& inFile)
{
    inFile >> R;  	//risikoloser Zins (1+r)>0
    inFile >> alphaf;	//Risikoaversionsparameter eines Fundamentalisten 
    inFile >> alphan;	//Risikoaversionsparameter eines Noise--Trader
    inFile >> ef;
    inFile >> en;
    inFile >> mu0;	//Startwert fuer Gruppenanteile
    inFile >> beta;	//Parameter fuer Gruppenswitchfunction
    inFile >> xAll1;	//Gesamtzahl an Anteilsscheinen der Firma 1
    inFile >> xAll2;	//Gesamtzahl an Anteilsscheinen der Firma 2
    inFile >> p01;	//Startwert fuer den Preis eines Anteilscheines der Firma 1
    inFile >> p02;	//Startwert fuer den Preis eines Anteilscheines der Firma 2
    inFile >> zetamin1; //Parameterwerte fuer die Dividenenprozesse
    inFile >> zetamax1;
    inFile >> gamma1;
    inFile >> zetamin2;
    inFile >> zetamax2;
    inFile >> gamma2;
    
    inFile >> var1err;
    inFile >> var2err;
    inFile >> coerr;
    
    inFile >> zetamin3; //Parameterwerte fuer die Fehleinschaetzung der Noise--Trader
    inFile >> zetamax3;
    
    inFile >> zetamin4;
    inFile >> zetamax4;
    
    
    inFile >>length;	//Iterationslaenge
	
    initialize();  
}



///////////////////////////////////////////////////////////////////////////////
// Funktionsname:	sendStateSpace
// Beschreibung:	return pointers to the state variables
///////////////////////////////////////////////////////////////////////////////
void finanzmarkt_gruppenswitch::sendStateSpace(int &quantity,const qreal*** stateSpace)
{
    if( stateSpace )
	delete stateSpace;
    *stateSpace= new const qreal* [2]; // 2 asset in Model finanzmarkt_gruppenswitch
    if( !(*stateSpace) )
	fatalError("finanzmarkt_gruppenswitch::sendStateSpace","Speicherfehler");
    quantity=2; // 2 asset in Model finanzmarkt_gruppenswitch
   (*stateSpace)[0]=&p1;
   (*stateSpace)[1]=&p2;
};

///////////////////////////////////////////////////////////////////////////////
// Funktionsname:	iteration
// Beschreibung:	Durchlauf einer Periode
///////////////////////////////////////////////////////////////////////////////
void finanzmarkt_gruppenswitch::iteration(const long& t)
{
Log::log() << "epsilon=" << epsilon << endl;

//die Abfragen finden statt, um nichtdefierte Werte abzufangen

if((1-mu)<epsilon) {
	A->m[0][0]= 0;		
	A->m[1][0]= 0;
	A->m[0][1]= 0;	
	A->m[1][1]= 0;
	}
else {	
	//A=(Id+(Xi_N/Xi_F)*VN*VF^(-1))^(-1)
	
		point->inverse(VF,Z1);			  //             VF^(-1)
		point->multiply(VN,Z1,Z4);		  //          VN*VF^(-1)
		Xi_NzuXi_F=(alphan/(1-mu))*(mu/alphaf);
		point->multiplyScalar(Xi_NzuXi_F,Z4,Z1);  //   (Xi_N//Xi_F)VN*VF^(-1)
		point->add(Id,Z1,Z4);			  //Id+(Xi_N//Xi_F)VN*VF^(-1)	
		point->inverse(Z4,A);
     }

	//B=Id-A

		point->subtract(Id,A,B);


if(mu<epsilon) {
	//C=Xi_N*VN*X
		point->multiply(VN,X,Z6);	
		point->multiplyScalar(alphan,Z6,C);
	   } 
else {   
	//C=Xi_F(B*VF)*X
		Xi_F=alphaf/mu;
		point->multiply(B,VF,Z1);
		point->multiplyScalar(Xi_F,Z1,Z4);
		point->multiply(Z4,X,C);
      }



//Zufallsdividende:
	d1=gamma1 * d1 + zvar1->dice();
	Ed1= gamma1 * d1 + ((zetamax1-zetamin1)/2) ;
	d2=gamma2 * d2 + zvar2->dice();	
	Ed2= gamma2 * d2 + ((zetamax2-zetamin2)/2) ;

	D->m[0][0]= d1;
	D->m[1][0]= d2;

	ED->m[0][0]= Ed1; //die Erwartungswerte ueber die Dividende werden
	ED->m[1][0]= Ed2; //in jeder Iteration mit t>1 neu berechnet

//Abspeichern der Werte der Vorperiode,
//fuer spaetere Berechnungen und Auswertungen:

	P_old->m[0][0] = P->m[0][0];
	P_old->m[1][0] = P->m[1][0];
	XF_old->m[0][0] = XF->m[0][0];
	XF_old->m[1][0] = XF->m[1][0];
	XN_old->m[0][0] = XN->m[0][0];
	XN_old->m[1][0] = XN->m[1][0];



//Fehleinschaetzung der der Noise--Trader ueber die Preise: 
	
	
//	rho1=gamma3 * rho1 + zvar3->dice();
//	rho2=gamma4 * rho2 + zvar4->dice();	
	
	rho1=zvar3->dice();
	rho2=zvar4->dice();

	RHO->m[0][0]=rho1; 
	RHO->m[1][0]=rho2; 

//Vorhersage der Fundamentalisten:
//PF=(R*PF)-ED-(A*RHO)+C

	point->multiplyScalar(R,PF,Z2);
	point->subtract(Z2,ED,Z3);
	point->multiply(A,RHO,Z2);
	point->subtract(Z3,Z2,Z6);
	point->add(Z6,C,PF);


//Vorhersage der Noise-Trader:
//PN=PF+RHO

	point->add(PF,RHO,PN);
	
	
//Abspeichern der Werte der Vorperiode,
//fuer spaetere Berechnungen und Auswertungen:

//Preisprozess:
//P=(A*PN+B*PF+ED-C)/R

	point->multiply(A,PN,P);
	point->multiply(B,PF,Z3);
	point->add(P,Z3,Z2);
	point->add(Z2,ED,Z3);
	point->subtract(Z3,C,Z6);
	AbZins=1/R;
	point->multiplyScalar(AbZins,Z6,P);


//Errechung der Portfolios:
//Wertpapierbestand der Fundamentalisten
//XF=(Xi_F*VF)^(-1)*[PF+ED-R*P]

if(mu<epsilon) {

	XF->m[0][0]=0;
	XF->m[1][0]=0;
	}
else {
	Xi_F=alphaf/mu;
	point->multiplyScalar(R,P,Z2);  	//      R*P
	point->subtract(ED,Z2,Z3);		//   ED-R*P
	point->add(PF,Z3,Z2);			//PF+ED-R*P
	point->multiplyScalar(Xi_F,VF,Z1);	// Xi_F*VF
	point->inverse(Z1,Z4);			//(Xi_F*VF)^(-1)
	point->multiply(Z4,Z2,XF);		//(Xi_F*VF)^(-1)*[PF+ED-R*P]
	}
	
		
	xF1=XF->m[0][0];
	xF2=XF->m[1][0];


//Werpapierbestand der Noise--Trader
//XN=(Xi_N*VN)^(-1)*[PN+ED-R*P]
if((1-mu)<epsilon) {

	XN->m[0][0]=0;
	XN->m[1][0]=0;
	}
else {
	Xi_N=alphan/(1-mu);
	point->multiplyScalar(R,P,Z2);		//      R*P
	point->subtract(ED,Z2,Z3);		//   ED-R*P
	point->add(PN,Z3,Z2);			//PN+ED-R*P
	point->multiplyScalar(Xi_N,VN,Z1);	// Xi_N*VN
	point->inverse(Z1,Z4);			//(Xi_N*VN)^(-1)
	point->multiply(Z4,Z2,XN);		//(Xi_N*VN)^(-1)*[PF+ED-R*P]
	}

	xN1=XN->m[0][0];
	xN2=XN->m[1][0];


/////////Auswertung://////////////////

//Preise

	p1=P->m[0][0];
	p2=P->m[1][0];


//qrealisierter Return der "alten" Fundamentalisten

//	return_F=(P+D-R*P_old)^(T)*XF_old	

	point->multiplyScalar(R,P_old,Z2);	//     R*P_old
	point->subtract(D,Z2,Z3);		//   D-R*P_old
	point->add(P,Z3,Z2);			// P+D-R*P_old
	point->multiplyATB(Z2,XF_old,Return_F);	//(P+D-R*P_old)^(T)*XF_old

	return_F=Return_F->m[0][0];

if(mu<epsilon) {
	return_F=0;}
else {
	return_F=(return_F/(mu*ef))+(R-1);
	}

//qrealisierter Return der "alten" Noise--Trader

//	return_N=(P+D-R*P_old)^(T)*XN_old	

	point->multiplyScalar(R,P_old,Z2);	//     R*P_old
	point->subtract(D,Z2,Z3);		//   D-R*P_old
	point->add(P,Z3,Z2);			// P+D-R*P_old
	point->multiplyATB(Z2,XN_old,Return_N);	//(P+D-R*P_old)^(T)*XF_old


	return_N=Return_N->m[0][0];

if((1-mu)<epsilon) {
	return_N=0;}
else {
	return_N=(return_N/((1-mu)*en))+(R-1);
	}

	
//Der Gruppenanteil wird jede Periode neu Berechnet!!!


//Errechnung der ,,fitness Function''
//U^F=(mean_f-(alpha/2)variance_f)
//U^N=(mean_n-(alpha/2)variance_n)

Log::log() << "Periode:" << t << endl;
Log::log() << "mu:" << mu << endl;
Log::log() << "return_F:" << return_F << endl;
Log::log() << "return_N:" << return_N << endl;

mean_f=((t-1)*mean_f+return_F)/t;
variance_f=((t-1)*variance_f+(return_F-mean_f)*(return_F-mean_f))/t;


mean_n=((t-1)*mean_n+return_N)/t;
variance_n=((t-1)*variance_n+(return_N-mean_n)*(return_N-mean_n))/t;



U_f=(1+mean_f)*ef-(alphaf/2)*variance_f*ef*ef;
U_n=(1+mean_n)*en-(alphan/2)*variance_n*en*en;

//mu=1/(1+exp(beta*(U_n-U_f)))

z1=(U_n-U_f);
z1=beta*z1;
z1=exp(z1);

if(t<=1000){mu=mu0;}
else {
mu=1/(1+z1);
	}


//////////mu=max{0,min{1,mu+epsilon*(return_f-return_n)}}//////////////////////////////
//
//
//	z1=return_f-return_n;	
//	z1=epsilon*z1;
//	z1=mu+z1;	
//	z1=MIN(1,z1);
//	
//	Log::log() << "z1=" << z1 << endl;
//	
//	mu=MAX(0,z1);
//
//	Log::log() << "mu=" << mu << endl;
////////////////////////////////////////////////////////////////////////////////////

}
	










