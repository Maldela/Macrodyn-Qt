/******************************************************************************/
/*                                                                            */
/* Module name:  hashTab.C                                                    */
/* Contents:     Member functions of the classes                              */
/*               1) hashEntry                                                 */
/*               2) hashTable                                                 */
/*                                                                            */
/******************************************************************************/

#include "hashTab.h"
#include "error.h"

extern void fatalError(const QString&, const QString&);

/******************************************************************************/
/*                                                                            */
/* Class name:      hashEntry                                                 */
/* Member function: hashEntry                                                 */
/* Purpose:         constructor                                               */
/* Last modified:   11.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

hashEntry::hashEntry()
{
    bucket=0;
    hits=0;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      hashTable                                                 */
/* Member function: hashTable                                                 */
/* Purpose:         constructor                                               */
/* Last modified:   15.02.1995 (Markus Lohmann)                               */
/* Modified:        15.02.95 -- extensions for the n-dimensional state space  */
/*                                                                            */
/******************************************************************************/

hashTable::hashTable(const uint& entries,const xyRange& range)
          :domain(range)
{
    base=0;
    
    if( !entries )
	fatalError("hashTable::hashTable","Number of entries must be positive");

    for(int i=0;i<domain.dimension;i++) 
	base=base > domain.res[i] ? base : domain.res[i];
    
    base++;
    
    noEntries=entries;
    hashTab=new hashEntry[noEntries];
    if( !hashTab )
	fatalError("hashTable::hashTable","Can't allocate hash table");
    if( !(discreteState=new uint[domain.dimension]) )
	fatalError("hashTable::hashTable",
		   "Can't create discrete representation vector");

    resetHashTable();

}

/******************************************************************************/
/*                                                                            */
/* Class name:      hashTable                                                 */
/* Member function: ~hashTable                                                */
/* Purpose:         destructor                                                */
/* Last modified:   15.02.1995 (Markus Lohmann)                               */
/* Modified:        15.02.95 -- extensions for the n-dimensional state space  */
/*                                                                            */
/******************************************************************************/

hashTable::~hashTable()
{
    if( hashTab )
	delete hashTab;
    if( discreteState )
	delete discreteState;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      hashTable                                                 */
/* Member function: which cell                                                */
/* Purpose:         returns the no of the cell that has been hit              */
/* Last modified:   11.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

qreal hashTable::whichCell(const qreal** actualState) const
{
    qreal cell=0.0;
    static qreal logBase=log(double(base));

    for(int i=0;i<domain.dimension;i++) {
    discreteState[i]=(unsigned) ( (domain.res[i]-1)*
                    (*actualState[i]-domain.min[i])/
                   (domain.max[i]-domain.min[i]) );

    cell += discreteState[i]*exp(i*logBase);
/**************
    prod=1;
    for(l=0;l<i;l++)
        prod *= domain.res[l];
        cell += discreteState[i]*prod;
******************/
    }
    return cell;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      hashTable                                                 */
/* Member function: discrete2Cont                                             */
/* Purpose:         computes the middle point of a given cell                 */
/* Last modified:   15.02.1995 (Markus Lohmann)                               */
/* Modified:        15.02.95 -- extensions for the n-dimensional state space  */
/*                                                                            */
/******************************************************************************/

void hashTable::discrete2Cont(const qreal& cell,qreal** state)
{
    static qreal baseLog=log(double(base));
    qreal divResult;
    qreal *a,*b;
    for(int i=0;i<domain.dimension;i++) {
    a=&(domain.min[i]);
    b=&(domain.max[i]);
	divResult=floor(cell/exp(i*baseLog));
    discreteState[i]=(uint)(divResult-base*floor(divResult/base));
	*state[i]=*a+(discreteState[i]-1) * (*b - *a)/domain.res[i] +
	          0.5*(*b - *a);
    }
}

/******************************************************************************/
/*                                                                            */
/* Class name:      hashTable                                                 */
/* Member function: hashFunction                                              */
/* Purpose:         computes the home address (bucket) for a cell             */
/* Last modified:   15.02.1995 (Markus Lohmann)                               */
/* Modified:        15.02.95 -- extensions for the n-dimensional state space  */
/* Last modified:   11.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

uint hashTable::hashFunction(const qreal &cell)
{
    return ( (uint) (cell-noEntries*floor(cell/noEntries)) );
}

/******************************************************************************/
/*                                                                            */
/* Class name:      hashTable                                                 */
/* Member function: storePoint                                                */
/* Purpose:         stores a point that is identified by its n-D coordinates  */
/*                  in the hash table; 0 is returned on success, 1 otherwise  */
/* Last modified:   15.02.1995 (Markus Lohmann)                               */
/* Modified:        15.02.95 -- extensions for the n-dimensional state space  */
/*                                                                            */
/******************************************************************************/

