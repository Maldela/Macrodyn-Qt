/******************************************************************************/
/* Module name:     martinNeu.C                                               */
/* Contents:        Member functions of the class pureExchange                */
/*                                                                            */
/* Modified:        20.10.1999 Marc Mueller                                   */
/*                                                                            */
/******************************************************************************/

#include "martinNeu.h"
#include "../error.h"

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
real* martinNeu::setLabels(char* label) {
    if( !strcmp(label,"xBundle") )
	return &xBundle;
    if( !strcmp(label,"yBundle") )
	return &yBundle;

    if( !strcmp(label,"gamma") )
	return( &gamma);
    if( !strcmp(label,"rho") )
	return( &rho);

    if( !strcmp(label,"g") )
	return( &g_tm1);
    if( !strcmp(label,"thetaEold") )
	return( &thetaEold);
    if( !strcmp(label,"thetaEt") )
	return( &theta_e_t);
    if( !strcmp(label,"thetaEtp1") )
	return( &theta_e_tp1);	
    if( !strcmp(label,"thetaE") )
	return( &theta_e_tp1);	
    if( !strcmp(label,"thetaOld") )
	return( &thetaOld);
    if( !strcmp(label,"thetatm1") )
	return( &theta_tm1);
    if( !strcmp(label,"theta") )
	return( &theta_t);
    if( !strcmp(label,"Err") )
	return( &Err);

    if( !strcmp(label,"p") )
	return( &p_t);
    if( !strcmp(label,"pe") )
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
void martinNeu::loadParamset(ifstream& inFile) {
    inFile >> g_tm1_0 >> theta_e_t_0 >> theta_tm1_0;
    inFile >> gamma >> rho >> lambda;
    inFile >> length;

    initialize();
}

/******************************************************************************/
/* Class name:      martinNeu                                                 */
/* Member function: sendStateSpace                                            */
/* Purpose:         return pointers to the state variables and inform about   */
/*                  the systems dimension                                     */
/******************************************************************************/
void martinNeu::sendStateSpace(int &quantity,const real*** stateSpace) {
    if( *stateSpace )
	delete *stateSpace;
    *stateSpace= new const real* [dimension];
    if( !stateSpace )
      fatalError("martinNeu::sendStateSpace","Can't create state space vector");
    quantity=dimension;
    (*stateSpace)[0]=&g_t;
    (*stateSpace)[1]=&theta_e_tp1;
    (*stateSpace)[2]=&theta_t;
};

/******************************************************************************/
/* Class name:      martinNeu                                                 */
/* Member function: savings                                                   */
/* Purpose:                                                                   */
/******************************************************************************/

real martinNeu::savings(const real& x) {  // irr (martin)
    real r=rho/(rho-1);
    return( 1/exp( cos(10/(1+exp(r*log(x))))));
}
/*
real martinNeu::savings(const real& x) { //cobbD (martin2)
    return( 1-lambda*x);
}

real martinNeu::savings(const real& x) { // ces (martin3)
    real r=rho/(rho-1);
    return( 2-(2+2*lambda*x)/(1+exp(r*log(x))) );
}

real martinNeu::savings(const real& x) { // (martin4)
    return( (1/x) - a * ((x-b)*(x-b)+c));
}
*/

/******************************************************************************/
/* Class name:      martinNeu                                                 */
/* Member function: iteration                                                 */
/* Purpose:         calculate one period of jan´s dynamics                    */
/******************************************************************************/
void martinNeu::iteration(const long& t) {

//    theta_e_tp1 = theta_e_t + g_tm1 * ( theta_tm1 - theta_e_t ) ;
//    real dummy = g_tm1 * theta_tm1 * theta_tm1 ;
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
cout<<"t="<<t<<"  theta_e_tp1="<<theta_e_tp1<<"  theta_tm1="<<theta_tm1<<"  p_t="<< p_t <<endl;
}
