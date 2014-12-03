/******************************************************************************/
/*                                                                            */
/* Module name:  printer.C                                                    */
/* Contents:     Member functions of the classes                              */
/*               1) printer (virtual base class)                              */
/*               2) pclPrinter                                                */
/*               3) psPrinter                                                 */
/*               4) deskjetColor                                              */
/*               5) XPMPrinter                                                */
/* Last modified: Sat Jun 19 14:37:23 CEST 1999 (Marc Mueller)                */
/*                                                                            */
/******************************************************************************/

#include "printer.h"
#include <string.h>
#include <stdio.h>
#include <iostream.h>
#include <iomanip.h>
#include <stdlib.h>  // for function abs()

static char ESC=(char)27;

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

    graphMatrixColor=new int[32]; // 32 colors are hardcoded jet
    if( graphMatrixColor == NULL ) 
        fatalError("printer::getMatrix","Can't allocate memory");

    for(j=0;j<32;j++)
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
    if( color > 31 )
	color=31;       
    
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
/* Last modified:   Thu Jun  3 17:19:48 CEST 1999  (Marc Mueller)           */
/*                                                                          */
/****************************************************************************/
void printer::drawLine(const real& ax, const real& ay,
                       const real& bx, const real& by, short color)
{
	short error,delta,schwelle,dx,dy,inc_x,inc_y;
	int x0,y0,x1,y1;
	real Ax,Ay,Bx,By;
	Ax=ax;Ay=ay;// because of the const real type
	Bx=bx;By=by;

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
/* Class name:      pclPrinter                                                */
/* Member function: pclPrinter                                                */
/* Purpose:         constructor 1                                             */
/* Last modified:   17.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

pclPrinter::pclPrinter(const resolution& myRes,const size& pSize,
                       const char* name,const xyRange& range) 
             : printer(myRes,pSize,name,range)
{
}

/******************************************************************************/
/*                                                                            */
/* Class name:      pclPrinter                                                */
/* Member function: pclprinter                                                */
/* Purpose:         constructor 3                                             */
/* Last modified:   17.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

pclPrinter::pclPrinter(const char* name,const xyRange& range) 
             : printer(name,range)
{
}

/******************************************************************************/
/*                                                                            */
/* Class name:      pclPrinter                                                */
/* Member function: resetPrinter                                              */
/* Purpose:         reset printer                                             */
/* Last modified:   11.01.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void pclPrinter::resetPrinter(ofstream& outFile)
{
    outFile << ESC << "E";
}

/******************************************************************************/
/*                                                                            */
/* Class name:      pclPrinter                                                */
/* Member function: setLines                                                  */
/* Purpose:         specify the end of line termination                       */
/* Last modified:   17.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void pclPrinter::setLines(ofstream& outFile)
{
    outFile << ESC << "&k2G";		// CR=CR, LF=CR+LF, FF=CR+FF
}

/******************************************************************************/
/*                                                                            */
/* Class name:      pclPrinter                                                */
/* Member function: setRasterRes                                              */
/* Purpose:         specify the desired resolution                            */
/* Last modified:   17.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void pclPrinter::setRasterRes(ofstream &outFile)
{
    outFile << ESC << "*t300R";       
}

/******************************************************************************/
/*                                                                            */
/* Class name:      pclPrinter                                                */
/* Member function: setLeftMargin                                             */
/* Purpose:         specify the desired left margin                           */
/* Last modified:   17.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void pclPrinter::setLeftMargin(ofstream& outFile)
{
    outFile << ESC << "&a9L";		// left margin: 9 columns
}

/******************************************************************************/
/*                                                                            */
/* Class name:      pclPrinter                                                */
/* Member function: setNumOfBytes                                             */
/* Purpose:         specify the amount of bytes that will be transferred      */
/* Last modified:   17.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void pclPrinter::setNumOfBytes(ofstream& outFile)
{
    char rowsChar[5];
    sprintf(rowsChar,"%d",rows);

    outFile << ESC << "*b" << rowsChar << "W";
}

/******************************************************************************/
/*                                                                            */
/* Class name:      pclPrinter                                                */
/* Member function: setRasterMode                                             */
/* Purpose:         specify the desired raster mode                           */
/* Last modified:   17.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void pclPrinter::setRasterMode(ofstream& outFile)
{
    outFile << ESC << "*r1A";		// start raster graphics at current
					// cursor position
}

/******************************************************************************/
/*                                                                            */
/* Class name:      pclPrinter                                                */
/* Member function: exitRasterMode                                            */
/* Purpose:         end raster graphics                                       */
/* Last modified:   17.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void pclPrinter::exitRasterMode(ofstream& outFile)
{
    outFile << ESC << "*rC";
}

/******************************************************************************/
/*                                                                            */
/* Class name:      pclPrinter                                                */
/* Member function: formFeed                                                  */
/* Purpose:         send form feed command to the printer                     */
/* Last modified:   17.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void pclPrinter::formFeed(ofstream& outFile)
{
    static char twelve=(char)12;
    outFile << twelve;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      pclPrinter                                                */
/* Member function: printToFile                                               */
/* Purpose:         print matrix that holds the graphics image into the file  */
/* Last modified:   17.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void pclPrinter::printToFile()
{
    ofstream outFile(fileName,ios::out);
    int i,j;
    unsigned char* help= new unsigned char[rows];
    
    
    if( !outFile.good() )
        fatalError("pclPrinter::print","Can't open output file");
    if( help == NULL )
        fatalError("pclPrinter::print","Can't allocate memory"); 

    cout << "Printing..." << endl;
    resetPrinter(outFile);
//    setLines(outFile);
    setRasterRes(outFile);
        setLeftMargin(outFile);
        setRasterMode(outFile);
    for(j=0;j<xDim;j++)
    {
        setNumOfBytes(outFile);

        for(i=0;i<rows;i++) {
            help[i]=graphMatrix[i][j];
//	    outFile << graphMatrix[i][j];
//	    cout << help[i] << "\t" << (int)(help[i]) << endl;
	}
        outFile.write(help,rows);
    }
        exitRasterMode(outFile);
    formFeed(outFile);
    delete help;
    outFile.flush();
    outFile.close();
}

/******************************************************************************/
/*                                                                            */
/* Class name:      psPrinter                                                 */
/* Member function: psPrinter                                                 */
/* Purpose:         constructor 1                                             */
/* Last modified:   17.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

psPrinter::psPrinter(const resolution& myRes,const size& pSize,const char* name,
                     const xyRange& range) 
        : printer(myRes,pSize,name,range)
{
}

/******************************************************************************/
/*                                                                            */
/* Class name:      psPrinter                                                 */
/* Member function: psPrinter                                                 */
/* Purpose:         constructor 3                                             */
/* Last modified:   17.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

psPrinter::psPrinter(const char* name,const xyRange& range) 
           : printer(name,range)
{
}

/******************************************************************************/
/*                                                                            */
/* Class name:      psPrinter                                                 */
/* Member function: drawAxes                                                  */
/* Purpose:         draw axes of the image and label them                     */
/* Last modified:   17.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void psPrinter::drawAxes(ofstream& outFile)
{
    outFile << "initgraphics" << endl;
    outFile << "0.8 setlinewidth" << endl;

    outFile << "85 762 moveto" << endl;	// first draw the axes
    outFile << "0 -674 rlineto" << endl;
    outFile << "79 756 moveto" << endl;
    outFile << "458 0 rlineto" << endl;
    outFile << "79 594 moveto 6 0 rlineto" << endl;
    outFile << "79 432 moveto 6 0 rlineto" << endl;
    outFile << "79 270 moveto 6 0 rlineto" << endl;
    outFile << "79 108 moveto 6 0 rlineto" << endl;
    outFile << "193 756 moveto 0 6 rlineto" << endl;
    outFile << "301 756 moveto 0 6 rlineto" << endl;
    outFile << "409 756 moveto 0 6 rlineto" << endl;
    outFile << "517 756 moveto 0 6 rlineto" << endl;
    outFile << "stroke" << endl;
    outFile << "85 88 moveto" << endl;
    outFile << "85 88 10 70 110 arc closepath fill" << endl; // draw the arrows
    outFile << "543 756 moveto" << endl;
    outFile << "543 756 10 160 200 arc closepath fill" << endl;

    outFile << "/Times-Roman findfont 10 scalefont setfont" << endl; 
					// now label the axes
    outFile << "82 85 rotRight" << endl;
    outFile << "(" << limits->label[0] << ") show" << endl; // x-label
    outFile << "90 rotate" << endl;
    outFile << "546 756 rotRight" << endl;
    outFile << "(" << limits->label[1] << ") show" << endl; // y-label
    outFile << "90 rotate" << endl;
    outFile << "70 756 rotRight" << endl;
    outFile << "(" << limits->min[0] << ") show" << endl;	  // x-min
    outFile << "90 rotate" << endl;

    outFile << "70 594 rotRight" << endl;                 // the x-stics
    outFile << limits->max[0]-limits->min[0] << " dup" << endl;
    outFile << " 0.25 mul " << limits->min[0] << " add" << endl;
    outFile << "st2Str show" << endl;
    outFile << "90 rotate" << endl;
    outFile << "70 432 rotRight" << endl;
    outFile << "dup" << endl;
    outFile << " 0.5 mul " << limits->min[0] << " add st2Str show" << endl;
    outFile << "90 rotate" << endl;
    outFile << "70 270 rotRight" << endl;
    outFile << " 0.75 mul " << limits->min[0] << " add st2Str show" << endl;
    outFile << "90 rotate" << endl;
    outFile << "70 108 rotRight" << endl;
    outFile << limits->max[0] << " st2Str show" << endl;    // x-max
    outFile << "90 rotate" << endl;

    outFile << "(" << limits->min[1] << ") dup" << endl;    // y-min
    outFile << "stringwidth  pop" << endl;
    outFile << "765 add 85 exch rotRight show" << endl;

    outFile << "90 rotate" << endl;
    outFile << limits->max[1]-limits->min[1] << " dup" << endl;
    outFile << "0.25 mul " << limits->min[1] << " add" << endl;
    outFile << "st2Str dup" << endl;
    outFile << "stringwidth pop" << endl;
    outFile << "765 add 193 exch rotRight show" << endl;

    outFile << "90 rotate" << endl;                       // the y-tics
    outFile << "dup" << endl;
    outFile << "0.5 mul " << limits->min[1] << " add st2Str dup" << endl;
    outFile << "stringwidth pop" << endl;
    outFile << "765 add 301 exch rotRight show" << endl;

    outFile << "90 rotate" << endl;
    outFile << " 0.75 mul " << limits->min[1] << " add st2Str dup" << endl;
    outFile << "stringwidth pop" << endl;
    outFile << "765 add 409 exch rotRight show" << endl;

    outFile << "90 rotate" << endl;
    outFile << limits->max[1] << " st2Str dup" << endl;     //y-max
    outFile << "stringwidth pop" << endl;
    outFile << "765 add 517 exch rotRight show" << endl;
    outFile << "90 rotate" << endl;
}


/******************************************************************************/
/*                                                                            */
/* Class name:      psPrinter                                                 */
/* Member function: printHeader                                               */
/* Purpose:         print the postscript header                               */
/* Last modified:   17.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void psPrinter::printHeader(ofstream& outFile)
{
    outFile << "%!PS-Adobe-2.0" << endl;
}
    
/******************************************************************************/
/*                                                                            */
/* Class name:      psPrinter                                                 */
/* Member function: defineProcs                                               */
/* Purpose:         define some useful postscript procedures                  */
/* Last modified:   17.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void psPrinter::defineProcs(ofstream& outFile)
{
    outFile << "/stStr 10 string def" << endl;      // string for top of stack
    outFile << "/st2Str { stStr cvs } def" << endl; // create string containing
                                                    // the top of the stack
    outFile << "/rotRight { moveto -90 rotate } def" << endl; 
}

/******************************************************************************/
/*                                                                            */
/* Class name:      psPrinter                                                 */
/* Member function: printToFile                                               */
/* Purpose:         print graphics image into the file                        */
/* Last modified:   17.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void psPrinter::printToFile()
{
    ofstream outFile(fileName,ios::out);
    int i,j;
    int i40=1;
    char text[20];
    
    if( !outFile.good() )
        fatalError("psPrinter::printToFile","Can't open output file");

    cout << "Printing..." << endl;
    printHeader(outFile);
    defineProcs(outFile);
    outFile << "initgraphics" << endl;
    outFile << "85 746.972222 translate" << endl;
    outFile << "432 9.027778 scale" << endl;
    outFile << "0 setgray" << endl;
    outFile << "3600 75" << endl;
    outFile << "true" << endl;
    outFile << "[3600 0 0 -75 0 75]" << endl;
    outFile << "{<";

    for(j=0;j<xDim;j++) {
	for(i=0;i<rows;i++) {
	    if ( i40 == 40 ) { 
		outFile << endl; 
		i40 = 1; 
	    }
	    else 
		i40++ ;
//	    outFile << hex << setfill('0') << setw(2) << graphMatrix[i][j];
	    if( graphMatrix[i][j] < 16 )
		outFile << "0";
	    sprintf(text,"%x",graphMatrix[i][j]);
	    outFile << text;
	}

	if( !((j+1) % 75)  && ((j+1) != xDim) ) {
	    outFile << ">}" << endl;
	    outFile << "imagemask" << endl;
	    outFile << "gsave" << endl;
	    if( !((j+1) % 750) ) {
		outFile << "grestoreall" << endl; 
		outFile << "gsave" << endl;
	    }
	    outFile << "initgraphics" << endl;
	    sprintf(text,"%.6f",756-9.027778*(((j+1)/75.0)+1));
	    outFile << "85 " << text << " translate" << endl;
	    outFile << "432  9.027778 scale" << endl;
	    outFile << "0 setgray" << endl;
	    outFile << "3600 75" << endl;
	    outFile << "true" << endl;
	    outFile << "[3600 0 0 -75 0 75]" << endl;
	    outFile << "{<";
	    i40 = 1;
	}
    }
    outFile << ">}" << endl;
    outFile << "imagemask" << endl;
    outFile << "gsave" << endl;
    drawAxes(outFile);
    outFile << "grestoreall" << endl;
    outFile << "showpage" << endl;
    outFile << "%%Trailer" << endl;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      deskjetColor                                              */
/* Member function: deskjetColor                                              */
/* Purpose:         constructor 1                                             */
/* Last modified:   11.01.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

deskjetColor::deskjetColor(const resolution& myRes,const size& pSize,
                       const char* name,const xyRange& range) 
             : pclPrinter(myRes,pSize,name,range)
{
    getColorMatrices();
}

/******************************************************************************/
/*                                                                            */
/* Class name:      deskjetColor                                              */
/* Member function: deskjetColor                                              */
/* Purpose:         constructor 3                                             */
/* Last modified:   11.01.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

deskjetColor::deskjetColor(const char* name,const xyRange& range) 
             : pclPrinter(name,range)
{
    getColorMatrices();
}

/******************************************************************************/
/*                                                                            */
/* Class name:      deskjetColor                                              */
/* Member function: ~deskjetColor                                             */
/* Purpose:         destructor                                                */
/* Last modified:   11.01.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

deskjetColor::~deskjetColor()
{
    for(int i=0; i< rows; i++){
        delete cyanMatrix[i];		// free allocated memory
        delete magentaMatrix[i];
        delete yellowMatrix[i];
    }
    delete cyanMatrix;
    delete magentaMatrix;
    delete yellowMatrix;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      deskjetColor                                              */
/* Member function: getColorMatrices                                          */
/* Purpose:         allocate memory to hold the color graphics image          */
/* Last modified:   11.01.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void deskjetColor::getColorMatrices()
{
    int j;

    cyanMatrix=new unsigned char*[rows];
    if( cyanMatrix == NULL ) 
        fatalError("deskjetColor::getColorMatrices","Can't allocate memory");

    magentaMatrix=new unsigned char*[rows];
    if( magentaMatrix == NULL ) 
        fatalError("deskjetColor::getColorMatrices","Can't allocate memory");

    yellowMatrix=new unsigned char*[rows];
    if( yellowMatrix == NULL ) 
        fatalError("deskjetColor::getColorMatrices","Can't allocate memory");

    for(int i=0; i<rows; i++) {
        cyanMatrix[i]=new unsigned char[xDim];
        if( cyanMatrix[i] == NULL ) 
            fatalError("deskjetColor::getColorMatrices",
		       "Can't allocate memory");
        magentaMatrix[i]=new unsigned char[xDim];
        if( magentaMatrix[i] == NULL ) 
            fatalError("deskjetColor::getColorMatrices",
		       "Can't allocate memory");
        yellowMatrix[i]=new unsigned char[xDim];
        if( yellowMatrix[i] == NULL ) 
            fatalError("deskjetColor::getColorMatrices",
		       "Can't allocate memory");
    
        for(j=0;j<xDim;j++){
            cyanMatrix[i][j]=0;
            magentaMatrix[i][j]=0;
            yellowMatrix[i][j]=0;
	}
    }
}

/******************************************************************************/
/*                                                                            */
/* Class name:      deskjetColor                                              */
/* Member function: setNumberOfPlanesPerRow                                   */
/* Purpose:         use the KCMY palette to print graphics                    */
/* Last modified:   11.01.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void deskjetColor::setNumberOfPlanesPerRow(ofstream& outFile)
{
    outFile << ESC << "*r-4U";	// KCMY palette
}

/******************************************************************************/
/*                                                                            */
/* Class name:      deskjetColor                                              */
/* Member function: setRasterGraphicsDepletion                                */
/* Purpose:         set depletion to 25%                                      */
/* Last modified:   11.01.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void deskjetColor::setRasterGraphicsDepletion(ofstream& outFile)
{
    outFile << ESC << "*o2D";	// 25% depletion
}

/******************************************************************************/
/*                                                                            */
/* Class name:      deskjetColor                                              */
/* Member function: setRasterGraphicsShingling                                */
/* Purpose:         use 4 pass printing (high qualtity)                       */
/* Last modified:   11.01.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void deskjetColor::setRasterGraphicsShingling(ofstream& outFile)
{
    outFile << ESC << "*o2Q";	// 4 pass printing
}

/******************************************************************************/
/*                                                                            */
/* Class name:      deskjetColor                                              */
/* Member function: setRasterGraphicsQualtity                                 */
/* Purpose:         use high qualtity printing                                */
/* Last modified:   11.01.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void deskjetColor::setRasterGraphicsQuality(ofstream& outFile)
{
    outFile << ESC << "*r2Q";	// high qualtity
}

/******************************************************************************/
/*                                                                            */
/* Class name:      deskjetColor                                              */
/* Member function: setDataByPlane                                            */
/* Purpose:         specify the amount of bytes that will be transferred      */
/* Last modified:   11.01.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void deskjetColor::setDataByPlane(ofstream& outFile)
{
    char rowsChar[5];
    sprintf(rowsChar,"%d",rows);

    outFile << ESC << "*b" << rowsChar << 'V';
}

/******************************************************************************/
/*                                                                            */
/* Class name:      deskjetColor                                              */
/* Member function: setBits                                                   */
/* Purpose:         set bits in the matrix that holds the graphics image (2D) */
/* Last modified:   11.01.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void deskjetColor::setBits(real& x, real& y)
{
    pclPrinter::setBits(x,y);
}

/******************************************************************************/
/*                                                                            */
/* Class name:      deskjetColor                                              */
/* Member function: setBits                                                   */
/* Purpose:         set bits in the matrix that holds the graphics image (2D) */
/* Last modified:   11.01.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void deskjetColor::setBits(real& x, real& y,real& z)
{
    pclPrinter::setBits(x,y,z);
}

/******************************************************************************/
/*                                                                            */
/* Class name:      deskjetColor                                              */
/* Member function: setBits                                                   */
/* Purpose:         set bits in the matrix that holds the graphics image (2D) */
/* Last modified:   11.01.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void deskjetColor::setBits(real& x, real& y,int color)
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
	  //fatalError("deskjetColor::setBits","Error computing row");
    if( (col <0) || (col >= xDim) ) 
	  return;//tw
	  //fatalError("deskjetColor::setBits","Error computing column");

    switch( color ) {
	case Yellow: yellowMatrix[rowDiv][col] |= (1 << (7-rowMod));
	    break;
	case Magenta:magentaMatrix[rowDiv][col] |= (1 << (7-rowMod));
	    break; 
	case Red:magentaMatrix[rowDiv][col] |= (1 << (7-rowMod));
	    yellowMatrix[rowDiv][col] |= (1 << (7-rowMod));
	    break; 
	case Cyan:cyanMatrix[rowDiv][col] |= (1 << (7-rowMod));
	    break; 
	case Green:cyanMatrix[rowDiv][col] |= (1 << (7-rowMod));
	    yellowMatrix[rowDiv][col] |= (1 << (7-rowMod));
	    break; 
	case Blue:cyanMatrix[rowDiv][col] |= (1 << (7-rowMod));
	    magentaMatrix[rowDiv][col] |= (1 << (7-rowMod));
	    break; 
	case CompBlack:cyanMatrix[rowDiv][col] |= (1 << (7-rowMod));
	    magentaMatrix[rowDiv][col] |= (1 << (7-rowMod));
	    yellowMatrix[rowDiv][col] |= (1 << (7-rowMod));
	    break; 
	case Black: 
        default:graphMatrix[rowDiv][col] |= (1 << (7-rowMod));
            break;
    };
}
/******************************************************************************/
/*                                                                            */
/* Class name:      deskjetColor                                              */
/* Member function: printToFile                                               */
/* Purpose:         print graphics image into the file                        */
/* Last modified:   11.01.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/

void deskjetColor::printToFile()
{
    ofstream outFile(fileName,ios::out);
    int i,j;
    unsigned char* help= new unsigned char[rows];
    
    
    if( !outFile.good() )
        fatalError("pclPrinter::print","Can't open output file");
    if( help == NULL )
        fatalError("deskjetColor::print","Can't allocate memory"); 

    cout << "Printing..." << endl;
    resetPrinter(outFile);
    setNumberOfPlanesPerRow(outFile);
    setRasterRes(outFile);
    setRasterGraphicsQuality(outFile);
    setRasterGraphicsDepletion(outFile);
    setRasterGraphicsShingling(outFile);
//    setLines(outFile);
	
    setLeftMargin(outFile);
    setRasterMode(outFile);

    for(j=0;j<xDim;j++)
    {
        setDataByPlane(outFile);

        for(i=0;i<rows;i++) {
            help[i]=graphMatrix[i][j];
	}
        outFile.write(help,rows);

        setDataByPlane(outFile);

        for(i=0;i<rows;i++) {
            help[i]=cyanMatrix[i][j];
	}
        outFile.write(help,rows);

        setDataByPlane(outFile);

        for(i=0;i<rows;i++) {
            help[i]=magentaMatrix[i][j];
	}
        outFile.write(help,rows);

        setNumOfBytes(outFile);

        for(i=0;i<rows;i++) {
            help[i]=yellowMatrix[i][j];
	}
        outFile.write(help,rows);
    }
    exitRasterMode(outFile);
    formFeed(outFile);
    delete help;
    outFile.flush();
    outFile.close();
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

    graphMatrixColor=new int[32]; // 32 colors are hardcoded jet
    if( graphMatrixColor == NULL ) 
        fatalError("printer::getMatrix","Can't allocate memory for graphMatrixColor");

    for(i=0; i<32; i++)
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

    if( color > 31 )
	color=31;
    
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
    graphMatrixColor[color]=1;	// sign which color is used
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
    char *colorCodes[35];
        colorCodes[0]= "FFFFFF\0";
	if((jobType==COLOR_PLOT)||(jobType==COLOR_PLOT_B))
          {
		colorCodes[1]="8B0000\0";	//"red4";
		colorCodes[2]="FF0000\0";	//"red";
		colorCodes[3]="FF4500\0";	//"orange red";
		colorCodes[4]="FF8C00\0";	//"dark orange";
		colorCodes[5]="FFD700\0";	//"gold";
		colorCodes[6]="FFFF00\0";	//"yellow";
		colorCodes[7]="ADFF2F\0";	//"green yellow";
		colorCodes[8]="00CD00\0";	//"green3";
		colorCodes[9]="54FF9F\0";	//"SeaGreen1";
		if((jobType==COLOR_PLOT_B))
			colorCodes[9]="000000\0";//"black";
		colorCodes[10]="66CDAA\0";	//"aquamarine3";
		colorCodes[11]="20B2AA\0";	//"light sea green";
		colorCodes[12]="009ACD\0";	//"DeepSkyBlue3";
		colorCodes[13]="0000FF\0";	//"blue";
		colorCodes[14]="3A5FCD\0";	//"RoyalBlue3";
		colorCodes[15]="6959CD\0";	//"SlateBlue3";
		colorCodes[16]="8A2BE2\0";	//"blue violet";
		colorCodes[17]="9400D3\0";	//"DarkViolet";

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

          }
  
        if((jobType==CONTOURLINE)||(jobType==CONTOURLINE_B))
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
		if((jobType==CONTOURLINE_B))
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
   int orders[32];
   orders[0]=0; //white background
   i=1; 
   for(j=1;j<32;j++)	//count colors
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
   for(j=0;j<32;j++)
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
