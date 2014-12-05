#ifndef MATRIX_INCLUDED
#define MATRIX_INCLUDED

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// Module name:  matrix.h                                                     //
// Author:       Marc Mueller                                                 //
//                                                                            //
// Contents:     Class definitions                                            //
//               1) matrixDef                                                 //
//               2) matrixFn                                                  //
//               3) OLS                                                       //
//               4) RLS                                                       //
//               5) ELS                                                       //
//               6) SG                                                        //
//                                                                            //
// Modified:     22.10.1999 Marc Mueller                                      //
// Last Modified:24.01.2000 Marc Mueller                                      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "sim.h"
#include "PublicModels/baseModel.h"
#include "Models/rand_var.h"

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// Class name:    matrixDef                                                   //
// Purpose:       defines matrix structure                                    //
// Modified:      22.10.1999 Marc Mueller                                     //
// Last Modified: 22.10.1999 Marc Mueller                                     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
struct matrixDef {		
    friend QDataStream& operator<<(QDataStream&, const matrixDef&);
    matrixDef(const uint&, const uint&);	// constructor
	~matrixDef();	// destructor
		
    uint row; // size of matrix
    uint column; // size of matrix
    qreal** m;
};   

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// Class name:    matrixFn                                                    //
// Purpose:       defines functions of matrices                               //
// Modified:      22.10.1999 Marc Mueller                                     //
// Last Modified: 29.10.1999 Marc Mueller                                     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
class matrixFn {
  private:
	matrixDef* matrix;
  public:
	matrixFn(void);	// constructor
//	~matrixFn();	// destructor

    void coutMatrix(const matrixDef*); // print matrix on standard out

    qreal determinant(const matrixDef*);

	void inverse(const matrixDef*,matrixDef*); // in,out
      // dimension and memory of out matrix won't be done
	  // in function inverse(in,out)
	matrixDef* inverse(const matrixDef*); // out=(in)^-1
	  // generates new container => use a empty pointer for output!

	void transpose(const matrixDef*,matrixDef*); // in,out
      // dimension and memory of out matrix won't be done
	  // in function transpose(in,out)
    matrixDef* transpose(const matrixDef*);
	  // generates new container => use a empty pointer for output!

	void subtract(const matrixDef* A, const matrixDef* B, matrixDef* C);
	  // C=A-B  !dimension and memory of C will not be
	  //         done in function subtract(A,B,C)
	matrixDef* subtract(const matrixDef* A, const matrixDef* B);// out=(A-B)
	  // C=A-B  !use a empty pointer with C!

	void add(const matrixDef* A, const matrixDef* B, matrixDef* C);
	  // C=A+B  !dimension and memory of C will not be
	  //         done in function subtract(A,B,C)
	matrixDef* add(const matrixDef* A, const matrixDef* B);// out=(A+B)
	  // C=A+B  !use a empty pointer with C!

    void multiplyScalar(const qreal& a, const matrixDef* A, matrixDef* B);
	  // B=a*A  dimension and memory of B will not be done
	  // in function multiplyScalar(a,A,B)
    matrixDef* multiplyScalar(const qreal& a, const matrixDef* A);
	  // B=a*A  !use a empty pointer with B!

	void multiply(const matrixDef*, const matrixDef*, matrixDef*);//A,B,C C=A+B
	  // dimension and memory of C will not be done in function multiplyAB
	matrixDef* multiply(const matrixDef* A, const matrixDef* B);
	  // out=A*B  !use a empty pointer with output!

	void multiplyATB(const matrixDef* A, const matrixDef* B, matrixDef* C);
      // dimension and memory of C will not be done in function multiplyATB
	matrixDef* multiplyATB(const matrixDef* A, const matrixDef* B);
	  // out=A'*B  !use a empty pointer with output!

