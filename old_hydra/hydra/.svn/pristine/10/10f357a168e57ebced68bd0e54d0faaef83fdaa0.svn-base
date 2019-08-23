#ifndef HTRIGGERHISTFILLER_H
#define HTRIGGERHISTFILLER_H
#include "TNamed.h"

class HParamList;
class HMonHistAddon;
class HMonHist;
class HMonHist2;
class HTrendHist;
class HHistArray;
class HHistArray2;
class HTrendArray;
class HMatchURich;
class HMatchUTof;
class HMatchUShower;
class HTriggerParShowerMap;
class HIterator;
class TList;
class TArrayI;
class TString;

/** Class to fill Trigger histograms.
 * This class is responible of booking, creating
 * and filling of the Trigger histograms. The fill
 * functions are called by HTriggerGo4Mon or TGo4Hades.
 * All actions on the histograms have to go through
 * HTriggerHistBooker and HTriggerHistFiller.
 */

class HTriggerHistFiller : public TNamed {

protected:
    // Histpointers

    static HMonHist*    htof;                        //!< hist pointer
    static HHistArray*  htriggermultdistrib;         //!< hist pointer
    static HHistArray*  htriggerrichringmultdistrib; //!< hist pointer
    static HHistArray*  htriggertofhitmultdistrib;   //!< hist pointer
    static HHistArray*  htriggershowerhitmultdistrib;//!< hist pointer
    static HHistArray*  htriggermatchuhitmultdistrib;//!< hist pointer
    static HMonHist*    htofnumbers;                 //!< hist pointer
    static HMonHist*    hrichnumbers;                //!< hist pointer
    static HTrendArray* hiputrendRich;               //!< hist pointer
    static HTrendArray* hiputrendTof;                //!< hist pointer
    static HHistArray2* hrichCorrPhi;              //!< hist pointer
    static HHistArray2* hrichCorrTheta;            //!< hist pointer
    static HHistArray2* htofCorrPhi;                 //!< hist pointer
    static HHistArray2* htofCorrTheta;               //!< hist pointer
    static HHistArray2* hshowerCorrPhi;              //!< hist pointer
    static HHistArray2* hshowerCorrTheta;            //!< hist pointer
    static HHistArray2* hmuCorrPhi;              //!< hist pointer
    static HHistArray2* hmuCorrTheta;            //!< hist pointer
    static HTrendArray* hiputrendShower;             //!< hist pointer
    static HTrendArray* hiputrendMatchU;             //!< hist pointer
    static HTrendArray* htriggerTrendTime;           //!< hist pointer
    static HTrendArray* hiputrendRichTime;           //!< hist pointer
    static HTrendArray* hiputrendTofTime;            //!< hist pointer
    static HTrendArray* hiputrendShowerTime;         //!< hist pointer
    static HTrendArray* hiputrendMatchUTime;         //!< hist pointer
    static HMonHist2*   hipubutterfly;         //!< hist pointer
    static HMonHist*    hmulepmom;         //!< hist pointer
    static HMonHist2*   hmuCorrAll;         //!< hist pointer

    static HMonHistAddon*  htofA;                        //!< hist addon pointer
    static HMonHistAddon*  htriggermultdistribA;         //!< hist addon pointer
    static HMonHistAddon*  htriggerrichringmultdistribA; //!< hist addon pointer
    static HMonHistAddon*  htriggertofhitmultdistribA;   //!< hist addon pointer
    static HMonHistAddon*  htriggershowerhitmultdistribA;//!< hist addon pointer
    static HMonHistAddon*  htriggermatchuhitmultdistribA;//!< hist addon pointer
    static HMonHistAddon*  htofnumbersA;                 //!< hist addon pointer
    static HMonHistAddon*  hrichnumbersA;                //!< hist addon pointer
    static HMonHistAddon*  hiputrendRichA;               //!< hist addon pointer
    static HMonHistAddon*  hiputrendTofA;                //!< hist addon pointer
    static HMonHistAddon*  hrichCorrPhiA;              //!< hist addon pointer
    static HMonHistAddon*  hrichCorrThetaA;            //!< hist addon pointer
    static HMonHistAddon*  htofCorrPhiA;                 //!< hist addon pointer
    static HMonHistAddon*  htofCorrThetaA;               //!< hist addon pointer
    static HMonHistAddon*  hshowerCorrPhiA;              //!< hist addon pointer
    static HMonHistAddon*  hshowerCorrThetaA;            //!< hist addon pointer
    static HMonHistAddon*  hmuCorrPhiA;              //!< hist addon pointer
    static HMonHistAddon*  hmuCorrThetaA;            //!< hist addon pointer
    static HMonHistAddon*  hiputrendShowerA;             //!< hist addon pointer
    static HMonHistAddon*  hiputrendMatchUA;             //!< hist addon pointer
    static HMonHistAddon*  htriggerTrendTimeA;           //!< hist addon pointer
    static HMonHistAddon*  hiputrendRichTimeA;           //!< hist addon pointer
    static HMonHistAddon*  hiputrendTofTimeA;            //!< hist addon pointer
    static HMonHistAddon*  hiputrendShowerTimeA;         //!< hist addon pointer
    static HMonHistAddon*  hiputrendMatchUTimeA;         //!< hist addon pointer
    static HMonHistAddon*  hipubutterflyA;         //!< hist pointer
    static HMonHistAddon*  hmulepmomA;         //!< hist pointer
    static HMonHistAddon*  hmuCorrAllA;         //!< hist pointer
    // Colors
    static TArrayI colorsModule;   ///< colors for hists to be compared in one sector
    static TArrayI colorsCuts;     ///< colors for hists to be compared for cuts
    static TArrayI colorsIPUtrend; ///< colors for hists to be compared for cuts

