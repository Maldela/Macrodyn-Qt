//$Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/hetoni.C,v 1.1 2000/08/31 15:57:21 mmeyer Exp $
/******************************************************************************/
/*                                                                            */
/* Module name:  hetoni.C                                                     */
/* Contents:     Member functions of the class toni                           */
/*                Standardversion of Cars Hommes                              */
/* Modified:     20.04.1997 (Marc Mueller, Toni Stiefenhofer)                 */
/* Last Modified:20.04.1997 (Marc Mueller, Toni Stiefenhofer)                 */
/*                                                                            */
/******************************************************************************/

#include "hetoni.h"             
#include "../error.h"
/******************************************************************************/
/*                                                                            */
/* Class name:      hetoni                                                    */
/* Member function: hetoni                                                    */
/* Purpose:         constructor                                               */
/* Last modified:   30.03.1997 (Marc Mueller, Toni Stiefenhofer)              */
/*                                                                            */
/******************************************************************************/
hetoni::hetoni() : baseModel(1)
{
    pp0=NULL;
    pp=NULL;
    vv=NULL;
    vvs=NULL;
}
/******************************************************************************/
/*                                                                            */
/* Class name:      hetoni                                                    */
/* Member function: ~hetoni                                                   */
/* Purpose:         destructor                                                */
/* Last modified:   03.03.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
hetoni::~hetoni()
{
    if( pp0 )
	delete [] pp0;
    if( pp )
	delete [] pp;
    if( vv )
      delete [] vv;
    if( vvs )
	delete [] vvs;
}
/******************************************************************************/
/*                                                                            */
/* Class name:      hetoni                                                    */
/* Member function: initialize                                                */
/* Purpose:         initialize the model, set the initial state               */
/* Last modified:   29.11.1995 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void hetoni::initialize()
{
	for(int k=0;k<=L;k++) pp[k]=pp0[k];

    qreal wsum=0;
    qreal wwsum=0;
    for (int k=0;k<=L;k++)
	   {
		wsum+=hoch(w,k);
        //log() << "k=" << k  << " wsum=" << wsum << "\n";
	   }
    //qreal ko=0;
    for (int k=0;k<=Ls;k++)
	   {
		wwsum+=hoch(w,k);
        //log() << "k=" << k  << " wsum=" << wsum << "\n";
	   }
    //qreal ko=0;
	for(int i=0;i<=L;i++)
	   {
		vv[i]=hoch(w,i)/wsum;
		//ko+=vv[i];
        //log() << "i=" << i  << " w_i=" << vv[i] << " ko="<< ko << "\n";
	   }

    for(int i=0;i<=Ls;i++)
	   {
		vvs[i]=hoch(w,i)/wwsum;
		//ko+=vv[i];
        //log() << "i=" << i  << " w_i=" << vv[i] << " ko="<< ko << "\n";
	   }
}
/******************************************************************************/
/*                                                                            */
/* Class name:      hetoni                                                    */
/* Member function: loadParamset                                              */
/* Purpose:         load a parameterset from a specified input file           */
/* Last modified:   20.04.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void hetoni::loadParamset(ifstream& inputFile)
{

    inputFile >> a >> b >> alpha >> w >> L0 >> L1 >> shr;
    inputFile >> length ;

    L =(int)L0;
    Ls=(int)L1;
    if( L < Ls )
	fatalError("hetoni::loadParamset","ERROR: L < Ls !");
    if( pp0 )
	delete [] pp0;
        pp0 = new qreal[L+1];
    if( !pp0 )
	fatalError("hetoni::loadParamset","Can't create initprice vector");
 
    for(int k=0;k<=L;k++)
        {
    	inputFile >> pp0[k] ;
        //log() << "L=" << L << "k=" << k  << "po=" << pp0[k];
    	}

    if( pp )
	delete [] pp;
        pp = new qreal[L+1];
    if( !pp )
	fatalError("hetoni::loadParamset","Can't create price vector");
    if( vv )
	delete [] vv;
        vv = new qreal[L+1];
    if( !vv )
	fatalError("hetoni::loadParamset","Can't create short weight vector");
    if( vvs )
	delete [] vvs;
        vvs = new qreal[L+1];
    if( !vvs )
	fatalError("hetoni::loadParamset","Can't create short weight vector");
    	
    initialize();
}
/******************************************************************************/
/*                                                                            */
/* Class name:      hetoni                                                    */
/* Member function: saveParamset                                              */
/* Purpose:         write parameterset into a file                            */
/* Last modified:   20.04.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void hetoni::saveParamset(ofstream& outputFile)
{
    outputFile << a << "\t" << b << "\t" << alpha << "\t" << w << "\t";
    outputFile << L0 << "\t"<< L1 << "\t" << shr << "\t" << length << "\t";
    for(int k=0;k<=L;k++)
        outputFile << pp0[k] << "\t";
    }
/******************************************************************************/
/*                                                                            */
/* Class name:      hetoni                                                    */
/* Member function: saveParamsetWithNames                                     */
/* Purpose:         add  parameterset to printfile                            */
/* Last modified:   20.04.1995 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void hetoni::saveParamsetWithNames(ofstream& outputFile)
{
    outputFile << "\nModel Hetoni\n";
    outputFile << "a = " << a << "\tb = " << b << "\talpha = " << alpha ;
    outputFile << "\tL = " << L0 << "\tLs = " << Ls << "\t";
    outputFile << "\tshr = " << shr << "\tlength = "<< length << "\t";
    for(int k=0;k<=L;k++)
        outputFile << "p0_" << k << " = " << pp0[k] << "\t";
    for (int k=0;k<=L;k++)
        outputFile << "p_t-" << k << " = " << pp[k] << "\t";
    outputFile << "\n";
}
/******************************************************************************/
/*                                                                            */
/* Class name:      hetoni                                                    */
/* Member function: printParamset                                             */
/* Purpose:         print parameterset on the screen                          */
/* Last modified:   20.04.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void hetoni::printParamset()
{
    log() << b  << "\t" << b << "\t" << alpha << "\t" << w << "\t" << L0 << "\n";
    log() << Ls << "\t" << shr << "\t" << length << "\n";
    for(int k=0;k<=L;k++)
        log() << pp0[k] << "\t";
}
/******************************************************************************/
/*                                                                            */
/* Class name:      hetoni                                                    */
/* Member function: sendParameters                                            */
/* Purpose:         write all parameters into an array and return the numbers */
/*                  of parameters                                             */
/* Last modified:   20.04.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void hetoni::sendParameters(int& amount,qreal** parameters)
{
    if( *parameters )
	delete *parameters;
    amount=L+9;
    *parameters=new qreal[amount];
    if( !(*parameters) )
	fatalError("hetoni::sendParameters",
		   "Can't create array for parameters");
    (*parameters)[0]=a;
    (*parameters)[1]=b;
    (*parameters)[2]=alpha;
    (*parameters)[3]=w;
    (*parameters)[4]=(qreal)L;
    (*parameters)[5]=(qreal)Ls;
    (*parameters)[6]=shr;
    (*parameters)[7]=length;
    for(int k=8;k<=(L+8);k++)
        (*parameters)[k]=pp0[k];
}
/******************************************************************************/
/*                                                                            */
/* Class name:      hetoni                                                    */
/* Member function: receiveParameters                                         */
/* Purpose:         receive parameter values                                  */
/* Last modified:   20.04.1995 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void hetoni::receiveParameters(const qreal* parameters)
{
    a=parameters[0];
    b=parameters[1];
    alpha=parameters[2];
    w=parameters[3];
    L=(int)parameters[4];
    Ls=(int)parameters[5];
    shr=parameters[6];
    length=parameters[7];
    for(int k=8;k<=(L+8);k++)
       pp0[k]=parameters[k];
}
/******************************************************************************/
/*                                                                            */
/* Class name:      hetoni                                                    */
/* Member function: setlabels                                                 */
/* Purpose:         returns a pointer to a variable or parameter of the system*/
/*                  that is specified by its name                             */
/* Last modified:   09.04.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
qreal* hetoni::setLabels(char *name)
{
    if( !strcmp(name,"xBundle") )
	return &xBundle;
    if( !strcmp(name,"yBundle") )
	return &yBundle;

    if( !strcmp(name,"a") )
        return( &a );
    if( !strcmp(name,"b") )
        return( &b );
    if( !strcmp(name,"alpha") )
        return( &alpha );
    if( !strcmp(name,"w") )
        return( &w );
    if( !strcmp(name,"shr") )
        return( &shr );
    if( !strcmp(name,"L") )
        return( &L0 );
    if( !strcmp(name,"Ls") )
        return( &L1 );
    if( !strcmp(name,"v") )
        return( &v );
    if( !strcmp(name,"s") )
        return( &s );

    if( !strcmp(name,"p") )
        return( pp );
    if( !strcmp(name,"p0") )
        return( pp0 );

    return( NULL );
}
/******************************************************************************/
/*                                                                            */
/* Class name:      hetoni                                                    */
/* Member function: sendModelVar                                              */
/* Purpose:         returns a pointer to the qreal wage, the main model var.   */
/* Last modified:   20.04.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
qreal* hetoni::sendModelVar()
{
    return pp;
}
/******************************************************************************/
/*                                                                            */
/* Class name:      hetoni                                                    */
/* Member function: sendStateSpace                                            */
/* Purpose:         returns pointers to the state variables;                  */
/*                  returns the dimension of the system;                      */
/* Last modified:   20.04.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void hetoni::sendStateSpace(int &quantity,const qreal*** stateSpace)
{
    //log() << "hetoni sendStateSpace";
    if( *stateSpace )
	delete *stateSpace;
    *stateSpace= new const qreal* [dimension];
    if( !(*stateSpace) )
	fatalError("hetoni::sendStateSpace",
		   "Can't create state space vector");
    quantity=dimension;
    (*stateSpace)[0]=pp;
}
/******************************************************************************/
/*                                                                            */
/* Class name:      hetoni                                                    */
/* Member function: hoch                                                      */
/* Purpose:         compute exponent                                          */
/* Last modified:   20.04.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
qreal hetoni::hoch(qreal &x,int &i)
{
    qreal count=1.0;
	for (int k=i;k>0;k--)
		count*=x;
	return count;
}
/******************************************************************************/
/*                                                                            */
/* Class name:      hetoni                                                    */
/* Member function: fFunction                                                 */
/* Purpose:         compute                                                   */
/* Last modified:   20.04.1997 (Marc Mueller, Toni Stiefenhofer)              */
/*                                                                            */
/******************************************************************************/
qreal hetoni::fFunction(qreal &x, qreal &y)
{
        qreal loshr=1.0-shr;
  
	return ( a - b*( loshr*atan(alpha*x) + shr*atan(alpha*y) )/M_PI_2 );
}
/******************************************************************************/
/*                                                                            */
/* Class name:      hetoni                                                    */
/* Member function: dynamics                                                  */
/* Purpose:         compute modifications for the next period                 */
/* Last modified:   20.04.1997 (Marc Mueller. Toni Stiefenhofer)              */
/*                                                                            */
/******************************************************************************/
void hetoni::dynamics(qreal &a, qreal &b)
{
	for (int i=1;i<=L;i++) 
		pp[L+1-i]=pp[L-i];
	pp[0]=fFunction(a,b);				
    //log() << "\n" << "add=" << a << "\t pp[0]=" << pp[0];
	v=pp[0]-a;
}
/******************************************************************************/
/*                                                                            */
/* Class name:      hetoni                                                    */
/* Member function: iteration                                                 */
/* Purpose:         describes one step of the dynamical system                */
/* Last modified:   20.04.1997 (Marc Mueller, Toni Stiefenhofer)              */
/*                                                                            */
/******************************************************************************/
void hetoni::iteration(const long& t)
{
    qreal addlo=0;
    qreal addso=0;
    qreal ev=0;
    qreal abw=0;

	for (int i=0;i<=L;i++) 
	{
		addlo+=vv[i]*pp[i];
		ev+=pp[i];
	}
	ev*=(L+1);		//Average

    for (int i=0;i<=Ls;i++)
	{
		addso+=vvs[i]*pp[i];
	}
	
    for (int i=0;i<=L;i++)
		abw+=((pp[i]-ev)*(pp[i]-ev));
	s=sqrt(abw)/L;

	dynamics(addlo, addso);

    //log() << "\nPeriod " << ii << "\t";
    //for(int k=0;k<=L;k++) log() << pp[k] << "\t";
}
