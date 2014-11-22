#ifndef CONSTANTLAG_H
#define CONSTANTLAG_H

#include "defaultModel.h"
// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/constantLag.h,v 1.1 2000/08/31 15:24:54 mmeyer Exp $

/******************************************************************************/
/*                                                                            */
/* Module name:  constantLag.h                                                */
/* Contents:     Class definition of the class constantLag                    */
/*               This model is derived from the defaultModel. The difference  */
/*               is that the expected infalation rate is computed by using    */
/*               constant lags, i.e. expInflRate=theta[t-lag]                 */
/*                                                                            */
/* Last Modified: 24.02.1995 (Markus Lohmann)                                 */
/*                                                                            */
/******************************************************************************/

class constantLag : public defaultModel {
protected:
    qreal expectedInflationRate(const long);
public:
    void iteration(const long&);            // perform one iteration 
};
#endif
