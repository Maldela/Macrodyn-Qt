// File:	basin.h
// Contents:	definition of class basin
// Author:	Uli Middelberg
// Creation:	Mon Oct 28 20:40:04 MEZ 1996
// Modified:	Tue Oct 29 15:16:47 MEZ 1996
// Changes:	

#ifndef BASIN_H
#define BASIN_H
#include	"geometricJob.h"

/******************************************************************************/
/*                                                                            */
/* Class name:    basin                                                       */
/* Derived from:  geometric job                                               */
/* Purpose:       Computes the basin of attraction for a given cycle          */
/* Author:        Markus Lohmann                                              */
/* Last modified: 06.02.1995                                                  */
/* Modified:      06.02.95 - cycle identification independent from the        */
/*                system's dimension (MaLo)                                   */
/*                                                                            */
/******************************************************************************/

class basin : public geometricJob {
  protected:
    int period1;                       // period of the cycle
    int dimension;                     // dimension of the system
    qreal **cycle;                    // vector to store the cycle
    qreal *stateVars;                 // vector of the model's state variables
    qreal stepX;                      // stepsize in x-direction
    qreal stepY;                      // .. y-direction
    void initCycle(char* const,qreal***,int&);
				       // read cycle out of a file
    uint isAttracting(qreal**,qreal**,const int&);
                                       // determine wether a given orbit tends
                                       // to the cycle or not

  public:
    basin(baseModel* const,const xyRange&, char* const, MacrodynGraphicsItem* const=NULL);
    virtual ~basin();
    void setStepX(const qreal&);
    void simulation();
};

#endif
//eof
