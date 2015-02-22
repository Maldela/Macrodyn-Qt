// File:	powerSpec.C
// Contents:	member functions of class powerSpec
// Author:	Uli Middelberg
// Creation:	Mon Oct 28 20:40:04 MEZ 1996
// Modified:	Tue Oct 29 15:25:09 MEZ 1996
// Changes:	

#include	"powerSpec.h"
#include	"../logger.h"

/******************************************************************************/
/*                                                                            */
/* Class name:      powerSpec                                                 */
/* Member function: powerSpec                                                 */
/* Purpose:         constructor                                               */
/* Last modified:   11.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

powerSpec::powerSpec(baseModel* const bMod, const window& w,
             const QString& label, MacrodynGraphicsItem* const graph)
          :timeSeriesJob(bMod,label,graph),
       Pi(3.141592635897932), win(w)
{
    timeSeriesImag=new qreal[length];
    if( !timeSeriesImag )
	fatalError("powerSpec::powerSpec","Can't allocate enough memory");
    for(int i=0;i<length;i++)
	timeSeriesImag[i]=0.0;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      powerSpec                                                 */
/* Member function: ~powerSpec                                                */
/* Purpose:         destructor                                                */
/* Last modified:   11.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

powerSpec::~powerSpec()
{
    delete timeSeriesImag;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      powerSpec                                                 */
/* Member function: squareAndSum                                              */
/* Last modified:   11.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

qreal powerSpec::squareAndSum(const qreal& a,const qreal& b)
{
    return( a*a + b*b );
}

/******************************************************************************/
/*                                                                            */
/* Class name:      powerSpec                                                 */
/* Member function: FFTSwap                                                   */
/* Last modified:   11.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void powerSpec::FFTSwap(qreal& x, qreal& y)
{
    qreal temp;

    temp = x;
    x = y;
    y = temp;
}


/******************************************************************************/
/*                                                                            */
/* Class name:      powerSpec                                                 */
/* Member function: FFTSolve                                                  */
/* Purpose:         Does a fast fourier transformation                        */
/* Last modified:   11.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void powerSpec::FFTSolve(const int& flag)
{
    qint64 j,i,k,a,b;
    int maxpower,cntr;
    qint64 arg,pnt0,pnt1;
    qreal harm, prodqreal, prodimag, sign, hi;
    qreal *cosary;
    qreal *sinary;

    cosary = new qreal[length];
    sinary = new qreal[length];
    if( !cosary || !sinary ) 
	fatalError("powerSpec::FFTSolve","Error allocating memory!" );
    
    j = 0;
    if( flag ) {
	sign = 1.0;
	for(i=0;i<length;i++) {
	    timeSeriesqreal[i] /= length;
	    timeSeriesImag[i] /= length;
	}
    }
    else
	sign = -1.0;

    for(i=0;i<length-1;i++) {
	if(i < j ) { 
	    FFTSwap(timeSeriesqreal[i],timeSeriesqreal[j]);
	    FFTSwap(timeSeriesImag[i],timeSeriesImag[j]);
	}

	k = length / 2;
	while( k <= j ) {
	    j -= k;
	    k /=2;
	}
	j +=k;
    }
    maxpower=0;
    i=length;
    while(i != 1){
    maxpower++;
	i /= 2;
    }
    harm = 2 * Pi / length;
    for(i=0;i<length;i++) {
	hi = harm * i;
	sinary[i] = sign * sin(hi);
	cosary[i] = cos(hi);
    }
    a=2;
    b=1;
    for(cntr=1;cntr <= maxpower;cntr++) {
	pnt0=length / a;
	pnt1=0;
	for(k=0;k<=b-1;k++){
	    i=k;
	    while(i < length){
		arg=i+b;
		if( !k ) { 
		    prodqreal=timeSeriesqreal[arg];
		    prodimag=timeSeriesImag[arg];
		}
		else { 
		    prodqreal=timeSeriesqreal[arg]*cosary[pnt1]-
			     timeSeriesImag[arg]*sinary[pnt1];
		    prodimag=timeSeriesqreal[arg]*sinary[pnt1]+
			     timeSeriesImag[arg]*cosary[pnt1];
		}		    
		timeSeriesqreal[arg]=timeSeriesqreal[i]-prodqreal;
		timeSeriesImag[arg]=timeSeriesImag[i]-prodimag;
		timeSeriesqreal[i] += prodqreal;
		timeSeriesImag[i] += prodimag;
		i += a;
	    }
	    pnt1 += pnt0;
	}
	a *= 2;
	b *= 2;
    }
    delete cosary;
    delete sinary;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      powerSpec                                                 */
/* Member function: powerSpecCalc                                             */
/* Purpose:         Computes the powerspectrum                                */
/* Last modified:   11.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void powerSpec::powerSpecCalc(const qreal& delta)
{
    qint64 i;
    qreal normal, timespan;
    
    normal = 1.0 / (length * length);
    
    FFTSolve(0);
    timeSeriesqreal[0] = squareAndSum(timeSeriesqreal[0],timeSeriesImag[0]) 
	                * normal;
    for(i=1;i<=(length/2)-1;i++) 
	timeSeriesqreal[i] = normal * (squareAndSum(timeSeriesqreal[i],
						   timeSeriesImag[i])
			    + squareAndSum(timeSeriesqreal[length-i],
					   timeSeriesImag[length-i]));
    
    i=length/2;
    timeSeriesqreal[i] = squareAndSum(timeSeriesqreal[i],timeSeriesImag[i]) 
	                * normal;
    timespan = length * delta;
    for(i=0;i<=(length/2);i++)
	timeSeriesImag[i] = i / timespan;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      powerSpec                                                 */
