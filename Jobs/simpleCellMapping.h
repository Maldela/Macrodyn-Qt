// File:	simpleCellMapping.h
// Contents:	definition of class simpleCellMapping
// Author:	Uli Middelberg
// Creation:	Mon Oct 28 20:40:04 MEZ 1996
// Modified:	Tue Oct 29 15:16:47 MEZ 1996
// Changes:	

#ifndef SIMPLECELLMAPPING_H
#define SIMPLECELLMAPPING_H
#include	"geometricJob.h"

/******************************************************************************/
/*                                                                            */
/* Class name:    simpleCellMapping (2D)                                      */
/* Derived from:  geometricJob                                                */
/* Purpose:       Performs a global analysis of a simple cell mapping         */
/*                (all k-cycles and their basins of attraction)               */
/* Author:        Markus Lohmann                                              */
/* Last modified: 29.11.1994                                                  */
/*                                                                            */
/******************************************************************************/

class simpleCellMapping : public geometricJob {
  protected:
    unsigned h[2];          	        // finenesses
    int *Gr;			        // group vector
    int *P;                             // vector of the periodicities
    int *St;			        // vector of the step numbers
    unsigned K;			        // number of cells
    unsigned lastGroup;			// id of the last detected group
    const qreal **modelVars;

    void discrete2Cont(const unsigned&,qreal&,qreal&); // transformation of
					// a discrete cell to the corresponding
					// center point in the state space
    unsigned iteration(const unsigned&); // iteration of the cell mapping
    unsigned virginCell();              // returns the id of a virgin cell
	              			// Gr(j)=0
    void oldGroup(unsigned&,unsigned&,unsigned&); // a cell (or a path) is found
					// that lies in the basin of attraction
                                        // of an already known cycle; all cells
					// of that path are assigned the group
					// numberof that cycle
    void newGroup(unsigned&,unsigned&,unsigned&); // a new cycle is found; all
					// the cells belonging to that cycles
					// are assigned a new group number
  public:
    simpleCellMapping(baseModel* const, const xyRange&, MacrodynGraphicsItem* const=NULL);
    virtual ~simpleCellMapping();
    void simulation();
};

#endif
//eof
