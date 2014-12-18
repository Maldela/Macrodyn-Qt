
///////////////////////////////////////////////////////////////////////////////
//
// $Header: matrix_neu.C, Fri Aug  1 astarke
//
// Module name:		matrix_neu.C
// Contents:		class matrix_neu
//                       
//
// Author:			Andreas Starke
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

#include "matrix_neu.h"
#include "error.h"
#include <math.h>
#include <stdio.h>

#define SIGN(a,b) ((b) > 0.0 ? fabs(a) : -fabs(a))


///////////////////////////////////////////////////////////////////////////////
// 
// Initialisation of static class matrices
//
///////////////////////////////////////////////////////////////////////////////

matrix_neu* matrix_neu::mat__temp1=new matrix_neu(1,1);
matrix_neu* matrix_neu::mat__temp2=new matrix_neu(1,1);
matrix_neu* matrix_neu::mat__temp3=new matrix_neu(1,1);

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		matrix_neu.C
// Member function:	matrix_neu
// Purpose:		constructor
//
// Author:		Andreas Starke
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////
matrix_neu::matrix_neu(const int& r, const int& c) : row(r), col(c)
//default value is a 2x2-Matrix
{
    m = new qreal* [r];
    if( !m ) {
    	fatalError("matrix_neu::matrix_neu","Can't allocate memory");
    }
    for(int i=0;i<r;i++){
        m[i]=new qreal [c];
        if( !m[i] ){
	        fatalError("matrix_neu::matrix_neu","Can't allocate memory");
        }
        for(int j=0;j<c;j++)
            m[i][j]=0;
    }
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		matrix_neu.C
// Member function:	~matrix_neu
// Purpose:		destructor
//
// Author:		Andreas Starke
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

matrix_neu::~matrix_neu()
{
	if( m ) {
		for(int i=0;i<row;i++ ) 
			if( m[i] ) {
				delete m[i]; // free column i
				}
		delete m; // free rows	
	}
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		matrix_neu.C
// Member function:	new_cells
// Purpose:		redimensioning of matrix
//
// Author:		Andreas Starke
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

void matrix_neu::new_cells (int rows, int cols)
{	
    m = new qreal* [rows];
	if( !m ) {
		fatalError("matrix_neu::new_cells","Can't allocate memory");
    	}
	
    	for(int i=0;i<rows;i++){
            m[i]=new qreal [cols];
	        if( !m[i] ){
		        fatalError("matrix_neu::new_cells","Can't allocate memory");
	        }	
	    }

	row=rows;
	col=cols;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		matrix_neu.C
// Member Function:	operator()
// Purpose:		overload the operator () to use it with matrices
//			A(i,j) is the element of row i, column j
//
// Author:		Andreas Starke
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

qreal& matrix_neu::operator()(const int& r, const int& c)
{
    return m[r][c];
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		matrix_neu.C
// Friend Function:	operator*
// Purpose:		overload the operator * to use it with matrices				
//
// Author:		Andreas Starke
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

matrix_neu operator*(const matrix_neu& A, const matrix_neu& B)
{
	if ((matrix_neu::mat__temp1!=&A)&&(matrix_neu::mat__temp1!=&B)) {
		matrix_neu::mat__temp1->new_cells(A.row,B.col);
		if( !matrix_neu::mat__temp1 )
			fatalError("matrix_neu friend operator*","Can't allocate memory");
		for(int i=0;i<A.row;i++)
        		for(int j=0;j<B.col;j++){
		            matrix_neu::mat__temp1->m[i][j]=0;
        	    	for(int k=0;k<B.row;k++)
                	matrix_neu::mat__temp1->m[i][j]+= A.m[i][k] * B.m[k][j];
	        }
        return *matrix_neu::mat__temp1;
	}
	
	if ((matrix_neu::mat__temp2!=&A)&&(matrix_neu::mat__temp2!=&B)) {
		matrix_neu::mat__temp2->new_cells(A.row,B.col);
		if( !matrix_neu::mat__temp2 )
			fatalError("matrix_neu friend operator*","Can't allocate memory");
		for(int i=0;i<A.row;i++)
        		for(int j=0;j<B.col;j++){
		            matrix_neu::mat__temp2->m[i][j]=0;
        	    	for(int k=0;k<B.row;k++)
                	matrix_neu::mat__temp2->m[i][j]+= A.m[i][k] * B.m[k][j];
	        }		
        return *matrix_neu::mat__temp2;
	}
	
	if ((matrix_neu::mat__temp3!=&A)&&(matrix_neu::mat__temp3!=&B)) {
		matrix_neu::mat__temp3->new_cells(A.row,B.col);
		if( !matrix_neu::mat__temp3 )
			fatalError("matrix_neu friend operator*","Can't allocate memory");
		for(int i=0;i<A.row;i++)
        		for(int j=0;j<B.col;j++){
		            matrix_neu::mat__temp3->m[i][j]=0;
        	    	for(int k=0;k<B.row;k++)
                	matrix_neu::mat__temp3->m[i][j]+= A.m[i][k] * B.m[k][j];
	        }
        return *matrix_neu::mat__temp3;
	}
    return matrix_neu();
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		matrix_neu.C
// Friend Function:	operator+
// Purpose:		overload the operator + to use it with matrices				
//
// Author:		Andreas Starke
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

matrix_neu operator+(const matrix_neu& A, const matrix_neu& B)
{
	if ((matrix_neu::mat__temp1!=&A)&&(matrix_neu::mat__temp1!=&B)) {
		matrix_neu::mat__temp1->new_cells(A.row,B.col);
		if( !matrix_neu::mat__temp1 )
			fatalError("matrix_neu friend operator+","Can't allocate memory");
	    	for(int i=0;i<A.row;i++)
			for(int j=0;j<B.col;j++)
		        	matrix_neu::mat__temp1->m[i][j] = A.m[i][j] + B.m[i][j];
        return *matrix_neu::mat__temp1;
	}
	
	if ((matrix_neu::mat__temp2!=&A)&&(matrix_neu::mat__temp2!=&B)) {
		matrix_neu::mat__temp2->new_cells(A.row,B.col);
		if( !matrix_neu::mat__temp2 )
			fatalError("matrix_neu friend operator+","Can't allocate memory");
	    	for(int i=0;i<A.row;i++)
			for(int j=0;j<B.col;j++)
		        	matrix_neu::mat__temp2->m[i][j] = A.m[i][j] + B.m[i][j];
        return *matrix_neu::mat__temp2;
	}
	
	if ((matrix_neu::mat__temp3!=&A)&&(matrix_neu::mat__temp3!=&B)) {
		matrix_neu::mat__temp3->new_cells(A.row,B.col);
		if( !matrix_neu::mat__temp3 )
			fatalError("matrix_neu friend operator+","Can't allocate memory");
	    	for(int i=0;i<A.row;i++)
			for(int j=0;j<B.col;j++)
		        	matrix_neu::mat__temp3->m[i][j] = A.m[i][j] + B.m[i][j];
        return *matrix_neu::mat__temp3;
	}
    return matrix_neu();
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		matrix_neu.C
// Friend Function:	operator-
// Purpose:		overload the operator - to use it with matrices				
//
// Author:		Andreas Starke
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

matrix_neu operator-(const matrix_neu& A, const matrix_neu& B)
{
	if ((matrix_neu::mat__temp1!=&A)&&(matrix_neu::mat__temp1!=&B)) {
		matrix_neu::mat__temp1->new_cells(A.row,B.col);
		if( !matrix_neu::mat__temp1 )
			fatalError("matrix_neu friend operator-","Can't allocate memory");
	    	for(int i=0;i<A.row;i++)
			for(int j=0;j<B.col;j++)
		        	matrix_neu::mat__temp1->m[i][j] = A.m[i][j] - B.m[i][j];
        return *matrix_neu::mat__temp1;
	}
	
	if ((matrix_neu::mat__temp2!=&A)&&(matrix_neu::mat__temp2!=&B)) {
		matrix_neu::mat__temp2->new_cells(A.row,B.col);
		if( !matrix_neu::mat__temp2 )
			fatalError("matrix_neu friend operator-","Can't allocate memory");
	    	for(int i=0;i<A.row;i++)
			for(int j=0;j<B.col;j++)
		        	matrix_neu::mat__temp2->m[i][j] = A.m[i][j] - B.m[i][j];
        return *matrix_neu::mat__temp2;
	}
	
	if ((matrix_neu::mat__temp3!=&A)&&(matrix_neu::mat__temp3!=&B)) {
		matrix_neu::mat__temp3->new_cells(A.row,B.col);
		if( !matrix_neu::mat__temp3 )
			fatalError("matrix_neu friend operator-","Can't allocate memory");
	    	for(int i=0;i<A.row;i++)
			for(int j=0;j<B.col;j++)
		        	matrix_neu::mat__temp3->m[i][j] = A.m[i][j] - B.m[i][j];
        return *matrix_neu::mat__temp3;
	}
    return matrix_neu();
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		matrix_neu.C
// Friend Function:	operator=
// Purpose:		overload the operator = to use it with matrices				
//			A = B = C is possible now
//
// Author:		Andreas Starke
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

matrix_neu& matrix_neu::operator=(const matrix_neu& C)
{
    for(int i=0;i<C.row;i++)
        for(int j=0;j<C.col;j++)
            m[i][j] = C.m[i][j];
    row = C.row;
    col = C.col;
    return *this;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		matrix_neu.C
// Friend Function:	operator<<
// Purpose:		overload the operator << to use it with matrices				
//				log()<<A is possible now
//
// Author:		Andreas Starke
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

QTextStream& operator<<(QTextStream& ostr, const matrix_neu& C)
{
    ostr<<"\n";
    for(int i=0;i<C.row;i++){
        for(int j=0;j<C.col;j++)
            ostr << "["<<i<<","<<j<<"]:"<<C.m[i][j]<<"\t";
        ostr<<"\n";
    }
    return ostr;
}
///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		matrix_neu.C
// Member Function:	initialize
// Purpose:		sets all entries of a matrix equal zero				
//
// Author:		Andreas Starke
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

void matrix_neu::set_to_zero()
{

	for(int i=0;i<row;i++)
		for(int j=0;j<col;j++)
			m[i][j]=0;	
}



///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		matrix_neu.C
// Member Function:	deterqMinat
// Purpose:		computes deterqMinant of a matrix
//
// Author:		Andreas Starke
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

qreal matrix_neu::deterqMinant()
{
	int s,z,sz,i,sign;
    qreal value;
	matrix_neu* C;
	C = new matrix_neu(row-1,col-1);
	if( !C )
        fatalError("matrix_neu::deterqMinant","Can't allocate memory");

	if(row==1) {
		value=m[0][0];
		delete C;
		return value;
		}
	else {
		if(row==2) {
			value=(m[0][0]*m[1][1])-(m[1][0]*m[0][1]);
			delete C;
			return value;
			}
		else {
			value=0;
			sign=1;
			for(sz=0;sz<row;sz++) {
				i=0;
				if(i==sz)
					i++;
				for(s=0;s<(row-1);s++) {
					for(z=0;z<(row-1);z++)
						C->m[z][s]=m[z+1][i];
					i++;
					if(i==sz)
						i++;
				}
                value+=sign*m[0][sz]*C->deterqMinant();
				sign*=(-1);
			}
			delete C;
			return value;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		matrix_neu.C
// Member Function:	trace
// Purpose:		computes the trace of a matrix				
//
// Author:		Andreas Starke
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

qreal matrix_neu::trace()
{
    qreal value = 0;
	
	if ( row != col ) fatalError("matrix_neu::trace","no square matrix");
	
	int i,j;
	
	for ( i=0, j=0; i<row; i++, j++)
		value += m[i][j];
	
	return value;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		matrix_neu.C
// Member Function:	eigenvalues
// Purpose:		computes the eigenvalues of a matrix				
//
// Author:		Andreas Starke
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

matrix_neu matrix_neu::eigenvalues()
{
	if ( row != col ) fatalError("matrix_neu::eigenvalues","no square matrix");
	
	// temp1 is used for calculation purposes -> destroyed through computation
    // temp2 will hold eigenvalues temp2(i,0) qreal part temp2(i,1) imaginary part
	matrix_neu::mat__temp1->new_cells( col, row );
	matrix_neu::mat__temp2->new_cells( col, 2 );
	(*matrix_neu::mat__temp1) = *this;
	matrix_neu::mat__temp1->balance();
    matrix_neu::mat__temp1->eliqMinate_to_Hessenberg();
	int n = row;
    int nn,mm,l,k,j,its,i,mqMin;
	double z,y,x,w,v,u,t,s,r,q,p,anorm;
	
	anorm = fabs((*matrix_neu::mat__temp1)(0,0));
	for ( i=1; i<n; i++)
		for ( j=(i-1);j<n;j++ )
			anorm += fabs((*matrix_neu::mat__temp1)(i,j));
			
	nn = n-1;
	t = 0.0;
	while ( nn >= 0){
		its = 0;
		do {
			for (l=nn;l>=1;l--){
				s=fabs((*matrix_neu::mat__temp1)(l-1,l-1))+fabs((*matrix_neu::mat__temp1)(l,l));
				if ( s == 0.0 ) s = anorm;
				if ((double)(fabs((*matrix_neu::mat__temp1)(l,l-1)) + s) == s) break;
			}
			x = (*matrix_neu::mat__temp1)(nn,nn);
			if ( l == nn ){
				(*matrix_neu::mat__temp2)(nn,0) = x+t;
				(*matrix_neu::mat__temp2)(nn,1) = 0.0;
				nn--;
			} else {
				y = (*matrix_neu::mat__temp1)(nn-1,nn-1);
				w = (*matrix_neu::mat__temp1)(nn,nn-1)*(*matrix_neu::mat__temp1)(nn-1,nn);
				if ( l == (nn-1) ){
					p = 0.5*(y-x);
					q = p*p+w;
					z = sqrt(fabs(q));
					x += t;
					if ( q >= 0.0 ){
						z = p+SIGN(z,p);
						(*matrix_neu::mat__temp2)(nn-1,0) = (*matrix_neu::mat__temp2)(nn,0) = x+z;
						if ( z ) (*matrix_neu::mat__temp2)(nn,0) = x-w/z;
						(*matrix_neu::mat__temp2)(nn-1,1) = (*matrix_neu::mat__temp2)(nn,1) = 0.0;
					} else {
						(*matrix_neu::mat__temp2)(nn-1,0) = (*matrix_neu::mat__temp2)(nn,0) = x+p;
						(*matrix_neu::mat__temp2)(nn-1,1) = -((*matrix_neu::mat__temp2)(nn,1) = z);
					}
					nn -= 2;
				} else {
					if ( its == 30 ) fatalError("matrix_neu::eigenvalues","Too many iterations!");
					if ( its == 10 || its == 20 ){
						t += x;
                        for (int i=0; i<=nn; i++) (*matrix_neu::mat__temp1)(i,i) -= x;
						s = fabs((*matrix_neu::mat__temp1)(nn,nn-1))+fabs((*matrix_neu::mat__temp1)(nn-1,nn-2));
						y = x = 0.75*s;
						w = -0.4375*s*s;
					}
					++its;
					for (mm=(nn-2);mm>=l;mm--){
						z = (*matrix_neu::mat__temp1)(mm,mm);
						r = x-z;
						s = y-z;
						p = (r*s-w)/(*matrix_neu::mat__temp1)(mm+1,mm)+(*matrix_neu::mat__temp1)(mm,mm+1);
						q = (*matrix_neu::mat__temp1)(mm+1,mm+1)-z-r-s;
						r = (*matrix_neu::mat__temp1)(mm+2,mm+1);
						s = fabs(p)+fabs(q)+fabs(r);
						p /= s;
						q /= s;
						r /= s;
						if ( mm == l ) break;
						u = fabs((*matrix_neu::mat__temp1)(mm,mm-1))*(fabs(q)+fabs(r));
						v = fabs(p)*(fabs((*matrix_neu::mat__temp1)(mm-1,mm-1))+fabs(z)
						+ fabs((*matrix_neu::mat__temp1)(mm+1,mm+1)));
						if ( (double)(u+v) == v ) break;
					}
					for ( i=mm+2; i<=nn; i++){
						(*matrix_neu::mat__temp1)(i,i-2) = 0.0;
						if ( i != (mm+2) ) (*matrix_neu::mat__temp1)(i,i-3) = 0.0;
					}
					for ( k=mm; k<=nn-1; k++){
						if ( k != mm ){
							p = (*matrix_neu::mat__temp1)(k,k-1);
							q = (*matrix_neu::mat__temp1)(k+1,k-1);
							r = 0.0;
							if ( k != (nn-1) ) r = (*matrix_neu::mat__temp1)(k+2,k-1);
							if ( (x=fabs(p)+fabs(q)+fabs(r)) != 0.0 ){
								p /= x;
								q /= x;
								r /= x;
							}
						}
						if ((s = SIGN(sqrt(p*p+q*q+r*r),p)) != 0.0 ){
							if ( k == mm ){
								if ( l != mm )
								(*matrix_neu::mat__temp1)(k,k-1) = -((*matrix_neu::mat__temp1)(k,k-1));
							} else
								(*matrix_neu::mat__temp1)(k,k-1) = -s*x;
							p += s;
							x = p/s;
							y = q/s;
							z = r/s;
							q /= p;
							r /= p;
							for ( j=k; j<=nn; j++){
								p = (*matrix_neu::mat__temp1)(k,j)+q*(*matrix_neu::mat__temp1)(k+1,j);
								if ( k != (nn-1) ){
									p += r*(*matrix_neu::mat__temp1)(k+2,j);
									(*matrix_neu::mat__temp1)(k+2,j) -= p*z;
								}
								(*matrix_neu::mat__temp1)(k+1,j) -= p*y;
								(*matrix_neu::mat__temp1)(k,j) -= p*x;
							}
                            mqMin = nn<k+3 ? nn : k+3;
                            for ( i=l; i<=mqMin; i++){
								p = x*(*matrix_neu::mat__temp1)(i,k)+y*(*matrix_neu::mat__temp1)(i,k+1);
								if ( k != (nn-1) ){
									p += z*(*matrix_neu::mat__temp1)(i,k+2);
									(*matrix_neu::mat__temp1)(i,k+2) -= p*r;
								}
								(*matrix_neu::mat__temp1)(i,k+1) -= p*q;
								(*matrix_neu::mat__temp1)(i,k) -= p;
							}
						}
					}
				}
			}
		} while ( l< nn-1 );
	}
	return *matrix_neu::mat__temp2;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		matrix_neu.C
// Member Function:	balance
// Purpose:		balance the matrix				
//
// Author:		Andreas Starke
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

void matrix_neu::balance()
{
	if ( row != col ) fatalError("matrix_neu::balance","no square matrix");
	
	const double RADIX = 2.0;
	double s,r,g,f,c;
	int n = row;
	int last,i,j;
	double sqrdx = RADIX*RADIX;
	last=0;
	
	while ( last==0 ){
		last=1;
        for (int i=0;i<n;i++){
			r=c=0.0;
			for (j=0;j<n;j++)
				if ( j!=i ){
					c += fabs(m[j][i]);
					r += fabs(m[i][j]);
				}
			if ( c && r ){
				g = r/RADIX;
				f = 1.0;
				s = c+r;
				while (c<g){
					f *= RADIX;
					c *= sqrdx;
				}
				g = r*RADIX;
				while ( c>g ){
					f /= RADIX;
					c /= sqrdx;
				}
				if ((c+r)/f < 0.95*s){
					last = 0;
					g = 1.0/f;
					for (j=0;j<n;j++) m[i][j] *= g;
					for (j=0;j<n;j++) m[j][i] *= f;
				}
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		matrix_neu.C
// Member Function:	eliqMinate_to_Hessenberg
// Purpose:		reduction to Hessenberg form
//
// Author:		Andreas Starke
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

void matrix_neu::eliqMinate_to_Hessenberg()
{
    if ( row != col ) fatalError("matrix_neu::eliqMinate_to_Hessenberg","no square matrix");
	
	int l,j,i;
	int n = row;
	double y,x;
	
	for ( l=1;l<n-1;l++){
		x = 0.0;
		i = l;
		for (j=l;j<n;j++){
			if ( fabs(m[j][l-1]) > fabs(x) ){
				x = m[j][l-1];
				i = j;
			}
		}
		if ( i != l ){
			for (j=l-1;j<n;j++) swap_elements( m[i][j], m[l][j] );
			for (j=1;j<n;j++) swap_elements( m[j][i], m[j][l] );
		}
		if ( x ){
            for (int i=l+1;i<n;i++){
				if((y=m[i][l-1]) != 0.0 ){
					y /= x;
					m[i][l-1] = y;
					for (j=l;j<n;j++)
						m[i][j] -= y*m[l][j];
					for (j=0;j<n;j++)
						m[j][l] += y*m[j][i];
				}
			}
		}
	}
	
	for ( j=0; j<n-2; j++ )
		for ( i=j+2; i<n; i++)
			m[i][j] = 0;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		matrix_neu.C
// Member Function:	swap
// Purpose:		swap the two elements
//
// Author:		Andreas Starke
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

void matrix_neu::swap_elements(qreal a, qreal b)
{
    qreal c = a;
	a = b;
	b = c;
}


///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		matrix_neu.C
// Member Function:	inverse
// Purpose:		computes inverse of a matrix				
//
// Author:		Andreas Starke
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

matrix_neu matrix_neu::inverse()
{
if(row==2){
	matrix_neu::mat__temp1->new_cells(2,2);
	if( !matrix_neu::mat__temp1 )
		fatalError("matrix_neu::inverse","Can't allocate memory");
    qreal det;
	det = (m[0][0] * m[1][1]) - (m[1][0] * m[0][1]);
	if(fabs(det)<0.0000000001) {
        log() << "fabs(deterqMinant)=" << fabs(det) << " < 0.0000000001";
        log() << "  will be set to null !!!";
        fatalError("matrix_neu::inverse","deterqMinant=0 break");
	}
	matrix_neu::mat__temp1->m[0][0] = (1/det) * m[1][1];
	matrix_neu::mat__temp1->m[0][1] = -(1/det) * m[0][1];
	matrix_neu::mat__temp1->m[1][0] = -(1/det) * m[1][0];
	matrix_neu::mat__temp1->m[1][1] = (1/det) * m[0][0];
    return *matrix_neu::mat__temp1;
}
else{
	matrix_neu* A;
	A = new matrix_neu(row-1,col-1);
	if( !A )
		fatalError("matrix_neu::inverse","Can't allocate memory");
	matrix_neu::mat__temp3->new_cells(row,col);
	if( !matrix_neu::mat__temp3 )
		fatalError("matrix_neu::inverse","Can't allocate memory");

    qreal	detm;
	int	ss,zz,s,z,i,j,sign1,sign;

    detm=deterqMinant();
	if(fabs(detm)<0.0000000001) {
        log() << "fabs(deterqMinant)=" << fabs(detm) << " < 0.0000000001";
        log() << "  will be set to null !!!";
		detm=0;
	}
	if(detm==0)
        fatalError("matrix_neu::inverse","deterqMinant=0 break");
	if(row==1) {
		if(m[0][0]==0)
			matrix_neu::mat__temp3->m[0][0]=1/0.000001;
		 else
			matrix_neu::mat__temp3->m[0][0]=1/m[0][0];
	}
	else {
		sign1=1;
		for(zz=0;zz<row;zz++) {
			sign=sign1;
			sign1*=(-1);
			for(ss=0;ss<row;ss++) {
				i=0;
				if(i==ss)
					i++;
				for(s=0;s<(row-1);s++) {
					j=0;
					if(j==zz)
						j++;
					for(z=0;z<(row-1);z++) {
						A->m[z][s]=m[j][i];
						j++;
						if(j==zz)
							j++;
					}
					i++;
					if(i==ss)
						i++;
				}
                matrix_neu::mat__temp3->m[ss][zz]=sign*(A->deterqMinant()/detm); //[zz,ss] =[ss,zz]
				sign*=(-1);
			}
		}
	}
    delete A;	
    return *matrix_neu::mat__temp3;
}
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		matrix_neu.C
// Member Function:	transpose
// Purpose:		computes the transpose of a matrix				
//
// Author:		Andreas Starke
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

matrix_neu matrix_neu::transpose()
{
///////////////////////////////////////////////////////////////////////////////
//
// CAUTION:
// It is impossible to do (*A)=A->transpose();
// because of operator pointer redirection. Resulting matrix will be
// original matrix!!! Use temporary matrices to solve calculation problem!!!
//
///////////////////////////////////////////////////////////////////////////////

	matrix_neu::mat__temp1->new_cells(col,row);
	if( !matrix_neu::mat__temp1 )
		fatalError("matrix_neu::transpose","Can't allocate memory");
    	for(int i=0;i<row;i++)
		for(int j=0;j<col;j++){
		        matrix_neu::mat__temp1->m[j][i] = m[i][j];
			}
	return *matrix_neu::mat__temp1;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		matrix_neu.C
// Member Function:	muliplyATB
// Purpose:		transpose matrix A and compute A*B			
//
// Author:		Andreas Starke
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

matrix_neu matrix_neu::multiplyATB(const matrix_neu& B)
{ 
   	matrix_neu::mat__temp1->new_cells(col,B.col);
	if( !matrix_neu::mat__temp1 )
		fatalError("matrix_neu::multiplyATB","Can't allocate memory");
    	for(int i=0;i<col;i++)
		for(int j=0;j<B.col;j++){
			matrix_neu::mat__temp1->m[i][j]=0;
			for (int k=0;k<row;k++)
		        	matrix_neu::mat__temp1->m[i][j] += m[k][i]*B.m[k][j];
			}
	return *matrix_neu::mat__temp1;
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		matrix_neu.C
// Member Function:	muliplyABT
// Purpose:		transpose matrix B and compute A*B			
//
// Author:		Andreas Starke
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

matrix_neu matrix_neu::multiplyABT(const matrix_neu& B)
{
   	matrix_neu::mat__temp1->new_cells(row,B.row);
	if( !matrix_neu::mat__temp1 )
		fatalError("matrix_neu::multiplyABT","Can't allocate memory");
    	for(int i=0;i<row;i++)
		for(int j=0;j<B.row;j++){
			matrix_neu::mat__temp1->m[i][j]=0;
			for (int k=0;k<col;k++)
		        	matrix_neu::mat__temp1->m[i][j] += m[i][k]*B.m[j][k];
			}
	return *matrix_neu::mat__temp1;   
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		matrix_neu.C
// Member Function:	muliplyScalar
// Purpose:		computes a*B, where a is a scalar and B a matrix			
//
// Author:		Andreas Starke
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

matrix_neu matrix_neu::multiplyScalar(const qreal& a)
{
   	matrix_neu::mat__temp1->new_cells(row,col);
	if( !matrix_neu::mat__temp1 )
		fatalError("matrixDef::matrixDef","Can't allocate memory");

	for(int i=0;i<row;i++)
		for(int j=0;j<col;j++) {
				matrix_neu::mat__temp1->m[i][j]=a*m[i][j];
			}
    return *matrix_neu::mat__temp1;
}

//eof
