#ifndef LEO_H
#define LEO_H

#include "defaultModel.h"

//$Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/leo.h,v 1.1 2000/08/31 15:58:51 mmeyer Exp $
/******************************************************************************/
/*                                                                            */
/* Module name:  leo.h                                                        */
/* Contents:     Class definition of the class leo                            */
/*               This model is derived from the defaultModel. The difference  */
/*               is that Leo Kaas has modified the dynamics.                  */
/*                                                                            */
/* Last Modified: 26.02.1991 (Marc Mueller)                                   */
/* Modified:      23.02.1991 (Marc Mueller)                                   */
/*                                                                            */
/******************************************************************************/

class leo : public defaultModel {
protected:
    real eta;				// weight parameter
    real zeta;				// weight parameter
    void leoDynamics();			// leos dynamics
public:
    leo();				// constructor
    void iteration(const long&);	// perform one iteration
    real* setLabels(char*);
    void loadParamset(ifstream&);	// load parameterset from a file
    void saveParamset(ofstream&);	// write parameterset into a file
    void printParamset();		// print parameterset on the screen
    void sendParameters(int&,real**);	// write all parameters
					// into an array and return the numbers
					// of parameters
    void receiveParameters(const real*);// receive parameter values 
};
#endif
