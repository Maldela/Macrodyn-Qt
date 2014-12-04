///////////////////////////////////////////////////////////////////////////////
//
// $Header: armax_eindim.C, Thu Jul  6 17:09:23 METDST 2000 mmeyer
//
// Module name:		armax_eindim.C
// Contents:		Member functions of the class armax_eindim
//
//
// Author:		    mhoffman
// Last modified:	Feb 27 METDST 2001
// By:				mhoffman			
//
///////////////////////////////////////////////////////////////////////////////

#include "../error.h"
#include "armax_eindim.h"

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:	armax_eindim
// Member function:	constructor
// Purpose:		constructor
//
// Author:		    mhoffman
// Last modified:	Feb 27 METDST 2001
// By:				mhoffman			
//
///////////////////////////////////////////////////////////////////////////////

armax_eindim::armax_eindim() : baseModel(1)
{
	TempLKRGH_1 = new matrix_neu(L+K+R+G+H,1);
	if( !TempLKRGH_1 )
		fatalError("matrix_neu::matrix_neu","Can't allocate memory");

	TempLKRGH_1_2 = new matrix_neu(L+K+R+G+H,1);
	if( !TempLKRGH_1_2 )
		fatalError("matrix_neu::matrix_neu","Can't allocate memory");

	Temp1_LKRGH = new matrix_neu(1,L+K+R+G+H);
	if( !Temp1_LKRGH )
		fatalError("matrix_neu::matrix_neu","Can't allocate memory");

	x_tm1 = new matrix_neu(L+K+R+G+H,1);
	if( !x_tm1 )
		fatalError("matrix_neu::matrix_neu","Can't allocate memory");

	Theta = new matrix_neu(L+K+R+G+H,1);
	if( !Theta )
		fatalError("matrix_neu::matrix_neu","Can't allocate memory");

	Temp11 = new matrix_neu(1,1);
	if( !Temp11 )
		fatalError("matrix_neu::matrix_neu","Can't allocate memory");

	TempLKRGH_LKRGH = new matrix_neu(L+K+R+G+H,L+K+R+G+H);
	if( !TempLKRGH_LKRGH )
		fatalError("matrix_neu::matrix_neu","Can't allocate memory");

	TempLKRGH_LKRGH_2 = new matrix_neu(L+K+R+G+H,L+K+R+G+H);
	if( !TempLKRGH_LKRGH_2 )
		fatalError("matrix_neu::matrix_neu","Can't allocate memory");

	P_tm1 = new matrix_neu(L+K+R+G+H,L+K+R+G+H);
	if( !P_tm1 )
		fatalError("matrix_neu::matrix_neu","Can't allocate memory");

	P_t = new matrix_neu(L+K+R+G+H,L+K+R+G+H);
	if( !P_t )
		fatalError("matrix_neu::matrix_neu","Can't allocate memory");

	point = new matrixFn();
	if( !point )
		fatalError("matrix_neu::matrix_neu","Can't allocate memory");

	if ( rand_dis != NULL ) delete rand_dis;
	rand_dis = new distribution();
	if( !(rand_dis) )
		fatalError("armax_eindim:","can't create rand_dis");
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:	armax_eindim	
// Member function:	 ~armax_eindim
// Purpose:		destructor
//
// Author:		    mhoffman
// Last modified:	Feb 27 METDST 2001
// By:				mhoffman			
//
///////////////////////////////////////////////////////////////////////////////

armax_eindim::~armax_eindim()
{
	if(P_tm1)			delete P_tm1;	
	if(Theta)			delete Theta;	
	if(Temp11)			delete Temp11;
	if(P_t)				delete P_t;
	if(TempLKRGH_1)			delete TempLKRGH_1;
	if(TempLKRGH_1_2)		delete TempLKRGH_1_2;
	if(TempLKRGH_LKRGH)		delete TempLKRGH_LKRGH;
	if(TempLKRGH_LKRGH_2)		delete TempLKRGH_LKRGH_2;
	if(Temp1_LKRGH)			delete Temp1_LKRGH;
	if(x_tm1)			delete x_tm1;
	if(point)			delete point;
	if(rand_dis) 			delete rand_dis;

}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:	armax_eindim
// Member function:	loadParamset
// Purpose:		load a parameterset from a specified input file
//
// Author:		    mhoffman
// Last modified:	Feb 27 METDST 2001
// By:				mhoffman			
//
///////////////////////////////////////////////////////////////////////////////

void armax_eindim::loadParamset(ifstream& inFile)
{ 	
 	for (int l=0;l<L;l++)
		inFile >> y_old_ini[l];
 	for (int k=0;k<K;k++)
		inFile >> u_old_ini[k];
  	for (int r=0;r<R;r++)
		inFile >> v_old_ini[r];
    for (int l=0;l<L;l++)
		inFile >> a[l];
    for (int k=0;k<K;k++)
		inFile >> b[k];
    for (int r=0;r<R;r++)
		inFile >> c[r];
	if (G==1)
		inFile >> d;
	if (H==1)
		inFile >> e;
    for (int l=0;l<L;l++)
		inFile >> a_e[l];
    for (int k=0;k<K;k++)
		inFile >> b_e[k];
    for (int r=0;r<R;r++)
		inFile >> c_e[r];
 	if (G==1)	
 		inFile >> d_e;
	if (H==1)
		inFile >> e_e;

	inFile >> eps;
	inFile >> sigma_eps;	
			
  	inFile >> length;
  	
 	initialize();

}


///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:	armax_eindim	
// Member function:	initialize
// Purpose:		initialize the model, define the systems initial state
//
// Author:		    mhoffman
// Last modified:	Feb 27 METDST 2001
// By:				mhoffman			
//
///////////////////////////////////////////////////////////////////////////////

void armax_eindim::initialize()
{	
	for (int l=0;l<L;l++)
		y_old[l] = y_old_ini[l];
	for (int k=0;k<K;k++)
		u_old[k] = u_old_ini[k];
	for (int r=0;r<R;r++)
		v_old[r] = v_old_ini[r];	
    for (int l=0;l<L+1;l++)
		error_as_e[l] = 0;
    for (int k=0;k<K-1;k++)
		error_bs_e[k] = 0;
    for (int r=0;r<R;r++)
		error_cs_e[r] = 0;
	if (G==1)
		error_ds_e = 0;
	if (H==1)
		error_es_e = 0;		

	y_t = 0;
	u_t = 0;
	v_t = 0;
	lambda_tm1 = 0;
	eps_t = 0;
	var1 = 0;
	var2 = 0;
	sum_var1 = 0;
	sum_var2 = 0;
	s_t = 0;	
	rand_rec = 0;
	eta_t = 0;
    norm_u = 0;
    norm_y = 0;
    sum_u = 0;
    sum_y = 0;
    sum_uy = 0;
	sum_u_buf = 0;
	sum_y_buf = 0;
	
	
//P_0:
	for (int i=0;i<L+K+R+G+H;i++)
		for (int j=0;j<L+K+R+G+H;j++)
			if (i==j)
				(*P_tm1)(i,j)=0.2;
			else
				(*P_tm1)(i,j)=0;	
//Theta_0:	
	(*Theta)(0,0) = as_e[0] = 1/b_e[0];	// = Theta_tm1	
    for(int l=0;l<L;l++)
		(*Theta)(l+1,0) = as_e[l+1] = (-1/b_e[0]) * a_e[l];	
    for(int k=1;k<K;k++)
		(*Theta)(L+k,0) = bs_e[k-1] = (-1/b_e[0]) * b_e[k];
    for(int r=0;r<R;r++)
		(*Theta)(L+K+r,0) = cs_e[r] = (-1/b_e[0]) * c_e[r];
	if (G==1)
		(*Theta)(L+K+R,0)= ds_e = (-1/b_e[0]) * d_e;	
	if (H==1)
		(*Theta)(L+K+R+G,0) = es_e = (-1/b_e[0]) * e_e;	

//x_tm1_0 (=x_0):
//x_tm1 = (y_t, y_t-1,..., y_t-L, u_t-2,...,u_t-K, v_t-1,...,v_t-R, w_t, 1)	
		
	(*x_tm1)(0,0) = 0;		//y_t isn't calculated yet (done in the iteration)
    for(int l=0;l<L;l++)
		(*x_tm1)(l+1,0) = y_old[l];
    for(int k=0;k<K-1;k++)
		(*x_tm1)(L+1+k,0) = u_old[k+1];
    for(int r=0;r<R;r++)
		(*x_tm1)(L+K+r,0) = v_old[r];
	if (G==1)
		(*x_tm1)(L+K+R,0) = 0;	//w_t isn't drawn yet (done in the iteration)
	if (H==1)
		(*x_tm1)(L+K+R+G,0) = 1;	

}


///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		armax_eindim
// Member function:	iteration
// Purpose:		perform one iteration of the system
//
// Author:		    mhoffman
// Last modified:	Feb 27 METDST 2001
// By:				mhoffman			
//
///////////////////////////////////////////////////////////////////////////////

void armax_eindim::iteration(const qint64& t)
{ 	
    qreal sum1, sum2, sum3;
    qreal temp_max = ((L>K) ? L : K);	//max of L,K
    const qreal maxLKR = ((temp_max>R) ? temp_max : R);	//max of L,K,R
    const qreal eps_max = 1.0/(2.0*(maxLKR + L*1.0));	// if m is changed, update
												// this calculation
	if((eps<=0)||(eps>=eps_max))
		fatalError("armax::iteration","eps out of range");		
	
	rand_rec = rand_dis->rectangular(); // generates rectangular unit variate
										// in (0,1)
	v_t = rand_rec/5 - 0.1;				// get random number in (-0.1,0.1)	
//v_t=0;		//without noise	
    qreal w_t;
		
	if (G==1){
		rand_rec = rand_dis->rectangular();	
		w_t = rand_rec - 0.5;     		// get random number in (-0.5,0.5)
	} else
		d = w_t = 0;                    //if switch G is off (==0), d and w_t aren't needed

// y_t:
	sum1 = sum2 = sum3 = 0;
	for(int l=0;l<L;l++)
		sum1 += a[l]*y_old[l];
	for(int k=0;k<K;k++)
		sum2 += b[k]*u_old[k];
	for(int r=0;r<R;r++)
		sum3 += c[r]*v_old[r];
	if (H==0)                              //if switch H is off (==0), e isn't needed
		e = 0;
	y_t = sum1 + sum2 + sum3 + v_t + d*w_t + e;
		
// ELS (estimate as, bs, cs, ds, es)

//actualize x_tm1 with "observed" y_t and drawn w_t
	(*x_tm1)(0,0) = y_t;
	if (G==1)
		(*x_tm1)(L+K+R,0) = w_t;
	
//lambda_tm1:
	(*Temp1_LKRGH) = x_tm1->multiplyATB(*P_tm1);
	(*Temp11) = (*Temp1_LKRGH) * (*x_tm1);
	lambda_tm1 = 1/(1 + (*Temp11)(0,0));

//P_t:
	(*TempLKRGH_LKRGH) = x_tm1->multiplyABT(*x_tm1);
	(*TempLKRGH_LKRGH_2) = (*P_tm1) * (*TempLKRGH_LKRGH) * (*P_tm1);
	(*P_t) = (*P_tm1) - (TempLKRGH_LKRGH_2->multiplyScalar(lambda_tm1));

//Theta_t:
//up to now Theta is equal to Theta_tm1
	sum1 = 0;
	for(int i=0;i<L+K+R+G+H;i++)
		sum1 += (*Theta)(i,0) * (*x_tm1)(i,0);
	temp1 = u_old[0] - sum1;
	(*Temp11)(0,0) = temp1;
	(*TempLKRGH_LKRGH) = (*P_tm1) * ((*x_tm1) * (*Temp11));
	(*Theta) = (*Theta) + (TempLKRGH_LKRGH->multiplyScalar(lambda_tm1));
//now Theta is equal to Theta_t

//eta_t:
/*	
	P_t_inv = new matrix_neu(L+K+R+G+H,L+K+R+G+H);
	if( !P_t_inv )
		fatalError("matrix_neu::matrix_neu","Can't allocate memory");
	*P_t_inv = P_t->inverse();
	s_t = 1;
	for(i=0;i<L+K+R+G+H;i++)
		s_t *= (*P_t_inv)(i,i);
*/
	s_t = t;   	
   	rand_rec = rand_dis->rectangular();
   	eps_t =  rand_rec*(2.0*sigma_eps) - sigma_eps;           //eps_t in [-sigma_eps,sigma_eps]	
	eta_t = eps_t / pow(s_t,(eps/2));	
//	delete(P_t_inv);

//create new x_tm1:
    for(int i=0;i<L;i++)
		(*x_tm1)(L-i,0) = (*x_tm1)(L-1-i,0);
//	(*x_tm1)(0,0) = u_old[K-2]; //u_t-q+1 is the new target value for y_tp1 =(y*_tp1)
//	(*x_tm1)(0,0) = 0.5;	//constant target value	
	rand_rec = rand_dis->rectangular(); 	// iid-Prozess als Sollwert: generate rectangular unit variate in (0,1)
	(*x_tm1)(0,0) = rand_rec/2 - 0.25;		// get random number in (-0.25,0.25)	
   	
   		
    for(int i=0;i<K-2;i++)
		(*x_tm1)(L+K-1-i,0) = (*x_tm1)(L+K-2-i,0);
	if(K>1)
		(*x_tm1)(L+1,0) = u_old[0];		
    for(int i=0;i<R-1;i++)
		(*x_tm1)(L+K+R-1-i,0) = (*x_tm1)(L+K+R-2-i,0);
	if(R>0)
		(*x_tm1)(L+K,0) = v_t;	
				
//calculate u_t
	sum1 = 0;
    for(int i=0;i<(L+1)+(K-1)+R+G+H;i++)
		sum1 += (*Theta)(i,0) * (*x_tm1)(i,0);
	u_t = sum1 + eta_t;
	
/* nicht mehr aktuell:(alte Version)	
// variance check
	if(t>49){		//first 49 iterations aren't considered
		dif = y_t - u_old[K-1];  //y_t - y*_t 		
		sum_var1 += (y_t - u_old[K-1]) * (y_t - u_old[K-1]);
        var1 = 1.0/qreal(t-49) * sum_var1;
		sum_var2 += v_t * v_t;
//		var2 = 1/qreal(t-49) * sum_var2;
		var2=(0.2 * 0.2 * 0.2)/12.0;	//for iid: var=(b-a) /12
		dif_var = var1 - var2;	//should converge to zero
	}
	
*/	

//actualize as_e, bs_e, cs_e, ds_e, es_e
    for (int l=0;l<L+1;l++)
		as_e[l] = (*Theta)(l,0);
    for (int k=0;k<K-1;k++)
		bs_e[k] = (*Theta)(L+1+k,0);
    for (int r=0;r<R;r++)
		cs_e[r] = (*Theta)(L+K+r,0);
	if (G==1)
		ds_e = (*Theta)(L+K+R,0);		
  	if (H==1)
		es_e = (*Theta)(L+K+R+G,0);		
			
// shifting for next period
    for (int i=0;i<L+K+R+G+H;i++)
		for (int j=0;j<L+K+R+G+H;j++)
			(*P_tm1)(i,j) = (*P_t)(i,j);
			
    for (int l=1;l<L;l++)
		y_old[L-l] = y_old[L-1-l];
	y_old[0] = y_t;

    for (int k=1;k<K;k++)
		u_old[K-k] = u_old[K-1-k];
	u_old[0] = u_t;

    for (int r=1;r<R;r++)
		v_old[R-r] = v_old[R-1-r];
	if (R>0)
		v_old[0] = v_t;
		
	
	
	
	norm_u = u_t * u_t;
	norm_y = y_t * y_t;
	sum_u_buf += norm_u;
	sum_u = 1.0/t * sum_u_buf;
	sum_y_buf += norm_y;
	sum_y = 1.0/t * sum_y_buf;
	sum_uy = sum_u + sum_y;



// calculate error of estimated values

		error_as_e[0] = 1/b[0] - as_e[0];
        for (int l=1;l<L+1;l++)
			error_as_e[l] = ((-1/b[0]) * a[l-1]) - as_e[l];
        for (int k=0;k<K-1;k++)
			error_bs_e[k] = ((-1/b[0]) * b[k+1]) - bs_e[k];
        for (int r=0;r<R;r++)
			error_cs_e[r] = ((-1/b[0]) * c[r]) - cs_e[r];
		if (G==1)
			error_ds_e = ((-1/b[0]) * d) - ds_e;
		if (H==1)
			error_es_e = ((-1/b[0]) * e) - es_e;		
// output on screen in the last period
	if (t==length){		
        log() << "\nas[0] - as_e[0] = " << 1/b[0] - as_e[0] << "\n";
        for (int l=1;l<L+1;l++)
            log() << "as[" << l << "] - as_e[" << l << "] = " << ((-1/b[0]) * a[l-1]) - as_e[l] << "\n";
        for (int k=0;k<K-1;k++)
            log() << "bs[" << k << "] - bs_e[" << k << "] = " << ((-1/b[0]) * b[k+1]) - bs_e[k] << "\n";
        for (int r=0;r<R;r++)
            log() << "cs[" << r << "] - cs_e[" << r << "] = " << ((-1/b[0]) * c[r]) - cs_e[r] << "\n";
		if (G==1)
            log() << "ds - ds_e = " << ((-1/b[0]) * d) - ds_e << "\n";
		if (H==1)
            log() << "es - es_e = " << ((-1/b[0]) * e) - es_e << "\n";
//		log() << "last dif_var=" << dif_var << "\n";
//		log() << "var1 = " << var1 << "\n";
//		log() << "var2 = " << var2 << "\n";
//		log() << "sum_var1 = " << sum_var1 << "\n";
	}
	
	
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		armax_eindim
// Member function:	setLabels
// Purpose:		return a pointer to a variable or a parameter specified
//
// Author:		    mhoffman
// Last modified:	Feb 27 METDST 2001
// By:				mhoffman			
//
///////////////////////////////////////////////////////////////////////////////

qreal* armax_eindim::setLabels(char* label)
{
	if( !strcmp(label,"y_t") ) return(&y_t);
    if( !strcmp(label,"u_t") ) return(&u_t);
    if( !strcmp(label,"v_t") ) return(&v_t);
    if( !strcmp(label,"dif_var") ) return(&dif_var);
    if( !strcmp(label,"dif") ) return(&dif);
    if( !strcmp(label,"eta_t") ) return(&eta_t);
    if( !strcmp(label,"eps_t") ) return(&eps_t);
    if( !strcmp(label,"ds_e") ) return(&ds_e);
    if( !strcmp(label,"es_e") ) return(&es_e);
    if( !strcmp(label,"var1") ) return(&var1);
    if( !strcmp(label,"sum_var1") ) return(&sum_var1);
    if( !strcmp(label,"as_e[0]") ) return(&as_e[0]);
    if( !strcmp(label,"norm_u") ) return(&norm_u);
    if( !strcmp(label,"norm_y") ) return(&norm_y);
    if( !strcmp(label,"sum_u") ) return(&sum_u);
    if( !strcmp(label,"sum_y") ) return(&sum_y);
    if( !strcmp(label,"sum_uy") ) return(&sum_uy);

    if( !strcmp(label,"error_as_e[0]") ) return(&error_as_e[0]);
if(L>=1)
    if( !strcmp(label,"error_as_e[1]") ) return(&error_as_e[1]);
if(L>=2)
    if( !strcmp(label,"error_as_e[2]") ) return(&error_as_e[2]);
if(L>=3)
    if( !strcmp(label,"error_as_e[3]") ) return(&error_as_e[3]);
if(L>=4)
    if( !strcmp(label,"error_as_e[4]") ) return(&error_as_e[4]);
if(K>=2)
    if( !strcmp(label,"error_bs_e[0]") ) return(&error_bs_e[0]);
if(K>=3)
    if( !strcmp(label,"error_bs_e[1]") ) return(&error_bs_e[1]);
if(K>=4)
    if( !strcmp(label,"error_bs_e[2]") ) return(&error_bs_e[2]);
if(R>=1)
    if( !strcmp(label,"error_cs_e[0]") ) return(&error_cs_e[0]);
if(R>=2)
    if( !strcmp(label,"error_cs_e[1]") ) return(&error_cs_e[1]);
if(R>=3)
    if( !strcmp(label,"error_cs_e[2]") ) return(&error_cs_e[2]);
if(R>=4)
    if( !strcmp(label,"error_cs_e[3]") ) return(&error_cs_e[3]);
if(G!=0)
    if( !strcmp(label,"error_ds_e") ) return(&error_ds_e);
if(H!=0)
    if( !strcmp(label,"error_es_e") ) return(&error_es_e);

    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:	armax_eindim	
// Member function:	saveParamset
// Purpose:		write parameterset into a file
//
// Author:		    mhoffman
// Last modified:	Feb 27 METDST 2001
// By:				mhoffman			
//
///////////////////////////////////////////////////////////////////////////////

void armax_eindim::saveParamset(ofstream& outFile)
{	
	for (int l=0;l<L;l++)
		outFile << y_old_ini[l] << "\t";
 	for (int k=0;k<K;k++)
		outFile << u_old_ini[k] << "\t";
 	for (int r=0;r<R;r++)
		outFile << v_old_ini[r] << "\t";
    for (int l=0;l<L;l++)
		outFile << a[l] << "\t";
    for (int k=0;k<K;k++)
		outFile << b[k] << "\t";
    for (int r=0;r<R;r++)
		outFile << c[r] << "\t";
	if (G==1)
		outFile << d << "\t";
	if (H==1)
		outFile << e << "\t";
    for (int l=0;l<L;l++)
		outFile << a_e[l] << "\t";
    for (int k=0;k<K;k++)
		outFile << b_e[k] << "\t";
    for (int r=0;r<R;r++)
		outFile << c_e[r] << "\t";
	if (G==1)
		outFile << d_e << "\t";
	if (H==1)
		outFile << e_e << "\t";

	outFile << eps;
	outFile << sigma_eps;		
		
    outFile << length;

}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:	armax_eindim	
// Member function:	printParamset
// Purpose:		print parameterset on the screen
//
// Author:		    mhoffman
// Last modified:	Feb 27 METDST 2001
// By:				mhoffman			
//
///////////////////////////////////////////////////////////////////////////////

void armax_eindim::printParamset()
{ 	
 	for (int l=0;l<L;l++)
        log() << y_old_ini[l] << "\n";
 	for (int k=0;k<K;k++)
        log() << u_old_ini[k] << "\n";
	for (int r=0;r<R;r++)
        log() << v_old_ini[r] << "\n";
    for (int l=0;l<L;l++)
        log() << a[l] << "\n";
    for (int k=0;k<K;k++)
        log() << b[k] << "\n";
    for (int r=0;r<R;r++)
        log() << c[r] << "\n";
	if (G==1)
        log() << d << "\n";
	if (H==1)
        log() << e << "\n";
    for (int l=0;l<L;l++)
        log() << a_e[l] << "\n";
    for (int k=0;k<K;k++)
        log() << b_e[k] << "\n";
    for (int r=0;r<R;r++)
        log() << c_e[r] << "\n";
	if (G==1)
        log() << d_e << "\n";
	if (H==1)
        log() << e_e << "\n";
 	
    log() << eps << "\n";
    log() << sigma_eps << "\n";
		
    log() << length << "\n";
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:	armax_eindim	
// Member function:	sendStateSpace
// Purpose:		return pointers to the state variables and inform about
//			the systems dimension
//
// Author:		    mhoffman
// Last modified:	Feb 27 METDST 2001
// By:				mhoffman			
//
///////////////////////////////////////////////////////////////////////////////

void armax_eindim::sendStateSpace(int &quantity,const qreal*** stateSpace)
{
    if( stateSpace )
	delete stateSpace;
    *stateSpace= new const qreal* [dimension];
    if( !(*stateSpace) )
	fatalError("armax_eindim::sendStateSpace",
		   "Can't create state space vector");
    quantity=dimension;
    (*stateSpace)[0]=&y_t;

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
qreal* armax_eindim::sendModelVar(void)
{ error("macrodyn::armax_eindim::sendModelVar is not implemented");
  return NULL;
}
void armax_eindim::sendParameters(int& ,qreal** )
{ error("macrodyn::armax_eindim::sendParameters is not implemented");
}
void armax_eindim::receiveParameters(const qreal* )
{ error("macrodyn::armax_eindim::receiveParameters is not implemented");
} 
