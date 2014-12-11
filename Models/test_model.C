///////////////////////////////////////////////////////////////////////////////
//
// Module name:		test_model.C
// Contents:		Member functions of the class test_model
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

#include "../error.h"
#include "test_model.h"


///////////////////////////////////////////////////////////////////////////////
//
// Class name:		test_model
// Member function:	test_model
// Purpose:		constructor
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

test_model::test_model() : baseModel(1)
{
	U = s1 = s2 = 0;
}

///////////////////////////////////////////////////////////////////////////////
//
// Class name:		test_model
// Member function:	test_model
// Purpose:		constructor
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

test_model::~test_model()
{
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		test_model
// Member function:	iteration
// Purpose:		perform one iteration of the system
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void test_model::iteration(const qint64& time)
{
// 	d = distri.st_normal();
//	d2 = distri.st_normal();
	d = distri.triangular(0,1,0.5);

//	s1 = 1.0-(1.0/2.71)*(1.0+1.0-d*0.1+0.3);
//	s2 = (0.81/2.71)*(1.0+1.0-d*0.1+0.3)-0.3;
//	U = log(10.0-s1)+0.9*log(10.0+s1-d*1.0-s2)+0.81*log(s2+3.0);

// Fixpunktgleichung Diplomarbeit CD-Nutzen prop. Kosten I 
// weiche Parameterwerte
// l1=1 l2=1.4 A=10 B=0.5 delta=0.9 varphi=0.45 alpha_max=1.0697564
// kein Rentensystem
/*	qreal A = 10.0;
    qreal B = 0.1;
    qreal l_1 = 1.0;
    qreal l_2 = 1.71;
    qreal delta = 0.9;
    qreal varphi = 0.45;
    qreal alpha_max = 1.0697564;

    qreal w = A*(1.0-B)*pow(d,B);
    qreal R = 1.0+A*B*pow(d,B-1.0);
    //qreal R=1.0;
	
	s1 = l_1*w-1.0/(1.0+delta+delta*delta)*(l_1*w+l_2*w/R);
    log() << "w: " << w <<"\tR: " << R << "\ts1: " << s1 << "\ts2: " << s2 
	s2 = delta/(1.0+delta)*(l_2*w+R*s1);
	d2 = 1.0/(l_2*(1.0+sqrt(l_2/l_1)))*(sqrt(l_2/l_1)*s1+s2)-d;

	d2 = distri2.random( d , 1.0);
*/
/*	// Lorenz-Attraktor
	dx = a*(y-x);
	dy = x*(b-z)-y;
	dz = x*y-c*z;
	
	x += dx;
	y += dy;
	z += dz;
*/
}
    
    
///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		test_model
// Member function:	initialize
// Purpose:		initialize the model, define the systems initial state
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void test_model::initialize()
{
//	distri.setseed( 1 );
	x = x0;
	y = y0;
	z = z0;
	dx=dy=dz=0;
}
 
    
///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		test_model
// Member function:	sendModelVar
// Purpose:		return a pointer to the main model variable
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

qreal* test_model::sendModelVar()
{
    return &d;
}

///////////////////////////////////////////////////////////////////////////////
//
// Class name:		test_model
// Member function:	setLabels
// Purpose:		return a pointer to a variable or a parameter specified
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

qreal* test_model::setLabels(const QString& label)
{
    if (label == "Ed")
		return( &Ed);
    if (label == "Vd")
		return( &Vd);
    if (label == "d")
		return( &d );
    if (label == "d2")
		return( &d2 );
    if (label == "s1")
		return( &s1 );
    if (label == "s2")
		return( &s2 );
    if (label == "U")
		return( &U );
    if (label == "x")
		return( &x );
    if (label == "y")
		return( &y );
    if (label == "z")
		return( &z );
    if (label == "a")
		return( &a );
    if (label == "b")
		return( &b );
    if (label == "c")
		return( &c );
    if (label == "x0")
		return( &x0 );
    if (label == "y0")
		return( &y0 );
    if (label == "z0")
		return( &z0 );
    if (label == "dx")
		return( &dx );
    if (label == "dy")
		return( &dy );
    if (label == "dz")
		return( &dz );
		
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		test_model
// Member function:	sendStateSpace
// Purpose:		return pointers to the state variables and inform about
//			the systems dimension
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void test_model::sendStateSpace(int &quantity,const qreal*** stateSpace)
{
    if( stateSpace )
	delete stateSpace;
    *stateSpace= new const qreal* [dimension];
    if( !(*stateSpace) )
	fatalError("test_model::sendStateSpace",
		   "Can't create state space vector");
    quantity=dimension;
    (*stateSpace)[0]=&d;
};

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		test_model
// Member function:	loadParamset
// Purpose:		load a parameterset from a specified input file
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void test_model::loadParamset(QTextStream& inFile)
{
/*	inFile >> d;
	inFile >> x0 >> y0 >> z0;
	inFile >> a >> b >> c;
*/	inFile >> length;
   
	initialize();
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		test_model
// Member function:	saveParamset
// Purpose:		write parameterset into a file
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void test_model::saveParamset(QTextStream& outFile)
{
	outFile << d << "\t" << length;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		test_model
// Member function:	saveParamsetWithNames
// Purpose:		write parameterset into a file
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////


void test_model::saveParamsetWithNames(QTextStream& outFile)
{
    outFile << "d=" << d << "\nlength = " << length;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		test_model
// Member function:	printParamset
// Purpose:		print parameterset on the screen
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void test_model::printParamset()
{
    log() << d << "\t" << length;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		test_model
// Member function:	sendParameters
// Purpose:		write all parameters into an array and return the numbers
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void test_model::sendParameters(int& amount,qreal** parameters)
{
    if( *parameters )
	delete *parameters;
    amount=2;
    *parameters=new qreal[amount];
    if( !(*parameters) )
	fatalError("test_model::sendParameters",
		   "Can't create array for parameters");
    (*parameters[0])=d;
    (*parameters[1])=length;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		test_model
// Member function:	receiveParameters
// Purpose:		receive parameter values
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

void test_model::receiveParameters(const QList<qreal>& parameters)
{
    d=parameters[0];
    length=qint64(parameters[1]);
}

