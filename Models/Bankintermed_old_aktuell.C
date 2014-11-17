///////////////////////////////////////////////////////////////////////////////
//
// $Header: Bankintermed.C Fri Feb  2 11:42:19 MET 2001 mmeyer 
//
// Module name:		Bankintermed.C
// Contents:		Member functions of the classes
//                       
//
// Author:			Michael Meyer
// Last modified:	Tue Feb  6 11:11:09 MET 2001
// By:				Tue Feb  6 11:11:13 MET 2001
//
///////////////////////////////////////////////////////////////////////////////

#include "../error.h"
#include "Bankintermed.h"

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		Bankintermed	
// Member function:	
// Purpose:			constructor
//
// Author:			Michael Meyer
// Last modified:	Tue Feb  6 11:11:51 MET 2001
// By:				Tue Feb  6 11:11:53 MET 2001
//
///////////////////////////////////////////////////////////////////////////////

Bankintermed::Bankintermed() : baseModel(1)
{
zvar=NULL;	
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		Bankintermed
// Member function:	
// Purpose:			destructor
//
// Author:			Michael Meyer
// Last modified:	Wed Feb  7 15:36:55 MET 2001
// By:				Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

Bankintermed::~Bankintermed() 
{
if(zvar) delete zvar;
}
///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		Bankintermed
// Member function:	loadParamset
// Purpose:			load a parameterset from a specified input file
//
// Author:			Michael Meyer
// Last modified:	Wed Feb  7 12:10:01 MET 2001
// By:				Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

void Bankintermed::loadParamset(ifstream& inFile)
{
	inFile >> e_0;
	inFile >> e_alpha;
	inFile >> e_beta;
	inFile >> I;
 	inFile >> s;	
	inFile >> nu;
	inFile >> alpha;
 	inFile >> beta;	
	inFile >> gamma;
	inFile >> delta;
	inFile >> q_min;
	inFile >> q_max;
 	inFile >> a;	
	inFile >> b;
	inFile >> c;
	inFile >> df_0;

	inFile >> length;

 	initialize();  
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		Bankintermed
// Member function:	initialize
// Purpose:			initialize the model, define the systems initial state
//
// Author:			Michael Meyer	
// Last modified:	Wed Feb  7 15:37:36 MET 2001
// By:				Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

void Bankintermed::initialize()
{
	e=e_0;
	df=df_0;

	temp2=(2*q_min)/(q_max-q_min);
	S=(1-nu)*e*s;

	df_min=-(S+nu*e);
	df_max=nu*I-S;

	y=b/a*pow((e+I),a);
	df_reg=-alpha*I*df_min/(e+I-alpha*I);
	
	if ( zvar != NULL ) delete zvar;
	zvar = new rand_var("ranf",1,q_min,q_max);
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		Bankintermed
// Member function:	I_G()
// Purpose:			Kritische Unternehmer
//
// Author:			Michael Meyer
// Last modified:	Wed Feb  7 15:37:55 MET 2001
// By:				Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

void Bankintermed::I_G_Fktn()
{
	I_G = (df_max - df)/(e + I);

}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		Bankintermed
// Member function:	R()
// Purpose:			Gleichgewichtszinsfaktor
//
// Author:			Michael Meyer	
// Last modified:	Wed Feb  7 15:38:13 MET 2001
// By:				Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

void Bankintermed::R_Fktn()
{
	real buffer, buffer1;
	
	if(I/e <= temp2){
		R=(1+I_G)*y/(e+I)*(q_max+q_min)/2;
//		R=R-1;
	}
	else{
		buffer=(1+((q_max-q_min)/q_max)*e/I)*(1+((q_max-q_min)/q_max)*e/I)-1;
		buffer1=sqrt(buffer);

		R=(1+I_G)*y*q_max/I*(1+(q_max-q_min)/q_max*e/I - buffer1);
//		R=R-1;
	}
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		Bankintermed
// Member function:	R_min()
// Purpose:			minimaler Einlagenzinsfaktor IG()=nu
//
// Author:			Michael Meyer	
// Last modified:	Wed Feb  7 15:38:13 MET 2001
// By:				Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

void Bankintermed::R_min_Fktn()
{
	real buffer, buffer1;
	
	if(I/e <= temp2){
		R_min=(1+nu)*y/(e+I)*(q_max+q_min)/2;
	}
	else{
		buffer=(1+((q_max-q_min)/q_max)*e/I)*(1+((q_max-q_min)/q_max)*e/I)-1;
		buffer1=sqrt(buffer);

		R_min=(1+nu)*y*q_max/I*(1+(q_max-q_min)/q_max*e/I - buffer1);
	}
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		Bankintermed
// Member function:	R_reg()
// Purpose:			Einlagenzinsfaktor
//
// Author:			Michael Meyer
// Last modified:	Mon May  7 15:15:54 METDST 2001
// By:				Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

void Bankintermed::R_reg_Fktn(){

	real buffer,buffer1,buffer2;

	if(gamma<=0){
	  R_reg=1;
	}
	else{
		if(df<df_reg){
		  buffer=1/delta;
		  buffer1=pow(R_min,buffer);

		  buffer2=(1-1/buffer1)*(df-df_min)/(df_reg-df_min)+ 1/buffer1;
		  buffer1=pow(buffer2,delta);

		  R_reg=R*buffer1*gamma;
		}
		else{
			R_reg=R;
		}
	}
}		

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		Bankintermed
// Member function:	R_c()
// Purpose:			Kreditzinsfaktor
//
// Author:			Michael Meyer
// Last modified:	Wed Feb  7 15:38:44 MET 2001
// By:				Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

void Bankintermed::R_c_Fktn(){

	real buffer, buffer1;

	if(I/e <= temp2)
		R_c = (1+I_G)*y/I*(q_max+q_min)/2 - e/I*R_reg;
	else{
		buffer= 2*(q_max-q_min)*e/(y*q_max*q_max)*R_reg/(1+I_G);
		buffer1=sqrt(buffer);

		R_c = (1+I_G)*y*q_max/I*(1-buffer1);
	}		
}
///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		Bankintermed
// Member function:	I_B()
// Purpose:			Firmenbankrott	
//
// Author:			Michael Meyer
// Last modified:	Wed Feb  7 15:39:00 MET 2001
// By:				Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

void Bankintermed::I_B_Fktn(){

	real buffer1,buffer2;

	buffer1=I/y*R_c/q -1;
	buffer2=MAX(I_G,buffer1);

	I_B = MIN(nu,buffer2);
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		Bankintermed
// Member function:	P()
// Purpose:			Aggregierter Output der Firmen	
//
// Author:			Michael Meyer
// Last modified:	Mon May  7 15:16:14 METDST 2001
// By:				Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

void Bankintermed::P_Fktn(){

	if(I_B <= I_G){
		P= (nu-I_G)*I*R_c;
	}
	else{
		P=q*y*( (nu-I_G)*(1+ 1/2*(nu+I_G))-1/2*(nu-I_B)*(nu-I_B));
	}	
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		Bankintermed
// Member function:	iteration
// Purpose:			perform one iteration of the system
//
// Author:			Michael Meyer
// Last modified:	Tue May  8 11:23:45 METDST 2001
// By:				Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

void Bankintermed::iteration(const long& ){

	real buffer;

	q = zvar->dice();						// get random number in [q_min,q_max]

	e_old=e;
	e=e_beta*e_old+e_alpha*(q-(q_max+q_min)/2)+(1-e_beta)*e_0;
		
 	S=(1-nu)*e*s;

	df_min=-(S+nu*e);
	df_max=nu*I-S;

	y=(b/a)*pow((e+I),a)+c;
	df_reg=-alpha*I*df_min/(e+I-alpha*I);
	
	I_G_Fktn();
	R_min_Fktn();
	R_Fktn();
	R_reg_Fktn();
	R_c_Fktn();
	I_B_Fktn();
	P_Fktn();

	G = P-(((nu-I_G)*I-df)*R_reg);			// Systemdynamik
	K = (nu-I_G)*I;							// Kreditvolumen
	
	if(I_G < nu) {
		B = (I_B-I_G)/(nu-I_G);				// Bankrotte
	}
        else B=1;

	S_vol = S+e*I_G;						// Sparvolumen
	RF = P/K;								// Rueckzahlungsfaehigkeit
	Y = q*y*((nu-I_G)+0.5*(nu*nu-I_G*I_G));	// Einkommen

	buffer=MIN(df_max,G);					// 
	df=MAX(df_min,buffer);					// neues Defizit
}
///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		Bankintermed
// Member function:	setLabels
// Purpose:			return a pointer to a variable or a parameter specified
//
// Author:			Michael Meyer	
// Last modified:	Wed Feb  7 15:39:57 MET 2001
// By:				Michael Meyer	
//
///////////////////////////////////////////////////////////////////////////////

real* Bankintermed::setLabels(char* label){
    
	if( !strcmp(label,"R") ) return(&R);
    if( !strcmp(label,"R_c") ) return(&R_c);
    if( !strcmp(label,"R_reg") ) return(&R_reg);
    if( !strcmp(label,"P") ) return(&P);
    if( !strcmp(label,"I_B") ) return(&I_B);
    if( !strcmp(label,"I_G") ) return(&I_G);
    if( !strcmp(label,"Y") ) return(&Y);
    if( !strcmp(label,"y") ) return(&y);
    if( !strcmp(label,"K") ) return(&K);
    if( !strcmp(label,"RF") ) return(&RF);
    if( !strcmp(label,"S_vol") ) return(&S_vol);
    if( !strcmp(label,"df") ) return(&df);
    if( !strcmp(label,"e") ) return(&e);
    if( !strcmp(label,"df_min") ) return(&df_min);
    if( !strcmp(label,"df_max") ) return(&df_max);
    if( !strcmp(label,"df_reg") ) return(&df_reg);
    if( !strcmp(label,"G") ) return(&G);
    if( !strcmp(label,"B") ) return(&B);
    if( !strcmp(label,"xBundle") ) return(&xBundle);

    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		Bankintermed
// Member function:	saveParamset
// Purpose:			write parameterset into a file
//
// Author:			Michael Meyer
// Last modified:	Wed Feb  7 15:45:49 MET 2001
// By:				Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

void Bankintermed::saveParamset(ofstream& outFile){

	outFile << e_0;
	outFile << e_alpha;
	outFile << e_beta;
	outFile << I;
 	outFile << s;	
	outFile << nu;
	outFile << alpha;
 	outFile << beta;	
	outFile << gamma;
	outFile << delta;
	outFile << q_min;
	outFile << q_max;
 	outFile << a;	
	outFile << b;
	outFile << c;
	outFile << df_0;

    outFile << length;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		Bankintermed
// Member function:	printParamset
// Purpose:			print parameterset on the screen
//
// Author:			Michael Meyer
// Last modified:	Wed Feb  7 15:46:17 MET 2001
// By:				Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

void Bankintermed::printParamset(){

    cout <<	e_0 << endl;
    cout <<	e_alpha << endl;
    cout <<	e_beta << endl;
    cout <<	I << endl;
    cout << s << endl;	
    cout <<	nu << endl;
    cout <<	alpha << endl;
    cout <<	beta << endl;	
    cout <<	gamma << endl;
    cout <<	delta << endl;
    cout <<	q_min << endl;
    cout <<	q_max << endl;
    cout <<	a << endl;
    cout <<	b << endl;
    cout <<	c << endl;
    cout <<	df_0 << endl;

    cout << length << endl;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		Bankintermed
// Member function:	sendStateSpace
// Purpose:			return pointers to the state variables and inform about
//					the systems dimension
//
// Author:			Michael Meyer
// Last modified:	Wed Feb  7 15:48:28 MET 2001
// By:				Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

void Bankintermed::sendStateSpace(int &quantity,const real*** stateSpace){

    if( stateSpace )
	delete stateSpace;
    *stateSpace= new const real* [dimension];
    if( !(*stateSpace) )
	fatalError("Bankintermed::sendStateSpace",
		   "Can't create state space vector");
    quantity=dimension;
    (*stateSpace)[0]=&G;   

};
///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:			Bankintermed
// Member functions:	sendModelVar, sendParameters, receiveParameters
// Purpose:				for compatibity reasons only
//
// Author:				Michael Meyer
// Last modified:		Wed Feb  7 15:48:59 MET 2001
// By:					Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////
real* Bankintermed::sendModelVar(void){
 error("macrodyn::Bankintermed::sendModelVar is not implemented");
  return NULL;
} 
void Bankintermed::sendParameters(int& ,real** ){
 error("macrodyn::Bankintermed::sendParameters is not implemented");
}
void Bankintermed::receiveParameters(const real* ){
 error("macrodyn::Bankintermed::receiveParameters is not implemented");
} 











