#include "hparhadasciifileio.h"
#include "hrichcorrelatorpar.h"
#include <fstream.h>
#include <iostream.h>
#include <string.h>


ClassImp(HRichCorrelatorPar)

  //---------------------------------------------------------------------------- 
HRichCorrelatorPar::HRichCorrelatorPar(){
  SetName("RichCorrelatorParameters");
  clear();
}
//============================================================================

//---------------------------------------------------------------------------- 
HRichCorrelatorPar::~HRichCorrelatorPar(){
clear();
}
//============================================================================

//---------------------------------------------------------------------------- 
void HRichCorrelatorPar::clear() {

  kShowerCond= kFALSE;// =1 if we ask for a shower in the PreShower det.
  kIsNoCutOnTOFtof= kFALSE;
  kIsNoCutOnTOFINOtof= kFALSE;
  kNoSectorSelected= kFALSE;
  kSwitchOffRich= kFALSE;
  kRingQualityCut= kFALSE;
  kTofinoMultCut= kFALSE;
  kMagnet= kFALSE;
 

  fRichMdcThetaCut= 0.;
  fRichMdcThetaMinCut= 0.;
  fRichMdcThetaMaxCut= 0.;
  fRichTofThetaCut= 0.;
  fRichTofThetaMinCut= 0.;
  fRichTofThetaMaxCut= 0.;//!
  fRichShowerThetaCut= 0.;
  fRichShowerThetaMinCut= 0.;//!
  fRichShowerThetaMaxCut= 0.;//!
  fMdcTofThetaCut= 0.;
  fMdcTofThetaMinCut= 0.;
  fMdcTofThetaMaxCut= 0.;
  fMdcShowerThetaCut= 0.;
  fMdcShowerThetaMinCut= 0.;
  fMdcShowerThetaMaxCut= 0.;
  fTofTofMinCut= 0.;
  fTofinoTofMinCut= 0.;
  fTofTofMaxCut= 0.;
  fTofinoTofMaxCut= 0.;
  fShowerAmplification= 0.;
  fRichMdcPhiCut= 0.;
  fRichTofPhiCut= 0.;
  fRichShowerPhiCut= 0.;
  fMdcTofPhiCut= 0.;
  fMdcShowerPhiCut= 0.;

  nTofinoMultCut= 0;
  fMdcThetaCorrection= 0.;

  nPatMatCut = 0;
  nHouTraCut = 0;
  nPadNrCut = 0;
  nRingAmplCut = 0;
  nLocMax4Cut = 0;
}
//============================================================================

