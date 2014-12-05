//$Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/models2D.C,v 1.1 2000/08/31 16:06:52 mmeyer Exp $
/******************************************************************************/
/*                                                                            */
/* Module name:  models2D.C                                                   */
/* Contents:     Member functions of the class models2D                       */
/*                                                                            */
/* Last modified: 02.03.1995 (Markus Lohmann)                                 */
/* Modified:      24.02.95 -- sendParameters and receiveParameters added      */
/*                02.03.95 -- abstract class models2D introduced              */
/*                                                                            */
/******************************************************************************/

#include "models2D.h"
#include "../error.h"
/******************************************************************************/
/*                                                                            */
/* Class name:      models2D                                                  */
/* Member function: models2D                                                  */
/* Purpose:         constructor                                               */
/* Last modified:   20.02.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

models2D::models2D() : baseModel(2)
{
}

models2D::~models2D()
{
}
/******************************************************************************/
/*                                                                            */
/* Class name:      models2D                                                  */
/* Member function: initialize                                                */
/* Purpose:         initialize the model, define the systems initial state    */
/* Last modified:   20.02.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void models2D::initialize()
{
    x=x0;
    y=y0;
}
    
/******************************************************************************/
/*                                                                            */
/* Class name:      models2D                                                  */
/* Member function: sendModelVar                                              */
/* Purpose:         return a pointer to the main model variable               */
/* Last modified:   20.02.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

qreal* models2D::sendModelVar()
{
    return &x;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      models2D                                                  */
/* Member function: setLabels                                                 */
/* Purpose:         return a pointer to a variable or a parameter specified   */
/*                  by its name                                               */
/* Last modified:   20.02.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

qreal* models2D::setLabels(const QString& label)
{
    if (label == "xBundle")
	return &xBundle;
    if (label == "yBundle")
	return &yBundle;
    if (label == "alpha")
	return( &alpha);
    if (label == "beta")
	return( &beta);
    if (label == "x")
	return( &x);
    if (label == "x0")
	return( &x0 );
    if (label == "y")
	return( &y);
    if (label == "y0")
	return( &y0 );

    return NULL;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      models2D                                                  */
/* Member function: sendStateSpace                                            */
/* Purpose:         return pointers to the state variables and inform about   */
/*                  the systems dimension                                     */
/* Last modified:   20.02.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void models2D::sendStateSpace(int &quantity,const qreal*** stateSpace)
{
    if( *stateSpace )
	delete *stateSpace;
    *stateSpace= new const qreal* [dimension];
    if( !stateSpace )
	fatalError("models2D::sendStateSpace",
		   "Can't create state space vector");
    quantity=dimension;
    (*stateSpace)[0]=&x;
    (*stateSpace)[1]=&y;
};

/******************************************************************************/
/*                                                                            */
/* Class name:      models2D                                                  */
/* Member function: loadParamset                                              */
/* Purpose:         load a parameterset from a specified input file           */
/* Last modified:   07.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void models2D::loadParamset(QDataStream& inFile)
{
    inFile >> x0 >> y0;
    inFile >> alpha >> beta;
    inFile >> length;
    
    initialize();
}

/******************************************************************************/
/*                                                                            */
/* Class name:      models2D                                                  */
/* Member function: saveParamset                                              */
/* Purpose:         write parameterset into a file                            */
/* Last modified:   20.02.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void models2D::saveParamset(QDataStream& outFile)
{
    outFile << x0 << "\t" << y0 << "\t";
    outFile << alpha  << "\t" << beta << "\t";
    outFile << length;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      models2D                                                  */
/* Member function: printParamset                                             */
/* Purpose:         print parameterset on the screen                          */
/* Last modified:   20.02.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void models2D::printParamset()
{
    log() << x0 << "\t" << y0 << "\n";
    log() << alpha << "\t" << beta << "\n";
    log() << length << "\n";
}

/******************************************************************************/
/*                                                                            */
/* Class name:      models2D                                                  */
/* Member function: sendParameters                                            */
/* Purpose:         write all parameters into an array and return the numbers */
/*                  of parameters                                             */
/* Last modified:   24.02.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void models2D::sendParameters(int& amount,qreal** parameters)
{
    if( *parameters )
	delete *parameters;
    amount=5;
    *parameters=new qreal[amount];
    if( !parameters )
	fatalError("models2D::sendParameters",
		   "Can't create array for parameters");
    (*parameters[0])=x0;
    (*parameters[1])=y0;
    (*parameters[2])=alpha;
    (*parameters[3])=beta;
    (*parameters[4])=length;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      models2D                                                  */
/* Member function: receiveParameters                                         */
/* Purpose:         receive parameter values                                  */
/* Last modified:   24.02.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void models2D::receiveParameters(const qreal* parameters)
{
    x0=parameters[0];
    y0=parameters[1];
    alpha=parameters[2];
    beta=parameters[3];
    length=(qint64)(parameters[4]);
}

