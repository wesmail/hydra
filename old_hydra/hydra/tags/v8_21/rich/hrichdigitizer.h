#ifndef HRICHDIGITISER_H
#define HRICHDIGITISER_H

#include "TArrayF.h"
#include "TVector.h"

#include "hlocation.h"
#include "hreconstructor.h"

class TF1;
class TList;
class TRandom3;

class HCategory;
class HGeantRichDirect;
class HGeantRichPhoton;
class HIterator;
class HLocation;
class HParSet;
class HRichCalPar;
class HRichCalSim;
class HRichDigitisationPar;
class HRichGeometryPar;
class HRichPad;
class HRichTrack;


class HRichDigitizer : public HReconstructor {

private:

   // Input / output categories
   HCategory* catRichPhoton;        //! Pointer to the geant rich photon category
   HCategory* catRichDirect;        //! Pointer to the geant rich direct hits category
   HCategory* catTrack;             //! Pointer to the cal data category
   HCategory* catCal;               //! Pointer to the richtrack category        

   // Iterators
   HIterator* iterRichPhoton;       //! Iterator over the geant rich photon category
   HIterator* iterRichDirect;       //! Iterator over the geant rich direct hits category
   HIterator* iterRichTrack;        //! Iterator over the carRichTrack category
   HIterator* iterRichCal;          //!

   // Parameters
   HRichDigitisationPar* pDigitisationPar;     //! Pointer to digitisation parameters
   HRichGeometryPar*     pGeometryPar;         //! Pointer to geometry parameters
   HRichCalPar*          pCalPar;              //! Pointer to calibration parameters

   TF1*       ga;                   //! Gauss function

   TList      fChargeOnWireList;    //! holds charge on a wire
   TList      fTrackNumberList;     //! holds track number and flag for a hit
   HLocation  loc;


   static const Float_t noiseCharge[1000];//!


   Bool_t isActiveNoise;            //! flag to switch on/off the electronic noise
   Bool_t isOEM;                    //! flag to select oem analysis.

   Int_t countFBphot;               //! counter for feedback photons
   Int_t countNoisePad;             //! number of pads with a noise cont. above threshold.


   // Geometry parameters
   Int_t   fWiresNr;                //!
   Float_t fWiresDist;              //!
   Float_t distWirePads;            //! distance between wires and pad plane. in cm
   Float_t fYShift;                 //! shift on the y pad plane coordinate necessary to
                                    //! get the correct pad position and the correct theta.

   // Digitization parameters
   Int_t   binsQE;                  //!
   Float_t fChargePerChannel;       //!
   Float_t fChargeScaling;          //!
   Float_t fElectronsNr;            //!
   Float_t fFactor1;                //!
   Float_t fFactor1Sig;             //!
   Float_t fFactor2;                //!
   Float_t fFactor2Sig;             //!
   Float_t fIncreaseNoise;          //!
   Float_t fParam1;                 //!
   Float_t fParam2;                 //!
   Float_t fQupper;                 //!
   Float_t fSigmaValue;             //!
   Float_t fSlopeCorrection;        //!
   TArrayF fExpSlope;               //!
   TArrayF correction[6];           //!
   TArrayF photeffic;               //!
   TArrayF photlength;              //!


   Float_t noiseProb;               //!
   Float_t fDigitPadMatrix[9];      //!



public:
   HRichDigitizer();
   HRichDigitizer(const Text_t* name,
		  const Text_t* title,
		  Bool_t  kNoise = kFALSE,
		  Float_t slope  = 1.0,
		  Bool_t  oem    = kFALSE );
   ~HRichDigitizer();

   Bool_t init();
   Bool_t reinit();
   Int_t  execute();
   Bool_t finalize();


private:
   Bool_t calcQE(const Float_t photlen,
		 const Int_t sec);
   Bool_t calcFeedBack(const Int_t sec,
		       const Float_t xhit,
		       const Float_t yhit,
		             Float_t &ene,
		             Float_t &xhittFB,
		             Float_t &yhittFB,
		       const Float_t charge);

   Float_t GaussFun(const Float_t mean,
		    const Float_t sigma);
   Float_t calcIndCharge(const Float_t yCharge,
			 const Float_t q4,
			 const Int_t iPdaIndex,
			 const Int_t iWireNr,
			 const Float_t param11,
			 const Float_t param21   );
   Float_t qX(const Float_t pos);
   Float_t q4Calc(const Float_t charge,
		  const Float_t pos,
		  const Float_t par1,
		  const Float_t par2   );
   Float_t calcChargeOnWire(const Int_t sector,
			    const Float_t xhit,
			    const Float_t yhit,
			    const Float_t nTrack,
			    const Float_t nFlag,
			          Float_t ene     );
   Float_t calcNoiseOnPad(const Float_t fSigmaPad,
			  const Float_t fFloatMean);

   Int_t getWireNr(const Float_t xhit);
   Int_t checkPad(HRichCalSim *calSim);

   void setDefaults();
   void makeNoiseOnPads();
   void addNoiseToCharge(HRichCalSim* calSim);
   void digitisePads();
   void digitiseCherenkovHits(HGeantRichPhoton *pCerHit,
			      const Int_t count);
   void digitiseDirectHits(HGeantRichDirect *pDirHit);
   void processPhoton(const Float_t ene,
		      const Float_t xPos,
		      const Float_t yPos,
		      const Int_t track,
		      const Int_t sector );
   void updateCharge(const Int_t sector,
		     HRichPad* pPad,
		     const Float_t charge,
		     TVector * rTrack,
		     const Float_t ene);
    
   void updateTrack(HRichCalSim *pCalSim,
		    HLocation & loc,
		    TVector * rTrack);

   HRichPad* translateCorners(HRichPad *pPad,
			      const Float_t dx,
			      const Float_t dy  );


 public:
   ClassDef(HRichDigitizer,0) // Rich digitizer

};

#endif // HRICHDIGITISER_H 







