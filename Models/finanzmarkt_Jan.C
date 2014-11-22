///////////////////////////////////////////////////////////////////////////////
//
// $Header: finanzmarkt_Jan.C, Thu Jul  6 17:09:23 METDST 2000 mmeyer
//
// Module name:		finanzmarkt_Jan.C
// Contents:		Member functions of the class finanzmarkt_Jan
//
//
// Author:		    mhoffman
// Last modified:	Feb 27 METDST 2001
// By:				mhoffman			
//
///////////////////////////////////////////////////////////////////////////////

#include "../error.h"
#include "finanzmarkt_Jan.h"

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:	finanzmarkt_Jan
// Member function:	constructor
// Purpose:		constructor
//
// Author:		    mhoffman
// Last modified:	Feb 27 METDST 2001
// By:				mhoffman			
//
///////////////////////////////////////////////////////////////////////////////

finanzmarkt_Jan::finanzmarkt_Jan() : baseModel(1)
{
	if ( rand_dis != NULL ) delete rand_dis;
	rand_dis = new distribution();
	if( !(rand_dis) )
		fatalError("finanzmarkt_Jan:","can't create rand_dis");	

	d = new matrix_neu(2,1);
	if( !(d) )
		fatalError("finanzmarkt_Jan:","can't create matrix");	
	p = new matrix_neu(2,1);
	if( !(p) )
		fatalError("finanzmarkt_Jan:","can't create matrix");	
	p_old = new matrix_neu(2,1);
	if( !(p_old) )
		fatalError("finanzmarkt_Jan:","can't create matrix");	
	q0 = new matrix_neu(2,1);
	if( !(q0) )
		fatalError("finanzmarkt_Jan:","can't create matrix");	
	q = new matrix_neu(2,1);
	if( !(q) )
		fatalError("finanzmarkt_Jan:","can't create matrix");	
	q_C = new matrix_neu(2,1);
	if( !(q_C) )
		fatalError("finanzmarkt_Jan:","can't create matrix");	
	q_F = new matrix_neu(2,1);
	if( !(q_F) )
		fatalError("finanzmarkt_Jan:","can't create matrix");	
	x_all = new matrix_neu(2,1);
	if( !(x_all) )
		fatalError("finanzmarkt_Jan:","can't create matrix");	
	x_N = new matrix_neu(2,1);
	if( !(x_N) )
		fatalError("finanzmarkt_Jan:","can't create matrix");	
	x_F = new matrix_neu(2,1);
	if( !(x_F) )
		fatalError("finanzmarkt_Jan:","can't create matrix");	
	x_Nold = new matrix_neu(2,1);
	if( !(x_Nold) )
		fatalError("finanzmarkt_Jan:","can't create matrix");	
	x_Fold = new matrix_neu(2,1);
	if( !(x_Fold) )
		fatalError("finanzmarkt_Jan:","can't create matrix");	
	xi = new matrix_neu(2,1);
	if( !(xi) )
		fatalError("finanzmarkt_Jan:","can't create matrix");	
	E_xi = new matrix_neu(2,1);
	if( !(E_xi) )
		fatalError("finanzmarkt_Jan:","can't create matrix");	
	A = new matrix_neu(2,2);
	if( !(A) )
		fatalError("finanzmarkt_Jan:","can't create matrix");	
	A_C = new matrix_neu(2,2);
	if( !(A_C) )
		fatalError("finanzmarkt_Jan:","can't create matrix");	
	A_F = new matrix_neu(2,2);
	if( !(A_F) )
		fatalError("finanzmarkt_Jan:","can't create matrix");	
	V_C = new matrix_neu(2,2);
	if( !(V_C) )
		fatalError("finanzmarkt_Jan:","can't create matrix");	
	V_F = new matrix_neu(2,2);
	if( !(V_F) )
		fatalError("finanzmarkt_Jan:","can't create matrix");	

	for(int i=0;i<L;i++){
		Rho[i] = new matrix_neu(2,2);
		if( !(Rho[i]) )
			fatalError("finanzmarkt_Jan:","can't create matrix");	
		q_MA[i] = new matrix_neu(2,1);
		if( !(q_MA[i]) )
			fatalError("finanzmarkt_Jan:","can't create matrix");	
	}

	V_C_inv = new matrix_neu(2,2);
	if( !(V_C_inv) )
		fatalError("finanzmarkt_Jan:","can't create matrix");
	V_F_inv = new matrix_neu(2,2);
	if( !(V_F_inv) )
		fatalError("finanzmarkt_Jan:","can't create matrix");	
	temp1_2_2 = new matrix_neu(2,2);
	if( !(temp1_2_2) )
		fatalError("finanzmarkt_Jan:","can't create matrix");	
	temp2_2_2 = new matrix_neu(2,2);
	if( !(temp2_2_2) )
		fatalError("finanzmarkt_Jan:","can't create matrix");	
	temp3_2_2 = new matrix_neu(2,2);
	if( !(temp3_2_2) )
		fatalError("finanzmarkt_Jan:","can't create matrix");	
	temp1_2_1 = new matrix_neu(2,1);
	if( !(temp1_2_1) )
		fatalError("finanzmarkt_Jan:","can't create matrix");	
	temp2_2_1 = new matrix_neu(2,1);
	if( !(temp2_2_1) )
		fatalError("finanzmarkt_Jan:","can't create matrix");	
	temp3_2_1 = new matrix_neu(2,1);
	if( !(temp3_2_1) )
		fatalError("finanzmarkt_Jan:","can't create matrix");	
	temp1_1_1 = new matrix_neu(1,1);
	if( !(temp1_1_1) )
		fatalError("finanzmarkt_Jan:","can't create matrix");	
	V_xi = new matrix_neu(2,2);
	if( !(V_xi) )
		fatalError("finanzmarkt_Jan:","can't create matrix");	
	V_xi_inv = new matrix_neu(2,2);
	if( !(V_xi_inv) )
		fatalError("finanzmarkt_Jan:","can't create matrix");	

}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:	finanzmarkt_Jan	
// Member function:	 ~finanzmarkt_Jan
// Purpose:		destructor
//
// Author:		    mhoffman
// Last modified:	Feb 27 METDST 2001
// By:				mhoffman			
//
///////////////////////////////////////////////////////////////////////////////

