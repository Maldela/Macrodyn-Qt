/******************************************************************************/
/*                                                                            */
/* Module name:  printer.C                                                    */
/* Contents:     Member functions of the classes                              */
/*               1) printer (virtual base class)                              */
/*               2) pclPrinter                                                */
/*               3) psPrinter                                                 */
/*               4) deskjet                            		              */
/*               5) XPMPrinter                                                */
/* Last modified: Sat Jun 19 14:37:23 CEST 1999 (Marc Mueller)                */
/*                                                                            */
/******************************************************************************/

#include "printer.h"
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <stdlib.h>  // for function abs()

XColor printer::parsi;
const int y_dimension=300;
int x_dimension=600;

extern void fatalError(const char*, const char*); // error handler
extern baseModel* model; /*test*/



/******************************************************************************/
/*                                                                            */
/* Class name:      printer                                                   */
/* Member function: printer                                                   */
/* Purpose:         constructor 1                                             */
/* Last modified:   17.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

printer::printer(const resolution myRes,const size& pSize,const char* name,
                 const xyRange& range) 
        :  pageSize(pSize),dpi(myRes)
{
    limits = new xyRange(range);
    if( !limits )
	fatalError("printer::printer","Can't allocate memory");
    strcpy(fileName,name);
    xDim=limits->res[0];
    yDim=limits->res[1];
    rows=yDim/8;		        // 8 bits are a byte, storing bytes
    getMatrix();			// alloacte memory for the print image
}
/******************************************************************************/
/*                                                                            */
/* Class name:      printer                                                   */
/* Member function: printer                                                   */
/* Purpose:         constructor 2                                             */
/* Last modified:   14.01.1997 (Mueller, Nikutta)                             */
/*                                                                            */
/******************************************************************************/

printer::printer()
{
}
 
/******************************************************************************/
/*                                                                            */
/* Class name:      printer                                                   */
/* Member function: printer                                                   */
/* Purpose:         constructor                                               */
/* Last modified:   17.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

printer::printer(const char* name, const xyRange& range)
{
    limits = new xyRange(range);
    if( !limits )
	fatalError("printer::printer","Can't allocate memory");
    strcpy(fileName,name);
    dpi=high;
    xDim=limits->res[0];
    yDim=limits->res[1];
    rows=yDim/8;		        // 8 bits are a byte, storing bytes
					// saves memory!
    getMatrix();			// alloacte memory for the print image
}

/******************************************************************************/
/*                                                                            */
/* Class name:      printer                                                   */
/* Member function: ~printer                                                  */
/* Purpose:         destructor                                                */
/* Last modified:   17.10.1994 (Markus Lohmann)                               */
/* Last modified:   Sat Jun 19 19:51:58 CEST 1999  (Marc Mueller)             */
/*                                                                            */
/******************************************************************************/

