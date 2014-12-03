///////////////////////////////////////////////////////////////////////////////
//
// Name: linFimaErwRueck.C
//       lineares Finanzmarktmodell mit Erwartungsr"uckkoppelung
//	 K=1
//
// Code: Marc Mueller 
//
///////////////////////////////////////////////////////////////////////////////

#include "../error.h"
#include "linFimaErwRueck.h"

///////////////////////////////////////////////////////////////////////////////
// Funktionsname:	linFimaErwRueck
// Beschreibung:	constructor
///////////////////////////////////////////////////////////////////////////////
linFimaErwRueck::linFimaErwRueck() : baseModel(1) {
}

///////////////////////////////////////////////////////////////////////////////
// Funktionsname:	linFimaErwRueck
// Beschreibung:	destructor
///////////////////////////////////////////////////////////////////////////////
linFimaErwRueck::~linFimaErwRueck() {
}

///////////////////////////////////////////////////////////////////////////////
// Funktionsname:	setLabels
// Beschreibung:	return a pointer to a variable or a parameter specified
///////////////////////////////////////////////////////////////////////////////
qreal* linFimaErwRueck::setLabels(char* label)
{
//	if( !strcmp(label,"intVarName") ) return( (qreal*)(&intVarName) );
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
	if( !strcmp(label,"d") ) return(&d);

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


  return NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Funktionsname:	initialize
// Beschreibung:	initialize the model, define the systems initial state
///////////////////////////////////////////////////////////////////////////////
void linFimaErwRueck::initialize()
{
	p=p0;
	pOld=p0;
	pPerf=p0;
	pPerfOld=p0;

	pF0=p0;
	pF=pF0;
	pFShift=pF0;
	pFOld=pF0;
	pFperf=pF0;

	pC=pC0;
	pCShift=pC0;
	pCOld=pC0;
	pCperf=pC0;

	VF=Vd;
	VC=vc*Vd;

	errvar=0;
	dOld=d;
	divvar=0;

	// sp"ater gebrauchte Konstanten
	// Annahme: VC,VF zeitinvariant
	if(rC==0)
		r=0; // !!!
	 else	r=1/( 1 + ( (rF/VF) * (VC/rC) ) );
	a1VFx=(1/rF)*VF*xAll;
	b=xAll/((rC/VC)+(rF/VF));
//	b=((1-r)*VF*xAll)/rF;     // Problem bei rF=0 !!!

log() <<"Ed=" << Ed <<"Vd=" << Vd <<"d=" << d <<endl;
log() <<"r=" << r << endl;
log() <<"b=" << b << endl;
qreal beta=1.03;qreal L=2;
printf("THETA=%10.10f %10.10f %10.10f %10.10f %10.10f \n",
	(beta/(L*R))*rC/(rC+rF), (beta/(L*R))*rC/(rC+rF), ((1-r)/R), 1/R, -b/R );

}

///////////////////////////////////////////////////////////////////////////////
// Class name:		linFimaErwRueck
// Funktionsname:	loadParamset
// Beschreibung:	Einlesen der Parameter aus der .sim Datei 
///////////////////////////////////////////////////////////////////////////////
void linFimaErwRueck::getString(ifstream& inFile,char* dummy) {
	inFile >> dummy;
	while( !strcmp(dummy,"/*") ) {
		while(strcmp(dummy,"*/"))
			inFile >> dummy;
		inFile >> dummy;	
	}
}
qreal linFimaErwRueck::getqreal(ifstream& inFile) {
	char dummy[1024];
	inFile >> dummy;
	while( !strcmp(dummy,"/*") ) {
		while(strcmp(dummy,"*/"))
			inFile >> dummy;
	inFile >> dummy;	
	}
	return (atof(dummy));
}
int linFimaErwRueck::getInt(ifstream& inFile) {
	char dummy[1024];
	inFile >> dummy;
	while( !strcmp(dummy,"/*") ) {
		while(strcmp(dummy,"*/"))
			inFile >> dummy;
	inFile >> dummy;	
	}
	return (atoi(dummy));
}

void linFimaErwRueck::loadParamset(ifstream& inFile) {
	char dummy[1024];

	getString(inFile,dummy); // lese Versionsnummer
	if( strcmp(dummy,"V1.4") )
		fatalError("loadParamset","wrong version");
	
	getString(inFile,dummy);
	if( !strcmp(dummy,"PrognoseFundamentalisten=perfekt") )
		perfectPredictor=1; // Ja
	 else	perfectPredictor=0; // Nein

	getString(inFile,dummy);
	if( !strcmp(dummy,"Dividenden=bekannt") )
		DividendenErwartung=bekannt;
	 else	DividendenErwartung=unbekannt;

	vc=getqreal(inFile); // Skalierung der erw. Div.Varianz der C

	R=getqreal(inFile);

	rF=getqreal(inFile); // Risikotoleranz
	rC=getqreal(inFile);

	eF=getqreal(inFile); // Anfangsausstattung
	eC=getqreal(inFile);

	xAll=getqreal(inFile);

	p0=getqreal(inFile);
	pF0=p0;
	pC0=p0;	

	length=getInt(inFile);

	initialize();
}

///////////////////////////////////////////////////////////////////////////////
// Funktionsname: saveParamsetWithNames
// Beschreibung:         add  parameterset to printfile
///////////////////////////////////////////////////////////////////////////////
void linFimaErwRueck::saveParamsetWithNames(ofstream& outputFile)
{
    outputFile << "\nModel linFimaErwRueck\n";
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
void linFimaErwRueck::sendStateSpace(int &quantity,const qreal*** stateSpace)
{
    if( stateSpace )
	delete stateSpace;
    *stateSpace= new const qreal* [1]; // 1 asset in Model linFimaErwRueck
    if( !(*stateSpace) )
	fatalError("linFimaErwRueck::sendStateSpace","Speicherfehler");
    quantity=1; // 1 asset in Model linFimaErwRueck
   (*stateSpace)[0]=&p;
};

///////////////////////////////////////////////////////////////////////////////
// Funktionsname:	iteration
// Beschreibung:	Durchlauf einer Periode
///////////////////////////////////////////////////////////////////////////////
void linFimaErwRueck::iteration(const long& t)
{
// Die Prognosen der Chartisten und Fundamentalisten ist extern vorgeschaltet !

qreal beta=1.03;qreal L=2;
//if(t>1)
// { pC=(beta/L)*(p+pOld);
//	  pCperf=(beta/L)*(pPerf+pPerfOld); 
//	 }
/*
printf("p=%6.6f ",p);
printf("pPerf=%6.6f \n",pPerf);
printf("pOld=%6.6f ",pOld);
printf("pPerfOld=%6.6f\n ",pPerfOld);
printf("pC=%6.6f ",pC);
printf("pCperf=%6.6f \n",pCperf);
*/

// lokale Variablen
//	int i,j,jj;
	qreal pOld2,pPerfOld2;

// Bestimmung von r,b,a^{-1}VFxAll
	// r ist konstant
	// b ist konstant
	// a1VFx ist konstant

// Perfekte Prognose
	if(t>1)
		{ // in Periode 1 werden die Startwerte genommen
		if(perfectPredictor) // wenn Fundis perfekte Prognosen machen sollen
			pF=((R*pF-r*pC-Ed)/(1-r))+(1/rF)*VF*xAll;
			// falls Fundis extern vorgeschaltet sind, dann wird
			// ihre Prognose mit der perfekten Prognose "uberschrieben
		pFperf=((R*pFperf-r*pCperf-Ed)/(1-r))+(1/rF)*VF*xAll; // zum Vergleich !
		 // kann in der gleichen Zeitreihe mit p dargestellt werden
		 // qCperf muss dazu zus"atzlich vorgeschaltet werden und in
		 // der gleichen Weise wie qC gebildet werden !!!
//printf("pF=%6.6f ",pF);
//printf("pFperf=%6.6f \n",pFperf);
	}

// Dynamik
	pPerfOld2=pPerfOld;
	pPerfOld=pPerf;

	pOld2=pOld;
	pOld=p;
	xFold=xF;
	xCold=xC;
	pFOld=pFShift; // da die Prognosen vorgeschaltet sind 
	pFShift=pF; // kann nicht direkt pFOld=pF gesetzt werden
	pCOld=pCShift;
	pCShift=pC;

// Preisprozess
	if(DividendenErwartung==bekannt) // bei der Preisbildung
		de=d;
	 else	de=Ed;

	// r,b siehe oben
	p=(r*pC+(1-r)*pF+de-b)/R; 
	pPerf=(r*pCperf+(1-r)*pFperf+de-b)/R; // nur zum Vergleich !!!

//	if(p<0) log() <<"periode="<<t<<" p="<<p<<" !"<<endl;

/*
printf("Modell: p=%6.6f ",p);
printf("pOld=%6.6f ",pOld);
//printf("pOld2=%6.6f ",pOld2);
//printf("pF=%6.6f ",pF);
printf("pC=%6.6f (!!!pC=%6.6f) ",pC,(beta/L)*(pOld+pOld2));
//printf("de=%6.6f \n",de);
//printf("\t!!!p=%6.6f ", (r/R)*(beta/L)*pOld+ (r/R)*(beta/L)*pOld2 +((1-r)/R)*pF +  de/R - b/R );
//printf("DIFF=%6.6f\n",  p - ( (r/R)*(beta/L)*pOld+ (r/R)*(beta/L)*pOld2 +((1-r)/R)*pF +  de/R - b/R )  );
printf("\tpPerf=%6.6f ",pPerf);
//printf("pFperf=%6.6f ",pFperf);
printf("pCperf=%6.6f \n",pCperf);
*/


// Portfolio
	xF=rF*(pF+Ed-R*p)/Vd; // tatsaechliches Portfolio der Jungen (varianzminimal)
	xC=rC*(pC+Ed-R*p)/Vd;
	xFrel=xF/xAll;	// rel. Portfolio bezug zur Aktienanzahl
	xCrel=xC/xAll;

// Auswertung
	wF=eF+(p+d-R*pOld)*xFold; // tatsaechliches Vermoegen der Alten
	wC=eC+(p+d-R*pOld)*xCold;

	RF=(wF-eF)/eF; // Rendite der Alten
	RC=(wC-eC)/eC;
	RpF=RF-(R-1); // tatsaechliche Risikopraemie der Alten
	RpC=RC-(R-1);

	ferrF=(p-pFOld)/p;
	ferrC=(p-pCOld)/p;

	deltap=p-pOld;

	ertrag=(deltap+d)/p;

	deltapF = pF-pFOld;

	errvar=(errvar*(t-1)+((p+d-pFOld-Ed)*(p+d-pFOld-Ed)))/t;// Schaetzer Fehlervarianz

	divvar=(divvar*(t-1)+((d-dOld)*(d-dOld)))/t;// Schaetzer Dividendenvarianz
	dOld=d;

	pDiff=(pPerf-p)/p;


//log() << t << " pCperf=" << pCperf << " pC=" << pC ;//<< endl;
//log() << " p=" << p << " pPerf=" << pPerf << endl;
//log() << " errvar=" << errvar  << endl;
//log() << "d=" << d << " Ed=" << Ed << " de=" << de << endl; 

//log() << " p=" << p << " pF=" << pF << endl;
//log() << " xF=" << xF << " xC=" << xC << endl;
//log() << "\nPeriode=" << t << endl;
//printf(" pF=%10.10f Ed=%10.10f\n",pF, Ed);
//printf("p=%6.6f ",p);
//printf("pPerf=%6.6f \n\n",pPerf);

}
