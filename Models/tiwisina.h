

#ifndef _TIWISINAMAP_INCLUDED
#define _TIWISINAMAP_INCLUDED

#include "baseModel.h"
#include "rand_var.h"
#include "markov_chain.h"


class tiwisina : public baseModel
{
    Q_OBJECT

protected:
    qreal alpha,delta,beta,ybar,r;	// parameter
    qreal xt,yt;			// state variables
    qreal x0,y0;			// initial values for x, y
    qreal pt,rett,absrett,sqrett;    // statistics (prices, returns, absolute returns, sqared returns)


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
};

#endif	// _TIWISINAMAP_INCLUDED

// eof
