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
qreal* linFimaErwRueck::setLabels(const QString& label)
{
//	if (label == "intVarName") return( (qreal*)(&intVarName) );
	if (label == "R") return(&R);
	if (label == "x") return(&xAll);

	if (label == "p") return(&p);
	if (label == "p0") return(&p0);
  	if (label == "pOld") return(&pOld);
  	if (label == "pPerf") return(&pPerf);
  	if (label == "pDiff") return(&pDiff);

	if (label == "rF") return(&rF);
	if (label == "eF") return(&eF);
	if (label == "pF0") return(&pF0);
	if (label == "pF") return(&pF);
	if (label == "pFOld") return(&pFOld);
	if (label == "qOld") return(&pFOld);
  	if (label == "pFperf") return(&pFperf);
	if (label == "xF") return(&xF);
	if (label == "xFold") return(&xFold);
	if (label == "xFrel") return(&xFrel);
	if (label == "wF") return(&wF);
	if (label == "RF") return(&RF);
	if (label == "RpF") return(&RpF);

	if (label == "rC") return(&rC);
	if (label == "eC") return(&eC);
	if (label == "pC0") return(&pC0);
	if (label == "pC") return(&pC);
  	if (label == "pCperf") return(&pCperf);
	if (label == "xC") return(&xC);
	if (label == "xCold") return(&xCold);
	if (label == "xCrel") return(&xCrel);
	if (label == "wC") return(&wC);
	if (label == "RC") return(&RC);
	if (label == "RpC") return(&RpC);

	if (label == "de") return(&de);
	if (label == "Ed") return(&Ed);
	if (label == "Vd") return(&Vd);
	if (label == "d") return(&d);

	if (label == "ferr") return(&ferrF);
	if (label == "ferrF") return(&ferrF);
	if (label == "ferrC") return(&ferrC);
	if (label == "deltap") return(&deltap);
	if (label == "deltaq") return(&deltapF);
	if (label == "deltapF") return(&deltapF);
	if (label == "ertrag") return(&ertrag);
	if (label == "return") return(&ertrag);

	if (label == "errvar") return(&errvar);
	if (label == "divvar") return(&divvar);


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

    // später gebrauchte Konstanten
	// Annahme: VC,VF zeitinvariant
	if(rC==0)
		r=0; // !!!
	 else	r=1/( 1 + ( (rF/VF) * (VC/rC) ) );
	a1VFx=(1/rF)*VF*xAll;
	b=xAll/((rC/VC)+(rF/VF));
//	b=((1-r)*VF*xAll)/rF;     // Problem bei rF=0 !!!

    log() <<"Ed=" << Ed <<"Vd=" << Vd <<"d=" << d;
    log() <<"r=" << r;
    log() <<"b=" << b;
    qreal beta=1.03;qreal L=2;
    printf("THETA=%10.10f %10.10f %10.10f %10.10f %10.10f \n",
        (beta/(L*R))*rC/(rC+rF), (beta/(L*R))*rC/(rC+rF), ((1-r)/R), 1/R, -b/R );

}

///////////////////////////////////////////////////////////////////////////////
// Class name:		linFimaErwRueck
// Funktionsname:	loadParamset
// Beschreibung:	Einlesen der Parameter aus der .sim Datei 
///////////////////////////////////////////////////////////////////////////////

void linFimaErwRueck::loadParamset(QTextStream& inFile)
{
    QString str;
    inFile >> str;
    if(str != "V1.4")
    {
		fatalError("loadParamset","wrong version");
        return;
    }
	
    inFile >> str;
    if(str == "PrognoseFundamentalisten=perfekt")
		perfectPredictor=1; // Ja
	 else	perfectPredictor=0; // Nein

    inFile >> str;
    if(str == "Dividenden=bekannt")
		DividendenErwartung=bekannt;
	 else	DividendenErwartung=unbekannt;

    inFile >> vc; // Skalierung der erw. Div.Varianz der C

    inFile >> R;

    inFile >> rF; // Risikotoleranz
    inFile >> rC;

    inFile >> eF; // Anfangsausstattung
    inFile >> eC;

    inFile >> xAll;

    inFile >> p0;
	pF0=p0;
	pC0=p0;	

    inFile >> length;

	initialize();
}

void linFimaErwRueck::receiveParameters(const QList<qreal> &parameters)
{
    if (parameters.size() != 11) log() << "Wrong number of parameters!";
    else
    {
        perfectPredictor = parameters.at(0);
        DividendenErwartung = parameters.at(1) ? bekannt : unbekannt;
        vc = parameters.at(2);
        R = parameters.at(3);
        rF = parameters.at(4);
        rC = parameters.at(5);
        eF = parameters.at(6);
        eC = parameters.at(7);
        xAll = parameters.at(8);
        p0 = parameters.at(9);
        length = parameters.at(10);
    }
}

///////////////////////////////////////////////////////////////////////////////
// Funktionsname: saveParamsetWithNames
// Beschreibung:         add  parameterset to printfile
///////////////////////////////////////////////////////////////////////////////
void linFimaErwRueck::saveParamsetWithNames(QTextStream& outputFile)
{
    outputFile << "Model linFimaErwRueck";
    if(perfectPredictor)
    outputFile << "perfectPredictor";
    outputFile << "R = " << R;
    outputFile << "xAll= " << xAll << " ";
    outputFile << "p0= " << p0 << " ";
    outputFile << "length = "<< length;
}

///////////////////////////////////////////////////////////////////////////////
// Funktionsname:	sendStateSpace
// Beschreibung:		return pointers to the state variables
///////////////////////////////////////////////////////////////////////////////
void linFimaErwRueck::sendStateSpace(int &quantity,QList<qreal *> *stateSpace)
{
    stateSpace->clear();
    quantity = 1; // 1 asset in Model linFimaErwRueck
    *stateSpace << &p;
}

///////////////////////////////////////////////////////////////////////////////
// Funktionsname:	iteration
// Beschreibung:	Durchlauf einer Periode
///////////////////////////////////////////////////////////////////////////////
void linFimaErwRueck::iteration(const qint64& t)
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

//	if(p<0) log() <<"periode="<<t<<" p="<<p<<" !"<<"\n";

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


//log() << t << " pCperf=" << pCperf << " pC=" << pC ;//
//log() << " p=" << p << " pPerf=" << pPerf 
//log() << " errvar=" << errvar  
//log() << "d=" << d << " Ed=" << Ed << " de=" << de  

//log() << " p=" << p << " pF=" << pF 
//log() << " xF=" << xF << " xC=" << xC 
//log() << "\nPeriode=" << t 
//printf(" pF=%10.10f Ed=%10.10f\n",pF, Ed);
//printf("p=%6.6f ",p);
//printf("pPerf=%6.6f \n\n",pPerf);

}
