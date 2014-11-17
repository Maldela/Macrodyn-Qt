
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

 		

class det_var : public rSolow {

private:
    real _z;				// variable for the original process
    real _z_0;				// initial value for z
    char _z_var_name[256];		// lexical name of the stochastified parameter
    real * _z_var_ptr;			// pointer to the stochastified parameter

    real trans(real z,real* x,real* a,real* b); // transforms z to delta_p
    real * trans_x;			// pointer to array classes
    real * trans_a;			// parameter for the transformation
    real * trans_b;			// parameter for the transformation

    real (* theta) (real z, real m);	// theta defines the basic dynamic for z
    real theta_type;			// the current type of the theta function
    real my;				// parameters for theta
    void theta_init (void);		// sets theta to a specific type
    
public:
    depreciate();			// constructor
    void sendStateSpace(int &,const real***);
    real * setLabels(char*);		// get a variable by a name
    void initialize(void);		// initialize the model
    void loadParamset(ifstream&);	// load parameterset from a file
    void saveParamset(ofstream&);
    void printParamset();
    void iteration(const long&);	// perform one iteration 
};
