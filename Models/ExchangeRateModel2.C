/******************************************************************************/
/*                                                                            */
/* Module name:		ExchangeRateModel2.C                                      */
/* Contents:		Member functions of the class ExchangeRateModel2          */
/*					Whenever an equation is taken from the paper mentioned in */
/*					the description of the class definition this is marked by */
/*					(eq. equation number)                                     */
/* Autor:		 	Michael Meyer											  */
/*                                                                            */
/* Last Modified: 	Don Mär 14 11:54:51 CET 2002							  */
/* by:				Michael Meyer                           				  */	
/* Modified:      														      */
/*                                                                            */
/******************************************************************************/

#include "ExchangeRateModel2.h"             
#include "../error.h"

/******************************************************************************/
/*                                                                            */
/* Class name:      ExchangeRateModel2                                        */
/* Member function: ExchangeRateModel2                                        */
/* Purpose:         constructor                                               */
/* Last modified:   Don Mär 14 11:56:11 CET 2002 (Michael Meyer)              */
/*                                                                            */
/******************************************************************************/

ExchangeRateModel2::ExchangeRateModel2() : baseModel(5)
{
	theta=NULL;
    st_shift=new real[3];
    if( !st_shift )
	fatalError("ExchangeRateModel2::loadParamset","Can't create st vector");
	part_array = new real[3];
	if( !part_array)
	fatalError("ExchangeRateModel2::loadParamset","Can't create part_array vector");

}

/******************************************************************************/
/*                                                                            */
/* Class name:      ExchangeRateModel2                                        */
/* Member function: ~ExchangeRateModel2                                       */
/* Purpose:         destructor                                                */
/* Last modified:   Don Mär 14 11:57:22 CET 2002 (Michael Meyer)              */
/*                                                                            */
/******************************************************************************/

ExchangeRateModel2::~ExchangeRateModel2()
{
    if( theta )
	delete theta;
	if(st_shift)
	delete st_shift;
	if(part_array)
	delete part_array;

}
/******************************************************************************/
/*                                                                            */
/* Class name:      ExchangeRateModel2                                        */
/* Member function: initialize                                                */
/* Purpose:         initialize the model, set the initial state               */
/* Last modified:   Don Mär 14 11:57:22 CET 2002 (Michael Meyer)              */
/*                                                                            */
/******************************************************************************/

void ExchangeRateModel2::initialize()
{
    theta_Init(theta);
	st_Init(st_shift);
	part_array_Init(part_array);
//	st_shift[1]=st_1_0;
//	st_shift[2]=st_2_0;
	
    st=st_1_0;

	eerealt_1=eerealt_1_0;
	alphat=alphat_0;
	betat=betat_0;


}

/******************************************************************************/
/*                                                                            */
/* Class name:      ExchangeRateModel2                                        */
/* Member function: theta_et Init                                             */
/* Purpose:         initialize the theta_et vector                            */
/* Last modified:   Don Mär 14 11:58:07 CET 2002 (Michael Meyer)              */
/*                                                                            */
/******************************************************************************/

void ExchangeRateModel2::theta_Init(real *theta)
{
    int i;

    for( i=1; i<=tau+1; i++ )
        theta[i] = 0.0;
    	theta[0] = 1.0;

}

/******************************************************************************/
/*                                                                            */
/* Class name:      ExchangeRateModel2                                        */
/* Member function: bondprice Init                                            */
/* Purpose:         initialize the st vector 	                              */
/* Last modified:   Don Mär 14 11:58:07 CET 2002 (Michael Meyer)              */
/*                                                                            */
/******************************************************************************/

