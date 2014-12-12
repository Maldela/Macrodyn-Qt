///////////////////////////////////////////////////////////////////////////////
//
// Module name:		ELS.C
// Contents:		Member functions of the class ELS
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

#include "../error.h"

#include "ELS.h"


///////////////////////////////////////////////////////////////////////////////
//
// Class name:		ELS
// Member function:	ELS
// Purpose:		constructor
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

ELS::ELS() : baseModel(1)
{
}


///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		ELS
// Member function:	iteration
// Purpose:		perform one iteration of the system
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void ELS::iteration(const qint64&)
{
    qreal oldY=y;

    y = c0+c1*oldY+pow(qreal_money,alpha)*pow(oldY,1-alpha)+G-c1*T;
}
    
    
///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		ELS
// Member function:	initialize
// Purpose:		initialize the model, define the systems initial state
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void ELS::initialize()
{
    y=y0;
}
 
    
///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		ELS
// Member function:	sendModelVar
// Purpose:		return a pointer to the main model variable
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

qreal* ELS::sendModelVar()
{
    return &y;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		ELS
// Member function:	setLabels
// Purpose:		return a pointer to a variable or a parameter specified
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

qreal* ELS::setLabels(const QString& label)
{
    if (label == "xBundle")
		return( &xBundle );
    if (label == "yBundle")
		return( &yBundle );
    if (label == "alpha")
		return( &alpha);
    if (label == "y")
		return( &y);
    if (label == "y0")
		return( &y0 );
    if (label == "c0")
		return( &c0 );
    if (label == "c1")
		return( &c1 );
    if (label == "qreal_money")
		return( &qreal_money );
    if (label == "G")
		return( &G );
    if (label == "T")
		return( &T );

	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		ELS
// Member function:	sendStateSpace
// Purpose:		return pointers to the state variables and inform about
//			the systems dimension
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void ELS::sendStateSpace(int &quantity,QList<qreal *> *stateSpace)
{
    stateSpace->clear();
    quantity = dimension;
    *stateSpace << &y;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		ELS
// Member function:	loadParamset
// Purpose:		load a parameterset from a specified input file
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void ELS::loadParamset(QTextStream& inFile)
{
	inFile >> c0 >> c1 >> qreal_money >> G >> T;
	inFile >> alpha >> y0;
	inFile >> length;
   
	initialize();
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		ELS
// Member function:	saveParamset
// Purpose:		write parameterset into a file
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void ELS::saveParamset(QTextStream& outFile)
{
	outFile << c0 << "\t" << c1 << "\t" << qreal_money << "\t";
	outFile << G << "\t" << T << "\t" << alpha  << "\t" << y0;
	outFile << "\t" << length;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		ELS
// Member function:	saveParamsetWithNames
// Purpose:		write parameterset into a file
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////


void ELS::saveParamsetWithNames(QTextStream& outFile)
{
	outFile << "c0 = " << c0;
	outFile << "\nc1 = " << c1;
	outFile << "\nqreal_money = " << qreal_money;
	outFile << "\nG = " << G << "\tT = " << T;
	outFile << "\nalpha = " << alpha << "\ty0 = " << y0;
    outFile << "length = " << length;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		ELS
// Member function:	printParamset
// Purpose:		print parameterset on the screen
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void ELS::printParamset()
{
	log() << c0 << "\t" << c1 << "\t" << qreal_money << "\t";
	log() << G << "\t" << T << "\t" << alpha  << "\t" << y0;
    log() << "\t" << length;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		ELS
// Member function:	sendParameters
// Purpose:		write all parameters into an array and return the numbers
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void ELS::sendParameters(int& amount,qreal** parameters)
{
    if( *parameters )
	delete *parameters;
    amount=8;
    *parameters=new qreal[amount];
    if( !(*parameters) )
	fatalError("ELS::sendParameters",
		   "Can't create array for parameters");
    (*parameters[0])=c0;
    (*parameters[1])=c1;
    (*parameters[2])=qreal_money;
    (*parameters[3])=G;
    (*parameters[4])=T;
    (*parameters[5])=alpha;
    (*parameters[6])=y0;
    (*parameters[7])=length;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		ELS
// Member function:	receiveParameters
// Purpose:		receive parameter values
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void ELS::receiveParameters(const QList<qreal>& parameters)
{
    c0=parameters[0];
    c1=parameters[1];
    qreal_money=parameters[2];
    G=parameters[3];
    T=parameters[4];
    alpha=parameters[5];
    y0=parameters[6];
    length=qint64(parameters[7]);
}

