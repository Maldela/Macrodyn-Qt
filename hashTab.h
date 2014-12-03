#ifndef hashTable_h
#define hashTable_h

/******************************************************************************/
/*                                                                            */
/* Module name:  hashTable.h                                                  */
/* Contents:     Class definitions                                            */
/*               1) hashEntry                                                 */
/*               2) hashTable                                                 */
/* Last modified: 15.02.1995 (Markus Lohmann)                                 */
/* Modified:  15.02.95 -- n-dimensional state space                           */
/*                                                                            */
/******************************************************************************/

#include "axes.h"
#include <fstream>

/******************************************************************************/
/*                                                                            */
/* Class name:    hashEntry                                                   */
/* Derived from:  -                                                           */
/* Purpose:       defines an entry in the hash table                          */
/* Author:        Markus Lohmann                                              */
/* Last modified: 11.10.1994                                                  */
/*                                                                            */
/******************************************************************************/

struct hashEntry {
    qreal bucket;		// cell that has been hit
    unsigned hits;		// # hits into the bucket

    hashEntry();		// default constructor
};

/******************************************************************************/
/*                                                                            */
/* Class name:    hashTable                                                   */
/* Derived from:  -                                                           */
/* Purpose:       defines a hash table that hashes entries of an n dimensional*/
/*                state space                                                 */
/* Author:        Markus Lohmann                                              */
/* Last modified: 15.02.1995                                                  */
/*                state space is now n-dimensional (ML)                       */
/*                                                                            */
/******************************************************************************/

class hashTable {
  protected:
    xyRange domain;		// section of the state space under 
				// consideration	
    unsigned noEntries;         // # entries in the hash table 
    hashEntry* hashTab;         // hash table
    unsigned *discreteState;       // vector for the dicrete representation of
				// a systems state
    unsigned base;		// base used for the discrete representation of
				// a systems state
    unsigned noHits;            // # hits at all               
    unsigned noBuckets;         // # hit buckets               
    unsigned collisions;        // # collisions                
    qreal whichCell(const qreal**) const; // computes the cell number of
        			// the state space that has been hit
    unsigned hashFunction(const qreal&); // the hash function
    ofstream logFile;
  public:
    hashTable(const unsigned&,const xyRange&);    // constructor
    virtual ~hashTable();	   // destructor
    int storePoint(const qreal**);
    void discrete2Cont(const qreal&,qreal**); //computes the middle
                                   // point of a given cell
    unsigned numberOfHits();	   // returns noHits
    unsigned numberOfBuckets();	   // returns noBuckets
    unsigned orderOfCycle();	   // returns the order of the cycle or zero if
				   // no periodic behaviour could be found
    unsigned numberOfCollisions(); // returns collisions
    void resetHashTable();         // resets the hash table
    void resetDomain(const xyRange&); // resets the state space under 
				   // consideration
    qreal noCells();		   // return the number of cells in the state
				   // space
};

#endif