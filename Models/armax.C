 ///////////////////////////////////////////////////////////////////////////////
//
// $Header: armax.C, Thu Jul  6 17:09:23 METDST 2000 mmeyer
//
// Module name:		armax.C
// Contents:		Member functions of the class armax
//                       
//
// Author:		    mhoffman
// Last modified:	Feb 27 METDST 2001
// By:				mhoffman			
//
///////////////////////////////////////////////////////////////////////////////

#include "../error.h"
#include "armax.h"

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:	armax
// Member function:	constructor
// Purpose:		constructor
//
// Author:		    mhoffman
// Last modified:	Feb 27 METDST 2001
// By:				mhoffman			
//
///////////////////////////////////////////////////////////////////////////////

armax::armax() : baseModel(1)
{
	if ( rand_dis != NULL ) delete rand_dis;
	rand_dis = new distribution();
	if( !(rand_dis) )
        fatalError("armax:","can't create rand_dis");
	B_1_inv = new matrix_neu(N1+0,M+0);
	if( !B_1_inv )
		fatalError("matrix_neu::matrix_neu","Can't allocate memory");
	eta_t = new matrix_neu(N1+0,1);
	if( !eta_t )
		fatalError("matrix_neu::matrix_neu","Can't allocate memory");
	x_tm1 = new matrix_neu(1,M + L*M + (K-1)*N1 + R*N2 + N3*G + 1*H);
	if( !x_tm1 )
		fatalError("matrix_neu::matrix_neu","Can't allocate memory");
	Theta = new matrix_neu(N1+0,M + L*M + (K-1)*N1 + R*N2 + N3*G + 1*H);
	if( !Theta )
		fatalError("matrix_neu::matrix_neu","Can't allocate memory");
	P_tm1 = new matrix_neu(M + L*M + (K-1)*N1 + R*N2 + N3*G + 1*H,M + L*M + (K-1)*N1 + R*N2 + N3*G + 1*H);
	if( !P_tm1 )
		fatalError("matrix_neu::matrix_neu","Can't allocate memory");
	P_t = new matrix_neu(M + L*M + (K-1)*N1 + R*N2 + N3*G + 1*H,M + L*M + (K-1)*N1 + R*N2 + N3*G + 1*H);
	if( !P_t )
		fatalError("matrix_neu::matrix_neu","Can't allocate memory");

	
	y_t = new matrix_neu(M+0,1);
	if( !y_t )
		fatalError("matrix_neu::matrix_neu","Can't allocate memory");
	u_t = new matrix_neu(N1+0,1);
	if( !u_t )
		fatalError("matrix_neu::matrix_neu","Can't allocate memory");
	v_t = new matrix_neu(N2+0,1);
	if( !v_t )
		fatalError("matrix_neu::matrix_neu","Can't allocate memory");
  	w_t = new matrix_neu(N3+0,1);
	if( !w_t )
		fatalError("matrix_neu::matrix_neu","Can't allocate memory");

	for (int l=0;l<L;l++){			
		A[l] = new matrix_neu(M+0,M+0);
		if( !A[l])
			fatalError("matrix_neu::matrix_neu","Can't allocate memory");
		A_e[l] = new matrix_neu(M+0,M+0);
		if( !A_e[l])
			fatalError("matrix_neu::matrix_neu","Can't allocate memory");
		y_old[l] = new matrix_neu(M+0,1);
		if( !y_old[l] )
			fatalError("matrix_neu::matrix_neu","Can't allocate memory");
		y_old_ini[l] = new matrix_neu(M+0,1);
		if( !y_old_ini[l] )
			fatalError("matrix_neu::matrix_neu","Can't allocate memory");
	}
    for (int l=0;l<L+1;l++){
		As_e[l] = new matrix_neu(N1+0,M+0);
		if( !As_e[l])
			fatalError("matrix_neu::matrix_neu","Can't allocate memory");
	}

	for (int k=0;k<K;k++){			
		B[k] = new matrix_neu(M+0,N1+0);
		if( !B[k])
			fatalError("matrix_neu::matrix_neu","Can't allocate memory");
		B_e[k] = new matrix_neu(M+0,N1+0);
		if( !B_e[k])
			fatalError("matrix_neu::matrix_neu","Can't allocate memory");
		u_old[k] = new matrix_neu(N1+0,1);
		if( !u_old[k] )
			fatalError("matrix_neu::matrix_neu","Can't allocate memory");
		u_old_ini[k] = new matrix_neu(N1+0,1);
		if( !u_old_ini[k] )
			fatalError("matrix_neu::matrix_neu","Can't allocate memory");
	}
    for (int k=0;k<K-1;k++){
		Bs_e[k] = new matrix_neu(N1+0,N1+0);
		if( !Bs_e[k])
			fatalError("matrix_neu::matrix_neu","Can't allocate memory");	
	}
	
	for (int r=0;r<R;r++){			
		C[r] = new matrix_neu(M+0,N2+0);
		if( !C[r])
			fatalError("matrix_neu::matrix_neu","Can't allocate memory");
		C_e[r] = new matrix_neu(M+0,N2+0);
		if( !C_e[r])
			fatalError("matrix_neu::matrix_neu","Can't allocate memory");
		Cs_e[r] = new matrix_neu(N1+0,N2+0);
		if( !Cs_e[r])
			fatalError("matrix_neu::matrix_neu","Can't allocate memory");
		v_old[r] = new matrix_neu(N2+0,1);
		if( !v_old[r] )
			fatalError("matrix_neu::matrix_neu","Can't allocate memory");
		v_old_ini[r] = new matrix_neu(N2+0,1);
		if( !v_old_ini[r] )
			fatalError("matrix_neu::matrix_neu","Can't allocate memory");
 	}
 	D = new matrix_neu(M+0,N3+0);
	if( !D)
		fatalError("matrix_neu::matrix_neu","Can't allocate memory");
	D_e = new matrix_neu(M+0,N3+0);
	if( !D_e)
		fatalError("matrix_neu::matrix_neu","Can't allocate memory");	
	Ds_e = new matrix_neu(N1+0,N3+0);
	if( !Ds_e)
		fatalError("matrix_neu::matrix_neu","Can't allocate memory");

	E = new matrix_neu(M+0,1);
	if( !E )
		fatalError("matrix_neu::matrix_neu","Can't allocate memory");
	E_e = new matrix_neu(M+0,1);
	if( !E_e )
		fatalError("matrix_neu::matrix_neu","Can't allocate memory");
	Es_e = new matrix_neu(N1+0,1);
	if( !Es_e )
		fatalError("matrix_neu::matrix_neu","Can't allocate memory");

	dif = new matrix_neu(N1+0,1);
	if( !dif )
		fatalError("matrix_neu::matrix_neu","Can't allocate memory");



}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:	armax
// Member function:	 ~armax
// Purpose:		destructor
//
// Author:		    mhoffman
// Last modified:	Feb 27 METDST 2001
// By:				mhoffman			
//
///////////////////////////////////////////////////////////////////////////////

