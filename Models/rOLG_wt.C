///////////////////////////////////////////////////////////////////////////////
//
// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/rOLG_wt.C,v 1.8 2000/11/09 14:15:18 mhoffman Exp $
//
// Module name:		rOLG_wt.C
//
// Author:		Michael Meyer 
// Last modified:	08/03/99
// By:			Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

#include "rOLG_wt.h"
#include "../error.h"
#include "../strnchr.h"

//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
//****                                                                     ****
//****                        r O L G _ w t                                ****
//****                                                                     ****
//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
//*****************************************************************************

st_olg_paramset *z_st_paramset = new st_olg_paramset;

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		rOLG_wt
// Member function:	trans
// Purpose:		transforms z into the interval [a,b]
//
// Author:		Uli Middelberg
// Last modified:	Fri Feb  12 17:15:14 METDST 1999
// By:			Michael Meyer 
//
///////////////////////////////////////////////////////////////////////////////

qreal TRANS(qreal z, qreal* x, qreal* a, qreal* b)
{
    int i=0;
    while(x[i]<1) {
	if(  ( (z>=x[i]) && (z<x[i+1]) ) || (x[i+1]==1)  )
		return( a[i] + (z-x[i])*(b[i]-a[i])/(x[i+1]-x[i]) );
	i++;
	}
		
    return(-1);    
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		rOLG_wt
// Member function:	theta_xxx
// Purpose:		specific types of thetas
//
// Author:		Uli Middelberg
// Last modified:	Fri Feb 12 14:59:42 METDST 1999
// By:			Michael Meyer 
//
///////////////////////////////////////////////////////////////////////////////

static qreal theta_logistic ( qreal d, qreal m)
{ return (m * d * (1-d)) ;
}

static qreal theta_tent ( qreal d, qreal m)
{ 
  
  if( d<=m ) {
    return (1/m * d);
  } else {
    return (1/(1-m)*(1-d));
  }
}

static qreal theta_saw ( qreal d, qreal m)
{ if( d<m ) {
    return (1/m * d);
  } else {
    return (1/(1-m)*(d-m));
  }
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		rOLG_wt
// Member function:	rOLG_wt
// Purpose:		constructor
//
// Author:		Michael Meyer 
// Last modified:	Tue Aug 03 14:09:17 MET 1999
// By:			Michael Meyer 
//
///////////////////////////////////////////////////////////////////////////////

rOLG_wt::rOLG_wt() : baseModel(1)
{
z_st_paramset->mc = NULL;
z_st_paramset->zvar = NULL;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		rOLG_wt
// Member function:	rOLG_wt
// Purpose:		destructor
//
// Author:		Michael Meyer
// Last modified:	Tue Aug 03 14:09:17 MET 1999
// By:			Michael Meyer 
//
///////////////////////////////////////////////////////////////////////////////

rOLG_wt::~rOLG_wt(void) 
{
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:          rOLG_wt
// Member function:     noise_iteration
// Purpose:             perform one iteration of the system
//
// Author:              Michael Meyer 
// Last modified:       Tue Aug 03 14:09:17 MET 1999
// By:                  Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

void rOLG_wt::noise_iteration(st_olg_paramset *temp_paramset)
{ 

qreal z_1,z_2;

z_1 = 0;
z_2 = 0;

switch  (temp_paramset->type) {
  case -1 :
  break;  

  case 0 :
    z_1 = temp_paramset->z_0;    
    temp_paramset->z_trans = z_1;
    temp_paramset->z_0 = z_1;
  break;  
  
  case 1 :
    switch  (temp_paramset->theta_type) {
      case 0 :	
    	z_1 = theta_logistic (temp_paramset->z_0, temp_paramset->my);    
      break;
      case 1 :		
    	z_1 = theta_tent (temp_paramset->z_0, temp_paramset->my);
      break;
      case 2 :		
    	z_1 = theta_saw (temp_paramset->z_0, temp_paramset->my);
      break;
      
      default:
        error("requested theta_type not available");
      }   
      z_2 = TRANS(z_1,temp_paramset->trans_x,temp_paramset->trans_a,temp_paramset->trans_b);
      temp_paramset->z_trans = z_2;
      temp_paramset->z_0 = z_1;
  break;
  
  case 2 :
    switch  (temp_paramset->mc_flag) {
      case 0 :	
    	z_1 = temp_paramset->zvar->dice(); 
      break;
      case 1 :		
    	z_1 = temp_paramset->mc->dice();
      break;
      
      default:
        error("requested theta_type not available");
      } 
     temp_paramset->z_trans = z_1;
     temp_paramset->z_0 = z_1;
  break;   
  
  
  default:
        error("requested type not available");
  }
  
}
///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:          rOLG_wt
// Member function:     iteration
// Purpose:             perform one iteration of the system
//
// Author:              Michael Meyer 
// Last modified:       Tue Aug 03 14:09:17 MET 1999
// By:                  Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

void rOLG_wt::iteration(const long& )
{ 
  qreal K_olg_n;	   // value of k for the next period
  qreal K_olg_count;
  qreal Y_olg_count;
  qreal W_olg_count;
  qreal M_olg_count;
  qreal r_count;
  qreal av_Ez_n;
  qreal K_olg_V_n;
  qreal Y_olg_V_n;
  qreal W_olg_V_n;
  qreal M_olg_V_n;
  qreal r_V_n;

  if (z_st_paramset->type > -1) {
    noise_iteration(z_st_paramset);
    z_st = z_st_paramset->z_trans;
    }
  av_Ez_n = z_st/T + (T-1)/T*av_Ez;
  av_Ez = av_Ez_n;

  m_olg = A_olg*z_st*(log(1+K_olg)-(K_olg/(1+K_olg)))+(alpha_olg*K_olg/(1-alpha_olg));
  a_m = 1+n_olg+m_olg;
  b_m = (1+n_olg)*(1-A_olg*Ez)-(1-delta_olg)*a_m;
  c_olg = A_olg*Ez+1-delta_olg;

  R_olg = (-b_m+sqrt((b_m*b_m)+(4*a_m*c_olg)))/(2*a_m);
  K_olg_n = R_olg/(1+R_olg)*m_olg*((1-alpha_olg)/(1+n_olg));
  K_olg = K_olg_n;   

  Y_olg =  A_olg*z_st*log(1+K_olg); 
  W_olg = Y_olg - A_olg*z_st*(K_olg/(1+K_olg));
  M_olg = W_olg + (alpha_olg*K_olg/(1-alpha_olg));   

  r=(A_olg*z_st)/(1+K_olg);
  S_olg=((1+n_olg)*K_olg)/(1-alpha_olg);
  c1=M_olg-S_olg;
  c2=(1-delta_olg+r)*(1-alpha_olg)*S_olg;  

qreal x2;
ran_rec = rand_dis->rectangular();


//Log::log() << "a"<<ran_rec<<"b"<<endl;
ran_norm = rand_dis->st_normal();
ran_chi = rand_dis->chi_square(dof);
ran_cauchy = rand_dis->st_cauchy();
ran_student_t = rand_dis->student_t(dof);
ran_exp = rand_dis->exponential(b_exponential);
ran_pareto = rand_dis->pareto(a_pareto,c_pareto);


  if (T <= 1000)
  {
  	K_olg_count = K_olg_n/T + (T-1)/T*K_olg_E;
  	K_olg_E = K_olg_count;
  	Y_olg_count = Y_olg/T + (T-1)/T*Y_olg_E;
  	Y_olg_E = Y_olg_count;
  	W_olg_count = W_olg/T + (T-1)/T*W_olg_E;
  	W_olg_E = W_olg_count;
  	M_olg_count = M_olg/T + (T-1)/T*M_olg_E;
  	M_olg_E = M_olg_count;
  	r_count = r/T + (T-1)/T*r_E;
  	r_E = r_count;
  }  
  if(T > 1000)
  {  
 	K_olg_V_n = (K_olg_n-K_olg_E)*(K_olg_n-K_olg_E)/TT + (TT-1)/TT*K_olg_V;
 	K_olg_V = K_olg_V_n;  
 	K_olg_S = sqrt(K_olg_V);
 	Y_olg_V_n = (Y_olg-Y_olg_E)*(Y_olg-Y_olg_E)/TT + (TT-1)/TT*Y_olg_V;
 	Y_olg_V = Y_olg_V_n;  
  	Y_olg_S = sqrt(Y_olg_V);
	W_olg_V_n = (W_olg-W_olg_E)*(W_olg-W_olg_E)/TT + (TT-1)/TT*W_olg_V;
 	W_olg_V = W_olg_V_n;  
 	W_olg_S = sqrt(W_olg_V);
	M_olg_V_n = (M_olg-M_olg_E)*(M_olg-M_olg_E)/TT + (TT-1)/TT*M_olg_V;
	M_olg_V = M_olg_V_n;  
 	M_olg_S = sqrt(M_olg_V);
	r_V_n = (r-r_E)*(r-r_E)/TT + (TT-1)/TT*r_V;
 	r_V = r_V_n;  
	r_S = sqrt(r_V);
	TT+=1;
  }
  T += 1; 
}    

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		rOLG_w
// Member function:	initialize 
// Purpose:		initialize the model, define the systems initial state
//
// Author:		Michael Meyer 
// Last modified:	Tue Aug 03 14:09:17 MET 1999
// By:			Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

void rOLG_wt::delete_st_olg_paramset_stochptr(st_olg_paramset *temp_paramset){
if (temp_paramset->mc_flag){
	if (temp_paramset->mc != NULL) {
		   delete temp_paramset->mc;
		   }
	temp_paramset->mc = new markov_chain (this, "ranf", temp_paramset->zvar_expr,temp_paramset->mc_matrix);
	
	}else {
	if( temp_paramset->zvar != NULL){
	       	   delete temp_paramset->zvar;
		}
	temp_paramset->zvar = new rand_var (this, "ranf", temp_paramset->zvar_expr);
}
}

void rOLG_wt::initialize()
{
K_olg=K_olg_0;

ran_rec=0;
ran_norm=0;
ran_chi=0;
ran_cauchy=0;
ran_student_t=0;
ran_exp=0;
ran_pareto=0;

z_st_n=0;
T = 1;
TT = 1;

av_Ez=0;
K_olg_E=0;
Y_olg_E=0;
M_olg_E=0;
W_olg_E=0;
r_E=0;

K_olg_V=0;
Y_olg_V=0;
W_olg_V=0;
M_olg_V=0;
r_V=0;

delete_st_olg_paramset_stochptr(z_st_paramset);
z_st_paramset->z_0 = z_st_0;

if ( rand_dis != NULL ) delete rand_dis;
	rand_dis = new distribution();
			if( !(rand_dis) )
				fatalError("rOlg:","can't create rand_dis");
}	

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		rOLG_wt
// Member function:	setLabels
// Purpose:		return a pointer to a variable or a parameter specified
//
// Author:		Uli Middelberg
// Last modified:	99/08/03
// By:			Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

qreal* rOLG_wt::setLabels(char* label)
{
    if( !strcmp(label,"K_olg") ) return(&K_olg);
    if( !strcmp(label,"K_olg_0") ) return(&K_olg_0);    
    if( !strcmp(label,"A_olg") ) return(&A_olg);    
    if( !strcmp(label,"alpha_olg") ) return(&alpha_olg);
    if( !strcmp(label,"a_m") ) return(&a_m);
    if( !strcmp(label,"b_m") ) return(&b_m);
    if( !strcmp(label,"c_olg") ) return(&c_olg);
    if( !strcmp(label,"n_olg") ) return(&n_olg);
    if( !strcmp(label,"delta_olg") ) return(&delta_olg);
    if( !strcmp(label,"R_olg") ) return(&R_olg);
    if( !strcmp(label,"m_olg") ) return(&m_olg);
    if( !strcmp(label,"z_st") ) return(&z_st);
    if( !strcmp(label,"av_Ez") ) return(&av_Ez);
    if( !strcmp(label,"Ez") ) return(&Ez);
    if( !strcmp(label,"Y_olg") ) return(&Y_olg);
    if( !strcmp(label,"W_olg") ) return(&W_olg);
    if( !strcmp(label,"M_olg") ) return(&M_olg);
    if( !strcmp(label,"r") ) return(&r);
    if( !strcmp(label,"S_olg") ) return(&S_olg);
    if( !strcmp(label,"c1") ) return(&c1);
    if( !strcmp(label,"c2") ) return(&c2);
    if( !strcmp(label,"K_olg_V") ) return(&K_olg_V);
    if( !strcmp(label,"K_olg_E") ) return(&K_olg_E);
    if( !strcmp(label,"K_olg_S") ) return(&K_olg_S);
    if( !strcmp(label,"W_olg_V") ) return(&W_olg_V);
    if( !strcmp(label,"W_olg_E") ) return(&W_olg_E);
    if( !strcmp(label,"W_olg_S") ) return(&W_olg_S);
    if( !strcmp(label,"ran_rec") ) return(&ran_rec);
    if( !strcmp(label,"ran_norm") ) return(&ran_norm);
    if( !strcmp(label,"ran_chi") ) return(&ran_chi);
    if( !strcmp(label,"ran_cauchy") ) return(&ran_cauchy);
	if( !strcmp(label,"ran_student_t") ) return(&ran_student_t);
	if( !strcmp(label,"ran_exp") ) return(&ran_exp);
	if( !strcmp(label,"ran_pareto") ) return(&ran_pareto);

 return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		rOLG_wt
// Member function:	sendStateSpace
// Purpose:		return pointers to the state variables and inform about
//			the systems dimension
//
// Author:		Uli Middelberg
// Last modified:	Tue Aug 03 14:09:17 MET 1999
// By:			Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

void rOLG_wt::sendStateSpace(int &quantity,const qreal*** stateSpace)
{
    if( stateSpace )
	delete stateSpace;
    *stateSpace= new const qreal* [dimension];
    if( !(*stateSpace) )
	error("rOLG_wt::sendStateSpace",
		   "Can't create state space vector");
    quantity=dimension;
    (*stateSpace)[0]=&K_olg;                                
};

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		rOLG_w
// Member function:	load x_paramset
// Purpose:		
//
// Author:		Michael Meyer 
// Last modified:	Tue Aug 03 14:09:17 MET 1999
// By:			Michael Meyer 
//
///////////////////////////////////////////////////////////////////////////////

void rOLG_wt::read_sim(ifstream& inFile, st_olg_paramset *temp_paramset){
   
  int	i,j;			// Index

  	inFile >> temp_paramset->type;

  
	
    switch  (temp_paramset->type) {

      case -1 :	
    	inFile >> temp_paramset->z_0;	
	break;
	
      case 0 :	
    	inFile >> temp_paramset->z_0;	 
        break;

      case 1 :		
  	inFile >> temp_paramset->theta_type;
	inFile >> temp_paramset->my;
    	inFile >> temp_paramset->z_0; 
      
        i=0;
        while(i<trans_x_MAX) {
	      inFile >> temp_paramset->trans_x[i];
	      //printf("x[%i]=%f\n",i,trans_x[i]); 
	  if(temp_paramset->trans_x[i]==1) break;
	  i++;
	}
	//printf("i=%i\n",i); 
 	
        if(i==trans_x_MAX)
	  error("macrodyn::rOLG_wt",
	      "the number i of x[i] must be less trans_x_Max");

        // length of trans_a and trans_b = trans_x - 1
        for (int j=0;j<i;j++) {
	 inFile >> temp_paramset->trans_a[j];
	//printf("a[%j]=%f\n",i,trans_a[j]); 
 	}
        for (int j=0;j<i;j++) {
	 inFile >> temp_paramset->trans_b[j];
	//printf("b[%j]=%f\n",i,trans_b[j]); 
	}
	//printf("end trans\n"); 
      break;
      
      case 2:		
 	  inFile >> in_string;

          if( in_string[0] == '{' ) {		
	     // we have a definition of a markov chain
            temp_paramset->mc_flag = 1; 
	        
           n_states = strnchr(in_string, ';');
           temp_paramset->zvar_expr = in_string;

          	for (int n = 0; n < n_states; n++) {
            	 inFile >> in_string;	
	    	 	// compose the matrix from
		 //temp_paramset->mc_matrix[0] = '\0';	
           	 strcat(temp_paramset->mc_matrix, in_string);	
	    	   // the input file with blank at the
           	 strcat(temp_paramset->mc_matrix, " ");		
            	  // end of each row
	 	}  
		
		temp_paramset->mc = NULL;
	        temp_paramset->mc = new markov_chain (this, "ranf", temp_paramset->zvar_expr,temp_paramset->mc_matrix);
         } else {
         temp_paramset->mc_flag = 0;
	 temp_paramset->zvar_expr = in_string;
	
	 temp_paramset->zvar = NULL;
	 temp_paramset->zvar = new rand_var (this, "ranf", temp_paramset->zvar_expr);
	 }
	break;
		    	
      default :
        error("macrodyn::rOLG_wt",
	      "error in definition of stochastic parameter types");
    }
  }
   
///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		rOLG_wt
// Member function:	loadParamset
// Purpose:		load a parameterset from a specified input file
//
// Author:		Michael Meyer 
// Last modified:	Tue Aug 03 14:09:17 MET 1999
// By:			Michael Meyer 
//
///////////////////////////////////////////////////////////////////////////////

void rOLG_wt::loadParamset(ifstream& inFile)
{  
	inFile >> dof;
	inFile >> b_exponential;
	inFile >> a_pareto;
	inFile >> c_pareto;
	            	
    inFile >> K_olg_0 ;
    inFile >> Ez;       	    
    inFile >> A_olg;
    inFile >> delta_olg;
    inFile >> n_olg;
    inFile >> alpha_olg;
    read_sim(inFile,z_st_paramset);
    z_st_0 = z_st_paramset->z_0;

    inFile >> length;         	    
    initialize();       
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		rOLG_wt
// Member functions:	sendModelVar, sendParameters, receiveParameters
// Purpose:		for compatibity reasons only
//
// Author:		Uli Middelberg
// Last modified:	Tue Aug 03 14:09:17 MET 1999
// By:			Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

qreal* rOLG_wt::sendModelVar(void)
{ error("macrodyn::rOLG_wt::sendModelVar is not implemented");
  return NULL;
}
void rOLG_wt::sendParameters(int& ,qreal** )
{ error("macrodyn::rOLG_wt::sendParameters is not implemented");
}
void rOLG_wt::receiveParameters(const qreal* )
{ error("macrodyn::rOLG_wt::receiveParameters is not implemented");
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		rOLG_wt
// Member function:	save_paramset
// Purpose:		write parameterset into a file
//
// Author:		Michael Meyer 
// Last modified:	Tue Aug 03 14:09:17 MET 1999
// By:			Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

void rOLG_wt::save_st_olg_Paramset(ofstream& outFile, st_olg_paramset *temp_paramset){

   int	i,j;			// Index
  
    	outFile << temp_paramset->type << "\t";
	
    switch  (temp_paramset->type) {
      case -1:
 	outFile << temp_paramset->z_0 << "\t";  
      break;
      case 0 :	
    	outFile << temp_paramset->z_0 << "\t";	 
      break;
	
      case 1 :		
	outFile << temp_paramset->theta_type << "\t";
	outFile << temp_paramset->my << "\t";
    	outFile << temp_paramset->z_0 << "\t"; 
	
        i=0;
        while(i<trans_x_MAX) {
	      outFile << temp_paramset->trans_x[i] << "\t";
	      //printf("x[%i]=%f\n",i,trans_x[i]); 
	      if(temp_paramset->trans_x[i]==1) break;
	      i++;
	}
       outFile << "\t";
       for (int j=0;j<i;j++) {
	 outFile << temp_paramset->trans_a[j] << "\t";
	}
	outFile << "\t";
       for (int j=0;j<i;j++) {
	 outFile << temp_paramset->trans_b[j] << "\t";
	}
        outFile << "\t";
        break;
   
      case 2:
        outFile << temp_paramset->zvar_expr << "\t";
	  if( temp_paramset->mc_flag == 1) {
	     outFile << temp_paramset->mc_matrix << "\t";
          }
       break;
	
    outFile << "\n";  
    }

 }

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		rOLG_wt
// Member function:	saveParamset
// Purpose:		save a parameterset from a specified input file
//
// Author:		Michael Meyer 
// Last modified:	Tue Aug 03 14:09:17 MET 1999
// By:			Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

void rOLG_wt::saveParamset(ofstream& outFile)

{     
    outFile << K_olg_0 << "\n" ;				
    outFile << Ez_0 << "\n";			
    save_st_olg_Paramset(outFile,z_st_paramset);			
    outFile << A_olg << "\n";
    outFile << delta_olg << "\n" ;				
    outFile << n_olg << "\n";
    outFile << alpha_olg << "\n" ;				
    outFile << length << "\n";    
    outFile << "\n";  
    baseModel::saveParamset (outFile);
}            	    
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// Class name:      rOLG_wt                                                    //
// Member function: saveParamsetWithNames                                     //
// Purpose:         add  parameterset to printfile                            //
// Last modified:   Tue Aug 03 14:09:17 MET 1999
// By:		    Michael Meyer                                                          //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
void rOLG_wt::saveParamsetWithNames(ofstream& outFile)	
{
outFile << "rOLG_wt:\n";
outFile << "K_olg_0 =" << K_olg_0 << "\t";
outFile << "Ez_0 =" << Ez_0 << "\t";
outFile << "z Parameterset" << "\t";
save_st_olg_ParamsetWithNames(outFile,z_st_paramset);  
outFile << "A_olg =" << A_olg << "\t";
outFile << "delta_olg =" << delta_olg << "\t";
outFile << "n_olg =" << n_olg << "\t";
outFile << "alpha_olg =" << alpha_olg << "\t";
outFile << "Length = " << length << "\n"; 
}
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// Class name:      rOLG_wt                                                     //
// Member function: save_st_olg_ParamsetWithNames                                 //
// Purpose:         add  parameterset to printfile                            //
// Last modified:   Tue Aug 03 14:09:17 MET 1999
// By:		    Michael Meyer                                                           //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
void rOLG_wt::save_st_olg_ParamsetWithNames(ofstream& outFile, st_olg_paramset *temp_paramset)	
{
   int	i,j;			// Index
  	
    	outFile << "type = " << temp_paramset->type << "\t";
	
    switch  (temp_paramset->type) {
      case -1:
 	 outFile << "z_st_0 = " << temp_paramset->z_0 << "\t";  
      break;
      case 0 :	
    	 outFile << "z_st_0 = " << temp_paramset->z_0 << "\t";	 
      break;
	
      case 1 :		
	outFile <<  "theta_type = " << temp_paramset->theta_type << "\t";
	outFile <<  "my = " << temp_paramset->my << "\t";
    	outFile <<  "z_st_0 = " << temp_paramset->z_0 << "\t"; 
	
        i=0;
        while(i<trans_x_MAX) {
	      outFile << "trans_x = " << temp_paramset->trans_x[i] << "\t";
	      //printf("x[%i]=%f\n",i,trans_x[i]); 
	      if(temp_paramset->trans_x[i]==1) break;
	      i++;
	}
       outFile << "\t";
       for (int j=0;j<i;j++) {
	 outFile << "trans_a = " << temp_paramset->trans_a[j] << "\t";
	}
	outFile << "\t";
       for (int j=0;j<i;j++) {
	 outFile << "trans_b = " << temp_paramset->trans_b[j] << "\t";
	}
        outFile << "\t";
        break;
   
      case 2:
        outFile << "zvar_expr = " << temp_paramset->zvar_expr << "\t";
	  if( temp_paramset->mc_flag == 1) {
	     outFile << "mc_matrix = " << temp_paramset->mc_matrix << "\t";
          }
       break;
	
    outFile << "\n";  
    }
  outFile << "\n";
}
///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		rOLG_wt
// Member function:	printParamset
// Purpose:		print paramset on the screen
//
// Author:		Michael Meyer 
// Last modified:	Tue Aug 03 14:09:17 MET 1999
// By:			Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////
void rOLG_wt::print_st_olg_Paramset(st_olg_paramset *temp_paramset){

    int	i,j;			// Index
      	
      Log::log() << temp_paramset->type << "\n";
      
      switch  (temp_paramset->type) {
      case -1:
    Log::log() << temp_paramset->z_0 << "\n";
      break;
      case 0 :	
        Log::log() << temp_paramset->z_0 << "\n";
      break;
	
      case 1 :		
    Log::log() << temp_paramset->theta_type << "\n";
    Log::log() << temp_paramset->my << "\n";
        Log::log() << temp_paramset->z_0 << "\n";
	
        i=0;
        while(i<trans_x_MAX) {
          Log::log() << temp_paramset->trans_x[i] << "\n";
	      //printf("x[%i]=%f\n",i,trans_x[i]); 
	      if(temp_paramset->trans_x[i]==1) break;
	      i++;
	}
       Log::log() << "\n";
       for (int j=0;j<i;j++) {
     Log::log() << temp_paramset->trans_a[j] << "\n";
	}
    Log::log() << "\n";
       for (int j=0;j<i;j++) {
     Log::log() << temp_paramset->trans_b[j] << "\n";
	}
        Log::log() << "\n";
        break;
   
      case 2:
        Log::log() << temp_paramset->zvar_expr << "\n";
	  if( temp_paramset->mc_flag == 1) {
         Log::log() << temp_paramset->mc_matrix << "\n";
          }
       break;
	
    Log::log() << "\n";
    }

 }

  ///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		rOLG_wt
// Member function:	printParamset
// Purpose:		save a parameterset from a specified input file
//
// Author:		Michael Meyer & Stefan Lueke
// Last modified:	Mon May 11 13:07:57 METDST 1998
// By:			Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

void rOLG_wt::printParamset()
{     
    Log::log() << K_olg_0 << "\n" ;
    Log::log() << Ez_0 << "\n";
    print_st_olg_Paramset(z_st_paramset);			
    Log::log() << A_olg << "\n";
    Log::log() << delta_olg << "\n" ;
    Log::log() << n_olg << "\n";
    Log::log() << alpha_olg << "\n";
    Log::log() << length << "\n";
    Log::log() << "\n";
    baseModel::printParamset();
} 
