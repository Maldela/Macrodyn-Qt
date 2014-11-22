#include "uniform_distri.h"
#include "multi_normal_distri.h"
#include <math.h>
#include "error.h"


///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       multi_normal_Distri
//
//  Member function:  compute_lowerleft_inverse_and_sqrt_of_determinate
//
//  Purpose:          check positive definiteness, compute its lower_left, the
//                    determinante and inverse and then initialize the corres-
//                    ponding member elements 
//                    direct solution in the cases dim <= 3
//
//
//  Created:          2001-12-08 (Achim Flammenkamp)
//
//  Modified:         2001-12-23 (Achim Flammenkamp)
//
///////////////////////////////////////////////////////////////////////////////

void multi_normal_Distri::compute_lowerleft_inverse_and_sqrt_of_determinate()
{
    sqrt_det = 1.0;
    if (dim == 0)
        return;
    else if (dim == 1)
    {
        if (inver[0] <= 0.0)
           fatalError("multi_normal_Distri::compute_lowerleft_inverse","covariance matrix is not positive definite");
        sqrt_det = lower_left[0] = sqrt(inver[0]);
        inver[0] = 1.0/inver[0];
        return;
    }
    else if (dim == 2)
    {
        qreal  v = inver[0]*inver[3] - inver[1]*inver[1];
        if (v <= 0.0 || inver[0]+inver[3] <= 0.0)
           fatalError("multi_normal_Distri::compute_lowerleft_inverse","covariance matrix is not positive definite");

        sqrt_det = sqrt(v);
        lower_left[0] = sqrt(inver[0]);
        lower_left[2] = inver[1]/lower_left[0];
        lower_left[1] = lower_left[2];
        lower_left[3] = sqrt(inver[3]-lower_left[1]*lower_left[2]);
        v = 1.0/v;
        inver[2] = inver[0];   // safe old value
        inver[0] =  inver[3]*v;
        inver[1] = -inver[1]*v;
        inver[2] =  inver[1];
        inver[3] =  inver[2]*v;  // use old safed value
        return;
    }
//  else if (dim == 3)
//  {
//      qreal  p[6];
//      for (unsigned h=0, i=0; i<3 ; i++)
//      for (unsigned j=i; j<3 ; j++, h++)
//          p[h] = inver[j+3*i];
//
//      qreal  v = p[0]*p[3]*p[5] + 2*p[1]*p[2]+p[4] -
//                p[1]*p[1]*p[5] - p[4]*p[4]*p[0] - p[2]*p[2]*p[3];
//      if (v <= 0.0 || p[0]+p[3]+p[5] <= 0.0)
//         fatalError("multi_normal_Distri::compute_lowerleft_inverse","covariance matrix is not positive definite");
//      sqrt_det_inv = sqrt(v);
//      v = 1.0/v;
//      inver[0] =  (p[3]*p[5]-p[4]*p[4])*v;
//      inver[1] =  (p[2]*p[4]-p[1]*p[5])*v;
//      inver[2] =  (p[1]*p[4]-p[2]*p[3])*v;
//      inver[3] =  inver[1];
//      inver[4] =  (p[0]*p[5]-p[2]*p[2])*v;
//      inver[5] =  (p[1]*p[2]-p[0]*p[4])*v;
//      inver[6] =  inver[2];
//      inver[7] =  inver[5];
//      inver[8] =  (p[0]*p[3]-p[1]*p[1])*v;
//      return;
//  }


//  Cholesky-Decomposition  cov = L * L^T   // L lower-left triangle matrix
//  store L as lower-left triangle matrix in inverse , except the diagonal

    qreal  *diag = new qreal[dim];
    for (unsigned i=0; i < dim ; i++)
    for (unsigned j=i; j < dim ; j++)
    {
        qreal v = inver[j+dim*i];
        for (unsigned h=0; h < i ; h++)
           v -= inver[h+dim*j]*inver[h+dim*i];
        if (j != i)
            lower_left[i+dim*j] = inver[i+dim*j] = v * diag[i];
        else if (v > 0.0)
           diag[i] = 1.0/(lower_left[i+dim*i]=sqrt(v));
        else
           fatalError("multi_normal_Distri::compute_lowerleft_inverse","covariance matrix is not positive definite");
    }

//  Compute Sqrt of Determinate of Covariance-Matrix
    sqrt_det = 1.0;
    for (unsigned h=0; h < dim*dim ; h+=dim+1)
       sqrt_det *= lower_left[h];

//  Compute D as inverse of Lower-left L triangle matrix
//  and store it as upper-right triangle matrix in inverse
    for (unsigned i=0; i < dim ; i++)
    for (unsigned h=0; h <= i ; h++)
    {   qreal  v = (i==h ? 1.0 : 0.0);
        for (unsigned j=0; j < h ; j++)
            v -= inver[j+dim*i]*inver[h+dim*j];
        for (unsigned j=h; j < i ; j++)
            v -= inver[j+dim*i]*inver[j+dim*h];
        inver[i+dim*h] = v * diag[i];
    }

//  Compute L^-1*L^-1^T and store it as lower-left triangle matrix in inverse
    for (unsigned i=0; i < dim ; i++)
    for (unsigned j=i; j < dim ; j++)
    {   qreal  v = 0.0;
        for (unsigned h=j; h < dim ; h++)
            v += inver[h+dim*i]*inver[h+dim*j];
        inver[i+dim*j] = v;
    }

    delete[]  diag;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       multi_normal_Distri
//
//  Member function:  multi_normal_Distri
//
//  Purpose:          default Konstruktor
//
//
//  Created:          2001-12-29 (Achim Flammenkamp)
//
//  Modified:         2001-12-30 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////

multi_normal_Distri::multi_normal_Distri(const unsigned d, const qreal *med ,
const qreal *covar ) :
prob_Distri(0), dim(d), median(new qreal[d]), lower_left(new qreal[d*d]), inver(new qreal[d*d]) 
{
    if (!dim)
        fatalError("multi_normal_Distri::multi_normal_Distri","dimension must be positive");
    for (unsigned i=0;i<dim;i++)
        median[i] = (med?med[i]:0.0);
    if (!covar)
        for (unsigned i=0;i<dim;i++)
        for (unsigned j=0;j<dim;j++)
            inver[i*dim+j] = (i==j ? 1.0 : 0.0);
    else
    {   //  store Triangle-Covariance matrix into upper-right of inverse
        for (unsigned h=0, i=0; i<dim ; i++)
        for (unsigned j=i; j<dim ; j++, h++)
            inver[j+dim*i] = covar[h];
        compute_lowerleft_inverse_and_sqrt_of_determinate();
    }

}


///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       multi_normal_Distri
//
//  Member function:  multi_normal_Distri
//
//  Purpose:          Kopie-Konstruktor
//
//
//  Created:          2001-12-29 (Achim Flammenkamp)
//
//  Modified:         2001-12-30 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////

multi_normal_Distri::multi_normal_Distri(const multi_normal_Distri &init)
: prob_Distri(0), median(0), lower_left(0), inver(0)
{
    this->operator=(init);
}


///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       multi_normal_Distri
//
//  Member function:  ~multi_normal_Distri
//
//  Purpose:          Virtuel Destructor for classes
//
//
//  Created:          2001-12-29 (Achim Flammenkamp)
//
//  Modified:         2001-12-30 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////

inline multi_normal_Distri::~multi_normal_Distri()
{
   delete[] median;
   delete[] lower_left;
   delete[] inver;
}


///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       multi_normal_Distri
//
//  Member function:  operator=
//
//  Purpose:          assignment from multi_normal_Distri object
//
//
//  Created:          2001-12-29 (Achim Flammenkamp)
//
//  Modified:         2001-12-30 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////

multi_normal_Distri& multi_normal_Distri::operator=(const multi_normal_Distri &given)
{
    if (this != &given)
    {
        delete[] median;
        delete[] lower_left;
        delete[] inver;
        state = given.state;
        dim = given.dim;
        median = new qreal[dim];
        for (unsigned i=0;i<dim;i++)
            median[i]= given.median[i];
        inver = new qreal[dim*dim];
        for (unsigned h=dim*dim;h--;)
            inver[h]= given.inver[h];
        lower_left = new qreal[dim*dim];
        for (unsigned h=dim*dim;h--;)
            lower_left[h]= given.lower_left[h];
        sqrt_det = given.sqrt_det;
    }
    return  *this;
}


///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       multi_normal_Distri
//
//  Member function:  density
//
//  Purpose:          multi_variate normal probability density function on R
//
//
//  Created:          2001-12-14 (Achim Flammenkamp)
//
//  Modified:         2001-12-30 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////

qreal multi_normal_Distri::density(const qreal x[]) const
{
    qreal  sum = 0.0;
    for (unsigned h=0, j=0; j<dim ; j++, h+=dim)
    for (unsigned i=0; i<=j ; i++)
        sum -= (i==j?0.5:1.0) * (x[i]-median[i])* inver[i+h] *(x[j]-median[j]);
    return   0.39894228040143267794/sqrt_det*exp(sum);  // 1/sqrt(2.0*M_PI)
}

///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       multi normal_Distri
//
//  Member function:  cumulat
//
//  Purpose:          cumulative probability function for normal density on R
//
//
//  Created:          2001-12-08 (Achim Flammenkamp)
//
//  Modified:         2001-12-27 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////

// qreal multi_normal_Distri::cumulat(const qreal x[]) const
// {
// }


///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       multi_normal_Distri
//
//  Member function:  inverse
//
//  Purpose:          inverse cumulative probability function for normal
//                    distribution
//                    Argument must be in ]0,1[
//
//
//  Created:          2001-12-08 (Achim Flammenkamp)
//
//  Modified:         2001-12-27 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////
//
//  Class name:       multi_normal_Distri
//
//  Member function:  random
//
//  Purpose:          random value in R with normal probability distribution
//                    Ratio method for normal deviates
//                    A.J. Kinderman & J.F. Monahan 1976
//                    see: D.E. Knuth Seminumerical Algorithm Vol. 2 Chap. 3.4.1
//
//
//  Created:          2001-12-08 (Achim Flammenkamp)
//
//  Modified:         2001-12-27 (Achim Flammenkamp)
//
//
////////////////////////////////////////////////////////////////////////////////

qreal* multi_normal_Distri::random()
{
    qreal const  C1 = 1.71552776992141359296;   // sqrt(8/E)
    qreal const  C2 = 5.13610166675096593629;   // 4*E^0.25
    qreal const  C3 = 1.036961042583566030287;  // 4/E^1.35
    static uniform_Distri  uni_distribution;
    static qreal *x= 0; 
    delete[]  x;
    x = new qreal[dim];
    qreal  *arg = new qreal[dim];
    for (unsigned i=0;i<dim;i++)
    {
        qreal val;
        //  following loop will be called 1.369 times on the average
        do {   
            uni_distribution.swap_state(state);
            arg[i] = uni_distribution.random() - 0.5;
            val = uni_distribution.random();
            uni_distribution.swap_state(state);
            arg[i] *= C1/val;
        } while ( arg[i]*arg[i] > 5-C2*val  &&  (arg[i]*arg[i] >= C3/val+1.4
                                            || arg[i]*arg[i] > -4.0*log(val)) );
        x[i] = median[i];
        for (unsigned j=0;j<=i;j++)
            x[i] += lower_left[j+i*dim]*arg[j];
    }
    delete[]  arg;
    return  x;
}
