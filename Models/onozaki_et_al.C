///////////////////////////////////////////////////////////////////////////////
//
// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/onozaki_et_al.C,v 1.1 2000/08/31 16:07:47 mmeyer Exp $
//
// Module name:		onozaki_et_al.C
// Contents:		Member functions of the class
//                        onozaki_et_al
//   
//
// Author:		mhoffman
// Last modified:	27 March 14:39:03 MET 2000
// By:			
//
///////////////////////////////////////////////////////////////////////////////

#include "../error.h"
#include "onozaki_et_al.h"


///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		onozaki_et_al
// Member function:	onozaki_et_al
// Purpose:		constructor
//
//
///////////////////////////////////////////////////////////////////////////////

onozaki_et_al::onozaki_et_al() : baseModel(1)
{ 
}
    

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		onozaki_et_al
// Member function:	iteration
// Purpose:		perform one iteration of the system
//
// Author:		mhoffman
// Last modified:	27 March 14:39:03 MET 2000
// By:			
//
///////////////////////////////////////////////////////////////////////////////

void onozaki_et_al::iteration(const qint64& )
{ 
  qreal z_n;				// value of z for the next period
	z_n = (1-alpha)*z + alpha/exp(log(z)*beta); 
 	// z_n=(1-alpha)z+alpha/zt^beta
  z = z_n;
}
    
///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		onozaki_et_al
// Member function:	initialize
// Purpose:		initialize the model, define the systems initial state
//
// Author:		mhoffman
// Last modified:	27 March 14:39:03 MET 2000
// By:			
//
///////////////////////////////////////////////////////////////////////////////

void onozaki_et_al::initialize()
{
  z = z_0;
}
 

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		onozaki_et_al
// Member function:	setLabels
// Purpose:		return a pointer to a variable or a parameter specified
//
// Author:		mhoffman
// Last modified:	27 March 14:39:03 MET 2000
// By:	
/////////////////////////////////////////////////////////////////////////////

qreal* onozaki_et_al::setLabels(const QString& label)
{
   
    if (label == "z") return(&z);
    if (label == "z_0") return(&z_0);
	if (label == "alpha") return(&alpha);
    if (label == "beta") return(&beta);
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		onozaki_et_al
// Member function:	sendStateSpace
// Purpose:			return pointers to the state variables and inform about
//					the systems dimension
//
// Author:			mhoffman
// Last modified:	27 March 14:39:03 MET 2000
// By:			
//
///////////////////////////////////////////////////////////////////////////////

void onozaki_et_al::sendStateSpace(int &quantity,const qreal*** stateSpace)
{
    if( stateSpace )
	delete stateSpace;
    *stateSpace= new const qreal* [dimension];
    if( !(*stateSpace) )
	fatalError("onozaki_et_al::sendStateSpace",
		   "Can't create state space vector");
    quantity=dimension;
    (*stateSpace)[0]=&z;
};

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		onozaki_et_al
// Member function:	loadParamset
// Purpose:		load a parameterset from a specified input file
//
// Author:		mhoffman
// Last modified:	27 March 14:39:03 MET 2000
// By:			
//
///////////////////////////////////////////////////////////////////////////////

void onozaki_et_al::loadParamset(QTextStream& inFile)
{
    inFile >> z_0 ;
    inFile >> alpha;
    inFile >> beta;
  
    inFile >> length;
    initialize();
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		onozaki_et_al
// Member function:	saveParamset
// Purpose:		write parameterset into a file
//
// Author:		mhoffman
// Last modified:	27 March 14:39:03 MET 2000
// By:		
//
///////////////////////////////////////////////////////////////////////////////

void onozaki_et_al::saveParamset(QTextStream& outFile)
{
    outFile << z_0 << "\t";
    outFile << alpha << "\t";
    outFile << beta << "\t";

    outFile << length;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		onozaki_et_al
// Member function:	printParamset
// Purpose:		print parameterset on the screen
//
// Author:		mhoffman
// Last modified:	27 March 14:39:03 MET 2000
// By:			
//
///////////////////////////////////////////////////////////////////////////////

void onozaki_et_al::printParamset()
{
    log() << z_0;
    log() << alpha;
    log() << beta;
    
    log() << length;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		onozaki_et_al
// Member function:	saveParamsetWithNames
// Purpose:		print parameterset on the screen
//
// Author:		mhoffman
// Last modified:	27 March 14:39:03 MET 2000
// By:			
//
///////////////////////////////////////////////////////////////////////////////

void onozaki_et_al::saveParamsetWithNames(QTextStream& outputFile)
{
    outputFile << "growthModel:\n\t";
		outputFile << "z_0 =" << z_0 << "\nalpha =" << alpha << "\nbeta=" << beta
							 << "\nlength=" << length;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		onozaki_et_al
// Member functions:	sendModelVar, sendParameters, receiveParameters
// Purpose:		for compatibity reasons only
//
// Author:		mhoffman
// Last modified:	27 March 14:39:03 MET 2000
// By:			
//
///////////////////////////////////////////////////////////////////////////////
qreal* onozaki_et_al::sendModelVar(void)
{ error("macrodyn::onozaki_et_al::sendModelVar is not implemented");
  return NULL;
}
void onozaki_et_al::sendParameters(int& ,qreal** )
{ error("macrodyn::onozaki_et_al::sendParameters is not implemented");
}
void onozaki_et_al::receiveParameters(const QList<qreal>&)
{ error("macrodyn::onozaki_et_al::receiveParameters is not implemented");
}

//*****************************************************************************
// eof
