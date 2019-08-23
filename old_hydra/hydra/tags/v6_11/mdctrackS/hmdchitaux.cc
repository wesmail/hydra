# include <math.h>
# include "hdebug.h"
# include "hmatrixcategory.h"
# include "hmdccal3.h"
# include "hmdchitaux.h"
# include "hsymmat.h"

ClassImp(HMdcHitAux)

//*-- Author : Beatriz Fuentes Arenaz
//*-- Modified: 25/2/2000 by Beatriz Fuentes
//*-- Modified : 11/06/98 by Beatriz Fuentes 
//*-- Copyright : GENP (Univ. Santiago de Compostela)

////////////////////////////////////////////////////////////////////
// An HMdcHitAux is a segment in a mdc module.                   //
// Basically, it has a point and two slopes and the corresponding //
// errors. It also keep track of the calibrated hits that belong  //
// to the segment.                                                //
// It has the following functions:                                //
//  - a default constructor                                       //
//  - a copy constructor                                          //
//  - a default destructor                                        //
//  - addCal(HMdcCal3* data): add a pointer to the calibrated// 
//           data passed as argument to the segment.              //
//           The pointer to the calibrated hit will be stored in  //
//           the HMatrixCategory, and the number of hits in the   //
//           segment increased by one.                            //
//  - addCal(HMdcCal3* data, int pos):add the calibrated data//
//           passed as argument to the segment in the position pos//
//           pos is an integer between 1 and 6, corresponding to  // 
//           the six planes in a mdc module.                      //
//           The calibrated hit will be stored in the HCategory,  //
//           the number of hits in the segment is increased only  //
//           if ...                                               //
//  - print(ostream &file):prints the segment information to file //
//  - printCal():prints the calibrated hits to the standard output//
//  - operator=                                                   //
//  - markHits(void):increments the flag of all the hits in the   //
//    segment                                                     //
//  - unMarkHits(void):decrements the flag of all the hits in the //
//    segment                                                     //
//                                                                //
////////////////////////////////////////////////////////////////////

  HMdcHitAux :: HMdcHitAux(void){
    // Default constructor
  clear();
}


HMdcHitAux :: HMdcHitAux (HMdcHitAux &copy){
#if DEBUG_LEVEL>3
  gDebuger->enterFunc("HMdcHitAux::HMdcHitAux(copy)");
#endif

    myPoint[0] = copy.getPointX();
    myPoint[1] = copy.getPointY();
    myPoint[2] = copy.getPointZ();
    mySlope[0] = copy.getSlopeX();
    mySlope[1] = copy.getSlopeY();
    myChi = copy.getChi();
    myNumberOfHits = copy.getNumHits();
    for (Int_t nplane=0; nplane < 6 ; nplane++){
      addCal(copy.getCal(nplane),nplane);
    }
    toBeDeleted = copy.isDel();
    isUsed = copy.getIsUsed();

#if DEBUG_LEVEL>3
  gDebuger->leaveFunc("HMdcHitAux::HMdcHitAux(copy)");
#endif

}






  HMdcHitAux :: ~HMdcHitAux(void){
    // Destructor
 
}


void HMdcHitAux ::  clear(void){

    Int_t i;
    for(i=0;i<=2;i++){
      myPoint[i] = 20000.;
    }
    for(i=0;i<2;i++){
      mySlope[i]= 20000.;
    }
    cov.Clear();
    myChi= 20000.;
    myNumberOfHits=0;

    for(i=0; i<6; i++){
      cal[i]= NULL;
    }

    toBeDeleted = 0;
    isUsed = 0;

}


void HMdcHitAux :: setPoint(Float_t x, Float_t y, Float_t z){
  myPoint[0] = x;
  myPoint[1] = y;
  myPoint[2] = z;
}

void HMdcHitAux :: setSlope(Float_t xSlope, Float_t ySlope){
  mySlope[0] = xSlope;
  mySlope[1] = ySlope;
}

void HMdcHitAux :: setError(Float_t xError, Float_t yError){
  cov.setErr(0,xError);
  cov.setErr(1,yError);
}

void HMdcHitAux :: setErrorSlope(Float_t xerror, Float_t yerror){
  cov.setErr(2,xerror);
  cov.setErr(3,yerror);
}

void HMdcHitAux :: addCal(HMdcCal3* data){  

  // add a pointer to the calibrated data passed as argument to the segment.
  // The pointer to the calibrated hit will be stored in the HMatrixCategory, 
  // and the number of hits in the segment increased by one.

  cal[myNumberOfHits] = data;  
  myNumberOfHits++;
  }

