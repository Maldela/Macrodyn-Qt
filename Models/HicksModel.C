///////////////////////////////////////////////////////////////////////////////
//
// Module name:		HicksModel.C
// Contents:		Member functions of the class HicksModel
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

#include "../error.h"
#include "../strnchr.h"
#include "HicksModel.h"


///////////////////////////////////////////////////////////////////////////////
//
// Class name:		HicksModel
// Member function:	HicksModel
// Purpose:		constructor
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

HicksModel::HicksModel() : baseModel(1)
{
}

///////////////////////////////////////////////////////////////////////////////
//
// Class name:		HicksModel
// Member function:	HicksModel
// Purpose:		destructor
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

HicksModel::~HicksModel() 
{
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		HicksModel
// Member function:	iteration
// Purpose:		perform one iteration of the system
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void HicksModel::iteration(const qint64&)
{
	noise = distri.rectangular()*2.0*lambda;
	Ct = m*y2;
    Iind = qMax( v*(y2-y1), -If );
	It = Iind + Iaut + noise;
	y1 = y2;
    y2 = qMin( Ct+It, Yc );
}
    
    
///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		HicksModel
// Member function:	initialize
// Purpose:		initialize the model, define the systems initial state
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void HicksModel::initialize()
{
	y1 = y1_0;
	y2 = y2_0;
	if ( myseed!=0 ){
		distri.setseed( myseed );
	}
}
 
    
///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		HicksModel
// Member function:	sendModelVar
// Purpose:		return a pointer to the main model variable
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

qreal* HicksModel::sendModelVar()
{
    return &y2;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		HicksModel
// Member function:	setLabels
// Purpose:		return a pointer to a variable or a parameter specified
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

qreal* HicksModel::setLabels(const QString& label)
{
    if (label == "xBundle")
		return( &xBundle );
    if (label == "yBundle")
		return( &yBundle );
    if (label == "y1_0")
		return( &y1_0);
    if (label == "y2_0")
		return( &y2_0);		
    if (label == "y1")
		return( &y1 );
    if (label == "y2")
		return( &y2 );
    if (label == "If")
		return( &If );
    if (label == "Yc")
		return( &Yc );
    if (label == "v")
		return( &v );
    if (label == "m")
		return( &m );
    if (label == "It")
		return( &It );
    if (label == "Iind")
		return( &Iind );
    if (label == "Iaut")
		return( &Iaut );				
    if (label == "Ct")
		return( &Ct );
    if (label == "lambda")
		return( &lambda );	
    if (label == "noise")
		return( &noise );			
		
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		HicksModel
// Member function:	sendStateSpace
// Purpose:		return pointers to the state variables and inform about
//			the systems dimension
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void HicksModel::sendStateSpace(int &quantity,const qreal*** stateSpace)
{
    if( *stateSpace )
	delete *stateSpace;
    *stateSpace= new const qreal* [dimension];
    if( !(*stateSpace) )
	fatalError("HicksModel::sendStateSpace",
		   "Can't create state space vector");
    quantity=dimension;
    (*stateSpace)[0]=&y2;
};

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		HicksModel
// Member function:	loadParamset
// Purpose:		load a parameterset from a specified input file
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void HicksModel::loadParamset(QDataStream& inFile)
{
	inFile >> y1_0 >> y2_0 >> If >> Iaut >> Yc >> v >> m >> lambda >> myseed;
	inFile >> length;
   
	initialize();
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		HicksModel
// Member function:	saveParamset
// Purpose:		write parameterset into a file
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void HicksModel::saveParamset(QDataStream& outFile)
{
	outFile << y1_0 << "\t" << y2_0 << "\t" << If << "\t";
	outFile << Iaut << "\t" << Yc << "\t" << v << "\t" << m << "\t" << lambda << "\t" << myseed;
	outFile << "\t" << length;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		HicksModel
// Member function:	saveParamsetWithNames
// Purpose:		write parameterset into a file
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////


void HicksModel::saveParamsetWithNames(QDataStream& outFile)
{
	outFile << "y1_0 = " << y1_0;
	outFile << "\ny2_0 = " << y2_0;
	outFile << "\nIf = " << If;
	outFile << "\nIaut = " << Iaut;
	outFile << "\nYc = " << Yc;
	outFile << "\nv = " << v;
	outFile << "\nm = " << m;
	outFile << "\nlambda = " << lambda;
	outFile << "\nseed = " << myseed;
	outFile << "\nlength = " << length << "\n";
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		HicksModel
// Member function:	printParamset
// Purpose:		print parameterset on the screen
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void HicksModel::printParamset()
{
    log() << y1_0 << "\t" << y2_0 << "\t" << If << "\t" << Iaut << "\t";
    log() << Yc << "\t" << v << "\t" << m  << "\t" << lambda << "\t" << myseed << "\t" << length << "\n";
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		HicksModel
// Member function:	sendParameters
// Purpose:		write all parameters into an array and return the numbers
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void HicksModel::sendParameters(int& amount,qreal** parameters)
{
    if( *parameters )
	delete *parameters;
    amount=9;
    *parameters=new qreal[amount];
    if( !(*parameters) )
	fatalError("HicksModel::sendParameters",
		   "Can't create array for parameters");
    (*parameters[0])=y1_0;
    (*parameters[1])=y2_0;
    (*parameters[2])=If;
    (*parameters[3])=Iaut;
    (*parameters[4])=Yc;
    (*parameters[5])=v;
    (*parameters[6])=m;
    (*parameters[7])=lambda;
    (*parameters[8])=length;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		HicksModel
// Member function:	receiveParameters
// Purpose:		receive parameter values
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void HicksModel::receiveParameters(const qreal* parameters)
{
    y1_0=parameters[0];
    y2_0=parameters[1];
    If=parameters[2];
    Iaut=parameters[3];
    Yc=parameters[4];
    v=parameters[5];
    m=parameters[6];
    lambda=parameters[7];
    length=qint64(parameters[8]);
}

