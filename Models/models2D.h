#ifndef MODELS2D_H
#define MODELS2D_H

//$Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/models2D.h,v 1.1 2000/08/31 16:06:55 mmeyer Exp $
/******************************************************************************/
/*                                                                            */
/* Module name:  models2D.h                                                   */
/* Contents:     Class definition of the classes models2D                     */
/*                                                                            */
/* Last Modified: 02.03.1995 (Markus Lohmann)                                 */
/* Modified:      24.02.95 -- sendParameters and receiveParameters added      */
/*                02.03.95 -- Abstract class models2D introduced              */
/*                                                                            */
/******************************************************************************/

#include "baseModel.h"

/******************************************************************************/
/*                                                                            */
/* Class name:    models2D                                                    */
/* Derived from:  baseModel                                                   */
/* Purpose:       defines an abstract class for 2D models2D                   */
/*                this class is abstract because the member function iteration*/
/*                isn't defined                                               */
/*                                                                            */
/* Last modified: 02.03.1995 (Markus Lohmann)                                 */
/*                                                                            */
/******************************************************************************/

class models2D : public baseModel {
  protected:
    real alpha;			        // parameter
    real beta;
    real x;			        // state variable
    real y;                           
    real x0;				// initial value for x
    real y0;				// initial value for y
public:
    models2D();		    	        // constructor
    virtual ~models2D()=0;
    void loadParamset(ifstream&);	
    void saveParamset(ofstream&);
    void printParamset();
    void initialize();
    real* sendModelVar();
    real* setLabels(char*);
    void sendStateSpace(int &,const real***);
    void sendParameters(int&,real**); // write all parameters
                                // into an array and return the numbers
				// of parameters
    void receiveParameters(const real*);// receive parameter values 
};
   
#endif