    Int_t iputrendhit  [4][7];
    Int_t iputrendevent[4][7];

    Int_t tofLeptons[7];

    Int_t richCounter [3][3]; ///< 0row: counter to 100, 1k, 10k. 1row: counter rich events >=1 hit corresponding to row0. 2row: counter rich events >=2 hits corresponding to row0.
    Float_t richResult[2][3];

    UInt_t timeEventCounter;
    UInt_t timeOld         ;
    Int_t  timeOffset      ;

    UInt_t ipuTimeEventCounter[4][7];
    UInt_t ipuTimeHitCounter  [4][7];
    UInt_t ipuTimeOld         [4];

public:
    HTriggerHistFiller(const Char_t* name   ="TriggerHistFiller",
		       const Char_t* title  ="parameters for Trigger hists");
    ~HTriggerHistFiller();
    /// put parameter to list of HParCond
    void   putTriggerParams(HParamList*,TList*);
    /// get parameter from list of HParCond
    Bool_t getTriggerParams(HParamList*,TList*,TList*);
    /// fill rich histograms
    void   fillRich(HIterator*);
    void   fillRichCorrelation(HIterator*, HIterator*);
    /// fill tof histograms
    void   fillTof(HIterator*);
    void   fillTofCorrelation(HIterator*, HIterator*);
    /// fill shower histograms
    void   fillShower(HIterator*);
    void   fillShowerCorrelation(HIterator*, HIterator*, HTriggerParShowerMap*);
    /// fill matching unit histograms
    void   fillMatchU(HIterator*);
    void   fillMatchUCorrelation(HIterator* iterHMatchURich,
         HIterator *iterHMatchUTof,HIterator *iterHMatchUShower,
         HIterator *iterHMULeptons,HIterator *iterHMUEMULeptons);
    /// fill trend histograms
    void   fillTrend();
    /// fill rich histograms
    void   fillRichTime(HIterator*, UInt_t currentTime);
    /// fill tof histograms
    void   fillTofTime(HIterator*, UInt_t currentTime);
    /// fill shower histograms
    void   fillShowerTime(HIterator*, UInt_t currentTime);
    /// fill matching unit histograms
    void   fillMatchUTime(HIterator*, UInt_t currentTime);
    /// fill trend histograms
    void   fillTrendTime(UInt_t ipu, UInt_t offset);
    /// fill time dependent stuff
    void   fillTime(UInt_t currentTime);
    /// reset some counters
		void   resetCounter()
			{
				for(Int_t i=0;i<4;i++){
					for(Int_t j=0;j<7;j++){
						iputrendhit  [i][j]=0;
						iputrendevent[i][j]=0;
					}
				}
				for(Int_t i=0;i<7;i++)
					{
						tofLeptons[i]=0;
					}
			}
		// reset some counters
		void   resetTimeCounter()
			{
				timeEventCounter=0;
				timeOld       =0;
				timeOffset    =0;	    
			}
		// reset ipu time counters
		void   resetAllIpuTimeCounter()
			{
				for(Int_t i=0;i<4;i++)
					{
						ipuTimeOld[i]=0;
						for(Int_t j=0;j<7;j++)
							{
								ipuTimeEventCounter[i][j]=0;
								ipuTimeHitCounter  [i][j]=0;
							}
					}
			}
		// reset counter for specific ipu (0:rich, 1:tof, 2:shower, 3:matching unit)
		void   resetSingleIpuTimeCounter(UInt_t ipu)
			{
				ipuTimeOld[ipu]=0;
				for(Int_t j=0;j<7;j++)
					{
						ipuTimeEventCounter[ipu][j]=0;
						ipuTimeHitCounter  [ipu][j]=0;
					}
			}
				
				
		ClassDef(HTriggerHistFiller,1) // Container for the Trigger Hists
};

#endif  /*!HTRIGGERHISTFILLER_H*/

