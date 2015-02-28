///////////////////////////////////////////////////////////////////////////////
//
// 
//
// Module name:		boehmhuels.C
// Contents:		Member functions of the class boehmhuels
//
// Author:		Oliver Claas
// Last modified:	
// By:			Oliver Claas
//
///////////////////////////////////////////////////////////////////////////////

//#include "../error.h"
//#include "../strnchr.h"
#include "boehmhuels.h"


///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		boehmhuels
// Member function:	boehmhuels
// Purpose:		constructor
//
// Author:		Oliver Claas
// Last modified:	
// By:			Oliver Claas
//
///////////////////////////////////////////////////////////////////////////////

boehmhuels::boehmhuels() : baseModel(1)
{
}


///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		boehmhuels
// Member function:	iteration
// Purpose:		perform one iteration of the system
//
// Author:		Oliver Claas
// Last modified:	
// By:			Oliver Claas
//
///////////////////////////////////////////////////////////////////////////////

void boehmhuels::iteration(const qint64&)
{
    qreal x=xt;
    qreal y=yt;
    qreal p=pt;

    xt= x*(delta+(1.-delta)/(alpha-y));
    yt= delta*y+(1.-delta)*x*x*((1./(alpha-y))-1.)*((1./(alpha-y))-1.);

    pt= (ybar/r)+(1./exp(0.5*log(-beta)))*(x/(alpha-y));
    rett= pt/p-1;
    if(rett>0.){absrett= rett;}else{absrett= -rett;}
    sqrett= rett*rett;
}
    
    
///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		boehmhuels
// Member function:	initialize
// Purpose:		initialize the model, define the systems initial state
//
// Author:		Oliver Claas
// Last modified:	
// By:			Oliver Claas
//
///////////////////////////////////////////////////////////////////////////////

void boehmhuels::initialize()
{
	xt = x0;
	yt = y0;
    pt = 1.0;
}
 
    
///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		boehmhuels
// Member function:	sendModelVar
// Purpose:		return a pointer to the main model variable
//
// Author:		Oliver Claas
// Last modified:	
// By:			Oliver Claas
//
///////////////////////////////////////////////////////////////////////////////

qreal* boehmhuels::sendModelVar()
{
    return &xt;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		boehmhuels
// Member function:	setLabels
// Purpose:		return a pointer to a variable or a parameter specified
//
// Author:		Oliver Claas
// Last modified:	
// By:			Oliver Claas
//
///////////////////////////////////////////////////////////////////////////////

qreal* boehmhuels::setLabels(const QString& label)
{
    if( label == "alpha" )
		return( &alpha);
    if( label=="delta" )
		return( &delta);
    if( label=="beta" )
		return( &beta);
    if( label=="ybar" )
		return( &ybar);
    if( label=="r" )
		return( &r);
    if( label=="xt" )
		return( &xt);
    if( label=="yt" )
		return( &yt);
    if( label=="x0" )
		return( &x0 );
    if( label=="y0" )
        return( &y0 );
    if( label=="pt" )
        return( &pt );
    if( label=="rett" )
        return( &rett );
    if( label=="absrett" )
        return( &absrett );
    if( label=="sqrett" )
        return( &sqrett );
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		boehmhuels
// Member function:	sendStateSpace
// Purpose:		return pointers to the state variables and inform about
//			the systems dimension
//
// Author:		Oliver Claas
// Last modified:	
// By:			Oliver Claas
//
///////////////////////////////////////////////////////////////////////////////

void boehmhuels::sendStateSpace(int &quantity,QList<qreal *> *stateSpace)
{
    stateSpace->clear();
    quantity = dimension;
    *stateSpace << &xt;
    *stateSpace << &yt;

//	if( stateSpace )
//		delete stateSpace;
//    *stateSpace = new const qreal* [dimension];
//	if( !(*stateSpace) )
//		fatalError("boehmhuels::sendStateSpace",
//		           "Can't create state space vector");
//	quantity=dimension;
//	(*stateSpace)[0]=&xt;
//	(*stateSpace)[1]=&yt;
};

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		boehmhuels
// Member function:	loadParamset
// Purpose:		load a parameterset from a specified input file
//
// Author:		Oliver Claas
// Last modified:	
// By:			Oliver Claas
//
///////////////////////////////////////////////////////////////////////////////

void boehmhuels::loadParamset(QTextStream& inFile)
{
	inFile >> alpha ;
	inFile >> delta;
	inFile >> beta;
	inFile >> ybar;
	inFile >> r;
	inFile >> x0;
	inFile >> y0;
	inFile >> length;
	
	initialize();
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		boehmhuels
// Member function:	saveParamset
// Purpose:		write parameterset into a file
//
// Author:		Oliver Claas
// Last modified:	
// By:			Oliver Claas
//
///////////////////////////////////////////////////////////////////////////////

void boehmhuels::saveParamset(QTextStream& outFile)
{
    outFile << alpha	<< "\t";
    outFile << delta	<< "\t";
    outFile << beta 	<< "\t";
    outFile << ybar 	<< "\t";
    outFile << r 	<< "\t";
    outFile << x0 	<< "\t";
    outFile << y0 	<< "\t";
    outFile << length;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		boehmhuels
// Member function:	printParamset
// Purpose:		print parameterset on the screen
//
// Author:		Oliver Claas
// Last modified:	
// By:			Oliver Claas
//
///////////////////////////////////////////////////////////////////////////////

void boehmhuels::printParamset()
{
    log() << alpha;
    log() << delta;
    log() << beta;
    log() << ybar;
    log() << r;
    log() << x0;
    log() << y0;
    log() << length;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		boehmhuels
// Member function:	sendParameters
// Purpose:		write all parameters into an array and return the numbers
//
// Author:		Oliver Claas
// Last modified:	
// By:			Oliver Claas
//
///////////////////////////////////////////////////////////////////////////////

void boehmhuels::sendParameters(int& amount,qreal** parameters)
{
	if( *parameters )
		delete *parameters;
	amount=8;
    *parameters=new qreal[amount];
	if( !(*parameters) )
		fatalError("boehmhuels::sendParameters",
		           "Can't create array for parameters");
	(*parameters[0])=alpha;
	(*parameters[1])=delta;
	(*parameters[2])=beta;
	(*parameters[3])=ybar;
	(*parameters[4])=r;
	(*parameters[5])=x0;
	(*parameters[6])=y0;
	(*parameters[7])=length;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		boehmhuels
// Member function:	receiveParameters
// Purpose:		receive parameter values
//
// Author:		Oliver Claas
// Last modified:	
// By:			Oliver Claas
//
///////////////////////////////////////////////////////////////////////////////

void boehmhuels::receiveParameters(const QList<qreal>& parameters)
{
	alpha=parameters[0];
	delta=parameters[1];
	beta=parameters[2];
	ybar=parameters[3];
	r=parameters[4];
	x0=parameters[5];
	y0=parameters[6];
	length=(long)(parameters[7]);
}


// eof