armax::~armax()
{
	if(rand_dis) 		delete rand_dis;
	if(B_1_inv)			delete B_1_inv;
	if(P_tm1)			delete P_tm1;	
	if(Theta)			delete Theta;	
	if(P_t)				delete P_t;
	if(x_tm1)				delete x_tm1;
	
	if(y_t)			delete y_t;
	if(u_t)			delete u_t;	
	if(v_t)			delete v_t;
	if(w_t)			delete w_t;
	
	for(int l=0;l<L;l++){
		if(A[l])	delete A[l];
		if(A_e[l])	delete A_e[l];
		if(As_e[l])	delete As_e[l];
		if(y_old[l]) 		delete y_old[l];
		if(y_old_ini[l])	delete y_old_ini[l];	
	}
	for(int k=0;k<K;k++){
		if(B[k])	delete B[k];
		if(B_e[k])	delete B_e[k];
		if(Bs_e[k])	delete Bs_e[k];
		if(u_old[k])		delete u_old[k];
		if(u_old_ini[k])	delete u_old_ini[k];
	}
	for(int r=0;r<R;r++){
		if(C[r])	delete C[r];
		if(C_e[r])	delete C_e[r];	
		if(Cs_e[r])	delete Cs_e[r];	
		if(v_old[r])		delete v_old[r];
		if(v_old_ini[r])	delete v_old_ini[r];
	}
	if(D)			delete D;
	if(D_e)			delete D_e;
 	if(Ds_e)		delete Ds_e;
	if(E)			delete E;
	if(E_e)			delete E_e;
 	if(Es_e)		delete Es_e;

 	if(dif)			delete dif;     	
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:	armax
// Member function:	loadParamset
// Purpose:		load a parameterset from a specified input file
//
// Author:		    mhoffman
// Last modified:	Feb 27 METDST 2001
// By:				mhoffman			
//
///////////////////////////////////////////////////////////////////////////////

void armax::loadParamset(QTextStream& inFile)
{ 	
 	for (int l=0;l<L;l++)
 		for (int m=0;m<M;m++)
 			inFile >> (*y_old_ini[l])(m,0);
 	for (int k=0;k<K;k++)
 		for (int n1=0;n1<N1;n1++)	
			inFile >> (*u_old_ini[k])(n1,0);
  	for (int r=0;r<R;r++)
 		for (int n2=0;n2<N2;n2++)
			inFile >> (*v_old_ini[r])(n2,0);
    for (int l=0;l<L;l++)
 		for (int m=0;m<M;m++)
 			for (int mm=0;mm<M;mm++)
				inFile >> (*A[l])(m,mm);
    for (int k=0;k<K;k++)
		for (int m=0;m<M;m++)	
			for (int n1=0;n1<N1;n1++)	
				inFile >> (*B[k])(m,n1);
    for (int r=0;r<R;r++)
		for (int m=0;m<M;m++)
 			for (int n2=0;n2<N2;n2++)
				inFile >> (*C[r])(m,n2);
	if (G==1)
		for (int m=0;m<M;m++)	
			for (int n3=0;n3<N3;n3++)	
				inFile >> (*D)(m,n3);
	if (H==1)
		for (int m=0;m<M;m++)
			inFile >> (*E)(m,0);
    for (int l=0;l<L;l++)
 		for (int m=0;m<M;m++)
			for (int mm=0;mm<M;mm++)
				inFile >> (*A_e[l])(m,mm);
    for (int k=0;k<K;k++)
		for (int m=0;m<M;m++)	
			for (int n1=0;n1<N1;n1++)	
				inFile >> (*B_e[k])(m,n1);
    for (int r=0;r<R;r++)
		for (int m=0;m<M;m++)
			for (int n2=0;n2<N2;n2++)
				inFile >> (*C_e[r])(m,n2);
	if (G==1)
		for (int m=0;m<M;m++)	
			for (int n3=0;n3<N3;n3++)	
				inFile >> (*D_e)(m,n3);
	if (H==1)
		for (int m=0;m<M;m++)
			inFile >> (*E_e)(m,0);
	inFile >> eps;	
	inFile >> sigma_eps;


  	inFile >> length;
  	
 	initialize();

}

    
///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:	armax
// Member function:	initialize
// Purpose:		initialize the model, define the systems initial state
//
// Author:		    mhoffman
// Last modified:	Feb 27 METDST 2001
// By:				mhoffman			
//
///////////////////////////////////////////////////////////////////////////////

void armax::initialize()
{	
	
	u=y=0;
	
	
	for (int l=0;l<L;l++)
			*y_old[l] = *y_old_ini[l];
	for (int k=0;k<K;k++)
			*u_old[k] = *u_old_ini[k];
	for (int r=0;r<R;r++)
			*v_old[r] = *v_old_ini[r];

	norm_u = 0;
	norm_y = 0;
	sum_u = 0;
	sum_y = 0;
	lambda_tm1 = 0;
	eps_t = 0;
	var1 = 0;
	var2 = 0;
	sum_var1 = 0;
	sum_var2 = 0;
	s_t = 0;	
	rand_rec = 0;

//P_0:
	for (int i=0;i<M + L*M + (K-1)*N1 + R*N2 + N3*G + 1*H;i++)
		for (int j=0;j<M + L*M + (K-1)*N1 + R*N2 + N3*G + 1*H;j++)
			if (i==j)
				(*P_tm1)(i,j)=0.2;
			else
				(*P_tm1)(i,j)=0;

	
//ini fuer alle As, Bs,...
/*	matrix_neu* B_inv;
	B_inv = new matrix_neu(N1+0,M+0);
	if( !B_inv )
		fatalError("matrix_neu::matrix_neu","Can't allocate memory");
	
	(*B_inv) = B[0]->inverse();
    log()<<"-------------------------------------------------------------------";
    log() << (*B_inv);
	(*B_inv) = B_inv->multiplyScalar(-1);		
    for (int l=1;l<L+1;l++)
        log() << (*B_inv) * (*A[l-1]);
    for (int k=0;k<K-1;k++)
        log()<< (*B_inv) * (*B[k+1]);
	delete(B_inv);
    log()<<"-------------------------------------------------------------------";
	
*/	
	
	(*B_1_inv) = B_e[0]->inverse();
//	(*B_1_inv) = B[0]->inverse();
	
	
	(*As_e[0]) = (*B_1_inv);

	(*B_1_inv) = B_1_inv->multiplyScalar(-1);	
    for(int l=1;l<L+1;l++)
		*As_e[l] = (*B_1_inv) * (*A_e[l-1]);
    for(int k=0;k<K-1;k++)
		*Bs_e[k] = (*B_1_inv) * (*B_e[k+1]);
    for(int r=0;r<R;r++)
		*Cs_e[r] = (*B_1_inv) * (*C_e[r]);
	if(G==1)
		*Ds_e = (*B_1_inv) * (*D_e);
	if(H==1)
		*Es_e = (*B_1_inv) * (*E_e);		
		



//Theta_0 (= Theta_tm1):	
    for (int l=0;l<L+1;l++)
		for (int n1=0;n1<N1;n1++)
			for (int m=0;m<M;m++)
				(*Theta)(n1,l*M + m) = (*As_e[l])(n1,m);		
    for(int k=0;k<K-1;k++)
		for (int n1=0;n1<N1;n1++)		
			for (int n1_2=0;n1_2<N1;n1_2++)
				(*Theta)(n1,(L+1)*M + k*N1 + n1_2) = (*Bs_e[k])(n1,n1_2);			
    for(int r=0;r<R;r++)
		for (int n1=0;n1<N1;n1++)
			for (int n2=0;n2<N2;n2++)
				(*Theta)(n1,(L+1)*M + (K-1)*N1 + r*N2 + n2) = (*Cs_e[r])(n1,n2);
	if (G==1)
		for (int n1=0;n1<N1;n1++)
			for (int n3=0;n3<N3;n3++)
				(*Theta)(n1,(L+1)*M + (K-1)*N1 + R*N2 + n3)= (*Ds_e)(n1,n3);	
	if (H==1)
		for (int n1=0;n1<N1;n1++)
			(*Theta)(n1,(L+1)*M + (K-1)*N1 + R*N2 + N3*G) = (*Es_e)(n1,0);	


//x_tm1_0 (=x_0):
//x_tm1 = (y_t, y_t-1,..., y_t-L, u_t-2,...,u_t-K, v_t-1,...,v_t-R, w_t, 1)	
		
	for(int m=0;m<M;m++)
		(*x_tm1)(0,m) = 0;		//y_t isn't calculated yet (done in the iteration)
    for(int l=0;l<L;l++)
		for(int m=0;m<M;m++)
			(*x_tm1)(0,M + l*M + m) = (*y_old[l])(m,0);
    for(int k=0;k<K-1;k++)
		for(int n1=0;n1<N1;n1++)
			(*x_tm1)(0,(L+1)*M + k*N1 + n1) = (*u_old[k+1])(n1,0);
    for(int r=0;r<R;r++)
		for(int n2=0;n2<N2;n2++)
			(*x_tm1)(0,(L+1)*M + (K-1)*N1 + r*N2 + n2) = (*v_old[r])(n2,0);
	if (G==1)
		for(int n3=0;n3<N3;n3++)
			(*x_tm1)(0,(L+1)*M + (K-1)*N1 + R*N2 + n3 ) = 0;	//w_t isn't drawn yet (done in the iteration)
	if (H==1)
		(*x_tm1)(0,(L+1)*M + (K-1)*N1 + R*N2 + N3*G) = 1;	

//benötigt für B^-1 * A[i] usw    unten in Iteration



}


///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		armax
// Member function:	iteration
// Purpose:		perform one iteration of the system
//
// Author:		    mhoffman
// Last modified:	Feb 27 METDST 2001
// By:				mhoffman			
//
///////////////////////////////////////////////////////////////////////////////

void armax::iteration(const qint64& t)
{ 	

//	qreal temp_max = ((L>K) ? L : K);	//qMax of L,K
//	const qreal qMaxLKR = ((temp_max>R) ? temp_max : R);	//qMax of L,K,R
//	const qreal eps_max = 1/(4*(qMaxLKR + L*1));	// if m is changed, update
											// this calculation
    const qreal eps_max=1.0/12.0;
    if((eps<=0)||(eps>=eps_max))
        fatalError("armax::iteration","eps out of range");
	
	
	rand_rec = rand_dis->rectangular(); // another r.u.variate
//	eps_t = rand_rec/5 - 0.1;			// get random number in (-0.1,0.1)

	matrix_neu *Temp1, *Temp2, *Temp3, *Temp4;		//temporary matrices

	for(int n2=0;n2<N2;n2++){
		rand_rec = rand_dis->rectangular(); // generate rectangular unit variate in (0,1)
		(*v_t)(n2,0) = rand_rec/5 - 0.1;	// get random number in (-0.1,0.1)	
//	(*v_t)(n2,0) =0;       falls kein Zufall!
	}	
// y_t:

	Temp1 = new matrix_neu(M+0,1);
	if( !Temp1)
		fatalError("matrix_neu::iteration","Can't allocate memory");	
//A*y
	for(int l=0;l<L;l++)
		(*Temp1) = (*Temp1) + (*A[l] * (*y_old[l]));
//B*u
	for(int k=0;k<K;k++)
		(*Temp1) = (*Temp1) + (*B[k] * (*u_old[k]));
//C*v
	for(int r=0;r<R;r++)
		(*Temp1) = (*Temp1) + (*C[r] * (*v_old[r]));
//D*w
	if (G==1){
		for(int n3=0;n3<N3;n3++){				//if switch G is on (==1), initialize w_t 									
			rand_rec = rand_dis->rectangular();	//and compute D*w_t
			(*w_t)(n3,0) = rand_rec - 0.5;     		// get random number in (-0.5,0.5)
		}
		(*Temp1) = (*Temp1) + ((*D) * (*w_t));
	}
//E
	if (H==1)                              //if switch H is off (==1), add E 
		(*Temp1) = (*Temp1) + (*E);

	(*y_t) = (*Temp1) + (*v_t);
	delete(Temp1);	


//now y_t is actualized

// ELS (estimate as, bs, cs, ds, es)

//actualize x_tm1 with "observed" y_t and drawn w_t

	for(int m=0;m<M;m++)
		(*x_tm1)(0,m) = (*y_t)(m,0);		
	if (G==1)
		for(int n3=0;n3<N3;n3++)
			(*x_tm1)(0,(L+1)*M + (K-1)*N1 + R*N2 + n3 ) = (*w_t)(n3,0);
//lambda_tm1:

	Temp1 = new matrix_neu(1,1);
	if( !Temp1)
		fatalError("matrix_neu::iteration","Can't allocate memory");	
	Temp2 = new matrix_neu(M + L*M + (K-1)*N1 + R*N2 + N3*G + 1*H,1);
	if( !Temp2)
		fatalError("matrix_neu::iteration","Can't allocate memory");	

	(*Temp2) = P_tm1->multiplyABT(*x_tm1);
	(*Temp1)= (*x_tm1) * (*Temp2);
	lambda_tm1 = 1/(1 + (*Temp1)(0,0));
	delete(Temp1);delete(Temp2);

//P_t:
	Temp1 = new matrix_neu(M + L*M + (K-1)*N1 + R*N2 + N3*G + 1*H,M + L*M + (K-1)*N1 + R*N2 + N3*G + 1*H);
	if( !Temp1)
		fatalError("matrix_neu::iteration","Can't allocate memory");	
	(*Temp1) = x_tm1->multiplyATB(*x_tm1);
	(*Temp1) = (*P_tm1) * (*Temp1) * (*P_tm1);
	(*P_t) = (*P_tm1) - (Temp1->multiplyScalar(lambda_tm1));
	delete(Temp1);


//Theta_t:
//up to now Theta is equal to Theta_tm1
//Temp1 = Theta_tm1*x_tm1
	Temp1 = new matrix_neu(N1+0,1);
	if( !Temp1)
		fatalError("matrix_neu::iteration","Can't allocate memory");		
	Temp2 = new matrix_neu(1,N1+0);
	if( !Temp2)
		fatalError("matrix_neu::iteration","Can't allocate memory");		
	Temp3 = new matrix_neu(M + L*M + (K-1)*N1 + R*N2 + N3*G + 1*H,N1+0);
	if( !Temp3)
		fatalError("matrix_neu::iteration","Can't allocate memory");		
	Temp4 = new matrix_neu(N1+0,M + L*M + (K-1)*N1 + R*N2 + N3*G + 1*H);
	if( !Temp4)
		fatalError("matrix_neu::iteration","Can't allocate memory");		
    for(int l=0;l<L+1;l++)
		for(int n1=0;n1<N1;n1++)
			for(int m=0;m<M;m++)
				(*Temp1)(n1,0) = (*Temp1)(n1,0) + ((*Theta)(n1,l*M + m) * (*x_tm1)(0,l*M + m));				
    for(int k=0;k<K-1;k++)
		for(int n1=0;n1<N1;n1++)
			for(int n1_2=0;n1_2<N1;n1_2++)
				(*Temp1)(n1,0) = (*Temp1)(n1,0) + ((*Theta)(n1,(L+1)*M + n1_2) * (*x_tm1)(0,(L+1)*M + n1_2));				
    for(int r=0;r<R;r++)
		for(int n1=0;n1<N1;n1++)
			for(int n2=0;n2<N2;n2++)
				(*Temp1)(n1,0) = (*Temp1)(n1,0) + ((*Theta)(n1,(L+1)*M + (K-1)*N1 + r) * (*x_tm1)(0,(L+1)*M + (K-1)*N1 + r));				
	if(G==1)
		for(int n1=0;n1<N1;n1++)
			for(int n3=0;n3<N3;n3++)
				(*Temp1)(n1,0) = (*Temp1)(n1,0) + ((*Theta)(n1,(L+1)*M + (K-1)*N1 + R*N2 + n3) * (*x_tm1)(0,(L+1)*M + (K-1)*N1 + R*N2 + n3));	
	if(H==1)
		for(int n1=0;n1<N1;n1++)
				(*Temp1)(n1,0) = (*Temp1)(n1,0) + ((*Theta)(n1,(L+1)*M + (K-1)*N1 + R*N2 + G*N3) * (*x_tm1)(0,(L+1)*M + (K-1)*N1 + R*N2 + G*N3));	
	(*Temp1) = (*u_old[0]) - (*Temp1);
	(*Temp2) = Temp1->transpose();
	(*Temp3) = x_tm1->multiplyATB(*Temp2);
	(*Temp3) = (*P_tm1) * (*Temp3);
	(*Temp3) = Temp3->multiplyScalar(lambda_tm1);
	
//Temp3 ist jetzt (LKR..xN1)-Matrix. Theta aber (N1xLKR..). Einfach transponieren geht bei Blockmatrizen nicht.
//also von Hand umschreiben:
    for(int l=0;l<L+1;l++)
		for(int n1=0;n1<N1;n1++)
			for(int m=0;m<M;m++)
				(*Temp4)(n1,l*M + m) = (*Temp3)(l*N1 + n1,m);		
    for(int k=0;k<K-1;k++)
		for(int n1=0;n1<N1;n1++)
			for(int n1_2=0;n1_2<N1;n1_2++)
				(*Temp4)(n1,(L+1)*M + k*N1 + n1_2) = (*Temp3)((L+1)*N1 + k*N1 + n1,n1_2);			
    for(int r=0;r<R;r++)
		for(int n1=0;n1<N1;n1++)
			for(int n2=0;n2<N2;n2++)
				(*Temp4)(n1,(L+1)*M + (K-1)*N1 + r*N2 + n2) = (*Temp3)((L+1)*N1 + (K-1)*N1 + r*N1 + n1,n2);			
	if(G==1)
		for(int n1=0;n1<N1;n1++)
			for(int n3=0;n3<N3;n3++)
				(*Temp4)(n1,(L+1)*M + (K-1)*N1 + R*N2 + n3) = (*Temp3)((L+1)*N1 + (K-1)*N1 + R*N1 + n1,n3); 	
	if(H==1)
		for(int n1=0;n1<N1;n1++)
			(*Temp4)(n1,(L+1)*M + (K-1)*N1 + R*N2 + G*N3) = (*Temp3)((L+1)*N1 + (K-1)*N1 + R*N1 + G*N3 + n1,0);  	
	(*Theta) = (*Theta) + (*Temp4);
	delete(Temp1); delete(Temp2);delete(Temp3); delete(Temp4);
//now Theta is equal to Theta_t

//eta_t:
/*	Temp1 = new matrix_neu(M + L*M + (K-1)*N1 + R*N2 + N3*G + 1*H,M + L*M + (K-1)*N1 + R*N2 + N3*G + 1*H);
	if( !Temp1)
		fatalError("matrix_neu::iteration","Can't allocate memory");		
	s_t = 1;                                                    //s_t=tr(P_t^-1);
	Temp1 = P_t->inverse();
	for (int i=0;i<M + L*M + (K-1)*N1 + R*N2 + N3*G + 1*H;i++)
		s_t *= (*Temp1)(i,i);
	delete(Temp1);
*/
  	s_t=t;
  	for(int n1=0;n1<N1;n1++){
       	rand_rec = rand_dis->rectangular();
      	eps_t =  rand_rec*(2.0*sigma_eps) - sigma_eps;           //eps_t in [-sigma_eps,sigma_eps]
       (*eta_t)(n1,0) = eps_t/(pow(s_t,eps/2.0));
   }

//create new x_tm1:
	for(int i=0;i<(L+1)*M-1;i++)
		(*x_tm1)(0,(L+1)*M-1-i) = (*x_tm1)(0,(L+1)*M-1-i-M);
//	for(int n1=0;n1<N1;n1++)
//		(*x_tm1)(0,n1) = (*u_old[K-2])(n1,0); 	//u_t-q+1 is the new target value for y_tp1 =(y*_tp1)
    for(int n1=0;n1<N1;n1++){
//		(*x_tm1)(0,n1) = 0.5;		//fuer konstante Kontrolle									
		rand_rec = rand_dis->rectangular(); 	// iid-Prozess als Kontrolle: generate rectangular unit variate in (0,1)
		(*x_tm1)(0,n1) = rand_rec/2 - 0.25;		// get random number in (-0.25,0.25)	
	}	
	
    for(int i=0;i<(K-2)*N1;i++)
		(*x_tm1)(0,(L+1)*M + (K-1)*N1-1-i) = (*x_tm1)(0,(L+1)*M + (K-1)*N1-1-i-N1);
	if(K>1)
        for(int n1=0;n1<N1;n1++)
			(*x_tm1)(0,(L+1)*M + n1) = (*u_old[0])(n1,0);		
    for(int i=0;i<(R-1)*N2;i++)
		(*x_tm1)(0,(L+1)*M + (K-1)*N1 + R*N2-1-i) = (*x_tm1)(0,(L+1)*M + (K-1)*N1 + R*N2-1-i-N2);
	if(R>0)
        for(int n2=0;n2<N2;n2++)
			(*x_tm1)(0,(L+1)*M + (K-1)*N1 + n2) = (*v_t)(n2,0);
	if(G==1)
		for(int n3=0;n3<N3;n3++)
			(*x_tm1)(0,(L+1)*M + (K-1)*N1 + R*N2 + n3) = (*w_t)(n3,0);	
			
//calculate u_t
	

	Temp1 = new matrix_neu(N1+0,1);
	if( !Temp1)
		fatalError("matrix_neu::iteration","Can't allocate memory");		
    for(int l=0;l<L+1;l++)
		for(int n1=0;n1<N1;n1++)
			for(int m=0;m<M;m++)
				(*Temp1)(n1,0) = (*Temp1)(n1,0) + ((*Theta)(n1,l*M + m) * (*x_tm1)(0,l*M + m));				
    for(int k=0;k<K-1;k++)
		for(int n1=0;n1<N1;n1++)
			for(int n1_2=0;n1_2<N1;n1_2++)
				(*Temp1)(n1,0) = (*Temp1)(n1,0) + ((*Theta)(n1,(L+1)*M + n1_2) * (*x_tm1)(0,(L+1)*M + n1_2));				
    for(int r=0;r<R;r++)
		for(int n1=0;n1<N1;n1++)
			for(int n2=0;n2<N2;n2++)
				(*Temp1)(n1,0) = (*Temp1)(n1,0) + ((*Theta)(n1,(L+1)*M + (K-1)*N1 + r) * (*x_tm1)(0,(L+1)*M + (K-1)*N1 + r));				
	if(G==1)
		for(int n1=0;n1<N1;n1++)
			for(int n3=0;n3<N3;n3++)
				(*Temp1)(n1,0) = (*Temp1)(n1,0) + ((*Theta)(n1,(L+1)*M + (K-1)*N1 + R*N2 + n3) * (*x_tm1)(0,(L+1)*M + (K-1)*N1 + R*N2 + n3));	
	if(H==1)
		for(int n1=0;n1<N1;n1++)
				(*Temp1)(n1,0) = (*Temp1)(n1,0) + ((*Theta)(n1,(L+1)*M + (K-1)*N1 + R*N2 + G*N3) * (*x_tm1)(0,(L+1)*M + (K-1)*N1 + R*N2 + G*N3));	
	*u_t = (*Temp1) + (*eta_t);
	delete(Temp1);	

// variance check
	
	if(t>49){		//first 49 iterations aren't considered
		Temp1 = new matrix_neu(M+0,1);
		if( !Temp1)
			fatalError("matrix_neu::iteration","Can't allocate memory");		
		Temp2 = new matrix_neu(1,1);
		if( !Temp2)
			fatalError("matrix_neu::iteration","Can't allocate memory");		
		*dif = (*y_t) - (*u_old[K-1]);  //y_t - y*_t 		
		*Temp1 = *dif;
		*Temp2= dif->multiplyATB(*Temp1);
		delete(Temp1);		
		sum_var1 += (*Temp2)(0,0);
        var1 = (1.0/qreal(t-49)) * sum_var1;
		Temp1 = new matrix_neu(N2+0,1);
		if( !Temp1)
			fatalError("matrix_neu::iteration","Can't allocate memory");		
		*Temp1 = *v_t;
		*Temp2 = v_t->multiplyATB(*Temp1);
		sum_var2 += (*Temp2)(0,0);
//		var2 = 1/qreal(t-49) * sum_var2;
		var2=(0.2 * 0.2 * 0.2)/12.0;	//for iid: var=(b-a) /12
		delete(Temp1);delete(Temp2);
		dif_var = var1 - var2;	//should converge to zero
	}


//log() << "\ny_t:"<<*y_t;
//log() << "\nu_t:"<<*u_t;
//log() << "\nTheta:" << *Theta;
//log() << "\nx_tm1:" << *x_tm1;

/*
log() << "\ndif"<<*dif;
log() << "\nsum_var1="<<sum_var1;
log() << "\nsum_var2="<<sum_var2;
log() << "\nvar1="<<var1;
log() << "\nvar2="<<var2;
log() << "\ndif_var="<<dif_var;
*/
/*		



//actualize as_e, bs_e, cs_e, ds_e, es_e
	for (l=0;l<L+1;l++)
		as_e[l] = (*Theta)(l,0);
	for (k=0;k<K-1;k++)
		bs_e[k] = (*Theta)(L+1+k,0);
	for (r=0;r<R;r++)
		cs_e[r] = (*Theta)(L+K+r,0);
	if (G==1)
		ds_e = (*Theta)(L+K+R,0);		
  	if (H==1)
		es_e = (*Theta)(L+K+R+G,0);		
	
*/		
// shifting for next period

	(*P_tm1) = (*P_t);			
    for (int l=1;l<L;l++)
		*y_old[L-l] = *y_old[L-1-l];
	*y_old[0] = *y_t;
    for (int k=1;k<K;k++)
		*u_old[K-k] = *u_old[K-1-k];
	*u_old[0] = *u_t;
    for (int r=1;r<R;r++)
		*v_old[R-r] = *v_old[R-1-r];
	if (R>0)
		*v_old[0] = *v_t;





/*		
// calculate error of estimated values in the last period
	if (t==length){
		error_as_e[0] = 1/b[0] - as_e[0];
		for (l=1;l<L+1;l++)
			error_as_e[l] = ((-1/b[0]) * a[l-1]) - as_e[l];
		for (k=0;k<K-1;k++)
			error_bs_e[k] = ((-1/b[0]) * b[k+1]) - bs_e[k];
		for (r=0;r<R;r++)
			error_cs_e[r] = ((-1/b[0]) * c[r]) - cs_e[r];
		if (G==1)
			error_ds_e = ((-1/b[0]) * d) - ds_e;
		if (H==1)
			error_es_e = ((-1/b[0]) * e) - es_e;		
// output on screen in the last period
        log() << "\nas[0] - as_e[0] = " << 1/b[0] - as_e[0] 
		for (l=1;l<L+1;l++)
            log() << "as[" << l << "] - as_e[" << l << "] = " << ((-1/b[0]) * a[l-1]) - as_e[l] 
		for (k=0;k<K-1;k++)
            log() << "bs[" << k << "] - bs_e[" << k << "] = " << ((-1/b[0]) * b[k+1]) - bs_e[k] 
		for (r=0;r<R;r++)
            log() << "cs[" << r << "] - cs_e[" << r << "] = " << ((-1/b[0]) * c[r]) - cs_e[r] 
		if (G==1)
            log() << "ds - ds_e = " << ((-1/b[0]) * d) - ds_e 
		if (H==1)
            log() << "es - es_e = " << ((-1/b[0]) * e) - es_e 
        log() << "last dif_var=" << dif_var 
        log() << "var1 = " << var1 
        log() << "var2 = " << var2 
        log() << "sum_var1 = " << sum_var1 
	}
*/

		difA = new matrix_neu(2,2);
		if( !difA)
			fatalError("matrix_neu::iteration","Can't allocate memory");		
		difB = new matrix_neu(2,2);
		if( !difB)
			fatalError("matrix_neu::iteration","Can't allocate memory");		
  		Temp1 = new matrix_neu(2,2);
		if( !Temp1)
			fatalError("matrix_neu::iteration","Can't allocate memory");		
			
   	if (t==length){
	    (*As_e[0])(0,0) = (*Theta)(0,0);
  		(*As_e[0])(1,0) = (*Theta)(1,0);
  		(*As_e[0])(0,1) = (*Theta)(0,1);
  		(*As_e[0])(1,1) = (*Theta)(1,1);
//  	    log() << "As_e0:"<<*As_e[0];
 		(*As_e[1])(0,0) = (*Theta)(0,2);
  		(*As_e[1])(1,0) = (*Theta)(1,2);
  		(*As_e[1])(0,1) = (*Theta)(0,3);
  		(*As_e[1])(1,1) = (*Theta)(1,3);
//  	 	log() << "As_e1:"<<*As_e[1];
		(*As_e[2])(0,0) = (*Theta)(0,4);
  		(*As_e[2])(1,0) = (*Theta)(1,4);
  		(*As_e[2])(0,1) = (*Theta)(0,5);
  		(*As_e[2])(1,1) = (*Theta)(1,5);  	 					
// 	 	log() << "As_e2:"<<*As_e[2];
		(*Bs_e[0])(0,0) = (*Theta)(0,6);
  		(*Bs_e[0])(1,0) = (*Theta)(1,6);
  		(*Bs_e[0])(0,1) = (*Theta)(0,7);
  		(*Bs_e[0])(1,1) = (*Theta)(1,7);  		
//	 	log() << "Bs_e0:"<<*Bs_e[0];

 		(*B_1_inv) = B[0]->inverse();
		(*Temp1) = (*B_1_inv);
		(*difA) = (*As_e[0]) - (*Temp1);		

	    normA1 = (*difA)(0,0) * (*difA)(0,0);
	    normA1 +=  (*difA)(0,1) * (*difA)(0,1);
     	normA1 +=  (*difA)(1,0) * (*difA)(1,0);
        normA1 +=  (*difA)(1,1) * (*difA)(1,1);
        normA1 = sqrt(normA1);
        log() << "\nnormA_inv_1="<<normA1;
		
		(*B_1_inv) = B_1_inv->multiplyScalar(-1);	
		*Temp1 = (*B_1_inv) * (*A[0]);
		(*difA) = (*As_e[1]) - (*Temp1);		
	    normA2 = (*difA)(0,0) * (*difA)(0,0);
	    normA2 +=  (*difA)(0,1) * (*difA)(0,1);
     	normA2 +=  (*difA)(1,0) * (*difA)(1,0);
        normA2 +=  (*difA)(1,1) * (*difA)(1,1);
        normA2 = sqrt(normA2);
        log() << "\nnormA_inv_2="<<normA2;

 	    *Temp1 = (*B_1_inv) * (*A[1]);
		(*difA) = (*As_e[2]) - (*Temp1);		
	    normA3 = (*difA)(0,0) * (*difA)(0,0);
	    normA3 +=  (*difA)(0,1) * (*difA)(0,1);
     	normA3 +=  (*difA)(1,0) * (*difA)(1,0);
        normA3 +=  (*difA)(1,1) * (*difA)(1,1);
        normA3 = sqrt(normA3);
        log() << "\nnormA_inv_3="<<normA3;

	    *Temp1 = (*B_1_inv) * (*B[1]);
		(*difA) = (*Bs_e[0]) - (*Temp1);		
	    normB1 = (*difA)(0,0) * (*difA)(0,0);
	    normB1 +=  (*difA)(0,1) * (*difA)(0,1);
     	normB1 +=  (*difA)(1,0) * (*difA)(1,0);
        normB1 +=  (*difA)(1,1) * (*difA)(1,1);
        normB1 = sqrt(normB1);
        log() << "\nnormB_inv_1="<<normB1<<"\n";
	
		delete(Temp1);	
	
	
	
	     }
	

	
	
	

		u=(*u_t)(0,0);
		y=(*y_t)(0,0);
		norm_u = ((*u_t)(0,0)*(*u_t)(0,0)) + ((*u_t)(1,0)*(*u_t)(1,0));
        norm_y = ((*y_t)(0,0)*(*y_t)(0,0)) + ((*y_t)(1,0)*(*y_t)(1,0));
		sum_u += 1.0/t * norm_u;
		sum_y += 1.0/t * norm_y;
        sum_uy = sum_u + sum_y;



}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		armax
// Member function:	setLabels
// Purpose:		return a pointer to a variable or a parameter specified
//
// Author:		    mhoffman
// Last modified:	Feb 27 METDST 2001
// By:				mhoffman			
//
///////////////////////////////////////////////////////////////////////////////

qreal* armax::setLabels(const QString& label)
{
    if (label == "dif_var") return(&dif_var);
    if (label == "var1") return(&var1);
    if (label == "var2") return(&var2);
    if (label == "sum_var1") return(&sum_var1);
    if (label == "y") return(&y);
    if (label == "u") return(&u);
    if (label == "norm_u") return(&norm_u);
    if (label == "norm_y") return(&norm_y);
    if (label == "sum_u") return(&sum_u);
    if (label == "sum_y") return(&sum_y);
    if (label == "sum_uy") return(&sum_uy);
    if (label == "normA1") return(&normA1);
    if (label == "normA2") return(&normA2);
    if (label == "normA3") return(&normA3);
    if (label == "normB1") return(&normB1);


    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:	armax
// Member function:	saveParamset
// Purpose:		write parameterset into a file
//
// Author:		    mhoffman
// Last modified:	Feb 27 METDST 2001
// By:				mhoffman			
//
///////////////////////////////////////////////////////////////////////////////

void armax::saveParamset(QTextStream& outFile)
{	
	for (int l=0;l<L;l++)
 		for (int m=0;m<M;m++)
 			outFile << (*y_old_ini[l])(m,0) << "\t";
 	for (int k=0;k<K;k++)
 		for (int n1=0;n1<N1;n1++)	
			outFile << (*u_old_ini[k])(n1,0) << "\t";
  	for (int r=0;r<R;r++)
 		for (int n2=0;n2<N2;n2++)
			outFile << (*v_old_ini[r])(n2,0) << "\t";
    for (int l=0;l<L;l++)
 		for (int m=0;m<M;m++)
 			for (int mm=0;mm<M;mm++)
				outFile << (*A[l])(m,mm) << "\t";
    for (int k=0;k<K;k++)
		for (int m=0;m<M;m++)	
			for (int n1=0;n1<N1;n1++)	
				outFile << (*B[k])(m,n1) << "\t";
    for (int r=0;r<R;r++)
		for (int m=0;m<M;m++)
			for (int n2=0;n2<N2;n2++)
				outFile << (*C[r])(m,n2) << "\t";
	if (G==1)
		for (int m=0;m<M;m++)	
			for (int n3=0;n3<N3;n3++)	
				outFile << (*D)(m,n3) << "\t";
	if (H==1)
		for (int m=0;m<M;m++)
			outFile << (*E)(m,0) << "\t";
    for (int l=0;l<L;l++)
 		for (int m=0;m<M;m++)
 			for (int mm=0;mm<M;mm++)
				outFile << (*A_e[l])(m,mm) << "\t";
    for (int k=0;k<K;k++)
		for (int m=0;m<M;m++)	
			for (int n1=0;n1<N1;n1++)	
				outFile << (*B_e[k])(m,n1) << "\t";
    for (int r=0;r<R;r++)
		for (int m=0;m<M;m++)
			for (int n2=0;n2<N2;n2++)
				outFile << (*C_e[n2])(m,n2) << "\t";
	if (G==1)
		for (int m=0;m<M;m++)	
			for (int n3=0;n3<N3;n3++)	
				outFile << (*D_e)(m,n3) << "\t";
	if (H==1)
		for (int m=0;m<M;m++)
			outFile << (*E_e)(m,0) << "\t";
	outFile << eps;
	outFile << sigma_eps;

			
			
    outFile << length;

}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:	armax
// Member function:	printParamset
// Purpose:		print parameterset on the screen
//
// Author:		    mhoffman
// Last modified:	Feb 27 METDST 2001
// By:				mhoffman			
//
///////////////////////////////////////////////////////////////////////////////

void armax::printParamset()
{ 	
 	for (int l=0;l<L;l++)
 		for (int m=0;m<M;m++)
            log() << (*y_old_ini[l])(m,0);
 	for (int k=0;k<K;k++)
 		for (int n1=0;n1<N1;n1++)	
            log() << (*u_old_ini[k])(n1,0);
  	for (int r=0;r<R;r++)
 		for (int n2=0;n2<N2;n2++)
            log() << (*v_old_ini[r])(n2,0);
    for (int l=0;l<L;l++)
 		for (int m=0;m<M;m++)
 			for (int mm=0;mm<M;mm++)
                log() << (*A[l])(m,mm);
    for (int k=0;k<K;k++)
		for (int m=0;m<M;m++)	
			for (int n1=0;n1<N1;n1++)	
                log() << (*B[k])(m,n1);
    for (int r=0;r<R;r++)
		for (int m=0;m<M;m++)
			for (int n2=0;n2<N2;n2++)
                log() << (*C[r])(m,n2);
	if (G==1)
		for (int m=0;m<M;m++)	
			for (int n3=0;n3<N3;n3++)	
                log() << (*D)(m,n3);
	if (H==1)
		for (int m=0;m<M;m++)
            log() << (*E)(m,0);
    for (int l=0;l<L;l++)
 		for (int m=0;m<M;m++)
			for (int mm=0;mm<M;mm++)
                log() << (*A_e[l])(m,mm);
    for (int k=0;k<K;k++)
		for (int m=0;m<M;m++)	
			for (int n1=0;n1<N1;n1++)	
                log() << (*B_e[k])(m,n1);
    for (int r=0;r<R;r++)
		for (int m=0;m<M;m++)
			for (int n2=0;n2<N2;n2++)
                log() << (*C_e[r])(m,n2);
	if (G==1)
		for (int m=0;m<M;m++)	
			for (int n3=0;n3<N3;n3++)	
                log() << (*D_e)(m,n3);
	if (H==1)
		for (int m=0;m<M;m++)
            log() << (*E_e)(m,0);
    log() << eps;
    log() << sigma_eps;

			
    log() << length;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:	armax
// Member function:	sendStateSpace
// Purpose:		return pointers to the state variables and inform about
//			the systems dimension
//
// Author:		    mhoffman
// Last modified:	Feb 27 METDST 2001
// By:				mhoffman			
//
///////////////////////////////////////////////////////////////////////////////

void armax::sendStateSpace(int &quantity,const qreal*** stateSpace)
{
    if( stateSpace )
	delete stateSpace;
    *stateSpace= new const qreal* [dimension];
    if( !(*stateSpace) )
    fatalError("armax::sendStateSpace",
		   "Can't create state space vector");
    quantity=dimension;
    for (int m=0;m>M;m++)
        (*stateSpace)[m]=&((*y_t)(m,0));

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
qreal* armax::sendModelVar(void)
{ error("macrodyn::armax::sendModelVar is not implemented");
  return NULL;
} 
void armax::sendParameters(int& ,qreal** )
{ error("macrodyn::armax::sendParameters is not implemented");
}
void armax::receiveParameters(const QList<qreal>&)
{ error("macrodyn::armax::receiveParameters is not implemented");
} 
