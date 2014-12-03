#ifndef PRINTER_H
#define PRINTER_H

/******************************************************************************/
/*                                                                            */
/* Module name:  printer.h                                                    */
/* Contents:     Class definitions                                            */
/*               1) printer                                                   */
/*               2) pclPrinter                                                */
/*               3) psPrinter                                                 */
/*               4) colorDeskjet                                              */
/*               5) XPM-Printer                                               */
/*                                                                            */
/* Last Modified: Thu Jun  3 19:55:17 CEST 1999   (Marc Mueller)              */
/*                                                                            */
/******************************************************************************/

#include "../Models/baseModel.h"
#include "axes.h"
#include "../Graphics/macrodyngraphicsitem.h"
#include <fstream>

enum resolution { medium=300, high=600 };
enum size { A4, A4_2 };

/******************************************************************************/
/*                                                                            */
/* Class name:    printer                                                     */
/* Derived from:  -                                                           */
/* Purpose:       defines a default printer (virtual base class)              */
/*                                                                            */
/* Last modified: Mon Nov 10 15:59:46 MET 1997 (Marc Mueller)                 */
/*                                                                            */
/******************************************************************************/


class printer {
  private:
    printer(const printer&);            // Hide copy constructor and
    printer& operator=(const printer&); // operator =
    void getMatrix();                   // Allocate Memory
    void freeMatrix();                  // Free allocated memory
    void pageSetup(const size&, const resolution&);
    
  protected:
    void computeBits(qreal&,qreal&,int&,int&);
    void computeBits(qreal&,qreal&,qreal&,int&,int&);
    size pageSize;               // DIN A4 or A4_2
    resolution dpi;              // 300dpi or 600dpi
    unsigned short xDim;         // #dots in X-direction
    unsigned short yDim;         // #dots in Y-direction
    unsigned short rows;         // #rows in graphMatrix (yDim/8)
    unsigned char **graphMatrix; // Matrix to store graphic
    int *graphMatrixColor;       // Matrix to store graphic color
    char fileName[1024];         // Output filename
    xyRange* limits;             // Describes area to be printed
    
  public:
    printer();
    printer(const resolution,const size&,const char*,const xyRange&);
    printer(const char*,const xyRange&);
    virtual ~printer();          // Destructor
    
    void reset();                // Reset graphMatrix
    void setResolution(const resolution&);
    void setSize(const size&);
    void setLimits(const xyRange&);
    void set_axis(int, qreal, qreal);

//	short getPoint(XImage* , const int&, const int& );			
//	void find_color(const unsigned long&, short&);
    
    virtual void setBits(qreal&,qreal&);
    virtual void setBits(qreal&,qreal&,qreal&);
    virtual void setBits(qreal&,qreal&,int);
    virtual void setBit(int&,int&,short);
    virtual void drawLine(const qreal&, const qreal&,
                  const qreal&, const qreal&, short);
    virtual void printToFile()=0;      // Print graphic into file
    virtual void screenshot(MacrodynGraphicsItem* grafik, char* filename="screenshot.xpm")=0;
    virtual void screenshot_to_file(unsigned char** graphMatrix2, int* graphMatrixColor2,
    char* filename="screenshot.xpm")=0;  
    
    unsigned char isOk(qreal&,qreal&);
    unsigned char isOk(qreal&,qreal&,qreal&);
    
//    static XColor parsi;
	
    virtual void setBigPoint(const qreal&,const qreal&,int,int&);
	
};

/******************************************************************************/
/*                                                                            */
/* Class name:    xpmPrinter                                                  */
/* Derived from:  printer                                                     */
/* Purpose:       defines xpm-Graphics Printer                                */
/*                                                                            */
/* Last modified: Sat Jun 19 12:41:51 CEST 1999 (Marc Mueller)                */
/*                                                                            */
/******************************************************************************/

class xpmPrinter : public printer {
private:
    void getMatrix();                   // Allocate Memory
    void computeBits(qreal& x,qreal& y,int& row, int& col); // used automatical adjusting
    void computeBits(qreal& x,qreal& y,qreal &z,int& row, int& col); // not working, just testing
    char **multiplotPtrs;
    int multiplot_num;
    int jobType;
public:
    xpmPrinter(const char*,const xyRange&,char **const,const int&,const int&);
    xpmPrinter(const xyRange&, const int&);
    virtual ~xpmPrinter();
    void printToFile();
    void screenshot(MacrodynGraphicsItem* grafik, char* filename="screenshot.xpm");
    void screenshot_to_file(unsigned char** graphMatrix2, int* graphMatrixColor2,
    char* filename="screenshot.xpm");  
    
    void setBits(qreal&,qreal&,int);
    void setBits(qreal&,qreal&);
    void setBits(qreal&,qreal&,qreal&);
    void setBigPoint(const qreal&,const qreal&,int,int&);
};

#endif
