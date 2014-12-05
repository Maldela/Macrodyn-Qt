////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// Module name:  hashTab.C                                                    //
// Author:       Marc Mueller                                                 //
//                                                                            //
// Contents:     Member functions of the classes                              //
//               1) matrixDef                                                 //
//               2) matrixFn                                                  //
//               3) OLS RLS ELS SG                                            //
//                                                                            //
// Modified:     22.10.1999 Marc Mueller                                      //
// Last Modified:2000/02/20 Marc Mueller                                      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "matrix.h"
#include "error.h"







////////////////////////////////////////////////////////////////////////////////
// Class name:      matrixDef                                                 //
// Member function: matrixDef                                                 //
// Purpose:         constructor                                               //
// Modified:        22.10.1999 Marc Mueller                                   //
// Last Modified:   29.10.1999 Marc Mueller                                   //
////////////////////////////////////////////////////////////////////////////////
matrixDef::matrixDef(const uint& r,const uint& c) {
	row=r;
	column=c;
	
    m = new qreal*[r];
	if( !m )
		fatalError("matrixDef::matrixDef","Can't allocate memory");
    for(uint i=0; i<r; i++) {
        m[i]=new qreal[c];
		if( !m[i] ) 
			fatalError("matrixDef::matrixDef","Can't allocate memory");
        for(uint j=0;j<c;j++)
			m[i][j]=0;
	}
}

////////////////////////////////////////////////////////////////////////////////
// Class name:      matrixDef                                                 //
// Member function: ~matrixDef                                                //
// Purpose:         destructor                                                //
// Modified:        22.10.1999 Marc Mueller                                   //
// Last Modified:   22.10.1999 Marc Mueller                                   //
////////////////////////////////////////////////////////////////////////////////
matrixDef::~matrixDef() {
	if( m ) {
        for( uint i=0; i< row; i++ )
			if( m[i] )
				delete [] m[i]; // free column i
		delete [] m; // free rows
	}
}


///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		matrixDef
// Friend Function:		operator<<
// Purpose:		overload the operator << to use it with matrizes				
//				log()<<A is possible now
//
// Author:		mhoffman
// Last modified:	02.03.2001
// By:			mhoffman
//
///////////////////////////////////////////////////////////////////////////////

QDataStream& operator<<(QDataStream& ostr, const matrixDef& C)
{
    ostr<<"\n";
    for(uint i=0;i<C.row;i++){
        for(uint j=0;j<C.column;j++)
            ostr << "["<<i<<"]["<<j<<"]:"<<C.m[i][j]<<"\t";
        ostr<<"\n";
    }
    return ostr;
}








////////////////////////////////////////////////////////////////////////////////
// Class name:      matrixFn                                                  //
// Member function: matrixFn                                                  //
// Purpose:         constructor                                               //
// Modified:        22.10.1999 Marc Mueller                                   //
// Last Modified:   22.10.1999 Marc Mueller                                   //
////////////////////////////////////////////////////////////////////////////////
matrixFn::matrixFn() {
	matrix=NULL;
}

////////////////////////////////////////////////////////////////////////////////
// Class name:      matrixFn                                                  //
// Member function: log()Matrix                                                //
// Purpose:         print matrix on standard out                              //
// Modified:        27.10.1999 Marc Mueller                                   //
// Last Modified:   27.10.1999 Marc Mueller                                   //
////////////////////////////////////////////////////////////////////////////////
void matrixFn::coutMatrix(const matrixDef* A) {
    for(uint i=0;i<A->row;i++) {
        for(uint j=0;j<A->column;j++)
            log() << A->m[i][j] << " ";
        log() << "\n";
		}
//	printf("\n");
}

////////////////////////////////////////////////////////////////////////////////
// Class name:      matrixFn                                                  //
// Member function: determinant                                               //
// Purpose:         calculate the determinant of a matrix                     //
// Modified:        26.10.1999 Marc Mueller                                   //
// Last Modified:   26.10.1999 Marc Mueller                                   //
////////////////////////////////////////////////////////////////////////////////
qreal matrixFn::determinant(const matrixDef* m) {
	int s,z,sz,i,sign;
    qreal value;
	matrixDef* a;
    int row = m->row;

	if(row==1)
		return(m->m[0][0]);
	else {
		if(row==2)
			return((m->m[0][0]*m->m[1][1])-(m->m[1][0]*m->m[0][1]));
		else {
			a = new matrixDef(row-1,(m->column)-1);
			if( !a )
				fatalError("matrixFn::determinant",
						"Can't allocate memory");
			value=0;
			sign=1;
			for(sz=0;sz<row;sz++) {
				i=0;
				if(i==sz)
					i++;
				for(s=0;s<(row-1);s++) {
					for(z=0;z<(row-1);z++)
						a->m[z][s]=m->m[z+1][i];
					i++;
					if(i==sz)
						i++;
				}
				value+=sign*m->m[0][sz]*determinant(a);
				sign*=(-1);
			}
			delete a;
			return(value);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// Class name:      matrixFn                                                  //
// Member function: inverse                                                   //
// Purpose:         calculate the inverse of matrix m and use a given         //
//                  matrix container mi for output                            //
// Modified:        26.10.1999 Marc Mueller                                   //
// Last Modified:   31.10.1999 Marc Mueller                                   //
////////////////////////////////////////////////////////////////////////////////
void matrixFn::inverse(const matrixDef* m,matrixDef* mi) {
	matrixDef* a;
    qreal	detm;
	int	ss,zz,s,z,i,j,sign1,sign;
    int row = m->row;

	detm=determinant(m);
	if(fabs(detm)<0.0000000001) {
        log() << "fabs(determinant)=" << fabs(detm) << " < 0.0000000001";
        log() << "  will be set to null !!!" << "\n";
		detm=0;
	}
	if(detm==0)
		fatalError("matrixFn::inverse","determinant=0 break");

	if(row==1) {
		if(m->m[0][0]==0)
			mi->m[0][0]=1/0.000001;
		 else
			mi->m[0][0]=1/m->m[0][0];
	}
	else {
		a = new matrixDef(row-1,(m->column)-1);
		if( !a )
			fatalError("matrixFn::inverse",
					"Can't allocate memory");
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
						a->m[z][s]=m->m[j][i];
						j++;
						if(j==zz)
							j++;
					}
					i++;
					if(i==ss)
						i++;
				}
                mi->m[ss][zz]=sign*(determinant(a)/detm);
				sign*=(-1);
			}
		}
		delete a;
	}
}

////////////////////////////////////////////////////////////////////////////////
// Class name:      matrixFn                                                  //
// Member function: inverse                                                   //
// Purpose:         calculate the inverse of a matrix in a new matrixcontainer//
// Modified:        26.10.1999 Marc Mueller                                   //
// Last Modified:   26.10.1999 Marc Mueller                                   //
////////////////////////////////////////////////////////////////////////////////
matrixDef* matrixFn::inverse(const matrixDef* A) {
	matrixDef* INV;
	INV = new matrixDef(A->row,A->column);
	if( !INV )
		fatalError("matrixFn::inverse(const matrixDef*)",
				"Can't allocate memory");
	inverse(A,INV);
	return INV;
}

