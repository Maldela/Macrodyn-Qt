//$Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/toniLag.C,v 1.1 2000/08/31 16:10:25 mmeyer Exp $
/******************************************************************************/
/*                                                                            */
/* Module name:  toniLag.C                                                    */
/* Contents:     Member functions of the class toniLag                        */
/*                Standardversion of Cars Hommes                              */
/* Modified:     28.01.1997 (Marc Mueller)                                    */
/* Last Modified:20.02.1997 (Marc Mueller)                                    */
/*                                                                            */
/******************************************************************************/

#include "toniLag.h"             
#include "../error.h"
/******************************************************************************/
/*                                                                            */
/* Class name:      toniLag                                                   */
/* Member function: toniLag                                                   */
/* Purpose:         constructor                                               */
/* Last modified:   30.01.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
toniLag::toniLag() : baseModel(50)
{
    pp0=NULL;
    pp=NULL;
    vv=NULL;
}
/******************************************************************************/
/*                                                                            */
/* Class name:      toniLag                                                   */
/* Member function: ~toniLag                                                  */
/* Purpose:         destructor                                                */
/* Last modified:   03.02.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
toniLag::~toniLag()
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
/* Class name:      toniLag                                                   */
/* Member function: initialize                                                */
/* Purpose:         initialize the model, set the initial state               */
/* Last modified:   29.11.1995 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void toniLag::initialize()
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
/* Class name:      toniLag                                                   */
/* Member function: loadParamset                                              */
/* Purpose:         load a parameterset from a specified input file           */
/* Last modified:   30.01.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void toniLag::loadParamset(QTextStream& inputFile)
{

    inputFile >> a >> b >> alpha >> w >> L0;
    inputFile >> length ;

    L=(int)L0;
    if( pp0 )
	delete [] pp0;
        pp0 = new qreal[L+1];
    if( !pp0 )
	fatalError("toniLag::loadParamset","Can't create initprice vector");
 
    for(int k=0;k<=L;k++)
        {
    	inputFile >> pp0[k] ;
        //log() << "L=" << L << "k=" << k  << "po=" << pp0[k];
    	}

    if( pp )
	delete [] pp;
        pp = new qreal[L+1];
    if( !pp )
	fatalError("toniLag::loadParamset","Can't create price vector");
    if( vv )
	delete [] vv;
        vv = new qreal[L+1];
    if( !vv )
	fatalError("toniLag::loadParamset","Can't create weight vector");
    	
    initialize();
}
/******************************************************************************/
/*                                                                            */
/* Class name:      toniLag                                                   */
/* Member function: saveParamset                                              */
/* Purpose:         write parameterset into a file                            */
/* Last modified:   30.01.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void toniLag::saveParamset(QTextStream& outputFile)
{
    outputFile << a << "\t" << b << "\t" << alpha << "\t" << w << "\t";
    outputFile << L0 << "\t"<< length << "\t";
    for(int k=0;k<=L;k++)
        outputFile << pp0[k] << "\t";
    }
/******************************************************************************/
/*                                                                            */
/* Class name:      toniLag                                                   */
/* Member function: saveParamsetWithNames                                     */
/* Purpose:         add  parameterset to printfile                            */
/* Last modified:   20.02.1995 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void toniLag::saveParamsetWithNames(QTextStream& outputFile)
{
    outputFile << "\nModel ToniLag\n";
    outputFile << "a = " << a << "\tb = " << b << "\talpha = " << alpha ;
    outputFile << "\tL = " << L0 << "\tlength = "<< length << "\t";
    for(int k=0;k<=L;k++)
        outputFile << "p0_" << k << " = " << pp0[k] << "\t";
    for (int k=0;k<=L;k++)
        outputFile << "p_t-" << k << " = " << pp[k] << "\t";
}
/******************************************************************************/
/*                                                                            */
/* Class name:      toniLag                                                   */
/* Member function: printParamset                                             */
/* Purpose:         print parameterset on the screen                          */
/* Last modified:   30.01.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void toniLag::printParamset()
{
    log() << a  << "\t" << b << "\t" << alpha << "\t" << w << "\t" << L0;
    log() << length;
    for(int k=0;k<=L;k++)
        log() << pp0[k] << "\t";
}
/******************************************************************************/
/*                                                                            */
/* Class name:      toniLag                                                   */
/* Member function: sendParameters                                            */
/* Purpose:         write all parameters into an array and return the numbers */
/*                  of parameters                                             */
/* Last modified:   30.01.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void toniLag::sendParameters(int& amount,qreal** parameters)
{
    if( *parameters )
	delete *parameters;
    amount=L+7;
    *parameters=new qreal[amount];
    if( !(*parameters) )
	fatalError("toniLag::sendParameters",
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
/* Class name:      toniLag                                                   */
/* Member function: receiveParameters                                         */
/* Purpose:         receive parameter values                                  */
/* Last modified:   30.01.1995 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void toniLag::receiveParameters(const QList<qreal>& parameters)
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
/* Class name:      toniLag                                                   */
/* Member function: setlabels                                                 */
/* Purpose:         returns a pointer to a variable or parameter of the system*/
/*                  that is specified by its name                             */
/* Last modified:   19.08.1997 (Anton Stiefenhofer)                           */
/*                                                                            */
/******************************************************************************/
qreal* toniLag::setLabels(const QString& name)
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
    if (name == "p0")
        return( pp0 );
    

    if (name.startsWith("p_"))
    {
        QString temp = name;
        temp.remove(0, 2);
        int ind = temp.toInt();

        if( ind <= L )
            return ( pp+ind );
        else
        {
            fatalError("toniLag::setlabels", "Too large: p_i > L");
            return(NULL);
        }
    }
    /*    if (name == "p")
          return( pp );           */  

    return( NULL );
}
/******************************************************************************/
/*                                                                            */
/* Class name:      toniLag                                                   */
/* Member function: sendModelVar                                              */
/* Purpose:         returns a pointer to the qreal wage, the main model var.   */
/* Last modified:   30.01.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
qreal* toniLag::sendModelVar()
{
    return pp;
}
/******************************************************************************/
/*                                                                            */
/* Class name:      toniLag                                                   */
/* Member function: sendStateSpace                                            */
/* Purpose:         returns pointers to the qreal balances and the qreal wage;  */
/*                  returns the dimension of the system for rho=0             */
/* Last modified:   30.01.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void toniLag::sendStateSpace(int &quantity,QList<qreal *> *stateSpace)
{
    //log() << "toniLag sendStateSpace";
    stateSpace->clear();
    quantity = dimension;
    *stateSpace << pp;
}
/******************************************************************************/
/*                                                                            */
/* Class name:      toniLag                                                   */
/* Member function: hoch                                                      */
/* Purpose:         compute exponent                                          */
/* Last modified:   28.01.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
qreal toniLag::hoch(qreal &x,int &i)
{
    qreal count=1.0;
	for (int k=i;k>0;k--)
		count*=x;
	return count;
}
/******************************************************************************/
/*                                                                            */
/* Class name:      toniLag                                                   */
/* Member function: fFunction                                                 */
/* Purpose:         compute                                                   */
/* Last modified:   19.03.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
qreal toniLag::fFunction(qreal &x)
{
	return ( (a-3.0)/b - (1.0/b)*atan(alpha*(x-3.0)) );
}
/******************************************************************************/
/*                                                                            */
/* Class name:      toniLag                                                   */
/* Member function: dynamics                                                  */
/* Purpose:         compute modifications for the next period                 */
/* Last modified:   04.02.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void toniLag::dynamics(qreal &a)
{
	for (int i=1;i<=L;i++) 
		pp[L+1-i]=pp[L-i];
	pp[0]=fFunction(a);				
    //log() << "\n" << "add=" << a << "\t pp[0]=" << pp[0];
	v=pp[0]-a;
}
/******************************************************************************/
/*                                                                            */
/* Class name:      toniLag                                                   */
/* Member function: iteration                                                 */
/* Purpose:         describes one step of the dynamical system                */
/* Last modified:   04.02.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void toniLag::iteration(const qint64& t)
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
