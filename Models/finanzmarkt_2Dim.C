///////////////////////////////////////////////////////////////////////////
//																		 //
// Name: finanzmarkt_2Dim.C
//       lineares Finanzmarktmodell mit Erwartungsr"uckkoppelung	
//		 sowie zwei heterogenen Händlergruppen
//	 K=2
//
// Code: Andreas Tonn 
//
///////////////////////////////////////////////////////////////////////////////

#include "../error.h"
#include "finanzmarkt_2Dim.h"

///////////////////////////////////////////////////////////////////////////////
// Funktionsname:	finanzmarkt_2Dim
// Beschreibung:	constructor
///////////////////////////////////////////////////////////////////////////////
finanzmarkt_2Dim::finanzmarkt_2Dim() : baseModel(2) {

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
// Funktionsname:	finanzmarkt_2Dim
// Beschreibung:	destructor
///////////////////////////////////////////////////////////////////////////////
finanzmarkt_2Dim::~finanzmarkt_2Dim() {

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
qreal* finanzmarkt_2Dim::setLabels(char* label)
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
	if( !strcmp(label,"return_f") ) return(&return_f);
	if( !strcmp(label,"return_n") ) return(&return_n);
	if( !strcmp(label,"cf") ) return(&cf);
	if( !strcmp(label,"cn") ) return(&cn);
	if( !strcmp(label,"rend_F") ) return(&rend_F);
	if( !strcmp(label,"rend_N") ) return(&rend_N);
	if( !strcmp(label,"rend_diff") ) return(&rend_diff);		
	if( !strcmp(label,"quote_F1") ) return(&quote_F1);
	if( !strcmp(label,"quote_F2") ) return(&quote_F2);
	if( !strcmp(label,"quote_N1") ) return(&quote_N1);
	if( !strcmp(label,"quote_N2") ) return(&quote_N2);
	if( !strcmp(label,"quote_f1") ) return(&quote_f1);
	if( !strcmp(label,"quote_f2") ) return(&quote_f2);
	if( !strcmp(label,"quote_n1") ) return(&quote_n1);
	if( !strcmp(label,"quote_n2") ) return(&quote_n2);
	if( !strcmp(label,"quote_rF") ) return(&quote_rF);
	if( !strcmp(label,"quote_rN") ) return(&quote_rN);	

	
  return NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Funktionsname:	initialize
// Beschreibung:	initialize the model, define the systems initial state
///////////////////////////////////////////////////////////////////////////////
void finanzmarkt_2Dim::initialize()
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


	aF=F/alphaf;		//aggregierte Risikoaversion aller Fundamentalisten
	aN=N/alphan;		//aggregierte Risikoaversion aller Noise--Trader

	eF=F*ef;		//aggregierte Grundausstattung aller Fundamentalisten
	eN=N*en;		//aggregierte Grundausstattung aller Noise--Trader


	X->m[0][0]= xAll1;	//Erstellen des Vektors der gesamten Wertpapiere
	X->m[1][0]= xAll2;	
	
	XF_old->m[0][0] = 0;
	XF_old->m[1][0] = 0;
	XN_old->m[0][0] = 0;
	XN_old->m[1][0] = 0;

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
	VF->m[1][0]= -0.5;
	VF->m[0][1]= -0.5;	
	VF->m[1][1]= var2;

	VN->m[0][0]= var1;	//Erstellen der Kovarianzmatrix der Noise--Trader
	VN->m[1][0]= -0.5;
	VN->m[0][1]= -0.5;	
	VN->m[1][1]= var2;


	D->m[0][0]= d1;
	D->m[1][0]= d2;
	
	ED->m[0][0]= Ed1;
	ED->m[1][0]= Ed2;
	
	RHO->m[0][0]= rho1;
	RHO->m[1][0]= rho2;

//Errechnung der Matrizen A,B,C
	
//A=(Id+(aF/aN)*VN*VF^(-1))^(-1)
	
	point->inverse(VF,Z1);			//             VF^(-1)
	point->multiply(VN,Z1,Z4);		//          VN*VF^(-1)
	aFzuaN=aF/aN;
	point->multiplyScalar(aFzuaN,Z4,Z1);	//   (aF/aN)VN*VF^(-1)
	point->add(Id,Z1,Z4);			//Id+(aF/aN)VN*VF^(-1)	
	point->inverse(Z4,A);

//for(int j=0;j<=1;j++)	
//{		
//	log() << "A" << j << 0 << "=" << A->m[j][0] << "\n";
//	log() << "A" << j << 1 << "=" << A->m[j][1] << "\n";
//}

//B=Id-A
	point->subtract(Id,A,B);

//C=aF^(-1)(B*VF)*X
	point->multiply(B,VF,Z1);
	z1uaF=1/aF;
	point->multiplyScalar(z1uaF,Z1,Z4);
	point->multiply(Z4,X,C);
	
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
void finanzmarkt_2Dim::loadParamset(ifstream& inFile)
{
    inFile >> R;  	//risikoloser Zins (1+r)>0
    inFile >> F;	//Anzahl Fundamentalisten
    inFile >> N;	//Anzahl Noise--Trader
    inFile >> alphaf;	//Risikoaversionsparameter eines Fundamentalisten 
    inFile >> alphan;	//Risikoaversionsparameter eines Noise--Trader
    inFile >> ef;	//Grundausstattung eines Fundamentalisten
    inFile >> en;	//Grundausstattung eines Noise--Trader
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
void finanzmarkt_2Dim::sendStateSpace(int &quantity,const qreal*** stateSpace)
{
    if( stateSpace )
	delete stateSpace;
    *stateSpace= new const qreal* [2]; // 2 asset in Model finanzmarkt_2Dim
    if( !(*stateSpace) )
	fatalError("finanzmarkt_2Dim::sendStateSpace","Speicherfehler");
    quantity=2; // 2 asset in Model finanzmarkt_2Dim
   (*stateSpace)[0]=&p1;
   (*stateSpace)[1]=&p2;
};

///////////////////////////////////////////////////////////////////////////////
// Funktionsname:	iteration
// Beschreibung:	Durchlauf einer Periode
///////////////////////////////////////////////////////////////////////////////
void finanzmarkt_2Dim::iteration(const long& t)
{

if(t>1)	{ //in Periode 1 werden die Startwerte genommen


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
	
if(rho1<=0.75){
		rho1=rho1*(1+1)*(0.5+1);
		rho1=-1+sqrt(rho1);
	}
else{
	rho1=(1-rho1)*(1+1)*(1-0.5);
	rho1=1-sqrt(rho1);	
	}	

	rho2=zvar4->dice();

if(rho2<=0.75){
		rho2=rho2*(1+1)*(0.5+1);
		rho2=-1+sqrt(rho2);
	}
else{
	rho2=(1-rho2)*(1+1)*(1-0.5);
	rho2=1-sqrt(rho2);	
	}	


//	rho2=(zvar3->dice()+zvar4->dice())/2;

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

}
//Errechung der Portfolios:
//Wertpapierbestand der Fundamentalisten
//XF=aF*VF^(-1)*[PF+ED-R*P]

	point->multiplyScalar(R,P,Z2);  //      R*P
	point->subtract(ED,Z2,Z3);	//   ED-R*P
	point->add(PF,Z3,Z2);		//PF+ED-R*P
	point->inverse(VF,Z4);		//   VF^(-1)
	point->multiply(Z4,Z2,Z3);	//VF^(-1)*[PF+ED-R*P]
	point->multiplyScalar(aF,Z3,XF);//aF*VF^(-1)*[PF+ED-R*P]
	
	xF1=XF->m[0][0];
	xF2=XF->m[1][0];
	xf1=xF1/F;
	xf2=xF2/F;

//Werpapierbestand der Noise--Trader
//XN=aN*VN^(-1)*[PN+ED-R*P]

	point->multiplyScalar(R,P,Z2);	//      R*P
	point->subtract(ED,Z2,Z3);	//   ED-R*P
	point->add(PN,Z3,Z2);		//PN+ED-R*P
	point->inverse(VN,Z4);		//   VN^(-1)
	point->multiply(Z4,Z2,Z3);	//   VN^(-1)*[PN+ED-R*P]
	point->multiplyScalar(aN,Z3,XN);//aN*VN^(-1)*[PN+ED-R*P]

	xN1=XN->m[0][0];
	xN2=XN->m[1][0];
	xn1=xN1/N;
	xn2=xN2/N;


/////////Auswertung://////////////////

//Anteilsquoten:

	quote_F1=xF1/xAll1;
	quote_F2=xF2/xAll2;
	
	quote_f1=xf1/(xf1+xn1);
	quote_f2=xf2/(xf2+xn2);
	
	quote_N1=xN1/xAll1;
	quote_N2=xN2/xAll2;

	quote_n1=xn1/(xf1+xn1);
	quote_n2=xn2/(xf2+xn2);

//Preise

	p1=P->m[0][0];
	p2=P->m[1][0];


//qrealisierter Return der "alten" Fundamentalisten (aggregiert und einzeln)

//	return_F=(P+D-R*P_old)^(T)*XF_old	

	point->multiplyScalar(R,P_old,Z2);	//     R*P_old
	point->subtract(D,Z2,Z3);		//   D-R*P_old
	point->add(P,Z3,Z2);			// P+D-R*P_old
	point->multiplyATB(Z2,XF_old,Return_F);	//(P+D-R*P_old)^(T)*XF_old

	return_F=Return_F->m[0][0];
	return_f=return_F/F;

//qrealisierter Konsum der "alten" Fundamentalisten (aggregiert und einzeln)

	cF=(R*eF)+return_F;			//cF=R*eF+(P+D-R*P_old)^(T)*XF_old
	cf=cF/F;

//Rendite der Fundamentalisten	

	rend_F=(cF-eF)/eF;

//qrealisierter Return der "alten" Noise--Trader (aggregiert und einzeln)

//	return_N=(P+D-R*P_old)^(T)*XN_old	

	point->multiplyScalar(R,P_old,Z2);	//     R*P_old
	point->subtract(D,Z2,Z3);		//   D-R*P_old
	point->add(P,Z3,Z2);			// P+D-R*P_old
	point->multiplyATB(Z2,XN_old,Return_N);	//(P+D-R*P_old)^(T)*XF_old

	return_N=Return_N->m[0][0];
	return_n=return_N/N;

//qrealisierter Konsum der "alten" Noise--Trader (aggregiert und einzeln)

	cN=(R*eN)+return_N;				//cN=R*eN+(P+D-R*P_old)^(T)*XN_old
	cn=cN/N;

//Rendite der Noise--Trader

	rend_N=(cN-eN)/eN;

//RenditenDifferenz:

	rend_diff=rend_F-rend_N;
	quote_rF=rend_F/(rend_N+rend_F);
	quote_rN=rend_N/(rend_N+rend_F);

}









