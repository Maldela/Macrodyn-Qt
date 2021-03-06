///////////////////////////////////////////////////////////////////////////////
//
// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/dynofin_rmv.C,v 1.1 2000/08/31 15:53:55 mmeyer Exp $
//
// Module name:		dynofin_rmv.C
//
// Author:		Michael Meyer 
// Last modified:	04/22/99
// By:			Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

#include "dynofin_rmv.h"
#include "../logger.h"


//*******************************************************************************   
//*******************************************************************************
//*******************************************************************************
//***                                                                         ***
//***									      ***
//***			   D Y N O F I N _  R M V 			      ***
//***  									      ***
//***									      ***
//*******************************************************************************
//*******************************************************************************
//*******************************************************************************

///////////////////////////////////////////////////////////////////////////////
//
//
// Functions:		dynofin_rmc.C
// Purpose:		function  
//
// Author:		Michael Meyer
// Last modified:	Thu Apr 22 14:44:08 MET 1997
// By:			Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
//
//
// Functions:		pf_gamma
// Purpose:		function : Gamma
//
// Author:		Michael Meyer
// Last modified:	Thu Apr 22 14:44:08 MET 1997
// By:			Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////
static qreal pf_gam ( qreal , qreal , qreal )
{ 
  return (0.5);  
}
///////////////////////////////////////////////////////////////////////////////
//
//
// Functions:		pf_variance
// Purpose:		function : Variance
//
// Author:		Michael Meyer
// Last modified:	Thu Apr 22 14:44:08 MET 1997
// By:			Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////
static qreal pf_var (qreal , qreal , qreal )
{ 
  return (0.5);  
}
///////////////////////////////////////////////////////////////////////////////
//
//
// Functions:		pf_expectation
// Purpose:		function : Expectation
//
// Author:		Michael Meyer
// Last modified:	Thu Apr 22 14:44:08 MET 1997
// By:			Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////
static qreal pf_exp (qreal , qreal , qreal )
{
  return (10.5);  
}
///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		dynofin_rmv
// Member function:	allokation
//
// Author:		Michael Meyer 
// Last modified: 	04/22/99
// By:			Michael Meyer & Stefan Lueke
//
///////////////////////////////////////////////////////////////////////////////

st_fin_paramset *r_fin_paramset = new st_fin_paramset;
st_fin_paramset *d_fin_paramset = new st_fin_paramset; 
      	        
///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		dynofin_rmv
// Member function:	trans
// Purpose:		transforms z into the interval [a,b]
//
// Author:		Uli Middelberg
// Last modified:	Fri Feb  12 17:15:14 METDST 1999
// By:			Michael Meyer & Stefan Lueke
//
///////////////////////////////////////////////////////////////////////////////

qreal Trans(qreal z, qreal* x, qreal* a, qreal* b)
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
// Class name:		dynofin_rmv
// Member function:	theta_xxx
// Purpose:		specific types of thetas
//
// Author:		Uli Middelberg
// Last modified:	Fri Feb 12 14:59:42 METDST 1999
// By:			Michael Meyer & Stefan Lueke
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
// Class name:		dynofin_rmc
// Member function:	pf_init
//
// Author:		Michael Meyer
// Last modified:	Thu Apr 22 12:44:16 METDST 1999
// By:			Michael Meyer 
//
////////////////////////////////////////////////////////////////////////////////
//void dynofin_rmv::pf_init ( void )
//{  
//  switch ( int(pf_type)) {
//   case 0 :
//      pf=pf_gamma; 
//    break;
//    case 1 :
//      pf=pf_variance; 
//    break;
//    case 2 :
//      pf=pf_expectation;
//    break;
//    
//    default :
//      error("macrodyn::dynofin_rmc::pf_init: pf type %d not yet implemented : ", pf_type);
//  }
//}
///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		dynofin_rmv
// Purpose:		constructor
//
// Author:		Michael Meyer 
// Last modified:	Thu Apr 22 14:09:17 MET 1999
// By:			Michael Meyer 
//
///////////////////////////////////////////////////////////////////////////////

