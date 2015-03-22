

//#include "../error.h"
//#include "../strnchr.h"
#include "tiwisina.h"



tiwisina::tiwisina() : baseModel(1)
{
    bin();
}

//void tiwisina::fact(int n){
//  int i;
//  mpz_t p;

//  mpz_init_set_ui(p,1); /* p = 1 */
//  for (i=1; i <= n ; ++i){
//    mpz_mul_ui(p,p,i); /* p = p * i */
//  }
////  printf ("%d!  =  ", n);
////  mpz_out_str(stdout,10,p);
//  mpz_init_set(factTemp,p);
//  mpz_clear(p);

//}

//void tiwisina::bin(){
//    mpz_t temp1;
//    fact(100);
//    mpz_init_set(temp1,factTemp);
//    //mpz_clear(factTemp);

//    for(int i=0; i<101; i--){
//        mpz_t result;
//        fact(i);
//        mpz_div(result,temp1,factTemp);
//        //mpz_clear(factTemp);
//        fact(100-i);
//        mpz_div(result,result,factTemp);
//        //mpz_clear(factTemp);
//        //mpz_init_set(binomial[i],result);
//        //mpz_clear(result);
//    }
//    //mpz_clear(temp1);
//}

mpf_class tiwisina::fact(int n){
  int i;
  mpz_class p = 1;

  for (i=1; i <= n ; ++i){
    p = p*i;
  }
    return p;
}

void tiwisina::bin(){
    mpf_class temp1 = fact(100);

    for(int i=0; i<101; i++){
        binomial[i] = temp1/fact(i)/fact(100-i);
    }
}