finanzmarkt_Jan::~finanzmarkt_Jan()
{
	if(rand_dis) 		delete rand_dis;
	if(d) 				delete d;
	if(p) 				delete p;
	if(p_old)			delete p_old;
	if(q0) 		 		delete q0;
	if(q) 		 		delete q;
	if(q_C) 			delete q_C;
	if(q_F) 			delete q_F;
	if(xi) 		delete xi;
	if(E_xi) 		delete E_xi;
	if(x_all) 			delete x_all;
	if(x_N) 			delete x_N;
	if(x_F) 			delete x_F;
	if(x_Nold) 			delete x_Nold;
	if(x_Fold) 			delete x_Fold;
	if(A) 				delete A;
	if(A_F) 			delete A_F;
	if(A_C) 			delete A_C;
	if(V_F) 			delete V_F;
	if(V_C) 			delete V_C;
	if(Rho) 			delete Rho;
	for(int i=0;i<L;i++){
		if(Rho[i]) delete Rho[i];
		if(q_MA[i]) delete q_MA[i];
	}

	if(V_C_inv) delete(V_C_inv);
	if(V_F_inv) delete(V_F_inv);
	if(temp1_2_2) delete(temp1_2_2);
	if(temp2_2_2) delete(temp2_2_2);
	if(temp3_2_2) delete(temp3_2_2);
	if(temp1_2_1) delete(temp1_2_1);
	if(temp2_2_1) delete(temp2_2_1);
	if(temp3_2_1) delete(temp3_2_1);
	if(temp1_1_1) delete(temp1_1_1);

	if(V_xi_inv) delete(V_xi_inv);
	if(V_xi) delete(V_xi);

}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:	finanzmarkt_Jan
// Member function:	loadParamset
// Purpose:		load a parameterset from a specified input file
//
// Author:		    mhoffman
// Last modified:	Feb 27 METDST 2001
// By:				mhoffman			
//
///////////////////////////////////////////////////////////////////////////////

