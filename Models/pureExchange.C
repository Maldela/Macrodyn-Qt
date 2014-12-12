//$Header: pureExchange.C

/******************************************************************************/
/*                                                                            */
/* Module name:  pureExchange.C                                               */
/* Contents:     Member functions of the class pureExchange                   */
/*                                                                            */
/* Last Modified: 12.04.2000 (Mathias Hoffmann)                               */
/*                                                   						  */
/******************************************************************************/

#include "pureExchange.h"             
#include "../error.h"

/******************************************************************************/
/*                                                                            */
/* Class name:      pureExchange                                   		      */
/* Member function: 3 different savings functions       		        	  */
/* Purpose:         definition of the savings functions                       */
/* Last modified:  12.04.2000 (Mathias Hoffmann)                              */
/*                                                                            */
/******************************************************************************/

static qreal sf_1 (qreal theta_e, qreal w1, qreal w2, qreal delta, qreal rho)
{

    qreal theta_aut = delta*w1/w2;

    if (0 <= theta_e && theta_e < theta_aut)
		return 1/delta*(w1 - theta_e*w2);
	else return 0;
}

static qreal sf_2 (qreal theta_e, qreal w1, qreal w2, qreal delta, qreal rho)
{
	return w1-(w1+w2*theta_e) / (1+exp ((rho/(rho-1))*log(theta_e/delta))    ) ;
}

static qreal sf_3 (qreal theta_e, qreal w1, qreal w2, qreal delta, qreal rho)
{

 	return 1/(exp(cos(10/(1+exp(rho/(rho-1)*log(theta_e))))));
}


/******************************************************************************/
/*                                                                            */
/* Class name:      pureExchange                                   		      */
/* Member function: sf_init                   	   							  */
/* Purpose:         initialize the savings function                           */
/* Last modified: 12.04.2000 (Mathias Hoffmann)    						   	  */
/*                                                                            */
/******************************************************************************/

void pureExchange::sf_init()
{ 

  switch (int(sf_type)) {
    case 0 :
     	savingsFunc = sf_1;
    	break;
    case 1 :
      	savingsFunc = sf_2;
    	break;
    case 2 :
    	savingsFunc = sf_3;
   		break;

    default :
        error("macrodyn::pureExchange::sf_init: sf type %d not yet implemented : ", QString::number(sf_type));
  }
}


/******************************************************************************/
/*                                                                            */
/* Class name:      pureExchange                                              */
/* Member function: pureExchange                                              */
/* Purpose:         constructor                                               */
/* Last modified: 12.04.2000 (Mathias Hoffmann)                               */
/*                                                                            */
/******************************************************************************/

pureExchange::pureExchange() : baseModel(3)
{
	savingsFunc = NULL;
	theE_Old = 0;
	theE_You = 0; 
	pt = 0;
	g_tm1 = 0;


    
}

/******************************************************************************/
/*                                                                            */
/* Class name:      pureExchange                                              */
/* Member function: ~pureExchange                                             */
/* Purpose:         destructor                                                */
/* Last modified:  12.04.2000 (Mathias Hoffmann)    		                  */
/*                                                                            */
/******************************************************************************/

pureExchange::~pureExchange()
{

}

/******************************************************************************/
/*                                                                            */
/* Class name:      pureExchange                                              */
/* Member function: initialize                                                */
/* Purpose:         initialize the model, set the initial state               */
/* Last modified: 12.04.2000 (Mathias Hoffmann)  							  */
/*                                                                            */
/******************************************************************************/

void pureExchange::initialize()
{
	theE_Old = theta_e_null;

	theta_tm1 = gamma * (*savingsFunc)(theta_e_m1,w1,w2,delta,rho)
				/ (*savingsFunc)(theta_e_null,w1,w2,delta,rho);
	g_tm1 = g0;
	p_tm1 = p0;

	err=0;

	expInflRateYoung();
}

/******************************************************************************/
/*                                                                            */
/* Class name:      pureExchange                                              */
/* Member function: expInflRateYoung                                          */
/* Purpose:         computes the expected inflation rate 		              */
/* Last modified:   12.04.2000 (Mathias Hoffmann)                             */
/*                                                                            */
/******************************************************************************/

void pureExchange::expInflRateYoung()
{
    qreal dummy;
	theE_You = theE_Old + g_tm1 * ( theta_tm1 - theE_Old );
    dummy = g_tm1 * theta_tm1 * theta_tm1;
    g_tm1 = dummy / (dummy + 1.0);

}




/******************************************************************************/
/*                                                                            */
/* Class name:       pureExchange                                             */
/* Member function: iteration                                                 */
/* Purpose:         describes one step of the dynamical system                */
/* Last modified:   12.04.2000 (Mathias Hoffmann) 							  */
/*                                                                            */
/******************************************************************************/

void pureExchange::iteration(const qint64& t)
{

theta_t = gamma *  (*savingsFunc)(theE_Old, w1, w2, delta, rho)
					/ (*savingsFunc)(theE_You, w1, w2, delta, rho);

	pt = theta_t * p_tm1;
	err = theta_t - theE_Old;

// save values for plotting
	thetaOld = theta_tm1;
	theE_Old2 = theE_Old;
	theE_You2 = theE_You;

/* shifting for the next period:
   not allowed when job 83 is running */

if (job_type != 83) 
	{ theE_Old = theE_You;
	  theta_tm1 = theta_t;
	  p_tm1 = pt;
	  expInflRateYoung();
	}	


}

