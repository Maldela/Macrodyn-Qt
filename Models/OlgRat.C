
//$Header : $
/******************************************************************************/
/*                                                                            */
/* Module name:     OlgRat.C                                                  */
/* Contents:        Member functions of the class OlgRat                      */
/*                                                                            */
/* Last modified:   Fri Jun 21 15:09:50 MESZ 1996 (Uli Middelberg)            */
/*                                                                            */
/******************************************************************************/

#include "OlgRat.h"
#include "../error.h"


/******************************************************************************/
/*                                                                            */
/* Class name:      OlgRat                                                    */
/* Member function: OlgRat                                                    */
/* Purpose:         constructor / destructor                                  */
/* Last modified:   Fri Jun 21 15:09:50 MESZ 1996 (Uli Middelberg)            */
/*                                                                            */
/******************************************************************************/

OlgRat::OlgRat() : baseModel(1)
{
}

OlgRat::~OlgRat()
{
}


/******************************************************************************/
/*                                                                            */
/* Class name:      OlgRat                                                    */
/* Member function: initialize                                                */
/* Purpose:         initialize the model, define the systems initial state    */
/* Last modified:   Fri Jun 21 15:09:50 MESZ 1996 (Uli Middelberg)            */
/*                                                                            */
/******************************************************************************/

void OlgRat::initialize()
{
    m=m0;
}
    

/******************************************************************************/
/*                                                                            */
/* Class name:      OlgRat                                                    */
/* Member function: sendModelVar                                              */
/* Purpose:         return a pointer to the main model variable               */
/* Last modified:   Fri Jun 21 15:09:50 MESZ 1996 (Uli Middelberg)            */
/*                                                                            */
/******************************************************************************/

qreal* OlgRat::sendModelVar()
{
    return &m;
}


/******************************************************************************/
/*                                                                            */
/* Class name:      OlgRat                                                    */
/* Member function: setLabels                                                 */
/* Purpose:         return a pointer to a variable or a parameter specified   */
/*                  by its name                                               */
/* Last modified:   Fri Jun 21 15:09:50 MESZ 1996 (Uli Middelberg)            */
/*                                                                            */
/******************************************************************************/

qreal* OlgRat::setLabels(char* label)
{
    if( !strcmp(label,"xBundle") )
	return &xBundle;
    if( !strcmp(label,"yBundle") )
	return &yBundle;
    if( !strcmp(label,"g") )
	return( &g);
    if( !strcmp(label,"tau") )
	return( &tau);
    if( !strcmp(label,"s") )
	return( &s);
    if( !strcmp(label,"lambda") )
	return( &lambda);
    if( !strcmp(label,"m") ) 
	return( &m );
    if( !strcmp(label,"m0") ) 
	return( &m0 );

    return NULL;
}


/******************************************************************************/
/*                                                                            */
/* Class name:      OlgRat                                                    */
/* Member function: sendStateSpace                                            */
/* Purpose:         return pointers to the state variables and inform about   */
/*                  the systems dimension                                     */
/* Last modified:   Fri Jun 21 15:09:50 MESZ 1996 (Uli Middelberg)            */
/*                                                                            */
/******************************************************************************/

void OlgRat::sendStateSpace(int &quantity,const qreal*** stateSpace)
{
    if( *stateSpace )
	delete *stateSpace;
    *stateSpace= new const qreal* [dimension];
    if( !stateSpace )
	fatalError("OlgRat::sendStateSpace",
		   "Can't create state space vector");
    quantity=dimension;
    (*stateSpace)[0]=&m;
};


/******************************************************************************/
/*                                                                            */
/* Class name:      OlgRat                                                    */
/* Member function: loadParamset                                              */
/* Purpose:         load a parameterset from a specified input file           */
/* Last modified:   Fri Jun 21 15:09:50 MESZ 1996 (Uli Middelberg)            */
/*                                                                            */
/******************************************************************************/

void OlgRat::loadParamset(ifstream& inFile)
{
    inFile >> m0 >> g >> tau;
    inFile >> s >> lambda;
    inFile >> length;
    
    initialize();
}


/******************************************************************************/
/*                                                                            */
/* Class name:      OlgRat                                                    */
/* Member function: saveParamset                                              */
/* Purpose:         write parameterset into a file                            */
/* Last modified:   Fri Jun 21 15:09:50 MESZ 1996 (Uli Middelberg)            */
/*                                                                            */
/******************************************************************************/

void OlgRat::saveParamset(ofstream& outFile)
{
    outFile << m0 << "\t" << g << "\t" << tau << "\t";
    outFile << s  << "\t" << lambda << "\t";
    outFile << length;
}


/******************************************************************************/
/*                                                                            */
/* Class name:      OlgRat                                                    */
/* Member function: printParamset                                             */
/* Purpose:         print parameterset on the screen                          */
/* Last modified:   Fri Jun 21 15:09:50 MESZ 1996 (Uli Middelberg)            */
/*                                                                            */
/******************************************************************************/

void OlgRat::printParamset()
{
    log() << m0 << "\t" << g << "\t" << tau << endl;
    log() << s << "\t" << lambda << endl;
    log() << length << endl;
}


/******************************************************************************/
/*                                                                            */
/* Class name:      OlgRat                                                    */
/* Member function: sendParameters                                            */
/* Purpose:         write all parameters into an array and return the numbers */
/*                  of parameters                                             */
/* Last modified:   Fri Jun 21 15:09:50 MESZ 1996 (Uli Middelberg)            */
/*                                                                            */
/******************************************************************************/

void OlgRat::sendParameters(int& amount,qreal** parameters)
{
    if( *parameters )
	delete *parameters;
    amount=6;
    *parameters=new qreal[amount];
    if( !parameters )
	fatalError("OlgRat::sendParameters",
		   "Can't create array for parameters");
    (*parameters[0])=m0;
    (*parameters[1])=g;
    (*parameters[2])=tau;
    (*parameters[3])=s;
    (*parameters[4])=lambda;
    (*parameters[5])=length;
}


/******************************************************************************/
/*                                                                            */
/* Class name:      OlgRat                                                    */
/* Member function: receiveParameters                                         */
/* Purpose:         receive parameter values                                  */
/* Last modified:   Fri Jun 21 15:09:50 MESZ 1996 (Uli Middelberg)            */
/*                                                                            */
/******************************************************************************/

void OlgRat::receiveParameters(const qreal* parameters)
{
    m0=parameters[0];
    g=parameters[1];
    tau=parameters[2];
    s=parameters[3];
    lambda=parameters[4];
    length=(long)(parameters[5]);
}

/******************************************************************************/
/*                                                                            */
/* Class name:      OlgRat                                                    */
/* Member function: iteration                                                 */
/* Purpose:         perforn one iteration                                     */
/* Last modified:   Fri Jun 21 15:09:50 MESZ 1996 (Uli Middelberg)            */
/*                                                                            */
/******************************************************************************/


void OlgRat::iteration(const long&)
{
    qreal mg=m+g;
    qreal min;
    qreal max;
    
    if( s <= mg ) {
      min=s; max=mg;
    } else {
      min=mg; max=s;
    }
   
    m = (1-tau) * min * exp( (-lambda) * (mg-s) / max );
}

void OlgRat2::iteration(const long&)
{
    qreal mg=m+g;
    qreal min;
    qreal max;
    
    if( s <= mg ) {
      min=s; max=mg;
    } else {
      min=mg; max=s;
    }
   
    m = (1-tau) * 2 * min * exp( (-lambda) * (mg-s) / max );
}


//eof
