using namespace std;
# include <fstream>  
# include <math.h> 
# include "TFile.h" 
# include "TH1.h" 
# include "TMatrix.h" 
# include "TNtuple.h" 
# include "hades.h" 
# include "hdebug.h" 
# include "hdetector.h" 
# include "hevent.h" 
# include "hlocation.h" 
# include "hmatrixcategory.h" 
# include "hmatrixcatiter.h" 
# include "hmdccal2.h" 
# include "hmdccal3.h" 
# include "hmdccaltable.h" 
# include "hmdcdetector.h" 
# include "hmdcmodulegeometry.h" 
# include "hmdcgeomstruct.h" 
# include "hmdchit.h" 
# include "hmdchitaux.h" 
# include "hmdchitf.h" 
# include "hmdchitfpar.h" 
# include "hmdchitsfilter.h" 
# include "hmdctargetgeometry.h" 
# include "hreconstructor.h" 
# include "hruntimedb.h" 
# include "hspectrometer.h" 
# include "mdcsdef.h" 
 
ClassImp(HMdcHitF) 
 
//*-- Author : Beatriz Fuentes Arenaz 
//*-- Date: 5/98 
//*-- Last modified:16/08/2000 (adapt the code to the official data levels) 
//*-- Last modified : 9/10/99 
//*-- Copyright : GENP (Univ. Santiago de Compostela) 
 
