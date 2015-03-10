

//#include "../error.h"
//#include "../strnchr.h"
#include "tiwisina.h"



tiwisina::tiwisina() : baseModel(1)
{
}


void tiwisina::iteration(const qint64&)
{
    qreal x=xt;
    qreal y=yt;
    qreal p=pt;

    xt= x*(delta+(1.-delta)/(alpha-y));
    yt= delta*y+(1.-delta)*x*x*((1./(alpha-y))-1.)*((1./(alpha-y))-1.);

    pt= (ybar/r)+(1./exp(0.5*log(-beta)))*(x/(alpha-y));
    rett= pt/p-1;
    if(rett>0.){absrett= rett;}else{absrett= -rett;}
    sqrett= rett*rett;
}



void tiwisina::initialize()
{
    xt = x0;
    yt = y0;
    pt = 1.0;
}



qreal* tiwisina::sendModelVar()
{
    return &xt;
}


qreal* tiwisina::setLabels(const QString& label)
{
    if( label == "alpha" )
        return( &alpha);
    if( label=="delta" )
        return( &delta);
    if( label=="beta" )
        return( &beta);
    if( label=="ybar" )
        return( &ybar);
    if( label=="r" )
        return( &r);
    if( label=="xt" )
        return( &xt);
    if( label=="yt" )
        return( &yt);
    if( label=="x0" )
        return( &x0 );
    if( label=="y0" )
        return( &y0 );
    if( label=="pt" )
        return( &pt );
    if( label=="rett" )
        return( &rett );
    if( label=="absrett" )
        return( &absrett );
    if( label=="sqrett" )
        return( &sqrett );
    return NULL;
}


void tiwisina::sendStateSpace(int &quantity,QList<qreal *> *stateSpace)
{
    stateSpace->clear();
    quantity = dimension;
    *stateSpace << &xt;
    *stateSpace << &yt;

//	if( stateSpace )
//		delete stateSpace;
//    *stateSpace = new const qreal* [dimension];
//	if( !(*stateSpace) )
//		fatalError("tiwisina::sendStateSpace",
//		           "Can't create state space vector");
//	quantity=dimension;
//	(*stateSpace)[0]=&xt;
//	(*stateSpace)[1]=&yt;
};


void tiwisina::loadParamset(QTextStream& inFile)
{
    inFile >> alpha ;
    inFile >> delta;
    inFile >> beta;
    inFile >> ybar;
    inFile >> r;
    inFile >> x0;
    inFile >> y0;
    inFile >> length;

    initialize();
}


void tiwisina::saveParamset(QTextStream& outFile)
{
    outFile << alpha	<< "\t";
    outFile << delta	<< "\t";
    outFile << beta 	<< "\t";
    outFile << ybar 	<< "\t";
    outFile << r 	<< "\t";
    outFile << x0 	<< "\t";
    outFile << y0 	<< "\t";
    outFile << length;
}


void tiwisina::printParamset()
{
    log() << alpha;
    log() << delta;
    log() << beta;
    log() << ybar;
    log() << r;
    log() << x0;
    log() << y0;
    log() << length;
}


void tiwisina::sendParameters(int& amount,qreal** parameters)
{
    if( *parameters )
        delete *parameters;
    amount=8;
    *parameters=new qreal[amount];
    if( !(*parameters) )
        fatalError("tiwisina::sendParameters",
                   "Can't create array for parameters");
    (*parameters[0])=alpha;
    (*parameters[1])=delta;
    (*parameters[2])=beta;
    (*parameters[3])=ybar;
    (*parameters[4])=r;
    (*parameters[5])=x0;
    (*parameters[6])=y0;
    (*parameters[7])=length;
}


void tiwisina::receiveParameters(const QList<qreal>& parameters)
{
    alpha=parameters[0];
    delta=parameters[1];
    beta=parameters[2];
    ybar=parameters[3];
    r=parameters[4];
    x0=parameters[5];
    y0=parameters[6];
    length=(long)(parameters[7]);
}


// eof
