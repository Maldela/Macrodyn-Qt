#ifndef NEWMACRO_H
#define NEWMACRO_H

//$Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/newMacro.h,v 1.1 2000/08/31 16:07:15 mmeyer Exp $
/******************************************************************************/
/*                                                                            */
/* Module name:  newMacro.h                                              */
/* Contents:     Class definition of the class newMacro                  */
/*                                                                            */
/* Last Modified: 14.02.1995 (Markus Lohmann)                                 */
/*                                                                            */
/******************************************************************************/

#include "defaultModel.h"

class newMacro : public defaultModel {
protected:
    void notProd(real&,real&);
    void notCom(real&,real&,real&);                         
    void empAndOut(real&,real&,real&,real&);
    void wageAndPrice(real&,real&,real&,char*);            
    void dynamics();                        
public:
    void iteration(const long&);
};

#endif
