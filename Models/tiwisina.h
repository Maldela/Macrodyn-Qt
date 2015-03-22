#ifndef Q_WS_WIN

#ifndef _TIWISINAMAP_INCLUDED
#define _TIWISINAMAP_INCLUDED

#include "baseModel.h"
#include "rand_var.h"
#include "markov_chain.h"
#include <gmpxx.h>


class tiwisina : public baseModel
{
    Q_OBJECT

protected:
    qreal a1,a2,a3,a4,a5,a6,a7,a8;	// parameter
    qreal xt,yt;			// state variables
    qreal x0,y0;			// initial values for x, y
    qreal x_prime;
    mpf_class binomial[101];
    mpf_t factTemp;


public:
    tiwisina();				// constructor
    void loadParamset(QTextStream& );
    void saveParamset(QTextStream& );
    void printParamset();
    void iteration(const qint64&);
    void initialize();
    qreal* sendModelVar();
    qreal* setLabels(const QString&);
    void sendStateSpace(int &, QList<qreal *> *);
    void sendParameters(int&,qreal**);	// write all parameters
                        // into an array and return the
                        // numbers of parameters

    void receiveParameters(const QList<qreal>&);	// receive parameter values

    mpf_class fact(int n);
    void bin();
};

#endif	// _TIWISINAMAP_INCLUDED
#endif  //Q_WS_WIN
// eof
