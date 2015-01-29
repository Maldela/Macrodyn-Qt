/******************************************************************************/
/*                                                                            */
/* Module name:		ExchangeRateModel.C                                       */
/* Contents:		Member functions of the class ExchangeRateModel           */
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

#include "ExchangeRateModel.h"             
#include "../logger.h"

/******************************************************************************/
/*                                                                            */
/* Class name:      ExchangeRateModel                                         */
/* Member function: ExchangeRateModel                                         */
/* Purpose:         constructor                                               */
/* Last modified:   Don Mär 14 11:56:11 CET 2002 (Michael Meyer)              */
/*                                                                            */
/******************************************************************************/

ExchangeRateModel::ExchangeRateModel() : baseModel(5)
{
    theta=NULL;
    st_shift=new qreal[3];
    if( !st_shift )
	fatalError("ExchangeRateModel::ExchangeRateModel","Can't create st vector");

}

/******************************************************************************/
/*                                                                            */
/* Class name:      ExchangeRateModel                                         */
/* Member function: ~ExchangeRateModel                                        */
/* Purpose:         destructor                                                */
/* Last modified:   Don Mär 14 11:57:22 CET 2002 (Michael Meyer)              */
/*                                                                            */
/******************************************************************************/

ExchangeRateModel::~ExchangeRateModel()
{
    if( theta )
	delete theta;
	if(st_shift)
	delete st_shift;

}

/******************************************************************************/
/*                                                                            */
/* Class name:      ExchangeRateModel                                         */
/* Member function: initialize                                                */
/* Purpose:         initialize the model, set the initial state               */
/* Last modified:   Don Mär 14 11:57:22 CET 2002 (Michael Meyer)              */
/*                                                                            */
/******************************************************************************/

void ExchangeRateModel::initialize()
{
    theta_Init(theta);
	st_Init(st_shift);

	st_shift[1]=st_1_0;
	st_shift[2]=st_2_0;
	
    eerealt_1=eerealt_1_0;
	alphat=alphat_0;
	betat=betat_0;
// DEFAULT    rhoTilda=1-rho/(1-rho);
}

/******************************************************************************/
/*                                                                            */
/* Class name:      ExchangeRateModel                                         */
/* Member function: theta_et Init                                             */
/* Purpose:         initialize the theta_et vector                            */
/* Last modified:   Don Mär 14 11:58:07 CET 2002 (Michael Meyer)              */
/*                                                                            */
/******************************************************************************/

