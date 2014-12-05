///////////////////////////////////////////////////////////////////////////////
//
// Name: linFima_basic.C
//       lineares Finanzmarktmodell mit Erwartungsr"uckkoppelung (Basis Version)
//	 K=1
//
// Code: Markus Kopyciok 
//
///////////////////////////////////////////////////////////////////////////////

#include "linFima_basic.h"
#include "../error.h"


///////////////////////////////////////////////////////////////////////////////
// Funktionsname:	linFima_basic
// Beschreibung:	constructor
///////////////////////////////////////////////////////////////////////////////
 linFima_basic::linFima_basic() : baseModel(2) 
 {
 	zvar = NULL;
 };

///////////////////////////////////////////////////////////////////////////////
// Funktionsname:	initialize
// Beschreibung:	initialize the model, define the systems initial state
///////////////////////////////////////////////////////////////////////////////
void linFima_basic::initialize()
{
	x  =	x0;
	x1 =	x0;
	x2 =	x0;
	
	y  =	y0;
	y1 =	y0;
	y2 =	y0;
	
	d  =	d0;
	d1 =	d0;
	d2 =	d0;
	
	zvar = new rand_var( "ranf",1,xi_min,xi_max );	
	if( !(zvar) )
		fatalError("rand_var::initialize stoch_ar","can't create rand_var");
	
	d = psi*d+zvar->dice();
	
	summex  = x0;
	summe1x = summex;
	summey  = y0;
	summe1y = summey;
	mx     = summex;
	my     = summey;
		
};

///////////////////////////////////////////////////////////////////////////////
// Funktionsname:	sendModelVar
// Beschreibung:	return a pointer to the main model variable
///////////////////////////////////////////////////////////////////////////////

qreal* linFima_basic::sendModelVar()
{
	return &x;
	return &y;
};


///////////////////////////////////////////////////////////////////////////////
// Funktionsname:	setLabels
// Beschreibung:	return a pointer to a variable or a parameter specified
///////////////////////////////////////////////////////////////////////////////
qreal* linFima_basic::setLabels(const QString& label)
{
	
    if (label == "xBundle") return(&xBundle);
    if (label == "x0") return(&x0);
    if (label == "x") return(&x);
    if (label == "x1") return(&x1);
    if (label == "x2") return(&x2);
	
	
    if (label == "yBundle") return(&yBundle);
    if (label == "y0") return(&y0);
    if (label == "y") return(&y);
    if (label == "y1") return(&y1);
    if (label == "y2") return(&y2);

    if (label == "xi0")  return(&xi0);
    if (label == "xi_min")  return(&xi_min);
    if (label == "xi_max")  return(&xi_max);
		
    if (label == "d0")  return(&d0);
    if (label == "d")	 return(&d);
    if (label == "d1")  return(&d1);
    if (label == "d2")	 return(&d2);
    if (label == "psi")  return(&psi);
	
    if (label == "alpha") return(&alpha);
    if (label == "beta") return(&beta);
    if (label == "gamma")  return(&gamma);
    if (label == "L") 	 return(&L);
	
    if (label == "summex")  return(&summex);
    if (label == "summe1x")  return(&summe1x);
    if (label == "summey")  return(&summey);
    if (label == "summe1y")  return(&summe1y);
    if (label == "mx")  return(&mx);
    if (label == "my")  return(&my);
	
		
	return NULL;
};

///////////////////////////////////////////////////////////////////////////////
// Funktionsname:	sendStateSpace
// Beschreibung:		return pointers to the state variables
///////////////////////////////////////////////////////////////////////////////
void linFima_basic::sendStateSpace(int &quantity,const qreal*** stateSpace)
{
    if( *stateSpace )
	delete *stateSpace;
    *stateSpace= new const qreal* [dimension]; // 1 asset in Model linFima_basic
    if( !stateSpace )
	fatalError("linFima_basic::sendStateSpace","Speicherfehler");
    quantity=dimension; // 1 asset in Model linFima_basic
   (*stateSpace)[0]=&x;
   (*stateSpace)[1]=&y;
};


///////////////////////////////////////////////////////////////////////////////
// Class name:		linFima_basic
// Funktionsname:	loadParamset
// Beschreibung:	Einlesen der Parameter aus der .sim Datei 
///////////////////////////////////////////////////////////////////////////////
void linFima_basic::loadParamset(QDataStream& inputFile)
{
	inputFile >> MODELL;
	inputFile >> x0     >> y0;
	inputFile >> alpha  >> beta;
	inputFile >> d0     >> psi;
	inputFile >> xi_min >> xi_max;	
	inputFile >> gamma;
	inputFile >> L;
	inputFile >> length;

	initialize();
};