void finanzmarkt_Jan::loadParamset(ifstream& inFile)
{ 	


	inFile >> eta_F;		//Gruppenanteil (Fundamentalisten)			
    inFile >>  alpha_F >> alpha_C;	//Risikoparameter
    inFile >>  e_F >> e_N;	//Kumulierte Grundausstattung der Händlergruppen
	inFile >> r;			//Zinssatz
	inFile >> x_all1 >> x_all2;		//alle Papiere
	
	for(int i=0;i<L;i++){			//Diagonalmatrix für Moving Average
		inFile >> (*Rho[i])(0,0);	//eingelesener Wert auf die gesamte Diagonale
		for(int j=1;j<2;j++)		//setzen
			(*Rho[i])(j,j) = (*Rho[i])(0,0);	
	}
    for(int i=0;i<2;i++)
		inFile >> (*q0)(i,0);		
    inFile >> a1 >> b1 >> c1;	//Parameter fuer die Dreieicksverteilung,
    inFile >> a2 >> b2 >> c2;	//des noise xi(in der Initialisierung wird
								//der untere Rand auf -b1 bzw.-b2 gestetzt)
	if((b1<a1)||(b2<a2))
		fatalError("finanzmarkt_Jan::loadParameterset","invalid parameter");	
   	inFile >> zetamin1 >> zetamax1 >> gamma1; //Parameterwerte fuer die 
    inFile >> zetamin2 >> zetamax2 >> gamma2;  //Dividenenprozesse
  			
	inFile >> length;

 	initialize();

}


///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:	finanzmarkt_Jan	
// Member function:	initialize
// Purpose:		initialize the model, define the systems initial state
//
// Author:		    mhoffman
// Last modified:	Feb 27 METDST 2001
// By:				mhoffman			
//
///////////////////////////////////////////////////////////////////////////////

void finanzmarkt_Jan::initialize()
{	
	mu_N = 0;
	mu_F = 0;
	sigma_N = 0;
	sigma_F = 0;
	sr_N = 0;
	sr_F = 0;

	
    qreal koeff;
	R = 1+r;
	eta_C = 1-eta_F;
	e_N = e_N;
	e_F = e_F;
	(*x_all)(0,0) = x_all1;
	(*x_all)(1,0) = x_all2;
	(*x_N)(0,0) = 0;
	(*x_N)(1,0) = 0;
	(*x_F)(0,0) = 0;
	(*x_F)(1,0) = 0;

//Startwert der Dividenden = Mittelwert
	d1 = ((zetamax1-zetamin1)/2)/(1-gamma1);	
	d2 = ((zetamax2-zetamin2)/2)/(1-gamma2);
	(*d)(0,0) = d1;
	(*d)(1,0) = d2;

//Erechnen des Wertes der Peaks, (c1,c2), der Verteilungsfunktion für Noise
	z1=b1-a1;				
	if (z1==0){				
		F_c1=0;				
		}
	else {
		F_c1=(c1-a1)/(b1-a1);		
	}			
	z2=b2-a2;
	if (z2==0){
		F_c2=0;	
		}
	else {
		F_c2=(c2-a2)/(b2-a2);	
	}
	(*E_xi)(0,0) = (b1+a1+c1)/3;
	(*E_xi)(1,0) = (b2+a2+c2)/3;

	(*V_xi)(0,0) = (a1*a1 + b1*b1 + c1*c1 - a1*b1 - a1*c1 - b1*c1)/18;
	(*V_xi)(0,1) = 0;
	(*V_xi)(1,0) = 0;
	(*V_xi)(1,1) = (a2*a2 + b2*b2 + c2*c2 - a2*b2 - a2*c2 - b2*c2)/18;

	(*V_xi_inv) = V_xi->inverse();
//	koeff = (1/alpha_F + 1/alpha_C)*R;
	koeff = (1/alpha_F)*R;
	(*V_F) = V_xi_inv->multiplyScalar(koeff);
	(*V_C) = (*V_F);	//Spezialfall

//Initialisierung für Moving Average
	for(int i=0;i<L;i++)
		(*q_MA[i]) = (*q0);	
	
//Startwerte für Vorhersagen und cumdividend-Preise
	(*q_F) = (*q0);
	(*q_C) = (*q0);
	(*q) = (*q0);
	(*p) = (*q0)-(*d);
}


///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		finanzmarkt_Jan
// Member function:	iteration
// Purpose:		perform one iteration of the system
//
// Author:		    mhoffman
// Last modified:	Feb 27 METDST 2001
// By:				mhoffman			
//
///////////////////////////////////////////////////////////////////////////////

