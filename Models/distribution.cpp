//////////////////////////////////////////////////////////////////////////////
//																			//
// $Header: 		distribution.C											//
//																			//	
// Contents:		diverse distribution generators							//
//					1. rectangular unit 									//
//					2. normal												//
//					3. chi_spuare											//
//					4. Cauchy												//
//					5. Student T											//
//					6. exponential											//
//					7. Pareto												//
//					8. triangular
//														//
//					algorithms are taken from 								//
//					'Statistical Distributions (second edition)' 1993		//
//					written by Merran Evans, 								//
//							   Nicholas Hastings,							//
//							   Brian Peacock								//
//					hereafter refer to as EHP.								//
// Author:			Mathias Hoffmann & Michael Meyer						//
// Last modified:	22.03.05							//
// By:				Andreas Starke						//
//																			//
//////////////////////////////////////////////////////////////////////////////

#include "distribution.h"	
#include "../logger.h"
#include "time.h"

//////////////////////////////////////////////////////////////////////////////
//																			//
// $Header: 		distribution.C											//
//																			//	
// Module name:		distribution											//
// Contents:		constructor												//
//																			//
// Author:			Mathias Hoffmann & Michael Meyer						//
// Last modified:	Wed Sep 27 14:58:59 METDST 2000							//
// By:				Mathias Hoffmann & Michael Meyer						//
//																			//
//////////////////////////////////////////////////////////////////////////////

distribution::distribution(){
	newseed();
}

//////////////////////////////////////////////////////////////////////////////
//																			//
// $Header: 		distribution.C											//
//																			//	
// Module name:		distribution											//
// Contents:		destructor												//
//																			//
// Author:			Mathias Hoffmann & Michael Meyer						//
// Last modified:	Wed Sep 27 14:58:59 METDST 2000							//
// By:				Mathias Hoffmann & Michael Meyer						//
//																			//
//////////////////////////////////////////////////////////////////////////////

distribution::~distribution(){
}

//////////////////////////////////////////////////////////////////////////////
//																			//
// $Header: 		distribution.C											//
//																			//
// Member function:	newseed													//
// Contents:		generates new seed		 								//
//																			//
// Author:			Mathias Hoffmann & Michael Meyer						//
// Last modified:	Wed Sep 27 14:58:59 METDST 2000							//
// By:				Mathias Hoffmann & Michael Meyer						//
//																			//
//////////////////////////////////////////////////////////////////////////////

void distribution::newseed(){
    srand ((uint) time(&t));
}

//////////////////////////////////////////////////////////////////////////////
//																			//
// $Header: 		distribution.C											//
//																			//
// Member function:	setseed													//
// Contents:		sets the seed		 								//
//																			//
// Author:			Mathias Hoffmann & Michael Meyer						//
// Last modified:	Wed Sep 27 14:58:59 METDST 2000							//
// By:				Mathias Hoffmann & Michael Meyer						//
//																			//
//////////////////////////////////////////////////////////////////////////////


void distribution::setseed( uint val )
{
	srand ( val );
}

//////////////////////////////////////////////////////////////////////////////
//																			//
// $Header: 		distribution.C											//
//																			//
// Member function:	rectangular												//
// Contents:		generates rectangular unit variate	 					//
//																			//
//					R in (0,1)												//
//																			//
// Author:			Mathias Hoffmann & Michael Meyer						//
// Last modified:	Wed Sep 27 14:58:59 METDST 2000							//
// By:				Mathias Hoffmann & Michael Meyer						//
//																			//
//////////////////////////////////////////////////////////////////////////////

qreal distribution::rectangular(){
	static qreal random;

	do {
        random = rand()/qreal(RAND_MAX);
	} while((random==0)||(random==1));

	return random;
} 

//////////////////////////////////////////////////////////////////////////////
//																			//
// $Header: 		distribution.C											//
//																			//
// Member function:	triangular
// Contents:		generates triangular variate	 					//
//																			//
//					R in (a,b) with modus c												//
//																			//
// Author:			Andreas Starke
// Last modified:	
// By:				
//																			//
//////////////////////////////////////////////////////////////////////////////

qreal distribution::triangular(const qreal a, const qreal b, const qreal c)
{
	static qreal random;
	static qreal width, FL;
	
	random = rectangular();
	width = b-a;
	if ( width == 0 ){
		return a;
	} else {
		FL = (c-a)/(b-a);
		if ( random <= FL ){
			random = random*width*(c-a);
			random = a + sqrt(random);
		} else {
			random = (1.0-random)*width*(b-c);
			random = b - sqrt(random);
		};
	};
	

	return random;
}

//////////////////////////////////////////////////////////////////////////////
//																			//
// $Header: 		distribution.C											//
//																			//
// Member function:	st_normal												//
// Contents:		generates standard normal random variate				//
//																			//
//					R1, R2 denote independent rectangular unit variates		//
//					Two independent standard normal variates are generated	//
//					by		Box-Mueller method								//
//																			//
//					EHP p.118												//
// Author:			Mathias Hoffmann & Michael Meyer						//
// Last modified:	Wed Sep 27 14:58:59 METDST 2000							//
// By:				Mathias Hoffmann & Michael Meyer						//
//																			//
//////////////////////////////////////////////////////////////////////////////