///////////////////////////////////////////////////////////////////////////////
// Funktionsname: saveParamset
// Beschreibung:         write parameterset into a file
///////////////////////////////////////////////////////////////////////////////
void linFima_basic::saveParamset(QDataStream& outFile)
{
	outFile <<"\nlinFima_basic:"<<"\t";
	outFile << MODELL<<"\t";
	outFile << x0     << "\t" << y0   <<"\t"; 
	outFile << alpha  << "\t" << beta <<"\t";
	outFile << d0     << "\t" << psi  <<"\t";
	outFile << xi_min << "\t" << xi_max <<"\t";
	outFile << gamma;
	outFile << L;
	outFile << length;
};

///////////////////////////////////////////////////////////////////////////////
// Funktionsname: printParamset
// Beschreibung:        print parameterset on the screen
///////////////////////////////////////////////////////////////////////////////
void linFima_basic::printParamset()
{
	log() << "\nlinFima_basic:"<<"\t";
	log() << MODELL <<"\n";
	log() << x0     << "\t" << y0     <<"\n"; 
	log() << alpha  << "\t" << beta   <<"\n";
	log() << d0     << "\t" << psi    <<"\n";
	log() << xi_min << "\t" << xi_max <<"\n";
	log() << gamma  <<"\n";
	log() << L      <<"\n";
	log() << length <<"\n";

};

///////////////////////////////////////////////////////////////////////////////
// Funktionsname: sendParameters
// Beschreibung:        write all parameters into an array and return the number//                      of  parameters
///////////////////////////////////////////////////////////////////////////////
void linFima_basic::sendParameters(int& amount,qreal** parameters)
{
	if( *parameters )
		delete *parameters;
	amount=11;
	*parameters=new qreal[amount];
	if( !parameters )
		fatalError("linFima_basic::sendParameters",
			   "Can't create array for parameters");
	(*parameters[0]) =x0;
	(*parameters[1]) =y0;
	(*parameters[2]) =alpha;
	(*parameters[3]) =beta;
	(*parameters[4]) =d0;
	(*parameters[5]) =psi;
	(*parameters[6]) =xi_min;
	(*parameters[7]) =xi_max;
	(*parameters[8]) =gamma;
	(*parameters[9]) =L;
	(*parameters[10])=length;
};

///////////////////////////////////////////////////////////////////////////////
// Funktionsname: receiveParameters
// Beschreibung:         receive parameter values
///////////////////////////////////////////////////////////////////////////////
void linFima_basic::receiveParameters(const qreal* parameters)
{
	x0	= parameters[0];
	y0	= parameters[1];
	alpha	= parameters[2];
	beta	= parameters[3];
	d0	= parameters[4];
	psi	= parameters[5];
	xi_min	= parameters[6];
	xi_max	= parameters[7];
	gamma	= parameters[8];
	L	= parameters[9];
	length	= (qint64) (parameters[10]);
	
};

///////////////////////////////////////////////////////////////////////////////
// Funktionsname:	iteration
// Beschreibung:	Durchlauf einer Periode
///////////////////////////////////////////////////////////////////////////////
void linFima_basic::iteration(const qint64& t)
{	
	// noise
	d2 = d1;
	d1 = d;
	d  = psi * d1 + zvar->dice();
	
	// Preisprozess
	x2= x1;
	x1= x;
	//x = (1/alpha)*(y-beta); // Preisprozess nur unter Rationale Erwartungen
	  x = y1 - d1;	          // Allgemeiner Preiprozess
	
	// Erwartungsbildung
	
	//	- Rationale Erwartungen
	      	if (MODELL==1)
		{	 
	      	y1 = y;
		y  = beta+alpha*(y1-d1);
	      	} 
	
	//	- naive Erwartungen
		if (MODELL==2)
		{
		y2=y1;
		y1=y;
		y = gamma*y2+(1-gamma)*(x1+d1);
		}
	
	//	- durchschnittliche Erwartungen
		if (MODELL==3)
		{
		y = (1/L)*(x2+d2+x1+d1);
		}
		
	// rekursiver Mittelwert
	mx = rekursiverMittelwertx(t);
	my = rekursiverMittelwerty(t);		
		
};


///////////////////////////////////////////////////////////////////////////////
// Funktionsname: rekursiverMittelwert
// Beschreibung: Berechnet den rekursiven Mittelwert von Beobachtungen
///////////////////////////////////////////////////////////////////////////////

qreal linFima_basic::rekursiverMittelwertx(const qint64 t)
{
	// lokale Typen und Initialisierung
	double ttx    =0;	
	qreal mxx 	= summex;
		summe1x = summex;
		summex  = x+summe1x;

	
	ttx = t;
	
	mxx = (1/(ttx+1))*summex;
	return(mxx);
}

qreal linFima_basic::rekursiverMittelwerty(const qint64 t)
{
	// lokale Typen und Initialisierung
	double tty    =0;	
	qreal myy     = summey;
		summe1y = summey;
		summey  = y+summe1y;

	
	tty = t;
	
	myy = (1/(tty+1))*summey;
	return(myy);
}

	
	