/////////////////////////////////////////////////////////////////////////// 
// HMdcHitF is the reconstructor that transforms the calibrated      // 
// hits in one mdc module (HMdcCal3) in segments (HMdcHitAux).        // 
// It contains the parameter of the fit (maximum chi2 allowed, minimum   // 
// number of hits required in a segment...), a pointer to the calibrated // 
// data to be fitted, to the corresponding geometry, and the collection  // 
// of segments obtained from the fit.                                    // 
// It contains the following functions:                                  // 
//    - a default constructor: initializes  minNumHits = 4,  maxChi = 1, // 
//                             maxNumCommonHits = 3                      // 
//    - HMdcHitF(HMatrixCategory* data, HMdcModuleGeometry* mdcGeo):       // 
//           Constructor:                                                // 
//               - sets the minimum number of hits to 4                  // 
//               - sets the maximum chi2 to 1.                           // 
//               - mdcCal points to the data passed as argument, and the // 
//                 appropiate setup is done                              // 
//               - geo points to mdcGeo, the geometry passed as argument // 
//    - HMdcHitF(HCategory* data,HMdcModuleGeometry* mdcGeo,HCategory* seg)// 
//           Constructor:                                                // 
//               - sets the minimum number of hits to 4                  // 
//               - sets the maximum chi2 to 1.                           // 
//               - mdcCal points to the data passed as argument, and the // 
//                 appropiate setup is done                              // 
//               - geo points to mdcGeo, the geometry passed as argument // 
//               - mdcSegments points to seg, the segments category      // 
//    - a default destructor                                             // 
//    - setMinNumHits(int min): sets minimum number of hits to min.      // 
//        If min < 4, sets minimum number of hits to 4, and an error     //  
//        message is printed                                             // 
//    - fit(HMdcCal3* listCal[6], int listPlanes[6], int numHits):  // 
//          General Least Squares fit to get slopes and intercepts in 2  // 
//          projections plus their variances. It is based on the FORTRAN // 
//          subroutine STFIT1.                                           // 
//          Fits the calibrated data of listCal and stores the result of // 
//          the fit in a segment.                                        // 
//    - checkFit(HMdcHitAux* segment): Checks the fit: returns TRUE if  //  
//         * the number of hits in the segment is greater or equal than  // 
//           minNumHits.                                                 // 
//         and                                                           //  
//         * the chi2 of the fit is not greater than maxChi.             // 
//    - find(int listPlanes[6], Int_t nWantedHits, Int_t seg):               // 
//        Finds segments with hits belonging to the planes listed in the // 
//        nWantedHits first elements of listPlanes. The segments, once   // 
//        they pass several checks, are stored in the collection of      // 
//        segments, starting in the position given by seg.               // 
//        The basic procedure is the following:                          // 
//        * find hits in planes listPlanes[0] .. listPlanes[3].          // 
//        * calculate the parameters of the straight line defined by     // 
//          these hits                                                   // 
//        * looks for hits Int_t listPlanes[4] and/or listPlanes[5]        // 
//          (depending on nWantedHits), that are close to the            // 
//          intersection of that line with these planes.                 // 
//        * call the fit function with this collection of hits.          // 
//        * if the segment passes checkFit and checkCommon then it is    // 
//          added to the collection of segments in the position          // 
//          indicated by seg, and seg is incremented.                    // 
//     - markHits(Int_t first, Int_t last):  mark used-hits in segments      // 
//        in the positions between first and last in the collection of   // 
//        segments.                                                      // 
//     - checkCommon(HMdcHitAux* thissegment):  check if thissegment    // 
//         has common hits with other segments in the collection of      // 
//         segments. If some other segment (let's call it oldsegment) is // 
//         found with at least maxNumCommonHits in common with           // 
//         thissegment, then the chi-square of the segments are compared,// 
//         and the one with lower chi-square is keeped in the collection // 
//         of segments :                                                 // 
//            * if the lower chi-square belongs to thissegment, then     // 
//              oldsegment is marked to be deleted and then the function // 
//              returns FALSE. After that, in find function, thissegment // 
//              will be added to the collection of segments              // 
//            * if the lower chi-square corresponds to oldsegment, then  // 
//              the function returns TRUE, and thissegment will not be   // 
//              added to the collection of segments in the find function // 
//     - printSegments(ofstream& file): print information about the      // 
//         segments in the segments collection to file                   // 
//     - execute(void):finds combination of planes where to search for   // 
//         hits, and calls the function find with this combination.      // 
//     - plotResid (Int_t plane, Text_t* filename= "res1.root",            // 
//                 Text_t* hisname="hres", Int_t nbinsx =100,            // 
//                 Axis_t xlow = -0.05, Axis_t xup=0.05):                // 
//         Calculates residues (fitted coordinate - measured coordinate) // 
//         in the plane number "plane". These residues fill an histogram,//   
//         and the histogram is stored as "hisname"(default:"hres") in   // 
//         file "filename"(default:"res1.root"). The histogram will have // 
//         "nbinsx" bins in x axis (default: 100), and the range of x    // 
//         coordinate is (xlow,xup) (default: (-0.05,0.05)               // 
/////////////////////////////////////////////////////////////////////////// 
 
 
  void HMdcHitF :: fit(HMdcHitAux* data){ 
//  
// General Least Squares fit to get slopes and intercepts in 2 projections 
// plus their variances. It is based on the FORTRAN subroutine STFIT1 
// Fits the calibrated data of listCal passed as argument and stores  
// the result of the fit in a segment. 
//  
// Variables: 
//       nplane = number of planes involved in the fit 
//       cosn, sinn = angle of each plane involved in the fit 
//       zplane = z-position of planes involved in the fit 
//       theta = hit coordinate on each plane   (hit = xsin  - ycos) 
//       zpoint = z coordinate at which fit results are given, If zpoint<-1000 
//                then the mean value of the z positions of the planes used  
//                for the fit is used.  
// 
// The following relation is assumed: 
// 
// theta[i] = sinn[i] * (myPoint[0] + (zplane[i] - myPoint[2]) * mySlope[0]) 
//           -cosn[i] * (myPoint[1] + (zplane[i] - myPoint[2]) * mySlope[1]) 
//   
//  
// 
#if DEBUG_LEVEL>2  
  gDebuger->enterFunc("HMdcHitF::fit"); 
#endif 
 
  Double_t a11 = 0.0, a12 = 0.0, a13 = 0.0, a14 = 0.0; 
  Double_t a22 = 0.0, a23 = 0.0, a24 = 0.0; 
  Double_t a33 = 0.0, a34 = 0.0, a44 = 0.0;  
  Double_t b11, b12, b13, b14, b22, b23, b24, b33, b34, b44;  
  Double_t tt1 = 0.0, tt2 = 0.0, tt3 = 0.0, tt4 = 0.0; 
  Double_t s1, s2, s3, s4, aa, bb, th;  
  Double_t thx, xx, sss1, sss2, d1, d2, r311, r312, r321, r322;  
  Double_t r511, r512, r522, sig2, error4[10], xg, weight; 
  //  Double_t resid5[16]; //unused 
  Double_t thfit, chi;  
  Int_t nhit=0; 
 
  //HMdcHitAux data; 
 
Double_t theta[6]; 
 
//  for(i=0; i<numHits; i++){ 
//    theta[i] = listCal[i]->getPos(); 
//    data.addCal(listCal[i], listPlanes[i]); 
//  } 
//  data.setHits(numHits); 
 
    
  const Int_t nplane = 6; 
  const Int_t numHits = data->getNumHits();  
 
  Int_t j=0,i;                               
  Int_t listPlanes[6];                       
 
  for(i=0; i< nplane; i++){                  
    if (data->getCal(i)!=NULL){              
      theta[j] = data->getCal(i)->getPos();  
      listPlanes[j] = i;                     
      j++;                                   
    }                                        
  }                                          
 
#if DEBUG_LEVEL>3 
  ofstream fitfile("fit.txt",ios::app); 
  fitfile << "suceso " << fEventId << endl; 
  for(i=0; i<numHits; i++){ 
    fitfile << i << '\t' << listPlanes[i] << '\t' << theta[i] << endl; 
  } 
  fitfile << endl; 
  fitfile.close(); 
#endif 
 
  Int_t sector = fLoc[0]; 
  Int_t module = fLoc[1]; 
  //Double_t zpoint = geo->getZPlane(sector,module,0); 
  Double_t zpoint = parContainer->getZHit(sector,module); 
 
  Double_t cosn[nplane], sinn[nplane], zplane[nplane], sgpln[nplane]; 
  //  Double_t y[nplane], z[nplane], yslope, zslope; 
 
 
  for(i=0; i<numHits; i++){ 
    zplane[i] = geo->getZPlane(sector,module,listPlanes[i]); 
    cosn[i] = geo->getCosn(sector,module,listPlanes[i]); 
    sinn[i] = geo->getSinn(sector,module,listPlanes[i]); 
    sgpln[i] = geo->getSgpln(sector,module,listPlanes[i]); 
  } 
 
#if DEBUG_LEVEL>3 
/* 
//            Print input data 
*/ 
ofstream output("fit.txt",ios::app); 
output << "number of planes=" << nplane << '\n'; 
output << "number" << '\t' <<"zplane"<< '\t'<< "cos[i]" <<'\t' << 
  "sin[i]"<< '\t'<< "hits [i]" <<'\t' << "rms[i]" << '\n'; 
 
for (i=0; i<nplane; i++){ 
  output << i+1 <<'\t' << zplane[i] <<'\t' << cosn[i] <<'\t' << sinn[i] << 
    '\t' <<theta[i] << '\t'<< sgpln[i] << '\n'; 
} 
 
# endif 
 
 
 
/*  
//            Take z coordinate from  input??? 
*/  
 
 if (zpoint > -1000.0){ 
 
   nhit = numHits; 
   data->setPointZ(zpoint); 
} 
 else { 
/* 
//            Get z coordinate for results 
*/ 
   sss1 = 0.0; 
   sss2 = 0.0; 
   for(i=0; i < nplane; i++){ 
        nhit = nhit + 1; 
        weight = 1.0/(sgpln[i]*sgpln[i]); 
        sss1 = sss1 + zplane[i] * weight; 
        sss2 = sss2 + weight; 
     data->setPointZ(sss1/sss2); 
  } 
} 
 
 
/* 
//Protect against stupid values 
*/ 
 
 //  if(fabs(data.getPointZ()) > 20.0) data.setPointZ(0.0); 
   xg = data->getPointZ(); 
 
 
/* 
// Set up normal equations 
*/ 
 
  data->setChi(1.0E05); 
 
for (i=0; i<numHits; i++){ 
  aa = -cosn[i]; 
  bb = sinn[i]; 
  xx = zplane[i] - xg; 
  th = theta[i]; 
 
/*  
//Check there is a data point for this plane 
*/ 
 
  sig2 = 1.0/(sgpln[i] * sgpln[i]); 
  thx = th * xx; 
  s1 = aa * aa * sig2; 
  s2 = bb * bb * sig2; 
  s3 = aa * bb * sig2; 
  aa = aa * sig2; 
  bb = bb * sig2; 
  s4 = xx * xx; 
  a11 = a11 + s1; 
  a12 = a12 + s1 * xx; 
  a13 = a13 + s3; 
  a14 = a14 + s3 * xx; 
  a22 = a22 + s4 * s1; 
  a24 = a24 + s3 * s4; 
  a33 = a33 + s2; 
  a34 = a34 + s2 * xx; 
  a44 = a44 + s2 * s4; 
  tt1 = tt1 + aa * th; 
  tt2 = tt2 + aa * thx; 
  tt3 = tt3 + bb * th; 
  tt4 = tt4 + thx * bb; 
 
}  
 
 a23 = a14; 
 
 /* 
// Inverse of 4*4 strictly positive symmetric matrix 
*/ 
 
 d1   = 1.0 / (a12*a12 - a11*a22); 
 
 r311 = (a22*a13 - a12*a23) * d1; 
 r312 = (a22*a14 - a12*a24) * d1; 
 r321 = (a11*a23 - a12*a13) * d1; 
 r322 = (a11*a24 - a12*a14) * d1; 
 
 r511 = a13*r311 + a23*r321 + a33; 
 r512 = a13*r312 + a23*r322 + a34; 
 r522 = a14*r312 + a24*r322 + a44; 
 
 d2 = 1.0 / (r511*r522 - r512*r512); 
 
 b33 =  r522 * d2; 
 b34 = -r512 * d2; 
 b44 =  r511 * d2; 
 
 b13 = r311 * b33 + r312 * b34; 
 b14 = r311 * b34 + r312 * b44; 
 b23 = r321 * b33 + r322 * b34; 
 b24 = r321 * b34 + r322 * b44; 
 
 b11 = r311*b13 + r312*b14 - a22 * d1; 
 b12 = r311*b23 + r312*b24 + a12 * d1; 
 b22 = r321*b23 + r322*b24 - a11 * d1; 
 
/* 
// Fill packed error matrix on y', z', y, z (in that order) 
*/ 
 
 error4[0] = b44; 
 error4[1] = b24; 
 error4[2] = b34; 
 error4[3] = b14;  
 error4[4] = b22; 
 error4[5] = b23; 
 error4[6] = b12; 
 error4[7] = b33; 
 error4[8] = b13; 
 error4[9] = b11; 
 
 
/* 
// Fitted variables 
*/ 
/*  
 cout << "error X = " << b33 << endl; 
 cout << "error Y = " << b11 << endl; 
 cout << "errorSlope X = " << b44 << endl; 
 cout << "errorSlope Y = " << b22 << endl; 
*/ 
 
 data->setErrorX(b33); 
 data->setErrorY(b11); 
 data->setErrorSlopeX(b44); 
 data->setErrorSlopeY(b22); 
 data->setPointY(b11*tt1 + b12*tt2 + b13*tt3 + b14*tt4); 
 data->setSlopeY(b12*tt1 + b22*tt2 + b23*tt3 + b24*tt4); 
 data->setPointX(b13*tt1 + b23*tt2 + b33*tt3 + b34*tt4); 
 data->setSlopeX(b14*tt1 + b24*tt2 + b34*tt3 + b44*tt4); 
 
 // Fill not diagonal elements of the covariance matrix 
 
 data->setCovElement(0,1,b13); 
 data->setCovElement(0,2,b34); 
 data->setCovElement(0,3,b23); 
 data->setCovElement(1,2,b14); 
 data->setCovElement(1,3,b12); 
 data->setCovElement(2,3,b24); 
 
/*  
// For no D.O.F. set Chi = 0.0 
*/ 
 
 if(nhit == 4) data->setChi(0.0); 
 else{ 
 chi = 0.0; 
 for (i=0; i<numHits; i++){ 
     aa = -cosn[i]; 
     bb = sinn[i]; 
     xx = zplane[i] - xg; 
     thfit = aa*(data->getPointY() + data->getSlopeY()*xx) + 
             bb*(data->getPointX() + data->getSlopeX()*xx); 
          chi = chi + pow((thfit-theta[i]),2)/(pow(sgpln[i],2));  
	  // chi = chi + (theta[i] - thfit)*(theta[i] - thfit);  
 } 
 
 data->setChi(chi/(nhit-4)); 
 } 
 
 
# if DEBUG_LEVEL>3 
/* 
// Print out output 
*/ 
 ofstream output("fit.txt",ios::app); 
 output << "event " << fEventId << endl; 
 data->print(output); 
 
 /* 
// Make a Quick check of the fit 
*/ 
 
 output << '\n' << "Check results >>>" << '\n'; 
 Double_t thetth, deviat, xochi2; 
 
 for (i=0; i<nplane; i++){ 
   thetth = sinn[i] * (data->getPointX() + (zplane[i]-data->getPointZ())* 
            data->getSlopeX()) - 
            cosn[i] * (data->getPointY() + (zplane[i]-data->getPointZ())* 
            data->getSlopeY()); 
   deviat = fabs((thetth-theta[i])/thetth)*100; 
   xochi2 = pow((thetth-theta[i]),2)/(pow(sgpln[i],2)); 
 
   output << "Plane " << (i+1) << '\n'; 
   output << "Fitted Hit " << thetth << '\n'; 
   output << "Real Hit " << theta [i] << '\n'; 
   output << "Deviation " << deviat << '\n'; 
   output << "chi2 contribution " << xochi2 << '\n' << '\n'; 
 } 
output.close(); 
# endif 
 
 
#if DEBUG_LEVEL>2  
  gDebuger->leaveFunc("HMdcHitF::fit"); 
#endif 
 
//return data; 
} 
 
 
 
 
  HMdcHitF :: HMdcHitF(void){ 
    // Default constructor: sets the minimum number of hits to 4,  
    // the maximum number of common hits to 3 and  
    // maximum chi2 to 1. 
 
#if DEBUG_LEVEL>2 
    gDebuger->enterFunc("HMdcHitF::HMdcHitF()"); 
#endif 
 
    mdcCal           = NULL; 
    mdcSegments      = NULL; 
    fCalCat          = NULL; 
    fList            = NULL; 
    target           = kTRUE;     
    slopeCorrection  = kFALSE; 
    fLoc.setNIndex(2); 
    fEventId = 0; 
    fHitCat          = NULL; 
 
#if DEBUG_LEVEL>2 
    gDebuger->leaveFunc("HMdcHitF::HMdcHitF()"); 
#endif 
} 
 
 
  HMdcHitF :: HMdcHitF(Text_t* name, Text_t* title) :                                    HReconstructor(name, title){ 
 
    // constructor: sets the minimum number of hits to 4,  
    // the maximum number of common hits to 3 and  
    // maximum chi2 to 1. It also sets the reconstructor's name and title. 
#if DEBUG_LEVEL>2 
    gDebuger->enterFunc("HMdcHitF::HMdcHitF(name,title)"); 
#endif 
    mdcCal           = NULL; 
    mdcSegments      = NULL; 
    fCalCat          = NULL; 
    fList            = NULL; 
    fHitCat          = NULL; 
    target           = kTRUE; 
    slopeCorrection  = kFALSE; 
    fLoc.setNIndex(2); 
    fEventId = 0; 
 
#if DEBUG_LEVEL>2 
    gDebuger->leaveFunc("HMdcHitF::HMdcHitF(name,title)"); 
#endif 
} 
 
 
HMdcHitF :: HMdcHitF(HMatrixCategory* data, HMdcModuleGeometry* mdcGeo, HCategory* seg){ 
 
  // Constructor: 
  // - sets the minimum number of hits to 4 
  // - sets the maximum chi2 to 1. 
  // - mdcCal points to the data passed as argument, and the appropiate  
  //   setup is done 
  // - geo points to mdcGeo, the geometry passed as argument 
  // - mdcSegment points to seg 
 
#if DEBUG_LEVEL>2 
    gDebuger->enterFunc("HMdcHitF::HMdcHitF(HMatrixCategory*, HMdcModuleGeometry* , HCategory*)"); 
#endif 
 
    geo = mdcGeo; 
    mdcCal = data; 
    fCalCat = NULL; 
    mdcSegments = seg; 
    fLoc.setNIndex(2); 
    fList=NULL; 
    fHitCat = NULL; 
    target = kTRUE; 
    slopeCorrection  = kFALSE; 
    fEventId=0; 
 
#if DEBUG_LEVEL>2 
    gDebuger->leaveFunc("HMdcHitF::HMdcHitF(HMatrixCategory*, HMdcModuleGeometry* , HCategory*)"); 
#endif 
} 
 
 
 
 
 
