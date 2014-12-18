#ifndef MATRIX_NEU_H
#define MATRIX_NEU_H

///////////////////////////////////////////////////////////////////////////////
//
// $Header: matrix_neu.h Fri Aug  1 astarke
//
// Module name:		matrix_neu.h
// Contents:		Class definition of the class matrix_neu
//
// Author:			Andreas Starke
// Last modified:	
// By:			
//
///////////////////////////////////////////////////////////////////////////////

#include "PublicModels/baseModel.h"

class matrix_neu
{
// overload the operators *,+,- and <<
    friend matrix_neu operator*(const matrix_neu&, const matrix_neu&);
    friend matrix_neu operator+(const matrix_neu&, const matrix_neu&);
    friend matrix_neu operator-(const matrix_neu&, const matrix_neu&);
    friend QTextStream& operator<<(QTextStream&, const matrix_neu&);

	static matrix_neu* mat__temp1;
	static matrix_neu* mat__temp2;
	static matrix_neu* mat__temp3;

public:
    matrix_neu(const int& r=2, const int& c=2);		//constructor
	~matrix_neu();					//destructor
	matrix_neu& operator=(const matrix_neu&);	//overload =
    qreal& operator()(const int&, const int&);	//overload ()
    qreal deterqMinant();				//compute deterqMinant
    qreal trace();					//compute trace
	matrix_neu eigenvalues();			//compute eigenvalues
	void set_to_zero();				//set all a(i,j)=0
    matrix_neu inverse();				//compute inverse
	matrix_neu transpose();				//transpose a matrix: A -> AT
	matrix_neu multiplyATB(const matrix_neu&);	//given A,B, compute AT * B
	matrix_neu multiplyABT(const matrix_neu&);	//given A,B, compute A * BT
    matrix_neu multiplyScalar(const qreal&);		//compute c * B, c scalar, B matrix
	void new_cells (int rows, int cols);		//redimension statics
    int row, col;			//size of matrix, rows and columns
    qreal** m;			//values of a matrix stored in 2-dim-array

private:
 	void balance();			//balance the matrix (!)changing values 
	void eliqMinate_to_Hessenberg(); //reduction to Hessenberg form
    void swap_elements( qreal a, qreal b ); //swap values of a and b
};


#endif // MATRIX_NEU_H
