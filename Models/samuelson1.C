///////////////////////////////////////////////////////////////////////////////
//
// $Header: samuelson1.C, Thu Jul  6 17:09:23 METDST 2000 mmeyer 
//
// Module name:		samuelson1.C
// Contents:		Member functions of the classes
//                       
//
// Author:		
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

#include "../error.h"
#include "samuelson1.h"

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		
// Member function:	
// Purpose:		constructor
//
// Author:		
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

samuelson1::samuelson1() : baseModel(2)
{
	A = new matrixDef(2,2);
	if( !A )
		fatalError("matrixDef::matrixDef","Can't allocate memory");
	C = new matrixDef(2,1);
	if( !C )
		fatalError("matrixDef::matrixDef","Can't allocate memory");
	D = new matrixDef(2,1);
	if( !D )
		fatalError("matrixDef::matrixDef","Can't allocate memory");
	b = new matrixDef(2,1);
	if( !b )
		fatalError("matrixDef::matrixDef","Can't allocate memory");
	x = new matrixDef(2,1);
	if( !x )
		fatalError("matrixDef::matrixDef","Can't allocate memory");
	V=NULL;	
	point = new matrixFn();
	if( !point )
		fatalError("matrixDef::matrixDef","Can't allocate memory");
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		
// Member function:	
// Purpose:		destructor
//
// Author:		
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

samuelson1::~samuelson1() 
{
	if(A)	delete A;
	if(C)	delete C;
	if(D)	delete D;
	if(b)	delete b;	
	if(x)	delete x;	
	if(point)	delete point;
	if(V)	delete V;
}
///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		
// Member function:	loadParamset
// Purpose:		load a parameterset from a specified input file
//
// Author:		
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

void samuelson1::loadParamset(ifstream& inFile)
{
    inFile >> N;
    inFile >> x01;
    inFile >> x02;

	for(;1;) {
	inFile >> p1 >> m1 >> v1 >> m01 >> v01;
	if (N==1) break;
	inFile >> p2 >> m2 >> v2 >> m02 >> v02;
	if (N==2) break;
	inFile >> p3 >> m3 >> v3 >> m03 >> v03;
	if (N==3) break;
	inFile >> p4 >> m4 >> v4 >> m04 >> v04;
	if (N==4) break;
	inFile >> p5 >> m5 >> v5 >> m05 >> v05;
	if (N==5) break;
	inFile >> p6 >> m6 >> v6 >> m06 >> v06;
	if (N==6) break;
	inFile >> p7 >> m7 >> v7 >> m07 >> v07;
	if (N==7) break;
	inFile >> p8 >> m8 >> v8 >> m08 >> v08;
	if (N==8) break;
	inFile >> p9 >> m9 >> v9 >> m09 >> v09;
	if (N==9) break;
	inFile >> p10 >> m10 >> v10 >> m010 >> v010;
	if (N==10) break;
	} 

  	inFile >> length;

 	initialize();  
}

    
///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		
// Member function:	initialize
// Purpose:		initialize the model, define the systems initial state
//
// Author:		
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

void samuelson1::initialize()
{
    qreal sum_p=0;

	V = new matrixDef(N,5);
	if( !V )
		fatalError("matrixDef::matrixDef","Can't allocate memory");
	
	for(;1;) {
		V->m[0][0]= p1;V->m[0][1]=m1;V->m[0][2]=v1;V->m[0][3]=m01;V->m[0][4]=v01;
		if (N==1) break;
		V->m[1][0]= p2;V->m[1][1]=m2;V->m[1][2]=v2;V->m[1][3]=m02;V->m[1][4]=v02;		
		if (N==2) break;
		V->m[2][0]= p3;V->m[2][1]=m3;V->m[2][2]=v3;V->m[2][3]=m03;V->m[2][4]=v03;		
		if (N==3) break;
		V->m[3][0]= p4;V->m[3][1]=m4;V->m[3][2]=v4;V->m[3][3]=m04;V->m[3][4]=v04;		
		if (N==4) break;
		V->m[4][0]= p5;V->m[4][1]=m5;V->m[4][2]=v5;V->m[4][3]=m05;V->m[4][4]=v05;		
		if (N==5) break;
		V->m[5][0]= p6;V->m[5][1]=m6;V->m[5][2]=v6;V->m[5][3]=m06;V->m[5][4]=v06;		
		if (N==6) break;
		V->m[6][0]= p7;V->m[6][1]=m7;V->m[6][2]=v7;V->m[6][3]=m07;V->m[6][4]=v07;		
		if (N==7) break;
		V->m[7][0]= p8;V->m[7][1]=m8;V->m[7][2]=v8;V->m[7][3]=m08;V->m[7][4]=v08;		
		if (N==8) break;
		V->m[8][0]= p9;V->m[8][1]=m9;V->m[8][2]=v9;V->m[8][3]=m09;V->m[8][4]=v09;		
		if (N==9) break;
		V->m[9][0]= p10;V->m[9][1]=m10;V->m[9][2]=v10;V->m[9][3]=m010;V->m[9][4]=v010;		
		if (N==10) break;
	}

	x1_tp1 = x01;
 	x2_tp1 = x02;

	for (int i=0;i<=N-2;i++) {				// p[N]=1-sum(p[1],...,p[N-1]
		sum_p += V->m[i][0];  
	}
	V->m[N-1][0] = 1-sum_p;

	check();								// all values in range??
	if ( zvar != NULL ) delete zvar;
	zvar = new rand_var( this, "ranf","1[0,1];");
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		samuelson1
// Member function:	check
// Purpose:		checks if all entered values are in range
//
// Author:		
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

void samuelson1::check()
{
	flag=1;						// flag==1  =>  everything is ok
	for (int i=0;i<=N-1;i++) {
		if ((V->m[i][0]<0) || (V->m[i][0]>1)) {
            Log::log() << "probability out of range!";
			flag=0;
			break;
		}
		if ((V->m[i][1]<0) || (V->m[i][1]>=1)) {
            Log::log() << "m out of range";
			flag=0;
			break;
		}
		if ((V->m[i][2]<0) || (V->m[i][2]>4)) {
            Log::log() << "v out of range";
			flag=0;
			break;
		}
		if (V->m[i][3]<0) {
            Log::log() << "m0 out of range";
			flag=0;
			break;
		}
		if (V->m[i][4]<0) {
            Log::log() << "v0 out of range";
			flag=0;
			break;
		}
	}		
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		samuelson1
// Member function:	setLabels
// Purpose:		return a pointer to a variable or a parameter specified
//
// Author:		
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

qreal* samuelson1::setLabels(char* label)
{
    if( !strcmp(label,"x01") ) return(&x01);
    if( !strcmp(label,"x02") ) return(&x02);
    if( !strcmp(label,"x1_tp1") ) return(&x1_tp1);
    if( !strcmp(label,"x2_tp1") ) return(&x2_tp1);
    if( !strcmp(label,"p1") ) return(&p1);
    if( !strcmp(label,"m01") ) return(&m01);
    if( !strcmp(label,"v01") ) return(&v01);
    if( !strcmp(label,"m1") ) return(&m1);
    if( !strcmp(label,"v1") ) return(&v1);
    if( !strcmp(label,"p2") ) return(&p2);
    if( !strcmp(label,"m02") ) return(&m02);
    if( !strcmp(label,"v02") ) return(&v02);
    if( !strcmp(label,"m2") ) return(&m2);
    if( !strcmp(label,"v2") ) return(&v2);
    if( !strcmp(label,"p3") ) return(&p3);
    if( !strcmp(label,"m03") ) return(&m03);
    if( !strcmp(label,"v03") ) return(&v03);
    if( !strcmp(label,"m3") ) return(&m3);
    if( !strcmp(label,"v3") ) return(&v3);
    if( !strcmp(label,"p4") ) return(&p4);
    if( !strcmp(label,"m04") ) return(&m04);
    if( !strcmp(label,"v04") ) return(&v04);
    if( !strcmp(label,"m4") ) return(&m4);
    if( !strcmp(label,"v4") ) return(&v4);
    if( !strcmp(label,"p5") ) return(&p5);
    if( !strcmp(label,"m05") ) return(&m05);
    if( !strcmp(label,"v05") ) return(&v05);
    if( !strcmp(label,"m5") ) return(&m5);
    if( !strcmp(label,"v5") ) return(&v5);
    if( !strcmp(label,"p6") ) return(&p6);
    if( !strcmp(label,"m06") ) return(&m06);
    if( !strcmp(label,"v06") ) return(&v06);
    if( !strcmp(label,"m6") ) return(&m6);
    if( !strcmp(label,"v6") ) return(&v6);
    if( !strcmp(label,"p7") ) return(&p7);
    if( !strcmp(label,"m07") ) return(&m07);
    if( !strcmp(label,"v07") ) return(&v07);
    if( !strcmp(label,"m7") ) return(&m7);
    if( !strcmp(label,"v7") ) return(&v7);
    if( !strcmp(label,"p8") ) return(&p8);
    if( !strcmp(label,"m08") ) return(&m08);
    if( !strcmp(label,"v08") ) return(&v08);
    if( !strcmp(label,"m8") ) return(&m8);
    if( !strcmp(label,"v8") ) return(&v8);
    if( !strcmp(label,"p9") ) return(&p9);
    if( !strcmp(label,"m09") ) return(&m09);
    if( !strcmp(label,"v09") ) return(&v09);
    if( !strcmp(label,"m9") ) return(&m9);
    if( !strcmp(label,"v9") ) return(&v9);
    if( !strcmp(label,"p10") ) return(&p10);
    if( !strcmp(label,"m010") ) return(&m010);
    if( !strcmp(label,"v010") ) return(&v010);
    if( !strcmp(label,"m10") ) return(&m10);
    if( !strcmp(label,"v10") ) return(&v10);
    if( !strcmp(label,"xBundle") ) return(&xBundle);

    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		samuelson1
// Member function:	iteration
// Purpose:		perform one iteration of the system
//
// Author:		
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

void samuelson1::iteration(const long& )
{ 
if (flag) {					// all values are in range	
    qreal sum=V->m[0][0];	// sum is used to find out which intervall was hit

	z = zvar->dice();				// get random number in [0,1]
	for(int i=0;i<=N-1;i++) {
		if (z<=sum) {			// if z<=sum,row i is the currently drawn one
			A->m[0][0] = 0; 	// so take A[i] and b[i]
			A->m[0][1] = 1;
			A->m[1][0] = -(V->m[i][2]);
			A->m[1][1] = (V->m[i][1]) + (V->m[i][2]);
			b->m[0][0] = 0;
			b->m[1][0] = (V->m[i][3]) + (V->m[i][4]);
		break;
		}
	sum += V->m[i+1][0];		// go to next intervall
	}

	x->m[0][0] = x1_tp1;
	x->m[1][0] = x2_tp1;

	point->multiply(A,x,C);	// C = A*x
	point->add(C,b,D);		// C = A*x + b

	x1_tp1 = D->m[0][0];		// save values for plotting	
	x2_tp1 = D->m[1][0];

	}
else {				// some values were out of range
x1_tp1 = 0;
x2_tp1 = 0;
}
						
	x->m[0][0] = x1_tp1;		// shifting for the next period
	x->m[1][0] = x2_tp1;

for(;1;) {					// correct the value of pN
	if (N==1) {
		p1 = V->m[0][0];
		break;
	}
	if (N==2){
		p2 = V->m[1][0];
		break;
	}
	if (N==3) {
		p3 = V->m[2][0];
		break;
	}
	if (N==4) {
		p4 = V->m[3][0];
		break;
	}
	if (N==5) {
		p5 = V->m[4][0];
		break;
	}
	if (N==6) {
		p6 = V->m[5][0];
		break;
	}
	if (N==7) {
		p7 = V->m[6][0];
		break;
	}
	if (N==8) {
		p8 = V->m[7][0];
		break;
	}
	if (N==9) {
		p9 = V->m[8][0];
		break;
	}
	if (N==10) {
		p10 = V->m[9][0];
		break;
	}
}
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		
// Member function:	saveParamset
// Purpose:		write parameterset into a file
//
// Author:		
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

void samuelson1::saveParamset(ofstream& outFile)
{
    outFile << N << "\t";
    outFile << x01 << "\t";
    outFile << x02 << "\t";

	for(int i=0; i<=N-1;i++) {
		for(int j=0;j<=4;j++) {
			outFile << V->m[i][j] << "\t";
		}	
	}

    outFile << length;

}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		
// Member function:	printParamset
// Purpose:		print parameterset on the screen
//
// Author:		
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

void samuelson1::printParamset()
{
    Log::log() << N  << endl;
    Log::log() << x01 << endl;
    Log::log() << x02 << endl;

	for(int i=0; i<=N-1;i++) {
		for(int j=0;j<=4;j++) {
            Log::log() << V->m[i][j] << "\t";
		}	
	}

    Log::log() << length << endl;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		
// Member function:	sendStateSpace
// Purpose:		return pointers to the state variables and inform about
//			the systems dimension
//
// Author:		
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

void samuelson1::sendStateSpace(int &quantity,const qreal*** stateSpace)
{
    if( stateSpace )
	delete stateSpace;
    *stateSpace= new const qreal* [dimension];
    if( !(*stateSpace) )
	fatalError("samuelson1::sendStateSpace",
		   "Can't create state space vector");
    quantity=dimension;
    (*stateSpace)[0]=&x01;   
    (*stateSpace)[1]=&x02;

};
///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		
// Member functions:	sendModelVar, sendParameters, receiveParameters
// Purpose:		for compatibity reasons only
//
// Author:		
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////
qreal* samuelson1::sendModelVar(void)
{ error("macrodyn::samuelson1::sendModelVar is not implemented");
  return NULL;
} 
void samuelson1::sendParameters(int& ,qreal** )
{ error("macrodyn::samuelson1::sendParameters is not implemented");
}
void samuelson1::receiveParameters(const qreal* )
{ error("macrodyn::samuelson1::receiveParameters is not implemented");
} 