HMdcHitF :: HMdcHitF(HCategory* data, HMdcModuleGeometry* mdcGeo, HCategory* seg){ 
  // Constructor: 
  // - sets the minimum number of hits to 4 
  // - sets the maximum chi2 to 1. 
  // - mdcCal points to the data passed as argument, and the appropiate  
  //   setup is done 
  // - geo points to mdcGeo, the geometry passed as argument 
  // - mdcSegments points to seg, the segments category  
 
#if DEBUG_LEVEL>2 
    gDebuger->enterFunc("HMdcHitF::HMdcHitF(HCategory*, HMdcModuleGeometry* , HCategory*)"); 
#endif 
 
    geo = mdcGeo; 
    mdcCal = data; 
    mdcSegments = seg; 
    fCalCat = NULL; 
    fLoc.setNIndex(2); 
    fList=NULL; 
    fHitCat = NULL; 
    target = kTRUE; 
    slopeCorrection  = kFALSE; 
    fEventId=0; 
 
#if DEBUG_LEVEL>2 
    gDebuger->leaveFunc("HMdcHitF::HMdcHitF(HCategory*, HMdcModuleGeometry* , HCategory*)"); 
#endif 
 
} 
 
 
void HMdcHitF ::  setParContainer(void){ 
#if DEBUG_LEVEL>2 
    gDebuger->enterFunc("HMdcHitF::setParContainer"); 
#endif 
 
  geo=(HMdcModuleGeometry*)gHades->getRuntimeDb()->getContainer("MdcModuleGeometry"); 
 
  parContainer=(HMdcHitFPar*)gHades->getRuntimeDb()->getContainer("MdcHitFPar"); 
 
  if(target) 
    targetGeo=(HMdcTargetGeometry*)gHades->getRuntimeDb()->getContainer("MdcTargetGeometry"); 
#if DEBUG_LEVEL>2 
    gDebuger->leaveFunc("HMdcHitF::setParContainer"); 
#endif 
 
} 
 
 
 
 
Bool_t HMdcHitF :: init(void){ 
#if DEBUG_LEVEL>2 
  gDebuger->enterFunc("HMdcHitF::init"); 
#endif 
 
  setParContainer(); 
 
 
  fCalCat = gHades->getCurrentEvent()->getCategory(catMdcCal2); 
  if (!fCalCat) { 
    fCalCat=gHades->getSetup()->getDetector("Mdc")->buildCategory(catMdcCal2); 
    if (!fCalCat) return kFALSE; 
    else gHades->getCurrentEvent()->addCategory(catMdcCal2,fCalCat,"Mdc"); 
  } 
 
  fHitCat = gHades -> getCurrentEvent() -> getCategory (catMdcHit); 
  if (! fHitCat ){ 
    fHitCat = gHades->getSetup()->getDetector("Mdc")->buildCategory(catMdcHit); 
    if (!fHitCat) return kFALSE; 
    else gHades->getCurrentEvent()->addCategory(catMdcHit,fHitCat,"Mdc"); 
  } 
 
    TArrayI* ind = new TArrayI(4); 
    HMdcGeomStruct* p= 
      (HMdcGeomStruct*)gHades->getRuntimeDb()->getContainer("MdcGeomStruct"); 
    if (!p) return 0; 
    p->getMaxIndices(ind); 
    Int_t nSizes=0; 
    Int_t* sizes; 
 
  mdcCal = gHades->getCurrentEvent()->getCategory(catMdcCalHit); 
  if(!mdcCal){ 
 
    nSizes=ind->GetSize(); 
    sizes=new Int_t[nSizes]; 
    for (Int_t i=0;i<nSizes-1;i++) sizes[i]= ind->At(i)+1;  
 
    sizes[nSizes-1] = 2 * (ind->At(nSizes-1)+1);   // 2 calhits per cal2 
 
    mdcCal = new HMatrixCategory(getCalHitClassName(),nSizes,sizes,0.5); 
    gHades->getCurrentEvent()->addCategory(catMdcCalHit,mdcCal,"Mdc"); 
    delete [] sizes; 
  } 
 
  mdcSegments = gHades->getCurrentEvent()->getCategory(catMdcSegment); 
  if(!mdcSegments){ 
    sizes=new Int_t[nSizes-1]; 
    for (Int_t i=0;i<nSizes-2;i++) sizes[i] = ind->At(i)+1; 
    sizes[nSizes-2] = 1200;   
 
    mdcSegments = new HMatrixCategory("HMdcHitAux",nSizes-1,sizes,0.5); 
    mdcSegments->setPersistency(kFALSE); 
    gHades->getCurrentEvent()->addCategory(catMdcSegment,mdcSegments,"Mdc"); 
    delete [] sizes; 
  } 
 
  delete ind; 
  fActive = kTRUE; 
 
  Int_t layers = ((HMdcDetector* )(gHades->getSetup()->getDetector("Mdc")))->getMaxComponents(); 
  
 
  fList = new HMdcCalLinkList*[layers]; 
  for(Int_t i=0; i< layers; i++) fList[i] = new HMdcCalLinkList; 
 
 
#if DEBUG_LEVEL>2 
  gDebuger->leaveFunc("HMdcHitF::init"); 
#endif 
     
  return kTRUE; 
     
} 
 
 
 
void HMdcHitF :: setLoc(HLocation &location){ 
  fLoc.setNIndex(2); 
  fLoc.readIndexes(location); 
  if(fLoc[1] > 1) target = kFALSE; 
} 
 
 
HMdcHitF :: ~HMdcHitF(void){ 
  // Default destructor 
   
#if DEBUG_LEVEL >2 
  gDebuger->enterFunc("HMdcHitF::~HMdcHitF"); 
#endif 
 
  // Int_t layers  = ((HMdcDetector* )(gHades->getSetup()->getDetector("Mdc")))->getMaxComponents();       // el setup se borra antes que las tareas !! 
 
  Int_t layers = 6; 
 
  for(Int_t i = 0; i < layers; i++){ 
    if(fList[i]) {  
	fList[i]->deleteList(); 
	delete fList[i]; 
    } 
  } 
  if(fList) delete[] fList; 
 
#if DEBUG_LEVEL >2 
    gDebuger->leaveFunc("HMdcHitF::~HMdcHitF"); 
#endif 
} 
 
 
 
 
 