////////////////////////////////////////////////////////////////////////////////
// Class name:      matrixFn                                                  //
// Member function: transpose                                                 //
// Purpose:         transpose the input matrix                                //
//                dimension and memory of AT will not be done in this function//
// Modified:        27.10.1999 Marc Mueller                                   //
// Last Modified:   27.10.1999 Marc Mueller                                   //
////////////////////////////////////////////////////////////////////////////////
void matrixFn::transpose(const matrixDef* A,matrixDef* AT) { //in,out
    uint z,s;
	for(z=0;z<A->row;z++)
		for(s=0;s<A->column;s++)
				AT->m[s][z]=A->m[z][s];
}

////////////////////////////////////////////////////////////////////////////////
// Class name:      matrixFn                                                  //
// Member function: transpose                                                 //
// Purpose:         transpose the input matrix                                //
// Modified:        27.10.1999 Marc Mueller                                   //
// Last Modified:   27.10.1999 Marc Mueller                                   //
////////////////////////////////////////////////////////////////////////////////
matrixDef* matrixFn::transpose(const matrixDef* A) {
	matrixDef* AT;
	AT = new matrixDef(A->column,A->row);
	if( !AT )
		fatalError("matrixFn::transpose","Can't allocate memory");
	transpose(A,AT);
	return AT;
}

////////////////////////////////////////////////////////////////////////////////
// Class name:      matrixFn                                                  //
// Member function: subtract                                                  //
// Purpose:         subtract B from A gives C; C=A-B                          //
//                 dimension and memory of C will not be done in this function//
// Modified:        27.10.1999 Marc Mueller                                   //
// Last Modified:   27.10.1999 Marc Mueller                                   //
////////////////////////////////////////////////////////////////////////////////
void matrixFn::subtract(const matrixDef* A, const matrixDef* B, matrixDef* C) {
    uint z,s;
	for(z=0;z<A->row;z++)
		for(s=0;s<A->column;s++)
				C->m[z][s]=(A->m[z][s]-B->m[z][s]);
}

////////////////////////////////////////////////////////////////////////////////
// Class name:      matrixFn                                                  //
// Member function: subtract                                                  //
// Purpose:         subtract B from A gives C; C=A-B                          //
// Modified:        27.10.1999 Marc Mueller                                   //
// Last Modified:   27.10.1999 Marc Mueller                                   //
////////////////////////////////////////////////////////////////////////////////
matrixDef* matrixFn::subtract(const matrixDef* A, const matrixDef* B) {
	matrixDef* C;
	C = new matrixDef(A->row,A->column);
	if( !C )
		fatalError("matrixFn::subtract","Can't allocate memory");
	subtract(A,B,C);
	return C;
}

////////////////////////////////////////////////////////////////////////////////
// Class name:      matrixFn                                                  //
// Member function: add                                                       //
// Purpose:         adding A with B gives C; C=A+B                            //
//                 dimension and memory of C will not be done in this function//
// Modified:        27.10.1999 Marc Mueller                                   //
// Last Modified:   27.10.1999 Marc Mueller                                   //
////////////////////////////////////////////////////////////////////////////////
void matrixFn::add(const matrixDef* A, const matrixDef* B, matrixDef* C) {
    uint z,s;
	for(z=0;z<A->row;z++)
		for(s=0;s<A->column;s++)
				C->m[z][s]=(A->m[z][s]+B->m[z][s]);
}

////////////////////////////////////////////////////////////////////////////////
// Class name:      matrixFn                                                  //
// Member function: add                                                       //
// Purpose:         adding A with B gives C; C=A+B                            //
// Modified:        27.10.1999 Marc Mueller                                   //
// Last Modified:   27.10.1999 Marc Mueller                                   //
////////////////////////////////////////////////////////////////////////////////
matrixDef* matrixFn::add(const matrixDef* A, const matrixDef* B) {
	matrixDef* C;
	C = new matrixDef(A->row,A->column);
	if( !C )
		fatalError("matrixFn::add","Can't allocate memory");
	add(A,B,C);
	return C;
}

////////////////////////////////////////////////////////////////////////////////
// Class name:      matrixFn                                                  //
// Member function: multiplyScalar                                            //
// Purpose:         multiply matrix A with scalar a gives Matrix B            //
//                 dimension and memory of B will not be done in this function//
// Modified:        27.10.1999 Marc Mueller                                   //
// Last Modified:   27.10.1999 Marc Mueller                                   //
////////////////////////////////////////////////////////////////////////////////
void matrixFn::multiplyScalar(const qreal& a, const matrixDef* A, matrixDef* B) {
    uint z,s;
	for(z=0;z<A->row;z++)
		for(s=0;s<A->column;s++)
				B->m[z][s]=a*A->m[z][s];
}

////////////////////////////////////////////////////////////////////////////////
// Class name:      matrixFn                                                  //
// Member function: multiplyScalar                                            //
// Purpose:         multiply matrix A with scalar a and give new matrix back  //
// Modified:        27.10.1999 Marc Mueller                                   //
// Last Modified:   27.10.1999 Marc Mueller                                   //
////////////////////////////////////////////////////////////////////////////////
matrixDef* matrixFn::multiplyScalar(const qreal& a, const matrixDef* A) {
	matrixDef* B;
	B = new matrixDef(A->row,A->column);
	if( !B )
		fatalError("matrixFn::multiplyScalar","Can't allocate memory");
	multiplyScalar(a,A,B);
	return B;
}

////////////////////////////////////////////////////////////////////////////////
// Class name:      matrixFn                                                  //
// Member function: multiply                                                  //
// Purpose:         multiply matrix A with matrix B gives Matrix C            //
//                 dimension and memory of C will not be done in this function//
// Modified:        23.10.1999 Marc Mueller                                   //
// Last Modified:   23.10.1999 Marc Mueller                                   //
////////////////////////////////////////////////////////////////////////////////
void matrixFn::multiply(const matrixDef* A, const matrixDef* B, matrixDef* C) {
    uint z,s,i;
    uint row = A->row;
    uint ac = A->column;
    uint column = B->column;
	for(z=0;z<row;z++)
		for(s=0;s<column;s++) {
			C->m[z][s]=0;
			for(i=0;i<ac;i++)
				C->m[z][s]+=(A->m[z][i]*B->m[i][s]);
		}
}

////////////////////////////////////////////////////////////////////////////////
// Class name:      matrixFn                                                  //
// Member function: multiply                                                  //
// Purpose:         multiply matrix A with matrix B gives Matrix C            //
// Modified:        22.10.1999 Marc Mueller                                   //
// Last Modified:   23.10.1999 Marc Mueller                                   //
////////////////////////////////////////////////////////////////////////////////
matrixDef* matrixFn::multiply(const matrixDef* A, const matrixDef* B) {
	matrixDef* C;
	C = new matrixDef(A->row,B->column);
	if( !C )
		fatalError("matrixFn::multiply","Can't allocate memory");
	multiply(A,B,C);
	return C;
}

////////////////////////////////////////////////////////////////////////////////
// Class name:      matrixFn                                                  //
// Member function: multiplyATB                                               //
// Purpose:         C=A*B  transpose A and multiply it with B                 //
//                 dimension and memory of C will not be done in this function//
// Modified:        23.10.1999 Marc Mueller                                   //
// Last Modified:   23.10.1999 Marc Mueller                                   //
////////////////////////////////////////////////////////////////////////////////
void matrixFn::multiplyATB(const matrixDef* A,const matrixDef* B,matrixDef* C){
    uint z,s,i;
    uint row = A->column;
    uint ac = A->row;
    uint column = B->column;
	for(z=0;z<row;z++)
		for(s=0;s<column;s++) {
			C->m[z][s]=0;
			for(i=0;i<ac;i++)
                C->m[z][s]+=A->m[i][z]*B->m[i][s];
		}
}