/******************************************************************************/
/*                                                                            */
/* Class name:      pureExchange                                              */
/* Member function: setlabels                                                 */
/* Purpose:         returns a pointer to a variable or parameter of the system*/
/*                  that is specified by its name                             */
/* Last modified:    12.04.2000 (Mathias Hoffmann)                            */
/*                                                                            */
/******************************************************************************/

qreal* pureExchange::setLabels(const QString& name)
{
    if (name == "xBundle")
		return &xBundle;
    if (name == "yBundle")
		return &yBundle;
    if (name == "theta_t")
        return( &theta_t );
    if (name == "theE_Old")
        return( &theE_Old);
    if (name == "theE_You")
        return( &theE_You );

    if (name == "pt")
		return( &pt );
    if (name == "err")
		return( &err );
 	if (name == "theE_Old2")
		return( &theE_Old2 );
  	if (name == "theE_You2")
		return( &theE_You2 );

    return( NULL );
}



/******************************************************************************/
/*                                                                            */
/* Class name:       pureExchange                                             */
/* Member function: sendModelVar                                              */
/* Purpose:         returns a pointer to the main model var.  				  */
/* Last modified:   12.04.2000 (Mathias Hoffmann)                             */
/*                                                                            */
/******************************************************************************/

qreal* pureExchange::sendModelVar()
{
   return &theta_t;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      pureExchange                                              */
/* Member function: sendStateSpace                                            */
/* Purpose:         returns pointers to the exp.Infl.rate for period t+1,	  */
/*					infl.rate for period t and g(t-1)						  */
/*                  returns the dimension of the system			              */
/* Last modified:   12.04.2000 (Mathias Hoffmann)                             */
/*                                                                            */
/******************************************************************************/

void pureExchange::sendStateSpace(int &quantity,QList<qreal *> *stateSpace)
{
    log() << "sendStateSpace";
    stateSpace->clear();
    quantity = dimension;
    *stateSpace << &g_tm1;
	*stateSpace << &theE_You;
	*stateSpace << &theta_t;
}
    
/******************************************************************************/
/*                                                                            */
/* Class name:       pureExchange                                             */
/* Member function: loadParamset                                              */
/* Purpose:         load a parameterset from a specified input file           */
/* Last modified:  12.04.2000 (Mathias Hoffmann)                              */
/*                                                                            */
/******************************************************************************/

void pureExchange::loadParamset(QTextStream& inputFile)
{
    inputFile 	>> sf_type >> theta_e_m1 >> theta_e_null >> gamma >> p0 
				>> w1 >> w2 >> g0 >> delta >> rho >> job_type;
    inputFile >> length;

sf_init();


}

/******************************************************************************/
/*                                                                            */
/* Class name:      pureExchange                                              */
/* Member function: saveParamset                                              */
/* Purpose:         write parameterset into a file                            */
/* Last modified:   12.04.2000 (Mathias Hoffmann)                             */
/*                                                                            */
/******************************************************************************/

void pureExchange::saveParamset(QTextStream& outputFile)
{
    outputFile 	<< sf_type << theta_e_m1 << theta_e_null << gamma << p0 
				<< w1 << w2 << g0 << delta << rho << job_type;
	outputFile << length;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      pureExchange                                              */
/* Member function: printParamset                                             */
/* Purpose:         print parameterset on the screen                          */
/* Last modified:   12.04.2000 (Mathias Hoffmann)                             */
/*                                                                            */
/******************************************************************************/

void pureExchange::printParamset()
{
    log() << sf_type;
    log() << theta_e_m1 << "\t" << theta_e_null;
    log() << gamma << "\t" << p0;
    log() << w1 << "\t" << w2;
    log() << g0 << "\t" << delta;
    log() << rho << "\t" << job_type;
    log() << length;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      pureExchange                                              */
/* Member function: sendParameters                                            */
/* Purpose:         write all parameters into an array and return the numbers */
/*                  of parameters                                             */
/* Last modified:   12.04.2000 (Mathias Hoffmann)                             */
/*                                                                            */
/******************************************************************************/

void pureExchange::sendParameters(int& amount,qreal** parameters)
{
    if( *parameters )
	delete *parameters;
    amount=12;
    *parameters=new qreal[amount];
    if( !(*parameters) )
	fatalError("pureExchange::sendParameters",
		   "Can't create array for parameters");
    (*parameters)[0]=sf_type;
    (*parameters)[1]=theta_e_m1;
    (*parameters)[2]=theta_e_null;
    (*parameters)[3]=gamma;
    (*parameters)[4]=p0;
    (*parameters)[5]=w1;
	(*parameters)[6]=w2;
	(*parameters)[7]=g0;
	(*parameters)[8]=delta;
	(*parameters)[9]=rho;
	(*parameters)[10]=job_type;
	(*parameters)[11]=length;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      pureExchange                                              */
/* Member function: receiveParameters                                         */
/* Purpose:         receive parameter values                                  */
/* Last modified:   12.04.2000 (Mathias Hoffmann)                             */
/*                                                                            */
/******************************************************************************/

void pureExchange::receiveParameters(const QList<qreal>& parameters)
{
    sf_type=parameters[0];
    theta_e_m1=parameters[1];
    theta_e_null=parameters[2];
    gamma=(parameters[3]);
	p0=(parameters[4]);
	w1=(parameters[5]);
	w2=(parameters[6]);
	g0=(parameters[7]);
    delta=(parameters[8]);
	rho=(parameters[9]);
	job_type=(parameters[10]);
	length=(qint64)(parameters[11]);
}

// eof
