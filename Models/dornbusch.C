///////////////////////////////////////////////////////////////////////////////
//
// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/dornbusch.C,v 1.1 2000/08/31 15:48:35 mmeyer Exp $
//
// Module name:		michael.C
// Author:		Michael Meyer
// Last modified:	Fri Mar 12 14:39:03 MET 1999
// By:			
//
///////////////////////////////////////////////////////////////////////////////

#include "dornbusch.h"
#include "../error.h"



//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
//****                                                                     ****
//****                        d o r n b u s c h                            ****
//****                                                                     ****
//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
//*****************************************************************************


///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		dornbusch
// Purpose:		constructor
//
// Author:		Michael Meyer
// Last modified:	Mon Mar 15 14:09:17 MET 1999
// By:			Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

dornbusch::dornbusch() : baseModel(1)
{
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		dornbusch 
// Purpose:		destructor
//
// Author:		Michael Meyer & Stefan Lüke
// Last modified:	Mon Mar 10 14:09:17 MET 1999
// By:			Michael Meyer 
//
///////////////////////////////////////////////////////////////////////////////

dornbusch::~dornbusch(void) {
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		dornbusch
// Member function:	iteration
// Purpose:		perform one iteration of the system
//
// Author:		Michael Meyer
// Last modified:	Mon Mar  15 14:27:09 MET 1999
// By:			Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

void dornbusch::iteration(const long& )
{ 
  it = -1/lambda*(log(M/pt)-theta*log(Yr));
  xetr = xet;
  xet=xet*(1-(1/lambda)*(log(M/pt)-theta*log(Yr)))/(1+iA);
  ptr=pt; 
  pt=pt+pi*(u+delta*log(pA)+(sigma/lambda)*log(M)-log(Yr)*
     (1-gamma+sigma*theta/lambda)+delta*log(1+iA))*pt-
     pi*(delta+sigma/lambda)*pt*log(pt)-
     pi*delta*log((1-(1/lambda)*log(M)+(1/lambda)*theta*log(Yr))+
     (1/lambda)*log(pt))*pt+pi*delta*log(xet)*pt;  
}
    
///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		dornbusch
// Member function:	initialize
// Purpose:		initialize the model, define the systems initial state
//
// Author:		Uli Middelberg
// Last modified:	Mon Mar 10 15:43:17 MET 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

void dornbusch::initialize()
{
  pt=pt_0;
  xet=xet_0;  
}
 

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		dornbusch
// Member function:	setLabels
// Purpose:		return a pointer to a variable or a parameter specified
//
// Author:		Uli Middelberg
// Last modified:	98/10/15
// By:			Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////

real* dornbusch::setLabels(char* label)
{
    if( !strcmp(label,"it") ) return(&it);
    if( !strcmp(label,"xetr") ) return(&xetr);
    if( !strcmp(label,"ptr") ) return(&ptr);
    if( !strcmp(label,"M") ) return(&M);
    if( !strcmp(label,"pA") ) return(&pA);
    if( !strcmp(label,"iA") ) return(&iA);
    if( !strcmp(label,"Yr") ) return(&Yr);
    if( !strcmp(label,"pi") ) return(&pi);
    if( !strcmp(label,"u") ) return(&u);
    if( !strcmp(label,"delta") ) return(&delta);
    if( !strcmp(label,"gamma") ) return(&gamma);
    if( !strcmp(label,"sigma") ) return(&sigma);
    if( !strcmp(label,"lambda") ) return(&lambda);
    if( !strcmp(label,"theta") ) return(&theta);
    
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		dornbusch
// Member function:	sendStateSpace
// Purpose:		return pointers to the state variables and inform about
//			the systems dimension
//
// Author:		Uli Middelberg
// Last modified:	Mon Mar 10 15:43:28 MET 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

void dornbusch::sendStateSpace(int &quantity,const real*** stateSpace)
{
    if( stateSpace )
	delete stateSpace;
    *stateSpace= new const real* [dimension];
    if( !(*stateSpace) )
	fatalError("dornbusch::sendStateSpace",
		   "Can't create state space vector");
    quantity=dimension;
    (*stateSpace)[0]=&M;
};

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		dornbusch
// Member function:	loadParamset
// Purpose:		load a parameterset from a specified input file
//
// Author:		Michael Meyer 
// Last modified:	Mon May 11 13:07:57 METDST 1999
// By:			Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

void dornbusch::loadParamset(ifstream& inFile)
{
    inFile >> M;
    inFile >> Yr;    
    inFile >> pA;
    inFile >> iA;
    inFile >> pt_0;
    inFile >> xet_0;
    inFile >> pi;
    inFile >> u;    
    inFile >> delta;
    inFile >> gamma;
    inFile >> sigma;  
    inFile >> lambda;  
    inFile >> theta;  
    inFile >> length;
    initialize();
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		dornbusch
// Member function:	saveParamset
// Purpose:		write parameterset into a file
//
// Author:		Uli Middelberg
// Last modified:	Mon May 11 13:11:41 METDST 1998
// By:			Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////

void dornbusch::saveParamset(ofstream& outFile)
{
    outFile << M << "\t";
    outFile << Yr << "\t";
    outFile << pA << "\t";
    outFile << iA << "\t";
    outFile << pt_0 << "\t";
    outFile << xet_0 << "\t";
    outFile << pi << "\t";
    outFile << u << "\t";
    outFile << delta << "\t";
    outFile << gamma << "\t";
    outFile << sigma << "\t";
    outFile << lambda << "\t";
    outFile << theta << "\t";
    outFile << length;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		dornbusch
// Member function:	printParamset
// Purpose:		print parameterset on the screen
//
// Author:		Uli Middelberg
// Last modified:	Mon May 11 13:12:05 METDST 1998
// By:			Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////

void dornbusch::printParamset()
{
    cout << M << endl;
    cout << Yr << endl;
    cout << pA << endl;
    cout << iA << endl;
    cout << pt_0 << endl;
    cout << xet_0 << endl;
    cout << pi << endl;
    cout << u << endl;
    cout << delta << endl;
    cout << gamma << endl;
    cout << sigma << endl;    
    cout << lambda << endl;  
    cout << theta << endl;
    cout << length << endl;       
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		dornbusch
// Member functions:	sendModelVar, sendParameters, receiveParameters
// Purpose:		for compatibity reasons only
//
// Author:		Uli Middelberg
// Last modified:	Tue Mar 11 16:10:38 MET 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////
real* dornbusch::sendModelVar(void)
{ error("macrodyn::dornbusch::sendModelVar is not implemented");
  return NULL;
}
void dornbusch::sendParameters(int& ,real** )
{ error("macrodyn::dornbusch::sendParameters is not implemented");
}
void dornbusch::receiveParameters(const real* )
{ error("macrodyn::dornbusch::receiveParameters is not implemented");
}