Bool_t HMdcHitF :: checkFit(HMdcHitAux* segment){ 
  // Checks the fit: returns TRUE only if  
  // - the number of hits in the segment is greater or equal than the 
  //   minimum number of hits allowed in the HMdcHitF object. 
  // and 
  // - the chi2 of the fit is not greater than the maximum chi2 defined in the 
  //   HMdcHitF object. 
 
#if DEBUG_LEVEL>2 
  gDebuger->enterFunc("HMdcHitF::checkFit"); 
#endif 
 
 
  Int_t sector = fLoc[0]; 
  Int_t module = fLoc[1]; 
  Float_t maxChi = parContainer->getMaxChi(sector, module); 
  Int_t minNumHits = parContainer->getMinNumHits(sector,module); 
 
  if(segment->getNumHits() < minNumHits || segment->getChi() > maxChi) return kFALSE; 
 
 
#if DEBUG_LEVEL>2 
  gDebuger->leaveFunc("HMdcHitF::checkFit"); 
#endif 
 
  return kTRUE; 
 
} 
 
 
Bool_t HMdcHitF :: firstCheck(TMatrix param){ 
 
#if DEBUG_LEVEL>2 
  gDebuger->enterFunc("HMdcHitF::firstcheck"); 
#endif 
 
  Int_t sector = fLoc[0]; 
  Int_t module = fLoc[1]; 
  Float_t maxSlopeX = parContainer->getMaxSlopeX(sector, module); 
  Float_t minSlopeX = parContainer->getMinSlopeX(sector, module); 
  Float_t maxSlopeY = parContainer->getMaxSlopeY(sector, module); 
  Float_t minSlopeY = parContainer->getMinSlopeY(sector, module); 
 
  //  Float_t roadTargetX = parContainer->getRoadTargetX(sector, module); 
  //Float_t roadTargetY = parContainer->getRoadTargetY(sector, module); 
  //Float_t zplane0 = geo->getZPlane(sector,module,0); 
 
  Float_t zplane0 = parContainer->getZHit(sector,module); 
 
 
 if( (param(2,0) > maxSlopeX) || (param(2,0) < minSlopeX) ||  
     (param(3,0) > maxSlopeY) || (param(3,0) < minSlopeY)) return kFALSE; 
 
 
 if(target){ 
#if 0 
   Double_t x,y; 
   Double_t zdist = targetGeo->getZPos(sector, module)- zplane0; 
   x = param(0,0) + param(2,0) * zdist; 
   y = param(1,0) + param(3,0) * zdist; 
   if(fabs(x) > roadTargetX | fabs(y)> roadTargetY) return kFALSE;  
#endif 
 
 
   // probemos con corte en distancia al eje z 
   Float_t ztarget = targetGeo->getZPos(sector,module); 
 
   Float_t u1[3];          
 
   u1[0] = param(2,0);              //not normalized 
   u1[1] = param(3,0); 
   u1[2] = 1; 
 
 
   Float_t ut[3];         // unitary vector of z axis in mdc coord 
   ut[0] = 0; 
   if (module == 0) { 
     ut[1] = -0.697109; 
     ut[2] = 0.716965; 
   } 
   else if(module==1){ 
     ut[1] = -0.808962; 
     ut[2] = 0.587861; 
   } 
   else{ 
     Error("firstCheck","Unknown module %i",module); 
     return kFALSE; 
   } 
 
   Float_t points[3]; 
   points[0] = param(0,0);                     //xtarget = ytarget = 0 
   points[1] = param(1,0); 
   points[2] = zplane0 - ztarget; 
    
   
   Float_t normcross[3]; 
   TMath::NormCross(u1,ut,normcross); 
    
 
   Float_t distance = 0; 
   for(Int_t l=0; l< 3;l++){ 
     distance = distance + points[l]*normcross[l]; 
   } 
 
   distance = fabs(distance); 
    
   //Float_t maxDistance = 60; 
   Float_t maxDistance = parContainer->getDistZAxis(fLoc[0],fLoc[1]); 
 
   if(distance > maxDistance) return kFALSE; 
 } 
 
#if DEBUG_LEVEL>2 
  gDebuger->leaveFunc("HMdcHitF::firstcheck"); 
#endif 
 
 return kTRUE; 
 
} 
 
 
Int_t HMdcHitF :: find(Int_t listPlanes[6], Int_t nWantedHits, Int_t seg){ 
 
  // Finds segments with hits belonging to the planes listed in the  
  // nWantedHits first elements of listPlanes. The segments, once they 
  // pass several checks, are stored in the collection of segments,  
  // starting in the position given by seg. 
  // The basic procedure is the following: 
  //   - find hits in planes listPlanes[0] .. listPlanes[3]. 
  //   - calculate the parameters of the straight line defined by these hits 
  //   - looks for hits int listPlanes[4] and/or listPlanes[5] (depending 
  //     on nWantedHits), that are close to the intersection of that line 
  //     with these planes. 
  //   - call the fit function with this collection of hits. 
  //   - if the segment passes checkFit and checkCommon then it is added 
  //     to the collection of segments in the position indicated by seg, 
  //     and seg is incremented.  
 
#if DEBUG_LEVEL>2 
      gDebuger->enterFunc("HMdcHitF::find"); 
#endif   
 
  Int_t sector = fLoc[0]; 
  Int_t module = fLoc[1]; 
  Float_t road = parContainer->getRoad(sector,module); 
 
  TMatrix m(4,4);          // matrix to obtain a straight line from four theta 
                           // coordinates 
  TMatrix param(4,1);      // parameters of the straight line 
  TMatrix T(4,1);          // coordinates in each plane        
 
  Double_t z0 = parContainer->getZHit(sector,module); // z coordinate at which results are given 
 
  // the m matrix will allow to fit a straight line with the local coordinates 
  // of the first two planes(listPlanes[0] and listPlane[1]) and the last two  
  // planes (listPlanes[2] and listPlanes[3]). 
 
  HMdcCal3* listCal[6]; 
  HMdcCal3* data; 
  HLocation locSeg; 
  HMdcHitAux *segment= new HMdcHitAux; 
  HMdcHitAux *segment2; 
 
  //char filename[10]; 
  //sprintf(filename,"full%d.txt",fEventId); 
  //ofstream fitted("fitted.txt",ios::app); 
  //ofstream full(filename,ios::app); 
 
  //fitted << "suceso " << fEventId << endl; 
  //fitted << "nwantedhits " << nWantedHits << endl; 
 
  //full << "suceso " << fEventId << endl; 
  //full << "nwantedhits " << nWantedHits << endl; 
 
  Double_t thetaint, thetaint2; 
 
 
  locSeg.setNIndex(3); 
  locSeg.setIndex(0,fLoc.getIndex(0)); 
  locSeg.setIndex(1,fLoc.getIndex(1)); 
 
  //cout << "matriz" << endl; 
  for(Int_t i=0; i<4; i++){ 
    m(i,0) = geo->getSinn(sector, module, listPlanes[i]); 
    m(i,1) = -geo->getCosn(sector, module,listPlanes [i]); 
    m(i,2) = m(i,0) * (geo->getZPlane(sector, module,listPlanes[i])-z0); 
    m(i,3) = m(i,1) * (geo->getZPlane(sector, module,listPlanes[i])-z0); 
  } 
  m.Invert();                          // stores the inverse of m in m 
 
 
  Int_t layers = geo->getNLayers(); 
  HMdcCalLinkListIter **iter = new HMdcCalLinkListIter*[layers];  
  for (Int_t i=0;i<layers;i++) iter[i]=0; 
 
  HMdcCalLink* link; 
 
  for(Int_t i = 0; i< nWantedHits; i++){ 
    iter[listPlanes[i]] = (HMdcCalLinkListIter* )fList[listPlanes[i]]->MakeIterator(); 
  } 
 
 
  iter[listPlanes[0]]->Reset(); 
  while((data=(HMdcCal3 *)iter[listPlanes[0]]->Next())!=NULL){ 
    // loop in hits in plane listPlanes[0] 
 
    //data->print(full); 
    T(0,0) = data->getPos(); 
    listCal[0]= data; 
    //full << "empezando a iterar" << endl; 
    //full << data->getLayer() << '\t' << data->getCell() << '\t' << data->getHitNumber() << endl; 
 
    iter[listPlanes[1]]->Reset(); 
    while((data=(HMdcCal3 *)iter[listPlanes[1]]->Next())!=NULL){ 
      // loop in hits in plane listPlanes[1] 
 
      T(1,0) = data->getPos(); 
      listCal[1] = data; 
      //full << data->getLayer() << '\t' << data->getCell() << '\t' << data->getHitNumber() << endl; 
 
 
      iter[listPlanes[2]]->Reset(); 
      while((data=(HMdcCal3 *)iter[listPlanes[2]]->Next())!=NULL){ 
	// loop in hits in plane listPlanes[2] 
 
	T(2,0) = data->getPos(); 
	listCal[2] = data; 
	//full << data->getLayer() << '\t' << data->getCell() << '\t' << data->getHitNumber() << endl; 
 
 
	iter[listPlanes[3]]->Reset(); 
	while((data=(HMdcCal3 *)iter[listPlanes[3]]->Next())!=NULL){ 
	  // loop in hits in plane listPlanes[3] 
 
	  T(3,0) = data->getPos(); 
	  listCal[3] = data; 
	  //full << data->getLayer() << '\t' << data->getCell() << '\t' << data->getHitNumber() << endl; 
 
	  param.Mult(m,T); 
	  if(firstCheck(param)){ 
 
	    // loop over intermediate planes 
	    if (nWantedHits > 4){ 
	      // calculates the intersection points between the straight line  
	      // and the intermediate plane (listPlanes[4]) 
	       
	      thetaint = geo->getSinn(sector, module,listPlanes[4])* 
		         (param(0,0) + param(2,0)* 
                         (geo->getZPlane(sector, module,listPlanes[4])-z0))-  
                         geo->getCosn(sector, module,listPlanes[4])* 
	                 (param(1,0)+param(3,0)*  
		         (geo->getZPlane(sector, module,listPlanes[4])-z0)); 
 
	      Int_t cellNum = calculateCell(sector,module,listPlanes[4],thetaint); 
 
	      if(cellNum >=0){ 
		Float_t pitch = geo->getPitch(sector,module,listPlanes[4]); 
		Int_t roadCells = Int_t(road/pitch)+1; 
 
		Int_t minCell1 = cellNum - roadCells; 
		if (minCell1 < 0) minCell1 = 0; 
		Int_t maxCell1 = cellNum + roadCells; 
 
		//full << "extrapolacion a layer " << listPlanes[4] << ":"<< endl; 
		//full << "coord " << thetaint << '\t' << "cell " << cellNum << '\t' << minCell1 << '\t' << maxCell1 << endl; 
 
		//iter[listPlanes[4]]->Reset(); 
		//while((data=(HMdcCal3 *)iter[listPlanes[4]]->Next())!=NULL){ 
 
 
		for(Int_t m1 = minCell1; m1 <= maxCell1; m1++){ 
		  // loop in cells in listPlanes[4] 
 
 
		  HMdcCell* cell1 = fList[listPlanes[4]]->getCell(m1); 
		  Int_t nHits1 = cell1->getNHits(); 
 
		  //full << "cell " << m1 << "   nhits " << nHits1 << endl;  
		   
		  for(Int_t n1=0; n1<nHits1; n1++){ 
		    // loop in hits in  each cell 
		    link = cell1->getHit(n1); 
		    if(link!=NULL){ 
		      data = link->getData(); 
 
		      Double_t coord;               
		      coord = data->getPos();       
		      //  if (coord>thetaint+road) break;  
		    
		      if(fabs(coord-thetaint) <= road){    
			//full << "ok primer check con :" << endl; 
			listCal[4]= data; 
			//for(Int_t p=0; p<=4; p++) listCal[p]->print(full); 
			 
 
			if (nWantedHits > 5){ 
			  //calculates the intersection points between the straight  
			  // line and the intermediate plane (listPlanes[5]) 
			   
			  thetaint2 = geo->getSinn(sector,module,listPlanes[5])* 
			    (param(0,0)+param(2,0)* 
			     (geo->getZPlane(sector, module, 
					     listPlanes[5])-z0))-  
			    geo->getCosn(sector, module,listPlanes[5])* 
			    (param(1,0)+ param(3,0)* 
			     (geo->getZPlane(sector, module, 
					     listPlanes[5])-z0)); 
			   
			  cellNum = calculateCell(sector,module,listPlanes[5],thetaint2); 
			  if(cellNum>=0){ 
			    pitch = geo->getPitch(sector,module,listPlanes[5]); 
			    roadCells = Int_t(road/pitch)+1; 
			     
			    Int_t minCell2 = cellNum - roadCells; 
			    if (minCell2 < 0) minCell2 = 0; 
			    Int_t maxCell2 = cellNum + roadCells; 
			     
			    //full << "extrapolacion a layer " << listPlanes[5] << ":"<< endl; 
			    //full << "coord " << thetaint2 << '\t' << "cell " << cellNum << '\t' << minCell2 << '\t' << maxCell2 << endl; 
			     
			    //iter[listPlanes[5]]->Reset(); 
			    //while((data=(HMdcCal3 *)iter[listPlanes[5]]->Next())!=NULL){ 
			    for(Int_t m2 = minCell2; m2 <= maxCell2; m2++){ 
			    // loop in cells in listPlanes[5] 
			       
 
			      HMdcCell* cell2 = fList[listPlanes[5]]->getCell(m2); 
			      Int_t nHits2 = cell2->getNHits(); 
			      //full << "cell " << m2 << "   nhits " << nHits2 << endl;  
			      for(Int_t n2=0; n2<nHits2; n2++){ 
				// loop in hits in  each cell 
				link = cell2->getHit(n2); 
				if(link!=NULL){ 
				  data = link->getData(); 
			       
				  coord = data->getPos();       
				//  if (coord>thetaint2+road) break;  
				 
				  if(fabs(coord-thetaint2) <= road){      
				    listCal[5] = data; 
				    //full << "second check ok con: " << endl; 
				    //data->print(full); 
				    //full << endl << endl; 
				     
				    for(Int_t m=0; m< nWantedHits; m++){ 
				      segment->addCal(listCal[m],listPlanes[m]); 
				    } 
				    fit(segment); 
				     
				//segment->print(full); 
				 
				    if(checkFit(segment)){ 
				      //segment->print(fitted); 
				      if(!checkCommon(segment)){ 
				// if segment does not have to be deleted then  
				// add segment to segments category       
				//full<< "anhadir a la categoria de segmentos en la posicion " << seg <<endl; 
					locSeg.setIndex(2,seg); 
					segment2 = (HMdcHitAux*)mdcSegments->getSlot(locSeg); 
					if(segment2){  
					  segment2 = new(segment2) HMdcHitAux; 
					  //full << "nuevo segmento " << endl; 
					  *segment2 = *segment; 
					  //segment2->print(full); 
					} 
					seg++;   
					segment->clear(); 
				//full << "segment cleared " << endl; 
				      }                       // if checkCommon 
				    }                           // if checkfit 
				  }                             // if thetaint2 
				} 
			      } 
			    }   // end of loop in plane listPlanes[5] 
			  } 
			}                                  // if nWantedHits>5 
			else{ 
		   
			  for(Int_t m=0; m< nWantedHits; m++){ 
			    segment->addCal(listCal[m],listPlanes[m]); 
			  } 
			  fit(segment); 
			   
			  //segment->print(full); 
			  if(checkFit(segment)){ 
			    //segment->print(fitted); 
			    if(!checkCommon(segment)){ 
			      // if segment don't have to be deleted then  
			      // add segment to segments category       
			       
			      locSeg.setIndex(2,seg); 
			      segment2 = (HMdcHitAux*)mdcSegments->getSlot(locSeg); 
			      if(segment2){ 
			      segment2= new(segment2) HMdcHitAux; 
			      *segment2 = *segment; 
			      //full << "nuevo segmento " << endl; 
			      //segment->print(full); 
			      } 
			      seg++; 
			    segment->clear(); 
			    //full << endl; 
			    } 
			  }       
			} 
		       
			 
		      }                                      // if thetaint 1 
		    } 
		  }         // end of loop in hits en cells in listPlanes[4] 
		}               // end of loop in cells in listPlanes[4] 
	      } 
	    }                                      // if nWantedHits > 4 
	       
	    else{                                 // if nWantedHits=4 
	      //full << "y ahora todos los de 4 hits ... " << endl; 
	      for(Int_t i=0; i<nWantedHits; i++){ 
		segment->addCal(listCal[i], listPlanes[i]); 
	      } 
	      segment->setPointX(param(0,0)); 
	      segment->setPointY(param(1,0)); 
	      segment->setPointZ(z0); 
	      segment->setErrorX(0.); 
	      segment->setErrorY(0.); 
	      //	      segment->setErrorZ(0.); 
	      segment->setSlopeX(param(2,0)); 
	      segment->setSlopeY(param(3,0)); 
	      segment->setErrorSlopeX(0.); 
	      segment->setErrorSlopeY(0.); 
	      segment->setChi(0.); 
	      //fitted << "new 4-hit segment" << endl; 
	      //segment->print(fitted); 
 
	      locSeg.setIndex(2,seg); 
	      segment2 = (HMdcHitAux*)mdcSegments->getSlot(locSeg); 
	      if(segment2){ 
		segment2= new(segment2) HMdcHitAux; 
		*segment2 = *segment; 
	      } 
	      seg++; 
	      segment->clear(); 
	      //full << endl; 
	      //full << "new " << endl; 
	       
#if DEBUG_LEVEL>3 
	      HMdcHitAux* temp; 
	      HMatrixCatIter* iter = NULL; 
	       
	      iter=(HMatrixCatIter *)mdcSegments->MakeIterator(); 
	      iter->Reset(); 
	      iter->gotoLocation(fLoc); 
	      while((temp=(HMdcHitAux *)iter->Next())!=NULL) {  
		if(temp->isDel()==0) temp->print(full); 
	      } 
	      delete iter; iter = 0; 
 
 
 
	      full << endl << endl; 
	      full << endl << endl; 
#endif 
	    }                                      // else nWantedHits > 4 
 
	  } // end of firstCheck 
 
	   
	}                      // end of loop in listPlanes[3] 
 
      }                        // end of loop in listPlanes[2] 
 
    }                          // end of loop in listPlanes[1] 
 
  }                            // end of loop in listPlanes[0] 
  
  delete segment; 
  for(Int_t i=0; i< nWantedHits; i++){ 
    delete iter[listPlanes[i]]; 
    iter[listPlanes[i]] = 0; 
  } 
  delete[] iter; 
  //fitted.close(); 
  //full.close(); 
#if DEBUG_LEVEL>2 
      gDebuger->leaveFunc("HMdcHitF::find"); 
#endif 
  return seg; 
 
} 
 
 
void HMdcHitF :: markHits(Int_t first, Int_t last){ 
  // deletes from the linked list of hits those that have already been used  
  // and marks the corresponding calibrated data.  
  // This function only checks segments with positions between first and 
  // last in the collection of segments. 
#if DEBUG_LEVEL>2 
  gDebuger->enterFunc("HMdcHitF::markHits"); 
#endif 
 
  HMdcHitAux* temp; 
  HMdcCal3* hit, *hit2; 
  HMdcCalLink* partner, *link; 
 
  HLocation loc; 
  loc.setNIndex(3); 
  loc.setIndex(0,fLoc.getIndex(0)); 
  loc.setIndex(1,fLoc.getIndex(1)); 
  loc.setIndex(2,first); 
  Int_t layers = ((HMatrixCategory*)mdcCal)->getSize(2); 
 
  //ofstream mark1("mark1.txt", ios::app); 
  //mark1<< "suceso " << fEventId << endl; 
  //mark1 << "first " << first << endl; 
 
  //ofstream marcado("marcados.txt",ios::app); 
  //marcado << "suceso " << fEventId << endl; 
 
 
  for(; first <= last; first++){ 
    loc.setIndex(2,first); 
    temp = (HMdcHitAux*)mdcSegments->getObject(loc); 
    if(temp!=NULL){ 
      temp->markHits();            // habra que marcar cal2 OJO!!!!! 
      //  temp->print(mark1); 
 
       // remove marked hits (temp and its symetric) from linked list 
 
      for(Int_t i=0; i<layers; i++){ 
	hit = temp->getCal(i); 
	//marcado << "plano " << i << endl; 
	if(hit!=NULL){ 
	  link = hit->getLink(); 
	  if(link != NULL){ 
	    partner = hit->getLink()->getPartner(); 
	    if(partner != NULL){  
	      hit2 = partner->getData(); 
	      if(hit2!=NULL) hit2->incrFlag(); 
	    } 
	  } 
	  //marcado << "voy a borrar ... " << endl; 
	  //hit->print(marcado); 
	  fList[i]->remove(hit); 
	  //marcado << "borrado !!" << endl; 
	  hit->setLink(NULL);             
	} 
      }                                  // end of loop in layers 
    }                                    // end of if temp!=NULL 
 
  }                                     // end of loop in segments 
 
#if DEBUG_LEVEL>2 
  gDebuger->leaveFunc("HMdcHitF::markHits"); 
#endif 
 
} 
 
 
 