void tiwisina::iteration(const qint64&)
{
//    qreal x=xt;
//    qreal y=yt;

//    xt = a1*(1-pow(x,2));
//    x_prime= a1*2*x;

//    mpz_t x1,y1,result;

//    mpz_init_set_str(x1, "12", 10);
//    mpz_init_set_str(y1, "11", 10);
//    mpz_init(result);

//    mpz_mul(result, x1, y1);
////    gmp_printf("    %Zd\n"
////               "*\n"
////               "    %Zd\n"
////               "--------------------\n"
////               "%Zd\n", x1, y1, result);

//    xt = mpz_get_d(result)/(double)1000.0;
//    //log()<<xt;
//    /* free used memory */
//    mpz_clear(x1);
//    mpz_clear(y1);
//    mpz_clear(result);

//    mpz_t temp1,temp2,temp3,x1,summe;
//    mpz_init(temp1);
//    mpz_init(temp2);
//    mpz_init(temp3);

//    mpz_init_set_d(x1,x);
//    mpz_pow_ui(temp1,x1,71);

//    for(int i=100; i>=60; i--){
//        mpz_init_set_str(temp1,binomial[i],10);
//        mpz_pow_ui(temp2,x1,0);
//        mpz_mul(temp1,temp1,temp2);
//        mpz_add(temp3,temp3,temp1);
//    }



    mpf_class x = 0;
    mpf_class xtCopy = xt;
    //printf("vorher xtCopy = %s \n", xtCopy.get_str(1).c_str());
    mpf_class temp1,temp2,temp3 = 0;
    for(int i=0; i<=60; i++){
        mpf_pow_ui(temp1.get_mpf_t(),xtCopy.get_mpf_t(),i);
        temp2=1.0-xtCopy;
        mpf_pow_ui(temp2.get_mpf_t(),temp2.get_mpf_t(),(100-i));
        temp3+=temp1*temp2*binomial[i];
    }
    x+=temp3*a1;
    temp3=0;
    for(int i=0; i<=70; i++){
        mpf_pow_ui(temp1.get_mpf_t(),xtCopy.get_mpf_t(),i);
        temp2=1.0-xtCopy;
        mpf_pow_ui(temp2.get_mpf_t(),temp2.get_mpf_t(),(100-i));
        temp3+=temp1*temp2*binomial[i];
    }
    x+=(1-temp3)*a2 + a3;
    xt=x.get_d();


    mpf_class xprime = 0;

    for(int i=0; i<=60; i++){
        if(i-1<0)
            temp1=0;
        else{
            mpf_pow_ui(temp1.get_mpf_t(),xtCopy.get_mpf_t(),i-1);
            temp1*=i;
        }
        temp2=1.0-xtCopy;
        mpf_pow_ui(temp2.get_mpf_t(),temp2.get_mpf_t(),(100-i));
        temp3+=temp1*temp2*binomial[i];

        mpf_pow_ui(temp1.get_mpf_t(),xtCopy.get_mpf_t(),i);
        if(99-i<0)
            temp2=0;
        else{
            temp2=1.0-xtCopy;
            mpf_pow_ui(temp2.get_mpf_t(),temp2.get_mpf_t(),(99-i));
            temp2*=-(100-i);
        }
        temp3+=temp1*temp2*binomial[i];
        temp2=1.0-xtCopy;
    }
    xprime+=temp3*a1;
    temp3=0;
    for(int i=0; i<=70; i++){
        if(i-1<0)
            temp1=0;
        else{
            mpf_pow_ui(temp1.get_mpf_t(),xtCopy.get_mpf_t(),i-1);
            temp1*=i;
        }
        temp2=1.0-xtCopy;
        mpf_pow_ui(temp2.get_mpf_t(),temp2.get_mpf_t(),(100-i));
        temp3+=temp1*temp2*binomial[i];

        mpf_pow_ui(temp1.get_mpf_t(),xtCopy.get_mpf_t(),i);
        if(99-i<0)
            temp2=0;
        else{
            temp2=1.0-xtCopy;
            mpf_pow_ui(temp2.get_mpf_t(),temp2.get_mpf_t(),(99-i));
            temp2*=-(100-i);
        }
        temp3+=temp1*temp2*binomial[i];
        temp2=1.0-xtCopy;
    }
    xprime+=-(1-temp3)*a2 + a3;
    x_prime=xprime.get_d();


//    xt = a3 + a1*(Power(1 - x,100) + 100*Power(1 - x,99)*x + 4950*Power(1 - x,98)*Power(x,2) + 161700*Power(1 - x,97)*Power(x,3) + 3921225*Power(1 - x,96)*Power(x,4) + 75287520*Power(1 - x,95)*Power(x,5) +
//                  1192052400*Power(1 - x,94)*Power(x,6) + 16007560800*Power(1 - x,93)*Power(x,7) + 186087894300*Power(1 - x,92)*Power(x,8) + 1902231808400*Power(1 - x,91)*Power(x,9) +
//                  17310309456440*Power(1 - x,90)*Power(x,10) + 141629804643600*Power(1 - x,89)*Power(x,11) + 1050421051106700*Power(1 - x,88)*Power(x,12) + 7110542499799200*Power(1 - x,87)*Power(x,13) +
//                  44186942677323600*Power(1 - x,86)*Power(x,14) + 253338471349988640*Power(1 - x,85)*Power(x,15) + 1345860629046814650*Power(1 - x,84)*Power(x,16) +
//                  6650134872937201800*Power(1 - x,83)*Power(x,17) + 30664510802988208300*Power(1 - x,82)*Power(x,18) + 132341572939212267400*Power(1 - x,81)*Power(x,19) +
//                  535983370403809682970*Power(1 - x,80)*Power(x,20) + 2041841411062132125600*Power(1 - x,79)*Power(x,21) + 7332066885177656269200*Power(1 - x,78)*Power(x,22) +
//                  24865270306254660391200*Power(1 - x,77)*Power(x,23) + 79776075565900368755100*Power(1 - x,76)*Power(x,24) + 242519269720337121015504*Power(1 - x,75)*Power(x,25) +
//                  699574816500972464467800*Power(1 - x,74)*Power(x,26) + 1917353200780443050763600*Power(1 - x,73)*Power(x,27) + 4998813702034726525205100*Power(1 - x,72)*Power(x,28) +
//                  12410847811948286545336800*Power(1 - x,71)*Power(x,29) + 29372339821610944823963760*Power(1 - x,70)*Power(x,30) + 66324638306863423796047200*Power(1 - x,69)*Power(x,31) +
//                  143012501349174257560226775*Power(1 - x,68)*Power(x,32) + 294692427022540894366527900*Power(1 - x,67)*Power(x,33) + 580717429720889409486981450*Power(1 - x,66)*Power(x,34) +
//                  1095067153187962886461165020*Power(1 - x,65)*Power(x,35) + 1977204582144932989443770175*Power(1 - x,64)*Power(x,36) + 3420029547493938143902737600*Power(1 - x,63)*Power(x,37) +
//                  5670048986634686922786117600*Power(1 - x,62)*Power(x,38) + 9013924030034630492634340800*Power(1 - x,61)*Power(x,39) + 13746234145802811501267369720*Power(1 - x,60)*Power(x,40) +
//                  20116440213369968050635175200*Power(1 - x,59)*Power(x,41) + 28258808871162574166368460400*Power(1 - x,58)*Power(x,42) + 38116532895986727945334202400*Power(1 - x,57)*Power(x,43) +
//                  49378235797073715747364762200*Power(1 - x,56)*Power(x,44) + 61448471214136179596720592960*Power(1 - x,55)*Power(x,45) + 73470998190814997343905056800*Power(1 - x,54)*Power(x,46) +
//                  84413487283064039501507937600*Power(1 - x,53)*Power(x,47) + 93206558875049876949581681100*Power(1 - x,52)*Power(x,48) + 98913082887808032681188722800*Power(1 - x,51)*Power(x,49) +
//                  100891344545564193334812497256*Power(1 - x,50)*Power(x,50) + 98913082887808032681188722800*Power(1 - x,49)*Power(x,51) + 93206558875049876949581681100*Power(1 - x,48)*Power(x,52) +
//                  84413487283064039501507937600*Power(1 - x,47)*Power(x,53) + 73470998190814997343905056800*Power(1 - x,46)*Power(x,54) + 61448471214136179596720592960*Power(1 - x,45)*Power(x,55) +
//                  49378235797073715747364762200*Power(1 - x,44)*Power(x,56) + 38116532895986727945334202400*Power(1 - x,43)*Power(x,57) + 28258808871162574166368460400*Power(1 - x,42)*Power(x,58) +
//                  20116440213369968050635175200*Power(1 - x,41)*Power(x,59) + 13746234145802811501267369720*Power(1 - x,40)*Power(x,60)) +
//               a2*(1 - Power(1 - x,100) - 100*Power(1 - x,99)*x - 4950*Power(1 - x,98)*Power(x,2) - 161700*Power(1 - x,97)*Power(x,3) - 3921225*Power(1 - x,96)*Power(x,4) - 75287520*Power(1 - x,95)*Power(x,5) -
//                  1192052400*Power(1 - x,94)*Power(x,6) - 16007560800*Power(1 - x,93)*Power(x,7) - 186087894300*Power(1 - x,92)*Power(x,8) - 1902231808400*Power(1 - x,91)*Power(x,9) -
//                  17310309456440*Power(1 - x,90)*Power(x,10) - 141629804643600*Power(1 - x,89)*Power(x,11) - 1050421051106700*Power(1 - x,88)*Power(x,12) - 7110542499799200*Power(1 - x,87)*Power(x,13) -
//                  44186942677323600*Power(1 - x,86)*Power(x,14) - 253338471349988640*Power(1 - x,85)*Power(x,15) - 1345860629046814650*Power(1 - x,84)*Power(x,16) -
//                  6650134872937201800*Power(1 - x,83)*Power(x,17) - 30664510802988208300*Power(1 - x,82)*Power(x,18) - 132341572939212267400*Power(1 - x,81)*Power(x,19) -
//                  535983370403809682970*Power(1 - x,80)*Power(x,20) - 2041841411062132125600*Power(1 - x,79)*Power(x,21) - 7332066885177656269200*Power(1 - x,78)*Power(x,22) -
//                  24865270306254660391200*Power(1 - x,77)*Power(x,23) - 79776075565900368755100*Power(1 - x,76)*Power(x,24) - 242519269720337121015504*Power(1 - x,75)*Power(x,25) -
//                  699574816500972464467800*Power(1 - x,74)*Power(x,26) - 1917353200780443050763600*Power(1 - x,73)*Power(x,27) - 4998813702034726525205100*Power(1 - x,72)*Power(x,28) -
//                  12410847811948286545336800*Power(1 - x,71)*Power(x,29) - 29372339821610944823963760*Power(1 - x,70)*Power(x,30) - 66324638306863423796047200*Power(1 - x,69)*Power(x,31) -
//                  143012501349174257560226775*Power(1 - x,68)*Power(x,32) - 294692427022540894366527900*Power(1 - x,67)*Power(x,33) - 580717429720889409486981450*Power(1 - x,66)*Power(x,34) -
//                  1095067153187962886461165020*Power(1 - x,65)*Power(x,35) - 1977204582144932989443770175*Power(1 - x,64)*Power(x,36) - 3420029547493938143902737600*Power(1 - x,63)*Power(x,37) -
//                  5670048986634686922786117600*Power(1 - x,62)*Power(x,38) - 9013924030034630492634340800*Power(1 - x,61)*Power(x,39) - 13746234145802811501267369720*Power(1 - x,60)*Power(x,40) -
//                  20116440213369968050635175200*Power(1 - x,59)*Power(x,41) - 28258808871162574166368460400*Power(1 - x,58)*Power(x,42) - 38116532895986727945334202400*Power(1 - x,57)*Power(x,43) -
//                  49378235797073715747364762200*Power(1 - x,56)*Power(x,44) - 61448471214136179596720592960*Power(1 - x,55)*Power(x,45) - 73470998190814997343905056800*Power(1 - x,54)*Power(x,46) -
//                  84413487283064039501507937600*Power(1 - x,53)*Power(x,47) - 93206558875049876949581681100*Power(1 - x,52)*Power(x,48) - 98913082887808032681188722800*Power(1 - x,51)*Power(x,49) -
//                  100891344545564193334812497256*Power(1 - x,50)*Power(x,50) - 98913082887808032681188722800*Power(1 - x,49)*Power(x,51) - 93206558875049876949581681100*Power(1 - x,48)*Power(x,52) -
//                  84413487283064039501507937600*Power(1 - x,47)*Power(x,53) - 73470998190814997343905056800*Power(1 - x,46)*Power(x,54) - 61448471214136179596720592960*Power(1 - x,45)*Power(x,55) -
//                  49378235797073715747364762200*Power(1 - x,44)*Power(x,56) - 38116532895986727945334202400*Power(1 - x,43)*Power(x,57) - 28258808871162574166368460400*Power(1 - x,42)*Power(x,58) -
//                  20116440213369968050635175200*Power(1 - x,41)*Power(x,59) - 13746234145802811501267369720*Power(1 - x,40)*Power(x,60) - 9013924030034630492634340800*Power(1 - x,39)*Power(x,61) -
//                  5670048986634686922786117600*Power(1 - x,38)*Power(x,62) - 3420029547493938143902737600*Power(1 - x,37)*Power(x,63) - 1977204582144932989443770175*Power(1 - x,36)*Power(x,64) -
//                  1095067153187962886461165020*Power(1 - x,35)*Power(x,65) - 580717429720889409486981450*Power(1 - x,34)*Power(x,66) - 294692427022540894366527900*Power(1 - x,33)*Power(x,67) -
//                  143012501349174257560226775*Power(1 - x,32)*Power(x,68) - 66324638306863423796047200*Power(1 - x,31)*Power(x,69) - 29372339821610944823963760*Power(1 - x,30)*Power(x,70))

//    qreal p=pt;

//    xt= x*(delta+(1.-delta)/(alpha-y));
//    yt= delta*y+(1.-delta)*x*x*((1./(alpha-y))-1.)*((1./(alpha-y))-1.);

//    pt= (ybar/r)+(1./exp(0.5*log(-beta)))*(x/(alpha-y));
//    rett= pt/p-1;
//    if(rett>0.){absrett= rett;}else{absrett= -rett;}
//    sqrett= rett*rett;
}



