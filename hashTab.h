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
    uint hits;		// # hits into the bucket

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
    uint noEntries;         // # entries in the hash table
    hashEntry* hashTab;         // hash table
    uint *discreteState;       // vector for the dicrete representation of
				// a systems state
    uint base;		// base used for the discrete representation of
				// a systems state
    uint noHits;            // # hits at all
    uint noBuckets;         // # hit buckets
    uint collisions;        // # collisions
    qreal whichCell(const qreal**) const; // computes the cell number of
        			// the state space that has been hit
    uint hashFunction(const qreal&); // the hash function
    QTextStream logFile;

  public:
    hashTable(const uint&,const xyRange&);    // constructor
    virtual ~hashTable();	   // destructor
    int storePoint(const qreal**);
    void discrete2Cont(const qreal&,qreal**); //computes the middle
                                   // point of a given cell
    uint numberOfHits();	   // returns noHits
    uint numberOfBuckets();	   // returns noBuckets
    uint orderOfCycle();	   // returns the order of the cycle or zero if
				   // no periodic behaviour could be found
    uint numberOfCollisions(); // returns collisions
    void resetHashTable();         // resets the hash table
    void resetDomain(const xyRange&); // resets the state space under 
				   // consideration
    qreal noCells();		   // return the number of cells in the state
				   // space
};

#endif