Bool_t HMdcHitF:: checkCommon(HMdcHitAux* thissegment){ 
  // check if thissegment has common hits with other segments  
  // in the segments collection. If some other segment (let's 
  // call it oldsegment) is found 
  // with at least maxNumCommonHits in common with thissegment, then 
  // the chi-square of the segments are compared, and the one with 
  // lower chi-square is keeped in the collection of segments : 
  // - if the lower chi-square belongs to thissegment, then oldsegment 
  //   is marked to be deleted and then the 
  //   function returns FALSE. After that, in find function, thissegment 
  //   will be added to the collection of segments 
  // - if the lower chi-square corresponds to oldsegment, then the 
  //   function returns TRUE, and thissegment will not be added to 
  //   the collection of segments in the find function 
 
#if DEBUG_LEVEL>2 
  gDebuger->enterFunc("HMdcHitF::checkCommon"); 
#endif 
  HMdcHitAux *temp; 
  Int_t numCommons = 0;                     // number of common hits   
  Int_t maxNumCommonHits = parContainer->getMaxNumCommonHits(fLoc[0],fLoc[1]); 
 
  //ofstream common("common.txt", ios::app); 
  //common << endl << endl; 
  //common << "suceso " << fEventId << endl; 
 
  //thissegment->print(common); 
  //common << endl; 
  //common << "thissegment has " <<thissegment->getNumHits() << "hits" << endl; 
  //common << "coleccion de segmentos no borrados" << endl; 
 
  HMatrixCatIter* iter = NULL; 
  iter=(HMatrixCatIter *)mdcSegments->MakeIterator(); 
  iter->Reset(); 
  iter->gotoLocation(fLoc); 
  while((temp=(HMdcHitAux *)iter->Next())!=NULL) {  
     
    //common << "temp has " << temp->getNumHits() << "hits" << endl; 
      if(temp->isDel()==0) { 
	//common << "no borrado" << endl; 
	if(temp->getNumHits() == thissegment->getNumHits()){ 
	  //common << "mismo numero de hits" << endl; 
	  //temp->print(common); 
	  for(Int_t i=0; i< 6; i++){ 
	    if(temp->getCal(i)!=NULL && thissegment->getCal(i)!=NULL){ 
	      HMdcCalLink *link=thissegment->getCal(i)->getLink(); 
	      HMdcCalLink *tlink=temp->getCal(i)->getLink(); 
	      if (link && tlink) { 
		HMdcCalLink *partner=link->getPartner(); 
		HMdcCalLink *tpartner=tlink->getPartner(); 
 
		if (partner && tpartner) { 
		  if ((temp->getCal(i)==thissegment->getCal(i) || 
		       temp->getCal(i)==thissegment->getCal(i)->getLink()-> 
		       getPartner()->getData() || 
		       temp->getCal(i)->getLink()->getPartner()->getData()== 
		       thissegment->getCal(i) || 
		       temp->getCal(i)->getLink()->getPartner()->getData()== 
		       thissegment->getCal(i)->getLink()->getPartner()->getData())){ 
		    numCommons++; 
		  }	     
		} else Warning("checkCommon","Partner not found");  
	      } else Warning("checkCommom","Inconsistency in link list"); 
	    }      
	  }                  
 
	  //common << "number of common hits " << numCommons << endl; 
	  if(numCommons > maxNumCommonHits){ 
	    //common << "common hits > maxNumCommonHits" << endl; 
	    if(thissegment->getChi()< temp->getChi()){ 
	      //common << "delete temp" << endl; 
	      temp->setDel();           // delete temp and continue searching 
	    } 
	    else { 
	      delete iter; 
	      //common << "return ktrue" << endl; 
	      return kTRUE; 
	    } 
	  }                 // end of if in numCommons 
	}                    
      }                     // end of if segment has not been deleted 
      //common << "siguiente en la coleccion de segmentos" << endl; 
      numCommons=0; 
    }                     // end of while 
 
    delete iter; iter = 0; 
#if DEBUG_LEVEL >2 
    gDebuger->leaveFunc("HMdcHitF::checkCommon"); 
#endif 
    //common << "return kfalse" << endl; 
    return kFALSE;         // if there is no segment in the collection of  
                          // segments, thissegment must be keeped  
} 
 
 
 
 
 
 
 
 
 
 
void HMdcHitF :: printSegments(ofstream& file){ 
  // print all the segments in the segments category to file 
 
HMdcHitAux* temp; 
HMatrixCatIter* iter = NULL; 
 
HLocation loc; 
iter=(HMatrixCatIter *)mdcSegments->MakeIterator(); 
iter->Reset(); 
iter->gotoLocation(fLoc); 
 while((temp=(HMdcHitAux *)iter->Next())!=NULL) {  
   loc = iter->getLocation(); 
   file << "pos" << '\t' << loc[0] << '\t' << loc[1] << '\t' <<loc[2] << endl;  
   temp->print(file); 
 } 
delete iter; 
 iter = 0; 
} 
 
 
 
