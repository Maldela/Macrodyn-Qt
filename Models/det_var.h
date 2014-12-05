
///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:          det_var
// Derived from:        rSolow
// Purpose:             defines the solow with several kinds
//			of chaotic depreciation rates
//			delta_p becomes a state variable now
//
// Author:              Michael Meyer & Stefan Lueke
// Last modified:       Fri Jun 12 14:48:41 METDST 1998
// By:                  
//
///////////////////////////////////////////////////////////////////////////////

#include "rSolow.h"

class det_var : public rSolow
{

private:
    qreal _z;				// variable for the original process
    qreal _z_0;				// initial value for z
    char _z_var_name[256];		// lexical name of the stochastified parameter
    qreal * _z_var_ptr;			// pointer to the stochastified parameter

    qreal trans(qreal z, qreal*, qreal*, qreal*); // transforms z to delta_p
    qreal *trans_x;			// pointer to array classes
    qreal *trans_a;			// parameter for the transformation
    qreal *trans_b;			// parameter for the transformation

    qreal (* theta) (qreal z, qreal m);	// theta defines the basic dynamic for z
    qreal theta_type;			// the current type of the theta function
    qreal my;				// parameters for theta
    void theta_init (void);		// sets theta to a specific type
    
public:
    det_var();			// constructor
    void sendStateSpace(int &,const qreal***);
    qreal * setLabels(const QString&);		// get a variable by a name
    void initialize(void);		// initialize the model
//    void loadParamset(QDataStream&);	// load parameterset from a file
//    void saveParamset(QDataStream&);
//    void printParamset();
    void iteration(const qint64&);	// perform one iteration 
};