dynofin_rmv::dynofin_rmv() : baseModel(1)
{
r_fin_paramset->mc = NULL;
r_fin_paramset->zvar = NULL;

d_fin_paramset->mc = NULL;
d_fin_paramset->zvar = NULL;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		dynofin_rmv
// Member function:	dynofin_rmv
// Purpose:		destructor
//
// Author:		Michael Meyer 
// Last modified:	Thu Apr 22 14:09:17 MET 1999
// By:			Michael Meyer 
//
///////////////////////////////////////////////////////////////////////////////

dynofin_rmv::~dynofin_rmv(void) {
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:          dynofin_rmv
// Member function:     noise_iteration
// Purpose:             perform one iteration of the system
//
// Author:              Michael Meyer 
// Last modified:       Thu Apr 22 14:13:52 MET 1999
// By:                  Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

void dynofin_rmv::noise_iteration(st_fin_paramset *temp_paramset)
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
      z_2 = Trans(z_1,temp_paramset->trans_x,temp_paramset->trans_a,temp_paramset->trans_b);
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
// Class name:          dynofin_rmv
// Member function:     iteration
// Purpose:             perform one iteration of the system
//
// Author:              Michael Meyer & Stefan Lueke
// Last modified:       Wed Feb 26 14:13:52 MET 1999
// By:                  Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

void dynofin_rmv::iteration(const qint64& )
{ 
  qreal w1_n;
  qreal w2_n;
  qreal thetaA1_n;
  qreal thetaA2_n;     
  qreal thetaB1_n;
  qreal thetaB2_n;    	
  qreal p_n;
  qreal p_count_1=0;
  qreal p_count_2=0;
  
  
  if (r_fin_paramset->type > -1) {
    noise_iteration(r_fin_paramset);
    r_fin = r_fin_paramset->z_trans;}

  if (d_fin_paramset->type > -1) {
    noise_iteration(d_fin_paramset);
    d_fin = d_fin_paramset->z_trans;}
  
  w1_n = r_fin*thetaB1+((p+d_fin)*thetaA1);
  w1 = w1_n;
  
  w2_n = r_fin*thetaB2+((p+d_fin)*thetaA2);
  w2 = w2_n;

  gam_1=(*pf_gam)(w1,d_fin,p);
  var_1=(*pf_var)(w1,d_fin,p);
  exp_1=(*pf_exp)(w1,d_fin,p);

  gam_2=(*pf_gam)(w2,d_fin,p);
  var_2=(*pf_var)(w2,d_fin,p);
  exp_2=(*pf_exp)(w2,d_fin,p);

  p_count_1+=(exp_1/(gam_1*var_1) + exp_2/(gam_2*var_2));
  p_count_2+= (1/(gam_1*var_1) + 1/(gam_2*var_2));
  p_n = (1/r_fin)*(p_count_1/p_count_2);
  p = p_n;
  
  thetaA1_t=thetaA1;
  thetaA2_t=thetaA2;
  
  thetaA1_n = (1/gam_1)*((exp_1-r_fin*p)/var_1);
  thetaA1 = thetaA1_n;
  
  thetaA2_n = (1/gam_2)*((exp_2-r_fin*p)/var_2);
  thetaA2 = thetaA2_n;  
  
  thetaB1_n = r_fin*thetaB1+d_fin*thetaA1-p*(thetaA1-thetaA1_t); 
  thetaB1 = thetaB1_n;
  
  thetaB2_n = r_fin*thetaB2+d_fin*thetaA2-p*(thetaA2-thetaA2_t);
  thetaB2 = thetaB2_n;
  
}  
      
///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		dynofin_rmv 
// Member function:	initialize 
// Purpose:		initialize the model, define the systems initial state
//
// Author:		Michael Meyer 
// Last modified:	Mon Mar 10 15:43:17 MET 1999
// By:			Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

void dynofin_rmv::delete_st_fin_paramset_stochptr(st_fin_paramset *temp_paramset){
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

void dynofin_rmv::initialize()
{

w1=w1_0;
w2=w2_0;
thetaA1=thetaA1_0;
thetaA2=thetaA2_0;
thetaB1=thetaB1_0;
thetaB1=thetaB1_0;
p=p_0;
//gam=gam_0;
//var=var_0;
//expe=expe_0;
 
delete_st_fin_paramset_stochptr(r_fin_paramset);
delete_st_fin_paramset_stochptr(d_fin_paramset);

r_fin_paramset->z_0 = r_fin_0;
d_fin_paramset->z_0 = d_fin_0;

}	

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		dynofin_rmv
// Member function:	setLabels
// Purpose:		return a pointer to a variable or a parameter specified
//
// Author:		Uli Middelberg
// Last modified:	99/04/22
// By:			Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

qreal* dynofin_rmv::setLabels(const QString& label)
{
    if (label == "w1") return(&w1);
    if (label == "w2") return(&w2);
    if (label == "thetaA1") return(&thetaA1);
    if (label == "thetaA2") return(&thetaA2);
    if (label == "thetaB1") return(&thetaB1);
    if (label == "thetaB2") return(&thetaB2);
    if (label == "p") return(&p);
    if (label == "gam_1") return(&gam_1);
    if (label == "var_1") return(&var_1);
    if (label == "exp_1") return(&exp_1);
    if (label == "gam_2") return(&gam_2);
    if (label == "var_2") return(&var_2);
    if (label == "exp_2") return(&exp_2);
    if (label == "r_fin") return(&r_fin);
    if (label == "d_fin") return(&d_fin);
   
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		dynofin_rmv
// Member function:	sendStateSpace
// Purpose:		return pointers to the state variables and inform about
//			the systems dimension
//
// Author:		Uli Middelberg
// Last modified:	Thu Apr 22 15:43:28 MET 1997
// By:			Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

void dynofin_rmv::sendStateSpace(int &quantity,QList<qreal *> *stateSpace)
{
    stateSpace->clear();
    quantity = dimension;
    *stateSpace << &w1;                                
}  

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		dynofin_rmv
// Member function:	load x_paramset
// Purpose:		
//
// Author:		Michael Meyer & Stefan Lueke
// Last modified:	Mon May 11 13:07:57 METDST 1998
// By:			
//
///////////////////////////////////////////////////////////////////////////////

void dynofin_rmv::read_sim(QTextStream& inFile, st_fin_paramset *temp_paramset){

  int	i;			// Index

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
        while(i<Trans_X_Max) {
	      inFile >> temp_paramset->trans_x[i];
	      //printf("x[%i]=%f\n",i,trans_x[i]); 
	  if(temp_paramset->trans_x[i]==1) break;
	  i++;
	}
	//printf("i=%i\n",i); 
 	
        if(i==Trans_X_Max)
	  error("macrodyn::dynofin_rmv",
          "the number i of x[i] must be less Trans_X_Max");

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
	        
           n_states = in_string.count(';');
           temp_paramset->zvar_expr = in_string;

          	for (int n = 0; n < n_states; n++) {
            	 inFile >> in_string;	
	    	 	// compose the matrix from
		 //temp_paramset->mc_matrix[0] = '\0';	
             temp_paramset->mc_matrix += in_string;
	    	   // the input file with blank at the
             temp_paramset->mc_matrix += " ";
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
        error("macrodyn::dynofin_rmv",
	      "error in definition of stochastic parameter types");
    }
  }
   


///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		dynofin
// Member function:	loadParamset
// Purpose:		load a parameterset from a specified input file
//
// Author:		Michael Meyer 
// Last modified:	Mon May 11 13:07:57 METDST 1998
// By:			
//
///////////////////////////////////////////////////////////////////////////////

void dynofin_rmv::loadParamset(QTextStream& inFile){          
    
    inFile >> w1_0;    
    inFile >> w2_0; 	         	  
    inFile >> p_0;
    inFile >> thetaA1_0; 
    inFile >> thetaA2_0;
    inFile >> thetaB1_0;	  
    inFile >> thetaB2_0;

    read_sim(inFile,r_fin_paramset);
 
    read_sim(inFile,d_fin_paramset);
 
    r_fin_0 = r_fin_paramset->z_0;
    d_fin_0 = d_fin_paramset->z_0;

    inFile >> length;         	    
    
    initialize();       
}
///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		dynofin_rmv
// Member functions:	sendModelVar, sendParameters, receiveParameters
// Purpose:		for compatibity reasons only
//
// Author:		Uli Middelberg
// Last modified:	Tue Mar 11 16:10:38 MET 1997
// By:			Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

qreal* dynofin_rmv::sendModelVar(void)
{ error("macrodyn::dynofin_rmv::sendModelVar is not implemented");
  return NULL;
}
void dynofin_rmv::sendParameters(int& ,qreal** )
{ error("macrodyn::dynofin_rmv::sendParameters is not implemented");
}
void dynofin_rmv::receiveParameters(const QList<qreal>&)
{ error("macrodyn::dynofin_rmv::receiveParameters is not implemented");
}  
///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		dynofin_rmv
// Member function:	save_paramset
// Purpose:		write parameterset into a file
//
// Author:		Michael Meyer 
// Last modified:	Mon Mar 11 13:11:41 METDST 1999
// By:			Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

void dynofin_rmv::save_st_fin_paramset(QTextStream& outFile, st_fin_paramset *temp_paramset){

   int	i;			// Index
  
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
        while(i<Trans_X_Max) {
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
    }

 }

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		dynofin_rmv
// Member function:	saveParamset
// Purpose:		save a parameterset from a specified input file
//
// Author:		Michael Meyer 
// Last modified:	Thu Apr 22 13:07:57 METDST 1999
// By:			Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

void dynofin_rmv::saveParamset(QTextStream& outFile)

{     
    outFile << w1_0;
    outFile << w2_0;
    outFile << gam_0;
    outFile << var_0;
    outFile << expe_0;
    outFile << p_0;
    outFile << thetaA1_0;
    outFile << thetaA2_0;
    outFile << thetaB1_0;
    outFile << thetaB2_0;
            	
    save_st_fin_paramset(outFile,r_fin_paramset);
 
    save_st_fin_paramset(outFile,d_fin_paramset);
 
 
    outFile << length;
    
    baseModel::saveParamset(outFile);
}            	    
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// Class name:      dynofin_rmv                                                   //
// Member function: saveParamsetWithNames                                     //
// Purpose:         add  parameterset to printfile                            //
// Last modified:                                                             //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
void dynofin_rmv::saveParamsetWithNames(QTextStream& outFile)	
{
outFile << "dynofin_rmv:";
outFile << "w1_0 =" << w1_0 << "\tw2_0 =" << w2_0;
outFile << "gamma_0 =" << gam_0 << "\tvar_0 =" << var_0;
outFile << "expectation_0 =" << expe_0 << "\tp_0 =" << p_0;
outFile << "thetaA1_0 =" << thetaA1_0 << "\tthetaA2_0 =" << thetaA2_0;
outFile << "thetaB1_0 =" << thetaB1_0 << "\tthetaB2_0 =" << thetaB2_0;
outFile << "r_fin Parameterset" << "\t";
save_st_fin_paramsetWithNames(outFile,r_fin_paramset);  
outFile << "d_fin Parameterset" << "\t";
save_st_fin_paramsetWithNames(outFile,d_fin_paramset);

outFile << "Length = " << length;
}
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// Class name:      dynofin_rmv                                                    //
// Member function: save_st_ParamsetWithNames                                 //
// Purpose:         add  parameterset to printfile                            //
// Last modified:                                                             //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
void dynofin_rmv::save_st_fin_paramsetWithNames(QTextStream& outFile, st_fin_paramset *temp_paramset)	
{
   int	i;			// Index
  	
    	outFile << "type = " << temp_paramset->type << "\t";
	
    switch  (temp_paramset->type) {
      case -1:
 	 outFile << "z_0 = " << temp_paramset->z_0 << "\t";  
      break;
      case 0 :	
    	 outFile << "z_0 = " << temp_paramset->z_0 << "\t";	 
      break;
	
      case 1 :		
	outFile <<  "theta_type = " << temp_paramset->theta_type << "\t";
	outFile <<  "my = " << temp_paramset->my << "\t";
    	outFile <<  "z_0 = " << temp_paramset->z_0 << "\t"; 
	
        i=0;
        while(i<Trans_X_Max) {
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
	
    }
}
///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		dynofin_rmv
// Member function:	printParamset
// Purpose:		print paramset on the screen
//
// Author:		Michael Meyer 
// Last modified:	Mon May 11 13:07:57 METDST 1998
// By:			
//
///////////////////////////////////////////////////////////////////////////////
void dynofin_rmv::print_st_fin_paramset(st_fin_paramset *temp_paramset){

    int	i;			// Index
      	
      log() << temp_paramset->type;
      
      switch  (temp_paramset->type) {
      case -1:
        log() << temp_paramset->z_0;
      break;
      case 0 :	
        log() << temp_paramset->z_0;
      break;
	
      case 1 :		
        log() << temp_paramset->theta_type;
        log() << temp_paramset->my;
        log() << temp_paramset->z_0;
	
        i=0;
        while(i<Trans_X_Max) {
          log() << temp_paramset->trans_x[i] ;
	      //printf("x[%i]=%f\n",i,trans_x[i]); 
	      if(temp_paramset->trans_x[i]==1) break;
	      i++;
	}
       for (int j=0;j<i;j++) {
     log() << temp_paramset->trans_a[j];
	}
       for (int j=0;j<i;j++) {
     log() << temp_paramset->trans_b[j];
	}
        break;
   
      case 2:
        log() << temp_paramset->zvar_expr;
	  if( temp_paramset->mc_flag == 1) {
         log() << temp_paramset->mc_matrix;
          }
       break;
    }

 }

  ///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		dynofin_rmv
// Member function:	saveParamset
// Purpose:		save a parameterset from a specified input file
//
// Author:		Michael Meyer 
// Last modified:	Mon May 11 13:07:57 METDST 1998
// By:			Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

void dynofin_rmv::printParamset()

{     
    log() << w1_0;
    log() << w2_0;
    log() << gam_0;
    log() << var_0;
    log() << expe_0;
    log() << p_0;
    log() << thetaA1_0;
    log() << thetaA2_0;
    log() << thetaB1_0;
    log() << thetaB2_0;
    
    print_st_fin_paramset(r_fin_paramset);
 
    print_st_fin_paramset(d_fin_paramset);
 
    log() << length;
             
    baseModel::printParamset();
}   
  
