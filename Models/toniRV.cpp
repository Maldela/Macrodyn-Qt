//$Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/toniRV.C,v 1.1 2000/08/31 16:10:45 mmeyer Exp $
/******************************************************************************/
/*                                                                            */
/* Module name:  toniRV.C                                                     */
/* Contents:     Member functions of the class toniRV                         */
/*                Standardversion of Cars Hommes                              */
/* Modified:     28.01.1997 (Marc Mueller)                                    */
/* Last Modified:20.02.1997 (Marc Mueller)                                    */
/*                                                                            */
/******************************************************************************/

#include "toniRV.h"             
#include "../logger.h"
/******************************************************************************/
/*                                                                            */
/* Class name:      toniRV                                                    */
/* Member function: toniRV                                                    */
/* Purpose:         constructor                                               */
/* Last modified:   30.01.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
toniRV::toniRV() : baseModel(1)
{
    pp0=NULL;
    pp=NULL;
    vv=NULL;
}
/******************************************************************************/
/*                                                                            */
/* Class name:      toniRV                                                    */
/* Member function: ~toniRV                                                   */
/* Purpose:         destructor                                                */
/* Last modified:   03.02.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
toniRV::~toniRV()
{
    if( pp0 )
	delete [] pp0;
    if( pp )
	delete [] pp;
    if( vv )
	delete [] vv;
}
/******************************************************************************/
/*                                                                            */
/* Class name:      toniRV                                                    */
/* Member function: initialize                                                */
/* Purpose:         initialize the model, set the initial state               */
/* Last modified:   29.11.1995 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void toniRV::initialize()
{
	for(int k=0;k<=L;k++) pp[k]=pp0[k];

    qreal wsum=0;
    for (int k=0;k<=L;k++)
	   {
		wsum+=hoch(w,k);
        //log() << "k=" << k  << " wsum=" << wsum 
	   }
    //qreal ko=0;
	for(int i=0;i<=L;i++)
	   {
		vv[i]=hoch(w,i)/wsum;
		//ko+=vv[i];
        //log() << "i=" << i  << " w_i=" << vv[i] << " ko="<< ko 
	   }
}
/******************************************************************************/
/*                                                                            */
/* Class name:      toniRV                                                    */
/* Member function: loadParamset                                              */
/* Purpose:         load a parameterset from a specified input file           */
/* Last modified:   30.01.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void toniRV::loadParamset(QTextStream& inputFile)
{

    inputFile >> a >> b >> alpha >> w >> L0;
    inputFile >> length ;

    L=(int)L0;
    if( pp0 )
	delete [] pp0;
        pp0 = new qreal[L+1];
    if( !pp0 )
	fatalError("toniRV::loadParamset","Can't create initprice vector");
 
    for(int k=0;k<=L;k++)
        {
    	inputFile >> pp0[k] ;
        //log() << "L=" << L << "k=" << k  << "po=" << pp0[k];
    	}

    if( pp )
	delete [] pp;
        pp = new qreal[L+1];
    if( !pp )
	fatalError("toniRV::loadParamset","Can't create price vector");
    if( vv )
	delete [] vv;
        vv = new qreal[L+1];
    if( !vv )
	fatalError("toniRV::loadParamset","Can't create weight vector");
    	
    initialize();
}
/******************************************************************************/
/*                                                                            */
/* Class name:      toniRV                                                    */
/* Member function: saveParamset                                              */
/* Purpose:         write parameterset into a file                            */
/* Last modified:   30.01.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void toniRV::saveParamset(QTextStream& outputFile)
{
    outputFile << a << "\t" << b << "\t" << alpha << "\t" << w << "\t";
    outputFile << L0 << "\t"<< length << "\t";
    for(int k=0;k<=L;k++)
        outputFile << pp0[k] << "\t";
    }
/******************************************************************************/
/*                                                                            */
/* Class name:      toniRV                                                    */
/* Member function: saveParamsetWithNames                                     */
/* Purpose:         add  parameterset to printfile                            */
/* Last modified:   20.02.1995 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void toniRV::saveParamsetWithNames(QTextStream& outputFile)
{
    outputFile << "\nModel ToniRV\n";
    outputFile << "a = " << a << "\tb = " << b << "\talpha = " << alpha ;
    outputFile << "\tL = " << L0 << "\tlength = "<< length << "\t";
    for(int k=0;k<=L;k++)
        outputFile << "p0_" << k << " = " << pp0[k] << "\t";
    for (int k=0;k<=L;k++)
        outputFile << "p_t-" << k << " = " << pp[k] << "\t";
}
/******************************************************************************/
/*                                                                            */
/* Class name:      toniRV                                                    */
/* Member function: printParamset                                             */
/* Purpose:         print parameterset on the screen                          */
/* Last modified:   30.01.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void toniRV::printParamset()
{
    log() << a  << "\t" << b << "\t" << alpha << "\t" << w << "\t" << L0;
    log() << length;
    for(int k=0;k<=L;k++)
        log() << pp0[k] << "\t";
}
/******************************************************************************/
/*                                                                            */
/* Class name:      toniRV                                                    */
/* Member function: sendParameters                                            */
/* Purpose:         write all parameters into an array and return the numbers */
/*                  of parameters                                             */
/* Last modified:   30.01.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void toniRV::sendParameters(int& amount,qreal** parameters)
{
    if( *parameters )
	delete *parameters;
    amount=L+7;
    *parameters=new qreal[amount];
    if( !(*parameters) )
	fatalError("toniRV::sendParameters",
		   "Can't create array for parameters");
    (*parameters)[0]=a;
    (*parameters)[1]=b;
    (*parameters)[2]=alpha;
    (*parameters)[3]=w;
    (*parameters)[4]=(qreal)L;
    (*parameters)[5]=length;
    for(int k=6;k<=(L+6);k++)
        (*parameters)[k]=pp0[k];
}
/******************************************************************************/
/*                                                                            */
/* Class name:      toniRV                                                    */
/* Member function: receiveParameters                                         */
/* Purpose:         receive parameter values                                  */
/* Last modified:   30.01.1995 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void toniRV::receiveParameters(const QList<qreal>& parameters)
{
    a=parameters[0];
    b=parameters[1];
    alpha=parameters[2];
    w=parameters[3];
    L=(int)parameters[4];
    length=parameters[5];
    for(int k=6;k<=(L+6);k++)
       pp0[k]=parameters[k];
}
/******************************************************************************/
/*                                                                            */
/* Class name:      toniRV                                                    */
/* Member function: setlabels                                                 */
/* Purpose:         returns a pointer to a variable or parameter of the system*/
/*                  that is specified by its name                             */
/* Last modified:   09.03.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
qreal* toniRV::setLabels(const QString& name)
{
    if (name == "xBundle")
	return &xBundle;
    if (name == "yBundle")
	return &yBundle;

    if (name == "a")
        return( &a );
    if (name == "b")
        return( &b );
    if (name == "alpha")
        return( &alpha );
    if (name == "w")
        return( &w );
    if (name == "L")
        return( &L0 );

    if (name == "v")
        return( &v );
    if (name == "s")
        return( &s );

    if (name == "p")
        return( pp );
    if (name == "p0")
        return( pp0 );

    return( NULL );
}
/******************************************************************************/
/*                                                                            */
/* Class name:      toniRV                                                    */
/* Member function: sendModelVar                                              */
/* Purpose:         returns a pointer to the qreal wage, the main model var.   */
/* Last modified:   30.01.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
qreal* toniRV::sendModelVar()
{
    return pp;
}
/******************************************************************************/
/*                                                                            */
/* Class name:      toniRV                                                    */
/* Member function: sendStateSpace                                            */
/* Purpose:         returns pointers to the qreal balances and the qreal wage;  */
/*                  returns the dimension of the system for rho=0             */
/* Last modified:   30.01.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void toniRV::sendStateSpace(int &quantity,QList<qreal *> *stateSpace)
{
    //log() << "toniRV sendStateSpace";
    stateSpace->clear();
    quantity = dimension;
    *stateSpace << pp;
}
/******************************************************************************/
/*                                                                            */
/* Class name:      toniRV                                                    */
/* Member function: hoch                                                      */
/* Purpose:         compute exponent                                          */
/* Last modified:   28.01.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
qreal toniRV::hoch(qreal &x,int &i)
{
    qreal count=1.0;
	for (int k=i;k>0;k--)
		count*=x;
	return count;
}
/******************************************************************************/
/*                                                                            */
/* Class name:      toniRV                                                    */
/* Member function: fFunction                                                 */
/* Purpose:         compute                                                   */
/* Last modified:   19.03.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
qreal toniRV::fFunction(qreal &x)
{
	return ( (a-3.0)/b - (1.0/(b*M_PI_2))*atan(alpha*(x-3.0)) );
}
/******************************************************************************/
/*                                                                            */
/* Class name:      toniRV                                                    */
/* Member function: dynamics                                                  */
/* Purpose:         compute modifications for the next period                 */
/* Last modified:   04.02.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void toniRV::dynamics(qreal &a)
{
	for (int i=1;i<=L;i++) 
		pp[L+1-i]=pp[L-i];
	pp[0]=fFunction(a);				
    //log() << "\n" << "add=" << a << "\t pp[0]=" << pp[0];
	v=pp[0]-a;
}
/******************************************************************************/
/*                                                                            */
/* Class name:      toniRV                                                    */
/* Member function: iteration                                                 */
/* Purpose:         describes one step of the dynamical system                */
/* Last modified:   04.02.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void toniRV::iteration(const qint64&)
{
    qreal add=0;
    qreal ev=0;
    qreal abw=0;

	for (int i=0;i<=L;i++) 
	{
		add+=vv[i]*pp[i];
		ev+=pp[i];
	}
	ev*=(L+1);		//Everage

    for (int i=0;i<=L;i++)
		abw+=((pp[i]-ev)*(pp[i]-ev));
	s=sqrt(abw)/L;

	dynamics(add);

    //log() << "\nPeriod " << ii << "\t";
    //for(int k=0;k<=L;k++) log() << pp[k] << "\t";
}