void HMdcHitF :: buildList(void){ 
// builds a double linked list with the cal data. 
 
#if DEBUG_LEVEL>2 
  gDebuger->enterFunc("HMdcHitF::buildList"); 
#endif 
// transforms cal2 data into calhit, and 
// builds a double linked list with the calhit data. 
 
#if DEBUG_LEVEL>2 
  gDebuger->enterFunc("HMdcHitF::buildList"); 
#endif 
 
  HMdcCal2* cal; 
 
// loop over cal2 category to fill calhit category 
  
  Int_t nlayers = geo->getNLayers(); 
 
  Float_t pitch[nlayers]; 
  Float_t centralWire[nlayers]; 
 
  Int_t sector = fLoc[0]; 
  Int_t module = fLoc[1]; 
  Int_t layer, cell, nHits; 
  Int_t offset[nlayers]; 
 
 
  for(Int_t i=0; i<nlayers; i++){  
    offset[i] = 0; 
    pitch[i] = geo ->getPitch(sector,module,i); 
    centralWire[i] = geo->getCentralWire(sector,module,i)-1; 
  } 
 
  HLocation loc; 
  loc.set(4,sector,module,0,0);        // location to be used when filling 
                                       // calhit category  
 
  HMdcCal3 *hit1, *hit2; 
  Float_t pos1, pos2, coordwire; 
 
  HIterator* iter=0; 
  iter = (HIterator*)fCalCat->MakeIterator(); 
  iter->Reset(); 
  iter->gotoLocation(fLoc); 
  while((cal=(HMdcCal2*)iter->Next())!=NULL){ 
    nHits = cal->getNHits(); 
 
    if(nHits != 0){ 
      layer = cal->getLayer(); 
      cell  = cal->getCell(); 
       
      // first calhit 
      loc[2] = layer; 
      loc[3] = offset[layer]; 
      hit1 = (HMdcCal3*)mdcCal->getSlot(loc); 
      offset[layer]++; 
       
      // second calhit 
      loc[3] = offset[layer]; 
      hit2 = (HMdcCal3*)mdcCal->getSlot(loc); 
      offset[layer]++; 
       
      	coordwire = (cell-centralWire[layer])*pitch[layer]; 
 
      if(hit1 && hit2){ 
	hit1 = new(hit1) HMdcCal3; 
	hit1->setAddress(sector,module,layer,cell,0); 
	hit1->setError(cal->getErrDist1()); 
	   
	if(layer==0 || layer ==  4) coordwire = -coordwire; 
	 
	pos1 = coordwire + cal->getDist1(); 
	pos2 = coordwire - cal->getDist1(); 
 
	hit1->setPos(pos1); 
	 
	hit2 = new(hit2) HMdcCal3(hit2); 
	hit2->setAddress(sector,module,layer,cell,1);     
	hit2->setPos(pos2); 
	hit2->setError(cal->getErrDist1()); 
	fList[layer]->addHits(hit1,hit2);	  
	 
      } 
       
      if(nHits<-1){ 
	 
	// first calhit 
 
	loc[3] = offset[layer]; 
	hit1 = (HMdcCal3*)mdcCal->getSlot(loc); 
	offset[layer]++; 
	 
	// second calhit 
	 loc[3] = offset[layer]; 
	 hit2 = (HMdcCal3*)mdcCal->getSlot(loc); 
	 offset[layer]++; 
	  
	 if(hit1 && hit2){ 
	   hit1 = new(hit1) HMdcCal3; 
	   hit1->setAddress(sector,module,layer,cell,2); 
	   hit1->setError(cal->getErrDist2()); 
	    
	   pos1 = coordwire + cal->getDist2(); 
	   pos2 = coordwire - cal->getDist2(); 
	    
	   hit1->setPos(pos1); 
	    
	   hit2 = new(hit2) HMdcCal3(hit2); 
	   //hit2->setHitNumber(3); 
	   hit2->setAddress(sector,module,layer,cell,3);     // nueva linea 
	   hit2->setPos(pos2); 
	   hit2->setError(cal->getErrDist2()); 
	   fList[layer]->addHits(hit1,hit2);	  
	    
	 }	  
	  
      }                                      // end of check in nHits < -1 
    }                                        // end of check in nHits != 0 
  }                                          // end of loop in cal2 category 
 
 
#if DEBUG_LEVEL>2 
  gDebuger->leaveFunc("HMdcHitF::buildList"); 
#endif 
 
} 
 
 
 