//---------------------------------------------------------------------------- 
Bool_t HRichCorrelatorPar::initAscii(HParHadAsciiFileIo* pHadAsciiFile) {

  using namespace HadAsciiAuxiliaryFunctions;
  if (!pHadAsciiFile) return kFALSE;
  
  Bool_t status = kTRUE;
  
  try {
 HAsciiKey &mydata = *pHadAsciiFile->GetKeyAscii();

 mydata.SetActiveSection("Correlator Switches");
 kShowerCond = mydata.ReadInt("Shower Condition");
 kIsNoCutOnTOFtof = mydata.ReadInt("No TofTOF cut");
 kIsNoCutOnTOFINOtof = mydata.ReadInt("No TofinoTOF cut");
 kNoSectorSelected = mydata.ReadInt("All Sectors correlated");
 nSelectedSector = mydata.ReadInt("Selected Sector");
 kSwitchOffRich = mydata.ReadInt("No Rich correlated");
 kRingQualityCut = mydata.ReadInt("Ring Quality cuts");
 kTofinoMultCut = mydata.ReadInt("Tofino Multiplicity cuts");
 kMagnet = mydata.ReadInt("Magnet On");

 mydata.SetActiveSection("Theta Cuts between Detectors (asymmetric)"); 
 fRichMdcThetaCut = mydata.ReadFloat("DTheta RICH MDC");
 fRichMdcThetaMinCut = mydata.ReadFloat("DThetaMin RICH MDC");
 fRichMdcThetaMaxCut = mydata.ReadFloat("DThetaMax RICH MDC");
 fRichTofThetaCut = mydata.ReadFloat("DTheta RICH TOF");
 fRichTofThetaMinCut = mydata.ReadFloat("DThetaMin RICH TOF"); 
 fRichTofThetaMaxCut = mydata.ReadFloat("DThetaMax RICH TOF");
 fRichShowerThetaCut = mydata.ReadFloat("DTheta RICH SHOWER");
 fRichShowerThetaMinCut = mydata.ReadFloat("DThetaMin RICH SHOWER");
 fRichShowerThetaMaxCut  = mydata.ReadFloat("DThetaMax RICH SHOWER");
 fMdcTofThetaCut = mydata.ReadFloat("DTheta MDC TOF");
 fMdcTofThetaMinCut = mydata.ReadFloat("DThetaMin MDC TOF");
 fMdcTofThetaMaxCut = mydata.ReadFloat("DThetaMax MDC TOF");
 fMdcShowerThetaCut = mydata.ReadFloat("DTheta MDC SHOWER"); 
 fMdcShowerThetaMinCut = mydata.ReadFloat("DThetaMin MDC SHOWER");
 fMdcShowerThetaMaxCut = mydata.ReadFloat("DThetaMax MDC SHOWER");

 mydata.SetActiveSection("Phi Cuts between Detectors");
 fRichMdcPhiCut = mydata.ReadFloat("DPhi RICH MDC");
 fRichTofPhiCut = mydata.ReadFloat("DPhi RICH TOF");
 fRichShowerPhiCut = mydata.ReadFloat("DPhi RICH SHOWER");
 fMdcTofPhiCut = mydata.ReadFloat("DPhi MDC TOF");
 fMdcShowerPhiCut = mydata.ReadFloat("DPhi MDC SHOWER");
 

 mydata.SetActiveSection("TOF Cuts");
 fTofTofMinCut = mydata.ReadFloat("Mininum T.O.F. in Tof");
 fTofinoTofMinCut = mydata.ReadFloat("Minimum T.O.F. in Tofino");
 fTofTofMaxCut = mydata.ReadFloat("Maximum T.O.F. in Tof");
 fTofinoTofMaxCut = mydata.ReadFloat("Maximum T.O.F. in Tofino");

 mydata.SetActiveSection("Miscellanea");
 fShowerAmplification = mydata.ReadFloat("Shower Amplification Factor");
 nTofinoMultCut = mydata.ReadInt("Tofino Multiplicity Cut");
 fMdcThetaCorrection = mydata.ReadFloat("MDC Theta correction factor");

 mydata.SetActiveSection("Ring Quality Cuts");
 nPatMatCut = mydata.ReadInt("Minimal Pattern Matrix Quality");
 nHouTraCut = mydata.ReadInt("Minimal Hough Trasformation Quality");
 nPadNrCut = mydata.ReadInt("Minimal PadNr per Ring");
 nRingAmplCut = mydata.ReadInt("Minimal Ring Charge Amplitude");
 nLocMax4Cut = mydata.ReadInt("Minimal Ring Local Maxima");
 
  }
// eof try block

 catch (Bool_t ret) {
  status = ret;
 }
 catch (...) {
  status = kFALSE;
  ErrorMsg(2,"HRichCorrelatorPar::initAscii",1,"Unidentified exception catched.");
 }

 if (status) 
  ErrorMsg(0,"HRichCorrelatorPar::initAscii",
   1,"Container \'RichCorrelatorParameters\' has been read from ASCII file.");

 return status;
}
//============================================================================