	void multiplyABT(const matrixDef* A, const matrixDef* B, matrixDef* C);
      // dimension and memory of C will not be done in function multiplyABT
	matrixDef* multiplyABT(const matrixDef* A, const matrixDef* B);
	  // out=A*B'  !use a empty pointer with output!

};

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// Class name:    olsClass                                                    //
// Purpose:       ordinary least square computation                           //
// Modified:      02.11.1999 Marc Mueller                                     //
// Last Modified: 03.01.2000 Marc Mueller                                     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
class olsClass {
	private:
        QString varname;
		matrixDef* Rt;	// varphi*varphi' one period
		matrixDef* R;	// sum of all periods varphi*varphi'
		matrixDef* VPYt; //varphi*y' one period
		matrixDef* VPY; //sum of all periods varphi*y'
        qreal	constant;
	public:
        qreal alpha;
		matrixDef* Y;  // state of system
		matrixDef* YF; // forecast
		matrixDef* FE; // forcast error
		matrixFn* matrix;
        qreal* y;
		int y_num;
		void updateY();
		matrixDef* VARPHI;
		matrixDef* VARPHI2; // for forecast2 
        qreal** varphi;
		int varphi_var_num; // number of pointers
		int *varphi_numPtr; // numbers of variables of each pointer
		int *varphi_lagPtr; // time lag of variables of each pointer
		int varphi_num; // sum numbers of variables of each pointer
		void updateVARPHI();
		matrixDef* P;	// sum of all periods varphi*varphi'
		matrixDef* THETA;

        olsClass( qreal* ,int& ,int& ,qreal** ,int* );
            //   qreal* y,int& y_num,
            //   int& varphi_var_num,qreal** varphi,int* varphi_numPtr
        olsClass(QDataStream& inFile,baseModel* const model);
			// input file stream must be:
			// yName  y_num
			// number_of_variables_in_varphi
			// Name1 mum1 ...
		virtual ~olsClass();	// destructor
		virtual void initialize();
		virtual void estimate();
		virtual void forecast();
		virtual void forecast2();
};

///////////////////////////////////////////////////////////////////////////////
//
// Class name:		rlsClass
// Derived from:	olsClass
// Purpose:		recursive least square computation 
// Modified:      14.11.1999 Marc Mueller
// Last Modified: 15.Jan.2000 Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////
class rlsClass : public olsClass {
	private:
	public:
		matrixDef* Pvarphi;  // help matrix  P*varphi
		matrixDef* VtP;  // help matrix varphi'*P
		matrixDef* vPv;  // help matrix  varphi*P*varphi
        qreal lambda;
		matrixDef* TEC;  // help matrix Theta_Error_Correction
		matrixDef* PEC;  // help matrix P_Error_Correction
        rlsClass( qreal* ,int& ,int& ,qreal** ,int* );
        rlsClass( QDataStream&, baseModel* const ) ;
		virtual ~rlsClass() ;
		virtual void initialize();
		virtual void estimate();
		//   forecast(); comes from OLS
		virtual void forecast2();
};
 
///////////////////////////////////////////////////////////////////////////////
//
// Class name:		elsClass
// Derived from:	rlsClass
// Purpose:		recursive least square computation 
// Modified:      11.01.2000 Marc Mueller
// Last Modified: 24.01.2000 Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////
class elsClass : public rlsClass {
	private:
		matrixDef* TV; // theta'*varphi
	public:
        qreal* upsilon;  // upsilon vector
		int upsilonLag;  // lag of upsilon, not vector length
        elsClass( qreal* ,int& ,int& ,qreal** ,int* );
        elsClass( QDataStream&, baseModel* const ) ;
		virtual ~elsClass() ;
		virtual void initialize();
		virtual void estimate();
		//   forecast(); comes from OLS
		virtual void forecast2();
};

///////////////////////////////////////////////////////////////////////////////
//
// Class name:		sgClass
// Derived from:	elsClass
// Purpose:		recursive least square computation 
// Modified:      16.01.2000 Marc Mueller
// Last Modified: 24.01.2000 Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////
class sgClass : public elsClass {
	private:
        qreal r;	// r_t
		int upsilonStart; // for evaluation only
        qreal* ye;
        qreal* tPointer;
		matrixDef* B;
		matrixDef* BI;
        qreal eps2;
        qreal b_t;
		rand_var** Kiid;
		int bKiid; // bool var
		long ttime;
	public:
        sgClass( qreal* ,int& ,int& ,qreal** ,int* );
        sgClass( QDataStream&, baseModel* const ) ;
		~sgClass() ;
		void initialize();
        void initializeUnbiasedForecast(const qreal& sigma,
              const qreal& epsilon, qreal* p3,const qreal* start);
		void estimate();
		//   forecast(); comes from OLS
		//   forecast2(); comes from ELS
		void unbiasedForecast(int);
};

#endif // MATRIX_INCLUDED