int hashTable::storePoint(const qreal** state)
{
    qreal cell;		        // id of the cell that represents the
					// point to be stored
    uint bucket;
    qreal *value;			// pointer to a bucket in the hash table
    uint finished=0;		// 0 until the hashing is finished

    if( !domain.inRange(state) ) {      // check wether the point is in the 
      					// domain to be considered or not
//	log() << "Domain error\t" << state[0] << "\t" << state[1] 
	return 1;
    }
    cell=whichCell(state);    		// transform the coordinates into a
				        // cell id
    bucket=hashFunction(cell);		// perform the hashing
    srand48(bucket);			// initialize the random number 
					// generator to solve possible 
                                        // collisions

    while( !finished ) {
	value=&(hashTab[bucket].bucket);
	if( !(hashTab[bucket].hits) ) {	// a new entry in the hash table
	    *value=cell;		// store the cell id
	    noBuckets++;		// a "new" cell has been hit
	    finished=1;
	}
	else
	    if( (*value) != cell ) {    // collision
		if( noBuckets == noEntries )
		    fatalError("hashTable::storePoint","Hash table overflow");
        bucket=(uint)(drand48()*noEntries);	// compute the new
							// bucket
		collisions++;
	    }
	    else			// the cell has already been hit
		finished=1;
    }

    hashTab[bucket].hits++;		// increment the # of hits for the
					// cell under considerations
    noHits++;				// increment the # of hits at all

    return 0;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      hashTable                                                 */
/* Member function: resetDomain                                               */
/* Purpose:         the domain of the state space under consideration has to  */
/*                  be changed                                                */
/* Last modified:   11.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void hashTable::resetDomain(const xyRange& newDomain)
{
    domain=newDomain;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      hashTable                                                 */
/* Member function: number of hits                                            */
/* Purpose:         return the number of hits at all                          */
/* Last modified:   11.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

uint hashTable::numberOfHits()
{
    return noHits;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      hashTable                                                 */
/* Member function: number of buckets                                         */
/* Purpose:         return the number of buckets that were necessary to store */
/*                  all hit cells                                             */
/* Last modified:   11.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

uint hashTable::numberOfBuckets()
{
    return noBuckets;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      hashTable                                                 */
/* Member function: numberOfCollisions                                        */
/* Purpose:         return the # of collsions that have been detected         */
/* Last modified:   11.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

uint hashTable::numberOfCollisions()
{
    return collisions;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      hashTable                                                 */
/* Member function: order of cycle                                            */
/* Purpose:         check wether a periodic behaviour can be found or not     */
/*                  if the saved orbit was periodic return the order of cycle */
/*                  that has been identified or zero otherwise                */
/* Last modified:   11.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

uint hashTable::orderOfCycle()
{
    uint ok=1;
    qreal hitsPerBuck=noHits/(qreal)noBuckets;
    uint hits;

    for(uint i=0;(i<noEntries) && ok ;i++) {
        hits=hashTab[i].hits;
        if( hits ) {
            if( hits > 1)
            ok= (uint)(fabs(hitsPerBuck-hits) < 2);
            else
            ok = 0;
        }
    }
    if( ok )
	return noBuckets;
    else
	return 0;
}
	
/******************************************************************************/
/*                                                                            */
/* Class name:      hashTable                                                 */
/* Member function: resetHashTable                                            */
/* Purpose:         reset the hash table and all variables                    */
/* Last modified:   11.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void hashTable::resetHashTable()
{
    for(uint i=0;i<noEntries ;i++) {
	hashTab[i].bucket=0;
	hashTab[i].hits=0;
    }
    noBuckets=0;
    noHits=0;
    collisions=0;
}
    
/******************************************************************************/
/*                                                                            */
/* Class name:      hashTable                                                 */
/* Member function: noCells                                                   */
/* Purpose:         returns the number of cells in the state space            */
/* Last modified:   15.02.1995 (Markus Lohmann)                               */
/* Modified:        15.02.95 -- extensions for the n-dimensional state space  */
/*                                                                            */
/******************************************************************************/

qreal hashTable::noCells()
{
    qreal cellsAtAll=1.0;
    for(int i=0;i<domain.dimension;i++)
	cellsAtAll *= domain.res[i];
    return cellsAtAll;
}
