//$Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/nicole.C,v 1.1 2000/08/31 16:07:33 mmeyer Exp $
/******************************************************************************/
/*                                                                            */
/* Module name:  nicole.C                                                     */
/* Contents:     Member functions of the class nicole                         */
/*                Standardversion of Cars Hommes                              */
/* Modified:     16.04.1997 (Marc Mueller)                                    */
/* Last Modified:21.04.1997 (Marc Mueller)                                    */
/*                                                                            */
/******************************************************************************/

#include "nicole.h"             
#include "../error.h"
/******************************************************************************/
/*                                                                            */
/* Class name:      nicole                                                    */
/* Member function: nicole                                                    */
/* Purpose:         constructor                                               */
/* Last modified:   21.04.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
nicole::nicole() : baseModel(1)
{
     zvar=NULL;
}
/******************************************************************************/
/*                                                                            */
/* Class name:      nicole                                                    */
/* Member function: ~nicole                                                   */
/* Purpose:         destructor                                                */
/* Last modified:   21.04.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
nicole::~nicole()
{
     if( zvar )
     	 delete zvar;
}
/******************************************************************************/
/*                                                                            */
/* Class name:      nicole                                                    */
/* Member function: initialize                                                */
/* Purpose:         initialize the model, set the initial state               */
/* Last modified:   21.04.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void nicole::initialize()
{
   //zvar_expr= ;
   zvar = new rand_var( this, "ranf", "1[0,1];" );
   d = d0;
}
/******************************************************************************/
/*                                                                            */
/* Class name:      nicole                                                    */
/* Member function: loadParamset                                              */
/* Purpose:         load a parameterset from a specified input file           */
/* Last modified:   21.04.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void nicole::loadParamset(QTextStream& inputFile)
{
    inputFile >> pi >> dl >> dh >> d0 >> e >> alpha >> xhat >> theta >> R;
    inputFile >> length ;

    initialize();
}
/******************************************************************************/
/*                                                                            */
/* Class name:      nicole                                                    */
/* Member function: saveParamset                                              */
/* Purpose:         write parameterset into a file                            */
/* Last modified:   21.04.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void nicole::saveParamset(QTextStream& outputFile)
{
    outputFile << pi << "\t" << dl << "\t" << dh << "\t" << d0 << "\t" ;
    outputFile << e << "\t" << alpha << "\t" << xhat << "\t";
    outputFile << theta << "\t" << R << "\t" << length << "\t";
}
/******************************************************************************/
/*                                                                            */
/* Class name:      nicole                                                    */
/* Member function: saveParamsetWithNames                                     */
/* Purpose:         add  parameterset to printfile                            */
/* Last modified:   21.04.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void nicole::saveParamsetWithNames(QTextStream& outputFile)
{
    outputFile << "\nModel nicole\n" << "pi = " << pi << "\tdl = " << dl;
    outputFile << "\tdh = " << dh << "\td0 = " << d0 << "\te = " << e << "\talpha = " << alpha ;
    outputFile << "\txhat = " << xhat << "\ttheta = " << theta  << "\tR = "<< R;
    outputFile << "\tlength = "<< length;
}
/******************************************************************************/
/*                                                                            */
/* Class name:      nicole                                                    */
/* Member function: printParamset                                             */
/* Purpose:         print parameterset on the screen                          */
/* Last modified:   21.04.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void nicole::printParamset()
{
    log() << pi << "\t" << dl << "\t" << dh << "\t" << d0 << "\t" << e  << "\t" << alpha ;
    log() << "\t" << xhat << "\t";
    log() << theta << "\t" << R << "\n" << length;
}
/******************************************************************************/
/*                                                                            */
/* Class name:      nicole                                                    */
/* Member function: sendParameters                                            */
/* Purpose:         unused                                                    */
/*                                                                            */
/******************************************************************************/
void nicole::sendParameters(int& amount,qreal** parameters)
{
}
/******************************************************************************/
/*                                                                            */
/* Class name:      nicole                                                    */
/* Member function: receiveParameters                                         */
/* Purpose:         unused                                                    */
/*                                                                            */
/******************************************************************************/
void nicole::receiveParameters(const QList<qreal>& parameters)
{
}
/******************************************************************************/
/*                                                                            */
/* Class name:      nicole                                                    */
/* Member function: setlabels                                                 */
/* Purpose:         returns a pointer to a variable or parameter of the system*/
/*                  that is specified by its name                             */
/* Last modified:   21.04.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
qreal* nicole::setLabels(const QString& name)
{
    if (name == "xBundle")
	return &xBundle;
    if (name == "yBundle")
	return &yBundle;

    if (name == "pi")
        return( &pi );
    if (name == "dl")
        return( &dl );
    if (name == "dh")
        return( &dh );
    if (name == "d0")
        return( &d0 );

    if (name == "e")
        return( &e );
    if (name == "alpha")
        return( &alpha );
    if (name == "xhat")
        return( &xhat );
    if (name == "theta")
        return( &theta );
    if (name == "R")
        return( &R );
 
    if (name == "p")
        return( &p );
    if (name == "d")
        return( &d );

    return( NULL );
}
/******************************************************************************/
/*                                                                            */
/* Class name:      nicole                                                    */
/* Member function: sendModelVar                                              */
/* Purpose:         unused                                                    */
/*                                                                            */
/******************************************************************************/
qreal* nicole::sendModelVar()
{
	return( &p );
}
/******************************************************************************/
/*                                                                            */
/* Class name:      nicole                                                    */
/* Member function: sendStateSpace                                            */
/* Purpose:         unused                                                    */
/*                  returns the dimension of the system for rho=0             */
/*                                                                            */
/******************************************************************************/
void nicole::sendStateSpace(int &quantity, QList<qreal *> *stateSpace)
{
    log() << "nicole::sendStateSpace is not implemented";
}
/******************************************************************************/
/*                                                                            */
/* Class name:      nicole                                                    */
/* Member function: getD                                                      */
/* Purpose:         get new interest rate                                     */
/* Last modified:   21.04.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void nicole::getD()
{
   qreal rand;
   
   rand=zvar->dice();
   if( d == dl )
   	{
   	if( rand <= pi )
   		d = dl ;
   	else	d = dh ;
   	}
   else
   	{
   	if( rand <= pi )
   		d = dh ;
   	else	d = dl ;
   	} 
}
/******************************************************************************/
/*                                                                            */
/* Class name:      nicole                                                    */
/* Member function: dynamics                                                  */
/* Purpose:         compute modifications for the next period                 */
/* Last modified:   21.04.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void nicole::dynamics()
{
	p = qMin((e/xhat),((theta*(1+alpha*d*xhat)+d)/(R*(alpha*theta*xhat+1))));
}
/******************************************************************************/
/*                                                                            */
/* Class name:      nicole                                                    */
/* Member function: iteration                                                 */
/* Purpose:         describes one step of the dynamical system                */
/* Last modified:   21.04.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void nicole::iteration(const qint64& t)
{
	getD();
	dynamics();
}