void finanzmarkt_Jan::iteration(const long& t)
{ 	
//Hä?
//  Matrix M(1,5,1,5);
//    for (int ii = 1; ii <= 5; ii++)
//        for (int jj = 1; jj <= 5; jj++) M[ii][jj] = 3.14;
//  Matrix N(1,5,1,5);
//    for (ii = 1; ii <= 5; ii++)
//        for (int jj = 1; jj <= 5; jj++) N[ii][jj] = 3.14;



//Log::log() << M*N;

    qreal a_F,a_C;					//gruppenspezifische Risikotoleranz
    qreal temp1,temp2;



//Log::log().precision(16);
//Abspeichern von Werten aus der Vorperiode 
//(für die Berechnung der Portefeuillerenditen)
	(*p_old) = (*p);
	(*x_Nold) = (*x_N);
	(*x_Fold) = (*x_F);

//Zufallsdividende fuer die laufende Periode 
//ACHTUNG: Position der Berechnung ist abhängig von Datierung 
//der bedingten Erwartungswerte
	d1=gamma1 * d1 + (rand_dis->rectangular() * (zetamax1-zetamin1) + zetamin1);	
	d2=gamma2 * d2 + (rand_dis->rectangular() * (zetamax2-zetamin2) + zetamin2);	
	(*d)(0,0) = d1;
	(*d)(1,0) = d2;

//Berechnung der neuen Gruppenanteile, (eta_F,eta_C), mithilfe des
//Discrete Choice Model
	//Spezialfall: eta=konst
	
//Berechnung der gruppenspezifische Risikotoleranzen
	a_F = eta_F / alpha_F;
	a_C = eta_C / alpha_C;

//Berechnung der subjektiven Kovarianz-Matrizen
	//Spezialfall: V_C = V_F = const

//Berchnung der risikoadjustierten Kovarianz-Matrizen
	(*V_C_inv) = V_C->inverse();
	(*V_F_inv) = V_F->inverse();

  //A=(a_F*(V_F^-1) + a_C*(V_C^-1))^-1
	(*temp1_2_2) = V_C_inv->multiplyScalar(a_C); 
	(*temp2_2_2) = V_F_inv->multiplyScalar(a_F); 
	(*temp3_2_2) = (*temp1_2_2) + (*temp2_2_2);
	(*A) = temp3_2_2->inverse();

  //A_F=a_F*(V_F*A^-1)^-1
	(*temp1_2_2) = (*V_F) * (*temp3_2_2);  //temp3_2_2 = A^-1
	(*temp2_2_2) = temp1_2_2->inverse();
	(*A_F) = temp2_2_2->multiplyScalar(a_F);

  //A_C=a_C*(V_C*A^-1)^-1
	(*temp1_2_2) = (*V_C) * (*temp3_2_2);
	(*temp2_2_2) = temp1_2_2->inverse();
	(*A_C) = temp2_2_2->multiplyScalar(a_C);

// Generierung der Dreiecksverteilung für den Noise-Prozess
	if(F_c1==0){
		xi1=0;		
		}
	else  {
		xi1=rand_dis->rectangular();
		if(xi1<=F_c1){
		 	xi1=xi1*(b1-a1)*(c1-a1);
			xi1=a1+sqrt(xi1);
			}
		else{
			xi1=(1-xi1)*(b1-a1)*(b1-c1);
			xi1=b1-sqrt(xi1);	
			}	
	}

	if(F_c2==0){
		xi2=0;		
		}	
	else  {
		xi2=rand_dis->rectangular();
		if(xi2<=F_c2){
			xi2=xi2*(b2-a2)*(c2-a2);
			xi2=a2+sqrt(xi2);
			}
		else{
			xi2=(1-xi2)*(b2-a2)*(b2-c2);
			xi2=b2-sqrt(xi2);	
		}	
	}
	(*xi)(0,0) = xi1;
	(*xi)(1,0) = xi2;

//cumdividend-Vorhersage der Chartisten, q_C (durch Moving Average)
	temp1_2_1->set_to_zero();	
	for(int i=0;i<L;i++)
		(*temp1_2_1) = (*temp1_2_1) + (*Rho[i]) * (*q_MA[i]);	
	(*q_C) = (*temp1_2_1);

//cumdividend-Vorhersage der Fundamentalisten, q_F (perfect mean-predictor)
// q_F = A_F^-1*[R*(q_F-d) - A_C*q_C + A*(x_all+E_xi)]

	(*temp1_2_1) = (*A)*((*x_all) + (*E_xi)) - (*A_C)*(*q_C);
    (*temp2_2_1) = (*q_F) - (*d);
	(*temp3_2_1) = temp2_2_1->multiplyScalar(R);
	(*temp2_2_1) = (*temp3_2_1) + (*temp1_2_1);
	(*temp1_2_2) = A_F->inverse();
	(*q_F) = (*temp1_2_2) * (*temp2_2_1);

//Preisprozess:
// p=1/R * [A_C*q_C + A_F*q_F - A*(x_all+xi)] 
	(*temp1_2_1) = (*A) * ((*x_all)+(*xi));
	(*temp2_2_1) = ((*A_F) * (*q_F)) + ((*A_C)*(*q_C));
	(*temp3_2_1) = (*temp2_2_1) - (*temp1_2_1);
	(*p) = temp3_2_1->multiplyScalar(1/R);

//cumdividend-Preis: q=p+d
	(*q) = (*p) + (*d);	

//Berechnung der Aktienportefeuilles
//Aktienportefeuille der Fundamentalisten: x_F=a_F*(V_F)^-1(q_F-R*p)
	(*temp1_2_1) = p->multiplyScalar(R);
	(*temp2_2_1) = (*q_F) - (*temp1_2_1);
	(*temp3_2_1) = (*V_F_inv) * (*temp2_2_1);
	(*x_F) = temp3_2_1->multiplyScalar(a_F);

//Aktienportefeuille der imaginären Noise-Traders
//x_N=xi
 	(*x_N) = xi->multiplyScalar(-1);

//Renditenberechung
//Renditen der Fundamentalisten	aus der Vorperiode
//wealth_F=(q-R*p_old)*x_Fold
	(*temp1_2_1) = p_old->multiplyScalar(R);
	(*temp2_2_1) = (*q) - (*temp1_2_1);
	(*temp1_1_1) = temp2_2_1->multiplyATB(*x_Fold);
	wealth_F = (*temp1_1_1)(0,0);
	rend_F=(wealth_F/e_F)+(R-1);

//Noise-Traders
//Renditen der Noise-Traders aus der Vorperiode
//wealth_N=(q-R*p_old)*x_Nold
	(*temp1_2_1) = p_old->multiplyScalar(R);
	(*temp2_2_1) = (*q) - (*temp1_2_1);
	(*temp1_1_1) = temp2_2_1->multiplyATB(*x_Nold);
	wealth_N = (*temp1_1_1)(0,0);
	rend_N=(wealth_N/e_N)+(R-1);

//mean and standard deviation of return der Fundamentalisten
    mu_F = (1/qreal(t+1)) * (rend_F + (t*mu_F));
    temp1 = ((rend_F - mu_F)*(rend_F - mu_F))/qreal(t+1);
    temp2 = (t/qreal(t+1)) * sigma_F*sigma_F;
	sigma_F = sqrt(temp1 + temp2);

//mean and standard deviation of return der Noise-Trader
    mu_N = (1/qreal(t+1)) * (rend_N + t*mu_N);
    temp1 = ((rend_N - mu_N)*(rend_N - mu_N))/qreal(t+1);
    temp2 = (t/qreal(t+1)) * sigma_N*sigma_N;
	sigma_N = sqrt(temp1 + temp2);

//sharpe ratio Noise-traders und Fundamentalisten
	sr_N = (mu_N - r)/sigma_N;
	sr_F = (mu_F - r)/sigma_F;

//shifting for the next period
    for (int i=1;i<L;i++)
		(*q_MA[L-i]) = (*q_MA[L-1-i]);
	(*q_MA[0]) = (*q);
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		finanzmarkt_Jan
// Member function:	setLabels
// Purpose:		return a pointer to a variable or a parameter specified
//
// Author:		    mhoffman
// Last modified:	Feb 27 METDST 2001
// By:				mhoffman			
//
///////////////////////////////////////////////////////////////////////////////

qreal* finanzmarkt_Jan::setLabels(char* label)
{
	if( !strcmp(label,"eta_F") ) return(&eta_F);
	if( !strcmp(label,"eta_C") ) return(&eta_C);
	if( !strcmp(label,"r") ) return(&r);
	if( !strcmp(label,"xi1") ) return(&xi1);
	if( !strcmp(label,"xi2") ) return(&xi2);
	if( !strcmp(label,"d1") ) return(&d1);
	if( !strcmp(label,"d2") ) return(&d2);
	if( !strcmp(label,"p1") ) return(&(*p)(0,0));
	if( !strcmp(label,"p2") ) return(&(*p)(1,0));
	if( !strcmp(label,"q1") ) return(&(*q)(0,0));
	if( !strcmp(label,"q2") ) return(&(*q)(1,0));
	if( !strcmp(label,"x_N1") ) return(&(*x_N)(0,0));
	if( !strcmp(label,"x_N2") ) return(&(*x_N)(1,0));
	if( !strcmp(label,"x_F1") ) return(&(*x_F)(0,0));
	if( !strcmp(label,"x_F2") ) return(&(*x_F)(1,0));
	if( !strcmp(label,"rend_N") ) return(&rend_N);
	if( !strcmp(label,"rend_F") ) return(&rend_F);
	if( !strcmp(label,"sr_N") ) return(&sr_N);
	if( !strcmp(label,"sr_F") ) return(&sr_F);
	if( !strcmp(label,"mu_N") ) return(&mu_N);
	if( !strcmp(label,"mu_F") ) return(&mu_F);
	if( !strcmp(label,"sigma_N") ) return(&sigma_N);
	if( !strcmp(label,"sigma_F") ) return(&sigma_F);



    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:	finanzmarkt_Jan	
// Member function:	saveParamset
// Purpose:		write parameterset into a file
//
// Author:		    mhoffman
// Last modified:	Feb 27 METDST 2001
// By:				mhoffman			
//
///////////////////////////////////////////////////////////////////////////////

void finanzmarkt_Jan::saveParamset(ofstream& outFile)
{	

	outFile << eta_F;
    outFile <<  alpha_F << alpha_C;
	outFile << r;
	outFile << x_all1 << x_all2;
	for(int i=0;i<L;i++)
		outFile << (*Rho[i])(0,0);	
    for(int i=0;i<2;i++)
		outFile << (*q0)(i,0);

	outFile << a1 << b1 << c1;
	outFile << a2 << b2 << c2;
   	outFile << zetamin1 << zetamax1 << gamma1;
   	outFile << zetamin2 << zetamax2 << gamma2;
		
    outFile << length;

}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:	finanzmarkt_Jan	
// Member function:	printParamset
// Purpose:		print parameterset on the screen
//
// Author:		    mhoffman
// Last modified:	Feb 27 METDST 2001
// By:				mhoffman			
//
///////////////////////////////////////////////////////////////////////////////

void finanzmarkt_Jan::printParamset()
{ 	

    Log::log() << eta_F << endl;
    Log::log() <<  alpha_F << alpha_C << endl;
    Log::log() << r << endl;
    Log::log() << x_all1 << x_all2 << endl;
	for(int i=0;i<L;i++)
        Log::log() << (*Rho[i])(0,0) << endl;
    for(int i=0;i<2;i++)
        Log::log() << (*q0)(i,0) << endl;
    Log::log() << a1 << b1 << c1 << endl;
    Log::log() << a2 << b2 << c2 << endl;
    Log::log() << zetamin1 << zetamax1 << gamma1 << endl;
    Log::log() << zetamin2 << zetamax2 << gamma2 << endl;

    Log::log() << length << endl;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:	finanzmarkt_Jan	
// Member function:	sendStateSpace
// Purpose:		return pointers to the state variables and inform about
//			the systems dimension
//
// Author:		    mhoffman
// Last modified:	Feb 27 METDST 2001
// By:				mhoffman			
//
///////////////////////////////////////////////////////////////////////////////

void finanzmarkt_Jan::sendStateSpace(int &quantity,const qreal*** stateSpace)
{
    if( stateSpace )
	delete stateSpace;
    *stateSpace= new const qreal* [dimension];
    if( !(*stateSpace) )
	fatalError("finanzmarkt_Jan::sendStateSpace",
		   "Can't create state space vector");
    quantity=dimension;
    (*stateSpace)[0]=&r;

};
///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		
// Member functions:	sendModelVar, sendParameters, receiveParameters
// Purpose:		for compatibity reasons only
//
// Author:		    mhoffman
// Last modified:	Feb 27 METDST 2001
// By:				mhoffman			
//
///////////////////////////////////////////////////////////////////////////////
qreal* finanzmarkt_Jan::sendModelVar(void)
{ error("macrodyn::finanzmarkt_Jan::sendModelVar is not implemented");
  return NULL;
}
void finanzmarkt_Jan::sendParameters(int& ,qreal** )
{ error("macrodyn::finanzmarkt_Jan::sendParameters is not implemented");
}
void finanzmarkt_Jan::receiveParameters(const qreal* )
{ error("macrodyn::finanzmarkt_Jan::receiveParameters is not implemented");
} 
