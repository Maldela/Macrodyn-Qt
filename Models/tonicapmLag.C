//$Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/tonicapmLag.C,v 1.1 2000/08/31 16:12:19 mmeyer Exp $
/******************************************************************************/
/*                                                                            */
/* Module name:  tonicapmLag.C                                                */
/* Contents:     Member functions of the class tonicapmLag                    */
/*                Standardversion of Cars Hommes                              */
/* Modified:     10.08.1997 (Toni Stiefenhofer)                               */
/* Last Modified:15.08.1997 (Toni Stiefenhofer)                               */
/*                                                                            */
/******************************************************************************/

#include "tonicapmLag.h"             
#include "../error.h"
/******************************************************************************/
/*                                                                            */
/* Class name:      tonicapmLag                                               */
/* Member function: tonicapmLag                                               */
/* Purpose:         constructor                                               */
/* Last modified:   30.01.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
tonicapmLag::tonicapmLag() : baseModel(50)
{
    pp0=NULL;
    pp=NULL;
    vv=NULL;
}
/******************************************************************************/
/*                                                                            */
/* Class name:      tonicapmLag                                               */
/* Member function: ~tonicapmLag                                              */
/* Purpose:         destructor                                                */
/* Last modified:   03.02.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
tonicapmLag::~tonicapmLag()
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
/* Class name:      tonicapmLag                                               */
/* Member function: initialize                                                */
/* Purpose:         initialize the model, set the initial state               */
/* Last modified:   29.11.1995 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void tonicapmLag::initialize()
{
	for(int k=0;k<=L;k++) pp[k]=pp0[k];

    qreal wsum=0;
    for (int k=0;k<=L;k++)
	   {
		wsum+=hoch(w,k);
        //log() << "k=" << k  << " wsum=" << wsum << "\n";
	   }
    //qreal ko=0;
	for(int i=0;i<=L;i++)
	   {
		vv[i]=hoch(w,i)/wsum;
		//ko+=vv[i];
        //log() << "i=" << i  << " w_i=" << vv[i] << " ko="<< ko << "\n";
	   }
}
/******************************************************************************/
/*                                                                            */
/* Class name:      tonicapmLag                                               */
/* Member function: loadParamset                                              */
/* Purpose:         load a parameterset from a specified input file           */
/* Last modified:   30.01.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void tonicapmLag::loadParamset(ifstream& inputFile)
{

    inputFile >> a >> b >> c >> d >> lambda >> sigma >> w >> L0;
    inputFile >> length ;

    L=(int)L0;
    if( pp0 )
	delete [] pp0;
        pp0 = new qreal[L+1];
    if( !pp0 )
	fatalError("tonicapmLag::loadParamset","Can't create initprice vector");
 
    for(int k=0;k<=L;k++)
        {
    	inputFile >> pp0[k] ;
        //log() << "L=" << L << "k=" << k  << "po=" << pp0[k];
    	}

    if( pp )
	delete [] pp;
        pp = new qreal[L+1];
    if( !pp )
	fatalError("tonicapmLag::loadParamset","Can't create price vector");
    if( vv )
	delete [] vv;
        vv = new qreal[L+1];
    if( !vv )
	fatalError("tonicapmLag::loadParamset","Can't create weight vector");
    	
    initialize();
}
/******************************************************************************/
/*                                                                            */
/* Class name:      tonicapmLag                                               */
/* Member function: saveParamset                                              */
/* Purpose:         write parameterset into a file                            */
/* Last modified:   30.01.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void tonicapmLag::saveParamset(ofstream& outputFile)
{
  outputFile << a << "\t" << b << "\t" << c << "\t" << d << "\t";
  outputFile << lambda << "\t" << sigma << "\t" << w << "\t";
    outputFile << L0 << "\t"<< length << "\t";
    for(int k=0;k<=L;k++)
        outputFile << pp0[k] << "\t";
    }
/******************************************************************************/
/*                                                                            */
/* Class name:      tonicapmLag                                               */
/* Member function: saveParamsetWithNames                                     */
/* Purpose:         add  parameterset to printfile                            */
/* Last modified:   20.02.1995 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void tonicapmLag::saveParamsetWithNames(ofstream& outputFile)
{
    outputFile << "\nModel TonicapmLag\n";
    outputFile << "a = " << a << "\tb = " << b << "\tc = " << c << "\td = " << d;
    outputFile << "\tlambda = " << lambda << "\tsigma = " << sigma << "\tL = " << L0 << "\tlength = "<< length << "\t";
    for(int k=0;k<=L;k++)
        outputFile << "p0_" << k << " = " << pp0[k] << "\t";
    for (int k=0;k<=L;k++)
        outputFile << "p_t-" << k << " = " << pp[k] << "\t";
    outputFile << "\n";
}
/******************************************************************************/
/*                                                                            */
/* Class name:      tonicapmLag                                               */
/* Member function: printParamset                                             */
/* Purpose:         print parameterset on the screen                          */
/* Last modified:   30.01.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void tonicapmLag::printParamset()
{
  log() << a  << "\t" << b << "\t" << c << "\t" << d << "\t";
  log() << lambda << "\t" << sigma << "\t" << w << "\t" << L0 << "\n";
    log() << length << "\n";
    for(int k=0;k<=L;k++)
        log() << pp0[k] << "\t";
}
/******************************************************************************/
/*                                                                            */
/* Class name:      tonicapmLag                                               */
/* Member function: sendParameters                                            */
/* Purpose:         write all parameters into an array and return the numbers */
/*                  of parameters                                             */
/* Last modified:   30.01.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void tonicapmLag::sendParameters(int& amount,qreal** parameters)
{
    if( *parameters )
	delete *parameters;
    amount=L+10;
    *parameters=new qreal[amount];
    if( !(*parameters) )
	fatalError("tonicapmLag::sendParameters",
		   "Can't create array for parameters");
    (*parameters)[0]=a;
    (*parameters)[1]=b;
    (*parameters)[2]=c;
    (*parameters)[3]=d;
    (*parameters)[4]=lambda;
    (*parameters)[5]=sigma;
    (*parameters)[6]=w;
    (*parameters)[7]=(qreal)L;
    (*parameters)[8]=length;
    for(int k=9;k<=(L+9);k++)
        (*parameters)[k]=pp0[k];
}
/******************************************************************************/
/*                                                                            */
/* Class name:      tonicapmLag                                               */
/* Member function: receiveParameters                                         */
/* Purpose:         receive parameter values                                  */
/* Last modified:   30.01.1995 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void tonicapmLag::receiveParameters(const qreal* parameters)
{
    a=parameters[0];
    b=parameters[1];
    c=parameters[2];
    d=parameters[3];
    lambda=parameters[4];
    sigma=parameters[5];
    w=parameters[6];
    L=(int)parameters[7];
    length=parameters[8];
    for(int k=9;k<=(L+9);k++)
       pp0[k]=parameters[k];
}
/******************************************************************************/
/*                                                                            */
/* Class name:      tonicapmLag                                               */
/* Member function: setlabels                                                 */
/* Purpose:         returns a pointer to a variable or parameter of the system*/
/*                  that is specified by its name                             */
/* Last modified:   09.03.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
qreal* tonicapmLag::setLabels(char *name)
{
    if( !strcmp(name,"xBundle") )
	return &xBundle;
    if( !strcmp(name,"yBundle") )
	return &yBundle;

    if( !strcmp(name,"a") )
        return( &a );
    if( !strcmp(name,"b") )
        return( &b );
    if( !strcmp(name,"c") )
        return( &c );
    if( !strcmp(name,"d") )
        return( &d );
    if( !strcmp(name,"lambda") )
        return( &lambda );
    if( !strcmp(name,"sigma") )
        return( &sigma );
    if( !strcmp(name,"w") )
        return( &w );
    if( !strcmp(name,"L") )
        return( &L0 );

    if( !strcmp(name,"v") )
        return( &v );
    if( !strcmp(name,"s") )
        return( &s );
    if( !strcmp(name,"p0") )
        return( pp0 );
    
    if( (name[0] == 'p') && (name[1] == '_') )
    {
      name+=2;
      int ind = atoi(name);

      if( ind <= L )
        return ( pp+ind );
      else
        fatalError("toniLag::setlabels",
                   "Too large: p_i > L");
    }

    /*    if( !strcmp(name,"p") )
          return( pp );           */  

    return( NULL );
}
/******************************************************************************/
/*                                                                            */
/* Class name:      tonicapmLag                                               */
/* Member function: sendModelVar                                              */
/* Purpose:         returns a pointer to the qreal wage, the main model var.   */
/* Last modified:   30.01.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
qreal* tonicapmLag::sendModelVar()
{
    return pp;
}
/******************************************************************************/
/*                                                                            */
/* Class name:      tonicapmLag                                                  */
/* Member function: sendStateSpace                                            */
/* Purpose:         returns pointers to the qreal balances and the qreal wage;  */
/*                  returns the dimension of the system for rho=0             */
/* Last modified:   30.01.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void tonicapmLag::sendStateSpace(int &quantity,const qreal*** stateSpace)
{
    //log() << "tonicapmLag sendStateSpace";
    if( *stateSpace )
	delete *stateSpace;
    *stateSpace= new const qreal* [dimension];
    if( !(*stateSpace) )
	fatalError("tonicapmLag::sendStateSpace",
		   "Can't create state space vector");
    quantity=dimension;
    (*stateSpace)[0]=pp;
}
/******************************************************************************/
/*                                                                            */
/* Class name:      tonicapmLag                                               */
/* Member function: hoch                                                      */
/* Purpose:         compute exponent                                          */
/* Last modified:   28.01.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
qreal tonicapmLag::hoch(qreal &x,int &i)
{
    qreal count=1.0;
	for (int k=i;k>0;k--)
		count*=x;
	return count;
}
/******************************************************************************/
/*                                                                            */
/* Class name:      tonicapmLag                                               */
/* Member function: fFunction                                                 */
/* Purpose:         compute                                                   */
/* Last modified:   10.08.1997 (Anton Stiefenhofer)                           */
/*                                                                            */
/******************************************************************************/
qreal tonicapmLag::fFunction(qreal &x)
{
  return ( (a/b) - (x - c)/( b * lambda * ( (x - c)*(x - c) + sigma * sigma * x * x)) );
}
/******************************************************************************/
/*                                                                            */
/* Class name:      tonicapmLag                                               */
/* Member function: dynamics                                                  */
/* Purpose:         compute modifications for the next period                 */
/* Last modified:   04.02.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void tonicapmLag::dynamics(qreal &a)
{
	for (int i=1;i<=L;i++) 
		pp[L+1-i]=pp[L-i];
	pp[0]=fFunction(a);				
    //log() << "\n" << "add=" << a << "\t pp[0]=" << pp[0];
	v=pp[0]-a;
}
/******************************************************************************/
/*                                                                            */
/* Class name:      tonicapmLag                                               */
/* Member function: iteration                                                 */
/* Purpose:         describes one step of the dynamical system                */
/* Last modified:   04.02.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void tonicapmLag::iteration(const long& t)
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