Float_t HMdcHitF :: calculatePosPartner(HMdcCal3* hit1){ 
   
#if DEBUG_LEVEL>2 
  gDebuger->enterFunc("HMdcHitF::calculatePosPartner"); 
#endif 
 
  Float_t coordwire, distwire; 
  Float_t posPartner; 
  Int_t layer = hit1->getLayer(); 
  Int_t cell = hit1->getCell(); 
  if(layer==0 || layer ==4) cell = -cell; 
 
  Float_t coord = hit1->getPos(); 
 
  coordwire = geo->getFirstWirePos(fLoc[0],fLoc[1],layer) +  
              cell*geo->getPitch(fLoc[0],fLoc[1],layer); 
 
  distwire = coord - coordwire; 
 
  posPartner = coord - 2*distwire; 
 
  //  posPartner = 2*coordwire - hit1->getPos(); 
 
#if DEBUG_LEVEL>2 
  gDebuger->leaveFunc("HMdcHitF::calculatePosPartner"); 
#endif 
 
  return posPartner; 
} 
 
 
 
Int_t HMdcHitF :: execute(void){ 
  // This function finds combination of planes where to search for hits, 
  // and calls the function find with this combination.  
 
#if DEBUG_LEVEL>2 
      gDebuger->enterFunc("HMdcHitF::execute"); 
#endif 
 
 
  buildList(); 
 
#if DEBUG_LEVEL>3 
  ofstream file ("calhit.txt",ios::app); 
  file << "suceso " << fEventId << endl; 
  printCalHitCat(file); 
  file.close(); 
#endif 
 
  Int_t nWantedHits, nMdcPlanes, nPlanesScanned; 
  Int_t nPossibleFirstPlanes, nMissingHitsTolerated; 
  Int_t minNumHits = parContainer->getMinNumHits(fLoc[0],fLoc[1]); 
 
  nMdcPlanes = geo->getNLayers();    
  Int_t segPos =0, posIni=0;         // points to : 
                                   // posIni: actual position 
                                   // segPos: the first available position 
                                   // in the collection of segments 
 
  Int_t listPlanes[6]; 
  HMdcHitsFilter* filtering = new HMdcHitsFilter; 
  // Loop over number of hits wanted on a track. 
  // From maximum number of available Mdc planes 
  // to minimum number of hits permitted per track. 
 
  for(nWantedHits=nMdcPlanes; nWantedHits>=minNumHits;nWantedHits--){ 
    // Loop over number of scanned planes. 
    // From maximum number of available Mdc planes 
    // to number of hits wanted on a track 
 
    for(nPlanesScanned=nMdcPlanes; nPlanesScanned>=nWantedHits; 
	nPlanesScanned--){ 
 
      nMissingHitsTolerated = nMdcPlanes - nWantedHits; 
      nPossibleFirstPlanes = nMdcPlanes - nPlanesScanned +1; 
      Int_t firstScannedPlane = 1; 
      Int_t lastFirstScannedPlane = firstScannedPlane + nPossibleFirstPlanes-1; 
 
      // Loop over all possible first planes to scan. 
      // For each possible first plane to scan there is a last plane to scan 
      // given the number of planes to scan 
 
 
      for(; firstScannedPlane<=lastFirstScannedPlane; firstScannedPlane++){ 
	 
	Int_t lastScannedPlane= firstScannedPlane+nPlanesScanned-1; 
	listPlanes[0] = firstScannedPlane-1; 
	listPlanes[3] = lastScannedPlane-1; 
 
	for(Int_t secondScannedPlane = firstScannedPlane+1; 
	    secondScannedPlane<= firstScannedPlane+nMissingHitsTolerated+1; 
	    secondScannedPlane++){ 
	  listPlanes[1] = secondScannedPlane-1; 
 
	  for(Int_t thirdScannedPlane = lastScannedPlane-1; 
	      thirdScannedPlane-secondScannedPlane>= 3-nMissingHitsTolerated; 
	      thirdScannedPlane--){ 
 
	    listPlanes[2] = thirdScannedPlane-1; 
 
	    Int_t nIntermPlanes = 2 - nMissingHitsTolerated; 
	    Int_t firstIntermPlane = secondScannedPlane +1;	     
	    Int_t lastFirstIntermPlane = thirdScannedPlane-nIntermPlanes; 
 
     	    if(nWantedHits==4){ 
	      segPos=find(listPlanes, nWantedHits,segPos); 
	    }  
	    else{ 
 
	      for (Int_t intermPlane=firstIntermPlane; 
		   intermPlane <= lastFirstIntermPlane; 
		   intermPlane++){ 
 
		for (Int_t index=0; index < nIntermPlanes; index++){ 
		  listPlanes[4+index] = intermPlane+index-1; 
		} 
	     
		segPos=find(listPlanes, nWantedHits,segPos); 
 
	      }    // end of loop over intermPlane 
	    }      // end of else 
	  }        // end of loop over third planes to scan 
	}          // end of loop over second planes to scan 
      }            // end of loop over all possible first planes to scan 
    }              // end of loop over number of scanned planes 
 
    //    ofstream premark("premark.txt",ios::app); 
    //ofstream postmark("postmark.txt",ios::app); 
    //ofstream prefilt("prefilter.txt",ios::app); 
 
    //premark << "suceso " << fEventId << endl; 
    //postmark << "suceso " << fEventId << endl; 
    //prefilt<< "suceso " << fEventId << endl; 
 
    //premark << "nWantedHits " << nWantedHits << endl; 
    //postmark << "nWantedHits " << nWantedHits << endl; 
    //prefilt<< "nWantedHits " << nWantedHits << endl; 
 
 
    //printSegments(prefilt); 
    mdcSegments->filter(*filtering); 
    //printSegments(premark); 
 
    markHits(posIni,segPos-1); 
    //printSegments(postmark); 
    posIni = segPos; 
 
    //premark.close(); 
    //postmark.close(); 
    //prefilt.close(); 
 
  }       // end of loop over number of hits wanted on a track 
 
  delete filtering; 
 
 
 
    for (Int_t i=0; i< 6; i++){ 
      if (fList[i]) fList[i]->deleteList(); 
    } 
 
    if(slopeCorrection) makeSlopeCorrection();   // slope correction function 
 
    fillHitCat();                    // fills official hit level 
    fEventId++; 
 
#if DEBUG_LEVEL>2 
      gDebuger->leaveFunc("HMdcHitF::execute"); 
#endif 
      return 0; 
} 
 
 
 
Int_t HMdcHitF :: calculateCell(Int_t sector, Int_t module, Int_t layer, Float_t coordinate){ 
// This function receives the sector, module, layer and position in layer  
// of the hit, and calculates the cell number that corresponds to the hit 
// in this layer. Needs some geometrical parameters, as coordinate of the first 
// wire in layer, pitch and maximum number of cells in layer.  
// The cell number must be greater or equal to 0, so if the coordinate is not  
// on the plane the function returns a negative number. 
 
 
  Float_t distance;              // distance of the hit to the first cell 
  Float_t diff; 
  Int_t cell;                     // cell number 
  Float_t pitch;                 // pitch of layer 
  Float_t offset;                // position of first wire  
  Int_t maxCells;                 // maximum number of cells in layer 
 
  pitch = geo->getPitch(sector,module,layer); 
  offset = geo->getFirstWirePos(sector,module,layer); 
  maxCells = geo->getMaxNumCells(sector,module,layer); 
 
 // First, check if the coordinate is in plane 
  diff     = coordinate-offset; 
  distance = fabs(diff)+pitch/2.; 
 
  if(layer==0 || layer==4) diff=-diff;      // layers with positive inclination 
                                            // should have diff <0 
 
  cell = Int_t(distance/pitch); 
 
  if(diff < 0 || cell > maxCells)    return -1; 
 
  else return cell; 
 
} 
 
 
 
