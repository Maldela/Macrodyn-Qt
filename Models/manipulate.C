////////////////////////////////////////////////////////////////////////////////
//                                                                             
// Module name:  manipulate.C                                                  
// Author:       Marc Mueller                                                  
//                                                                             
// Contents:     Member functions of the classes manipulate                    
//                                                                             
// Modified:     Jul 12 1999 by Marc Mueller                                   
// Last Modified: 20.6.2000                                 
//                                                                             
////////////////////////////////////////////////////////////////////////////////

#include "../error.h"
#include "manipulate.h"
#include "getModel.h"
#include "../PublicModels/getPubModel.h"
#include <fstream>

#include "../Random/ranlib.h"

////////////////////////////////////////////////////////////////////////////////
// Class name:		manipulate
// Member function:	manipulate
// Purpose:		constructor
// Author:		Marc Mueller  Jul 12 1999
// Last modified:	2000/6/20
////////////////////////////////////////////////////////////////////////////////
manipulate::manipulate() : baseModel(0) { // dimension is set zero! do not use
	model = NULL;			  // this basemodel variable in a job!

	zvar = NULL;
	markov = NULL;
    zvar_ptr = NULL;
	randname = NULL;

	errcor_ptr = NULL;	
	ma_ptr = NULL;	
	ma_k_ptr = NULL;
	ma_k_a_ptr = NULL;

	olsStart=0;
	OLS = NULL;
	RLS = NULL;
	ELS = NULL;
	SG = NULL;
	tstart = NULL;

	statvar = NULL;

	quanvar = NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Class name:		manipulate
// Member function:	manipulate
// Purpose:		destructor
// Author:		Marc Mueller  Jul 12 1999
// Last modified:	2000/6/20
///////////////////////////////////////////////////////////////////////////////
manipulate::~manipulate() {
log() << "manipulate destructor" << "\n";

	if( model ) delete model;
	if( manipulateTag ) delete [] manipulateTag;

	if( zvar ) delete zvar;
	if( zvar_ptr) delete zvar_ptr;
	if( randname) delete randname;
	if( markov ) delete markov;

	if( errcor_ptr ) delete [] errcor_ptr;
	if( ma_ptr ) delete [] ma_ptr;
	if( ma_k_ptr ) delete [] ma_k_ptr;
	if( ma_k_a_ptr ) delete [] ma_k_a_ptr;

	if( OLS ) delete OLS;
	if( RLS ) delete RLS;
	if( ELS ) delete ELS;
	if( SG ) delete SG;
	if( tstart ) delete [] tstart;

	if( statvar ) delete statvar;
	if( quanvar ) delete quanvar;
}

///////////////////////////////////////////////////////////////////////////////
// Class name:		manipulate
// Member function:     setLabels
// Purpose:             return a pointer to a variable or a parameter specified
// Last modified:	2000/2/12  Marc Mueller
///////////////////////////////////////////////////////////////////////////////

qreal* manipulate::setLabels(char* label) {
	char* labelp;
	if( !strcmp(label,"102[0]k") ) return((qreal*)(&ma_k_a_ptr[0].k));
	if( !strcmp(label,"102[0]a") ) return(&ma_k_a_ptr[0].a);
	if( !strcmp(label,"MA_Trend") ) return(&ma_k_a_ptr[0].a);

	if( !strncmp(label,"m(",2) ) {
		labelp=label+2;
		if( !strncmp(labelp,statvarname,strlen(statvarname)) )
			return(&mean);
	}
	if( !strncmp(label,"v(",2) ) {
		labelp=label+2;
		if( !strncmp(labelp,statvarname,strlen(statvarname)) )
			return(&variance);
	}
	if( !strncmp(label,"s(",2) ) {
		labelp=label+2;
		if( !strncmp(labelp,statvarname,strlen(statvarname)) )
			return(&standardDeviation);
	}
	if( !strncmp(label,"vk(",3) ) {
		labelp=label+3;
		if( !strncmp(labelp,statvarname,strlen(statvarname)) )
			return(&variationskoeff);
	}
	if( !strncmp(label,"sr(",3) ) {
		labelp=label+3;
		if( !strncmp(labelp,statvarname,strlen(statvarname)) )
			return(&sharpeRatio);
	}
	if( !strncmp(label,"mean(",5) ) {
		labelp=label+5;
		if( !strncmp(labelp,quanvarname,strlen(quanvarname)) )
			return(&mr[0]);
	}
	if( !strncmp(label,"var(",4) ) {
		labelp=label+4;
		if( !strncmp(labelp,quanvarname,strlen(quanvarname)) )
			return(&mc[1]);
	}
	if( !strncmp(label,"sd(",3) ) {
		labelp=label+3;
		if( !strncmp(labelp,quanvarname,strlen(quanvarname)) )
			return(&sd);
	}

	if( !strncmp(label,"sk(",3) ) {
		labelp=label+3;
		if( !strncmp(labelp,quanvarname,strlen(quanvarname)) )
			return(&sk);
	}
	if( !strncmp(label,"ku(",3) ) {
		labelp=label+3;
		if( !strncmp(labelp,quanvarname,strlen(quanvarname)) )
			return(&ku);
	}
	if( !strncmp(label,"quan(",5) ) {
		labelp=label+5;
		if( !strncmp(labelp,quanvarname,strlen(quanvarname)) )
			return(&xi);
	}
	return model->setLabels(label);
}

long  manipulate::getLength() const {return model->getLength();}
qreal* manipulate::sendModelVar() {return model->sendModelVar();}
void  manipulate::sendStateSpace(int &i,const qreal*** r) {model->sendStateSpace(i,r);}
void  manipulate::receiveParameters(const qreal* r) {model->receiveParameters(r);} 
void  manipulate::sendParameters(int& i,qreal** r) {model->sendParameters(i,r);}
void  manipulate::printParamset() {model->printParamset();}
void  manipulate::saveParamset(ofstream& os) {model->saveParamset(os);}

///////////////////////////////////////////////////////////////////////////////
// Class name:		manipulate
// Member function:     saveParamsetWithNames
// Purpose:             add  parameterset to printfile
//
// Author:		Marc Mueller  Jul 12 1999
// Last modified:	Mon Jul 12 12:11:54 CEST 1999  Marc Mueller
///////////////////////////////////////////////////////////////////////////////
void manipulate::saveParamsetWithNames(ofstream& outputFile) {
	outputFile << "\nManipulate Model\n";
	model->saveParamsetWithNames(outputFile);
}

///////////////////////////////////////////////////////////////////////////////
// Class name:		manipulate
// Member function:	loadParamset
// Purpose:		load a parameterset from a specified input file
//
// Author:		Marc Mueller  Jul 12 1999
// Last modified:	2000/5/25
///////////////////////////////////////////////////////////////////////////////
void manipulate::loadParamset(ifstream& inFile) {
	int i,j;
	char* name;
	char dummy[256];
	char dummy2[1024];
	forecasttype=1; // normal forecast is default

	inFile >> modelTyp;
	model = getModel(modelTyp);
	if(!model) {
		model = getPubModel(modelTyp);
		if(!model)
			fatalError("manipulate::loadParamset  Can't allocate model ",modelTyp);
	}
	model->loadParamset(inFile);
	length = model->getLength();

	inFile >> howMany;
	manipulateTag=new int[howMany];
	if(!manipulateTag)
		fatalError("manipulate::loadParamset ","Can't allocate manipulateTag");

	for (int hm=0;hm<howMany;hm++) {
		inFile >> manipulateTag[hm];
log() << "manipulateTag[" << hm << "]" << manipulateTag[hm] << "\n";
		switch( manipulateTag[hm] ) {

		  case stoch_uni:
			inFile >> varname;
			randname=model->setLabels(varname);
			if( !randname )
				fatalError("manipulate::loadParamset stoch_uni can't find",varname);
			inFile >> zvar_expr;
			break;

		  case stoch_markov:
			// 11 expectationname variancename randomname
			//    STATES TRANSFORMATIONMATRIX
			// Bsp.:
			//	11 Ed Vd d
			//	{0.01;0.25;0.5;}
			//	 0.5;0.0;0.5;
			//	 1.0;0.0;0.0;
			//	 0.0;1.0;0.0;
			// auslesen der Zustaende und der Uebergangsmatrix
			// 	statesNum	Anzahl der Zustaende
			// 	states[32]	Zustaende als qreal-Feld angeordnent
			// 	umatrix[32][32]	Uebergangsmatrix als qreal-Matrix
			// wegen Uli Middelberg wird fuer die Schnittstelle noch benoetigt
			// 	m_state[256]	Zustaende als Zeichenkette
			// 	m_matrix[1024]	Uebergangsmatrix als Zeichenkette
			inFile >> varname;
			expname=model->setLabels(varname);
			if( !expname )
				fatalError("manipulate::loadParamset stoch_markov can't find",varname);

			inFile >> varname;
			variancename=model->setLabels(varname);
			if( !variancename )
				fatalError("manipulate::loadParamset stoch_markov can't find",varname);

			inFile >> varname;
			randname=model->setLabels(varname);
			if( !randname )
				fatalError("manipulate::loadParamset stoch_markov can't find",varname);

			inFile >> m_state;
			statesNum = strnchr(m_state,';');
			char sdummy[256];
			char * pos;
			char * pos2;
			pos=m_state;
			for (i = 0; i < statesNum; i++) { // konvertiere States aus char in qreal
				pos2=sdummy;
				while(*++pos!=';')
					*pos2++ = *pos;
				*pos2 = '\0';
				states[i]=atof(sdummy); // trage States in vektor ein
				//log() << sdummy << " " << states[i] << "\n";
			}
			for (j = 0; j < statesNum; j++) { // Uli liest Uebergangsmatrix zeilenweise ein!
				inFile >> dummy;	
				pos=dummy-1;
				for (i = 0; i < statesNum; i++) {
					pos2=sdummy;
					while(*++pos!=';')
						*pos2++ = *pos;
					*pos2 = '\0';
					umatrix[j][i]=atof(sdummy); // trage Uebergangsmatrix ein
					//log() << sdummy << " " << umatrix[j][i] << "\n";
				}
				strcat(m_matrix,dummy); // anhaengen der Zeilen !
				strcat(m_matrix," ");		
			}
			break;
		  case stoch_ar: 
			// 12 expectationname variancename randomname gammavalue zetaminvalue zetamaxvalue
			// Bsp.: 12 Ed Vd d 0.75 0.01 0.13
			inFile >> varname;
			expname=model->setLabels(varname);
			if( !expname )
				fatalError("manipulate::loadParamset stoch_ar can't find",varname);

			inFile >> varname;
			variancename=model->setLabels(varname);
			if( !variancename )
				fatalError("manipulate::loadParamset stoch_ar can't find",varname);

			inFile >> varname;
			randname=model->setLabels(varname);
			if( !randname )
				fatalError("manipulate::loadParamset stoch_ar can't find",varname);
			inFile >> gamma;
			inFile >> zetamin;
			inFile >> zetamax;
			break;
		  case errcor: // 50 num price forecast alpha
			inFile >> errcor_num;		  
			errcor_ptr=new struct_errcor[errcor_num];
			if(!errcor_ptr)
				fatalError("manipulate::loadParamset ","Can't allocate errcor_prt");
			for (int j=0;j<errcor_num;j++) {
				inFile >> varname;			
				errcor_ptr[j].forecast = model->setLabels(varname);
				if( !errcor_ptr[j].forecast )
				  fatalError("manipulate::loadParamset errcor can't find ",varname);
				inFile >> varname;			
				errcor_ptr[j].price = model->setLabels(varname);
				if( !errcor_ptr[j].price )
				  fatalError("manipulate::loadParamset errcor can't find ",varname);
				inFile >> errcor_ptr[j].alpha;
			}
			break;
		  case ma: // 100 num outputvar_1 inputvar_1 ... outputvar_num inputvar_num 
			inFile >> ma_num;		  
			ma_ptr=new struct_ma[ma_num];
			if(!ma_ptr)
				fatalError("manipulate::loadParamset ",
					   "Can't allocate ma_prt");
			for (int j=0;j<ma_num;j++) {
				inFile >> varname;			
				ma_ptr[j].mname = model->setLabels(varname);
				if( !ma_ptr[j].mname )
				    fatalError("manipulate::loadParamset ma can't find ",
						varname);
				inFile >> varname;			
				ma_ptr[j].vname = model->setLabels(varname);
				if( !ma_ptr[j].vname )
				    fatalError("manipulate::loadParamset ma can't find ",
						varname);
			}
			break;
		  case ma_k: // 101 num outputvar_1 k_1 inputvar_1 ... outputvar_num k_num inputvar_num 
			inFile >> ma_k_num;		  
			ma_k_ptr=new struct_ma_k[ma_k_num];
			if(!ma_k_ptr)
				fatalError("manipulate::loadParamset ",
					   "Can't allocate ma_k_prt");
			for (int j=0;j<ma_k_num;j++) {
				inFile >> varname;			
				ma_k_ptr[j].mname = model->setLabels(varname);
				if( !ma_k_ptr[j].mname )
				    fatalError("manipulate::loadParamset ma_k can't find ",
						varname);
				inFile >> ma_k_ptr[j].k;
				inFile >> varname;			
				ma_k_ptr[j].vname = model->setLabels(varname);
				if( !ma_k_ptr[j].vname )
				    fatalError("manipulate::loadParamset ma_k can't find ",
						varname);
				ma_k_ptr[j].vn_k=new qreal[ma_k_ptr[j].k];
				if( !ma_k_ptr[j].vn_k )
				    fatalError("manipulate::loadParamset ma_k ",
						"Can't allocate ma_k_ptr[j].vn_k");				
			}
			break;
		  case ma_k_a: // 102 num outputvar_1 k_1 inputvar_1 a_1 ... outputvar_num k_num inputvar_num a_num
			inFile >> ma_k_a_num;		  
			ma_k_a_ptr=new struct_ma_k_a[ma_k_a_num];
			if(!ma_k_a_ptr)
				fatalError("manipulate::loadParamset ",
					   "Can't allocate ma_k_a_prt");
			for (int j=0;j<ma_k_a_num;j++) {
				inFile >> varname;			
				ma_k_a_ptr[j].mname = model->setLabels(varname);
				if( !ma_k_a_ptr[j].mname )
				    fatalError("manipulate::loadParamset ma_k_a can't find ",
						varname);
				inFile >> ma_k_a_ptr[j].k;
				inFile >> varname;			
				ma_k_a_ptr[j].vname = model->setLabels(varname);
				if( !ma_k_a_ptr[j].vname )
				    fatalError("manipulate::loadParamset ma_k_a can't find ",
						varname);
				ma_k_a_ptr[j].vn_k=new qreal[ma_k_a_ptr[j].k];
				if( !ma_k_a_ptr[j].vn_k )
				    fatalError("manipulate::loadParamset ma_k_a ",
						"Can't allocate ma_k_a_ptr[j].vn_k");				
				inFile >> ma_k_a_ptr[j].a;
			}
			break;
		  case rls0:	// 98 scalar inputvector outputvector
				// 98 g_0 p_tm1 p_e_tp1
			inFile >> g_0; // =g_tm1
			inFile >> varname;
			p_tm1 = model->setLabels(varname); // get pointer
			if( !p_tm1 )
				fatalError("manipulate::loadParamset rls0 can't find ",
						  varname);
			inFile >> varname;
			p_e_tp1 = model->setLabels(varname); // get pointer
			if( !p_e_tp1 )
				fatalError("manipulate::loadParamset rls0 can't find ",
						  varname);
			break;			
		  case rls1: // 99  g_0 theta_e_tp1 theta_e_t theta_tm1
			inFile >> g_tm1;
			inFile >> varname;
			theta_e_tp1 = model->setLabels(varname); // get pointer
			if( !theta_e_tp1 )
				fatalError("manipulate::loadParamset rls1 can't find ",
						  varname);
			inFile >> varname;
			theta_e_t = model->setLabels(varname); // get pointer
			if( !theta_e_t )
				fatalError("manipulate::loadParamset rls1 can't find ",
						  varname);
			inFile >> varname;
			theta_tm1 = model->setLabels(varname); // get pointer
			if( !theta_tm1 )
				fatalError("manipulate::loadParamset rls1 can't find ",
						  varname);
			break;			
		  case ols:
			OLS = new olsClass(inFile,model);
			if( !OLS )
				fatalError("manipulate::loadParamset ","no ols pointer");
			inFile >> varname; // not evaluated yet
			break;			
		  case rls: 
			RLS = new rlsClass(inFile,model);
			if( !RLS )
				fatalError("manipulate::loadParamset ","no rls pointer");
			inFile >> varname;
			if( !strcmp(varname,"F2") )
				forecasttype=2;
			break;			
		  case rls1d:
			inFile >> P;
			inFile >> varname;
			q = model->setLabels(varname); // get pointer
			if( !q )
				fatalError("manipulate::loadParamset rls1d can't find ",
						  varname);
			inFile >> varname;
			p = model->setLabels(varname); // get pointer
			if( !p )
				fatalError("manipulate::loadParamset rls1d can't find ",
						  varname);
			inFile >> varname;
			forecasttype=1; // normal forecast is default
			if( !strcmp(varname,"F2") )
				forecasttype=2;
			break;			
		  case els:
			ELS = new elsClass(inFile,model);
			if( !ELS )
				fatalError("manipulate::loadParamset ","no els pointer");
			inFile >> varname;
			if( !strcmp(varname,"F2") )
				forecasttype=2;
			break;			
		  case sg:
			SG = new sgClass(inFile,model);
			if( !SG )
				fatalError("manipulate::loadParamset ","no sg pointer");
			inFile >> varname;
			if( !strncmp(varname,"F",1) ) {
				name=varname+1;
				forecasttype=atoi(name);
			}
			if( !strncmp(varname,"U",1) ) { // UnbiasedForecast "U2" -> forecast t+2 
				name=varname+1;
				forecasttype=100+atoi(name);
				inFile >> sigma;
				inFile >> epsilon;
				inFile >> dummy; // varname or start matrix
				ipointer=NULL;
				tstart=NULL;
				if( strncmp(dummy,"(",1) ) { // not equal, then it must be a varname
					ipointer = model->setLabels(dummy); // get pointer
					if( !ipointer )
						fatalError("manipulate::loadParamset can't find ",dummy);
					inFile >> dummy; // start matrix with '('
				}
				// now reads start matrix
				if( strncmp(dummy,"(",1) )
					fatalError("manipulate::loadParamset '(' expected, not ",dummy);
				strcpy(dummy2,"\000"); // clear 
				char* pos;
				if(strlen(dummy)>1) { // erase `(` from string
					pos=dummy+1;
					strcpy(dummy2,pos);
					pos=dummy2+strlen(dummy2)-1; // last char in dummy
				}
				else	pos=dummy2; // is empty
				if( !strcmp(pos,")") )  // last is ")" 
					strcpy(pos,"\000"); // clear last
				else while(inFile>>dummy) {
					if( strcmp(pos,",") )  // last is not "," 
						strcat(dummy2,","); // add a ","
					if( !strncmp(dummy,",",1) ) {// first is "," 
						pos=dummy+1;
						strcpy(dummy,pos);
					}
					strcat(dummy2,dummy); // add a ","
					pos=dummy2+strlen(dummy2)-1; // last char in dummy2
					if( !strcmp(pos,")") ) { // last is ")" 
						strcpy(pos,"\000"); // clear last
						break;
					}
				}
				pos=dummy2+strlen(dummy2)-1; // last char in dummy2
				if( strcmp(pos,",") )  // last is not "," 
					strcat(dummy2,","); // add a ","
				if( !strncmp(dummy2,",",1) ) { // first is "," 
					pos=dummy2+1;
					strcpy(dummy2,pos);
				}
				// convert data string into array
				pos=dummy2;
				int cnum=0;
				for (int i=0;i<(int)strlen(dummy2);i++)
					if(!strncmp(pos++,",",1)) cnum++;
				tstart= new qreal[cnum+1];
				if( !tstart )
					fatalError("manipulate::loadParamset case sg",
						"Can't allocate tstart");
				tstart[0]=cnum;
				char* posS;
				pos=dummy2; //start
				posS=dummy2; //stop
				for (int i=1;i<=cnum;i++) {
					while( strncmp(posS,",",1) )
						posS++;
					strcpy(posS,"\000"); // put in line end
					posS++;
					tstart[i]=atof(pos);
					pos=posS; // take next
				}
			}
			break;			
		  case statistics: // 200 inputvarname
			inFile >> statvarname;
			statvar = model->setLabels(statvarname);
			if( !statvar )
				fatalError("manipulate::loadParamset statistics can't find ",statvarname);
			break;			

		  case quantiles:  //210 inputvarname
			inFile >> quanvarname;
			quanvar = model->setLabels(quanvarname);
			if( !quanvar )
				fatalError("manipulate::loadParamset quantile can't find ",quanvarname);
			inFile >> u;
			break;

		  default:
			cerr << "manipulate::loadParamset  Do not know manipulate Tag ";
			cerr << manipulateTag[hm] << "\n";
			exit(-1);
		}
	}
	initialize();
}

///////////////////////////////////////////////////////////////////////////////
// Class name:		manipulate
// Member function:	
// Purpose:		initialize the model
//
// Author:		Marc Mueller  Jul 12 1999
// Last modified:	2000/6/9
///////////////////////////////////////////////////////////////////////////////
void manipulate::initialize() {
	int i,j,hM;
	qreal exp_0=0;			
	qreal exp2_0=0;

	for (hM=0;hM<howMany;hM++) {
		switch( manipulateTag[hM] ) {


		   case stoch_uni:
		   if( zvar != NULL ) {		// delete the old one
           delete zvar;			// if it exists
           }					// and get a new one
           zvar = new rand_var( this, "ranf", zvar_expr);
           break;	

		   case stoch_markov:
			if ( markov != NULL ) delete markov;
				markov = new markov_chain (this,"ranf",m_state,m_matrix);
			if( !(markov) )
			   fatalError("manipulate::initialize stoch_markov ",
					"can't create markov chain");
			for (int j=0;j<statesNum;j++) {// compute expectation
				exp_0+=umatrix[0][j]*states[j];
				exp2_0+=umatrix[0][j]*states[j]*states[j];
			}
			*expname=exp_0;
			*variancename=exp2_0-(exp_0*exp_0);
			*randname=states[0];
			break;
		   case stoch_ar:
			if ( zvar != NULL ) delete zvar;
				zvar = new rand_var( "ranf",1,zetamin,zetamax );
			if( !(zvar) )
				fatalError("manipulate::initialize stoch_ar ","can't create rand_var");
				
// Aenderung 09.12.03 Erwartungswert 	A.Starke
//
//			*randname=((zetamax-zetamin)/2)/(1-gamma);
			*randname=((zetamax+zetamin)/2)/(1-gamma);
			*variancename=((zetamax-zetamin)*(zetamax-zetamin))/12; // ist konstant !
// Aenderung 09.12.03 bedingter Erwartungswert	A.Starke
//		
//			*expname= gamma * *randname + ((zetamax-zetamin)/2) ;
			*expname= gamma * *randname + ((zetamax+zetamin)/2);
			*randname=gamma * *randname + zvar->dice();
			break;
		}
	}
		
	model->initialize();

	for (hM=0;hM<howMany;hM++) {
		switch( manipulateTag[hM] ) {
		   case errcor:
			for( j=0;j<errcor_num;j++) {
				errcor_ptr[j].forecastOld = *errcor_ptr[j].forecast;
				errcor_ptr[j].forecastOld2 = errcor_ptr[j].forecastOld;
			}
			break;
		   case ma:
			for( j=0;j<ma_num;j++)
				ma_ptr[j].vn = *ma_ptr[j].vname;
			break;
		   case ma_k:
			for( j=0;j<ma_k_num;j++)
				for ( i = 0; i < ma_k_ptr[j].k ; i++ )
					ma_k_ptr[j].vn_k[i] = *ma_k_ptr[j].vname;
			break;									
		   case ma_k_a:
			for( j=0;j<ma_k_a_num;j++)
			   if( ma_k_a_ptr[j].vn_k ) {
				delete [] ma_k_a_ptr[j].vn_k;
				ma_k_a_ptr[j].vn_k=new qreal[ma_k_a_ptr[j].k];
				if( !ma_k_a_ptr[j].vn_k )
					fatalError("manipulate::initialize ma_k_a ",
						"Can't allocate ma_k_a_ptr[].vn_k");
				for ( i = 0; i < ma_k_a_ptr[j].k ; i++ )
					ma_k_a_ptr[j].vn_k[i] = *ma_k_a_ptr[j].vname;
			}
			//log() << ma_k_a_ptr[0].vn_k[0] << " a=" << ma_k_a_ptr[0].a << "\n";
			break;									
		   case rls0:
			beta_tm1 = 0 ;
			g_tm1 = g_0;
			p_tm2 = *p_tm1 ;
			break;									
		   case rls1:
			break;									
		   case ols:
		   	OLS->initialize();
			break;
		   case rls:
		   	RLS->initialize();
			break;
		   case rls1d:
			theta=0;
			oldp=*p;
			break;
		   case els:
		   	ELS->initialize();
			break;
		   case sg:
		   	SG->initialize();
			if((forecasttype>100)&&(forecasttype<200)) {
		  		SG->initializeUnbiasedForecast(sigma,epsilon,ipointer,tstart);
				firstperiod=0;
			}
			break;
		   case statistics:
			mean=0;
			variance=0;
			meansum=0;
			sharpeRatio=0;
			variationskoeff=0;
			break;
		   case quantiles:
			xi=0;
			sk=0;
			ku=0;
			x=0;
			zx=0;
			sd=0;
			for (int k=0;k<4;k++) {
				mr[k] = 0;
				mc[k] = 0;
			}
			break;
		}
	}
}
///////////////////////////////////////////////////////////////////////////////
// Class name:		manipulate
// Member function:	F_stoch_uni
// Purpose:			put stochastic random process into a modell
//
// Author:			Michael Meyer 2000/5/25
// Last modified:	2000/5/25
///////////////////////////////////////////////////////////////////////////////
void manipulate::F_stoch_uni() {

    * randname = zvar->dice();	// Set a new random value for the Parameter
}



///////////////////////////////////////////////////////////////////////////////
// Class name:		manipulate
// Member function:	F_stoch_markov
// Purpose:		put markov random process into a modell
//
// Author:		Marc Mueller 2000/5/25
// Last modified:	2000/5/25
///////////////////////////////////////////////////////////////////////////////
void manipulate::F_stoch_markov() {
	int j,jj=0;
	for (int j=0;j<statesNum;j++) // find state
		if(states[j]==*randname) jj=j;
	qreal exp=0;			
	qreal exp2=0;			
	for (int j=0;j<statesNum;j++) {// compute expectation
		exp+=umatrix[jj][j]*states[j];
		exp2+=umatrix[jj][j]*states[j]*states[j];
	}
	*expname=exp;
	*variancename=exp2-(exp*exp);
	*randname=markov->dice();
//log() << "F_stoch_markov() " << *randname << "\n";
}

///////////////////////////////////////////////////////////////////////////////
// Class name:		manipulate
// Member function:	F_stoch_ar
// Purpose:		put autoregressiv random process into a modell
//
// Author:		Marc Mueller 2000/5/25
// Last modified:	2000/5/25
///////////////////////////////////////////////////////////////////////////////
void manipulate::F_stoch_ar() {
// Aenderung 09.12.03 bedingter Erwartungswert	A.Starke
//	*expname= gamma * *randname + ((zetamax-zetamin)/2) ;
	*expname= gamma * *randname + ((zetamax+zetamin)/2);
	*randname=gamma * *randname + zvar->dice();
}

///////////////////////////////////////////////////////////////////////////////
// Class name:		manipulate
// Member function:	errcor
// Purpose:		forcast a new price with weighted last error
// 			Period t-1 is just done: 
//			   pF(t)=p(t-1)+alpha*(pF(t-2)-p(t-1))
//
// Author:		Marc Mueller 2000/6/20
// Last modified:	2000/6/20
///////////////////////////////////////////////////////////////////////////////
void manipulate::F_errcor() {
	for(int j=0;j<errcor_num;j++) { 
		*errcor_ptr[j].forecast=*errcor_ptr[j].price +
			errcor_ptr[j].alpha*
			(errcor_ptr[j].forecastOld2-*errcor_ptr[j].price);
		errcor_ptr[j].forecastOld2=errcor_ptr[j].forecastOld;
		errcor_ptr[j].forecastOld=*errcor_ptr[j].forecast;
	}
}

///////////////////////////////////////////////////////////////////////////////
// Class name:		manipulate
// Member function:	F_ma
// Purpose:		calculate total average
//
// Author:		Marc Mueller Jul 13 1999
// Last modified:	Tue Jul 13 15:01:38 CEST 1999
///////////////////////////////////////////////////////////////////////////////
void manipulate::F_ma(const long& t) {
	if(t>0)	for(int j=0;j<ma_num;j++) {
			ma_ptr[j].vn = (ma_ptr[j].vn*t + *ma_ptr[j].vname)/(t+1);
			*ma_ptr[j].mname = ma_ptr[j].vn;
		}
	 else	for(int j=0;j<ma_num;j++)
			*ma_ptr[j].mname = ma_ptr[j].vn;
}

///////////////////////////////////////////////////////////////////////////////
// Class name:		manipulate
// Member function:	ma_k
// Purpose:		calculate moving average with length k
//
// Author:		Marc Mueller Jul 13 1999
// Last modified:	Tue Jul 13 15:01:38 CEST 1999
///////////////////////////////////////////////////////////////////////////////
void manipulate::F_ma_k() {
	qreal dummy;
	for(int j=0;j<ma_k_num;j++) { // for all named variables
		for ( int i = (ma_k_ptr[j].k -1); i>0; i-- )
			ma_k_ptr[j].vn_k[i] = ma_k_ptr[j].vn_k[i-1];
		ma_k_ptr[j].vn_k[0] = *ma_k_ptr[j].vname;

		dummy=0;
		for(int k=0;k<ma_k_ptr[j].k;k++) {
			dummy+= ma_k_ptr[j].vn_k[k];
			}
		*ma_k_ptr[j].mname = dummy/ma_k_ptr[j].k;
	}
}

///////////////////////////////////////////////////////////////////////////////
// Class name:		manipulate
// Member function:	ma_k_a
// Purpose:		calculate moving average with length k 
//                      and multiply the result with parameter a
// Author:		Marc Mueller
// Last modified:	2000/2/12
///////////////////////////////////////////////////////////////////////////////
void manipulate::F_ma_k_a() {
	qreal dummy;
	for(int j=0;j<ma_k_a_num;j++) { // for all named variables
		for ( int i = ((ma_k_a_ptr[j].k) -1); i>0; i-- )
			ma_k_a_ptr[j].vn_k[i] = ma_k_a_ptr[j].vn_k[i-1];
		ma_k_a_ptr[j].vn_k[0] = *ma_k_a_ptr[j].vname;

		dummy=0;
		for(int k=0;k<ma_k_a_ptr[j].k;k++) {
			dummy+= ma_k_a_ptr[j].vn_k[k];
			}
		*ma_k_a_ptr[j].mname = ma_k_a_ptr[j].a*(dummy/ma_k_a_ptr[j].k);
	}
}

///////////////////////////////////////////////////////////////////////////////
// Class name:		manipulate
// Member function:	rls0
// Purpose:		like rls1 but for a price, not for inflation factor
//
// Author:		Marc Mueller Dec 08 1999
// Last modified:	Dec 08 1999
///////////////////////////////////////////////////////////////////////////////
void manipulate::F_rls0() {
    qreal theta_tm1,theta2;
//log() << " *p_tm1=" << *p_tm1 << " p_tm2=" << p_tm2;
//log() << " g_tm1=" << g_tm1 << " *p_e_tp1=" << *p_e_tp1 << "\n";

    theta_tm1= *p_tm1 / p_tm2; // calculate inflation factor
    beta_tm1 += g_tm1 * ( theta_tm1 - beta_tm1 ) ;  // recursiv estimate
    theta2 = theta_tm1*theta_tm1 ;
    g_tm1 *=  theta2 / ( g_tm1*theta2 + 1.0) ;
    p_tm2 = *p_tm1;  // save for next period

  // forecast
   *p_e_tp1 = beta_tm1*beta_tm1 * *p_tm1;
}

///////////////////////////////////////////////////////////////////////////////
// Class name:		manipulate
// Member function:	rls1
// Purpose:		calculate rls sequential over inflation factors
//			see PhD thesis from Martin
// Author:		Marc Mueller Jul 18 1999
// Last modified:	Fri Okt 29 1999
///////////////////////////////////////////////////////////////////////////////
void manipulate::F_rls1() {
    *theta_e_tp1 = *theta_e_t + g_tm1 * ( *theta_tm1 - *theta_e_t ) ;
    qreal dummy = g_tm1 * *theta_tm1 * *theta_tm1 ;
    g_tm1 = dummy / (dummy + 1.0) ;
//log() <<"99rls1: g_tm1="<<g_tm1<<" *theta_tm1="<<*theta_tm1<<" *theta_e_t="<<*theta_e_t<<" *theta_e_tp1="<<*theta_e_tp1<<"\n";
}

///////////////////////////////////////////////////////////////////////////////
// Class name:		manipulate
// Member function:	rls1d
// Purpose:		calculate rls sequential (with one dimension only!)
//			like rls0 but formula transformed, see Jan 
// Author:		Marc Mueller Dec 24 1999
// Last modified:	3.Jan.2000 Marc Mueller
///////////////////////////////////////////////////////////////////////////////
void manipulate::F_rls1d() {
//log() <<"121rls1d: oldp="<<oldp<<" pe="<<theta*oldp;
//log() <<" fe="<<( *p - theta*oldp ) <<"\n";

    theta += ((oldp*P)/(1+oldp*oldp*P)) * ( *p - theta*oldp ) ;

//log() <<"121rls1d: p="<<*p<<" lPp="<<((oldp*P)/(1+oldp*oldp*P))<<" theta="<<theta<<"\n";

    P /= ( 1+oldp*oldp*P );

    if(forecasttype==2)
	*q=theta*theta * *p; //forecast2
     else
	*q=theta * *p; //forecast
//log() <<"121rls1d: P="<<P<<" theta="<<theta<<" oldp="<<oldp<<" p="<<*p<<" pe="<<*q<<"\n";
    oldp=*p;
}

///////////////////////////////////////////////////////////////////////////////
// Class name:		manipulate
// Member function:	ols,rls,sg
// Purpose:		calculate ls
//
// Author:		Marc Mueller Jul 18 1999
// Last modified:	25.01.2000 Marc Mueller
///////////////////////////////////////////////////////////////////////////////
void manipulate::F_ols() {
//	int i,j,ij;

	if(olsStart>0) { // do ols and forecast
	OLS->estimate();
	OLS->forecast();
	}
	else { // initialize must be done twice, then R is invertable

		olsStart++; // build up P before a forecast is possible
	}
}
void manipulate::F_rls() {
	RLS->estimate();
	if(forecasttype==2)
		RLS->forecast2();
	 else
		RLS->forecast();
}
void manipulate::F_els() {
	ELS->estimate();
	if(forecasttype==1) ELS->forecast();
	 else if(forecasttype==2) ELS->forecast2();
	 else {
		cerr << "manipulate::F_els do not know forecasttype "<<forecasttype<<"\n";
		exit(-1);
	 }
}
void manipulate::F_sg() {
	if(forecasttype==1) {
		SG->estimate();
		SG->forecast();
	}
	else if(forecasttype==2) {
		SG->estimate();
		SG->forecast();
	}
	else if((forecasttype>100)&&(forecasttype<200)) {
//		if(!firstperiod)
//			firstperiod++;
//		else	{
			SG->estimate();
			SG->unbiasedForecast((forecasttype-100));
//		}
	}
	else {
		cerr << "manipulate::F_sg do not know forecasttype "<<forecasttype<<"\n";
		exit(-1);
	}
}


//////////////////////////////////////////////////////////////////////////////////
// Class name:		manipulate													//
// Member function:	Sequper														//
// Purpose:		    Given the qrealization generated at time t, i.e. xt, and the //
//					moments mr as well as phi computed at instance t-1 the 		//
//					function updates the moments mr, it calculates the central	//
//					moments mc, skewness and kurtosis as well as an estimate	//
//					of the quantile x_i specified by the user 					//
//																				//
// Author:			Mathias Hoffmann & Michael Meyer							//
// Last modified:	Thu Sep 21 11:51:02 METDST 2000								//
///////////////////////////////////////////////////////////////////////////////	//
void manipulate::Sequper(const long& t) {
	const qreal lnC = -706.893623549172;
	const int tbi = 5;

	if (t < tbi) {
		for (int j=0;j<4;j++) {
			mr[j] = mr[j] + (pow(*quanvar,j+1) - mr[j])/t;
		}
	}
	else {
		for (int k=0;k<4;k++) {
			mr[k] = mr[k] + (pow(*quanvar,k+1) - mr[k])/t;	
		}
		mc[0]=	mr[0];
		mc[1]=	mr[1] - mr[0]*mr[0];
		sd=sqrt(mc[1]);		
		if (mc[1] < 0)
			fatalError("manipulate::Sequper ","Negative Variance in Sequper");
		if ((log(mc[1])) <= (0.5*lnC))
			fatalError("manipulate::Sequper ","Higher moments: numerical problems due to small variance");
		mc[2] = mr[2] - 3*mr[0]*mr[1] + 2*mr[0]*mr[0]*mr[0];
		mc[3] = mr[3] - 4*mr[0]*mr[2] + 6*mr[0]*mr[0]*mr[1] - 3*mr[0]*mr[0]*mr[0]*mr[0];

		sk = mc[2]/sqrt(mc[1]*mc[1]*mc[1]);
		ku = mc[3]/(mc[1]*mc[1]) - 3;

		if (u != -1) { 
			if (t == tbi) Bisec();
			FXRoot();		
			xi = mc[0] + sqrt(mc[1]) * x;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////
// Class name:		manipulate														//
// Member function:	Bisec															//
// Purpose:		    Given a continuous function f:R->R defined over the intervall	//
//					[a,b] such that f(a)*f(b)<0, the function determines a scalar	//
//					x* in [a,b] such that f(x*)=0									//
//																					//
// Author:			Mathias Hoffmann & Michael Meyer								//
// Last modified:	Thu Sep 21 11:51:02 METDST 2000									//
//////////////////////////////////////////////////////////////////////////////////////
void manipulate::Bisec() {
	const int 	a = -4,
				b = 4;
	const qreal e = exp(-5*log(double(10)));
	double tmp_FXa, tmp_FXm;
	qreal j[2], m;

	x = m = 0.5*(a + b);
	j[0] = a; j[1] = b;
	do {
		tmp_FXm = FX();
		if (tmp_FXm == 0) 
			break;
		x = j[0];
		tmp_FXa = FX();
		if (tmp_FXa * tmp_FXm < 0) 
			j[1] = m;
		else
			j[0] = m;
		x = m = 0.5*(j[0] + j[1]);
	} while ((fabs(j[1]-j[0])) / (fabs(m)<1 ? 1 : fabs(m)) >= e);
}

//////////////////////////////////////////////////////////////////////////////////
// Class name:		manipulate													//
// Member function:	FXRoot														//
// Purpose:		    Given a starting value x_0 and the current estimates of 	//
//					skewness,kurtosis and u FXRoot calculates the root of the	//
//					function FX according to the one-point iteration formula	//
//																				//
// Author:			Mathias Hoffmann & Michael Meyer							//
// Last modified:	Thu Sep 21 11:51:02 METDST 2000								//
//////////////////////////////////////////////////////////////////////////////////
void manipulate::FXRoot() {
	double 	x_old = 0, x_save = 0,
			fx0 = 0,
			fx1 = 0,
			fx2 = 0;
	const double eps1 = exp(-103*log(4.64));
	const qreal eps2 = exp(-10*log(double(10)));

	x_old = x_save = x;
    int i;
    for (i=1; i<=100; i++) {
		fx0 = FX();
		fx1 = FX1();
		fx2 = FX2();
		if (eps1 > fabs(fx1)) {
			log() << "FXROOT::First derivative at current root estitmate extremely small" << "\n";
			x = x_save ;
			break;
		}
		x_old = x;
		x = x_old - (fx0/fx1) + 0.5*fx2*((fx0*fx0) / (fx1*fx1*fx1));
		if (eps2 >= fabs(x - x_old)) break;  		
	}
    if (i==101) {
		log() << "FXROOT::No convergence under current conditions (100, 10^(-10))" << "\n";
		x = x_save;
	}
}

//////////////////////////////////////////////////////////////////////////////////
// Class name:		manipulate													//
// Member function:	FX															//
// Purpose:		    Given skewness, kurtosis and u the function computes FX-u	//
//					at x. The root of this Edgeworth type CDF is accepted as 	//
// 					the quantile estimate										//
//																				//
// Author:			Mathias Hoffmann & Michael Meyer							//
// Last modified:	Thu Sep 21 11:51:02 METDST 2000								//
//////////////////////////////////////////////////////////////////////////////////
double manipulate::FX() {
	double fx, h1, c0, c1, c2, c3, c5, hx;

	ZX();
	if ((u<=0) || (u>=1)) 
		fatalError("manipulate::FX:: ","Argument u of FX out of range (0,1)");	
	if (x > 10) return fx = 1-u; 
	if (x < -10) return fx = -u; 
	h1 = sk*sk;
	c5 = 0.01388888888888889 * h1;
	c3 = 0.04166666666666667 * ku - 0.1388888888888889 * h1;
	c2 = 0.16666666666666667 * sk;
	c1 = -0.125*ku + 0.2083333333333333*h1;
	c0 = -c2;
	hx = c0 + x * (c1+x*(c2+x*(c3+c5*x*x)));
	return fx = PHIX() - hx * zx - u;
}

//////////////////////////////////////////////////////////////////////////////////
// Class name:		manipulate													//
// Member function:	FX1															//
// Purpose:		    Given skewness, kurtosis the function computes the first 	//
//					derivative of FX at x										//
//																				//
// Author:			Mathias Hoffmann & Michael Meyer							//
// Last modified:	Thu Sep 21 11:51:02 METDST 2000								//
//////////////////////////////////////////////////////////////////////////////////
double manipulate::FX1() {
	double fx1, h1, c0, c1, c2, c3, c4, c6, px;

	if (fabs(x)>= exp(50*log(double(10)))) 
		fatalError("manipulate::FX1 ","Argument x of FX1 out of range");
	h1 = sk*sk;
	c6 = 0.01388888888888889 * h1;
	c4 = 0.04166666666666667 * ku - 0.2083333333333333 * h1;
	c3 = 0.16666666666666667 * sk;
	c2 = -0.25 * ku + 0.625 * h1;
	c1 = -0.5 * sk;
	c0 = 1 + 0.125 * ku - 0.2083333333333333 * h1;
	px = c0 + x * (c1+x*(c2+x*(c3+x*(c4+c6*x*x))));
	return fx1 = px * zx;
}

//////////////////////////////////////////////////////////////////////////////////
// Class name:		manipulate													//
// Member function:	FX2															//
// Purpose:		    Given skewness, kurtosis the function computes the second 	//
//					derivative of FX at x										//
//																				//
// Author:			Mathias Hoffmann & Michael Meyer							//
// Last modified:	Thu Sep 21 11:51:02 METDST 2000								//		
//////////////////////////////////////////////////////////////////////////////////
double manipulate::FX2() {
	double fx2, h1, c0, c1, c2, c3, c4, c5, c7, px;

	if (fabs(x)>= exp(44*log(double(10)))) 
		fatalError("manipulate::FX2 ","Argument x of FX2 out of range");
	h1 = sk*sk;
	c7 = -0.01388888888888889 * h1;
	c5 = -0.04166666666666667 * ku + 0.29166666666666666667 * h1;
	c4 = -0.16666666666666667 * sk;
	c3 = 0.4166666666666667 * ku - 1.45833333333333333333 * h1;
	c2 = sk;
	c1 = (-0.625 * ku + 1.45833333333333333 * sk * sk) - 1;
	c0 = -0.5 * sk;
	px = c0 + x * (c1+x*(c2+x*(c3+x*(c4+x*(c5+c7*x*x)))));
	return fx2 = px * zx;	
}

//////////////////////////////////////////////////////////////////////////////////
// Class name:		manipulate													//
// Member function:	PHIX														//
// Purpose:		    PHIX approximates the CDF of a standard normal variate at x	//
//																				//
// Author:			Mathias Hoffmann & Michael Meyer							//
// Last modified:	Thu Sep 21 11:51:02 METDST 2000								//
//////////////////////////////////////////////////////////////////////////////////
double manipulate::PHIX() {
	double phi = 0;
	const qreal 	c1 = 0.4361836,
				c2 = -0.1201676,
				c3 = 0.987298;
	qreal scalar;

	if (x < -37) return phi;  
	if (x == 0) return phi = 0.5;
	if (x > 10) return phi = 1;
	if (x > 0) {
		scalar = 1/(1+0.33267*x);
		return phi = 1 - scalar*(c1+scalar*(c2+c3*scalar))*zx;
	}
	else {
		scalar = 1/(1+0.33267*x*(-1));
		return phi = 1 - (1 - scalar*(c1+scalar*(c2+c3*scalar))*zx);
	}
}
//////////////////////////////////////////////////////////////////////////////
// Class name:		manipulate												//
// Member function:	ZX														//
// Purpose:		    ZX computes the value of the n(0,1) density at x		//
//																			//
// Author:			Mathias Hoffmann & Michael Meyer						//
// Last modified:	Thu Sep 21 11:51:02 METDST 2000							//
//////////////////////////////////////////////////////////////////////////////
void manipulate::ZX() {
	const double c = 0.3989422804014327;

	if (fabs(x) >= 37) 
		zx = 0;
	else 
		zx = c*exp(-0.5*x*x);  	
}

//////////////////////////////////////////////////////////////////////////////
// Class name:		manipulate												//
// Member function:	iteration												//
// Purpose:			perform one iteration of the system						//
//																			//
// Author:			Marc Mueller  Jul 12 1999								//
// Last modified:	Thu Sep 21 11:50:25 METDST 2000 (M. Hoffmann & M. Meyer)//
//////////////////////////////////////////////////////////////////////////////
void manipulate::iteration(const long& t) {
	int i,j;
	for (int i=0;i<howMany;i++) {
		switch( manipulateTag[i] ) {
		   case errcor: F_errcor(); break;
		   case ma: F_ma(t); break;
		   case ma_k: F_ma_k(); break;		   
		   case ma_k_a: F_ma_k_a(); break;
		   case stoch_uni: F_stoch_uni(); break;	   
		}
	}
	model->iteration(t);
	for (int i=0;i<howMany;i++) {
		switch( manipulateTag[i] ) {
//Aenderung A.Starke 12.07.2004: Warum sollte die Stoerung erst nach der
//ersten Iteration einsetzen? nicht logisch und nicht kohaerent mit den
//Random-Modellen, siehe z.B. rdefault!!! Deshalb wurde stoch_uni in die
//erste Schleife aufgenommen.		
//		   case stoch_uni: F_stoch_uni(); break;
		   case stoch_markov: F_stoch_markov(); break;
		   case stoch_ar: F_stoch_ar(); break;
		   case rls0: F_rls0(); break;		   
		   case rls1: F_rls1(); break;		
		   case ols: F_ols(); break;		   
		   case rls: F_rls(); break;		   
		   case rls1d: F_rls1d(); break;		   
		   case els: F_els(); break;		   
		   case sg: F_sg(); break;		   
		   case statistics:
			meansum+=*statvar;
//			mean = meansum/t;
			mean=((t-1)*mean+ *statvar)/t;
			variance=((t-1)*variance+(*statvar-mean)*(*statvar-mean))/t;
			standardDeviation=sqrt(variance);
			if(t>=10){
				variationskoeff=standardDeviation/mean;
			    sharpeRatio = (mean + 0.01)/standardDeviation;
			}
			if(t==length)
				log() <<"mean="<<mean<<" variance="<<variance
					 <<"standardDeviation="<<standardDeviation
					 <<"Variationskoeffizient="<<variationskoeff<<"\n"
					 <<"sharpeRatio="<<sharpeRatio<<"\n";
		break;		
		   case quantiles: 
			Sequper(t); 
			if(t==length)
			{
				ofstream stat_output("statistic_output.tex");
				stat_output << "\\begin{tabular}{|l||l|}\\hline\n";
				stat_output << "statistic&estimate\\\\ \\hline\\hline\n"; 
				stat_output << "mean&" << mr[0] << "\\\\ \\hline\n"; 
				stat_output << "variance&" << mc[1] << "\\\\ \\hline\n"; 
				stat_output << "standard deviation&" << sd << "\\\\ \\hline\n"; 
				stat_output << "skewness&" << sk << "\\\\ \\hline\n"; 
				stat_output << "kurtosis&" << ku << "\\\\ \\hline\n"; 
				stat_output << "quantile (" << u << ")&" << xi << "\\\\ \\hline\n"; 
				stat_output << "\\end{tabular}";
				stat_output << "\n";
				stat_output << "\\begin{tabular}{|l|l|}\\hline\n";
				for (j = 1; j<=4; j++)
					stat_output <<  j << ". moment&" << mr[j-1] <<"\\\\ \\hline\n";
				for (j = 1; j<=4; j++)
					stat_output <<  j << ". centered moment&" << mc[j-1] <<"\\\\ \\hline\n";
				stat_output << "\\end{tabular}";
			}
			break;		
		}
	}
}