//---------------------------------------------------------------------------- 
void HRichCorrelatorPar::printParam(){
    cout<<"***************************************************************************"<<endl;
    cout<<"*** HRichCorrelatorPar ****************************************************"<<endl;
    cout<<"***************************************************************************"<<endl;
    cout<<"*** Shower Condition = "<<kShowerCond<<endl;
    cout<<"*** No TofTOF cut = "<<kIsNoCutOnTOFtof<<endl;
    cout<<"*** No TofinoTOF cut = "<<kIsNoCutOnTOFINOtof<<endl;
    cout<<"*** All Sectors correlated = "<<kNoSectorSelected<<endl;
    cout<<"*** Selected Sector = "<<nSelectedSector<<endl;
    cout<<"*** No Rich correlated = "<<kSwitchOffRich<<endl;
    cout<<"*** Ring Quality cuts = "<<kRingQualityCut<<endl;
    cout<<"*** Tofino Multiplicity cuts = "<<kTofinoMultCut<<endl;
    cout<<"*** Magnet On = "<<kMagnet<<endl;
    cout<<"***************************************************************************"<<endl;
    cout<<"*** DTheta RICH MDC = "<<fRichMdcThetaCut<<endl;
    cout<<"*** DThetaMin RICH MDC = "<<fRichMdcThetaMinCut<<endl;
    cout<<"*** DThetaMax RICH MDC = "<<fRichMdcThetaMaxCut<<endl;
    cout<<"*** DTheta RICH TOF = "<<fRichTofThetaCut<<endl;
    cout<<"*** DThetaMin RICH TOF = "<<fRichTofThetaMinCut<<endl;
    cout<<"*** DThetaMax RICH TOF = "<<fRichTofThetaMaxCut<<endl;
    cout<<"*** DTheta RICH SHOWER = "<<fRichShowerThetaCut<<endl;
    cout<<"*** DThetaMin RICH SHOWER = "<<fRichShowerThetaMinCut<<endl;
    cout<<"*** DThetaMax RICH SHOWER = "<<fRichShowerThetaMaxCut<<endl;
    cout<<"*** DTheta MDC TOF = "<<fMdcTofThetaCut<<endl;
    cout<<"*** DThetaMin MDC TOF = "<<fMdcTofThetaMinCut<<endl;
    cout<<"*** DThetaMax MDC TOF = "<<fMdcTofThetaMaxCut<<endl;
    cout<<"*** DTheta MDC SHOWER = "<<fMdcShowerThetaCut<<endl;
    cout<<"*** DThetaMin MDC SHOWER = "<<fMdcShowerThetaMinCut<<endl;
    cout<<"*** DThetaMax MDC SHOWER = "<<fMdcShowerThetaMaxCut<<endl;
    cout<<"***************************************************************************"<<endl;
    cout<<"*** DPhi RICH MDC = "<<fRichMdcPhiCut<<endl;
    cout<<"*** DPhi RICH TOF = "<<fRichTofPhiCut<<endl;
    cout<<"*** DPhi RICH SHOWER = "<<fRichShowerPhiCut<<endl;
    cout<<"*** DPhi MDC TOF = "<<fMdcTofPhiCut<<endl;
    cout<<"*** DPhi MDC SHOWER = "<<fMdcShowerPhiCut<<endl;
    cout<<"***************************************************************************"<<endl;
    cout<<"*** Mininum T.O.F. in Tof = "<<fTofTofMinCut<<endl;
    cout<<"*** Minimum T.O.F. in Tofino = "<<fTofinoTofMinCut<<endl;
    cout<<"*** Maximum T.O.F. in Tof = "<<fTofTofMaxCut<<endl;
    cout<<"*** Maximum T.O.F. in Tofino = "<<fTofinoTofMaxCut<<endl;
    cout<<"***************************************************************************"<<endl;
    cout<<"*** Shower Amplification Factor = "<<fShowerAmplification<<endl;
    cout<<"*** Tofino Multiplicity Cut = "<<nTofinoMultCut<<endl;
    cout<<"*** MDC Theta correction factor = "<<fMdcThetaCorrection<<endl;
    cout<<"***************************************************************************"<<endl;
    cout<<"*** Minimal Pattern Matrix Quality = "<<nPatMatCut<<endl;
    cout<<"*** Minimal Hough Trasformation Quality = "<<nHouTraCut<<endl;
    cout<<"*** Minimal Pad Nr per Ring = "<<nPadNrCut<<endl;
    cout<<"*** Minimal Ring Charge Amplitude = "<<nRingAmplCut<<endl;
    cout<<"*** Minimal Ring Local Maxima = "<<nLocMax4Cut<<endl;
    cout<<"***************************************************************************"<<endl;
}
//============================================================================

