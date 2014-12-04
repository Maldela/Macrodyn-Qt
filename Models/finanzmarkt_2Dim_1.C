///////////////////////////////////////////////////////////////////////////
//																		 //
// Name: finanzmarkt_2Dim_1.C
//       lineares Finanzmarktmodell mit Erwartungsr"uckkoppelung	
//		 sowie zwei heterogenen Ha"\n"ergruppen
//	 K=2
//
// Code: Andreas Tonn 
//
///////////////////////////////////////////////////////////////////////////////

#include "../error.h"
#include "finanzmarkt_2Dim_1.h"

///////////////////////////////////////////////////////////////////////////////
// Funktionsname:	finanzmarkt_2Dim_1
// Beschreibung:	constructor
///////////////////////////////////////////////////////////////////////////////
finanzmarkt_2Dim_1::finanzmarkt_2Dim_1() : baseModel(2) {

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
	deltaD = new matrixDef(2,1);
	if( !deltaD )
		fatalError("matrixDef::matrixDef","Can't allocate memory");
	ED = new matrixDef(2,1);
	if( !ED )
		fatalError("matrixDef::matrixDef","Can't allocate memory");
	ED2 = new matrixDef(2,1);
	if( !ED2 )
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
	RHO_old = new matrixDef(2,1);
	if( !RHO_old )
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
	PXF = new matrixDef(1,1);
	if( !PXF )
		fatalError("matrixDef::matrixDef","Can't allocate memory");
	PXN = new matrixDef(1,1);
	if( !PXN )
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
	Z7 = new matrixDef(1,1);
	if( !Z7 )
		fatalError("matrixDef::matrixDef","Can't allocate memory");
	Z8 = new matrixDef(1,1);
	if( !Z8 )
		fatalError("matrixDef::matrixDef","Can't allocate memory");
	Z9 = new matrixDef(2,1);
	if( !Z9 )
		fatalError("matrixDef::matrixDef","Can't allocate memory");
	wichtig1 = new matrixDef(2,2);
	if( !wichtig1 )
		fatalError("matrixDef::matrixDef","Can't allocate memory");
	wichtig2 = new matrixDef(2,2);
	if( !wichtig2 )
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
// Funktionsname:	finanzmarkt_2Dim_1
// Beschreibung:	destructor
///////////////////////////////////////////////////////////////////////////////
finanzmarkt_2Dim_1::~finanzmarkt_2Dim_1() {

	if(A)	delete A;
	if(B)	delete B;
	if(C)	delete C;
	if(Id)	delete Id;
	if(D)	delete D;
	if(deltaD)	delete deltaD;
	if(ED)	delete ED;	
    if(ED2)	delete ED2;		
	if(P)	delete P;
	if(P_old)	delete P_old;
	if(PF)	delete PF;
	if(PN)	delete PN;
	if(RHO)	delete RHO;
	if(RHO_old)	delete RHO_old;
	if(VF)	delete VF;	
	if(VN)	delete VN;	
	if(X)	delete X;
	if(XF)	delete XF;
	if(XN)	delete XN;		
	if(XF_old)	delete XF_old;
	if(XN_old)	delete XN_old;		
	if(PXF)	delete PXF;
	if(PXN)	delete PXN;	
	if(Return_F)	delete Return_F;
	if(Return_N)	delete Return_N;
	if(Z1)	delete Z1;
	if(Z2)	delete Z2;
	if(Z3)	delete Z3;
	if(Z4)	delete Z4;
	if(Z5)	delete Z5;
	if(Z6)	delete Z6;
	if(Z7)	delete Z7;
	if(Z8)	delete Z8;
	if(Z9)	delete Z9;
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
qreal* finanzmarkt_2Dim_1::setLabels(char* label)
{
	if( !strcmp(label,"R") ) return(&R);
	
	if( !strcmp(label,"xAll1") ) return(&xAll1);
	if( !strcmp(label,"xAll2") ) return(&xAll2);
	if( !strcmp(label,"xF1") ) return(&xF1);
	if( !strcmp(label,"xF2") ) return(&xF2);
  	if( !strcmp(label,"xN1") ) return(&xN1);
	if( !strcmp(label,"xN2") ) return(&xN2);
	
	if( !strcmp(label,"xF_old1") ) return(&xF_old1);
	if( !strcmp(label,"xF_old2") ) return(&xF_old2);
  	if( !strcmp(label,"xN_old1") ) return(&xN_old1);
	if( !strcmp(label,"xN_old2") ) return(&xN_old2);


	if( !strcmp(label,"xF11") ) return(&xF11);
	if( !strcmp(label,"xF22") ) return(&xF22);
  	if( !strcmp(label,"xN11") ) return(&xN11);
	if( !strcmp(label,"xN22") ) return(&xN22);	
	
	if( !strcmp(label,"xf1") ) return(&xf1);
	if( !strcmp(label,"xf2") ) return(&xf2);
  	if( !strcmp(label,"xn1") ) return(&xn1);
	if( !strcmp(label,"xn2") ) return(&xn2);
	if( !strcmp(label,"rho1") ) return(&rho1);
	if( !strcmp(label,"rho2") ) return(&rho2);

	if( !strcmp(label,"Erho1") ) return(&Erho1);
	if( !strcmp(label,"Erho2") ) return(&Erho2);

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
	if( !strcmp(label,"pR1") ) return(&pR1);	
	if( !strcmp(label,"Ed1") ) return(&Ed1);
	if( !strcmp(label,"Ed2") ) return(&Ed2);
	if( !strcmp(label,"var1") ) return(&var1);
	if( !strcmp(label,"var2") ) return(&var2);
	if( !strcmp(label,"d1") ) return(&d1);
	if( !strcmp(label,"d2") ) return(&d2);
	if( !strcmp(label,"gamma1") ) return(&gamma1);
	if( !strcmp(label,"gamma2") ) return(&gamma2);
	if( !strcmp(label,"Haben_F") ) return(&Haben_F);
	if( !strcmp(label,"Haben_N") ) return(&Haben_N);
	if( !strcmp(label,"Haben_f") ) return(&Haben_f);
	if( !strcmp(label,"Haben_n") ) return(&Haben_n);	
	if( !strcmp(label,"return_F") ) return(&return_F);
	if( !strcmp(label,"return_N") ) return(&return_N);	
	if( !strcmp(label,"return_f") ) return(&return_f);
	if( !strcmp(label,"return_n") ) return(&return_n);	
	if( !strcmp(label,"rend_F") ) return(&rend_F);
	if( !strcmp(label,"rend_N") ) return(&rend_N);
	if( !strcmp(label,"rend_f") ) return(&rend_f);
	if( !strcmp(label,"rend_n") ) return(&rend_n);
	if( !strcmp(label,"ErendF_p") ) return(&ErendF_p);
	if( !strcmp(label,"ErendF_ges") ) return(&ErendF_ges);
	if( !strcmp(label,"ErendN_p") ) return(&ErendN_p);
	if( !strcmp(label,"ErendN_ges") ) return(&ErendN_ges);
	if( !strcmp(label,"rendf_neu") ) return(&rendf_neu);
	if( !strcmp(label,"rendn_neu") ) return(&rendn_neu);

	if( !strcmp(label,"RPF") ) return(&RPF);
	if( !strcmp(label,"RPN") ) return(&RPN);
	if( !strcmp(label,"sdRF") ) return(&sdRF);
	if( !strcmp(label,"sdRN") ) return(&sdRN);
	if( !strcmp(label,"Eff_F") ) return(&Eff_F);
	if( !strcmp(label,"Eff_N") ) return(&Eff_N);
	if( !strcmp(label,"delta_Eff") ) return(&delta_Eff);

	if( !strcmp(label,"mRF") ) return(&mRF);
	if( !strcmp(label,"mRN") ) return(&mRN);
	if( !strcmp(label,"sRF") ) return(&sRF);
	if( !strcmp(label,"sRN") ) return(&sRN);
	if( !strcmp(label,"vRF") ) return(&vRF);
	if( !strcmp(label,"vRN") ) return(&vRN);
	if( !strcmp(label,"SIZF") ) return(&SIZF);
	if( !strcmp(label,"SIZN") ) return(&SIZN);	
	if( !strcmp(label,"SIF") ) return(&SIF);
	if( !strcmp(label,"SIN") ) return(&SIN);	
	if( !strcmp(label,"delta_SI") ) return(&delta_SI);	

	if( !strcmp(label,"a1") ) return(&a1);
	if( !strcmp(label,"b1") ) return(&b1);
	if( !strcmp(label,"c1") ) return(&c1);
	if( !strcmp(label,"a2") ) return(&a2);
	if( !strcmp(label,"b2") ) return(&b2);
	if( !strcmp(label,"c2") ) return(&c2);	
	if( !strcmp(label,"mu0") ) return(&mu0);	
	if( !strcmp(label,"mu") ) return(&mu);	
	
	if( !strcmp(label,"err_div1") ) return(&err_div1);
	if( !strcmp(label,"err_div2") ) return(&err_div2);
	if( !strcmp(label,"err_pN1") ) return(&err_pN1);
	if( !strcmp(label,"err_pN2") ) return(&err_pN2);
	if( !strcmp(label,"err_qN1") ) return(&err_qN1);
	if( !strcmp(label,"err_qN2") ) return(&err_qN2);

	if( !strcmp(label,"delta_p1") ) return(&delta_p1);
	if( !strcmp(label,"delta_p2") ) return(&delta_p2);
	if( !strcmp(label,"delta_div1") ) return(&delta_div1);
	if( !strcmp(label,"delta_div2") ) return(&delta_div2);
	if( !strcmp(label,"delta_q1") ) return(&delta_q1);
	if( !strcmp(label,"delta_q2") ) return(&delta_q2);
	
	if( !strcmp(label,"xBundle") ) return(&xBundle);
		

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
void finanzmarkt_2Dim_1::loadParamset(ifstream& inFile)
{
    inFile >> R;  		//risikoloser Zins (1+r)>0
    inFile >> alphaf;		//Risikoaversionsparameter eines Fundamentalisten 
    inFile >> alphan;		//Risikoaversionsparameter eines Noise--Trader
    inFile >> ef;
    inFile >> en;
    inFile >> mu0;		//Startwert fuer Anteil an Fundamentalisten [0,1]

//    inFile >> beta;             //Variablen fuer Gruppenswitch
//   inFile >> etafix;           //Achtung!!! bei etafix=1 gibt es keinen Gruppenswitch
//    inFile >> etaF;             //Unbelehrbare Fundamentalisten
	
    inFile >> xAll1;		//Gesamtzahl an Anteilsscheinen der Firma 1
    inFile >> xAll2;		//Gesamtzahl an Anteilsscheinen der Firma 2
    inFile >> p1;		//Startwert fuer den Preis eines Anteilscheines der Firma 1
    inFile >> p2;		//Startwert fuer den Preis eines Anteilscheines der Firma 2
    inFile >> zetamin1 >> zetamax1 >> gamma1; //Parameterwerte fuer die 
    inFile >> zetamin2 >> zetamax2 >> gamma2;  //Dividenenprozesse
    
    inFile >> var1err;		//Fehlerwerte, die die Noise--Trader auf die 
    inFile >> var2err;		//Werte der Kovarianzmatrix addieren, der 
    inFile >> coerr;		//Fundamentalisten addieren

    inFile >>  noise;		//
    inFile >>  b1 >> c1;	//Parameter fuer die Dreieicksverteilung,
    inFile >>  b2 >> c2;	//mit welcher die Fehleinschaetzung der
   				//Noise--Trader modelliert wird	(in der Initialisierung wird
				//der untere Rand auf -b1 bzw.-b2 gestetzt)
    
    inFile >>length;	//Iterationslaenge
	
    initialize();  
}

///////////////////////////////////////////////////////////////////////////////
// Funktionsname:	initialize
// Beschreibung:	initialize the model, define the systems initial state
///////////////////////////////////////////////////////////////////////////////
void finanzmarkt_2Dim_1::initialize()
{
//	epsilon=1e-16;
	
	mu=mu0;	        	//Startwert der Gruppenanteile 
	eF=mu*ef;		//aggregierte Grundausstattungen
	eN=(1-mu)*en;

//	beta=beta;		//Gewichtungskonstante fuer Gruppenswitch
//	etafix=etafix;          //Anteil der unbelehrbaren Strategen an der Gesamtzahl Marktteilnehmer
//        etaF=etaF;		//Anteil unbelehrbarer Fundamentalisten
	
	X->m[0][0]= xAll1;	//Erstellen des Vektors der gesamten Wertpapiere
	X->m[1][0]= xAll2;	
	
	XF->m[0][0] = 0;
	XF->m[1][0] = 0;
	XN->m[0][0] = 0;
	XN->m[1][0] = 0;

	p1=p1;			//Erstellen des Startvektors der Wertpapierpreise
	p2=p2;
	P->m[0][0]= p1;
	P->m[1][0]= p2;
	
	p1_old=p1;		//Erstellen des Startvektors der alten Wertpapierpreise
	p2_old=p2;
	P_old->m[0][0]= p1_old;
	P_old->m[1][0]= p2_old;
			

	pF01=p1;		//Erstellen des Startvektors fuer die Preisvorhersage
	pF02=p2;		//der Fundamentalisten
	PF->m[0][0]= pF01;
	PF->m[1][0]= pF02;


	pN01=p1;		//Erstellen des Startvektors fuer die Preisvorhersage
	pN02=p1;		//der Noise--Trader
	PN->m[0][0]= pN01;
	PN->m[1][0]= pN02;

	
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
	zvar3 = new rand_var( "ranf",1,0,1 );	
	if( !(zvar3) )
		fatalError("rand_var::initialize stoch_ar","can't create rand_var");
	zvar4 = new rand_var( "ranf",1,0,1 );	
	if( !(zvar4) )
		fatalError("rand_var::initialize stoch_ar","can't create rand_var");
	
	
	d1=((zetamax1-zetamin1)/2)/(1-gamma1);
	var1=((zetamax1-zetamin1)*(zetamax1-zetamin1))/12; // ist konstant !

	d2=((zetamax2-zetamin2)/2)/(1-gamma2);
	var2=((zetamax2-zetamin2)*(zetamax2-zetamin2))/12; // ist konstant !
	


	VF->m[0][0]= var1;	//Erstellen der Kovarianzmatrix der Fundamentalisten
	VF->m[1][0]= 0;
	VF->m[0][1]= 0;	
	VF->m[1][1]= var2;

	VN->m[0][0]= var1+var1err;	//Erstellen der Kovarianzmatrix der Noise--Trader
	VN->m[1][0]= 0+coerr;
	VN->m[0][1]= 0+coerr;	
	VN->m[1][1]= var2+var2err;
	

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

/////////////////////////////////////////////////////////////////////////////////////
//Berechnung der Konstanten Matrizen A=A, B=(Id-A) und C=((Id-A)*X_F*V_F*X):
//Die Matrizen bleiben konstant, da in dieser Form des Modells die Anteile mu und 
//(1-mu) konstant bleiben.
//Es gilt Xi_N=(alphan/(1-mu)) und Xi_F=(alphaf/mu).
//(die Abfragen finden statt, um nichtdefinierte Werte abzufangen)
////////////////////////////////////////////////////////////////////////////////////
//A:
if((1-mu)<epsilon) {
	A->m[0][0]= 0;		
	A->m[1][0]= 0;
	A->m[0][1]= 0;	
	A->m[1][1]= 0;
	}
else {	
	//A=(Id+(Xi_N/Xi_F)*VN*VF^(-1))^(-1)
	
		point->inverse(VF,Z1);			        //             VF^(-1)
		point->multiply(VN,Z1,wichtig1);	        //          VN*VF^(-1)
		Xi_NzuXi_F=(alphan/(1-mu))*(mu/alphaf);		
		point->multiplyScalar(Xi_NzuXi_F,wichtig1,Z1);  //   (Xi_N//Xi_F)VN*VF^(-1)
		point->add(Id,Z1,Z4);			        //Id+(Xi_N//Xi_F)VN*VF^(-1)	
		point->inverse(Z4,A);
     }
//B:
//B=(Id-A)
	point->subtract(Id,A,B);
//C:
if(mu<epsilon) {
	//C=Xi_N*VN*X
		point->multiply(VN,X,Z6);	
		point->multiplyScalar(alphan,Z6,C);
	   } 
else {   
	//C=Xi_F(B*VF)*X
		point->multiply(B,VF,Z1);
		point->multiply(Z1,X,wichtig2);
		Xi_F=alphaf/mu;
		point->multiplyScalar(Xi_F,wichtig2,C);
      }
}

///////////////////////////////////////////////////////////////////////////////
// Funktionsname:	sendStateSpace
// Beschreibung:	return pointers to the state variables
///////////////////////////////////////////////////////////////////////////////
void finanzmarkt_2Dim_1::sendStateSpace(int &quantity,const qreal*** stateSpace)
{
    if( stateSpace )
	delete stateSpace;
    *stateSpace= new const qreal* [2]; // 2 asset in Model finanzmarkt_2Dim_1
    if( !(*stateSpace) )
	fatalError("finanzmarkt_2Dim_1::sendStateSpace","Speicherfehler");
    quantity=2; // 2 asset in Model finanzmarkt_2Dim_1
   (*stateSpace)[0]=&p1;
   (*stateSpace)[1]=&p2;
};


///////////////////////////////////////////////////////////////////////////////
// Funktionsname:	iteration
// Beschreibung:	Durchlauf einer Periode
///////////////////////////////////////////////////////////////////////////////
void finanzmarkt_2Dim_1::iteration(const long& t)
{
//Neu--Berechnung der Matrizen A,B,C bei neuem Fundamentalisten--Marktanteil (mu)
//durch Gruppenswitch:
//Die Matrizen ,,wichtig1'' und ,,wichtig2'' werden einmal in der Initialisierung berechnet
//und bleiben immer konstant. Daher werden nur die mu--relevanten Rechnungen durchgefuehrt.

if((1-mu)<epsilon) {
	A->m[0][0]= 0;		
	A->m[1][0]= 0;
	A->m[0][1]= 0;	
	A->m[1][1]= 0;
	}
else {	
	//A=(Id+(Xi_N/Xi_F)*VN*VF^(-1))^(-1)
		Xi_NzuXi_F=(alphan/(1-mu))*(mu/alphaf);		
		point->multiplyScalar(Xi_NzuXi_F,wichtig1,Z1);  //   (Xi_N//Xi_F)VN*VF^(-1)
		point->add(Id,Z1,Z4);			        //Id+(Xi_N//Xi_F)VN*VF^(-1)	
		point->inverse(Z4,A);
     }
//B:
//B=(Id-A)
	point->subtract(Id,A,B);
//C:
if(mu<epsilon) {
	//C=Xi_N*VN*X
		point->multiply(VN,X,Z6);	
		point->multiplyScalar(alphan,Z6,C);
	   } 
else {   
	//C=Xi_F(B*VF)*X
		Xi_F=alphaf/mu;
		point->multiplyScalar(Xi_F,wichtig2,C);
      }

//Abspeichern der Werte der Vorperiode,
//fuer spaetere Berechnungen und Auswertungen:


	RHO_old->m[0][0] = RHO->m[0][0]; 
	RHO_old->m[1][0] = RHO->m[1][0];

    //Speichern der Vorhersagen fuer die Berechung der Vorhersagefehler
	pN_old1 = PN->m[0][0];
	pN_old2 = PN->m[1][0];

    Ed_old1 = Ed1;
    Ed_old2 = Ed2;	
	
		
	P_old->m[0][0] = P->m[0][0];
	P_old->m[1][0] = P->m[1][0];
	
	p1_old = P_old->m[0][0];
	p2_old = P_old->m[1][0];		
	
	d1_old = d1;
    d2_old = d2;
		
	XF_old->m[0][0] = XF->m[0][0];
	XF_old->m[1][0] = XF->m[1][0];
	XN_old->m[0][0] = XN->m[0][0];
	XN_old->m[1][0] = XN->m[1][0];


	xF_old1=XF_old->m[0][0];
	xF_old2=XF_old->m[1][0];
	xN_old1=XN_old->m[0][0];
	xN_old2=XN_old->m[1][0];
	
	
	
//Zufallsdividende fuer die laufende Periode:

    Ed11= gamma1*gamma1*d1+(1+gamma1)*((zetamax1-zetamin1)/2);	
	d1=gamma1 * d1 + zvar1->dice();
	delta_d1=d1-Ed1;	           //fuer alternative Renditenberchnung
	Ed1= gamma1 * d1 + ((zetamax1-zetamin1)/2) ;

	
	
    Ed22= gamma2*gamma2*d2+(1+gamma2)*((zetamax2-zetamin2)/2);		
	d2=gamma2 * d2 + zvar2->dice();	
	delta_d2=d2-Ed2;                   //fuer alternative Renditenberchnung
	Ed2= gamma2 * d2 + ((zetamax2-zetamin2)/2) ;

	
	
	D->m[0][0]= d1;
	D->m[1][0]= d2;

	deltaD->m[0][0]= delta_d1;         //fuer alternative Renditenberchnung
	deltaD->m[1][0]= delta_d2;

	ED->m[0][0]= Ed1; //die Erwartungswerte ueber die Dividende werden
	ED->m[1][0]= Ed2; //in jeder Iteration mit t>1 neu berechnet

	ED2->m[0][0]= Ed11;
	ED2->m[1][0]= Ed22;	
	
////////////////////////////////////////////////////////////////////////////////////
//Heutige Fehleinschaetzungen der Noise--Trader ueber die Preise der kommenden Periode 
//folgen jeweils einer Dreiecksverteilung zuwischen a und b und dem Modalwert c
////////////////////////////////////////////////////////////////////////////////////

if(F_c1==0){
	rho1=0;		
	}
else  {
	rho1=zvar3->dice();
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
	rho2=zvar4->dice();
		if(rho2<=F_c2){
			rho2=rho2*(b2-a2)*(c2-a2);
			rho2=a2+sqrt(rho2);
			}
		else{
			rho2=(1-rho2)*(b2-a2)*(b2-c2);
			rho2=b2-sqrt(rho2);	
			}	
	}

	RHO->m[0][0]=rho1; 
	RHO->m[1][0]=rho2; 
/////////////////////////////////////////////////////////////////////////////
//Preisprozess bei homogenen Erwartungen (Stand: 07.03.2001)
//Achtung!!!! Gilt nur wenn im *.sim-File mu0=0 ist!
////////////////////////////////////////////////////////////////////////////
if(mu==0){
//Prognoseregel der ,,Noise--Trader`` (alle sind gleich perfekt oder falsch)

//PN=R*(PN+RHO)-ED+C		      (da mu:=0 ist C:=Xi_N*VN*X)
	if(noise==1){
	point->add(PN,RHO,Z2);
	point->multiplyScalar(R,Z2,Z3);
	point->subtract(Z3,ED2,Z2);
	point->add(Z2,C,PN);
	}
    else{
	PN->m[0][0]= pR1;
	PN->m[1][0]= 1;
	}
		
//Marktraeumender Preis bei homogenen Erwartungen
//P=(PN+ED-C)/R
	
	point->add(PN,ED2,Z2);
	point->subtract(Z2,C,Z3);
	AbZins=1/R;
	point->multiplyScalar(AbZins,Z3,P);

	p1=P->m[0][0];
	p2=P->m[1][0];


}
/////////////////////////////////////////////////////////////////////////////
//Preisprozess bei heterogenen Erwartungen (Wissensstand: 23.03.2001)
//
/////////////////////////////////////////////////////////////////////////////
else{
//Vorhersage der Fundamentalisten:
//PF=(R*PF)-ED+(A*RHO)+C
//
	point->multiplyScalar(R,PF,Z2);
	point->subtract(Z2,ED,Z3);
	point->multiply(A,RHO,Z2);
	point->add(Z3,Z2,Z6);
	point->add(Z6,C,PF);

//Vorhersage der Noise-Trader:
//PN=PF-RHO

	point->subtract(PF,RHO,PN);
	
//Marktraeumender Preis bei heterogenen Erwartungen:
//P=(A*PN+B*PF+ED-C)/R

	point->multiply(A,PN,P);
	point->multiply(B,PF,Z3);
	point->add(P,Z3,Z2);
	point->add(Z2,ED,Z3);
	point->subtract(Z3,C,Z6);
	AbZins=1/R;
	point->multiplyScalar(AbZins,Z6,P);

	
	p1=P->m[0][0];
	p2=P->m[1][0];
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//        Berechung der Vorhersagefehler aus
//	  den Prognosen der Vorperiode und den heutigen Preisen
//	(!!!! nur fuer N--Trader, da die F--Trader immer perfekte ex-dividend Erwartungen liefern
/////////////////////////////////////////////////////////////////////////////////////////////////////

//Prognosefehler fuer Dividenden:

        err_div1 = d1-Ed_old1;
        err_div2 = d2-Ed_old2;	

//ex--dividend--Prognosefehler:

	err_pN1 = p1-pN_old1;
	err_pN2 = p1-pN_old2;

//cum--dividend--Prognosefehler:

	err_qN1 = err_div1-err_pN1;
	err_qN2 = err_div2-err_pN2;

/////////////////////////////////////////////////////////////////////////////////////////////////
//
//			Berechunung von ,,delta_p'' und ,,delta_q''
//
/////////////////////////////////////////////////////////////////////////////////////////////////

//delta_p:

        delta_p1= p1-p1_old;
        delta_p2= p2-p2_old;

//delta_div:
		
		delta_div1=d1-d1_old;
		delta_div2=d2-d2_old;

//delta_q1:

        delta_q1=delta_p1+delta_div1;
        delta_q2=delta_p1+delta_div2;




////////////////////////////////////////////////////////////////////////////////
//Errechung der Portfolios anhand der Funktionen der aggregierten Nachfragen
////////////////////////////////////////////////////////////////////////////////

//Wertpapierbestand der Fundamentalisten (aggregiert und einzeln)

if(mu<epsilon) {
	XF->m[0][0]=0;
	XF->m[1][0]=0;
	
	xF1=XF->m[0][0];
	xF2=XF->m[1][0];
	xf1=0;
	xf1=0;
	xF11=xF1/100;		//andere Skalierung fuer die graphische
	xF22=xF2/100;		//Ausgabe (sonst bekomme ich Probleme mit
				//der Bildaufloesung	


	}
else {	
	//XF=(Xi_F*VF)^(-1)*[PF+ED-R*P]
	Xi_F=alphaf/mu;
	point->multiplyScalar(R,P,Z2);  	//      R*P
	point->subtract(ED,Z2,Z3);		//   ED-R*P
	point->add(PF,Z3,Z2);			//PF+ED-R*P
	point->multiplyScalar(Xi_F,VF,Z1);	// Xi_F*VF
	point->inverse(Z1,Z4);			//(Xi_F*VF)^(-1)
	point->multiply(Z4,Z2,XF);		//(Xi_F*VF)^(-1)*[PF+ED-R*P]
	
	xF1=XF->m[0][0];
	xF2=XF->m[1][0];
	xf1=xF1/mu;
	xf2=xF2/mu;
	
	xF11=xF1/100;		//andere Skalierung fuer die graphische
	xF22=xF2/100;		//Ausgabe (sonst bekomme ich Probleme mit
				//der Bildaufloesung	

}

//Werpapierbestand der Noise--Trader (aggregiert und einzeln)

if((1-mu)<epsilon) {
	XN->m[0][0]=0;
	XN->m[1][0]=0;
	
	xN1=XN->m[0][0];
	xN2=XN->m[1][0];	
	xn1=0;
	xn2=0;
	xN11=xN1/100;		//andere Skalierung fuer die graphische
	xN22=xN2/100;		//Ausgabe (sonst bekomme ich Probleme mit
				//der Bildaufloesung	


	}
else {	//XN=(Xi_N*VN)^(-1)*[PN+ED-R*P]
	Xi_N=alphan/(1-mu);
	point->multiplyScalar(R,P,Z2);		//      R*P
	point->subtract(ED2,Z2,Z3);		//   ED-R*P
	point->add(PN,Z3,Z2);			//PN+ED-R*P
	point->multiplyScalar(Xi_N,VN,Z1);	// Xi_N*VN
	point->inverse(Z1,Z4);			//(Xi_N*VN)^(-1)
	point->multiply(Z4,Z2,XN);		//(Xi_N*VN)^(-1)*[PF+ED-R*P]
	
	xN1=XN->m[0][0];
//	xN2=XN->m[1][0];
	xN2=0;	
	xn1=xN1/(1-mu);
	xn2=xN2/(1-mu);
	xN11=xN1/100;		//andere Skalierung fuer die graphische
	xN22=xN2/100;		//Ausgabe (sonst bekomme ich Probleme mit
				//der Bildaufloesung	

}

///////////////////////////////////////////////////////////////////////////
//Habenstand:  Anfangsaustattung minus Kosten der Risikoanlage
//		Haben=e-px
///////////////////////////////////////////////////////////////////////////

//Fundamentalisten
if(mu<epsilon) {
	Haben_F=0;
	Haben_f=0;
	}
else {
	point->multiplyATB(P,XF,PXF);
	pxF=PXF->m[0][0];
	Haben_F=eF-pxF;
	Haben_f=Haben_F/mu;
	}

//Noise-Trader
if((1-mu)<epsilon) {
	Haben_N=0;
	Haben_n=0;
	}
else {
	point->multiplyATB(P,XN,PXN);
	pxN=PXN->m[0][0];
	Haben_N=eN-pxN;
	Haben_n=Haben_N/(1-mu);
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
	point->subtract(ED,P,Z2);		// ED-P
	point->add(PF,Z2,Z3);			// PF+ED-P
	point->multiplyATB(Z3,XF,Z7);		//(PF+ED-P)*XF
	point->multiplyATB(P,XF,Z8);		// P*XF
	z7=Z7->m[0][0];
	z8=Z8->m[0][0];
	ErendF_p=z7/z8;				//(PF+ED-P)*XF/P*XF
}

//Erwartete Rendite des Gesamtportfolios (Fundamentalisten)
if(mu<epsilon) {
	ErendF_ges=0;
	}
else {	//((PF+ED-R*P)*XF/mu*ef)+(R-1)
	point->multiplyScalar(R,P,Z2);  	//       R*P
	point->subtract(ED,Z2,Z3);		//    ED-R*P
	point->add(PF,Z3,Z2);			// PF+ED-R*P
	point->multiplyATB(Z2,XF,Z7);		//(PF+ED-R*P)*XF
	z7=Z7->m[0][0];
	z8=z7/eF;				//(PF+ED-R*P)*XF/mu*ef)
	ErendF_ges=z8+(R-1);			//((PF+ED-R*P)*XF/mu*ef)+(R-1)
}


//Erwartete Rendite des Portfolios aus Aktien (Noise--Trader)

if((1-mu)<epsilon) {
	ErendN_p=0;
	}
else {	//(PN+ED-P)*XN)/(P*XN)
	point->subtract(ED2,P,Z2);		// ED-P
	point->add(PN,Z2,Z3);			// PF+ED-P
	point->multiplyATB(Z3,XN,Z7);		//(PF+ED-P)*XF
	point->multiplyATB(P,XN,Z8);		// P*XF
	z7=Z7->m[0][0];
	z8=Z8->m[0][0];
	ErendN_p=z7/z8;				//(PF+ED-P)*XF/P*XF
}

//Erwartete Rendite des Gesamtportfolios (Noise--Trader)
if((1-mu)<epsilon) {
	ErendN_ges=0;
	}
else {	//((PN+ED-R*P)*XN/((1-mu)*en))+(R-1)
	point->multiplyScalar(R,P,Z2);  	//       R*P
	point->subtract(ED2,Z2,Z3);		//    ED-R*P
	point->add(PN,Z3,Z2);			// PF+ED-R*P
	point->multiplyATB(Z2,XN,Z7);		//(PF+ED-R*P)*XF
	z7=Z7->m[0][0];
	z8=z7/eN;				//(PF+ED-R*P)*XF/mu*ef)
	ErendN_ges=z8+(R-1);			//((PF+ED-R*P)*XF/mu*ef)+(R-1)
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
	
        point->multiplyScalar(R,P,Z2);          //       R*P
	point->subtract(ED,Z2,Z3);              //    ED-R*P
	point->add(PF,Z3,Z2);                   // PF+ED-R*P
	point->multiplyATB(Z2,XF,Z7);           //(PF+ED-R*P)*XF
	z7=Z7->m[0][0];
	RPF=z7/eF;                               //(PF+ED-R*P)*XF/mu*ef)

        //Bedingte Varianz des Fundamentalisten--Rendite:
        //1/e^F (sqrt ((x^F_t)*(V^F)*(x^F_t) ) )
	point->multiply(VF,XF,Z2);               //                (V^F)*(x^F_t)
	point->multiplyATB(XF,Z2,Z7);            //        (x^F_t)*(V^F)*(x^F_t)
	z7=Z7->m[0][0];
	z8=sqrt(z7);                             //sqrt ((x^F_t)*(V^F)*(x^F_t) ) 	
	sdRF=z8/eF;

        //Effizienzliniensteigung der Fundamentalisten:

	Eff_F=RPF/sdRF;
}


//Noise--Traders:


if((1-mu)<epsilon) {
	RPN=0;
	sdRN=0;
	Eff_N=0;
	}
else {  //Erwartete Risikopraemie der Noise--Traders:
        //1/e^N((p^F_t+Ed+R*p)*x^N_t)
    point->multiplyScalar(R,P,Z2);          //       R*P
	point->subtract(ED2,Z2,Z3);              //    ED-R*P
	point->add(PF,Z3,Z2);                   // PF+ED-R*P
	point->multiplyATB(Z2,XN,Z7);           //(PF+ED-R*P)*XN
	z7=Z7->m[0][0];
	RPN=z7/eN;                               //(PF+ED-R*P)*XN/(1-mu)*en)

        //Bedingte Varianz des Noise--Traders--Rendite:
        //1/e^N (sqrt ((x^N_t)*(V^F)*(x^N_t) ) )
	point->multiply(VF,XN,Z2);               //                (V^F)*(x^N_t)
	point->multiplyATB(XN,Z2,Z7);            //        (x^N_t)*(V^F)*(x^N_t)
	z7=Z7->m[0][0];
	z8=sqrt(z7);                             //sqrt ((x^N_t)*(V^F)*(x^N_t) ) 	
	sdRN=z8/eN;

        //Effizienzliniensteigung der Noise--Traders:

	Eff_N=RPN/sdRN;
}

     delta_Eff=Eff_F-Eff_N;



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
	point->multiplyScalar(R,P_old,Z2);	//     R*P_old
	point->subtract(D,Z2,Z3);		//   D-R*P_old
	point->add(P,Z3,Z2);			// P+D-R*P_old
	point->multiplyATB(Z2,XF_old,Return_F);	//(P+D-R*P_old)^(T)*XF_old

	return_F=Return_F->m[0][0];
	return_f=return_F/mu;
	rend_F=(return_F/eF)+(R-1);
	rend_f=(return_f/ef)+(R-1);
	}

//Renditen der "alten" Noise--Trader
if((1-mu)<epsilon) {
	return_N=0;
	return_n=0;
	rend_N=0;
	rend_n=0;
	}
else {	//return_N=(P+D-R*P_old)^(T)*XN_old	//Z2 kommt von oben
	point->multiplyScalar(R,P_old,Z2);	//     R*P_old
	point->subtract(D,Z2,Z3);		//   D-R*P_old
	point->add(P,Z3,Z2);			// P+D-R*P_old
	point->multiplyATB(Z2,XN_old,Return_N);	//(P+D-R*P_old)^(T)*XN_old

	return_N=Return_N->m[0][0];
	return_n=return_N/(1-mu);
	rend_N=(return_N/eN)+(R-1);
	rend_n=(return_n/en)+(R-1);
	}
///////////////////////////////////////////////////////////////////////////////
// Alternative Renditenberechnung, um Renditen zu ueberprufen
//
///////////////////////////////////////////////////////////////////////////////	
//rendf_neu, rendn_neu;

//Fundamentalisten:

if(mu<epsilon) {
	rendf_neu=0;
	}
else { //r+(1/alpha*ef)(deltaD+alpha*VF*X+(1-mu)*Rho_old)VF^{-1}(\alpha*VF*X+(1-mu)*Rho_old)
	muN=(1-mu);
	z7=alphaf*ef;
	z8=1/z7;

	point->multiplyScalar(muN,RHO_old,Z2);	        //                  (1-mu)*Rho_old
        point->multiply(VF,X,Z3);                       //             VF*X	
        point->multiplyScalar(alphaf,Z3,Z6);            //       alpha*VF*X
	point->add(Z2,Z6,Z3);                           //       alpha*VF*X+(1-mu)*Rho_old
	point->add(deltaD,Z3,Z2);                       //deltaD+alpha*VF*X+(1-mu)*Rho_old	
	
	point->inverse(VF,Z1);   			//(VF)^(-1)	

//jetzt: (R-1)z8*(Z2*Z1*Z3)
   
        point->multiply(Z1,Z3,Z6);
	point->multiplyATB(Z2,Z6,Z7);
	z7=Z7->m[0][0];
	rendf_neu=z8*z7;	
	rendf_neu=(R-1)+rendf_neu;
}



//Noise--Traders:

if((1-mu)<epsilon) {
	rendn_neu=0;
	}
else { //r+(1/alpha*en)(deltaD+\alpha*VF*X+(1-mu)*Rho_old)VF^{-1}(\alpha*VF*X-mu*Rho_old)

	muN=(1-mu);
	z7=alphan*en;
	z8=1/z7;

	point->multiplyScalar(muN,RHO_old,Z2);	        //                  (1-mu)*Rho_old
    point->multiply(VF,X,Z3);                       //             VF*X	
    point->multiplyScalar(alphaf,Z3,Z6);            //       alpha*VF*X
	point->add(Z2,Z6,Z3);                           //       alpha*VF*X+(1-mu)*Rho_old
	point->add(deltaD,Z3,Z2);                       //deltaD+alpha*VF*X+(1-mu)*Rho_old	
	
	point->inverse(VF,Z1);   			//(VF)^(-1)	
//Z2 und Z1 sind heilig!


	point->multiplyScalar(mu,RHO_old,Z9);           //           mu*Rho_old
        point->multiply(VF,X,Z3);                       //      VF*X	
        point->multiplyScalar(alphaf,Z3,Z6);            //alpha*VF*X
	point->subtract(Z6,Z9,Z3);			//alpha*VF*X-mu*Rho_old

        point->multiply(Z1,Z3,Z9);                      //VF^{-1}*(alpha*VF*X-mu*Rho_old)
	point->multiplyATB(Z2,Z9,Z7);			//Z2=(deltaD+\alpha*VF*X+(1-mu)*Rho_old); Z9=VF^{-1}(\alpha*VF*X-mu*Rho_old)
	z7=Z7->m[0][0];
	rendn_neu=z8*z7;	
	rendn_neu=(R-1)+rendn_neu;
}

///////////////////////////////////////////////////////////////////////////////
// Rekursive Ermittulung der Renditenmittelwerte, Standardabweichungen
// und Sharpe-Inidzes
///////////////////////////////////////////////////////////////////////////////	
//Renditemittelwert der Fundamentalisten
	mRF=t*mRF;
	mRF= rendf_neu+mRF;
	mRF= mRF/(1+t);
//Standardabweichung der Fundamentalistenrenditen
	z3=rendf_neu-mRF;
	z3=z3*z3;
	z3=z3/(1+t);
	
	vRF=sRF*sRF;
	z4=t*vRF;
	z4=z4/(1+t);

	z5=z3+z4;

	sRF=sqrt(z5);
//Sharpe-Index der Fundamentalisten
if(sRF>0){
	SIF=mRF-(R-1);
	SIF=SIF/sRF;
}

//Renditemittelwert der Noise--Traders
	mRN=t*mRN;
	mRN= rendn_neu+mRN;
	mRN= mRN/(1+t);
//Standardabweichung der Noise--Traders
	z3=rendn_neu-mRN;
	z3=z3*z3;
	z3=z3/(1+t);
	
	vRN=sRN*sRN;
	z4=t*vRN;
	z4=z4/(1+t);

	z5=z3+z4;

	sRN=sqrt(z5);
//Sharpe-Index der Noise--Traders
if(sRN>0){
	SIN=mRN-(R-1);
	SIN=SIN/sRN;
}

//Differenz der Sharpe--Inizes
	delta_SI=SIF-SIN;

////////////////////////////////////////////////////////////////////
//Neu--Berechung der Gruppenanteile bei Gruppenswitch
////////////////////////////////////////////////////////////////////

//Errechnung von mu mithilfe des Sharpe--Index und der ,,fitness Function''
//
//mu=[exp{beta*SIF}/(exp{beta*SIF}+exp{beta*SIN}]*(1-etafix)
//  =zwischen1/zwischen3


//zwischen1=beta*SIF;
//zwischen2=beta*SIN;

//zwischen1=exp(zwischen1);
//zwischen2=exp(zwischen2);

//zwischen3=zwischen1+zwischen2;

//wechselanteil=zwischen1/zwischen3;
//wechselanteil=wechselanteil*(1-etafix);

//if(wechselanteil>0){
//mu=wechselanteil+etaF;
//}

//letzte schliessende Klammer der Iteration:
}