void HMdcHitAux :: addCal(HMdcCal3* data, Int_t pos){  

  // add the calibrated data passed as argument to the segment in the 
  // position pos. pos is an integer between 0 and 5, corresponding to the
  // six planes in a mdc module.
  // The calibrated hit will be stored in the HCategory, and
  // the number of hits in the segment is increased if the data is included
  // in a new position

  if(cal[pos]==NULL) myNumberOfHits++;
  cal[pos]=data; 

}




  void HMdcHitAux :: print(ostream &file){

    // prints the segment information to file
    file << "isUsed ? " << isUsed << endl;
    file << "tobedeleted ?" << toBeDeleted << endl;
    file << "calibrated data: " << endl;

    HMdcCal3* cal = NULL;
    for (Int_t i=0; i<6; i++){
      cal = getCal(i);
      if(cal!=NULL){
	file << "plane " << i << endl;
	cal->print(file);
      }
    }

    file << "track at Z= " << myPoint[2] << " for " << myNumberOfHits << 
            " hits, " << myNumberOfHits-4 << " DOF " << endl;

  file << "X"       << '\t' << myPoint[0] << " +- " << sqrt(cov.getErr(0)) << endl;
  file << "X Slope" << '\t' << mySlope[0] << " +- " << sqrt(cov.getErr(1)) << endl;
  file << "Y"       << '\t' << myPoint[1] << " +- " << sqrt(cov.getErr(2)) << endl;
  file << "Y Slope" << '\t' << mySlope[1] << " +- " << sqrt(cov.getErr(3)) << 
           '\n'<< '\n';

  file << "chi**2/DOF" << '\t' << myChi << endl;
}

void HMdcHitAux :: printCal(){
// prints the calibrated hits that belong to the segment to the standard output

  for(Int_t i=0; i<6;  i++){
      if(getCal(i)!=NULL){
	cout << "plane " << i+1 << endl;
	getCal(i)->print();
      }
  }
}


void HMdcHitAux :: printCal(ostream& file){
// prints the calibrated hits that belong to the segment to file

  for(Int_t i=0; i<6; i++){
    if(getCal(i)!=NULL){
      getCal(i)->print(file);
    }
  }
}


void HMdcHitAux :: print(){
// prints the calibrated hits that belong to the segment to standard output
  cout << "isUsed ? " << isUsed << endl;
  cout << "number of hits" << '\t' << myNumberOfHits << endl;
  for(Int_t i=0; i<6; i++){
    if(getCal(i)!=NULL){
      getCal(i)->print();
    }
  }
}




void HMdcHitAux :: operator= (HMdcHitAux *copy){

#if DEBUG_LEVEL>3
  gDebuger->enterFunc("HMdcHitAux::operator=(HMdcHitAux*)");
#endif

    myPoint[0] = copy->getPointX();
    myPoint[1] = copy->getPointY();
    myPoint[2] = copy->getPointZ();
    cov.setErr(0,copy->getXError());
    cov.setErr(1,copy->getYError());
    mySlope[0] = copy->getSlopeX();
    mySlope[1] = copy->getSlopeY();
    cov.setErr(2,copy->getErrorSlope1());
    cov.setErr(3,copy->getErrorSlope2());
    myChi = copy->getChi();
    for (Int_t nplane=0; nplane < 6 ; nplane++){
      cal[nplane] = copy->getCal(nplane);
    }
    myNumberOfHits = copy->getNumHits();

#if DEBUG_LEVEL>3
  gDebuger->leaveFunc("HMdcHitAux::operator=(HMdcHitAux*)");
#endif
}




void HMdcHitAux :: operator= (HMdcHitAux &copy){

#if DEBUG_LEVEL>3
  gDebuger->enterFunc("HMdcHitAux::operator=(HMdcHitAux&)");
#endif

    myPoint[0] = copy.getPointX();
    myPoint[1] = copy.getPointY();
    myPoint[2] = copy.getPointZ();
    cov.setErr(0,copy.getXError());
    cov.setErr(1,copy.getYError());
    mySlope[0] = copy.getSlopeX();
    mySlope[1] = copy.getSlopeY();
    cov.setErr(2,copy.getErrorSlope1());
    cov.setErr(3,copy.getErrorSlope2());
    myChi = copy.getChi();
    for (Int_t nplane=0; nplane < 6 ; nplane++){
      cal[nplane] = copy.getCal(nplane);
    }
    myNumberOfHits = copy.getNumHits();

#if DEBUG_LEVEL>3
  gDebuger->leaveFunc("HMdcHitAux::operator=(HMdcHitAux&)");
#endif

}



void HMdcHitAux :: markHits(void){
  // increments the flag of all the hits in the segment
# if DEBUG_LEVEL >2
  gDebuger->enterFunc("HMdcHitAux::markHits");
#endif


  for(Int_t i=0; i<6 ; i++){
    if(cal[i]!=NULL) cal[i]->incrFlag();
  }

# if DEBUG_LEVEL >2
  gDebuger->leaveFunc("HMdcHitAux::markHits");
#endif
}


void HMdcHitAux :: unMarkHits(void){
  // decrements the flag of all the hits in the segment
  for(Int_t i=0; i<6 ; i++){
    if(cal[i]!=NULL){cal[i]->decrFlag();}
  }

}