void ExchangeRateModel::theta_Init(qreal *theta)
{
    int i;

    for( i=1; i<=tau+1; i++ )
        theta[i] = 0.0;
    theta[0]=1.0;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      ExchangeRateModel                                         */
/* Member function: bondprice Init                                            */
/* Purpose:         initialize the st vector 	                              */
/* Last modified:   Don Mär 14 11:58:07 CET 2002 (Michael Meyer)              */
/*                                                                            */
/******************************************************************************/

void ExchangeRateModel::st_Init(qreal *st_shift)
{
    int i;

    for( i=1; i<=2; i++ )
        st_shift[i] = 0.0;
    st_shift[0]=1.0;
}
/******************************************************************************/
/*                                                                            */
/* Class name:      ExchangeRateModel                                         */
/* Member function: expectedInflationRate                                     */
/* Purpose:         computes the expected inflation rate          			  */
/* Last modified:   Don Mär 14 11:58:07 CET 2002 (Michael Meyer)              */
/*                                                                            */
/******************************************************************************/

qreal ExchangeRateModel::expectedInflationRate(const qint64 t)
{
    qint64 T,counter;
    qreal help=0.0;
    
    T = qMin(t,tau);
    for( counter=0; counter < T; counter++ )
	help += theta[counter];
    return( help / T );
}

/******************************************************************************/
/*                                                                            */
/* Class name:      ExchangeRateModel                                         */
/* Member function: expectedInterestRate                                      */
/* Purpose:         computes the expected interest rate             		  */
/* Last modified:   Don Mär 14 11:58:07 CET 2002 (Michael Meyer)              */
/*                                                                            */
/******************************************************************************/
void ExchangeRateModel::expectedInterestRate()
{
	iet = (d+st_shift[1])/st_shift[2] - 1;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      ExchangeRateModel                                         */
/* Member function: qreal exchange Rate                                        */
/* Purpose:         shift the qreal exchange rate              				  */
/* Last modified:   Don Mär 14 11:58:07 CET 2002 (Michael Meyer)              */
/*                                                                            */
/******************************************************************************/
void ExchangeRateModel::qrealExchangeRate()
{
    eqrealt=eerealt_1;
}
	
/******************************************************************************/
/*                                                                            */
/* Class name:      ExchangeRateModel                                         */
/* Member function: qMaximum capacity output                                   */
/* Purpose:         											     		  */
/*                  	                		                      		  */
/* Last modified:   Don Mär 14 11:58:03 CET 2002 (Michael Meyer)              */
/*                                                                            */
/******************************************************************************/

qreal ExchangeRateModel::qMaxCapacityOutput(const qreal& L)
{
    qreal result;
	result=A/B * exp( B*log(L) );
	return(result);
}

/******************************************************************************/
/*                                                                            */
/* Class name:      ExchangeRateModel                                         */
/* Member function: Notional Labor Demand                                     */
/* Purpose:         											      		  */
/*                  	                		                      		  */
/* Last modified:   Don Mär 14 11:58:03 CET 2002 (Michael Meyer)              */
/*                                                                            */
/******************************************************************************/

void ExchangeRateModel::NotLaborDemand()

{
    qreal basis,exponent;
	basis=A/alphat;
	exponent=1/(1-B);

	Lnt = exp( exponent*log(basis) );
}

/******************************************************************************/
/*                                                                            */
/* Class name:      ExchangeRateModel                                         */
/* Member function: Notional Output                                           */
/* Purpose:         											       		  */
/*                  	                		                      		  */
/* Last modified:   Don Mär 14 11:58:03 CET 2002 (Michael Meyer)              */
/*                                                                            */
/******************************************************************************/

void ExchangeRateModel::NotOutput()

{
	yLt = A/B * exp( B*log(Lnt) );
}

/******************************************************************************/
/*                                                                            */
/* Class name:      ExchangeRateModel                                         */
/* Member function: effective aggregate demand                                */
/* Purpose:         											 		      */
/*                  	                		                      		  */
/* Last modified:   Don Mär 14 11:58:03 CET 2002 (Michael Meyer)              */
/*                                                                            */
/******************************************************************************/

void ExchangeRateModel::effaggrdemand(qreal &theta_et)
{
    qreal expo1, expo2;
	expo1=1/(1-rho);
	expo2=rho/(1-rho);
// DEFAULT    qreal sigma = rhoTilda / (rhoTilda-1);


	if(rho==0){
		c=1/(1+delta);
	}	
	else{
		c=1/(1+pow(delta,expo1)*pow((1+iet),expo2)*pow(theta_et,expo2));
// DEFAULT   c =   1/ ( 1 + exp( log(delta)/(1-rhoTilda) + sigma*log(theta_et) ) );

	}

    ydt=(g+betat*d+E*eqrealt)*(1+deltaB)/(tax+deltaB*(1-c*(1-tax)));

// DEFAULT	ydt=(g+betat)/(1-c*(1-tax));
}

/******************************************************************************/
/*                                                                            */
/* Class name:      ExchangeRateModel                                         */
/* Member function: Output                                              	  */
/* Purpose:             													  */
/*																			  */
/* Last modified:     Don Mär 14 11:58:03 CET 2002 (Michael Meyer)            */
/*                                                                            */
/******************************************************************************/

void ExchangeRateModel::output(qreal &ydt, qreal &ymaxt, qreal &yLt)
{
    qreal yt_save;

	yt_save=yt;
	
    yt = qMin(qMin(ydt,yLt),ymaxt );

	ytrate=yt/yt_save;

if (yt<1e-5)
	yt=1e-5;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      ExchangeRateModel                                         */
/* Member function: Employment                                             	  */
/* Purpose:             													  */
/*																			  */
/* Last modified:     Don Mär 14 11:58:03 CET 2002 (Michael Meyer)            */
/*                                                                            */
/******************************************************************************/

void ExchangeRateModel::employment()
{
    qreal expo1,expo2;
    qreal Lt_save;
	expo1=yt*B/A;
	expo2=1/B;

	Lt_save=Lt;

    if(yt==ymaxt)
    Lt=Lmax;
	else
	Lt = pow(expo1,expo2);

	Ltrate=Lt/Lt_save;
 
}

/******************************************************************************/
/*                                                                            */
/* Class name:      ExchangeRateModel                                         */
/* Member function: bond price                                            	  */
/* Purpose:             													  */
/*																			  */
/* Last modified:     Don Mär 14 11:58:03 CET 2002 (Michael Meyer)            */
/*                                                                            */
/******************************************************************************/
void ExchangeRateModel::bondprice()
{
//	qreal bondprice;

    qreal st_save;
    qreal buffer;

//	qreal comp1,comp2;
//	comp1=ydt-yt;
//	comp2=c*(1-tax)*ydt;

//	if(yt==ydt){

//		bondprice=((1-c)*(1-tax)*yt)/(betat*(1+deltaB));

		st_save=st;

        buffer = qMax((betat*(1+deltaB)),pow(double(10),double(-24)));

		st=((1-c)*(1-tax)*yt)/buffer;

		strate=st/st_save;

//		return bondprice;
//	}
//	else{
//		if(comp1<comp2){
//			st=(betat*d+E*eqrealt+g-tax*yt)/(betat*deltaB);
//			st=((1-deltaB)*(betat+d+E*eqrealt+g-yt*tax)
//				+(1-c)*(1-tax)*ydt)/(betat*((1+deltaB)*(1+deltaB)));
//		}
//		else{
//			st=((1-tax)*yt)/(betat*(1+deltaB));		
//		}
//	}						
}

/******************************************************************************/
/*                                                                            */
/* Class name:      ExchangeRateModel                                         */
/* Member function: Rationing Regime                                          */
/* Purpose:         					         							  */
/*                                                                    		  */
/* Last modified:   Don Mär 14 14:21:32 CET 2002 (Michael Meyer)              */
/*                                                                            */
/******************************************************************************/

void ExchangeRateModel::ratioRegime(qreal &ydt,qreal &yLt,char *x1)
{
    if( ydt > yLt ){
        if( ymaxt > yLt)
	    strcpy(x1,"C ");
		else {
            if( ymaxt == yLt )
	        strcpy(x1,"CI");
	    	else
				strcpy(x1,"I ");
		}
    }
    else {
        if( ydt == yLt ) {
            if( ymaxt > yLt )
			strcpy(x1,"CK");
	    	else {
                if( ymaxt == yLt )
		    	strcpy(x1,"WE");
			else
		    	strcpy(x1,"I ");
	    	}
		}	
		else {
	    	if( ydt < yLt ) {
                if( ymaxt > ydt )
		    	strcpy(x1,"K ");
					else {
                        if( ymaxt == ydt )
		        		strcpy(x1,"IK");
		    	else
		        	strcpy(x1,"I ");
					}	
	    	}
		}
    }
}

/******************************************************************************/
/*                                                                            */
/* Class name:      ExchangeRateModel                                         */
/* Member function: wageAndPrice                                              */
/* Purpose:         wage and price adjustment                  				  */
/*																			  */
/* Last modified:   Don Mär 14 14:29:07 CET 2002 (Michael Meyer)              */
/*                                                                            */
/******************************************************************************/

void ExchangeRateModel::wageAndpriceAdjust(qreal &ydt, qreal &yLt, qreal &Lnt, char *)
{
/*    if( (!strcmp(state,"K ")) || (!strcmp(state,"IK ")))
		part = 1+kappa*((yt-yLt)/yLt) ;

    else
		part = 1+gamma*( (ydt-yt)/ydt ) ;
    
    if( (!strcmp(state,"I "))||(!strcmp(state,"IK ")))
		wart =1+mu*((Lnt-Lt)/Lnt) ;
   
	else

        wart = 1+lambda*((Lt-Lmax)/Lmax) ;
	    
    if (part<1e-5)
	part=1e-5;
    if (wart<1e-5)
	wart=1e-5;*/


	if(ydt>yt)
//		part = 1+gamma*( (ydt-yt)/ydt );
		part = 1+gamma*( (yefft-yt)/yefft );
	else
		part = 1+kappa*((yt-yLt)/yLt);

    if(Lmax>Lt)
        wart = 1+lambda*((Lt-Lmax)/Lmax);
	else
		wart =1+mu*((Lnt-Lt)/Lnt);


}

/******************************************************************************/
/*                                                                            */
/* Class name:      ExchangeRateModel                                         */
/* Member function: dynamics                                                  */
/* Purpose:         describes the dynamics of the system 					  */
/* Last modified:   Don Mär 14 14:29:07 CET 2002 (Michael Meyer)              */
/*                                                                            */
/******************************************************************************/

void ExchangeRateModel::dynamics()
{
	alphat_1=alphat;
	betat_1=betat;
	
    qreal comp1,buffer;

//	log() << "st=" << st 
//	log() << "betat=" << betat 
//	log() << "c=" << c 
//	log() << "eqrealt=" << eqrealt 

// DEFAULT	buffer=betat+g;


    buffer=betat*(d+st)+E*eqrealt+g;

    comp1 = qMin(yt,buffer);

	alphat=alphat_1*(wart/part);

// DEFAULT	betat=(comp1-tax*yt)/part;

//alt	betat=1/(st*part)*(comp1-tax*yt);
	betat=(betat_1*(1+deltaB))/part;
	
    eerealt_1=1/part*eqrealt*(1+iet)/(1+fi);

    xtrate=eerealt_1/eqrealt*part;
    rxtrate=eerealt_1/eqrealt;

    for(int i=0; i<=tau; i++ )
	theta[tau+1-i]=theta[tau-i];     /* p(t) -> price[t+1] */
    theta[0]=part;

    for(int j=0; j<=1; j++ )
	st_shift[2-j]=st_shift[1-j];     /* st(t) -> st[t+1] */
    st_shift[0]=st;

	
}

/******************************************************************************/
/*                                                                            */
/* Class name:      ExchangeRateModel                                         */
/* Member function: iteration                                                 */
/* Purpose:         describes one step of the dynamical system    			  */
/* Last modified:   Don Mär 14 14:29:07 CET 2002 (Michael Meyer)              */
/*                                                                            */
/******************************************************************************/

void ExchangeRateModel::iteration(const qint64& t)
{
    qreal theta_et;
    char state[5];

    theta_et=expectedInflationRate(t);

	expectedInterestRate();
    qrealExchangeRate();
    ymaxt = qMaxCapacityOutput(Lmax);
	NotLaborDemand();
	NotOutput();
	effaggrdemand(theta_et);
    output(ydt,ymaxt,yLt);
	employment();
	bondprice();

yefft=(g+betat*d+E*eqrealt+st*betat)/(1-c*(1-tax));

	ratioRegime(ydt,yLt,state);
	wageAndpriceAdjust(ydt,yLt,Lnt,state);            
	dynamics();
}

/******************************************************************************/
/*                                                                            */
/* Class name:      ExchangeRateModel                                         */
/* Member function: setlabels                                                 */
/* Purpose:         returns a pointer to a variable or parameter of the system*/
/*                  that is specified by its name                             */
/* Last modified:   Don Mär 14 14:29:07 CET 2002 (Michael Meyer)              */
/*                                                                            */
/******************************************************************************/

qreal* ExchangeRateModel::setLabels(const QString& name)
{
    if (name == "xBundle")
		return &xBundle;
    if (name == "yBundle")
		return &yBundle;
    if (name == "g")
        return( &g );
    if (name == "Lt")
        return( &Lt );
    if (name == "A")
        return( &A );
    if (name == "ymaxt")
        return( &ymaxt );
    if (name == "B")
        return( &B );
    if (name == "E")
        return( &E );
    if (name == "wart")
        return( &wart );
    if (name == "part")
        return( &part );
    if (name == "theta")
        return( theta );
    if (name == "gamma")
        return( &gamma );
    if (name == "kappa")
        return( &kappa );
    if (name == "lambda")
        return( &lambda );
    if (name == "mu")
        return( &mu );
    if (name == "delta")
        return( &delta );
    if (name == "deltaB")
        return( &deltaB );
    if (name == "rho")
        return( &rho );
    if (name == "tax")
        return( &tax );
    if (name == "Lmax")
        return( &Lmax );
    if (name == "tau")
        return( (qreal*)(&tau) );
    if (name == "betat")
        return( (qreal*)(&betat) );
    if (name == "alphat")
        return( (qreal*)(&alphat) );
    if (name == "alphat_0")
        return( (qreal*)(&alphat_0) );
    if (name == "yt")
        return( (qreal*)(&yt) );
    if (name == "st")
        return( (qreal*)(&st) );
    if (name == "yefft")
        return( (qreal*)(&yefft) );
    if (name == "st_1")
        return( (qreal*)(&st_1) );
    if (name == "st_1_0")
        return( (qreal*)(&st_1_0) );
    if (name == "st_2")
        return( (qreal*)(&st_2) );
    if (name == "st_2_0")
        return( (qreal*)(&st_2_0) );
    if (name == "eqrealt")
        return( (qreal*)(&eqrealt) );
     if (name == "eerealt_1")
        return( (qreal*)(&eerealt_1) );
    if (name == "eerealt_1_0")
        return( (qreal*)(&eerealt_1_0) );
    if (name == "ydt")
        return( (qreal*)(&ydt) );
    if (name == "yt")
        return( (qreal*)(&yt) );
    if (name == "d")
        return( (qreal*)(&d) );
    if (name == "iet")
        return( (qreal*)(&iet) );
    if (name == "fi")
        return( (qreal*)(&fi) );
    if (name == "xtrate")
        return( (qreal*)(&xtrate) );
    if (name == "rxtrate")
        return( (qreal*)(&rxtrate) );
    if (name == "strate")
        return( (qreal*)(&strate) );
    if (name == "ytrate")
        return( (qreal*)(&ytrate) );
    if (name == "Ltrate")
        return( (qreal*)(&Ltrate) );

return( NULL );
}

/******************************************************************************/
/*                                                                            */
/* Class name:      ExchangeRateModel                                         */
/* Member function: sendModelVar                                              */
/* Purpose:         returns a pointer to the qreal wage, the main model var.   */
/* Last modified:   Don Mär 14 14:29:07 CET 2002 (Michael Meyer)              */
/*                                                                            */
/******************************************************************************/

qreal* ExchangeRateModel::sendModelVar()
{
    return &wart;
	return &part;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      ExchangeRateModel                                         */
/* Member function: sendStateSpace                                            */
/* Purpose:         returns pointers to the qreal balances and the qreal wage;  */
/*                  returns the dimension of the system for rho=0             */
/* Last modified:   Don Mär 14 14:29:07 CET 2002 (Michael Meyer)              */
/*                                                                            */
/******************************************************************************/

void ExchangeRateModel::sendStateSpace(int &quantity,QList<qreal *> *stateSpace)
{
    stateSpace->clear();
    quantity = dimension;
    *stateSpace << &wart;
    *stateSpace << &part;
    *stateSpace << theta;
    *stateSpace << st_shift;
    *stateSpace << &eqrealt;
}
    
/******************************************************************************/
/*                                                                            */
/* Class name:      ExchangeRateModel                                         */
/* Member function: loadParamset                                              */
/* Purpose:         load a parameterset from a specified input file           */
/* Last modified:   Don Mär 14 14:29:07 CET 2002 (Michael Meyer)              */
/*                                                                            */
/******************************************************************************/

void ExchangeRateModel::loadParamset(QTextStream& inputFile)
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
    theta = new qreal[tau+2];
    if( !theta )
	fatalError("ExchangeRateModel::loadParamset","Can't create theta vector");
    
    initialize();
}

/******************************************************************************/
/*                                                                            */
/* Class name:      ExchangeRateModel                                         */
/* Member function: saveParamset                                              */
/* Purpose:         write parameterset into a file                            */
/* Last modified:   Don Mär 14 14:29:07 CET 2002 (Michael Meyer)              */
/*                                                                            */
/******************************************************************************/

void ExchangeRateModel::saveParamset(QTextStream& outputFile)
{
    outputFile << A << "\t" << B << "\t";
    outputFile << gamma << "\t" << kappa << "\t" << lambda << "\t" << mu << "\t";
    outputFile << tau << "\t" << length << "\t";
    outputFile << st_1_0 << st_2_0 << eerealt_1_0 << "\t";
    outputFile << alphat_0 << betat_0 << "\t";
    outputFile << Lmax << "\t" << rho << "\t" << g << "\t" << tax << "\t";
    outputFile << delta << "\t" << deltaB << "\t" << d << "\t" << E << "\t"<< fi;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      ExchangeRateModel                                         */
/* Member function: saveParamsetWithNames                                     */
/* Purpose:         add  parameterset to printfile                            */
/* Last modified:   Don Mär 14 14:29:07 CET 2002 (Michael Meyer)              */
/*                                                                            */
/******************************************************************************/

void ExchangeRateModel::saveParamsetWithNames(QTextStream& outputFile)
{    
	outputFile << "ExchangeRateModel:\n\t";
    outputFile << "A = " << A << "\tB = " << B << "\n\tgamma = ";
    outputFile << gamma << "\tkappa = " << kappa << "\tlambda = ";
    outputFile << lambda << "\tmu = " << mu << "\ttau = ";
    outputFile << tau << "\n\tlength = " << length << "\n\tst_1_0 = ";
	outputFile << st_1_0  << "\tst_2_0 = " << st_2_0 
               << "\teerealt_1_0 = " << eerealt_1_0 << "\n\talphat_0 = ";
    outputFile << alphat_0 << "\tbetat_0 = " << betat_0 << "\n\tLmax =";
    outputFile << Lmax << "\trho = " << rho << "\tg = ";
    outputFile << g << "\ttax = " << tax << "\n\tdelta = ";
    outputFile << delta << "\tdeltaB = " << deltaB << "\td = " 
				<< d << "\tE = " << E << "\tfi= " << fi;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      ExchangeRateModel                                         */
/* Member function: printParamset                                             */
/* Purpose:         print parameterset on the screen                          */
/* Last modified:   Don Mär 14 14:29:07 CET 2002 (Michael Meyer)              */
/*                                                                            */
/******************************************************************************/

void ExchangeRateModel::printParamset()
{
    log() << A << "\t" << B;
    log() << gamma << "\t" << kappa << "\t" << lambda << "\t" << mu;
    log() << tau << "\t" << length;
    log() << st_1_0 << st_2_0 << eerealt_1_0;
    log() << alphat_0 << betat_0;
    log() << Lmax << "\t" << rho << "\t" << g << "\t" << tax;
    log() << delta << "\t" << deltaB << "\t" << d << "\t" << E << "\t"<< fi;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      ExchangeRateModel                                         */
/* Member function: sendParameters                                            */
/* Purpose:         write all parameters into an array and return the numbers */
/*                  of parameters                                             */
/* Last modified:   Don Mär 14 14:29:07 CET 2002 (Michael Meyer)              */
/*                                                                            */
/******************************************************************************/

void ExchangeRateModel::sendParameters(int& amount,qreal** parameters)
{
    if( *parameters )
	delete *parameters;
    amount=17;
    *parameters=new qreal[amount];
    if( !(*parameters) )
	fatalError("ExchangeRateModel::sendParameters",
		   "Can't create array for parameters");
    (*parameters)[0]=A;
    (*parameters)[1]=B;
    (*parameters)[2]=gamma;
    (*parameters)[3]=kappa;
    (*parameters)[4]=lambda;
    (*parameters)[5]=mu;
    (*parameters)[6]=tau;
    (*parameters)[7]=length;
    (*parameters)[8]=delta;
    (*parameters)[9]=betat;
    (*parameters)[13]=Lmax;
    (*parameters)[14]=rho;
    (*parameters)[15]=g;
    (*parameters)[16]=tax;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      ExchangeRateModel                                         */
/* Member function: receiveParameters                                         */
/* Purpose:         receive parameter values                                  */
/* Last modified:   Don Mär 14 14:29:07 CET 2002 (Michael Meyer)              */
/*                                                                            */
/******************************************************************************/

void ExchangeRateModel::receiveParameters(const QList<qreal>& parameters)
{
    A=parameters[0];
    B=parameters[1];
    gamma=parameters[2];
    kappa=parameters[3];
    lambda=parameters[4];
    mu=parameters[5];
    tau=(int)(parameters[6]);
    length=(qint64)(parameters[7]);
    delta=parameters[8];
    betat=parameters[9];
    Lmax=parameters[13];
    rho=parameters[14];
    g=parameters[15];
    tax=parameters[16];
}