void HMdcHitF :: makeSlopeCorrection(void){ 
  //this function must be called after a first segment finding and fitting 
  // has been done. Takes each segment in the segment category, makes in the 
  // calhits a correction in slope and makes again the fit. 
  // This is needed since when the cal2 data were converted into calhit, 
  // the data were supposed to be on the layer. 
 
 
#if DEBUG_LEVEL> 2 
  gDebuger->enterFunc("HMdcHitF::makeSlopeCorrection"); 
#endif 
 
Int_t sector = fLoc[0]; 
Int_t module = fLoc[1]; 
 
Int_t nlayers = geo->getNLayers(); 
//Float_t sinn[nlayers]; 
//Float_t cosn[nlayers]; 
Float_t pitch[nlayers]; 
Float_t wiredist,wirepos; 
Float_t centralWire[nlayers]; 
 
for(Int_t layer=0;layer<nlayers;layer++){ 
  //sinn[layer] = geo->getSinn(sector, module, layer); 
  //cosn[layer] = geo->getCosn(sector, module, layer);		 
    pitch[layer] = geo->getPitch(sector, module, layer);     
    centralWire[layer] = geo->getCentralWire(sector, module, layer) -1; 
} 
 
HMdcCal3 *hit; 
HMdcHitAux* seg; 
Float_t pos[nlayers]; 
Float_t newpos, newSlopeY, correction; 
//Float_t originalpos[nlayers]; 
//Defining simple angles. Change later for standard parameters  
Float_t mycosn[6] = {0.76604444,0.93969262,1.,1.,0.93969262,0.76604444}; 
Float_t mysinn[6] = {0.64278761,-0.34202014,0.,0.,0.34202014,-0.64278761}; 
Float_t chi; 
 
/* 
//Studying performance. Output in file slopecorr.txt 
ofstream slopefile("slopecorr.txt",ios::app); 
*/ 
 
HMatrixCatIter* iter=0; 
iter = (HMatrixCatIter *)mdcSegments->MakeIterator(); 
iter->Reset(); 
iter->gotoLocation(fLoc);  
while((seg=(HMdcHitAux*)iter->Next())!=NULL){ 
 
  /* 
  slopefile << "    MODULE:  " << module  << endl;  
  slopefile << "          Original info:        " << endl << "  X: " <<  
    seg->getPointX() << "  Y: " << seg->getPointY() << "  SlopeX: " << 
    seg->getSlopeX() << "  SlopeY: " << seg->getSlopeY() <<  
    endl << "  chi2: " << seg->getChi() << endl << endl; 
  */ 
 
  chi = seg->getChi(); 
 
  for(Int_t i=0; i<nlayers; i++){ 
    hit = seg->getCal(i); 
    if(hit!=NULL){ 
      pos[i] = hit->getPos(); 
 
      /*        
      slopefile << "     pos[" << i << "]"  
		<< pos[i] << "  mycosn[" << i << "]" << mycosn[i]  
		<<  "  mysinn[" << i << "]" << mysinn[i] <<endl; 
      */ 
 
      //getting distance to the wire 
      wirepos = (hit->getCell() - centralWire[i]) * pitch[i]; 
      if(i == 0 || i == 4) wirepos = -wirepos; 
      wiredist = pos[i] - wirepos; 
       
      //Slope corrected (newSlopeY) for layers 0, 1, 4 and 5  
      newSlopeY = seg->getSlopeY() * mycosn[i] - seg->getSlopeX() * mysinn[i]; 
      correction = (wiredist/cos(atan(newSlopeY)))-wiredist; 
 
      /*   
      slopefile << "  newSlopeY: " << newSlopeY <<  
	"     atan(newSlopeY)" << atan(newSlopeY) << 
	"     cos(atan(newSlopeY)" << cos(atan(newSlopeY)) << endl; 
      slopefile << "  wiredist " << wiredist <<"  correction"  
		<< correction << endl; 
      */ 
       
      newpos = pos[i] + correction; 
 
      /*   
      slopefile << "  newpos " << newpos << "  new wire dist "  
		<< newpos - wirepos << endl << endl; 
      */ 
 
      hit->setPos(newpos);   	 
    } 
  } 
  // fit again the segment in the module 
  fit(seg); 
 
 
  //NEW Trying to solve the problem of incorrect cal3 election! 
  //I suppose only one cal3 is wrong and I will change the side one by one 
  //!!It is a time-consuming task!! 
  if(seg->getChi()>chi){ 
    Float_t bestchi = seg->getChi(); 
    HMdcHitAux* seg2 = new HMdcHitAux(); 
    *seg2 = *seg; 
     
    /* 
    ofstream chitestfile("chitest.txt",ios::app); 
    */ 
 
    for(Int_t i=0;i<nlayers;i++){ 
      hit = seg->getCal(i); 
      if(hit!=NULL){ 
	pos[i] = hit->getPos(); 
 
	wirepos = (hit->getCell() - centralWire[i]) * pitch[i]; 
	if(i == 0 || i == 4) wirepos = -wirepos; 
	wiredist = pos[i] - wirepos; 
 
	newpos = wirepos - wiredist; 
 
	/* 
	chitestfile << "pos: " << pos[i] << "  wirepos: " << wirepos  
		    << "  newpos " << newpos << endl; 
	*/ 
 
	hit->setPos(newpos);   
	fit(seg); 
        
	/* 
	chitestfile << "  oldchi: " << chi << "  newchi: " <<  seg->getChi() 
		    << endl; 
	*/ 
 
	if(seg->getChi()>chi){            //when we get no improvement  
	  if(seg->getChi()<bestchi){      //but still better than previous 
	    *seg2 = *seg; 
	    bestchi = seg2->getChi(); 
	    /* 
	    chitestfile << " SEG2 accpt. "; 
	    */ 
	  } 
	  else hit->setPos(pos[i]);       
	  if(i==5) { 
	    *seg = *seg2; 
	    fit(seg); 
	  } 
	}	 
	else {          //we got improvement! 
	  /* 
	  chitestfile << "!!  "; 
	  */ 
	  break; 
	} 
      } 
    } 
    /* 
    chitestfile << endl; 
    chitestfile.close(); 
    */ 
  } 
 
 
  /* 
  slopefile << "New fit info: " << endl <<  
    "  X: " << seg->getPointX() << "   Y: " << seg->getPointY() << endl << 
    "  SlopeX: " << seg->getSlopeX() << "  SlopeY: " << seg->getSlopeY() <<  
    endl << "  chi2: " << seg->getChi() << endl; 
  */ 
} 
  
/* 
slopefile.close(); 
*/ 
#if DEBUG_LEVEL> 2 
 gDebuger->leaveFunc("HMdcHitF::makeSlopeCorrection"); 
#endif 
  
} 
 
 
void HMdcHitF :: fillHitCat(void) { 
 
  // fills category of hits 
 
#if DEBUG_LEVEL > 2 
  gDebuger->enterFunc("HMdcHitF::fillHitCat"); 
#endif 
 
  // loops over HMdcHitAux category and fill hit category 
 
  Int_t nlayers = geo->getNLayers(); 
 
  HMdcCal3 *calhit; 
  HMdcHitAux* seg; 
 
  HMdcHit *hit; 
  HLocation loc; 
  loc.set(3,fLoc[0],fLoc[1],0); 
  Int_t offset = 0; 
 
  HMatrixCatIter* iter=0; 
  iter = (HMatrixCatIter *)mdcSegments->MakeIterator(); 
  iter->Reset(); 
  iter->gotoLocation(fLoc); 
	 
  while((seg=(HMdcHitAux*)iter->Next())!=NULL){ 
 
    loc[2] = offset; 
    hit = (HMdcHit*)fHitCat->getSlot(loc); 
 
    if (hit){ 
      hit = new (hit) HMdcHit; 
      hit->setX( seg->getPointX(), seg->getXError()); 
      hit->setY( seg->getPointY(), seg->getYError()); 
 
      // transformation of xslope, yslope into xdir, ydir 
      Float_t xslope = seg->getSlopeX(); 
      Float_t yslope = seg->getSlopeY(); 
 
      Float_t det = sqrt(xslope*xslope + yslope*yslope + 1); 
      Float_t xdir = xslope/det; 
      Float_t ydir = yslope/det; 
      hit->setXDir(xdir, seg->getErrorSlope1()); 
      hit->setYDir(ydir, seg->getErrorSlope2()); 
 
      // offset????? 
 
      hit->setChi2(seg->getChi()); 
      hit->setFlag(seg->getIsUsed()); 
      // cov 
      hit->setSecMod(fLoc[0], fLoc[1]); 
      Int_t id=0;                          // first or second signal 
      Int_t hitNum = 0; 
 
      for(Int_t i=0; i<nlayers; i++){ 
	calhit = seg->getCal(i); 
	if(calhit!=NULL){ 
	  hitNum = calhit->getHitNumber(); 
	  if (hitNum == 0 || hitNum == 1) id = 1;         //first signal 
	  else if (hitNum == 2 || hitNum == 3) id = 2;    // second signal 
	  else id = 0;                                    // no signal 
	  hit->setSignId(i, calhit->getCell(), id);        
	} 
      } 
    offset++; 
    } 
  } 
 
  delete iter; 
 
#if DEBUG_LEVEL > 2 
  gDebuger->leaveFunc("HMdcHitF::fillHitCat"); 
#endif 
 
} 
 
 
 
void HMdcHitF :: printCalHitCat(ofstream &file){ 
#if DEBUG_LEVEL > 2 
    gDebuger->enterFunc("HMdcHitF::printCalHitCat"); 
#endif 
     
    HMdcCal3* calhit; 
 
    HIterator* iter = NULL; 
    iter = (HMatrixCatIter *)mdcCal->MakeIterator(); 
    iter->Reset(); 
 
    while((calhit=(HMdcCal3 *)iter->Next())!=NULL ){ 
      calhit->print(file); 
    } 
 
#if DEBUG_LEVEL > 2 
    gDebuger->enterFunc("HMdcHitF::printCalHitCat"); 
#endif 
 
} 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
