#ifndef SIM_H
#define SIM_H

#include<stdio.h>
#include<string.h>
#include<math.h>


// enum BOOLS { FALSE, TRUE, NOTSET, EQUALSET, USERBREAK, STREAMERROR };
//enum ADJUST { OLDADJUST, WALRASIAN, EXPECTEDOLD, NEWADJUST };
//enum OUTMOD { OLD, NEW };
enum window { NOWINDOW,PARZEN,HANNING,WELCH,HAMMING,EXACTBLACKMAN };
enum printMode { NONE, HP, PS, COLOR, XPM=10, XPMC=11 };
enum colors { White, Yellow, Magenta, Red, Cyan, Green, Blue, CompBlack, Black};

#define ROWS 500                           /* Rows containing PCL information */
#define WIDTH ROWS * 8                     /* No. of bits in the rows         */
#define COLUMNS 5000                       /* No. of columns                  */
#define WINWIDTH 600                       /* Width of the graphics window    */
#define WINHEIGHT 300                      /* Height of the graphics window   */
#define XPM_XSIZE 500
#define XPM_YSIZE 260                      /*Default Size for XPM-File (TAG 11) */
#define DEFAULT_XPM_COLOR 24;              /* Default color for non-color prints */
#define EPSILON 1e-20

////////////////////////////////////////////////////////////////////////
//
// Job definitions begin
//
////////////////////////////////////////////////////////////////////////

#define BIF2D	1	/* 2D Bifurcation diagram    */

#define BIF2PAR	3
#define BIFNPARAM	33 /* Bifurcation with an xBundle*/
#define D_BIFNPARAM	35 /* bundle bifurcation with density */

#define ATTRA	4	/* Attractor                 */
#define BASIN	5	/* Basin of attraction       */
#define BASIN2	6	/* Basin of attraction for 2 cycles*/

#define HORSE	7	/* Horseshoe diagram         */

#define POWER	8	/* Powerspectrum             */

#define PARSPACE	9	/* Parameter space diagram   */
#define CELLATTRA	10
#define CELLBIF2D	11
#define SIMPLECELL	12	// simple cell mapping
#define STATEANAL	13	// analysis of the state space 

#define DISCBIF2D	14	// bifurcation diagram for a discrete parameter 
#define NPARMAP		15	// parameter map for n parameters

#define TSPLOT		16	// time series plot
#define M_TSPLOT	17	// time series plot with multiple initial values
#define M_TSPLOT_v	18	// time series plot with multiple initial values of one specified variable
#define TSPLOT_LOOP	19	// multiple time series plots for different path of random variables

#define MAX_LAYP_EXP_1D	21	// print the maximal lyapunov exponent on the y-axis
#define MAX_LAYP_EXP_2D	22	// encode the maximal lyapunov exponent
#define MAX_LAYP_EXP_T	23	// print the trend of the lyapunov exponent

#define BIF3D_1PAR	24	// 3D bifurcation data
#define BIF3D_2PAR 	25	// 3D Bifurcation data	     
#define CYCLO3D		26	// 3D cyclogram data

#define D_BIF2D	       		31      // density 2D Bifurcation diagram
#define D_BIF2D_F2		32      // density 2D Bifurcation diagram for f^2
#define D_BIF2D_F4		34      // density 2D Bifurcation diagram for f^4

#define DENSITY_1D		41      // density plot
#define DENSITY_1D_1P		42      // a sequence of density plots variing 1 parameter
#define DENSITY_1D_VAR		43	// plots density of variable at T for varying parameter

#define INDICATOR_2D		51      // indicator plot in 2 dimensions

#define D_ATTRA			61	// density Attractor plot
#define L_ATTRA			62	// Attractor plot with points connected by lines
#define ATTRA3D			63	// 3d attractor plot

#define PHASE_PLOT		70	// plot x_t vs x_{t+k} 

#define COLOR_PLOT		80	// rainbow with variable size of points
#define COLOR_PLOT_B	81	        // rainbow with black line with variable size of points
#define CONTOURLINE		82	// plot of contour lines
#define CONTOURLINE_B	83	// plot of contour lines with black line
#define CONTOURLINE_C   84	// plot of contour lines without colorrange
#define PROBABILITY	    85	// plot of probability
#define DISTRIBUTION	86	// plot of distribution
#define ACF_PLOT	87	// plot of estimated autocorrelationfunction
#define PACF_PLOT	88	// plot of estimated partial autocorrelationfunction

#define MDMAP		90	// model cartography
#define TSSET		91	// creating data file for multiple time Series

////////////////////////////////////////////////////////////////////////
//
// Job definitions end
//
////////////////////////////////////////////////////////////////////////

#define maxtau 10
#define MAX(X,Y) ( (X) > (Y) ? (X) : (Y) )
#define MIN(X,Y) ( (X) < (Y) ? (X) : (Y) )

//#ifndef KSR
//#define __shared
//#define __private
//#endif

#endif








