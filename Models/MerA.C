///////////////////////////////////////////////////////////////////////////////
//
// Module name:	MerA.C
// Contents:		Member functions of the classes MerA
//
// Author:		Marc Mueller
// Last modified:	99/11/30
//
///////////////////////////////////////////////////////////////////////////////

#include "../error.h"
#include "MerA.h"

///////////////////////////////////////////////////////////////////////////////
// Class name:		MerA
// Member function:	MerA
// Purpose:		constructor
//
// Author:		Marc Mueller
// Last modified:	98/11/3
///////////////////////////////////////////////////////////////////////////////
MerA::MerA() : baseModel(1)
{
	zvar = NULL;
	markov = NULL;
	xAll = NULL;
	p = NULL;
	q = NULL;
	qShift = NULL;
    qmin = NULL;
    qmax = NULL;
	p0 = NULL;
	q0 = NULL;
	qOld = NULL;
	ferr = NULL;
	d = NULL;
	d0 = NULL;
	dmid = NULL;
    dmin = NULL;
    dmax = NULL;
	tau1x = NULL;
	
}

///////////////////////////////////////////////////////////////////////////////
// Class name:		MerA
// Member function:	MerA
// Purpose:		destructor
//
// Author:		Marc Mueller
// Last modified:	98/11/30
///////////////////////////////////////////////////////////////////////////////
MerA::~MerA()
{
    log() << "Model MerA destructor";

	if( zvar ) delete zvar;
	if( markov ) delete markov;
	if ( xAll ) delete [] xAll;
	if ( p ) delete [] p;
	if ( q ) delete [] q;
	if ( qShift ) delete [] qShift;
    if ( qmin ) delete [] qmin;
    if ( qmax ) delete [] qmax;
	if ( p0 ) delete [] p0;
	if ( q0 ) delete [] q0;
	if ( d ) delete [] d;
	if ( dmid ) delete [] dmid;
	if ( d0 ) delete [] d0;
    if ( dmin ) delete [] dmin;
    if ( dmax ) delete [] dmax;
	if ( tau1x ) delete [] tau1x;
}

