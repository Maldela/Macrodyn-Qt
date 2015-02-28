///////////////////////////////////////////////////////////////////////////////
//
// $Header: frepm.C, Thu Jul  6 17:09:23 METDST 2000 mmeyer 
//
// Module name:		frepm.C
// Contents:		Member functions of the classes
//                       
//
// Author:		
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

#include "../logger.h"
#include "frepm.h"

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

frepm::frepm() : baseModel(2)
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

frepm::~frepm() 
{
	if(A)		delete A;
	if(C)		delete C;
	if(D)		delete D;
	if(b)		delete b;	
	if(x)		delete x;	
	if(point)	delete point;
	if(V)		delete V;
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

void frepm::loadParamset(QTextStream& inFile)
{

    inFile >> N;
    inFile >> x01;
    inFile >> x02;

	for(;1;) {
	inFile >> p1 >> m11 >> m21 >> c11 >>  c21 >> i11 >> i21 >> m011 >> m021 >> i011 >> i021;
	if (N==1) break;
	inFile >> p2 >> m12 >> m22 >> c12 >>  c22 >> i12 >> i22 >> m012 >> m022 >> i012 >> i022; 
	if (N==2) break;
	inFile >> p3 >> m13 >> m23 >> c13 >>  c23 >> i13 >> i23 >> m013 >> m023 >> i013 >> i023; 
	if (N==3) break;
	inFile >> p4 >> m14 >> m24 >> c14 >>  c24 >> i14 >> i24 >> m014 >> m024 >> i014 >> i024; 
	if (N==4) break;
	inFile >> p5 >> m15 >> m25 >> c15 >>  c25 >> i15 >> i25 >> m015 >> m025 >> i015 >> i025; 
	if (N==5) break;
	inFile >> p6 >> m16 >> m26 >> c16 >>  c26 >> i16 >> i26 >> m016 >> m026 >> i016 >> i026; 
	if (N==6) break;
	inFile >> p7 >> m17 >> m27 >> c17 >>  c27 >> i17 >> i27 >> m017 >> m027 >> i017 >> i027; 
	if (N==7) break;
	inFile >> p8 >> m18 >> m28 >> c18 >>  c28 >> i18 >> i28 >> m018 >> m028 >> i018 >> i028; 
	if (N==8) break;
	inFile >> p9 >> m19 >> m29 >> c19 >>  c29 >> i19 >> i29 >> m019 >> m029 >> i019 >> i029; 
	if (N==9) break;
	inFile >> p10 >> m110 >> m210 >> c110 >>  c210 >> i110 >> i210 >> m0110 >> m0210 >> i0110 >> i0210; 
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

void frepm::initialize()
{
	qreal sum_p=0;

	V = new matrixDef(N,11);
	if( !V )
		fatalError("matrixDef::matrixDef","Can't allocate memory");
	
	for(;1;) {
		V->m[0][0]= p1;V->m[0][1]=m11; V->m[0][2]=m21; V->m[0][3]=c11; V->m[0][4]=c21;V->m[0][5]=i11;
		V->m[0][6]=i21;V->m[0][7]=m011;V->m[0][8]=m021;V->m[0][9]=i011;V->m[0][10]=i021;
		if (N==1) break;
		V->m[1][0]= p2;V->m[1][1]=m12; V->m[1][2]=m22; V->m[1][3]=c12; V->m[1][4]=c22;V->m[1][5]=i12;
		V->m[1][6]=i22;V->m[1][7]=m012;V->m[1][8]=m022;V->m[1][9]=i012;V->m[1][10]=i022;		
		if (N==2) break;
		V->m[2][0]= p3;V->m[2][1]=m13; V->m[2][2]=m23; V->m[2][3]=c13; V->m[2][4]=c23;V->m[2][5]=i13;
		V->m[2][6]=i23;V->m[2][7]=m013;V->m[2][8]=m023;V->m[2][9]=i013;V->m[2][10]=i023;		
		if (N==3) break;
		V->m[3][0]= p4;V->m[3][1]=m14; V->m[3][2]=m24; V->m[3][3]=c14; V->m[3][4]=c24;V->m[3][5]=i14;
		V->m[3][6]=i24;V->m[3][7]=m014;V->m[3][8]=m024;V->m[3][9]=i014;V->m[3][10]=i024;		
		if (N==4) break;
		V->m[4][0]= p5;V->m[4][1]=m15; V->m[4][2]=m25; V->m[4][3]=c15; V->m[4][4]=c25;V->m[4][5]=i15;
		V->m[4][6]=i25;V->m[4][7]=m015;V->m[4][8]=m025;V->m[4][9]=i015;V->m[4][10]=i025;		
		if (N==5) break;
		V->m[5][0]= p6;V->m[5][1]=m16; V->m[5][2]=m26; V->m[5][3]=c16; V->m[5][4]=c26;V->m[5][5]=i16;
		V->m[5][6]=i26;V->m[5][7]=m016;V->m[5][8]=m026;V->m[5][9]=i016;V->m[5][10]=i026;		
		if (N==6) break;
		V->m[6][0]= p7;V->m[6][1]=m17; V->m[6][2]=m27; V->m[6][3]=c17; V->m[6][4]=c27;V->m[6][5]=i17;
		V->m[6][6]=i27;V->m[6][7]=m017;V->m[6][8]=m027;V->m[6][9]=i017;V->m[6][10]=i027;		
		if (N==7) break;
		V->m[7][0]= p8;V->m[7][1]=m18; V->m[7][2]=m28; V->m[7][3]=c18; V->m[7][4]=c28;V->m[7][5]=i18;
		V->m[7][6]=i28;V->m[7][7]=m018;V->m[7][8]=m028;V->m[7][9]=i018;V->m[7][10]=i028;		
		if (N==8) break;
		V->m[8][0]= p9;V->m[8][1]=m19; V->m[8][2]=m29; V->m[8][3]=c19; V->m[8][4]=c29;V->m[8][5]=i19;
		V->m[8][6]=i29;V->m[8][7]=m019;V->m[8][8]=m029;V->m[8][9]=i019;V->m[8][10]=i029;		
		if (N==9) break;
		V->m[9][0]= p10;V->m[9][1]=m110; V->m[9][2]=m210; V->m[9][3]=c110; V->m[9][4]=c210;V->m[9][5]=i110;
		V->m[9][6]=i210;V->m[9][7]=m0110;V->m[9][8]=m0210;V->m[9][9]=i0110;V->m[9][10]=i0210;		
		if (N==10) break;
	}

	x1_tp1 = x01; 
	x2_tp1 = x02;

    for (uint i=0;i<=N-2;i++) {				// p[N]=1-sum(p[1],...,p[N-1]
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
// Class name:		frepm
// Member function:	check
// Purpose:		checks if all entered values are in range
//
// Author:		
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

void frepm::check()
{
	flag=1;						// flag==1  =>  everything is ok
    for (uint i=0;i<=N-1;i++) {
		if ((V->m[i][0]<0) || (V->m[i][0]>1)) {
			log() << "probability out of range!";
			flag=0;
			break;
		}
		if ((V->m[i][1]<0) || (V->m[i][1]>=1)) {
			log() << "m1 out of range";
			flag=0;
			break;
		}
		if ((V->m[i][2]<0) || (V->m[i][2]>=1)) {
			log() << "m2 out of range";
			flag=0;
			break;
		}
		if ((V->m[i][3]<0) || (V->m[i][3]>=1))  {
			log() << "c1 out of range";
			flag=0;
			break;
		}
		if ((V->m[i][4]<0) || (V->m[i][4]>=1))  {
			log() << "c2 out of range";
			flag=0;
			break;
		}
		if ((V->m[i][5]<0) || (V->m[i][5]>=1))  {
			log() << "i1 out of range";
			flag=0;
			break;
		}
		if ((V->m[i][6]<0) || (V->m[i][6]>=1))  {
			log() << "i2 out of range";
			flag=0;
			break;
		}
		if (V->m[i][7]<0) {
			log() << "m01 out of range";
			flag=0;
			break;
		}
		if (V->m[i][8]<0) {
			log() << "m02 out of range";
			flag=0;
			break;
		}
		if (V->m[i][9]<0) {
			log() << "i01 out of range";
			flag=0;
			break;
		}
		if (V->m[i][10]<0) {
			log() << "i02 out of range";
			flag=0;
			break;
		}
	}		
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		frepm
// Member function:	setLabels
// Purpose:		return a pointer to a variable or a parameter specified
//
// Author:		
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

qreal* frepm::setLabels(const QString& label)
{
    if (label == "x01") return(&x01);
    if (label == "x02") return(&x02);
    if (label == "x1_tp1") return(&x1_tp1);
    if (label == "x2_tp1") return(&x2_tp1);
    if (label == "p1")  return(&p1);
    if (label == "m11") return(&m11);
    if (label == "m21") return(&m21);
    if (label == "c11") return(&c11);
    if (label == "c21") return(&c21);
    if (label == "i11") return(&i11);
    if (label == "i21") return(&i21);
    if (label == "m011") return(&m011);
    if (label == "m021") return(&m021);
    if (label == "i011") return(&i011);
    if (label == "i021") return(&i021);
    if (label == "p2")  return(&p2);
    if (label == "m12") return(&m12);
    if (label == "m22") return(&m22);
    if (label == "c12") return(&c12);
    if (label == "c22") return(&c22);
    if (label == "i12") return(&i12);
    if (label == "i22") return(&i22);
    if (label == "m012") return(&m012);
    if (label == "m022") return(&m022);
    if (label == "i012") return(&i012);
    if (label == "i022") return(&i022);
    if (label == "p3")  return(&p3);
    if (label == "m13") return(&m13);
    if (label == "m23") return(&m23);
    if (label == "c13") return(&c13);
    if (label == "c23") return(&c23);
    if (label == "i13") return(&i13);
    if (label == "i23") return(&i23);
    if (label == "m013") return(&m013);
    if (label == "m023") return(&m023);
    if (label == "i013") return(&i013);
    if (label == "i023") return(&i023);
    if (label == "p4")  return(&p4);
    if (label == "m14") return(&m14);
    if (label == "m24") return(&m24);
    if (label == "c14") return(&c14);
    if (label == "c24") return(&c24);
    if (label == "i14") return(&i14);
    if (label == "i24") return(&i24);
    if (label == "m014") return(&m014);
    if (label == "m024") return(&m024);
    if (label == "i014") return(&i014);
    if (label == "i024") return(&i024);
    if (label == "p5")  return(&p5);
    if (label == "m15") return(&m15);
    if (label == "m25") return(&m25);
    if (label == "c15") return(&c15);
    if (label == "c25") return(&c25);
    if (label == "i15") return(&i15);
    if (label == "i25") return(&i25);
    if (label == "m015") return(&m015);
    if (label == "m025") return(&m025);
    if (label == "i015") return(&i015);
    if (label == "i025") return(&i025);
    if (label == "p6") return(&p6);
    if (label == "m16") return(&m16);
    if (label == "m26") return(&m26);
    if (label == "c16") return(&c16);
    if (label == "c26") return(&c26);
    if (label == "i16") return(&i16);
    if (label == "i26") return(&i26);
    if (label == "m016") return(&m016);
    if (label == "m026") return(&m026);
    if (label == "i016") return(&i016);
    if (label == "i026") return(&i026);
    if (label == "p7") return(&p7);
    if (label == "m17") return(&m17);
    if (label == "m27") return(&m27);
    if (label == "c17") return(&c17);
    if (label == "c27") return(&c27);
    if (label == "i17") return(&i17);
    if (label == "i27") return(&i27);
    if (label == "m017") return(&m017);
    if (label == "m027") return(&m027);
    if (label == "i017") return(&i017);
    if (label == "i027") return(&i027);
    if (label == "p8") return(&p8);
    if (label == "m18") return(&m18);
    if (label == "m28") return(&m28);
    if (label == "c18") return(&c18);
    if (label == "c28") return(&c28);
    if (label == "i18") return(&i18);
    if (label == "i28") return(&i28);
    if (label == "m018") return(&m018);
    if (label == "m028") return(&m028);
    if (label == "i018") return(&i018);
    if (label == "i028") return(&i028);
    if (label == "p9") return(&p9);
    if (label == "m19") return(&m19);
    if (label == "m29") return(&m29);
    if (label == "c19") return(&c19);
    if (label == "c29") return(&c29);
    if (label == "i19") return(&i19);
    if (label == "i29") return(&i29);
    if (label == "m019") return(&m019);
    if (label == "m029") return(&m029);
    if (label == "i019") return(&i019);
    if (label == "i029") return(&i029);
    if (label == "p10") return(&p10);
    if (label == "m110") return(&m110);
    if (label == "m210") return(&m210);
    if (label == "c110") return(&c110);
    if (label == "c210") return(&c210);
    if (label == "i110") return(&i110);
    if (label == "i210") return(&i210);
    if (label == "m0110") return(&m0110);
    if (label == "m0210") return(&m0210);
    if (label == "i0110") return(&i0110);
    if (label == "i0210") return(&i0210);
  
   
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		frepm
// Member function:	iteration
// Purpose:		perform one iteration of the system
//
// Author:		
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

void frepm::iteration(const qint64& )
{ 
if (flag) {					// all values are in range	
	qreal sum=V->m[0][0];	// sum is used to find out which intervall was hit

	z = zvar->dice();				// get random number in [0,1]
    for (uint i=0;i<=N-1;i++) {
		if (z<=sum) {			// if z<=sum,row i is the currently drawn one
			A->m[0][0] = V->m[i][3] + V->m[i][5] - V->m[i][1]; 	// so take A[i] and b[i]
			A->m[0][1] = V->m[i][2];
			A->m[1][0] = V->m[i][1];
			A->m[1][1] = V->m[i][4] + V->m[i][6] - V->m[i][2];
			b->m[0][0] = V->m[i][9] + V->m[i][8] - V->m[i][7];
			b->m[1][0] = V->m[i][10] + V->m[i][7] - V->m[i][8];
		break;
		}
	sum += V->m[i+1][0];		// go to next intervall
	}

	x->m[0][0] = x1_tp1;
	x->m[1][0] = x2_tp1;

	point->multiply(A,x,C);		// C = A*x
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

void frepm::saveParamset(QTextStream& outFile)
{
    outFile << N << "\t";
    outFile << x01 << "\t";
    outFile << x02 << "\t";

    for (uint i=0; i<=N-1;i++) {
		for(int j=0;j<=10;j++) {
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

void frepm::printParamset()
{
    log() << N;
    log() << x01;
    log() << x02;

    for (uint i=0; i<=N-1;i++) {
		for(int j=0;j<=10;j++) {
			log() << V->m[i][j] << "\t";
		}	
	}

    log() << length;
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

void frepm::sendStateSpace(int &quantity,QList<qreal *> *stateSpace)
{
    stateSpace->clear();
    quantity = dimension;
    *stateSpace << &x01;   
    *stateSpace << &x02;
}
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
qreal* frepm::sendModelVar(void)
{ error("macrodyn::frepm::sendModelVar is not implemented");
  return NULL;
} 
void frepm::sendParameters(int& ,qreal** )
{ error("macrodyn::frepm::sendParameters is not implemented");
}
void frepm::receiveParameters(const QList<qreal>&)
{ error("macrodyn::frepm::receiveParameters is not implemented");
} 