////////////////////////////////////////////////////////////////////////////////
// Class name:      matrixFn                                                  //
// Member function: multiplyATB                                               //
// Purpose:         multiply matrix A' with matrix B gives Matrix C           //
// Modified:        29.10.1999 Marc Mueller                                   //
// Last Modified:   29.10.1999 Marc Mueller                                   //
////////////////////////////////////////////////////////////////////////////////
matrixDef* matrixFn::multiplyATB(const matrixDef* A, const matrixDef* B) {
	matrixDef* C;
	C = new matrixDef(A->column,B->column);
	if( !C )
		fatalError("matrixFn::multiplyATB","Can't allocate memory");
	multiplyATB(A,B,C);
	return C;
}

////////////////////////////////////////////////////////////////////////////////
// Class name:      matrixFn                                                  //
// Member function: multiply                                                  //
// Purpose:         C=ABT first transpose B then do multiplication ABT        //
//                 dimension and memory of C will not be done in this function//
// Modified:        23.10.1999 Marc Mueller                                   //
// Last Modified:   23.10.1999 Marc Mueller                                   //
////////////////////////////////////////////////////////////////////////////////
void matrixFn::multiplyABT(const matrixDef* A,const matrixDef* B,matrixDef* C){
    uint z,s,i;
    uint row = A->row;
    uint ac = A->column;
    uint column = B->row;
	for(z=0;z<row;z++)
		for(s=0;s<column;s++) {
			C->m[z][s]=0;
			for(i=0;i<ac;i++)
                C->m[z][s]+=A->m[z][i]*B->m[s][i];
		}
}