//---------------------------------------------------------------------------- 
Bool_t HRichCorrelatorPar::writeAscii(HParHadAsciiFileIo* pHadAsciiFile) {
using namespace HadAsciiAuxiliaryFunctions;
 if (!pHadAsciiFile) return kFALSE;

 Bool_t status = kTRUE;

 try {
   HAsciiKey &mydata = *pHadAsciiFile->GetKeyAscii();
   mydata.WriteSection("Correlator Switches");
   mydata.WriteLine("\n");
   mydata.WriteInt("Shower Condition",kShowerCond);
   mydata.WriteInt("No TofTOF cut",kIsNoCutOnTOFtof);
   mydata.WriteInt("No TofinoTOF cut",kIsNoCutOnTOFINOtof);
   mydata.WriteInt("All Sectors correlated",kNoSectorSelected);
   mydata.WriteInt("Selected Sector",nSelectedSector);
   mydata.WriteInt("No Rich correlated",kSwitchOffRich);
   mydata.WriteInt("Ring Quality cuts",kRingQualityCut);
   mydata.WriteInt("Tofino Multiplicity cuts",kTofinoMultCut);
   mydata.WriteInt("Magnet On",kMagnet);
   mydata.WriteLine("\n");
   mydata.WriteLine("\n");
   mydata.WriteSection("Theta Cuts between Detectors (asymmetric)"); 
   mydata.WriteLine("\n");
   mydata.WriteFloat("DTheta RICH MDC",fRichMdcThetaCut);
   mydata.WriteFloat("DThetaMin RICH MDC",fRichMdcThetaMinCut);
   mydata.WriteFloat("DThetaMax RICH MDC",fRichMdcThetaMaxCut);
   mydata.WriteFloat("DTheta RICH TOF",fRichTofThetaCut);
   mydata.WriteFloat("DThetaMin RICH TOF",fRichTofThetaMinCut);
   mydata.WriteFloat("DThetaMax RICH TOF",fRichTofThetaMaxCut);
   mydata.WriteFloat("DTheta RICH SHOWER",fRichShowerThetaCut);
   mydata.WriteFloat("DThetaMin RICH SHOWER",fRichShowerThetaMinCut);
   mydata.WriteFloat("DThetaMax RICH SHOWER",fRichShowerThetaMaxCut);
   mydata.WriteFloat("DTheta MDC TOF",fMdcTofThetaCut);
   mydata.WriteFloat("DThetaMin MDC TOF",fMdcTofThetaMinCut);
   mydata.WriteFloat("DThetaMax MDC TOF",fMdcTofThetaMaxCut);
   mydata.WriteFloat("DTheta MDC SHOWER",fMdcShowerThetaCut);
   mydata.WriteFloat("DThetaMin MDC SHOWER",fMdcShowerThetaMinCut);
   mydata.WriteFloat("DThetaMax MDC SHOWER",fMdcShowerThetaMaxCut);
   mydata.WriteLine("\n");
   mydata.WriteLine("\n");
   mydata.WriteSection("Phi Cuts between Detectors");
   mydata.WriteLine("\n");
   mydata.WriteFloat("DPhi RICH MDC",fRichMdcPhiCut);
   mydata.WriteFloat("DPhi RICH TOF",fRichTofPhiCut);
   mydata.WriteFloat("DPhi RICH SHOWER",fRichShowerPhiCut);
   mydata.WriteFloat("DPhi MDC TOF",fMdcTofPhiCut);
   mydata.WriteFloat("DPhi MDC SHOWER",fMdcShowerPhiCut);
   mydata.WriteLine("\n");
   mydata.WriteLine("\n");
   mydata.WriteSection("TOF Cuts");
   mydata.WriteLine("\n");
   mydata.WriteFloat("Mininum T.O.F. in Tof",fTofTofMinCut);
   mydata.WriteFloat("Minimum T.O.F. in Tofino",fTofinoTofMinCut);
   mydata.WriteFloat("Maximum T.O.F. in Tof",fTofTofMaxCut);
   mydata.WriteFloat("Maximum T.O.F. in Tofino",fTofinoTofMaxCut);
   mydata.WriteLine("\n");
   mydata.WriteLine("\n");
   mydata.WriteSection("Miscellanea");
   mydata.WriteLine("\n");
   mydata.WriteFloat("Shower Amplification Factor",fShowerAmplification);
   mydata.WriteInt("Tofino Multiplicity Cut",nTofinoMultCut);
   mydata.WriteFloat("MDC Theta correction factor",fMdcThetaCorrection);
   mydata.WriteLine("\n");
   mydata.WriteLine("\n");
   mydata.WriteSection("Ring Quality Cuts");
   mydata.WriteInt("Minimal Pattern Matrix Quality",nPatMatCut);
   mydata.WriteInt("Minimal Hough Trasformation Quality",nHouTraCut);
   mydata.WriteInt("Minimal PadNr per Ring",nPadNrCut);
   mydata.WriteInt("Minimal Ring Charge Amplitude",nRingAmplCut);
   mydata.WriteInt("Minimal Ring Local Maxima",nLocMax4Cut);
 }
 catch (Bool_t ret) {
   status = ret;
 }
 catch (...) {
   status = kFALSE;
   ErrorMsg(1,"HRichCorrelatorPar::writeAscii",1,"Unidentified exception catched.");
 }
 
 if (status)         
   ErrorMsg(0,"HRichCorrelatorPar::writeAscii",
	    1,"Container \'RichCorrelatorParameters\' has been written to ASCII file.");
 
 return status;
}
//============================================================================

//---------------------------------------------------------------------------- 

//============================================================================

//---------------------------------------------------------------------------- 