/* Member function: parzen                                                    */
/* Purpose:         Computes the parzen multiplier for windowing the data     */
/* Last modified:   11.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

qreal powerSpec::parzen(const qint64& t)
{
    return( 1.0 - fabs((t - .5 * (length-1.0))/(0.5 * (length + 1.0))) );
}

/******************************************************************************/
/*                                                                            */
/* Class name:      powerSpec                                                 */
/* Member function: hanning                                                   */
/* Purpose:         cmputes the hanning multiplier for windowing data         */
/* Last modified:   11.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

qreal powerSpec::hanning(const qint64& t)
{
    return( .5 * (1 - cos(2.0 * Pi * t / (length-1))) );
}

/******************************************************************************/
/*                                                                            */
/* Class name:      powerSpec                                                 */
/* Member function: welch                                                     */
/* Purpose:         computes the welch multiplier for windowing data          */
/* Last modified:   11.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

qreal powerSpec::welch(const qint64& t)
{
    qreal dummy=((t - .5 * (length - 1)) / (.5 * (length + 1)));
    return( 1 -  dummy*dummy );
}

/******************************************************************************/
/*                                                                            */
/* Class name:      job                                                       */
/* Member function: job                                                       */
/* Purpose:         constructor                                               */
/* Last modified:   11.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

qreal powerSpec::hamqMing(const qint64& t)
{
    return( .54 - .46 * cos(2.0 * Pi * t / (length-1)) );
}

/******************************************************************************/
/*                                                                            */
/* Class name:      powerSpec                                                 */
/* Member function: excatBlackman                                             */
/* Purpose:         computes the excat blackman multilpier for windowing data */
/* Last modified:   11.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

qreal powerSpec::exactBlackman(const qint64& t)
{
    return( 0.42-0.5*cos(2.0*Pi*t/(length-1))+0.08*cos(4.0*Pi*t/(length-1)) );
}

/******************************************************************************/
/*                                                                            */
/* Class name:      powerSpec                                                 */
/* Member function: windowData                                                */
/* Purpose:         deterqMines the multiplier according to the value of win,  */
/*                  and windows the data                                      */
/* Last modified:   11.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void powerSpec::windowData (qreal *x)
{
    qreal  multiplier;
    qint64 i;

    for(i=0;i<=length-1;i++) {
	switch(win) {
	    case PARZEN:multiplier = parzen(i);
		break;
	    case HANNING:multiplier = hanning(i);
		break;
	    case WELCH:multiplier = welch(i);
		break;
        case HAMqMinG:multiplier = hamqMing(i);
		break;
	    case EXACTBLACKMAN:multiplier = exactBlackman(i);
		break;
	    default:multiplier = 1.0;
		break;
	}
	x[i] *= multiplier;
    }
}

/******************************************************************************/
/*                                                                            */
/* Class name:      powerSpec                                                 */
/* Member function: simulation                                                */
/* Purpose:         computes the power spectrum                               */
/* Last modified:   11.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void powerSpec::simulation()
{
    qint64 t;
    qreal ymax=-10000.0;
    qreal dummy;
    qreal ymin=10000.0;
    qreal oldX, oldY;
    
    model->initialize();                // initialize the model
    for(t=0;t<length;t++)
    {
        model->iteration(t+1);          // compute and save the
        saveSeries(t);                  // orbit
    }
    windowData(timeSeriesqreal);         // compute a window of the data
    powerSpecCalc(1.0);                 // compute the powerspectrum

    for(t=0;t<=length/2;t++)            // search for qMinimal and qMaximal
    {
        dummy = log10(timeSeriesqreal[t]);   // values in the results
        ymax = dummy > ymax ? dummy : ymax;
        ymin = dummy < ymin ? dummy : ymin;
    }
    if( screenGraphics )
    {
        QList<qreal> min = QList<qreal>() << 0.0 << ymin;
        QList<qreal> max = QList<qreal>() << 0.5 << ymax;
        QList<qint64> res = QList<qint64>() << 2 << 2;
        QStringList string = QList<QString>() << "1/t" << "N";
        QList<QColor> zeroList = QList<QColor>() << QColor(Qt::black) << QColor(Qt::black) << QColor(Qt::black);

        xyRange* range = new xyRange(2,string,min,max,res,zeroList);
        if( screenGraphics )
            screenGraphics->setXYRange(*range);
    }
    oldX=timeSeriesImag[0];
    oldY=log10(timeSeriesqreal[0]);
    for(t=1;t<=length/2;t++)
    {
        dummy=log10(timeSeriesqreal[t]);
        if( screenGraphics )
    //	    screenGraphics->setPoint(timeSeriesImag[t],dummy,9);
            screenGraphics->setLine(oldX,oldY,timeSeriesImag[t],dummy,9);
        oldX=timeSeriesImag[t];
        oldY=dummy;
    }
}

//eof