///////////////////////////////////////////////////////////////////////////////
// Class name:		MerA
// Member function:	setLabels
// Purpose:		return a pointer to a variable or a parameter specified
//
// Author:		Marc Mueller
// Last modified:	98/11/30
///////////////////////////////////////////////////////////////////////////////
qreal* MerA::setLabels(const QString& label)
{
    QString labelp = label;
	int num;
//    if (label == "N") return( (qreal*)(&N) );
    if (label == "R") return(&R);

    if (label == "d") return(d);
    if (label.startsWith("d_")) {
        labelp.remove(0, 2);
        num = labelp.toInt();
		return(&d[num]);
	}
    if (label.startsWith("d0_") ) {
        labelp.remove(0, 3);
        num = labelp.toInt();
		return(&d0[num]);
	}
    if (label.startsWith("dmin_") ) {
        labelp.remove(0, 5);
        num = labelp.toInt();
        return(&dmin[num]);
	}
    if (label.startsWith("dmax_") ) {
        labelp.remove(0, 5);
        num = labelp.toInt();
        return(&dmax[num]);
	}

    if (label == "p") return(p);
    if (label.startsWith("p_") ) {
        labelp.remove(0, 2);
        num = labelp.toInt();
		return(&p[num]);
	}

    if (label == "q") return(q);
    if (label.startsWith("q_") ) {
        labelp.remove(0, 2);
        num = labelp.toInt();
		return(&q[num]);
	}

    if (label == "qOld") return(qShift);
    if (label.startsWith("qOld_") ) {
        labelp.remove(0, 5);
        num = labelp.toInt();
		return(&qShift[num]);
	}
    if (label.startsWith("p0_") ) {
        labelp.remove(0, 3);
        num = labelp.toInt();
		return(&p0[num]);
	}
    if (label.startsWith("q0_") ) {
        labelp.remove(0, 3);
        num = labelp.toInt();
		return(&q0[num]);
	}
    if (label.startsWith("qmin_") ) {
        labelp.remove(0, 5);
        num = labelp.toInt();
        return(&qmin[num]);
	}
    if (label.startsWith("qmax_") ) {
        labelp.remove(0, 5);
        num = labelp.toInt();
        return(&dmax[num]);
	}

    if (label.startsWith("ferr_") ) {
        labelp.remove(0, 5);
        num = labelp.toInt();
		return(&ferr[num]);
	}
    if (label == "pOld") return(&pOld);
    if (label == "deltap") return(&deltap);
    if (label == "deltaq") return(&deltaq);
    if (label == "mef") return(&mef);

  return NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Class name:		MerA
// Member function:	get_new_d
// Purpose:		get new d
//
// Author:		Marc Mueller
// Last modified:	98/11/30
// By:			Marc Mueller
///////////////////////////////////////////////////////////////////////////////
//qreal MerA::get_new_d(int & i) {
qreal MerA::get_new_d(int & ) {
//	return  (zvar->dice() * (dmax[0]-dmin[0]) + dmin[0]);
	return  ( markov->dice() );
}

///////////////////////////////////////////////////////////////////////////////
// Class name:		MerA
// Member function:	initialize
// Purpose:		initialize the model, define the systems initial state
//
// Author:		Marc Mueller
// Last modified:	98/11/30
///////////////////////////////////////////////////////////////////////////////
void MerA::initialize()
{
deltap=0;
deltaq=0;
mef=0;

	for (int count=0;count<=K;count++) {
		p[count]=p0[count];
		q[count]=q0[count];
		qShift[count]=q0[count];
		qOld[count]=q0[count];
        dmid[count] = ((dmax[count]-dmin[count])/2)+dmin[count] ;
        d[count]=d0[count];
	}


    qreal DMAX;
    for(int i=0;i<K;i++){
            tau1x[i]= 0.05;

            if(perfectPredictor) {//bed. perfekter praediktor
                if(q0[i]<=((dmax[i]-dmin[i])/R)) {
                    log() <<"Asset "<<i<<" : q0["<<i<<"]="<<q0[i];
                    log() <<" <= (dmax-dmin)/R=";
                    log() <<((dmax[i]-dmin[i])/R)<<"\n";
                    exit(-1);
                }
                qmin[i]=(tau1x[i]-dmin[i])/(1-R);
                qmax[i]=(tau1x[i]-dmax[i])/(1-R);

                DMAX=((dmax[i]-dmin[i])/R)+dmin[i];
                if(dmax[i]>DMAX)
                    DMAX=dmax[i];
                if(tau1x[i]<=DMAX) {
                    log() << "Asset "<<i<<" : tau^-1x="<<tau1x[i];
                    log() <<" <= DMAX="<<DMAX<<"\n";
                    exit(-1);
                }
            }
        }

}

///////////////////////////////////////////////////////////////////////////////
// Class name:		MerA
// Member function:	loadParamset
// Purpose:		load a parameterset from a specified input file
//
// Author:		Marc Mueller
// Last modified:	98/11/30
///////////////////////////////////////////////////////////////////////////////
void MerA::loadParamset(QTextStream& inFile)
{
    QString dummy;
	int i;

	inFile >> dummy;
    if (dummy != "V1.1")
    {
	   fatalError("MerA::loadParamset - you need version V1.1 not",dummy);
       return;
    }

	inFile >> dummy;
    if (dummy == "perfect")
		perfectPredictor=1; // YES
	else perfectPredictor=0; // No
	inFile >> R;

	inFile >> K;
	if ( xAll != NULL ) delete [] xAll;
    xAll = new qreal[K+1];
     if( !(xAll) )
	   fatalError("macrodyn::MerA::loadParamset","can't create xAll");
	if ( p0 != NULL ) delete [] p0;
    p0 = new qreal[K+1];
     if( !(p0) )
	   fatalError("macrodyn::MerA::loadParamset","can't create p0");
	if ( q0 != NULL ) delete [] q0;
    q0 = new qreal[K+1];
     if( !(q0) )
	   fatalError("macrodyn::MerA::loadParamset","can't create q0");

	if ( d0 != NULL ) delete [] d0;
    d0 = new qreal[K+1];
        if( !(d0) )
	   fatalError("macrodyn::MerA::initialize","can't create d0");

    if ( dmin != NULL ) delete [] dmin;
    dmin = new qreal[K+1];
     if( !(dmin) )
       fatalError("macrodyn::MerA::initialize","can't create dmin");

    if ( dmax != NULL ) delete [] dmax;
    dmax = new qreal[K+1];
     if( !(dmax) )
       fatalError("macrodyn::MerA::initialize","can't create dmax");

    QString m_state;
    QString m_matrix;
	int statesNum,sn;
	for(i=0;i<K;i++) {
		inFile >> xAll[i];
//		inFile >> d0[i];
        inFile >> dmin[i] >> dmax[i];
        d0[i]=dmin[i];
		inFile >> p0[i];
        inFile >> q0[i];
	}
 
	inFile >> m_state;
    statesNum = m_state.count(';');
	for (sn = 0; sn < statesNum; sn++) {
		inFile >> dummy;	
        m_matrix += dummy;
        m_matrix += " ";
	}


	if ( zvar != NULL ) delete zvar;
		zvar = new rand_var( this, "ranf", "1[0,1];" );
	if( !(zvar) )
	   fatalError("macrodyn::MerA::loadParamset","can't create rand_var");

	if ( markov != NULL ) delete markov;
		markov = new markov_chain (this,"ranf",m_state,m_matrix);
	if( !(markov) )
	   fatalError("macrodyn::MerA::loadParamset","can't create markov chain");

	inFile >> length;

	if ( p != NULL ) delete [] p;
        p = new qreal[K+1];
	if( !(p) )
	   fatalError("macrodyn::MerA::initialize","can't create p");

	if ( q != NULL ) delete [] q;
        q = new qreal[K+1];
	if( !(q) )
	   fatalError("macrodyn::MerA::initialize","can't create q");

	if ( qShift != NULL ) delete [] qShift;
        qShift = new qreal[K+1];
	if( !(qShift) )
	   fatalError("macrodyn::MerA::initialize","can't create qShift");

	if ( qOld != NULL ) delete [] qOld;
        qOld = new qreal[K+1];
	if( !(qOld) )
	   fatalError("macrodyn::MerA::initialize","can't create qOld");

    if ( qmin != NULL ) delete [] qmin;
        qmin = new qreal[K+1];
    if( !(qmin) )
       fatalError("macrodyn::MerA::initialize","can't create qmin");

    if ( qmax != NULL ) delete [] qmax;
        qmax = new qreal[K+1];
    if( !(qmax) )
       fatalError("macrodyn::MerA::initialize","can't create qmax");

	if ( ferr != NULL ) delete [] ferr;
        ferr = new qreal[K+1];
	if( !(ferr) )
	   fatalError("macrodyn::MerA::initialize","can't create ferr");

	if ( d != NULL ) delete [] d;
        d = new qreal[K+1];
	if( !(d) )
	   fatalError("macrodyn::MerA::initialize","can't create d");

	if ( dmid != NULL ) delete [] dmid;
        dmid = new qreal[K+1];
	if( !(dmid) )
	   fatalError("macrodyn::MerA::initialize","can't create dmid");

	if ( tau1x != NULL ) delete [] tau1x;
        tau1x = new qreal[K+1];
        if( !(tau1x) )
	   fatalError("macrodyn::MerA::initialize","can't create tau1x");

	initialize();
}

void MerA::receiveParameters(const QList<qreal> &parameters)
{
    //TODO
    //initialize();
}

///////////////////////////////////////////////////////////////////////////////
// Class name:		MerA
// Member function: saveParamsetWithNames
// Purpose:         add  parameterset to printfile
//
// Author:		Marc Mueller
// Last modified:	
///////////////////////////////////////////////////////////////////////////////
void MerA::saveParamsetWithNames(QTextStream& outputFile)
{
    outputFile << "\nModel MerA\n";
    if(perfectPredictor)
    outputFile << "perfectPredictor";
    outputFile << "R = " << R;
    outputFile << "K = " << K;
    for(int i=0;i<K;i++) {
     	outputFile << "xAll[" << i << "] = " << xAll[i] << " ";
        outputFile << "dmin[" << i << "] = " << dmin[i] << " ";
        outputFile << "dmax[" << i << "] = " << dmin[i] << " ";
     	outputFile << "p0[" << i << "] = " << p0[i] << " ";
     	outputFile << "q0[" << i << "] = " << q0[i] << " ";
	}
    outputFile << "length = "<< length;
}

///////////////////////////////////////////////////////////////////////////////
// Class name:		MerA
// Member function:	sendStateSpace
// Purpose:		return pointers to the state variables
//	
// Author:		Marc Mueller
// Last modified:	
///////////////////////////////////////////////////////////////////////////////
void MerA::sendStateSpace(int &quantity,QList<qreal *> *stateSpace)
{
    stateSpace->clear();
    quantity=K; // K assets in Model MerA
    for(int i=0;i<K;i++)
        *stateSpace << &p[i];
}

///////////////////////////////////////////////////////////////////////////////
// Class name:		MerA
// Member function:	iteration
// Purpose:		perform one iteration of the system
//
// Author:		Marc Mueller
// Last modified:	98/11/30
///////////////////////////////////////////////////////////////////////////////
void MerA::iteration(const qint64& t)
{ 
pOld=p[0];
qreal dOld=d[0];
	int i;
	for(i=0;i<K;i++) {
		if(perfectPredictor)
			q[i]=R*q[i]+tau1x[i]-d[i];			
		        //else do not do a prediction
	//q[i]=p[i];

		qOld[i]=qShift[i];
		qShift[i]=q[i];

		d[i]=get_new_d(i);
		p[i]=(d[i]+q[i]-tau1x[i])/R;
		if(p[i]<0)
            log() <<"period="<<t<<" p["<<i<<"]="<<p[i]<<"!"<<"\n";
		ferr[i]=p[i]-qOld[i];
	}

mef = ( dOld + q[0] - tau1x[0] )/ R - qOld[0];
deltaq = q[0]-qOld[0];
deltap = p[0]-pOld;

expectation_ferr *= (t-1)/t; expectation_ferr += ferr[0]/t;
variance_ferr *= (t-1)/t; variance_ferr += (ferr[0]-expectation_ferr)*(ferr[0]-expectation_ferr)/t;
standardDeviation_ferr = sqrt(variance_ferr);
mean_ferr *= t-1; mean_ferr+=ferr[0]; mean_ferr/=t;

expectation_deltaq *= (t-1)/t; expectation_deltaq += deltaq/t;
variance_deltaq *= (t-1)/t; variance_deltaq += (deltaq-expectation_deltaq)*(deltaq-expectation_deltaq)/t;
standardDeviation_deltaq = sqrt(variance_deltaq);
mean_deltaq *= t-1; mean_deltaq+=deltaq; mean_deltaq/=t;

expectation_deltap *= (t-1)/t; expectation_deltap += deltap/t;
variance_deltap *= (t-1)/t; variance_deltap += (deltap-expectation_deltap)*(deltap-expectation_deltap)/t;
standardDeviation_deltap = sqrt(variance_deltap);
mean_deltap *= t-1; mean_deltap+=deltap; mean_deltap/=t;

expectation_p *= (t-1)/t; expectation_p += p[0]/t;
variance_p *= (t-1)/t; variance_p += (p[0]-expectation_p)*(p[0]-expectation_p)/t;
standardDeviation_p = sqrt(variance_p);
mean_p *= t-1; mean_p+=p[0]; mean_p/=t;

expectation_d *= (t-1)/t; expectation_d += d[0]/t;
variance_d *= (t-1)/t; variance_d += (d[0]-expectation_d)*(d[0]-expectation_d)/t;
standardDeviation_d = sqrt(variance_d);
mean_d *= t-1; mean_d+=d[0]; mean_d/=t;

log() << t << " d=" << d[0];
log() << " q=" << q[0] << " qOld=" << qOld[0] << " p=" << p[0];
log() << " d=" << d[0] << " Mean_d="  << mean_d << " SD_d="  << standardDeviation_d;
//log() << " p=" << p[0] << " Mean_p="  << mean_p << " SD_p="  << standardDeviation_p 
//log() << " ferr=" << ferr[0] << " Mean_ferr="  << mean_ferr << " SD_ferr="  << standardDeviation_ferr 
//log() << " deltaq=" << deltaq << " Mean_deltaq="  << mean_deltaq << " SD_deltaq="  << standardDeviation_deltaq 
//log() << " deltap=" << deltap << " Mean_deltap="  << mean_deltap << " SD_deltap="  << standardDeviation_deltap 

//log() << " q=" << q[0] << " qOld=" << qOld[0] << " mef" << mef ;

}