qreal distribution::st_normal(){
	static qreal rec1;
	static qreal rec2;
	rec1 = rectangular();
	rec2 = rectangular();

	return sqrt(-2*log(rec1))*sin(2*M_PI*rec2);		//second variate discarded
}

//////////////////////////////////////////////////////////////////////////////
//																			//
// $Header: 		distribution.C											//
//																			//
// Member function:	chi_square												//
// Contents:		generates a chi square (df) random variate				//
//																			//
//					Let X_i denote a standard normal variate.				//
//					Then sum((X_i)^2), i=1,...,df							//
//						,where df is the degrees of freedom					//
//					follows a chi square (df) distribution.					//
//					EHP p.49												//
//					For df >= 150 convergence to standard normal			//
//																			//
//																			//
//																			//
//																			//
// Author:			Mathias Hoffmann & Michael Meyer						//
// Last modified:	Wed Sep 27 14:58:59 METDST 2000							//
// By:				Mathias Hoffmann & Michael Meyer						//
//																			//
//////////////////////////////////////////////////////////////////////////////

qreal distribution::chi_square(const int degree) {	
	static qreal chi;
	chi=0;
	static qreal norm;

	if (degree <= 0) 
		fatalError("distribution::chi_square: ","degrees of freedom <= 0");
//	if (degree >= 150) return st_normal();    ???
	for(int i=1; i<=degree; i++){	
		norm = st_normal();
		chi += norm * norm;
	}
	return chi;
}

//////////////////////////////////////////////////////////////////////////////
//																			//
// $Header: 		distribution.C											//
//																			//
// Member function:	st_cauchy												//
// Contents:		generates standard Cauchy random variate				//
//																			//
//					Let R denote a rectangular unit variate.				//
//					Then tan(PI*(R-0.5)) follows a standard Cauchy 			//
//					distribution.											//
//																			//
//					EHP p.44												//
// Author:			Mathias Hoffmann & Michael Meyer						//
// Last modified:	Wed Sep 27 14:58:59 METDST 2000							//
// By:				Mathias Hoffmann & Michael Meyer						//
//																			//
//////////////////////////////////////////////////////////////////////////////

//bis pi/2 - 9e+15 und -pi/2 + 9e+15 rechnet der tan noch genau

qreal distribution::st_cauchy(){
	static qreal rec1;

	do {
		rec1=rectangular();
	} while((rec1<9e-15) || (1-rec1<9e-15));

	return tan(M_PI*(rec1-0.5));
}

//////////////////////////////////////////////////////////////////////////////
//																			//
// $Header: 		distribution.C											//
//																			//
// Member function:	student_t												//
// Contents:		generates Student (T) random variate					//
//																			//
//					Let N denote a standard normal variate, X chi-square	//
//					vaiate. Then N/sqrt(X/degree) follows a Student (T)		//
//					distribution.											//
//					For T >= 150 convergence to standard normal				//
//																			//
//					EHP p.146												//
// Author:			Mathias Hoffmann & Michael Meyer						//
// Last modified:	Wed Sep 27 14:58:59 METDST 2000							//
// By:				Mathias Hoffmann & Michael Meyer						//
//																			//
//////////////////////////////////////////////////////////////////////////////

qreal distribution::student_t(const int degree){
	static qreal norm;
	static qreal chi;

	if (degree >= 150) return st_normal();
	norm=st_normal();
	chi=chi_square(degree);
	return norm/sqrt(chi/degree);
}

//////////////////////////////////////////////////////////////////////////////
//																			//
// $Header: 		distribution.C											//
//																			//
// Member function:	exponential												//
// Contents:		generates a exponential (b) random variate				//
//																			//
//					Let R denote a rectangular unit variate.				//
//					Then -b*ln(R) follows an exponential distribution.		//
//																			//
//					EHP p.62												//
// Author:			Mathias Hoffmann & Michael Meyer						//
// Last modified:	Wed Sep 27 14:58:59 METDST 2000							//
// By:				Mathias Hoffmann & Michael Meyer						//
//																			//
//////////////////////////////////////////////////////////////////////////////

qreal distribution::exponential(const qreal b){
	static qreal rec1;

	if (b <= 0) 
		fatalError("distribution::exponential: ","b <= 0");
	rec1=rectangular();
	return -b*log(rec1);
}

//////////////////////////////////////////////////////////////////////////////
//																			//
// $Header: 		distribution.C											//
//																			//
// Member function:	pareto													//
// Contents:		generates a Pareto (a,c) random variate					//
//																			//
//					Let R denote a rectangular unit variate.				//
//					Then a*(1-R)^(-1/c) follows a Pareto (a,c) distribution.//
//																			//
//					EHP p.122												//
// Author:			Mathias Hoffmann & Michael Meyer						//
// Last modified:	Wed Sep 27 14:58:59 METDST 2000							//
// By:				Mathias Hoffmann & Michael Meyer						//
//																			//
//////////////////////////////////////////////////////////////////////////////

qreal distribution::pareto(const qreal a, const qreal c){
	static qreal rec1;
	static qreal buffer;

	if (a <= 0) 
		fatalError("distribution::pareto: ","a <= 0");
	if (c <= 0)
		fatalError("distribution::pareto: ","c <= 0");
	rec1 = rectangular();
	buffer = -1/c;
	return a*pow((1-rec1),buffer);
}



// end of file