void tiwisina::initialize()
{
    xt = x0;
    yt = y0;
}



qreal* tiwisina::sendModelVar()
{
    return &xt;
}


qreal* tiwisina::setLabels(const QString& label)
{
    if( label == "a1" )
        return( &a1);
    if( label == "a2" )
        return( &a2);
    if( label == "a3" )
        return( &a3);
    if( label == "a4" )
        return( &a4);
    if( label == "a5" )
        return( &a5);
    if( label == "a6" )
        return( &a6);
    if( label == "a7" )
        return( &a7);
    if( label == "a8" )
        return( &a8);
    if( label=="xt" )
        return( &xt);
    if( label=="yt" )
        return( &yt);
    if( label=="x0" )
        return( &x0 );
    if( label=="y0" )
        return( &y0 );
    if (label == "x_prime")
        return( &x_prime );
    return NULL;
}


void tiwisina::sendStateSpace(int &quantity,QList<qreal *> *stateSpace)
{
    stateSpace->clear();
    quantity = dimension;
    *stateSpace << &xt;
    *stateSpace << &yt;

//	if( stateSpace )
//		delete stateSpace;
//    *stateSpace = new const qreal* [dimension];
//	if( !(*stateSpace) )
//		fatalError("tiwisina::sendStateSpace",
//		           "Can't create state space vector");
//	quantity=dimension;
//	(*stateSpace)[0]=&xt;
//	(*stateSpace)[1]=&yt;
};


