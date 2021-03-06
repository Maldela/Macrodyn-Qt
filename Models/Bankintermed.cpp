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

#include "../logger.h"
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

void Bankintermed::loadParamset(QTextStream& inFile)
{
	inFile >> e_0;
	inFile >> e_alpha;
	inFile >> e_beta;
	inFile >> I_ini;
    inFile >> I_qMin;
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
	I=I_ini;
    I_qMin=S/nu;

    temp2=(2*q_min)/(q_max-q_min);
	S=(1-nu)*e*s;

    df_min=-(S+nu*e);
    log() << "df_min=" << df_min;
    df_max=nu*I_ini-S;
    log() << "df_max=" << df_max;

    df_crit_min=-alpha/(1+e/I_qMin-alpha)*df_min;
    df_crit_max=-alpha/(1+e/I-alpha)*df_min;

	y=b/a*pow((e+I),a)+c;
    df_reg=-alpha*I*df_min/(e+I-alpha*I);

	
	I_G_Fktn();
    R_min_Fktn();
	R_Fktn();
	R_reg_Fktn();
	R_c_Fktn();
	I_B_Fktn();

        R_c=R_c-1;                                              // Umsetzen in Zinssaetze nur fur Ausgabe
        R  =R-1;                                                // relevant
        R_reg=R_reg-1;
	
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
	I_G = (nu*I-S - df)/(e + I);
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
    qreal buffer, buffer1;
	
	if(I/e <= temp2){
        R=(1+I_G)*y/(e+I)*(q_max+q_min)/2;
	}
	else{
        buffer=(1+((q_max-q_min)/q_max)*e/I)*(1+((q_max-q_min)/q_max)*e/I)-1;
        buffer1=sqrt(buffer);

        R=(1+I_G)*y*q_max/I*(1+(q_max-q_min)/q_max*e/I - buffer1);
    }
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		Bankintermed
// Member function:	R_min()
// Purpose:			qMinimaler Einlagenzinsfaktor IG()=nu
//
// Author:			Michael Meyer	
// Last modified:	Wed Feb  7 15:38:13 MET 2001
// By:				Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

void Bankintermed::R_min_Fktn()
{
    qreal buffer, buffer1;
	
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

    qreal buffer,buffer1,buffer2;

	if(df<df_reg){
	    
	    if(gamma<=0){
	                R_reg=1;
	    }
	    else{

	        //buffer=1/delta;
        //buffer1=pow(R_min,buffer);

            //buffer2=(1-1/buffer1)*(df-df_min)/(df_reg-df_min)+ 1/buffer1;
	        //buffer1=pow(buffer2,delta);

          buffer=(df-df_min)/(df_reg-df_min);
	      buffer1=pow(buffer,delta);
          buffer2=(1-1/R_min)*buffer1+1/R_min;

          R_reg = qMax((R*buffer2*gamma),1.0);
            }		
        }
		
        else{
	    R_reg=R;
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

    qreal buffer, buffer1;

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

    qreal buffer1,buffer2;

	buffer1=I/y*R_c/q -1;
    buffer2 = qMax(I_G,buffer1);

    I_B = qMin(nu,buffer2);
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
// Member function:	I()
// Purpose:			Fremdkapital als Funktion von d und alpha	
//
// Author:			Michael Meyer
// Last modified:	Fre Nov 29 15:49:48 CET 2002
// By:				Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

void Bankintermed::I_Fktn(){

if(0<=df && df <=df_crit_min){
    I=I_qMin;
}
	else{
    if(df_crit_min < df && df < df_crit_max){
            I=(df*e) / ( alpha*(S+nu*e) - (1-alpha)*df );
		}
	else I=I_ini;
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
// Last modified:	Wed Feb  20 10:27
// By:				Jan Wenzelburger
//
///////////////////////////////////////////////////////////////////////////////

void Bankintermed::iteration(const qint64&)
{
    qreal buffer;

//	q = zvar->dice();						// get random number in [q_min,q_max]
    q = q_min;
	
	e_old=e;
    e=e_beta*e_old+e_alpha*(q-(q_max+q_min)/2)+(1-e_beta)*e_0;

 	S=(1-nu)*e*s;

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

	S_vol = S+e*I_G;							// Sparvolumen
	RF = P/K;									// Rueckzahlungsfaehigkeit
	Y = e+(q*y*((nu-I_G)+0.5*(nu*nu-I_G*I_G)));	// Einkommen

    buffer = qMin(df_max,G);						//
    df = qMax(df_min,buffer);						// neues Defizit

	I_Fktn();

        R_c=R_c-1;             // Umsetzen in Zinssaetze nur fur Ausgabe
        R  =R-1;               // relevant
        R_reg=R_reg-1;

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

qreal* Bankintermed::setLabels(const QString& label){
    
    if (label == "R") return(&R);
    if (label == "R_min") return(&R_min);
    if (label == "R_c") return(&R_c);
    if (label == "R_reg") return(&R_reg);
    if (label == "P") return(&P);
    if (label == "I_B") return(&I_B);
    if (label == "I_G") return(&I_G);
    if (label == "Y") return(&Y);
    if (label == "y") return(&y);
    if (label == "K") return(&K);
    if (label == "RF") return(&RF);
    if (label == "S_vol") return(&S_vol);
    if (label == "df") return(&df);
    if (label == "e") return(&e);
    if (label == "df_min") return(&df_min);
    if (label == "df_max") return(&df_max);
    if (label == "df_crit_min") return(&df_crit_min);
    if (label == "df_crit_max") return(&df_crit_max);
    if (label == "df_reg") return(&df_reg);
    if (label == "df_0") return(&df_0);
    if (label == "G") return(&G);
    if (label == "B") return(&B);
    if (label == "I") return(&I);
    if (label == "q") return(&q);
    if (label == "a") return(&a);
    if (label == "b") return(&b);
    if (label == "xBundle") return(&xBundle);

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

void Bankintermed::saveParamset(QTextStream& outFile){

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

    log() <<	e_0;
    log() <<	e_alpha;
    log() <<	e_beta;
    log() <<	I;
    log() <<    s;
    log() <<	nu;
    log() <<	alpha;
    log() <<	beta;
    log() <<	gamma;
    log() <<	delta;
    log() <<	q_min;
    log() <<	q_max;
    log() <<	a;
    log() <<	b;
    log() <<	c;
    log() <<	df_0;

    log() << length;
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

void Bankintermed::sendStateSpace(int &quantity,QList<qreal *> *stateSpace)
{
    stateSpace->clear();
    quantity = dimension;
    *stateSpace << &G;
}
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
qreal* Bankintermed::sendModelVar(void){
 error("macrodyn::Bankintermed::sendModelVar is not implemented");
  return NULL;
} 
void Bankintermed::sendParameters(int& ,qreal** ){
 error("macrodyn::Bankintermed::sendParameters is not implemented");
}
void Bankintermed::receiveParameters(const QList<qreal>&) {
 error("macrodyn::Bankintermed::receiveParameters is not implemented");
}