////////////////////////////////////////////////////////////////////////////////
// Class name:      matrixFn                                                  //
// Member function: multiplyABT                                               //
// Purpose:         multiply matrix A with matrix B' gives Matrix C           //
// Modified:        29.10.1999 Marc Mueller                                   //
// Last Modified:   29.10.1999 Marc Mueller                                   //
////////////////////////////////////////////////////////////////////////////////
matrixDef* matrixFn::multiplyABT(const matrixDef* A, const matrixDef* B) {
	matrixDef* C;
	C = new matrixDef(A->row,B->row);
	if( !C )
		fatalError("matrixFn::multiplyABT","Can't allocate memory");
	multiplyABT(A,B,C);
	return C;
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// Class name:      olsClass                                                  //
// Member function: olsClass                                                  //
// Purpose:         constructor                                               //
// Modified:        02.11.1999 Marc Mueller                                   //
// Last Modified:   19.11.1999 Marc Mueller                                   //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
olsClass::olsClass( qreal* y0, int& y_num0,
        int& varphi_var_num0, qreal** varphi0, int* varphi_numPtr0){
//log() << "ols constructor" << "\n";

	y = y0 ;
	y_num = y_num0 ;
	varphi_var_num = varphi_var_num0 ;
	varphi = varphi0 ; 
	varphi_numPtr = varphi_numPtr0 ;
	alpha=0;

	matrix = NULL;	
	VARPHI = NULL;	
	VARPHI2 = NULL;	
	Y = NULL;	
	YF = NULL;	
	FE = NULL;	
	Rt = NULL;	
	R = NULL;	
	P = NULL;	
	VPYt = NULL;	
	VPY = NULL;	
	THETA = NULL;
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// Class name:      olsClass                                                  //
// Member function: olsClass                                                  //
// Purpose:         constructor                                               //
// Modified:        14.11.1999 Marc Mueller                                   //
// Last Modified:   24.01.2000 Marc Mueller                                   //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
olsClass::olsClass(QDataStream& inFile,baseModel* const model) {
//log() << "ols constructor stream" << "\n";

	matrix = NULL;	
	VARPHI = NULL;	
	VARPHI2 = NULL;	
	Y = NULL;	
	YF = NULL;	
	FE = NULL;	
	Rt = NULL;	
	R = NULL;	
	P = NULL;	
	VPYt = NULL;	
	VPY = NULL;	
	THETA = NULL;

	// 110  10000 MEP 1   2  pPtr 3 2 dPtr 3 2
	inFile >> alpha; //  initialize factor for R(ols),P(els) matrix and a(sg)
	inFile >> varname;			
	inFile >> y_num;	// number of variables (number of assets)
//log() <<"varname="<<varname<<" y_num="<<y_num<<"\n";
	y = model->setLabels(varname); // get pointer
	if( !y )
		fatalError("olsClass::olsClass ols can't find ",varname);
	inFile >> varphi_var_num;	// number of pointers
//log() <<"varphi_var_num="<<varphi_var_num<<"\n";
    varphi = new qreal*[varphi_var_num+1]; // +1 gives space for ELS
	if( !varphi )
		fatalError("olsClass::olsClass ols ","Can't allocate *varphi[]");
	varphi_numPtr = new int[varphi_var_num+1];  // number of variables per name
	if( !varphi_numPtr )
		fatalError("olsClass::olsClass ols ","Can't allocate varphi_numPtr[]");
	varphi_lagPtr = new int[varphi_var_num+1];  // time lag of variables per name
	if( !varphi_lagPtr )
		fatalError("olsClass::olsClass ols ","Can't allocate varphi_lagPtr[]");
	varphi_num=0;
	for(int j=0;j<varphi_var_num;j++) {
		inFile >> varname;
//log() <<"name["<<j<<"]="<<varname<<"\n";
        if (varname == "CONST") {
			varphi[j] = &constant;
			inFile >> constant;
			varphi_numPtr[j]=1;
			varphi_lagPtr[j]=1;
		}
		else {
			varphi[j] = model->setLabels(varname);
			if( !varphi[j] )
				fatalError("olsClass::olsClass ols can't find ",varname);
			inFile >> varphi_numPtr[j];	// number of variables				
			inFile >> varphi_lagPtr[j];	// lag of variables				
		}
		varphi_num+=(varphi_numPtr[j]*varphi_lagPtr[j]); //sum (number_of_assets * time_lag_of_var_j)
//log() <<"varphi_numPtr["<<j<<"]="<<varphi_numPtr[j]<<"\n";
//log() <<"varphi_lagPtr["<<j<<"]="<<varphi_lagPtr[j]<<"\n";
	}
//log() <<"varphi_num="<<varphi_num<<"\n";

	if(y_num<varphi_numPtr[0]) {
        log() <<"length of forecast vector="<<y_num<<" < ";
        log() <<varphi_numPtr[0]<<"=length of first input vector in varphi"<<"\n";
        log() <<"--> this will work, but are you qreally sure?"<<"\n";
	}
	if(y_num>varphi_numPtr[0]) {
        log() <<"length of forecast vector="<<y_num<<" > ";
        log() <<varphi_numPtr[0]<<"=length of first input vector in varphi"<<"\n";
        log() <<"--> olsClass::updateY can not fill up Y"<<"\n";
		exit(-1);
	}

}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// Class name:      olsClass                                                  //
// Member function: ~olsClass                                                 //
// Purpose:         destructor                                                //
// Modified:        02.11.1999 Marc Mueller                                   //
// Last Modified:   19.11.1999 Marc Mueller                                   //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
olsClass::~olsClass() {
//log() << "ols destructor" << "\n";

	if( matrix ) delete matrix;

	if( VARPHI ) delete VARPHI;
	if( VARPHI2 ) delete VARPHI2;
	if( Y ) delete Y;
	if( YF ) delete YF;
	if( FE ) delete FE;
	if( Rt ) delete Rt;
	if( R ) delete R;
	if( P ) delete P;
	if( VPYt ) delete VPYt;
	if( VPY ) delete VPY;
	if( THETA ) delete THETA;
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// Class name:      olsClass                                                  //
// Member function: initialize                                                //
// Purpose:         initialize the ols method                                 //
// Modified:        02.11.1999 Marc Mueller                                   //
// Last Modified:   14.11.1999 Marc Mueller                                   //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
void olsClass::initialize() {
//log() << "ols initialize" << "\n";

	int i,j,jj,ij;
	matrix = new matrixFn();
	if( !matrix )
		fatalError("olsClass::initialize ","matrixFn break");
				
	VARPHI= new matrixDef(varphi_num,1);
	if( !VARPHI )
		fatalError("olsClass::initialize ","VARPHI break");
	VARPHI2= new matrixDef(varphi_num,1);
	if( !VARPHI2 )
		fatalError("olsClass::initialize ","VARPHI2 break");

	// initialize varphi
	ij=0;
	for(i=0;i<varphi_var_num;i++) { // for each pointer
// log() << "i="<<i<< "\n";
		for(jj=0;jj<varphi_numPtr[i];jj++) { // number of assets
			VARPHI->m[ij][0]=varphi[i][jj]; // set in asset jj
// log() << "VARPHI->m["<<ij<<"]="<< VARPHI->m[ij][0] << "\n";
			ij++;
		}
		for(j=1;j<varphi_lagPtr[i];j++) { // time lag 
			for(jj=0;jj<varphi_numPtr[i];jj++) { // number of assets
				VARPHI->m[ij][0]=0;
// log() << "VARPHI->m["<<ij<<"]="<< VARPHI->m[ij][0] << "\n";
				ij++;
			}
		}
	}

	Rt = new matrixDef(varphi_num,varphi_num);
	if( !Rt )
		fatalError("olsClass::initialize ","Rt break");
	R = new matrixDef(varphi_num,varphi_num);
	if( !R )
		fatalError("olsClass::initialize ","R break");
	for(i=0;i<varphi_num;i++) {
		for(j=0;j<varphi_num;j++)
			R->m[i][j]=alpha;
	}

	P = new matrixDef(varphi_num,varphi_num);
	if( !P )
		fatalError("olsClass::initialize ","P break");
	YF = new matrixDef(y_num,1);
	if( !YF )
		fatalError("olsClass::initialize ","YF break");
	Y = new matrixDef(y_num,1);
	if( !Y )
		fatalError("olsClass::initialize ","Y break");
	FE = new matrixDef(y_num,1);
	if( !FE )
		fatalError("olsClass::initialize ","FE break");
	VPY = new matrixDef(varphi_num,y_num);
	if( !VPY )
		fatalError("olsClass::initialize ","VPY break");
	VPYt = new matrixDef(varphi_num,y_num);
	if( !VPYt )
		fatalError("olsClass::initialize ","VPYt break");
	THETA = new matrixDef(varphi_num,y_num);
	if( !THETA )
		fatalError("olsClass::initialize ","THETA break");
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// Class name:      olsClass                                                  //
// Member function: updateVARPHI                                              //
// Purpose:         shift and update VARPHI vector                            //
// Modified:        18.12.1999 Marc Mueller                                   //
// Last Modified:   18.12.1999 Marc Mueller                                   //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
void olsClass::updateVARPHI() {
	int i,j,jj,ij;
	ij=varphi_num-1;
	for(i=varphi_var_num-1;i>-1;i--) { // for each pointer
		for(j=1;j<varphi_lagPtr[i];j++) { // shift if time lag >1
			for(jj=0;jj<varphi_numPtr[i];jj++) { // number of assets
				VARPHI->m[ij][0]=VARPHI->m[ij-varphi_numPtr[i]][0];
				ij--;
			}
		}
		for(jj=varphi_numPtr[i]-1;jj>-1;jj--) { // number of assets
			VARPHI->m[ij][0]=varphi[i][jj]; // set in asset jj
			ij--;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// Class name:      olsClass                                                  //
// Member function: updateY                                                   //
// Purpose:         update Y vector                                           //
// Modified:        18.12.1999 Marc Mueller                                   //
// Last Modified:   21.12.1999 Marc Mueller                                   //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
void olsClass::updateY() {
	for(int j=0;j<y_num;j++)
		Y->m[j][0]=varphi[0][j]; // y is first pointer in varphi
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// Class name:      olsClass                                                  //
// Member function: estimate                                                  //
// Purpose:         estimate theta                                            //
// Modified:        02.11.1999 Marc Mueller                                   //
// Last Modified:   18.11.1999 Marc Mueller                                   //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
void olsClass::estimate() {
//log() << "ols estimate" << "\n";
	matrix->multiplyABT(VARPHI,VARPHI,Rt); // P*VPY=THETA
	matrix->add(R,Rt,R);
	matrix->inverse(R,P);
//log() <<"P:" << "\n"; matrix->log()Matrix(P);

	// updateY
	updateY();

	matrix->multiplyABT(VARPHI,Y,VPYt); // VARPHI*Y'=VPYt
//log() <<"VPYt:" << "\n"; matrix->log()Matrix(VPYt);

	// update varphi
	updateVARPHI();

	// compute theta
	matrix->add(VPY,VPYt,VPY); // VPY+VPYt=VPY
	matrix->multiply(P,VPY,THETA); // P*VPY=THETA

//log() <<"THETA:" << "\n"; matrix->log()Matrix(THETA);

//fatalError("olsClass","break");
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// Class name:      olsClass                                                  //
// Member function: forecast                                                  //
// Purpose:         forecast y                                                //
// Modified:        02.11.1999 Marc Mueller                                   //
// Last Modified:   02.11.1999 Marc Mueller                                   //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
void olsClass::forecast() {
//log() << "ols forecast" << "\n";
	matrix->multiplyATB(THETA,VARPHI,YF); // Forecast
	for(int j=0;j<y_num;j++)	// set forecast y_t+1
		y[j] = YF->m[j][0];
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// Class name:      olsClass                                                  //
// Member function: forecast2                                                 //
// Purpose:         forecast y for the period after the next period           //
// Modified:        02.11.1999 Marc Mueller                                   //
// Last Modified:   Marc Mueller                                              //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
void olsClass::forecast2() {
log() << "ols forecast2 not implemented yet - exit" << "\n";
exit(-1);
}

///////////////////////////////////////////////////////////////////////////////
//
// Class name:		rlsClass
// Member function:	rlsClass
// Purpose:		constructor
//
// Modified:        14.11.1999 Marc Mueller
// Last Modified:   15.Jan.2000 Marc Mueller
// 
///////////////////////////////////////////////////////////////////////////////
rlsClass::rlsClass( qreal* y0,int& y_num0,int& varphi_var_num0,
                    qreal** varphi0, int* varphi_numPtr0 )
	: olsClass::olsClass( y0,y_num0,varphi_var_num0,varphi0,varphi_numPtr0) {
//log() << "rls constructor" << "\n";
	Pvarphi = NULL;	
	VtP = NULL;
	vPv = NULL;
	TEC = NULL;
	PEC = NULL;
	alpha=1000000;
}
rlsClass::rlsClass(QDataStream& inFile,baseModel* const model)
	: olsClass::olsClass(inFile,model) {
//log() << "rls constructor stream" << "\n";
	Pvarphi = NULL;	
	VtP = NULL;
	vPv = NULL;
	TEC = NULL;
	PEC = NULL;
}
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// Class name:      rlsClass                                                  //
// Member function: ~rlsClass                                                 //
// Purpose:         destructor                                                //
// Modified:        18.12.1999 Marc Mueller                                   //
// Last Modified:   15.Jan.2000 Marc Mueller                                  //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
rlsClass::~rlsClass() {
//log() << "rls destructor" << "\n";
	if( Pvarphi ) delete Pvarphi;
	if( VtP ) delete VtP;
	if( vPv ) delete vPv;
	if( TEC ) delete TEC;
	if( PEC ) delete PEC;
}

///////////////////////////////////////////////////////////////////////////////
//
// Class name:		rlsClass
// Member function:	initialize
// Purpose:		initialize the system
//
// Modified:        14.11.1999 Marc Mueller
// Last Modified:   18.11.1999 Marc Mueller
// 
///////////////////////////////////////////////////////////////////////////////
void rlsClass::initialize() {

//log() << "rls init 1" << "\n";
	olsClass::initialize();
//log() << "rls init 2" << "\n";
	int i,j;

// initialize matrix THETA
	for(i=0; i<varphi_num; i++)
		for(j=0;j<y_num;j++)
			THETA->m[i][j]=0;
    //log() <<"THETA:" << "\n"; matrix->log()Matrix(THETA);

// VARPHI is already initialized in olsClass::initialize

// initialize matrix P
	for(i=0;i<varphi_num;i++) {
		for(j=0;j<varphi_num;j++)
			P->m[i][j]=0;
		P->m[i][i]=alpha;
	}
//log() <<"P:" << "\n"; matrix->log()Matrix(P);

// initialize scalar lambda
	Pvarphi= new matrixDef(varphi_num,1);
	if( !Pvarphi )
		fatalError("rlsClass::initialize ","Pvarphi break");
	matrix->multiply(P,VARPHI,Pvarphi); // P*VARPHI=Pvarphi
    //log() <<"Pvarphi:" << "\n"; matrix->log()Matrix(Pvarphi);
	vPv= new matrixDef(1,1);
	if( !vPv )
		fatalError("rlsClass::initialize ","vPv break");
	matrix->multiplyATB(VARPHI,Pvarphi,vPv); // VARPHI'*Pvarphi=vPv
	lambda= 1/ ( 1 + vPv->m[0][0] ) ;
    //log() <<"vPv:" << "\n"; matrix->log()Matrix(vPv);
    //log() <<"lambda=" << lambda<< "\n";

// build matrix ThetaErrorCorrection
	TEC = new matrixDef(varphi_num,y_num);
	if( !TEC )
		fatalError("rlsClass::initialize ","TEC break");

// build matrix varphi'*P
	VtP= new matrixDef(1,varphi_num);
	if( !VtP )
		fatalError("rlsClass::initialize ","VtP break");

// build matrix P_ErrorCorrection
	PEC = new matrixDef(varphi_num,varphi_num);
	if( !PEC )
		fatalError("rlsClass::initialize ","PEC break");

// log() <<"THETA:" << "\n"; matrix->log()Matrix(THETA);
// log() <<"VARPHI:" << "\n"; matrix->log()Matrix(VARPHI);
// log() <<"P:" << "\n"; matrix->log()Matrix(P);
// log() <<"Pvarphi:" << "\n"; matrix->log()Matrix(Pvarphi);
// log() <<"vPv:" << "\n"; matrix->log()Matrix(vPv);
// log() <<"lambda=" << lambda<< "\n";
}

////////////////////////////////////////////////////////////////////////////////
// 
// Class name:      rlsClass  
// Member function: estimate
// Purpose:         estimate theta  
// Modified:        14.11.1999 Marc Mueller
// Last Modified:   03.Jan.2000 Marc Mueller
//
////////////////////////////////////////////////////////////////////////////////
void rlsClass::estimate() {
//log() << "rls estimate" << "\n";

// compute THETA recursive
	matrix->multiplyATB(THETA,VARPHI,YF); //  THETA'*VARPHI=YF Forecast for this period
	updateY();
	matrix->subtract(Y,YF,FE); //  Y-YF=FE Forecast Error
	matrix->multiplyScalar(lambda,Pvarphi,Pvarphi);
	matrix->multiplyABT(Pvarphi,FE,TEC); //  
	matrix->add(THETA,TEC,THETA); //  THETA+TEC=THETA
// log() <<"YF:" << "\n"; matrix->log()Matrix(YF);
// log() <<"Y:" << "\n"; matrix->log()Matrix(Y);
// log() <<"FE" << "\n"; matrix->log()Matrix(FE);
// log() <<"lambda*Pvarphi:" << "\n"; matrix->log()Matrix(Pvarphi);
// log() <<"TEC:" << "\n"; matrix->log()Matrix(TEC);
// log() <<"THETA:" << "\n"; matrix->log()Matrix(THETA);

// update P recursive
	matrix->multiplyATB(VARPHI,P,VtP); // VARPHI'*P=VtP 
	matrix->multiply(Pvarphi,VtP,PEC); 
	//multiplyScalar lambda on PEC is already done before in Pvarphi
	matrix->subtract(P,PEC,P);
// log() <<"VtP:" << "\n"; matrix->log()Matrix(VtP);
// log() <<"PEC:" << "\n"; matrix->log()Matrix(PEC);
// log() <<"P:" << "\n"; matrix->log()Matrix(P);

// update VARPHI
	updateVARPHI();
// log() <<"VARPHI:" << "\n"; matrix->log()Matrix(VARPHI);

// update lambda
	matrix->multiply(P,VARPHI,Pvarphi); // P*VARPHI=Pvarphi
	matrix->multiplyATB(VARPHI,Pvarphi,vPv); // VARPHI'*Pvarphi=vPv
	lambda= 1/ ( 1 + vPv->m[0][0] ) ;
// log() <<"Pvarphi:" << "\n"; matrix->log()Matrix(Pvarphi);
// log() <<"vPv:" << "\n"; matrix->log()Matrix(vPv);
// log() <<"lambda=" << lambda<< "\n";
// log() <<"lambda*P=" << lambda*P->m[0][0]<< "\n";
}

////////////////////////////////////////////////////////////////////////////////
// 
// Class name:      rlsClass  
// Member function: forecast2
// Purpose:         forecast y for the period after the next period
// Modified:        01.Jan.2000 Marc Mueller
// Last Modified:   03.Jan.2000 Marc Mueller
//
////////////////////////////////////////////////////////////////////////////////
void rlsClass::forecast2() {
log() << "rls forecast2" << "\n";

// Forecast y_{t+1}
	matrix->multiplyATB(THETA,VARPHI,YF); // Forecast y_{t+1}

// build VARPHI2 which is a updated VARPHI
	int i,j,jj,ij;
	ij=varphi_num-1;
	for(i=varphi_var_num-1;i>-1;i--) { // for each pointer
		for(j=1;j<varphi_lagPtr[i];j++) { // shift if time lag >1
			for(jj=0;jj<varphi_numPtr[i];jj++) { // number of assets
				VARPHI2->m[ij][0]=VARPHI->m[ij-varphi_numPtr[i]][0];
				ij--;
			}
		}
		for(jj=varphi_numPtr[i]-1;jj>-1;jj--) { // number of assets
			VARPHI2->m[ij][0]=varphi[i][jj]; // set in asset jj
			ij--;
		}
	}
// set Y=YF in VARPHI2
	for(j=0;j<y_num;j++)  // number of assets
			VARPHI2->m[j][0]=YF->m[j][0]  ; // set in asset jj

// Forecast y_{t+2}
	matrix->multiplyATB(THETA,VARPHI2,YF); // Forecast
	for(j=0;j<y_num;j++)	// set forecast y_{t+2}
		y[j] = YF->m[j][0];
}

///////////////////////////////////////////////////////////////////////////////
//
// Class name:		elsClass
// Member function:	elsClass
// Purpose:		constructor
//
// Modified:        11.01.2000 Marc Mueller
// Last Modified:   11.01.2000 Marc Mueller
// 
///////////////////////////////////////////////////////////////////////////////
elsClass::elsClass( qreal* y0,int& y_num0,int& varphi_var_num0,
                    qreal** varphi0, int* varphi_numPtr0 )
	: rlsClass::rlsClass( y0,y_num0,varphi_var_num0,varphi0,varphi_numPtr0) {
// remark : given vectors must already include space for upsilon in varphi !!!
//          and full data in varphi_numPtr,varphi_lagPtr !!!
//log() << "els constructor" << "\n";
	TV = NULL;	
	upsilon = NULL;
}

elsClass::elsClass(QDataStream& inFile,baseModel* const model)
	: rlsClass::rlsClass(inFile,model) {
//log() << "els constructor stream" << "\n";
	TV = NULL;	
	upsilon = NULL;
// remark: variable varphi_var_num = number of pointers
// remark: rls,ols has last entry in arrayname[varphi_var_num-1]
//         memory+1 for arrayname[varphi_var_num] is already defined in ols stream constructor

	inFile >> upsilonLag; // lag of upsilon in time				

// enlarge varphi_num for upsilon so that all other matricies can be initialized correctly
	if(upsilonLag>0) {
//log() <<"elsClass:org varphi_num="<<varphi_num<<"\n";
//log() <<"elsClass:org varphi_var_num="<<varphi_var_num<<"\n";
		varphi_numPtr[varphi_var_num]=y_num; // number of variables in upsilon vector			
		varphi_lagPtr[varphi_var_num]=upsilonLag;
//log() <<"elsClass:varphi_numPtr["<<varphi_var_num<<"]="<<varphi_numPtr[varphi_var_num]<<"\n";
//log() <<"elsClass:varphi_lagPtr["<<varphi_var_num<<"]="<<varphi_lagPtr[varphi_var_num]<<"\n";
		varphi_num+=(varphi_numPtr[varphi_var_num]  //sum (number_of_assets
			    *varphi_lagPtr[varphi_var_num]);// * time_lag_of_var_j)	
		varphi_var_num +=1;
	}
//log() <<"elsClass:varphi_num="<<varphi_num<<"\n";
//log() <<"elsClass:varphi_var_num="<<varphi_var_num<<"\n";

}
////////////////////////////////////////////////////////////////////////////////
//
// Class name:      elsClass
// Member function: ~elsClass
// Purpose:         destructor
//
// Modified:        11.01.2000 Marc Mueller
// Last Modified:   11.01.2000 Marc Mueller
//
////////////////////////////////////////////////////////////////////////////////
elsClass::~elsClass() {
//log() << "els destructor" << "\n";
	if( TV ) delete TV;
	if( upsilon ) delete upsilon;
}

///////////////////////////////////////////////////////////////////////////////
//
// Class name:		elsClass
// Member function:	initialize
// Purpose:		initialize the system
//
// Modified:        11.01.2000 Marc Mueller
// Last Modified:   24.01.2000 Marc Mueller
// 
///////////////////////////////////////////////////////////////////////////////
void elsClass::initialize() {

// build upsilon and TV
    upsilon = new qreal[y_num];
	if( !upsilon )
		fatalError("elsClass::initialize ","upsilon break");
	// upsilon0 is y0 because of zero theta0
	for(int j=0;j<y_num;j++) // upsilon_num is defined as y_num
		upsilon[j]=0; // init with zero, because upsilon_t=0 is not y_t=0 !!!
//		upsilon[j]=varphi[0][j]; // y is first pointer in varphi

	TV = new matrixDef(y_num,1);
	if( !TV )
		fatalError("elsClass::initialize ","TV break");

// set upsilon in varphi (variable collector)  / varphi gets pointer of upsilon
	if(upsilonLag>0)
		varphi[varphi_var_num-1] = upsilon; // others come from model->setLabels(varname);

//log() << "els init 1" << "\n";
	rlsClass::initialize(); // all other matricies will be build in memory
//log() << "els init 2" << "\n";

}

///////////////////////////////////////////////////////////////////////////////
//
// Class name:		elsClass
// Member function:	estimate
// Purpose:		estimate 
//
// Modified:        11.01.2000 Marc Mueller
// Last Modified:   12.01.2000 Marc Mueller
// 
///////////////////////////////////////////////////////////////////////////////
void elsClass::estimate() {
// compute THETA recursive
	matrix->multiplyATB(THETA,VARPHI,YF); //  THETA'*VARPHI=YF Forecast for this period
	updateY();
	matrix->subtract(Y,YF,FE); //  Y-YF=FE Forecast Error
	matrix->multiplyScalar(lambda,Pvarphi,Pvarphi);
	matrix->multiplyABT(Pvarphi,FE,TEC); //  
	matrix->add(THETA,TEC,THETA); //  THETA+TEC=THETA
// compute P recursive
	matrix->multiplyATB(VARPHI,P,VtP); // VARPHI'*P=VtP 
	matrix->multiply(Pvarphi,VtP,PEC); 
	//multiplyScalar lambda on PEC is already done before in Pvarphi
	matrix->subtract(P,PEC,P);
// compute upsilon
	matrix->multiplyATB(THETA,VARPHI,TV); //  THETA'*VARPHI=TV
	for(int j=0;j<y_num;j++) // upsilon_num is defined as y_num
		upsilon[j]=Y->m[j][0] - TV->m[j][0];
//for(j=0;j<y_num;j++) log() <<"upsilon["<<j<<"]="<<upsilon[j];log() << "\n";

// update VARPHI 
	updateVARPHI();
//log() <<"VARPHI:" << "\n"; matrix->log()Matrix(VARPHI);

// compute lambda
	matrix->multiply(P,VARPHI,Pvarphi); // P*VARPHI=Pvarphi
	matrix->multiplyATB(VARPHI,Pvarphi,vPv); // VARPHI'*Pvarphi=vPv
	lambda= 1/ ( 1 + vPv->m[0][0] ) ;
}

////////////////////////////////////////////////////////////////////////////////
// 
// Class name:      elsClass  
// Member function: forecast2
// Purpose:         forecast y for the period after the next period
// Modified:        copy from rls
// Last Modified:   15.Jan.2000 Marc Mueller
//
////////////////////////////////////////////////////////////////////////////////
void elsClass::forecast2() {
//log() << "els forecast2" << "\n";
	int i,j,jj,ij;

// Forecast y_{t+1}
	matrix->multiplyATB(THETA,VARPHI,YF); // Forecast y_{t+1}
// as the Modell do not iterate Y is YF therefore FE is 0 and THETA keeps the same 
 
// compute upsilon, so that build VARPHI can use it! 
	//  TV = THETA' * VARPHI  == YF because THETA is unchanged
	for(j=0;j<y_num;j++) // upsilon_num is defined as y_num
		upsilon[j]=0; // because Y=YF == TV ;
		// in SG case upsilon is 0 because forecasterror FE is 0

// build VARPHI2 which is a updated VARPHI
	ij=varphi_num-1;
	for(i=varphi_var_num-1;i>-1;i--) { // for each pointer
		for(j=1;j<varphi_lagPtr[i];j++) { // shift if time lag >1
			for(jj=0;jj<varphi_numPtr[i];jj++) { // number of assets
				VARPHI2->m[ij][0]=VARPHI->m[ij-varphi_numPtr[i]][0];
				ij--;
			}
		}
		for(jj=varphi_numPtr[i]-1;jj>-1;jj--) { // number of assets
			VARPHI2->m[ij][0]=varphi[i][jj]; // set in asset jj
			// problem: model does not iterate; 
			// therefore old values are taken for controll variables!!!
			ij--; 
		}
	}
// set Y=YF in VARPHI2  this is all what we might know about future
	for(j=0;j<y_num;j++)  // number of assets
			VARPHI2->m[j][0]=YF->m[j][0]  ; // set in asset jj

// Forecast y_{t+2}
	matrix->multiplyATB(THETA,VARPHI2,YF); // Forecast
	for(j=0;j<y_num;j++)	// set forecast y_{t+2}
		y[j] = YF->m[j][0];
}

///////////////////////////////////////////////////////////////////////////////
//
// Class name:		sgClass
// Member function:	sgClass
// Purpose:		constructor
//
// Modified:        16.01.2000 Marc Mueller
// Last Modified:   22.01.2000 Marc Mueller
// 
///////////////////////////////////////////////////////////////////////////////
sgClass::sgClass( qreal* y0,int& y_num0,int& varphi_var_num0,
                    qreal** varphi0, int* varphi_numPtr0 )
	: elsClass::elsClass( y0,y_num0,varphi_var_num0,varphi0,varphi_numPtr0) {
// remark : given vectors must already include space for upsilon in varphi !!!
//          and full data in varphi_numPtr,varphi_lagPtr !!!
//log() << "sg constructor" << "\n";
	B = NULL;
	BI = NULL;
	ye = NULL;
	Kiid = NULL;
}

sgClass::sgClass(QDataStream& inFile,baseModel* const model)
	: elsClass::elsClass(inFile,model) {
//log() << "sg constructor stream" << "\n";
	B = NULL;
	BI = NULL;
	ye = NULL;
	Kiid = NULL;
}

////////////////////////////////////////////////////////////////////////////////
//
// Class name:      sgClass
// Member function: ~sgClass
// Purpose:         destructor
//
// Modified:        16.01.2000 Marc Mueller
// Last Modified:   22.01.2000 Marc Mueller
//
////////////////////////////////////////////////////////////////////////////////
sgClass::~sgClass() {
//log() << "sg destructor" << "\n";

	if( B ) delete B;
	if( BI ) delete BI;
	if( ye ) delete [] ye;
	if( Kiid ) {
        	for(int j=0;j<y_num;j++)
			if( Kiid[j] ) delete Kiid[j];
		delete Kiid;
	}

}

///////////////////////////////////////////////////////////////////////////////
//
// Class name:		sgClass
// Member function:	initialize
// Purpose:		initialize the system
//
// Modified:        16.01.2000 Marc Mueller
// Last Modified:   30.01.2000 Marc Mueller
// 
///////////////////////////////////////////////////////////////////////////////
void sgClass::initialize() {

//log() << "sg init 1" << "\n";
	elsClass::initialize();
//log() << "sg init 2" << "\n";

//	matrix->multiplyATB(VARPHI,VARPHI,vPv); // VARPHI' * VARPHI = vPv
	r = 1 ;//+ vPv->m[0][0] ;

	upsilonStart=-1; // idea: upsilon in period 1 is still 0 !!!

//log() <<"THETA:" << "\n"; matrix->log()Matrix(THETA);
//for(int j=0;j<y_num;j++) log() <<"upsilon["<<j<<"]="<<upsilon[j];log() << "\n";
//log() <<"VARPHI:" << "\n"; matrix->log()Matrix(VARPHI);
//log() << "r=" << r << "\n";
//log() << "alpha=" << alpha << "\n";
}

///////////////////////////////////////////////////////////////////////////////
//
// Class name:		sgClass
// Member function:	initializeUnbiasedForecast
// Purpose:		if unbiased forecast is used, this function
//                      initializes all additional things
//
// Modified:        Don Jan 20 CET 2000 Marc Mueller
// Last Modified:   2000/2/20 Marc Mueller
// 
///////////////////////////////////////////////////////////////////////////////
void sgClass::initializeUnbiasedForecast(const qreal& sigma,const qreal& epsilon,
                     qreal* p3, const qreal* start) {
//log() << "sg initializeUnbiasedForecast" << "\n";
	int i,j,ij,ii,jj;

	//   y^e_Init must be in position 2 in varphi and lag must be >= 1 !!!
	if(varphi_lagPtr[1]<1)
		fatalError("sgClass::initializeUnbiasedForecast ","lag of ye must be >=1");
	if((y_num!=varphi_numPtr[0])||(y_num!=varphi_numPtr[1]))
		fatalError("sgClass::initializeUnbiasedForecast ","mum size does not fit");

	if((sigma==0)&&(epsilon==0))
		bKiid=0;
	 else	{ 	// not
		bKiid=1;
		int lagMax=0;
		for(i=0;i<varphi_var_num;i++)
			lagMax=MAX(lagMax,varphi_lagPtr[i]);
		if(upsilonLag>0)
			lagMax=MAX(lagMax,(upsilonLag+1));
        //log() << "lagMax=" << lagMax << "\n";

        qreal epsval=1.0/(4.0*(y_num+2)*lagMax);
		if(epsilon > epsval ) {
            log()  <<"epsilon="<<epsilon<<" > "<< epsval << "\n";
			fatalError("sgClass::initializeUnbiasedForecast ","epsilon too high");
		}
        //log()  <<"epsilon="<<epsilon<<" <= "<<epsval<<"\n";

		eps2=epsilon/2;
		ttime=1;

		if( Kiid ) {
	        	for(j=0;j<y_num;j++)
				if( Kiid[j] ) delete Kiid[j];
			delete Kiid;
		}
		Kiid = new rand_var*[y_num];
		if( !(Kiid) )
			fatalError("sgClass::initializeUnbiasedForecast",
					"can not create Kiid");
	        for(j=0;j<y_num;j++) {
			Kiid[j] = new rand_var("ranf",1,-sigma,sigma);
			if( !(Kiid[j]) )
				fatalError("sgClass::initializeUnbiasedForecast",
					"can not create Kiid[]");
		}
	} // end  sigma!=0 epsilon!=0

	if( B ) delete B;
	if( BI ) delete BI;
	if( ye ) delete [] ye;

	B = new matrixDef(y_num,y_num);
	if( !B )
		fatalError("sgClass::initializeUnbiasedForecast ","B break");
	BI = new matrixDef(y_num,y_num);
	if( !BI )
		fatalError("sgClass::initializeUnbiasedForecast ","BI break");

	// B,BI will be set in function unbiasedForecast

	// initialize y^e in varphi by copying y^e_init into y^e_t+j for all j=1,..,q 
	ij=varphi_numPtr[0]*varphi_lagPtr[0]; // startpos of y^e in varphi
	//  all lags will be initialized with the same value!
	for(j=0;j<varphi_lagPtr[1];j++) { // time lag 
		for(jj=0;jj<varphi_numPtr[1];jj++) { // number of assets
			VARPHI->m[ij][0]=varphi[1][jj]; // set in asset jj
			ij++;
		}
	}
    //log() <<"sgClass::initializeUnbiasedForecast VARPHI:" << "\n"; matrix->log()Matrix(VARPHI);

    ye = new qreal[y_num];
	if( !ye )
		fatalError("sgClass::initializeUnbiasedForecast ","ye break");
	for(j=0;j<y_num;j++) 
		ye[j]=varphi[1][j];
	varphi[1] = ye; // overwrite model->setLabels(varname) with ye vector

	if(p3) tPointer=p3; // use this as third pointer 
	 else	 tPointer=NULL;

	if(start[0]>0) { // in start[0] is number of theta values given
		if(start[0]!=(varphi_num*y_num))
			fatalError("sgClass::initializeUnbiasedForecast -","wrong start number size!");
		ii=0;
		for(j=0;j<y_num;j++)
			for(i=0;i<varphi_num;i++)
				THETA->m[i][j]=start[++ii];
	}
	else	{ // no start values for theta; 
		ij=varphi_numPtr[0]*varphi_lagPtr[0]; // startpos of y^e in varphi
		for(j=0;j<y_num;j++)  
			THETA->m[ij+j][j]=1; // this makes computation of forecast work
	}// end start values
//log() <<"initunbF  THETA:" << "\n"; matrix->log()Matrix(THETA);
}

///////////////////////////////////////////////////////////////////////////////
//
// Class name:		sgClass
// Member function:	estimate
// Purpose:		estimate 
//
// Modified:        16.01.2000 Marc Mueller
// Last Modified:   30.01.2000 Marc Mueller
// 
///////////////////////////////////////////////////////////////////////////////
void sgClass::estimate() {
	int j;
// compute THETA recursive
	updateY();
	matrix->multiplyATB(THETA,VARPHI,YF); //  THETA'*VARPHI=YF Forecast for this period
	matrix->subtract(Y,YF,FE); //  Y-YF=FE Forecast Error
// compute r (can not be done after update VARPHI, because of unbiased forecast!!!)
	matrix->multiplyATB(VARPHI,VARPHI,vPv); // VARPHI' * VARPHI = vPv
	r+= vPv->m[0][0] ;
//log() << "r=" << r << "\n";
	matrix->multiplyScalar(alpha/r,VARPHI,Pvarphi);
	matrix->multiplyABT(Pvarphi,FE,TEC); //  
	matrix->add(THETA,TEC,THETA); //  THETA+TEC=THETA

// set upsilon
	if(upsilonStart) // idea: upsilon in period 1 is still 0 !!!
		upsilonStart++;
	 else
		for(j=0;j<y_num;j++) // upsilon_num is defined as y_num
			upsilon[j]=FE->m[j][0];

//for(j=0;j<y_num;j++) log() <<"upsilon["<<j<<"]="<<upsilon[j];log() << "\n";

// update VARPHI 
	updateVARPHI();
//log() <<"VARPHI:" << "\n"; matrix->log()Matrix(VARPHI);
}

////////////////////////////////////////////////////////////////////////////////
// 
// Class name:      sgClass  
// Member function: unbiasedForecast
// Purpose:         
// Modified:        20.Jan.2000 Marc Mueller
// Last Modified:   20.Feb.2000 Marc Mueller
//
////////////////////////////////////////////////////////////////////////////////
void sgClass::unbiasedForecast(int fp) {
//log() << "sg unbiasedForecast" << "\n";
/*
qreal R=1.01 ;
qreal a=0.65 ;
qreal beta=1.04 ;
qreal L=2;
qreal tau1x=((0.49/R)+0.01)*1.001;


THETA->m[0][0]=(1-a)*beta/(L*R);
THETA->m[1][0]=(1-a)*beta/(L*R);
THETA->m[2][0]=a/R;
THETA->m[3][0]=1/R;
THETA->m[4][0]=-tau1x/R;
*/
	int i,j,ij,jj;
    qreal powTimeEps;

	ij=varphi_numPtr[0]*varphi_lagPtr[0]; // pos of y^e
	jj=ij+varphi_numPtr[1]*(varphi_lagPtr[1]-1); // pos of last y^e
	for(i=0;i<y_num;i++) {
		YF->m[i][0]=VARPHI->m[ij+i][0]; // save first ye
		VARPHI->m[ij+i][0]=0; // set first null
	}
//log() <<"VARPHI:" << "\n"; matrix->log()Matrix(VARPHI);
//log() <<"yeSav:" << "\n"; matrix->log()Matrix(YF);

	for(i=0;i<y_num;i++)
		for(j=0;j<y_num;j++)
			B->m[j][i]=THETA->m[i+ij][j]; // copy B1 from THETA into dummy B
//log() <<"B:" << "\n"; matrix->log()Matrix(B);

	matrix->inverse(B,BI); // BI=B^-1
//log() <<"BI:" << "\n"; matrix->log()Matrix(BI);

	if(tPointer) {
		jj=ij+varphi_numPtr[1]*varphi_lagPtr[1]; // pos of divident
		for(i=0;i<varphi_numPtr[2];i++)
			VARPHI->m[jj+i][0]=tPointer[i];  // set in ediv
//log() <<"VARPHI:" << "\n"; matrix->log()Matrix(VARPHI);
		matrix->multiplyATB(THETA,VARPHI,FE);
		for(i=0;i<varphi_numPtr[2];i++)
			VARPHI->m[jj+i][0]=varphi[2][i]; // set in divident
//log() <<"VARPHI:" << "\n"; matrix->log()Matrix(VARPHI);
	}
	else	matrix->multiplyATB(THETA,VARPHI,FE); // no tPointer
//log() <<"FE:" << "\n"; matrix->log()Matrix(FE);

	matrix->subtract(YF,FE,FE); // FE = YF - FE  
//log() <<"FE:" << "\n"; matrix->log()Matrix(FE);

	matrix->multiply(BI,FE,YF);// Forecast
//log() <<"YF:" << "\n"; matrix->log()Matrix(YF);

	if(bKiid) {
		ttime++;

		powTimeEps = pow(log(double(ttime)),eps2);

		for(j=0;j<y_num;j++) {	// copy newest forecast t+q into vector ye
			b_t=( Kiid[j]->dice() );
log() << 1/powTimeEps << "  "<< b_t;
			b_t/=powTimeEps;
log()  << "  " <<  b_t << "\n";
			ye[j] = YF->m[j][0] + b_t;
		}
	}
	else	for(j=0;j<y_num;j++)	// copy newest forecast t+q into vector ye
			ye[j] = YF->m[j][0];

//  updating y^e_t+q in varphi (pos is ij) ,because it was unknown while updating in estimate part
	for(j=0;j<y_num;j++)
		VARPHI->m[ij+j][0]=ye[j];
//log() <<"VARPHI:" << "\n"; matrix->log()Matrix(VARPHI);
// set in Forecast
	jj=ij+y_num*(varphi_lagPtr[1]-fp); // pos of fp in y^e 
	for(j=0;j<y_num;j++)
		y[j] = VARPHI->m[jj+j][0]; // set in Forecast
}