void tiwisina::loadParamset(QTextStream& inFile)
{
    inFile >> a1;
    inFile >> a2;
    inFile >> a3;
    inFile >> a4;
    inFile >> a5;
    inFile >> a6;
    inFile >> a7;
    inFile >> a8;
    inFile >> x0;
    inFile >> y0;
    inFile >> length;

    initialize();
}


void tiwisina::saveParamset(QTextStream& outFile)
{
    outFile << a1	<< "\t";
    outFile << a2	<< "\t";
    outFile << a3	<< "\t";
    outFile << a4	<< "\t";
    outFile << a5	<< "\t";
    outFile << a6	<< "\t";
    outFile << a7	<< "\t";
    outFile << a8	<< "\t";
    outFile << x0 	<< "\t";
    outFile << y0 	<< "\t";
    outFile << length;
}


void tiwisina::printParamset()
{
    log() << a1;
    log() << a2;
    log() << a3;
    log() << a4;
    log() << a5;
    log() << a6;
    log() << a7;
    log() << a8;
    log() << x0;
    log() << y0;
    log() << length;
}


void tiwisina::sendParameters(int& amount,qreal** parameters)
{
    if( *parameters )
        delete *parameters;
    amount=8;
    *parameters=new qreal[amount];
    if( !(*parameters) )
        fatalError("tiwisina::sendParameters",
                   "Can't create array for parameters");
    (*parameters[0])=a1;
    (*parameters[1])=a2;
    (*parameters[2])=a3;
    (*parameters[3])=a4;
    (*parameters[4])=a5;
    (*parameters[5])=a6;
    (*parameters[6])=a7;
    (*parameters[7])=a8;
    (*parameters[8])=x0;
    (*parameters[9])=y0;
    (*parameters[10])=length;
}


void tiwisina::receiveParameters(const QList<qreal>& parameters)
{
    a1=parameters[0];
    a2=parameters[1];
    a3=parameters[2];
    a4=parameters[3];
    a5=parameters[4];
    a6=parameters[5];
    a7=parameters[6];
    a8=parameters[7];
    x0=parameters[8];
    y0=parameters[9];
    length=(long)(parameters[10]);
}


// eof
