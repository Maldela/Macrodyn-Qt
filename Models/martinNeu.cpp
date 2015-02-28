/******************************************************************************/
/* Module name:     martinNeu.C                                               */
/* Contents:        Member functions of the class pureExchange                */
/*                                                                            */
/* Modified:        20.10.1999 Marc Mueller                                   */
/*                                                                            */
/******************************************************************************/

#include "martinNeu.h"
#include "../logger.h"

/******************************************************************************/
/* Class name:      martinNeu                                                 */
/* Member function: martinNeu                                                 */
/* Purpose:         con-, destructor                                          */
/******************************************************************************/
martinNeu::martinNeu() : baseModel(3) {
//	zvar = NULL;
}
/*
martinNeu::~martinNeu() {
	if( zvar ) delete zvar;
}
*/
/******************************************************************************/
/* Class name:      martinNeu                                                    */
/* Member function: setLabels                                                 */
/* Purpose:         return a pointer to a variable or a parameter specified   */
/*                  by its name                                               */
/******************************************************************************/
qreal* martinNeu::setLabels(const QString& label) {
    if (label == "xBundle")
	return &xBundle;
    if (label == "yBundle")
	return &yBundle;

    if (label == "gamma")
	return( &gamma);
    if (label == "rho")
	return( &rho);

    if (label == "g")
	return( &g_tm1);
    if (label == "thetaEold")
	return( &thetaEold);
    if (label == "thetaEt")
	return( &theta_e_t);
    if (label == "thetaEtp1")
	return( &theta_e_tp1);	
    if (label == "thetaE")
	return( &theta_e_tp1);	
    if (label == "thetaOld")
	return( &thetaOld);
    if (label == "thetatm1")
	return( &theta_tm1);
    if (label == "theta")
	return( &theta_t);
    if (label == "Err")
	return( &Err);

    if (label == "p")
	return( &p_t);
    if (label == "pe")
	return( &p_e_tp1);

    return NULL;
}

/******************************************************************************/
/* Class name:      martinNeu                                                 */
/* Member function: initialize                                                */
/* Purpose:         initialize the model, define the systems initial state    */
/******************************************************************************/
void martinNeu::initialize() {
    g_tm1 = g_tm1_0 ;
    theta_e_t = theta_e_t_0 ;
    theta_e_tp1 = theta_e_t_0 ;
    theta_tm1 = theta_tm1_0 ;

    p_t = 1;
    p_tm1 = 1;
    p_e_tp1 = 1;

    if ( zvar != NULL ) delete zvar;
    zvar = new rand_var( this, "ranf", "1[0,25];" );
    if( !(zvar) )
	   fatalError("martinNeu::initialize","can't create new rand_var");
}

/******************************************************************************/
/* Class name:      martinNeu                                                 */
/* Member function: loadParamset                                              */
/* Purpose:         load a parameterset from a specified input file           */
/******************************************************************************/
void martinNeu::loadParamset(QTextStream& inFile) {
    inFile >> g_tm1_0 >> theta_e_t_0 >> theta_tm1_0;
    inFile >> gamma >> rho >> lambda;
    inFile >> length;

    initialize();
}

void martinNeu::receiveParameters(const QList<qreal> &parameters)
{
    if (parameters.size() != 7) log() << "Wrong number of parameters!";
    else
    {
        g_tm1_0 = parameters.at(0);
        theta_e_t_0 = parameters.at(1);
        theta_tm1_0 = parameters.at(2);
        gamma = parameters.at(3);
        rho = parameters.at(4);
        lambda = parameters.at(5);
        length = parameters.at(6);

        initialize();
    }
}

/******************************************************************************/
/* Class name:      martinNeu                                                 */
/* Member function: sendStateSpace                                            */
/* Purpose:         return pointers to the state variables and inform about   */
/*                  the systems dimension                                     */
/******************************************************************************/
void martinNeu::sendStateSpace(int &quantity,QList<qreal *> *stateSpace) {
    stateSpace->clear();
    quantity = dimension;
    *stateSpace << &g_t;
    *stateSpace << &theta_e_tp1;
    *stateSpace << &theta_t;
}

/******************************************************************************/
/* Class name:      martinNeu                                                 */
/* Member function: savings                                                   */
/* Purpose:                                                                   */
/******************************************************************************/

qreal martinNeu::savings(const qreal& x) {  // irr (martin)
    qreal r=rho/(rho-1);
    return( 1/exp( cos(10/(1+exp(r*log(x))))));
}
/*
qreal martinNeu::savings(const qreal& x) { //cobbD (martin2)
    return( 1-lambda*x);
}

qreal martinNeu::savings(const qreal& x) { // ces (martin3)
    qreal r=rho/(rho-1);
    return( 2-(2+2*lambda*x)/(1+exp(r*log(x))) );
}

qreal martinNeu::savings(const qreal& x) { // (martin4)
    return( (1/x) - a * ((x-b)*(x-b)+c));
}
*/

/******************************************************************************/
/* Class name:      martinNeu                                                 */
/* Member function: iteration                                                 */
/* Purpose:         calculate one period of jan's dynamics                    */
/******************************************************************************/
void martinNeu::iteration(const qint64& t) {

//    theta_e_tp1 = theta_e_t + g_tm1 * ( theta_tm1 - theta_e_t ) ;
//    qreal dummy = g_tm1 * theta_tm1 * theta_tm1 ;
//    g_t = dummy / (dummy + 1.0) ;

//    theta_e_tp1=zvar->dice(); 


theta_e_tp1 = p_e_tp1 / p_t;
    
    theta_t = (gamma*savings(theta_e_t))/savings(theta_e_tp1) ;

    p_t = theta_t * p_tm1 ;

    Err = theta_t - theta_e_t ;

    // save values
    thetaOld = theta_tm1;
    thetaEold = theta_e_t;

    // dynamic
//    g_tm1 = g_t ;
    theta_e_t = theta_e_tp1 ;
    theta_tm1 = theta_t ;
    p_tm1 = p_t ;
    log() <<"t="<<t<<"  theta_e_tp1="<<theta_e_tp1<<"  theta_tm1="<<theta_tm1<<"  p_t="<< p_t;
}
