//$Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/toniadp.C,v 1.1 2000/08/31 16:11:10 mmeyer Exp $
/******************************************************************************/
/*                                                                            */
/* Module name:  toniadp.C                                                       */
/* Contents:     Member functions of the class toniadp                           */
/*                Standardversion of Cars Hommes                              */
/* Modified:     28.01.1997 (Marc Mueller)                                    */
/* Last Modified:20.02.1997 (Marc Mueller)                                    */
/*                                                                            */
/******************************************************************************/

#include "toniadp.h"             
#include "../error.h"
/******************************************************************************/
/*                                                                            */
/* Class name:      toniadp                                                      */
/* Member function: toniadp                                                      */
/* Purpose:         constructor                                               */
/* Last modified:   30.01.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
toniadp::toniadp() : baseModel(1)
{
    pp0=NULL;
    pp=NULL;
    vv=NULL;
}
/******************************************************************************/
/*                                                                            */
/* Class name:      toniadp                                                      */
/* Member function: ~toniadp                                                     */
/* Purpose:         destructor                                                */
/* Last modified:   03.02.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
toniadp::~toniadp()
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
/* Class name:      toniadp                                                      */
/* Member function: initialize                                                */
/* Purpose:         initialize the model, set the initial state               */
/* Last modified:   29.11.1995 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void toniadp::initialize()
{
	for(int k=0;k<=L;k++) pp[k]=pp0[k];

	real wsum=0;
	for(k=0;k<=L;k++)
	   {
		wsum+=hoch(w,k);
		//cout << "k=" << k  << " wsum=" << wsum << "\n";   
	   }
	//real ko=0;
	for(int i=0;i<=L;i++)
	   {
		vv[i]=hoch(w,i)/wsum;
		//ko+=vv[i];
		//cout << "i=" << i  << " w_i=" << vv[i] << " ko="<< ko << "\n";   
	   }
}
/******************************************************************************/
/*                                                                            */
/* Class name:      toniadp                                                      */
/* Member function: loadParamset                                              */
/* Purpose:         load a parameterset from a specified input file           */
/* Last modified:   30.01.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void toniadp::loadParamset(ifstream& inputFile)
{

    inputFile >> a >> b >> alpha >> w >> L0;
    inputFile >> length ;

    L=(int)L0;
    if( pp0 )
	delete [] pp0;
        pp0 = new real[L+1];
    if( !pp0 )
	fatalError("toniadp::loadParamset","Can't create initprice vector");
 
    for(int k=0;k<=L;k++)
        {
    	inputFile >> pp0[k] ;
    	//cout << "L=" << L << "k=" << k  << "po=" << pp0[k];
    	}

    if( pp )
	delete [] pp;
        pp = new real[L+1];
    if( !pp )
	fatalError("toniadp::loadParamset","Can't create price vector");
    if( vv )
	delete [] vv;
        vv = new real[L+1];
    if( !vv )
	fatalError("toniadp::loadParamset","Can't create weight vector");
    	
    initialize();
}
/******************************************************************************/
/*                                                                            */
/* Class name:      toniadp                                                      */
/* Member function: saveParamset                                              */
/* Purpose:         write parameterset into a file                            */
/* Last modified:   30.01.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void toniadp::saveParamset(ofstream& outputFile)
{
    outputFile << a << "\t" << b << "\t" << alpha << "\t" << w << "\t";
    outputFile << L0 << "\t"<< length << "\t";
    for(int k=0;k<=L;k++)
        outputFile << pp0[k] << "\t";
    }
/******************************************************************************/
/*                                                                            */
/* Class name:      toniadp                                                      */
/* Member function: saveParamsetWithNames                                     */
/* Purpose:         add  parameterset to printfile                            */
/* Last modified:   20.02.1995 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void toniadp::saveParamsetWithNames(ofstream& outputFile)
{
    outputFile << "\nModel Toniadp\n";
    outputFile << "a = " << a << "\tb = " << b << "\talpha = " << alpha ;
    outputFile << "\tL = " << L0 << "\tlength = "<< length << "\t";
    for(int k=0;k<=L;k++)
        outputFile << "p0_" << k << " = " << pp0[k] << "\t";
    for(k=0;k<=L;k++)
        outputFile << "p_t-" << k << " = " << pp[k] << "\t";
    outputFile << "\n";
}
/******************************************************************************/
/*                                                                            */
/* Class name:      toniadp                                                      */
/* Member function: printParamset                                             */
/* Purpose:         print parameterset on the screen                          */
/* Last modified:   30.01.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void toniadp::printParamset()
{
    cout << b  << "\t" << b << "\t" << alpha << "\t" << w << "\t" << L0 << "\n";
    cout << length << "\n";
    for(int k=0;k<=L;k++)
        cout << pp0[k] << "\t";
}
/******************************************************************************/
/*                                                                            */
/* Class name:      toniadp                                                      */
/* Member function: sendParameters                                            */
/* Purpose:         write all parameters into an array and return the numbers */
/*                  of parameters                                             */
/* Last modified:   30.01.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void toniadp::sendParameters(int& amount,real** parameters)
{
    if( *parameters )
	delete *parameters;
    amount=L+7;
    *parameters=new real[amount];
    if( !(*parameters) )
	fatalError("toniadp::sendParameters",
		   "Can't create array for parameters");
    (*parameters)[0]=a;
    (*parameters)[1]=b;
    (*parameters)[2]=alpha;
    (*parameters)[3]=w;
    (*parameters)[4]=(real)L;
    (*parameters)[5]=length;
    for(int k=6;k<=(L+6);k++)
        (*parameters)[k]=pp0[k];
}
/******************************************************************************/
/*                                                                            */
/* Class name:      toniadp                                                      */
/* Member function: receiveParameters                                         */
/* Purpose:         receive parameter values                                  */
/* Last modified:   30.01.1995 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void toniadp::receiveParameters(const real* parameters)
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
/* Class name:      toniadp                                                      */
/* Member function: setlabels                                                 */
/* Purpose:         returns a pointer to a variable or parameter of the system*/
/*                  that is specified by its name                             */
/* Last modified:   09.03.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
real* toniadp::setLabels(char *name)
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
    if( !strcmp(name,"L") )
        return( &L0 );

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
/* Class name:      toniadp                                                      */
/* Member function: sendModelVar                                              */
/* Purpose:         returns a pointer to the real wage, the main model var.   */
/* Last modified:   30.01.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
real* toniadp::sendModelVar()
{
    return pp;
}
/******************************************************************************/
/*                                                                            */
/* Class name:      toniadp                                                      */
/* Member function: sendStateSpace                                            */
/* Purpose:         returns pointers to the real balances and the real wage;  */
/*                  returns the dimension of the system for rho=0             */
/* Last modified:   30.01.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void toniadp::sendStateSpace(int &quantity,const real*** stateSpace)
{
    //cout << "toniadp sendStateSpace";
    if( *stateSpace )
	delete *stateSpace;
    *stateSpace= new const real* [dimension];
    if( !(*stateSpace) )
	fatalError("toniadp::sendStateSpace",
		   "Can't create state space vector");
    quantity=dimension;
    (*stateSpace)[0]=pp;
}
/******************************************************************************/
/*                                                                            */
/* Class name:      toniadp                                                      */
/* Member function: hoch                                                      */
/* Purpose:         compute exponent                                          */
/* Last modified:   28.01.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
real toniadp::hoch(real &x,int &i)
{
	real count=1.0;
	for (int k=i;k>0;k--)
		count*=x;
	return count;
}
/******************************************************************************/
/*                                                                            */
/* Class name:      toniadp                                                      */
/* Member function: fFunction                                                 */
/* Purpose:         compute                                                   */
/* Last modified:   19.03.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
real toniadp::fFunction(real &x)
{
	return ((1.0 - w)*((a-3.0)/b - (1.0/b)*atan(alpha*(x-3.0))) + w * x);
}
/******************************************************************************/
/*                                                                            */
/* Class name:      toniadp                                                      */
/* Member function: dynamics                                                  */
/* Purpose:         compute modifications for the next period                 */
/* Last modified:   04.02.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void toniadp::dynamics(real &a)
{
	for (int i=1;i<=L;i++) 
		pp[L+1-i]=pp[L-i];
	pp[0]=fFunction(a);				
	//cout << "\n" << "add=" << a << "\t pp[0]=" << pp[0];
	v=pp[0]-a;
}
/******************************************************************************/
/*                                                                            */
/* Class name:      toniadp                                                      */
/* Member function: iteration                                                 */
/* Purpose:         describes one step of the dynamical system                */
/* Last modified:   04.02.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void toniadp::iteration(const long& t)
{
	real add=0;
	real ev=0;
	real abw=0;

	for (int i=0;i<=L;i++) 
	{
		add+=vv[i]*pp[i];
		ev+=pp[i];
	}
	ev*=(L+1);		//Everage

	for (i=0;i<=L;i++) 
		abw+=((pp[i]-ev)*(pp[i]-ev));
	s=sqrt(abw)/L;

	dynamics(add);

	//cout << "\nPeriod " << ii << "\t";
	//for(int k=0;k<=L;k++) cout << pp[k] << "\t";
}