void ExchangeRateModel2::st_Init(real *st_shift)
{
    int i;

    for( i=1; i<=2; i++ )
    	st_shift[i] = 0.0;
    	st_shift[0] = 1.0;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      ExchangeRateModel2                                        */
/* Member function: bondprice Init                                            */
/* Purpose:         initialize the st vector 	                              */
/* Last modified:   Don Apr 17 14:55:42 CEST 2003 (Michael Meyer)              */
/*                                                                            */
/******************************************************************************/

void ExchangeRateModel2::part_array_Init(real *part_array)
{
    int i;

    for( i=0; i<=2; i++ )
    	part_array[i] = 1;
	part_1 = part_array[1];
	part_2 = part_array[2];


//for( i=0; i<=2; i++ )
//    cout << "part_array element" << i << " = "	<< part_array[i] << endl;

}
/******************************************************************************/
/*                                                                            */
/* Class name:      ExchangeRateModel2                                        */
/* Member function: expectedInflationRate                                     */
/* Purpose:         computes the expected inflation rate          			  */
/* Last modified:   Don Mär 14 11:58:07 CET 2002 (Michael Meyer)              */
/*                                                                            */
/******************************************************************************/

real ExchangeRateModel2::expectedInflationRate(const long t)
{
    long T,counter;
    real help=0.0;
    
    T=MIN(t,tau);
    for( counter=0; counter < T; counter++ )
	help += theta[counter];
    return( help / T );
}

/******************************************************************************/
/*                                                                            */
/* Class name:      ExchangeRateModel2                                        */
/* Member function: expectedInterestRate                                      */
/* Purpose:         computes the expected interest rate             		  */
/* Last modified:   Don Mär 14 11:58:07 CET 2002 (Michael Meyer)              */
/*                                                                            */
/******************************************************************************/
void ExchangeRateModel2::expectedInterestRate()
{
//	iet = (d+st_shift[1])/st_shift[2] - 1;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      ExchangeRateModel2                                        */
/* Member function: real exchange Rate                                        */
/* Purpose:         shift the real exchange rate              				  */
/* Last modified:   Don Mär 14 11:58:07 CET 2002 (Michael Meyer)              */
/*                                                                            */
/******************************************************************************/
void ExchangeRateModel2::realExchangeRate()
{
	erealt=eerealt_1;

// cout<<"erealt"<<erealt<<endl;

}
	
/******************************************************************************/
/*                                                                            */
/* Class name:      ExchangeRateModel2                                        */
/* Member function: Maximum capacity output                                   */
/* Purpose:         											     		  */
/*                  	                		                      		  */
/* Last modified:   Don Mär 14 11:58:03 CET 2002 (Michael Meyer)              */
/*                                                                            */
/******************************************************************************/

real ExchangeRateModel2::MaxCapacityOutput(const real& L)
{
	real result;
	result=A/B * exp( B*log(L) );
	return(result);
}

/******************************************************************************/
/*                                                                            */
/* Class name:      ExchangeRateModel2                                        */
/* Member function: Notional Labor Demand                                     */
/* Purpose:         											      		  */
/*                  	                		                      		  */
/* Last modified:   Don Mär 14 11:58:03 CET 2002 (Michael Meyer)              */
/*                                                                            */
/******************************************************************************/

void ExchangeRateModel2::NotLaborDemand()

{
	real basis,exponent;
	basis=A/alphat;
	exponent=1/(1-B);

	Lnt = exp( exponent*log(basis) );
}

/******************************************************************************/
/*                                                                            */
/* Class name:      ExchangeRateModel2                                        */
/* Member function: Notional Output                                           */
/* Purpose:         											       		  */
/*                  	                		                      		  */
/* Last modified:   Don Mär 14 11:58:03 CET 2002 (Michael Meyer)              */
/*                                                                            */
/******************************************************************************/

void ExchangeRateModel2::NotOutput()

{
	yLt = A/B * exp( B*log(Lnt) );
}

/******************************************************************************/
/*                                                                            */
/* Class name:      ExchangeRateModel2                                        */
/* Member function: effective aggregate demand                                */
/* Purpose:         											 		      */
/*                  	                		                      		  */
/* Last modified:   Don Mär 14 11:58:03 CET 2002 (Michael Meyer)              */
/*                                                                            */
/******************************************************************************/

void ExchangeRateModel2::effaggrdemand(real &theta_et)
{
	real expo1, expo2;
	expo1=1/(1-rho);
	expo2=rho/(1-rho);

	if(rho==0){
		c=1/(1+delta);
	}	
	else
	{
		c=1/(1+pow(delta,expo1)*pow((1+iet),expo2)*pow(theta_et,-expo2));
	}


	ydt=(1+deltaB)*(betat*d+g+E*erealt)/(tax+deltaB*(1-c*(1-tax)));

}

/******************************************************************************/
/*                                                                            */
/* Class name:      ExchangeRateModel2                                        */
/* Member function: Output                                              	  */
/* Purpose:             													  */
/*																			  */
/* Last modified:     Don Mär 14 11:58:03 CET 2002 (Michael Meyer)            */
/*                                                                            */
/******************************************************************************/

void ExchangeRateModel2::output(real &ydt, real &ymaxt, real &yLt)
{
	real yt_save;

	yt_save=yt;


    yt = MIN(MIN(ydt,yLt),ymaxt );    

	ytrate=yt/yt_save;
	

	

//cout << "yt" << yt << endl;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      ExchangeRateModel2                                        */
/* Member function: Employment                                             	  */
/* Purpose:             													  */
/*																			  */
/* Last modified:     Don Mär 14 11:58:03 CET 2002 (Michael Meyer)            */
/*                                                                            */
/******************************************************************************/

void ExchangeRateModel2::employment()
{	
	real expo1,expo2;
	real Lt_save;
	expo1=yt*B/A;
	expo2=1/B;

	Lt_save=Lt;

	if(yt==ymaxt)
	Lt=Lmax;
	else
	Lt = pow(expo1,expo2);

	Ltrate=Lt/Lt_save;
 
	uerate=(Lmax-Lt)/Lmax;


}

/******************************************************************************/
/*                                                                            */
/* Class name:      ExchangeRateModel2                                        */
/* Member function: bond price                                            	  */
/* Purpose:             													  */
/*																			  */
/* Last modified:     Don Mär 14 11:58:03 CET 2002 (Michael Meyer)            */
/*                                                                            */
/******************************************************************************/
void ExchangeRateModel2::bondprice()
{
	
//young last

//	st=(1-c)*(1-tax)/(betat*(1+deltaB))*yt;



//young first


real spoint= (yt-d*betat-g-E*erealt)/betat;
real salpha1=(1-tax)*yt/(betat*(1+deltaB));
real salpha2=(d*betat+g+E*erealt-tax*yt)/(betat*deltaB);


st_1=st;


if(ydt>yt)
{
if(salpha1>spoint)
st=salpha1;
else
st=salpha2;
}
else
st=(1-c)*(1-tax)*(betat*d+g+E*erealt)/( betat*(tax+deltaB*( 1-c*(1-tax) ) ) );


	yefft=yt*(1-tax)*c+(st+d)*betat+g+E*erealt;


//cout<<"st"<<st<<endl;


}



/******************************************************************************/
/*                                                                            */
/* Class name:      ExchangeRateModel2                                        */
/* Member function: Rationing Regime                                          */
/* Purpose:         					         							  */
/*                                                                    		  */
/* Last modified:   Don Mär 14 14:21:32 CET 2002 (Michael Meyer)              */
/*                                                                            */
/******************************************************************************/

void ExchangeRateModel2::ratioRegime(real &ydt,real &yLt,char *x1)
{
cout << "Unused" << endl;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      ExchangeRateModel2                                        */
/* Member function: wageAndPrice                                              */
/* Purpose:         wage and price adjustment                  				  */
/*																			  */
/* Last modified:   Don Mär 14 14:29:07 CET 2002 (Michael Meyer)              */
/*                                                                            */
/******************************************************************************/

void ExchangeRateModel2::wageAndpriceAdjust(real &ydt, real &yLt, real &Lnt, char *state)
{

// Pyperbolic Tangent Function



	if(ydt>yt)
		part = 1+gamma*tanh((yefft-yt)/yefft);
	else
		part = 1+kappa*tanh((yt-yLt)/yLt);

	if(Lmax>Lt)
		wart = 1+lambda*tanh((Lt-Lmax)/Lmax);
	else
		wart = 1+mu*tanh((Lnt-Lt)/Lnt);




/*

// Linear Price Adjustment Function

	if(ydt>yt)
		part = 1+gamma*( (yefft-yt)/yefft );
	else
		part = 1+kappa*((yt-yLt)/yLt);

	if(Lmax>Lt)
		wart = 1+lambda*((Lt-Lmax)/Lmax);
	else
		wart =1+mu*((Lnt-Lt)/Lnt);


*/


/*
	
// Exponential Price Adjustment Function


	if(ydt>yt)

		part = 1+pow(2,gamma*((yefft-yt)/yefft)/(1-fabs((yefft-yt)/yefft)));
	
	else

		part = 1+pow(2,kappa*((yt-yLt)/yLt)/(1-fabs((yt-yLt)/yLt)));
   

	if(Lmax>Lt)

		wart = 1+pow(2,lambda*((Lt-Lmax)/Lmax)/(1-fabs((Lt-Lmax)/Lmax)));

	else
		wart =1+pow(2,mu*((Lnt-Lt)/Lnt)/(1-fabs((Lnt-Lt)/Lnt)));


*/



}

/******************************************************************************/
/*                                                                            */
/* Class name:      ExchangeRateModel2                                        */
/* Member function: dynamics                                                  */
/* Purpose:         describes the dynamics of the system 					  */
/* Last modified:   Don Mär 14 14:29:07 CET 2002 (Michael Meyer)              */
/*                                                                            */
/******************************************************************************/

void ExchangeRateModel2::dynamics()
{
	alphat_1=alphat;
	alphat=alphat_1*(wart/part);


	betat_1=betat;
	betat=betat_1*(1+deltaB)/part;
	



//    cout<<"betat"<<betat<<endl;



	iet = (d+st)/st_1 - 1;

//    cout<<"st"<<st<<endl;

 //   cout<<"st_1"<<st_1<<endl;

	eerealt_1=1/part*erealt*(1+iet)/(1+fi);



    xtrate=eerealt_1/erealt*part; 
	rxtrate=eerealt_1/erealt;

//	cout<<"xtrate"<<xtrate<<endl;

	
    for(int i=0; i<=tau; i++ )
	theta[tau+1-i]=theta[tau-i];     /* p(t) -> price[t+1] */
    theta[0]=part;

    for(int j=0; j<=1; j++ )
	st_shift[2-j]=st_shift[1-j];     /* st(t) -> st[t+1] */
    st_shift[0]=st;

	for(int h=0; h<=1; h++ )
	part_array[2-h]=part_array[1-h];     /* st(t) -> st[t+1] */
    part_array[0]= part;

	part_1 = part_array[1];
	part_2 = part_array[2];

	delta_part = part_array[1] - part_array[2];
/*	cout << "part = " << part_array[0] << endl;
	cout << "part = " << part << endl;
	cout << "part_1 = " << part_array[1] << endl;
	cout << "part_2 = " << part_array[2] << endl;
	cout << "delta_part = " << delta_part << endl;
*/
}

/******************************************************************************/
/*                                                                            */
/* Class name:      ExchangeRateModel2                                        */
/* Member function: iteration                                                 */
/* Purpose:         describes one step of the dynamical system    			  */
/* Last modified:   Don Mär 14 14:29:07 CET 2002 (Michael Meyer)              */
/*                                                                            */
/******************************************************************************/

void ExchangeRateModel2::iteration(const long& t)
{
    real theta_et;
    char state[5];

	theta_et=expectedInflationRate(t);

	expectedInterestRate();
	realExchangeRate();
	ymaxt=MaxCapacityOutput(Lmax);

	NotLaborDemand();
	NotOutput();
	effaggrdemand(theta_et);
	output(ydt,ymaxt,yLt);
	employment();
	bondprice();
	wageAndpriceAdjust(ydt,yLt,Lnt,state);            
	dynamics();
}

/******************************************************************************/
/*                                                                            */
/* Class name:      ExchangeRateModel2                                        */
/* Member function: setlabels                                                 */
/* Purpose:         returns a pointer to a variable or parameter of the system*/
/*                  that is specified by its name                             */
/* Last modified:   Don Mär 14 14:29:07 CET 2002 (Michael Meyer)              */
/*                                                                            */
/******************************************************************************/

real* ExchangeRateModel2::setLabels(char *name)
{
    if( !strcmp(name,"xBundle") )
		return &xBundle;
    if( !strcmp(name,"yBundle") )
		return &yBundle;
    if( !strcmp(name,"g") )
        return( &g );
	if( !strcmp(name,"Lt") )
        return( &Lt );
    if( !strcmp(name,"yLt") )
        return( &yLt );
	if( !strcmp(name,"A") )
        return( &A );
    if( !strcmp(name,"ymaxt") )
        return( &ymaxt );
    if( !strcmp(name,"B") )
        return( &B );
    if( !strcmp(name,"E") )
        return( &E );
    if( !strcmp(name,"wart") )
        return( &wart );
    if( !strcmp(name,"part") )
        return( &part );
    if( !strcmp(name,"delta_part") )
        return( &delta_part );
	if( !strcmp(name,"part_1") )
        return( &part_1 );
    if( !strcmp(name,"part_2") )
        return( &part_2 );
    if( !strcmp(name,"theta") )
        return( theta );
    if( !strcmp(name,"gamma") )
        return( &gamma );
    if( !strcmp(name,"kappa") )
        return( &kappa );
    if( !strcmp(name,"lambda") )
        return( &lambda );
    if( !strcmp(name,"mu") )
        return( &mu );
    if( !strcmp(name,"rho") )
        return( &rho );
    if( !strcmp(name,"tax") )
        return( &tax );
    if( !strcmp(name,"Lmax") )
        return( &Lmax );
    if( !strcmp(name,"tau") )
		return( (real*)(&tau) );
    if( !strcmp(name,"betat") )
		return( (real*)(&betat) );
    if( !strcmp(name,"betat_0") )
		return( (real*)(&betat_0) );
    if( !strcmp(name,"alphat") )
		return( (real*)(&alphat) );
    if( !strcmp(name,"alphat_0") )
		return( (real*)(&alphat_0) );
    if( !strcmp(name,"yt") )
		return( (real*)(&yt) );
    if( !strcmp(name,"st") )
		return( (real*)(&st) );
    if( !strcmp(name,"yefft") )
		return( (real*)(&yefft) );
    if( !strcmp(name,"st_1") )
		return( (real*)(&st_1) );
    if( !strcmp(name,"st_1_0") )
		return( (real*)(&st_1_0) );
    if( !strcmp(name,"st_2") )
		return( (real*)(&st_2) );
    if( !strcmp(name,"st_2_0") )
		return( (real*)(&st_2_0) );
    if( !strcmp(name,"erealt") )
		return( (real*)(&erealt) );
    if( !strcmp(name,"eerealt_1") )
		return( (real*)(&eerealt_1) );
    if( !strcmp(name,"eerealt_1_0") )
		return( (real*)(&eerealt_1_0) );
    if( !strcmp(name,"ydt") )
		return( (real*)(&ydt) );
    if( !strcmp(name,"yt") )
		return( (real*)(&yt) );
    if( !strcmp(name,"d") )
		return( (real*)(&d) );
    if( !strcmp(name,"iet") )
		return( (real*)(&iet) );
    if( !strcmp(name,"fi") )
		return( (real*)(&fi) );
    if( !strcmp(name,"xtrate") )
		return( (real*)(&xtrate) );
    if( !strcmp(name,"rxtrate") )
		return( (real*)(&rxtrate) );
    if( !strcmp(name,"ytrate") )
		return( (real*)(&ytrate) );     
	if( !strcmp(name,"Ltrate") )
		return( (real*)(&Ltrate) );   
	
	if( !strcmp(name,"deltaB") )
		return( (real*)(&deltaB) );
	if( !strcmp(name,"uerate") )
		return( (real*)(&uerate) );

    if( !strcmp(name,"st_1") )
		return( (real*)(&st_1) );

return( NULL );
}

/******************************************************************************/
/*                                                                            */
/* Class name:      ExchangeRateModel2                                        */
/* Member function: sendModelVar                                              */
/* Purpose:         returns a pointer to the real wage, the main model var.   */
/* Last modified:   Don Mär 14 14:29:07 CET 2002 (Michael Meyer)              */
/*                                                                            */
/******************************************************************************/

real* ExchangeRateModel2::sendModelVar()
{
    return &wart;
	return &part;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      ExchangeRateModel2                                        */
/* Member function: sendStateSpace                                            */
/* Purpose:         returns pointers to the real balances and the real wage;  */
/*                  returns the dimension of the system for rho=0             */
/* Last modified:   Don Mär 14 14:29:07 CET 2002 (Michael Meyer)              */
/*                                                                            */
/******************************************************************************/

void ExchangeRateModel2::sendStateSpace(int &quantity,const real*** stateSpace)
{
    if( *stateSpace )
	delete *stateSpace;
    *stateSpace= new const real* [dimension];
    if( !(*stateSpace) )
	fatalError("ExchangeRateModel2::sendStateSpace",
		   "Can't create state space vector");
    quantity=dimension;
    (*stateSpace)[0]=&wart;
    (*stateSpace)[1]=&part;
    (*stateSpace)[2]=theta;
    (*stateSpace)[3]=st_shift;
    (*stateSpace)[4]=&erealt;

}
    
/******************************************************************************/
/*                                                                            */
/* Class name:      ExchangeRateModel2                                        */
/* Member function: loadParamset                                              */
/* Purpose:         load a parameterset from a specified input file           */
/* Last modified:   Don Mär 14 14:29:07 CET 2002 (Michael Meyer)              */
/*                                                                            */
/******************************************************************************/

void ExchangeRateModel2::loadParamset(ifstream& inputFile)
{
    inputFile >> A >> B;
    inputFile >> gamma>> kappa >> lambda >> mu;
    inputFile >> tau >> length;
    inputFile >> st_1_0 >> st_2_0 >> eerealt_1_0;
    inputFile >> alphat_0 >> betat_0;
    inputFile >> Lmax >> rho >> g >> tax;
	inputFile >> delta >> deltaB >> d >> E >> fi;

    if( theta )
	delete theta;
    theta = new real[tau+2];
    if( !theta )
	fatalError("ExchangeRateModel2::loadParamset","Can't create theta vector");
    
    initialize();
}

/******************************************************************************/
/*                                                                            */
/* Class name:      ExchangeRateModel2                                        */
/* Member function: saveParamset                                              */
/* Purpose:         write parameterset into a file                            */
/* Last modified:   Don Mär 14 14:29:07 CET 2002 (Michael Meyer)              */
/*                                                                            */
/******************************************************************************/

void ExchangeRateModel2::saveParamset(ofstream& outputFile)
{
    outputFile << A << "\t" << B << "\t";
    outputFile << gamma << "\t" << kappa << "\t" << lambda << "\t"<< mu << "\t";
    outputFile << tau << "\t" << length << "\t";
    outputFile << st_1_0 << st_2_0 << eerealt_1_0 << "\t";
    outputFile << alphat_0 << betat_0 << "\t";
    outputFile << Lmax << "\t" << rho << "\t" << g << "\t" << tax << "\t";
    outputFile << delta << "\t" << deltaB << "\t" 
			   << d << "\t" << E << "\t"<< fi <<"\t" <<endl;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      ExchangeRateModel2                                        */
/* Member function: saveParamsetWithNames                                     */
/* Purpose:         add  parameterset to printfile                            */
/* Last modified:   Don Mär 14 14:29:07 CET 2002 (Michael Meyer)              */
/*                                                                            */
/******************************************************************************/

void ExchangeRateModel2::saveParamsetWithNames(ofstream& outputFile)
{    
	outputFile << "ExchangeRateModel2:\n\t";
    outputFile << "A = " << A << "\tB = " << B << "\n\tgamma = ";
    outputFile << gamma << "\tkappa = " << kappa << "\tlambda = ";
    outputFile << lambda << "\tmu = " << mu << "\ttau = ";
    outputFile << tau << "\n\tlength = " << length << "\n\tst_1_0 = ";
	outputFile << st_1_0  << "\tst_2_0 = " << st_2_0 
			   << "\teerealt_1_0 = " << eerealt_1_0 << "\n\talphat_0 = ";
	outputFile << alphat_0 << "\tbetat_0 = " << betat_0 << "\n\tLmax =";
	outputFile << Lmax << "\trho = " << rho << "\tg = ";
    outputFile << g << "\ttax = " << tax << "\n\tdelta = ";
    outputFile << delta << "\tdelta = " << deltaB << "\td = "; 
    outputFile   << d << "\tE = " << E << "\tfi= " << fi;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      ExchangeRateModel2                                        */
/* Member function: printParamset                                             */
/* Purpose:         print parameterset on the screen                          */
/* Last modified:   Don Mär 14 14:29:07 CET 2002 (Michael Meyer)              */
/*                                                                            */
/******************************************************************************/

void ExchangeRateModel2::printParamset()
{
    cout << A << "\t" << B << "\n";
    cout << gamma << "\t" << kappa << "\t" << lambda << "\t" << mu << "\n";
    cout << tau << "\t" << length << "\n";
    cout << st_1_0 << st_2_0 << eerealt_1_0 << "\n";
    cout << alphat_0 << betat_0;
    cout << Lmax << "\t" << rho << "\t" << g << "\t" << tax << "\n";
    cout << delta << "\t" << deltaB<< "\t"  
		 << d << "\t" << E << "\t"<< fi << "\t" <<endl;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      ExchangeRateModel2                                        */
/* Member function: sendParameters                                            */
/* Purpose:         write all parameters into an array and return the numbers */
/*                  of parameters                                             */
/* Last modified:   Don Mär 14 14:29:07 CET 2002 (Michael Meyer)              */
/*                                                                            */
/******************************************************************************/

void ExchangeRateModel2::sendParameters(int& amount,real** parameters)
{
    if( *parameters )
	delete *parameters;
    amount=22;
    *parameters=new real[amount];
    if( !(*parameters) )
	fatalError("ExchangeRateModel2::sendParameters",
		   "Can't create array for parameters");
    (*parameters)[0]=A;
    (*parameters)[1]=B;
    (*parameters)[2]=gamma;
    (*parameters)[3]=kappa;
    (*parameters)[4]=lambda;
    (*parameters)[5]=mu;
    (*parameters)[6]=tau;
    (*parameters)[7]=length;
    (*parameters)[8]=st_1_0;
    (*parameters)[9]=st_2_0;
    (*parameters)[10]=eerealt_1_0;
    (*parameters)[11]=alphat_0;
    (*parameters)[12]=betat_0;
    (*parameters)[13]=Lmax;
    (*parameters)[14]=rho;
    (*parameters)[15]=g;
    (*parameters)[16]=tax;
    (*parameters)[17]=delta;
    (*parameters)[18]=deltaB;
    (*parameters)[19]=d;
    (*parameters)[20]=E;
    (*parameters)[21]=fi;
    
}

/******************************************************************************/
/*                                                                            */
/* Class name:      ExchangeRateModel2                                        */
/* Member function: receiveParameters                                         */
/* Purpose:         receive parameter values                                  */
/* Last modified:   Don Mär 14 14:29:07 CET 2002 (Michael Meyer)              */
/*                                                                            */
/******************************************************************************/

void ExchangeRateModel2::receiveParameters(const real* parameters)
{
    A=parameters[0];
    B=parameters[1];
    gamma=parameters[2];
    kappa=parameters[3];
    lambda=parameters[4];
    mu=parameters[5];
    tau=(int)(parameters[6]);
    length=(long)(parameters[7]);
    st_1_0=parameters[8];
    st_2_0=parameters[9];
    eerealt_1_0=parameters[10];
    alphat_0=parameters[11];
    betat_0=parameters[12];
    Lmax=parameters[13];
    rho=parameters[14];
    g=parameters[15];
    tax=parameters[16];
    delta=parameters[17];
    deltaB=parameters[18];
    d=parameters[19];
    E=parameters[20];
    fi=parameters[21];
 
}
