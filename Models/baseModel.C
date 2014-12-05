#include "baseModel.h"
#include "../error.h"

/******************************************************************************/
/*                                                                            */
/* Class name:      baseModel                                                 */
/* Member function: baseModel                                                 */
/* Purpose:         constructor                                               */
/* Modified:        08.11.1994 (Markus Lohmann)                               */
/* Last modified:   Wed Jun 17 18:30:00 METDST 1998 (Marc Mueller)            */
/*                                                                            */
/******************************************************************************/

baseModel::baseModel(const uint &dim) : dimension(dim)
{
}
baseModel::~baseModel()
{
}
/******************************************************************************/
/*                                                                            */
/* Class name:      baseModel                                                 */
/* Member function: getLength                                                 */
/* Purpose:         return length of simulation                               */
/* Last modified:   08.11.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

qint64 baseModel::getLength() const
{
    return length;
}


/******************************************************************************/
/*                                                                            */
/* Class name:      baseModel                                                 */
/* Member function: saveParamsetWithNames                                     */
/* Purpose:         add paramset to printfile (only .xpm)                     */
/* Modified:        13.01.1997 (Jorg Nikutta)                                 */
/* Last modified:   Wed Jun 17 18:30:00 METDST 1998 (Marc Mueller)            */
/*                                                                            */
/******************************************************************************/

void baseModel::saveParamsetWithNames(QDataStream& outputFile)
{
    outputFile << "\nunable to print Parameterset ";
    outputFile << "- not implemented for this model\n";
}

/******************************************************************************/
/*                                                                            */
/* Class name:      baseModel                                                 */
/* Member functions: printParamset, saveParamset                              */
/*                   sendModelVar, sendParameters, receiveParameters          */
/* Purpose:         only for development, not used!                           */
/* Modified:        Wed Jun 17 18:30:00 METDST 1998 (Marc Mueller)            */
/* Last modified:   Fri Aug 21 11:54:32 METDST 1998 (Marc Mueller)            */
/*                                                                            */
/******************************************************************************/

void baseModel::printParamset() {
	error("macrodyn::baseModel::printParamset is not implemented");
	}
void baseModel::saveParamset(QDataStream&) {
	error("macrodyn::baseModel::saveParamset is not implemented");
	}
qreal* baseModel::sendModelVar(void) {
	error("macrodyn::baseModel::sendModelVar is not implemented");
	return NULL;
	}
void baseModel::sendParameters(int&,qreal**) {
	error("macrodyn::baseModel::sendParameters is not implemented");
	}
void baseModel::receiveParameters(const qreal*) {
	error("macrodyn::baseModel::receiveParameters is not implemented");
	}