printer::~printer()
{
//cout << "printer::~printer" << endl;
    if( graphMatrix ) {
        for( int i=0; i< rows; i++ )
	     if( graphMatrix[i] )
                delete [] graphMatrix[i];		// free allocated memory
        delete [] graphMatrix;
//cout << "printer::~printer1" << endl;
    }
    if( limits ) {
//cout << "printer::~printer limits" << endl;
	delete limits;
   }
//cout << "printer::~printer DONE" << endl;
	// free color matrix
//	if ( graphMatrixColor )
//		delete [] graphMatrixColor;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      printer                                                   */
/* Member function: getMatrix                                                 */
/* Purpose:         allocate memory to hold the graphics image                */
/* Last modified:   17.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void printer::getMatrix()
{
    int j;

    graphMatrixColor=new int[95]; // 32 colors are hardcoded jet
    if( graphMatrixColor == NULL ) 
        fatalError("printer::getMatrix","Can't allocate memory");

    for(j=0;j<95;j++)
        graphMatrixColor[j]=0;

    graphMatrix=new unsigned char*[rows];
    if( graphMatrix == NULL ) 
        fatalError("printer::getMatrix","Can't allocate memory");


    for(int i=0; i<rows; i++) {
        graphMatrix[i]=new unsigned char[xDim];
        if( graphMatrix[i] == NULL ) 
            fatalError("printer::getMatrix","Can't allocate memory");
    
        for(j=0;j<xDim;j++)
            graphMatrix[i][j]=0;
    }
}

/******************************************************************************/
/*                                                                            */
/* Class name:      printer                                                   */
/* Member function: pageSetup                                                 */
/* Purpose:         setup resolution and page size                            */
/* Last modified:   17.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void printer::pageSetup(const size& sizeToSet,const resolution& resToSet)
{
    pageSize=sizeToSet;
    dpi=resToSet;

    unsigned char divisor= dpi == high ? 1 : 2;
    
    switch( pageSize ) {
        case A4:  xDim=5400 / divisor;	// 5400 dots fit on a landscape A4 page
                  yDim=3600 / divisor;	// 3600 dots in y-direction
                  break;
	case A4_2:xDim=2700 / divisor;
                  yDim=1800 / divisor;
                  break;
    }
    rows=yDim/8;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      printer                                                   */
/* Member function: reset                                                     */
/* Purpose:         reset the matrix that holds the graphics image            */
/* Last modified:   17.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void printer::reset()
{
    int j;
    for(int i=0; i < rows; i++)
        for(j=0; j < xDim; j++)
            graphMatrix[i][j]=0;
}


/******************************************************************************/
/*                                                                            */
/* Class name:      printer                                                   */
/* Member function: setResolution                                             */
/* Purpose:         change resolution of the output                           */
/* Last modified:   17.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void printer::setResolution(const resolution& resToSet)
{
    pageSetup(pageSize,resToSet);
}

/******************************************************************************/
/*                                                                            */
/* Class name:      printer                                                   */
/* Member function: setSize                                                   */
/* Purpose:         change paper size                                         */
/* Last modified:   17.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void printer::setSize(const size& sizeToSet)
{
    pageSetup(sizeToSet,dpi);
}

/******************************************************************************/
/*                                                                            */
/* Class name:      printer                                                   */
/* Member function: setLimits                                                 */
/* Purpose:         set domain under consideration (part of the state space)  */
/*                  2D domain                                                 */
/* Last modified:   17.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void printer::setLimits(const xyRange& toSet)
{
    *limits=toSet;
}

////////////////////////////////////////////////////////////////////////////////
//
// member function:	set_axis
// purpose:		set min and max values of axis
//
// author:		Andreas Starke
// last modified:	
//
////////////////////////////////////////////////////////////////////////////////

void printer::set_axis(int which, real to_min, real to_max)
{
	limits->min[which]=to_min;
	limits->max[which]=to_max;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      printer                                                   */
/* Member function: isOk                                                      */
/* Purpose:         determine wether the point to be printed lies in the      */
/*                  domain under consideration or not (2D domain)             */
/* Last modified:   17.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

unsigned char printer::isOk(real& x, real& y)
{
    unsigned char ok=0;

    ok= (x >= limits->min[0]) && (x <= limits->max[0]) &&
	(y >= limits->min[1]) && (y <= limits->max[1]);

    return ok;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      printer                                                   */
/* Member function: isOk                                                      */
/* Purpose:         determine wether the point to be printed lies in the      */
/*                  domain under consideration or not (3D domain)             */
/* Last modified:   17.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

unsigned char printer::isOk(real& x, real& y, real& z)
{
    unsigned char ok=isOk(x,y);

    ok = ok && (z >= limits->min[2]) && 
	       (z <= limits->max[2]);

    return ok;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      printer                                                   */
/* Member function: computeBits                                               */
/* Purpose:         compute bits to be set in the matrix                      */
/* Last modified:   11.01.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void printer::computeBits(real& x,real& y,int& row, int& col)
{
    if( !isOk(x,y) ) 
        return;
    col = (int)((x-limits->min[0]) * (xDim-1)/(limits->max[0]-limits->min[0]));
    row = (int)((y-limits->min[1]) * (yDim-1)/(limits->max[1]-limits->min[1]));
}

/******************************************************************************/
/*                                                                            */
/* Class name:      printer                                                   */
/* Member function: setBits                                                   */
/* Purpose:         set bits in the matrix that holds the graphics image (2D) */
/* Last modified:   11.01.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void printer::setBits(real& x, real& y)
{
    int row;
    int col;
    int rowDiv;
    int rowMod;
    
    computeBits(x,y,row,col);

    rowDiv=row / 8;
    rowMod=row % 8;
    if( (rowDiv <0) || (rowDiv >= rows) )
	  return;//tw
	  //fatalError("printer::setBits","Error computing row");
    if( (col <0) || (col >= xDim) ) 
	  return;//tw
	  //fatalError("printer::setBits","Error computing column");

    graphMatrix[rowDiv][col] |= (1 << (7-rowMod));
}

void printer::setBits(real& x, real& y,int)
{
    setBits(x,y);
}
/******************************************************************************/
/*                                                                            */
/* Class name:      printer                                                   */
/* Member function: setBit  (matrix coordinates)                              */
/* Purpose:         set a bit in the matrix that holds the graphics image (2D)*/
/* Last modified:   Mon Nov 10 15:44:15 MET 1997 (Marc Mueller)               */
/*                                                                            */
/******************************************************************************/

void printer::setBit(int& x, int& y,short color)
{
    if( color > 94 )
	color=94;       
    
    if( (y <0) || (y >= yDim) ){
	  printf("Error computing row %i >= %d\n x=%i, y=%i\n", y+1, yDim,x,y);
	  fatalError("Printer::setBit","Error computing row");
	  }
    if( (x <0) || (x >= xDim) ) 
	  fatalError("Printer::setBit","Error computing column");
    graphMatrix[y][x]=(char) color+95;
    graphMatrixColor[color]=1;	// sign which color is used
}

/******************************************************************************/
/*                                                                            */
/* Class name:      printer                                                   */
/* Member function: setBigPoint  (matrix coordinates)                         */
/* Purpose:         set a bit in the matrix that holds the graphics image (2D)*/
/* Last modified:   Andreas Starke 30.04.2004		                      */
/*                                                                            */
/******************************************************************************/

void printer::setBigPoint(const real& x, const real& y,int color, int& size)
{
	// not for general use
}

/******************************************************************************/
/*                                                                            */
/* Class name:      printer                                                   */
/* Member function: computeBits                                               */
/* Purpose:         compute bits to be set in the matrix                      */
/* Last modified:   11.01.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void printer::computeBits(real& x,real& y,real& z,int& row, int& col)
{
    row = (int)((z-limits->min[2])*0.75*(yDim-1) / 
		 (limits->max[2]-limits->min[2]));
    row+= (int)((y-limits->min[1])*0.25*(yDim-1) /
		(limits->max[1]-limits->min[1]));

    col = (int)((x-limits->min[0]) * 0.75*(xDim-1) /
		(limits->max[0]-limits->min[0]));
    col+= (int)((y-limits->min[1]) * 0.25*(yDim-1) /
		(limits->max[1]-limits->min[1]));
}

/******************************************************************************/
/*                                                                            */
/* Class name:      printer                                                   */
/* Member function: setBits                                                   */
/* Purpose:         set bits in the matrix that holds the graphics image (3D) */
/* Last modified:   17.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void printer::setBits(real& x,real& y,real& z)
{
    int rowDiv;
    int rowMod;
    int row;
    int col;

    computeBits(x,y,z,row,col);
    
    rowDiv=row / 8;
    rowMod=row % 8;
    
    graphMatrix[rowDiv][col] |= (1 << (7-rowMod));
}
/****************************************************************************/
/*                                                                          */
/* Class name:      printer                                                 */
/* Member function: drawLine                                                */
/* Purpose:         draw a line (Bresenham-Algorithmus)                     */
/* Last modified:   18.01.2005  (Andreas Starke) 		            */
/*                                                                          */
/****************************************************************************/
void printer::drawLine(const real& ax, const real& ay,
                       const real& bx, const real& by, short color)
{
	short error,delta,schwelle,dx,dy,inc_x,inc_y;
	int x0,y0,x1,y1;

	real Ax,Ay,Bx,By;
	Ax=ax;
	Ay=ay;
	Bx=bx;
	By=by;
	if ( (ax>limits->max[0]) || (bx>limits->max[0]) || (ax<limits->min[0]) || (bx<limits->min[0])
		 || (ay>limits->max[1]) || (by>limits->max[1]) || (ay<limits->min[1]) || (by<limits->min[1]) ) {
	if ( (ay>limits->max[1]) && (by<=limits->max[1]) ){
		real a,b;
		a=ay;
		b=(by-ay)/(bx-ax);
		Ax=(limits->max[1]-a)/b+ax;
		Ay=limits->max[1];
	}
	if ( (by>limits->max[1]) && (ay<=limits->max[1]) ){
		real a,b;
		a=ay;
		b=(by-ay)/(bx-ax);
		Bx=(limits->max[1]-a)/b+ax;
		By=limits->max[1];
	}	
	if ( (ay<limits->min[1]) && (by>=limits->min[1]) ){
		real a,b;
		a=ay;
		b=(by-ay)/(bx-ax);
		Ax=(limits->min[1]-a)/b+ax;
		Ay=limits->min[1];
	} 
	if ( (by<limits->min[1]) && (ay>=limits->min[1]) ){
		real a,b;
		a=ay;
		b=(by-ay)/(bx-ax);
		Bx=(limits->min[1]-a)/b+ax;
		By=limits->min[1];
	}
	if ( (ax>limits->max[0]) && (bx<=limits->max[0]) ){
		real a,b;
		a=ax;
		b=(bx-ax)/(by-ay);
		Ay=(limits->max[0]-a)/b+ay;
		Ax=limits->max[0];
	}
	if ( (bx>limits->max[0]) && (ax<=limits->max[0]) ){
		real a,b;
		a=ax;
		b=(bx-ax)/(by-ay);
		By=(limits->max[0]-a)/b+ay;
		Bx=limits->max[0];
	}
	if ( (ax<limits->min[0]) && (bx>=limits->min[0]) ){
		real a,b;
		a=ax;
		b=(bx-ax)/(by-ay);
		Ay=(limits->min[0]-a)/b+ay;
		Ax=limits->min[0];
	} 
	if ( (bx<limits->min[0]) && (ax>=limits->min[0]) ){
		real a,b;
		a=ax;
		b=(bx-ax)/(by-ay);
		By=(limits->min[0]-a)/b+ay;
		Bx=limits->min[0];
	}
	}



        if( (!isOk(Ax,Ay)) || (!isOk(Bx,By)) )  return;

        computeBits(Ax,Ay,y0,x0);
        computeBits(Bx,By,y1,x1);	
	dx=x1-x0;
	dy=y1-y0;
	if(dx>0) inc_x=1; else inc_x=-1;
	if(dy>0) inc_y=1; else inc_y=-1;
	if(abs(dy)<abs(dx)) {
		error=-abs(dx);
		delta=2*abs(dy);
		schwelle=2*error;
		while(x0!=x1) {
			setBit(x0,y0,color);
			x0+=inc_x;
			error=error+delta;
			if(error>0) {
				y0+=inc_y;
				error=error+schwelle;
			}
		}
	} else
	{
		error=-abs(dy);
		delta=2*abs(dx);
		schwelle=2*error;
		while (y0!=y1) {
			setBit(x0,y0,color);
			y0+=inc_y;
			error=error+delta;
			if(error>0) {
				x0+=inc_x;
				error=error+schwelle;
			}
		}
	}
}

/******************************************************************************/
/*                                                                            */
/* Class name:      xpm                                                       */
/* Member function: computeBits                                               */
/* Purpose:         compute bits to be set in the matrix   (uses "adjusting") */
/* Last modified:   21.01.1997 (Mueller, Nikutta)                             */
/*                                                                            */
/******************************************************************************/
void xpmPrinter::computeBits(real& x,real& y,int& row, int& col)
{
    row = (int)(0.5+(y-limits->min[1])*(yDim-1)/(limits->max[1]-limits->min[1]));
    col = (int)(0.5+(x-limits->min[0])*(xDim-1)/(limits->max[0]-limits->min[0]));
}

void xpmPrinter::computeBits(real& x,real& y,real &z,int& row, int& col)
{

    row = (int)((z-limits->min[2])*0.75*(yDim-1) / 
		 (limits->max[2]-limits->min[2]));
    row+= (int)((y-limits->min[1])*0.25*(yDim-1) /
		(limits->max[1]-limits->min[1]));

    col = (int)((x-limits->min[0]) * 0.75*(xDim-1) /
		(limits->max[0]-limits->min[0]));
    col+= (int)((y-limits->min[1]) * 0.25*(yDim-1) /
		(limits->max[1]-limits->min[1]));
}

/******************************************************************************/
/*                                                                            */
/* Class name:      xpmPrinter                                                */
/* Member function: getMatrix                                                 */
/* Purpose:         allocate memory to hold the graphics image                */
/* Last modified:   03.06.1997 (Marc Mueller)                                 */
/*                                                                            */
/******************************************************************************/
void xpmPrinter::getMatrix()
{
    int j,i;

    graphMatrix=new unsigned char*[rows];
    if( graphMatrix == NULL ) 
        fatalError("printer::getMatrix","Can't allocate memory for graphMatrix");

    for(i=0; i<rows; i++)
    	{
        graphMatrix[i]=new unsigned char[xDim];
        if( graphMatrix[i] == NULL ) 
            fatalError("xpmPrinter::getMatrix","Can't allocate memory");
    
        for(j=0;j<xDim;j++)
            graphMatrix[i][j]=95; // color of non set area! (oor color)
	}

    graphMatrixColor=new int[95]; // 32 colors are hardcoded jet
    if( graphMatrixColor == NULL ) 
        fatalError("printer::getMatrix","Can't allocate memory for graphMatrixColor");

    for(i=0; i<95; i++)
    	{
	graphMatrixColor[i]=0;	//clear sign of color use
	}
}

/******************************************************************************/
/*                                                                            */
/* Class name:      xpmPrinter                                                */
/* Member function: xpmPrinter                                                */
/* Purpose:         constructor                                               */
/* Last modified:   Sat Jun 19 12:41:51 CEST 1999  Marc Mueller               */
/*                                                                            */
/******************************************************************************/
xpmPrinter::xpmPrinter(const char* name,const xyRange& range,
			char ** const mpPtrs,const int&  mp_num,
			const int& jT) : printer() // use empty printer
			 // constructor because of other rows number for matrix
{
  jobType=jT;
  limits = new xyRange(range);
    if( !limits )
	fatalError("xpmPrinter::xpmPrinter","Can't allocate memory");
  strcpy(fileName,name);
  xDim=limits->res[0];
  yDim=limits->res[1];
  rows=yDim;
  
  getMatrix();			// alloacte memory for the print image    

  if(mpPtrs) {
	multiplotPtrs=mpPtrs;
	multiplot_num=mp_num;
  }
  else  multiplotPtrs=NULL;

  if(limits->zeroline[1]>-1) {
    drawLine(limits->min[0],0,limits->max[0],0,24);
  }
}

/******************************************************************************/
/*                                                                            */
/* Class name:      xpmPrinter                                                */
/* Member function: xpmPrinter                                                */
/* Purpose:         constructor2 for screenshot                               */
/* Last modified:   12.08.2003 Andreas Starke			              */
/*                                                                            */
/******************************************************************************/
xpmPrinter::xpmPrinter(const xyRange& range, const int& jT) : printer() // use empty printer
{
  jobType=jT;
  limits = new xyRange(range);
    if( !limits )
	fatalError("xpmPrinter::xpmPrinter","Can't allocate memory");
  xDim=limits->res[0];
  yDim=limits->res[1];
  rows=yDim;
  
  getMatrix();			// allocate memory for the print image    
}


/******************************************************************************/
/*                                                                            */
/* Class name:      xpmPrinter                                                */
/* Member function: xpmPrinter                                                */
/* Purpose:         destructor                                                */
/* Last modified:   Sat Jun 19 19:51:58 CEST 1999   Marc Mueller              */
/*                                                                            */
/******************************************************************************/
xpmPrinter::~xpmPrinter() {
// delete graphMatrix,limits will be done in the printer destructor!
}

/******************************************************************************/
/*                                                                            */
/* Class name:      xpmPrinter                                                */
/* Member function: setBits                                                   */
/* Purpose:         set bits in the matrix that holds the graphics image (2D) */
/* Last modified:   11. Aug.1997 (Nattkemper, Nikutta)                        */
/*                                                                            */
/******************************************************************************/
void xpmPrinter::setBits(real& x, real& y,int color)
{
    int row;
    int col;

    if( color > 94 )
	color=94;
    
    // if the right boundary is not hit exactly one needs to cut to back to the max
    // this problem occurs since the loops are made in real-Variables!!
    if ((x >= xDim) && (x < xDim+(limits->max[0]-limits->min[0])/(2*limits->res[0])))
    	x = limits->max[0];
    if ((y >=yDim) &&  (y < yDim+(limits->max[1]-limits->min[1])/(2*limits->res[1])))
    	y = limits->max[1];
       
    if( !isOk(x,y) ) 
        return;

    computeBits(x,y,row,col);
    
    
 //   printf("printer(+1): (%d,%d)\n",col+1,row+1); //TEST
 	   
 	  
    if( (row <0) || (row >= yDim) ){
	  //return;
	  printf("Error computing row %d >= %d\n x=%f, y=%f\n", row+1, yDim,x,y);
	  fatalError("xpmPrinter::setBits","Error computing row");}
    if( (col <0) || (col >= xDim) ) 
	  //return;
	  fatalError("xpmPrinter::setBits","Error computing column");
    graphMatrix[row][col]=(char) color+95;
/*
    graphMatrix[row-1][col-1]=(char) color+95;
    graphMatrix[row][col-1]=(char) color+95;
    graphMatrix[row+1][col-1]=(char) color+95;
    graphMatrix[row-1][col+1]=(char) color+95;
    graphMatrix[row][col+1]=(char) color+95;
    graphMatrix[row+1][col+1]=(char) color+95;
    graphMatrix[row-1][col]=(char) color+95;
    graphMatrix[row+1][col]=(char) color+95;
*/
    graphMatrixColor[color]=1;	// sign which color is used
}

/******************************************************************************/
/*                                                                            */
/* Class name:      xpmPrinter                                                */
/* Member function: setBigPoint  (matrix coordinates)                         */
/* Purpose:         set a bit in the matrix that holds the graphics image (2D)*/
/* Last modified:   Andreas Starke 30.04.2004		                      */
/*                                                                            */
/******************************************************************************/

void xpmPrinter::setBigPoint(const real& x, const real& y,int color, int& size)
{
	int row, col;
	real x_d, y_d;
	if( color > 94 )
		color=94;
	x_d = x;
	y_d = y;	
	computeBits(x_d,y_d,row,col);	
	
	for ( int i=0; i<size; i++)
		for ( int j=0; j<size; j++){
			if( !((row-i <0) || (row-i >= yDim)) )
			if( !((col+j <0) || (col+j >= xDim)) ){
			graphMatrix[row-i][col+j] = (char) color+95;
			graphMatrixColor[color] = 1;
			}
		}
}


/******************************************************************************/
/*                                                                            */
/* Class name:      xpmPrinter                                                */
/* Member function: setBits                                                   */
/* Purpose:         set bits in the matrix that holds the graphics image (3D) */
/* Last modified:   11. Aug.1997 (Nikutta)                                    */
/*                                                                            */
/******************************************************************************/
void xpmPrinter::setBits(real& x, real& y,real &z)
{
    int row;
    int col;
    int i;
 
 // if the right boundary is not hit exactly one needs to cut to back to the max
    // this problem occurs since the loops are made in real-Variables!!
    if ((x >= xDim) && (x < xDim+(limits->max[0]-limits->min[0])/(2*limits->res[0])))
    	x = limits->max[0];
    if ((y >=yDim) &&  (y < yDim+(limits->max[1]-limits->min[1])/(2*limits->res[1])))
    	y = limits->max[1];
       
        
    if( !isOk(x,y) ) 
        return;
        
        
    computeBits(x,y,z,row,col);
    if( (row <0) || (row >= yDim) )
	  //return;
	  fatalError("xpmPrinter::setBits","Error computing row");
    if( (col <0) || (col >= xDim) ) 
	  //return;
	  fatalError("xpmPrinter::setBits","Error computing column");
    graphMatrix[row][col]=(char) 95+DEFAULT_XPM_COLOR;
    i=DEFAULT_XPM_COLOR;
    graphMatrixColor[i]=1;
}
void xpmPrinter::setBits(real& x, real& y)
{
    int color =DEFAULT_XPM_COLOR;
    setBits(x,y, color);
}

/******************************************************************************/
/*                                                                            */
/* Class name:      xpmPrinter                                                */
/* Member function: printToFile                                               */
/* Purpose:         set bits in the matrix that holds the graphics image (2D) */
/* Last modified:   Sat Jun 19 14:37:23 CEST 1999  (Marc Mueller)             */
/*                                                                            */
/******************************************************************************/
void xpmPrinter::printToFile()
{
    char *colorCodes[95];
        colorCodes[0]= "FFFFFF\0";

        if((jobType==D_BIF2D)||(jobType==D_ATTRA)||(jobType==D_BIFNPARAM))
          {
		colorCodes[1]="191970\0";	//"MidnightBlue";					 
		colorCodes[2]="00007D\0";	//"NavyBlue";
		colorCodes[3]="6495ED\0";	//"CornflowerBlue";
		colorCodes[4]="483D8B\0";	//"DarkSlateBlue";	
 		colorCodes[5]="6A5ACD\0";	//"SlateBlue";	
		colorCodes[6]="7B68EE\0";	//"MediumSlateBlue"; 
		colorCodes[7]="8470FF\0";	//"LightSlateBlue";
		colorCodes[8]="0000CD\0";	//"MediumBlue"; 
		colorCodes[9]="4169E1\0";	//"RoyalBlue";
		colorCodes[10]="0000FF\0";	//"Blue";
		colorCodes[11]="1D90FF\0";	//"DodgerBlue";
		colorCodes[12]="00BFFF\0";	//"DeepSkyBlue"; 
		colorCodes[13]="87CEEB\0";	//"SkyBlue"; 
		colorCodes[14]="87CEFA\0";	//"LightSkyBlue";
		colorCodes[15]="4682B4\0";	//"SteelBlue"; 
		colorCodes[16]="B0C4DE\0";	//"LightSteelBlue";	
		colorCodes[17]="ADD8E6\0";	//"LightBlue";
		colorCodes[18]="B0E0E6\0";	//"PowderBlue";	
		colorCodes[19]="AFEEEE\0";	//"PaleTurquoise";
		colorCodes[20]="00CED1\0";	//"DarkTurquoise";
		colorCodes[21]="48D1CC\0";	//"MediumTurquoise";
		colorCodes[22]="40E0D0\0";	//"Turquoise";
		colorCodes[23]="00FFFF\0";	//"cyan"; 
		colorCodes[24]="E0FFFF\0";	//"LightCyan";
		colorCodes[25]="5F9EA0\0";	//"CadetBlue";
		colorCodes[26]="66CD11\0";	//"MediumAquamarine"
		colorCodes[27]="7FFFD4\0";	//"Aquamarine";
		colorCodes[28]="006400\0";	//"DarkGreen";
		colorCodes[29]="556B2F\0";	//"DarkOliveGreen";
		colorCodes[30]="8FBC57\0";	//"DarkSeaGreen";
		colorCodes[31]="548B57\0";	//"SeaGreen";
		colorCodes[32]="32B371\0";	//"MediumSeaGreen";
		colorCodes[33]="20B2AA\0";	//"LightSeaGreeen";
		colorCodes[34]="98FB98\0";	//"PaleGreen";
		colorCodes[35]="00FF7F\0";	//"SpringGreen";
		colorCodes[36]="7CFC00\0";	//"LawnGreen";
		colorCodes[37]="00FF00\0";	//"green";
		colorCodes[38]="7FFF00\0";	//"chartreuse";	
		colorCodes[39]="00FA9A\0";	//"MediumSpringGreen";
		colorCodes[40]="ADFF2F\0";	//"GreenYellow";
		colorCodes[41]="32CD32\0";	//"LimeGreen";
		colorCodes[42]="9ACD32\0";	//"YellowGreen";
		colorCodes[43]="228B22\0";	//"ForestGreen";
		colorCodes[44]="6B8E23\0";	//"OliveDrab";
		colorCodes[45]="BDB76B\0";	//"DarkKhaki";
		colorCodes[46]="F0E68C\0";	//"Khaki";
		colorCodes[47]="EEE8AA\0";	//"PaleGoldenrod";
		colorCodes[48]="FAFAD2\0";	//"LightGoldenrodYellow";
		colorCodes[49]="FFFFE0\0";	//"LightYellow";
		colorCodes[50]="FFFF00\0";	//"Yellow";
		colorCodes[51]="FF8D00\0";	//"gold";
		colorCodes[52]="EEC382\0";	//"LightGoldenrod";	
		colorCodes[53]="DAA520\0";	//"goldenrod";	
		colorCodes[54]="B8860B\0";	//"DarkGoldenRod";
		colorCodes[55]="BC8F8F\0";	//"RosyBrown";
		colorCodes[56]="CD5C5C\0";	//"IndianRed";	
		colorCodes[57]="8B4513\0";	//"SaddleBrown";
		colorCodes[58]="A05253\0";	//"sienna";	
		colorCodes[59]="CD853F\0";	//"peru";
		colorCodes[60]="DEB887\0";	//"burlywood";
		colorCodes[61]="F5F5DC\0";	//"beige";
		colorCodes[62]="F5DEB3\0";	//"wheat";	
		colorCodes[63]="F4A460\0";	//"Sandybrown";	
		colorCodes[64]="D2B48C\0";	//"tan";
		colorCodes[65]="D2691E\0";	//"chocolate";
		colorCodes[66]="B22222\0";	//"firebrick";
		colorCodes[67]="A52A2A\0";	//"brown";
		colorCodes[68]="E9967A\0";	//"DarkSalmon";
		colorCodes[69]="FA8072\0";	//"salmon";
		colorCodes[70]="FFA07A\0";	//"LightSalmon";
		colorCodes[71]="FFA500\0";	//"Orange";	
		colorCodes[72]="FF8C00\0";	//"DarkOrange";	
		colorCodes[73]="FF7F50\0";	//"coral";
		colorCodes[74]="F08080\0"; 	//"Lightcoral"	
		colorCodes[75]="FF6347\0";	//"tomato";	
		colorCodes[76]="FF4500\0";	//"OrangeRed";
		colorCodes[77]="FF0000\0";	//"red";
		colorCodes[78]="FF69B4\0";	//"HotPink";
		colorCodes[79]="FF1493\0";	//"DeepPink";
		colorCodes[80]="FFC0CB\0";	//"pink";	
		colorCodes[81]="FFB6C1\0";	//"LightPink";
		colorCodes[82]="DB7093\0";	//"PaleVioletRed";
		colorCodes[83]="B03060\0";	//"maroon";	
		colorCodes[84]="C71585\0";	//"mediumVioletRed";
		colorCodes[85]="D02090\0";	//"VioletRed";	 
		colorCodes[86]="FF00FF\0";	//"Magenta";	
		colorCodes[87]="EE82EE\0";	//"violet";	
		colorCodes[88]="DDA0DD\0";	//"plum";	
		colorCodes[89]="DA70D6\0";	//"orchid";
		colorCodes[90]="BA55D3\0";	//"MediumOrchid";
		colorCodes[91]="9932CC\0";	//"DarkOrchid";	
		colorCodes[92]="9400D3\0";	//"DarkViolet";
		colorCodes[93]="8A2BE2\0";	//"BlueViolet";	
		colorCodes[94]="A020F0\0";	//"purple";	


	  }		
	   else	
               {
		colorCodes[1]= "FFFF00\0";	//"yellow";
		colorCodes[2]= "FF00FF\0";	//"magenta";
		colorCodes[3]= "00FFFF\0";	//"cyan";
		colorCodes[4]= "D2B9D3\0";	//"thistle";
		colorCodes[5]= "00FF00\0";	//"green";
		colorCodes[6]= "FF0000\0";	//"red";
		colorCodes[7]= "FAAFBE\0";	//"pink";
		colorCodes[8]= "A09F9D\0";	//"lightgray";
		colorCodes[9]= "0000FF\0";	//"blue";
		colorCodes[10]="F87A17\0";	//"orange";
		colorCodes[11]="F3DAA9\0";	//"wheat";
		colorCodes[12]="79D867\0";	//"palegreen";
		colorCodes[13]="E9358A\0";	//"violetred";
		colorCodes[14]="728FCE\0";	//"lightsteelblue";
		colorCodes[15]="F76541\0";	//"coral";
		colorCodes[16]="25383C\0";	//"dark slate gray";
		colorCodes[17]="254117\0";	//"darkgreen";	
	  	colorCodes[18]="B93B8F\0";	//"plum";
	    colorCodes[19]="F63817\0";	//"orangered";
	    colorCodes[20]="43C6DB\0";	//"turquoise";
	    colorCodes[21]="306754\0";	//"medium sea green";
	    colorCodes[22]="4863A0\0";	//"steelblue";
	    colorCodes[23]="810541\0";	//"maroon";
	    colorCodes[24]="000000\0";	//"black";
	    colorCodes[25]="D8AF79\0";	//"tan";
		colorCodes[26]="7D1B7E\0";	//"darkorchid";
		colorCodes[27]="43B7BA\0";	//"aquamarine"
		colorCodes[28]="6698FF\0";	//"skyblue";
		colorCodes[29]="D4A017\0";	//"gold
		colorCodes[30]="151B54\0";	//"midnightblue";
		colorCodes[31]="980517\0";	//"brown";
		colorCodes[32]="FF4046\0";	//"brown1";
		colorCodes[33]="EE3B3B\0";	//"brown2";
		colorCodes[34]="CD3333\0";	//"brown3";
		colorCodes[35]="00FF7F\0";	//"SpringGreen";
		colorCodes[36]="7CFC00\0";	//"LawnGreen";
		colorCodes[37]="00FF00\0";	//"green";
		colorCodes[38]="7FFF00\0";	//"chartreuse";	
		colorCodes[39]="00FA9A\0";	//"MediumSpringGreen";
		colorCodes[40]="FF0000\0";	//"red";
		colorCodes[41]="000000\0";	//"black";
		colorCodes[42]="0000FF\0";	//"blue";
		colorCodes[43]="00FF00\0";	//"Green";
		colorCodes[44]="DAAA00\0";	//"gold";
		colorCodes[45]="FFFF00\0";	//"yellow";
		colorCodes[46]="F0E68C\0";	//"Khaki";
		colorCodes[47]="EEE8AA\0";	//"PaleGoldenrod";
		colorCodes[48]="FAFAD2\0";	//"LightGoldenrodYellow";
		colorCodes[49]="FFFFE0\0";	//"LightYellow";
		colorCodes[50]="FFFF00\0";	//"Yellow";
		colorCodes[51]="FF8D00\0";	//"gold";
		colorCodes[52]="EEC382\0";	//"LightGoldenrod";	
		colorCodes[53]="DAA520\0";	//"goldenrod";	
		colorCodes[54]="B8860B\0";	//"DarkGoldenRod";
		colorCodes[55]="BC8F8F\0";	//"RosyBrown";
		colorCodes[56]="CD5C5C\0";	//"IndianRed";	
		colorCodes[57]="8B4513\0";	//"SaddleBrown";
		colorCodes[58]="A05253\0";	//"sienna";	
		colorCodes[59]="CD853F\0";	//"peru";
		colorCodes[60]="DEB887\0";	//"burlywood";
		colorCodes[61]="F5F5DC\0";	//"beige";
		colorCodes[62]="F5DEB3\0";	//"wheat";	
		colorCodes[63]="F4A460\0";	//"Sandybrown";	
		colorCodes[64]="D2B48C\0";	//"tan";
		colorCodes[65]="D2691E\0";	//"chocolate";
		colorCodes[66]="B22222\0";	//"firebrick";
		colorCodes[67]="A52A2A\0";	//"brown";
		colorCodes[68]="E9967A\0";	//"DarkSalmon";
		colorCodes[69]="FA8072\0";	//"salmon";
		colorCodes[70]="FFA07A\0";	//"LightSalmon";
		colorCodes[71]="FFA500\0";	//"Orange";	
		colorCodes[72]="FF8C00\0";	//"DarkOrange";	
		colorCodes[73]="FF7F50\0";	//"coral";
		colorCodes[74]="F08080\0"; 	//"Lightcoral"	
		colorCodes[75]="FF6347\0";	//"tomato";	
		colorCodes[76]="FF4500\0";	//"OrangeRed";
		colorCodes[77]="FF0000\0";	//"red";
		colorCodes[78]="FF69B4\0";	//"HotPink";
		colorCodes[79]="FF1493\0";	//"DeepPink";
		colorCodes[80]="FFC0CB\0";	//"pink";	
		colorCodes[81]="FFB6C1\0";	//"LightPink";
		colorCodes[82]="DB7093\0";	//"PaleVioletRed";
		colorCodes[83]="B03060\0";	//"maroon";	
		colorCodes[84]="C71585\0";	//"mediumVioletRed";
		colorCodes[85]="D02090\0";	//"VioletRed";	 
		colorCodes[86]="FF00FF\0";	//"Magenta";	
		colorCodes[87]="EE82EE\0";	//"violet";	
		colorCodes[88]="DDA0DD\0";	//"plum";	
		colorCodes[89]="DA70D6\0";	//"orchid";
		colorCodes[90]="BA55D3\0";	//"MediumOrchid";
		colorCodes[91]="9932CC\0";	//"DarkOrchid";	
		colorCodes[92]="9400D3\0";	//"DarkViolet";
		colorCodes[93]="8A2BE2\0";	//"BlueViolet";	
		colorCodes[94]="A020F0\0";	//"purple";	
		}
        if((jobType==CONTOURLINE)||(jobType==CONTOURLINE_B)||
		   (jobType==COLOR_PLOT)||(jobType==COLOR_PLOT_B)||
		   (jobType==CONTOURLINE_C))
          {
		colorCodes[1]="CD0000\0";	//"red3";
		colorCodes[2]="EE0000\0";	//"red2";
		colorCodes[3]="FF0000\0";	//"red";
		colorCodes[4]="FF4500\0";	//"orange red";
		colorCodes[5]="FF8C00\0";	//"dark orange";
		colorCodes[6]="EE9A00\0";	//"orange2";
		colorCodes[7]="DAAA00\0";	//"gold";
		colorCodes[8]="CDAD00\0";	//"gold3";
		colorCodes[9]="CDCD00\0";	//"yellow3";
		colorCodes[10]="EEEE00\0";	//"yellow2";
		colorCodes[11]="FFFF00\0";	//"yellow";
		colorCodes[12]="ABFF2F\0";	//"green yellow";
		colorCodes[13]="00FF00\0";	//"green";
		colorCodes[14]="00EE00\0";	//"green2";
		colorCodes[15]="00CD00\0";	//"green3";
		colorCodes[16]="008B00\0";	//"green4";
		colorCodes[17]="529584\0";	//"SeaGreen";
		if((jobType==CONTOURLINE_B)||(jobType==COLOR_PLOT_B))
			colorCodes[17]="000000\0";//"black";
		colorCodes[18]="66CDAA\0";	//"aquamarine3";
		colorCodes[19]="20B2AA\0";	//"light sea green";
		colorCodes[20]="32BFC1\0";	//"aquamarine";
		colorCodes[21]="19CCDF\0";	//"turquoise";
		colorCodes[22]="00BFFF\0";	//"DeepSkyBlue1";
		colorCodes[23]="00B2EE\0";	//"DeepSkyBlue2";
		colorCodes[24]="4876FF\0";	//"RoyalBlue1";
		colorCodes[25]="4169E1\0";	//"RoyalBlue";
		colorCodes[26]="0000FF\0";	//"blue";
		colorCodes[27]="0000EE\0";	//"blue2";
		colorCodes[28]="0000CD\0";	//"blue3";
		colorCodes[29]="00008B\0";	//"blue4";
		colorCodes[30]="68228B\0";	//"darkorchid4";
		colorCodes[31]="9A32CD\0";	//"darkorchid3";
		colorCodes[32]="CD3278\0";	//"violetred3";
		colorCodes[33]="EE3A8C\0";	//"violetred2";
		colorCodes[34]="F33E96\0";	//"violetred";
	  }

    ofstream outFile(fileName,ios::out);
    int i,j;
    unsigned char* help= new unsigned char[rows];
    
    
    if( !outFile.good() )
        fatalError("xpmPrinter::print","Can't open output file");
    if( help == NULL )
        fatalError("xpmPrinter::print","Can't allocate memory"); 

   cout << "Save xpmPicture to file " << fileName << " ... ";
   outFile << "/* XPM */\n";
   outFile << "static char * macrodyn_xpm[] = {\n";
   outFile << "/* width height ncolors chars_per_pixel */\n";
   int orders[95];
   orders[0]=0; //white background
   i=1; 
   for(j=1;j<95;j++)	//count colors
   	if(graphMatrixColor[j]!=0) {
		orders[i]=j;
   		i+=1;
	}

   outFile << "\""  << xDim << " " << yDim << " " << i << " 1\",\n";

   outFile << "/* macrodyn_info\n";
   outFile << "{order ";
   for(j=1;j<i;j++)
   		outFile << orders[j] << " ";// store which Ordernumbers are used
   outFile << "}\n";

   outFile << "{axes\n\t";
   outFile << limits->label[0] << "\t";
   outFile << limits->min[0] << "\t";
   outFile << limits->max[0] << "\n\t";
   outFile << limits->label[1] << "\t";
   outFile << limits->min[1] << "\t";
   outFile << limits->max[1] << "\n\t";
   if(limits->dimension==3) {
   outFile << limits->label[2] << "\t";
   outFile << limits->min[2] << "\t";
   outFile << limits->max[2] << "\n";
   }
   outFile << "}\n";  

   if((limits->zeroline[0]>-1)||(limits->zeroline[1]>-1)) { 
      outFile << "{zerolines\n";
      if(limits->zeroline[0]>-1) outFile << "\tontoX\t" << limits->zeroline[0] << "\n";
      if(limits->zeroline[1]>-1) outFile << "\tontoY\t" << limits->zeroline[1] << "\n";
      outFile << "}\n";  
   }

   if(multiplotPtrs) {
	int dummy,di,dj;
	real dvalue;
	outFile << "{multiplot ";
	outFile << multiplot_num+1 << "\n";
	outFile << "\t" << limits->label[1] << "\t" << colorCodes[orders[1]] << "\t";
	for(dj=0;dj<6;dj+=2) { // write rgb codes [0..1],[0..1],[0..1]
		dvalue=0;
		for(di=0;di<2;di++) {
			dvalue*=16;
			dummy=(int)(colorCodes[orders[1]][dj+di]);
			if(dummy<58) dummy-=48; // ASCII chars 0 to 9
			 else  dummy-=55; // ASCII chars A to F
			dvalue+=dummy;
		}
		outFile << (dvalue/255);
		if(dj<4) outFile << ",";
		 else outFile << "\n";
	}

	for(j=0;j<multiplot_num;j++) { // write rgb codes in hex
		outFile << "\t" << multiplotPtrs[j] << "\t" << colorCodes[orders[j+2]] << "\t";
		for(dj=0;dj<6;dj+=2) { // write rgb codes [0..1],[0..1],[0..1]
			dvalue=0;
			for(di=0;di<2;di++) {
				dvalue*=16;
				dummy=(int)(colorCodes[orders[j+2]][dj+di]);
				if(dummy<58) dummy-=48; // ASCII chars 0 to 9
				 else  dummy-=55; // ASCII chars A to F
				dvalue+=dummy;
			}
			outFile << (dvalue/255);
			if(dj<4) outFile << ",";
			 else outFile << "\n";
		}
	}
   outFile << "}\n";  
   }

   outFile << "{parameterset\t";
   model->saveParamsetWithNames(outFile); 
   outFile << "}\n";  
   outFile << "end_of_macrodyn_info */\n";  

   if(graphMatrixColor[0]==0)	//oor color is used as not set color in Bifurcations
    	graphMatrixColor[0]=1;	//and if not used in Colormaps,its does not harm
   char colorchar;
   for(j=0;j<95;j++)
   	if(graphMatrixColor[j]!=0)
   		{
   		colorchar=95+j;
   		outFile << "\"" << colorchar << "     c #" << colorCodes[j] << "\",\n";
		}
   for(i=rows-1;i>0;i--)
	{
        outFile << "\"";
        for(j=0;j<xDim;j++)
            	outFile << graphMatrix[i][j];
        outFile << "\",\n";
	}
   outFile << "\"";
   for(j=0;j<xDim;j++)
   	{
        outFile << graphMatrix[i][j];
        }
   outFile << "\"};\n";

   outFile.flush();
   outFile.close();

   cout << " done." << endl; 
}



////////////////////////////////////////////////////////////////////////////////
//
// member function:	screenshot
// purpose:		copies output image to printer matrix
//
// author:		Andreas Starke
// last modified:	
//
////////////////////////////////////////////////////////////////////////////////

void xpmPrinter::screenshot(graphics* grafik,char* filename)
{
	unsigned long color;
	short color_return;
	char search_dummy;
	int i,j;
	unsigned char** graphMatrix2;
	int *graphMatrixColor2;
	
	if ( jobType==CONTOURLINE_C ) {
		x_dimension=400;
	}
	
	cout << "printer::screenshot copying output to printer..." <<endl;
	cout << "pinter file resolution: x= " << x_dimension << " y= " << y_dimension << endl;
	XImage* xi;
	XColor ColorRange[98];
	Display* displ=grafik->get_display();
	Colormap cmap=grafik->get_cmap();
	
	grafik->flushGraph();
	xi=XGetImage(displ,grafik->buffer_pix,0,0,
	 600,300, AllPlanes, XYPixmap);	
	cout << "sizeX()=" << grafik->sizeX()<<" sizeY()="<<grafik->sizeY()<<endl;
	cout << "XImage size: x= "<<xi->width<<" y= "<<xi->height<<endl;
	
	// set-up additional printer matrix and used color matrix
	
	graphMatrixColor2=new int[98]; // 97 colors are hardcoded jet
	if( graphMatrixColor2 == NULL ) 
        fatalError("printer::screenshot","Can't allocate memory");

	for(j=0;j<98;j++)
        graphMatrixColor2[j]=0;

	graphMatrix2=new unsigned char*[y_dimension];
	if( graphMatrix2 == NULL ) 
        fatalError("printer::screenshot","Can't allocate memory");


	for(i=0; i<y_dimension; i++) {
	        graphMatrix2[i]=new unsigned char[x_dimension];
        	if( graphMatrix2[i] == NULL ) 
            	fatalError("printer::screenshot","Can't allocate memory");
    	}	
	
	for (short k=0; k<98; k++) {
		ColorRange[k]=grafik->get_exact(k);
		 }

	real fortschritt=0;
	real schritt;
	schritt=1/real(y_dimension)*100;
	for (i=0;i<y_dimension;i++) {
		fortschritt+=schritt;
		for (int j=0;j<x_dimension;j++){
		color=XGetPixel(xi,j,y_dimension-i-1);
		color_return=-1;
		parsi.pixel=color;
		XQueryColor(displ, cmap, &parsi);
		for (k=0;k<98;k++)
			if (parsi.red==ColorRange[k].red)
			if (parsi.green==ColorRange[k].green)
			if (parsi.blue==ColorRange[k].blue)
				color_return=k;
		graphMatrix2[i][j]=char (95+color_return);
		graphMatrixColor2[color_return]=1;
		if (color_return==-1) 
			cout << "color not found..."<<" x="<<j<<"\ty="
			<<i<<endl;
		}
		cout << "\r" << int(fortschritt) << "%" << std::flush;
		}
// clear screenshot labels
// default color to be searched for is 96 (->seek char 191)
// default boxes to search are [(0,285);(80,299)] y-label
// and [(540,20);(600,40)] x-label for jobs without color-count
// and [(341,20);(400,40)] x-label for jobs with color-count
// default color to set is 0 (->set char 95)
	search_dummy=191;
//	cout << "gesucht wird : "<<search_dummy<<"ersetzte durch: "<<char(95)<<endl;
	for (i=0;i<80;i++) 
		for (j=285;j<300;j++) 
			if (char(graphMatrix2[j][i])==search_dummy) 
				graphMatrix2[j][i]=95;
        if ((jobType==CONTOURLINE)||(jobType==CONTOURLINE_B)||
		(jobType==COLOR_PLOT)||(jobType==COLOR_PLOT_B)||
		(jobType==CONTOURLINE_C)){
		for (i=341;i<400;i++)
			for (j=20;j<41;j++)
				if (char(graphMatrix2[j][i])==search_dummy) 
					graphMatrix2[j][i]=95;
		multiplotPtrs=NULL;	// leave it here job corrupts pointer (isn't possible anyway)
	} else {
	for (i=541;i<600;i++)
		for (j=20;j<41;j++)
			if (char(graphMatrix2[j][i])==search_dummy)
				graphMatrix2[j][i]=95;				
	}
// implementation: clearing of "color scale of <variable>"
// for color or contourline jobs
// deafult box is [(400,280);(600,300)]
        if ((jobType==CONTOURLINE)||(jobType==CONTOURLINE_B)||
		(jobType==COLOR_PLOT)||(jobType==COLOR_PLOT_B)){
		search_dummy=124;
		for (i=400;i<600;i++) 
			for (j=280;j<300;j++) 
				if (char(graphMatrix2[j][i])==search_dummy) {
					graphMatrix2[j][i]=95;
//					cout <<"point: ("<<i<<","<<j<<")"<<endl;
				}
	}

	screenshot_to_file(graphMatrix2,graphMatrixColor2,filename);
	
	// delete additionally set up matrices
	
	if( graphMatrix2 ) {
        for( i=0; i<y_dimension; i++ )
	     if( graphMatrix2[i] )
                delete [] graphMatrix2[i];		// free picture memory
        delete [] graphMatrix2;
    	}
	if ( graphMatrixColor2 )
		delete [] graphMatrixColor2;		// free color matrix
}

////////////////////////////////////////////////////////////////////////////////
//
// member function:	screenshot_to_file
// purpose:		copies output in xpm-file
//
// author:		Andreas Starke
// last modified:	
//
////////////////////////////////////////////////////////////////////////////////

void xpmPrinter::screenshot_to_file(unsigned char** graphMatrix2,
 int* graphMatrixColor2,char* filename)
{
	char *colorCodes[98];
        colorCodes[0]= "FFFFFF\0";

        if((jobType==D_BIF2D)||(jobType==D_ATTRA)||(jobType==D_BIFNPARAM))
          {
		colorCodes[1]="191970\0";	//"MidnightBlue";					 
		colorCodes[2]="00007D\0";	//"NavyBlue";
		colorCodes[3]="6495ED\0";	//"CornflowerBlue";
		colorCodes[4]="483D8B\0";	//"DarkSlateBlue";	
 		colorCodes[5]="6A5ACD\0";	//"SlateBlue";	
		colorCodes[6]="7B68EE\0";	//"MediumSlateBlue"; 
		colorCodes[7]="8470FF\0";	//"LightSlateBlue";
		colorCodes[8]="0000CD\0";	//"MediumBlue"; 
		colorCodes[9]="4169E1\0";	//"RoyalBlue";
		colorCodes[10]="0000FF\0";	//"Blue";
		colorCodes[11]="1D90FF\0";	//"DodgerBlue";
		colorCodes[12]="00BFFF\0";	//"DeepSkyBlue"; 
		colorCodes[13]="87CEEB\0";	//"SkyBlue"; 
		colorCodes[14]="87CEFA\0";	//"LightSkyBlue";
		colorCodes[15]="4682B4\0";	//"SteelBlue"; 
		colorCodes[16]="B0C4DE\0";	//"LightSteelBlue";	
		colorCodes[17]="ADD8E6\0";	//"LightBlue";
		colorCodes[18]="B0E0E6\0";	//"PowderBlue";	
		colorCodes[19]="AFEEEE\0";	//"PaleTurquoise";
		colorCodes[20]="00CED1\0";	//"DarkTurquoise";
		colorCodes[21]="48D1CC\0";	//"MediumTurquoise";
		colorCodes[22]="40E0D0\0";	//"Turquoise";
		colorCodes[23]="00FFFF\0";	//"cyan"; 
		colorCodes[24]="E0FFFF\0";	//"LightCyan";
		colorCodes[25]="5F9EA0\0";	//"CadetBlue";
		colorCodes[26]="66CD11\0";	//"MediumAquamarine"
		colorCodes[27]="7FFFD4\0";	//"Aquamarine";
		colorCodes[28]="006400\0";	//"DarkGreen";
		colorCodes[29]="556B2F\0";	//"DarkOliveGreen";
		colorCodes[30]="8FBC57\0";	//"DarkSeaGreen";
		colorCodes[31]="548B57\0";	//"SeaGreen";
		colorCodes[32]="32B371\0";	//"MediumSeaGreen";
		colorCodes[33]="20B2AA\0";	//"LightSeaGreeen";
		colorCodes[34]="98FB98\0";	//"PaleGreen";
		colorCodes[35]="00FF7F\0";	//"SpringGreen";
		colorCodes[36]="7CFC00\0";	//"LawnGreen";
		colorCodes[37]="00FF00\0";	//"green";
		colorCodes[38]="7FFF00\0";	//"chartreuse";	
		colorCodes[39]="00FA9A\0";	//"MediumSpringGreen";
		colorCodes[40]="ADFF2F\0";	//"GreenYellow";
		colorCodes[41]="32CD32\0";	//"LimeGreen";
		colorCodes[42]="9ACD32\0";	//"YellowGreen";
		colorCodes[43]="228B22\0";	//"ForestGreen";
		colorCodes[44]="6B8E23\0";	//"OliveDrab";
		colorCodes[45]="BDB76B\0";	//"DarkKhaki";
		colorCodes[46]="F0E68C\0";	//"Khaki";
		colorCodes[47]="EEE8AA\0";	//"PaleGoldenrod";
		colorCodes[48]="FAFAD2\0";	//"LightGoldenrodYellow";
		colorCodes[49]="FFFFE0\0";	//"LightYellow";
		colorCodes[50]="FFFF00\0";	//"Yellow";
		colorCodes[51]="FF8D00\0";	//"gold";
		colorCodes[52]="EEC382\0";	//"LightGoldenrod";	
		colorCodes[53]="DAA520\0";	//"goldenrod";	
		colorCodes[54]="B8860B\0";	//"DarkGoldenRod";
		colorCodes[55]="BC8F8F\0";	//"RosyBrown";
		colorCodes[56]="CD5C5C\0";	//"IndianRed";	
		colorCodes[57]="8B4513\0";	//"SaddleBrown";
		colorCodes[58]="A05253\0";	//"sienna";	
		colorCodes[59]="CD853F\0";	//"peru";
		colorCodes[60]="DEB887\0";	//"burlywood";
		colorCodes[61]="F5F5DC\0";	//"beige";
		colorCodes[62]="F5DEB3\0";	//"wheat";	
		colorCodes[63]="F4A460\0";	//"Sandybrown";	
		colorCodes[64]="D2B48C\0";	//"tan";
		colorCodes[65]="D2691E\0";	//"chocolate";
		colorCodes[66]="B22222\0";	//"firebrick";
		colorCodes[67]="A52A2A\0";	//"brown";
		colorCodes[68]="E9967A\0";	//"DarkSalmon";
		colorCodes[69]="FA8072\0";	//"salmon";
		colorCodes[70]="FFA07A\0";	//"LightSalmon";
		colorCodes[71]="FFA500\0";	//"Orange";	
		colorCodes[72]="FF8C00\0";	//"DarkOrange";	
		colorCodes[73]="FF7F50\0";	//"coral";
		colorCodes[74]="F08080\0"; 	//"Lightcoral"	
		colorCodes[75]="FF6347\0";	//"tomato";	
		colorCodes[76]="FF4500\0";	//"OrangeRed";
		colorCodes[77]="FF0000\0";	//"red";
		colorCodes[78]="FF69B4\0";	//"HotPink";
		colorCodes[79]="FF1493\0";	//"DeepPink";
		colorCodes[80]="FFC0CB\0";	//"pink";	
		colorCodes[81]="FFB6C1\0";	//"LightPink";
		colorCodes[82]="DB7093\0";	//"PaleVioletRed";
		colorCodes[83]="B03060\0";	//"maroon";	
		colorCodes[84]="C71585\0";	//"mediumVioletRed";
		colorCodes[85]="D02090\0";	//"VioletRed";	 
		colorCodes[86]="FF00FF\0";	//"Magenta";	
		colorCodes[87]="EE82EE\0";	//"violet";	
		colorCodes[88]="DDA0DD\0";	//"plum";	
		colorCodes[89]="DA70D6\0";	//"orchid";
		colorCodes[90]="BA55D3\0";	//"MediumOrchid";
		colorCodes[91]="9932CC\0";	//"DarkOrchid";	
		colorCodes[92]="9400D3\0";	//"DarkViolet";
		colorCodes[93]="8A2BE2\0";	//"BlueViolet";	
		colorCodes[94]="A020F0\0";	//"purple";	
	  }		
	   else	
               {
		colorCodes[1]= "FFFF00\0";	//"yellow";
		colorCodes[2]= "FF00FF\0";	//"magenta";
		colorCodes[3]= "00FFFF\0";	//"cyan";
		colorCodes[4]= "D2B9D3\0";	//"thistle";
		colorCodes[5]= "00FF00\0";	//"green";
		colorCodes[6]= "FF0000\0";	//"red";
		colorCodes[7]= "FAAFBE\0";	//"pink";
		colorCodes[8]= "A09F9D\0";	//"lightgray";
		colorCodes[9]= "0000FF\0";	//"blue";
		colorCodes[10]="F87A17\0";	//"orange";
		colorCodes[11]="F3DAA9\0";	//"wheat";
		colorCodes[12]="79D867\0";	//"palegreen";
		colorCodes[13]="E9358A\0";	//"violetred";
		colorCodes[14]="728FCE\0";	//"lightsteelblue";
		colorCodes[15]="F76541\0";	//"coral";
		colorCodes[16]="25383C\0";	//"dark slate gray";
		colorCodes[17]="254117\0";	//"darkgreen";	
	  	colorCodes[18]="B93B8F\0";	//"plum";
		colorCodes[19]="F63817\0";	//"orangered";
		colorCodes[20]="43C6DB\0";	//"turquoise";
		colorCodes[21]="306754\0";	//"medium sea green";
		colorCodes[22]="4863A0\0";	//"steelblue";
		colorCodes[23]="810541\0";	//"maroon";
		colorCodes[24]="000000\0";	//"black";
		colorCodes[25]="D8AF79\0";	//"tan";
		colorCodes[26]="7D1B7E\0";	//"darkorchid";
		colorCodes[27]="43B7BA\0";	//"aquamarine"
		colorCodes[28]="6698FF\0";	//"skyblue";
		colorCodes[29]="D4A017\0";	//"gold
		colorCodes[30]="151B54\0";	//"midnightblue";
		colorCodes[31]="980517\0";	//"brown";
		colorCodes[32]="FF4046\0";	//"brown1";
		colorCodes[33]="EE3B3B\0";	//"brown2";
		colorCodes[34]="CD3333\0";	//"brown3";
		colorCodes[35]="00FF7F\0";	//"SpringGreen";
		colorCodes[36]="7CFC00\0";	//"LawnGreen";
		colorCodes[37]="00FF00\0";	//"green";
		colorCodes[38]="7FFF00\0";	//"chartreuse";	
		colorCodes[39]="00FA9A\0";	//"MediumSpringGreen";
		colorCodes[40]="FF0000\0";	//"red";
		colorCodes[41]="000000\0";	//"black";
		colorCodes[42]="0000FF\0";	//"blue";
		colorCodes[43]="00FF00\0";	//"Green";
		colorCodes[44]="DAAA00\0";	//"gold";
		colorCodes[45]="FFFF00\0";	//"yellow";
		colorCodes[46]="F0E68C\0";	//"Khaki";
		colorCodes[47]="EEE8AA\0";	//"PaleGoldenrod";
		colorCodes[48]="FAFAD2\0";	//"LightGoldenrodYellow";
		colorCodes[49]="FFFFE0\0";	//"LightYellow";
		colorCodes[50]="FFFF00\0";	//"Yellow";
		colorCodes[51]="FF8D00\0";	//"gold";
		colorCodes[52]="EEC382\0";	//"LightGoldenrod";	
		colorCodes[53]="DAA520\0";	//"goldenrod";	
		colorCodes[54]="B8860B\0";	//"DarkGoldenRod";
		colorCodes[55]="BC8F8F\0";	//"RosyBrown";
		colorCodes[56]="CD5C5C\0";	//"IndianRed";	
		colorCodes[57]="8B4513\0";	//"SaddleBrown";
		colorCodes[58]="A05253\0";	//"sienna";	
		colorCodes[59]="CD853F\0";	//"peru";
		colorCodes[60]="DEB887\0";	//"burlywood";
		colorCodes[61]="F5F5DC\0";	//"beige";
		colorCodes[62]="F5DEB3\0";	//"wheat";	
		colorCodes[63]="F4A460\0";	//"Sandybrown";	
		colorCodes[64]="D2B48C\0";	//"tan";
		colorCodes[65]="D2691E\0";	//"chocolate";
		colorCodes[66]="B22222\0";	//"firebrick";
		colorCodes[67]="A52A2A\0";	//"brown";
		colorCodes[68]="E9967A\0";	//"DarkSalmon";
		colorCodes[69]="FA8072\0";	//"salmon";
		colorCodes[70]="FFA07A\0";	//"LightSalmon";
		colorCodes[71]="FFA500\0";	//"Orange";	
		colorCodes[72]="FF8C00\0";	//"DarkOrange";	
		colorCodes[73]="FF7F50\0";	//"coral";
		colorCodes[74]="F08080\0"; 	//"Lightcoral"	
		colorCodes[75]="FF6347\0";	//"tomato";	
		colorCodes[76]="FF4500\0";	//"OrangeRed";
		colorCodes[77]="FF0000\0";	//"red";
		colorCodes[78]="FF69B4\0";	//"HotPink";
		colorCodes[79]="FF1493\0";	//"DeepPink";
		colorCodes[80]="FFC0CB\0";	//"pink";	
		colorCodes[81]="FFB6C1\0";	//"LightPink";
		colorCodes[82]="DB7093\0";	//"PaleVioletRed";
		colorCodes[83]="B03060\0";	//"maroon";	
		colorCodes[84]="C71585\0";	//"mediumVioletRed";
		colorCodes[85]="D02090\0";	//"VioletRed";	 
		colorCodes[86]="FF00FF\0";	//"Magenta";	
		colorCodes[87]="EE82EE\0";	//"violet";	
		colorCodes[88]="DDA0DD\0";	//"plum";	
		colorCodes[89]="DA70D6\0";	//"orchid";
		colorCodes[90]="BA55D3\0";	//"MediumOrchid";
		colorCodes[91]="9932CC\0";	//"DarkOrchid";	
		colorCodes[92]="9400D3\0";	//"DarkViolet";
		colorCodes[93]="8A2BE2\0";	//"BlueViolet";	
		colorCodes[94]="A020F0\0";	//"purple";	
		}
        if((jobType==CONTOURLINE)||(jobType==CONTOURLINE_B)||
	   (jobType==COLOR_PLOT)||(jobType==COLOR_PLOT_B)||
	   (jobType==CONTOURLINE_C))
          {
		colorCodes[1]="CD0000\0";	//"red3";
		colorCodes[2]="EE0000\0";	//"red2";
		colorCodes[3]="FF0000\0";	//"red";
		colorCodes[4]="FF4500\0";	//"orange red";
		colorCodes[5]="FF8C00\0";	//"dark orange";
		colorCodes[6]="EE9A00\0";	//"orange2";
		colorCodes[7]="DAAA00\0";	//"gold";
		colorCodes[8]="CDAD00\0";	//"gold3";
		colorCodes[9]="CDCD00\0";	//"yellow3";
		colorCodes[10]="EEEE00\0";	//"yellow2";
		colorCodes[11]="FFFF00\0";	//"yellow";
		colorCodes[12]="ABFF2F\0";	//"green yellow";
		colorCodes[13]="00FF00\0";	//"green";
		colorCodes[14]="00EE00\0";	//"green2";
		colorCodes[15]="00CD00\0";	//"green3";
		colorCodes[16]="008B00\0";	//"green4";
		colorCodes[17]="529584\0";	//"SeaGreen";
		if((jobType==CONTOURLINE_B)||(jobType==COLOR_PLOT_B))
			colorCodes[17]="000000\0";//"black";
		colorCodes[18]="66CDAA\0";	//"aquamarine3";
		colorCodes[19]="20B2AA\0";	//"light sea green";
		colorCodes[20]="32BFC1\0";	//"aquamarine";
		colorCodes[21]="19CCDF\0";	//"turquoise";
		colorCodes[22]="00BFFF\0";	//"DeepSkyBlue1";
		colorCodes[23]="00B2EE\0";	//"DeepSkyBlue2";
		colorCodes[24]="4876FF\0";	//"RoyalBlue1";
		colorCodes[25]="4169E1\0";	//"RoyalBlue";
		colorCodes[26]="0000FF\0";	//"blue";
		colorCodes[27]="0000EE\0";	//"blue2";
		colorCodes[28]="0000CD\0";	//"blue3";
		colorCodes[29]="00008B\0";	//"blue4";
		colorCodes[30]="68228B\0";	//"darkorchid4";
		colorCodes[31]="9A32CD\0";	//"darkorchid3";
		colorCodes[32]="CD3278\0";	//"violetred3";
		colorCodes[33]="EE3A8C\0";	//"violetred2";
		colorCodes[34]="F33E96\0";	//"violetred";
	  }
	colorCodes[95]="000000\0";	//"AXISCOLOR black"
	colorCodes[96]="FF0000\0";	//"AXISLABELCOLOR red"
	colorCodes[97]="006400\0";	//"XCISMARKSCOLOR dark green"

    ofstream outFile(filename,ios::out);
    int i,j;
    unsigned char* help= new unsigned char[y_dimension];
    
    if( !outFile.good() )
        fatalError("xpmPrinter::screenshot_to_file","Can't open output file");
    if( help == NULL )
        fatalError("xpmPrinter::screenshot_to_file","Can't allocate memory"); 

   cout << "Save xpmPicture to file " << filename << " ... "<<std::flush;
   outFile << "/* XPM */\n";
   outFile << "static char * macrodyn_xpm[] = {\n";
   outFile << "/* width height ncolors chars_per_pixel */\n";
   int orders[98];
   orders[0]=0; //white background
   i=1; 
   for(j=1;j<98;j++)	//count colors
   	if(graphMatrixColor2[j]!=0) {
		orders[i]=j;
   		i+=1;
	}

   outFile << "\""  << x_dimension << " " << y_dimension << " " << i << " 1\",\n";

   outFile << "/* macrodyn_info\n";
   outFile << "{order ";
   for(j=1;j<i;j++)
   		outFile << orders[j] << " ";// store which Ordernumbers are used
   outFile << "}\n";

   outFile << "{axes\n\t";
   outFile << jobType << "\n\t";
   outFile << limits->label[0] << "\t";
   outFile << limits->min[0] << "\t";
   outFile << limits->max[0] << "\n\t";
   outFile << limits->label[1] << "\t";
   outFile << limits->min[1] << "\t";
   outFile << limits->max[1] << "\n\t";
   if(limits->dimension==3) {
   outFile << limits->label[2] << "\t";
   outFile << limits->min[2] << "\t";
   outFile << limits->max[2] << "\n";
   }
   outFile << "}\n";  

   if((limits->zeroline[0]>-1)||(limits->zeroline[1]>-1)) { 
      outFile << "{zerolines\n";
      if(limits->zeroline[0]>-1) outFile << "\tontoX\t" << limits->zeroline[0] << "\n";
      if(limits->zeroline[1]>-1) outFile << "\tontoY\t" << limits->zeroline[1] << "\n";
      outFile << "}\n";  
   }

   if(multiplotPtrs) {
	int dummy,di,dj;
	real dvalue;
	outFile << "{multiplot ";
	outFile << multiplot_num+1 << "\n";
	outFile << "\t" << limits->label[1] << "\t" << colorCodes[orders[1]] << "\t";
	for(dj=0;dj<6;dj+=2) { // write rgb codes [0..1],[0..1],[0..1]
		dvalue=0;
		for(di=0;di<2;di++) {
			dvalue*=16;
			dummy=(int)(colorCodes[orders[1]][dj+di]);
			if(dummy<58) dummy-=48; // ASCII chars 0 to 9
			 else  dummy-=55; // ASCII chars A to F
			dvalue+=dummy;
		}
		outFile << (dvalue/255);
		if(dj<4) outFile << ",";
		 else outFile << "\n";
	}

	for(j=0;j<multiplot_num;j++) { // write rgb codes in hex
		outFile << "\t" << multiplotPtrs[j] << "\t" << colorCodes[orders[j+2]] << "\t";
		for(dj=0;dj<6;dj+=2) { // write rgb codes [0..1],[0..1],[0..1]
			dvalue=0;
			for(di=0;di<2;di++) {
				dvalue*=16;
				dummy=(int)(colorCodes[orders[j+2]][dj+di]);
				if(dummy<58) dummy-=48; // ASCII chars 0 to 9
				 else  dummy-=55; // ASCII chars A to F
				dvalue+=dummy;
			}
			outFile << (dvalue/255);
			if(dj<4) outFile << ",";
			 else outFile << "\n";
		}
	}
   outFile << "}\n";  
   }

   outFile << "{parameterset\t";
   model->saveParamsetWithNames(outFile); 
   outFile << "}\n";  
   outFile << "end_of_macrodyn_info */\n";  

   if(graphMatrixColor2[0]==0)	//oor color is used as not set color in Bifurcations
    	graphMatrixColor2[0]=1;	//and if not used in Colormaps,its does not harm
   char colorchar;
   for(j=0;j<98;j++)
   	if(graphMatrixColor2[j]!=0)
   		{
   		colorchar=95+j;
   		outFile << "\"" << colorchar << "     c #" << colorCodes[j] << "\",\n";
		}
   for(i=y_dimension-1;i>0;i--)
	{
        outFile << "\"";
        for(j=0;j<x_dimension;j++)
            	outFile << graphMatrix2[i][j];
        outFile << "\",\n";
	}
   outFile << "\"";
   for(j=0;j<x_dimension;j++)
   	{
        outFile << graphMatrix2[i][j];
        }
   outFile << "\"};\n";

   outFile.flush();
   outFile.close();

   cout << " done." << endl; 
}

/*
///////////////////////////////////////////////////////////////////////////////
//
// member function:	getPoint
// purpose:		returns color of point (v,w)
//
// author:		Andreas Starke
// last modified:	
//
///////////////////////////////////////////////////////////////////////////////

short printer::getPoint(XImage* xi, const int& v, const int& w)
{
	unsigned long color;
	short color_return;

	color=XGetPixel(xi,v,w);
	find_color(color, color_return);
//	cout << " | " << color << " "<< color_return << " | ";
//	cout << "| " << v << " " << w << "|";
	return color_return;
}

///////////////////////////////////////////////////////////////////////
//
// member function:	find_color
// pupose:		read out entry number for color
//
// author:		Andreas Starke
// last modified:
//
///////////////////////////////////////////////////////////////////////

void printer::find_color(const unsigned long &which, short& value)
{
	value=-1;
	parsi2.pixel=which;
	XQueryColor(base->displ, base->cmap, &parsi2);
	for (short i=0; i<95; i++){
		XParseColor(base->displ, base->cmap, base->Color[i], &parsi);
		if (parsi.red==parsi2.red)
		if (parsi.green==parsi2.green)
		if (parsi.blue==parsi2.blue){
			value=i;
			break;
		}
	}
}
*/
