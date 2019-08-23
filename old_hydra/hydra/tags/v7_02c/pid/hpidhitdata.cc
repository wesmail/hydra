// @(#)$Id: hpidhitdata.cc,v 1.8 2004-04-30 15:08:08 christ Exp $
//*-- Author : Marcin Jaskula 02/03/2003
//  Modified : Marcin Jaskula 14/03/2003
//              tof and mdc variables added
//  Modified : Marcin Jaskula 16/04/2003
//              flags added

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HPidHitData                                                                //
//                                                                            //
// Class for storing all intersting data from hit objects matched in          //
// HPidTrackCand. Dedicated to store data for extensions of HPidParticle.     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "hpidhitdata.h"
#include "hpidtrackcand.h"

#include "hrichhit.h"
#include "hkicktrack.h"
#include "hpidmdcfunc.h"
#include "hshowerhittof.h"
#include "hshowerhittoftrack.h"

#include <TError.h>

// -----------------------------------------------------------------------------

ClassImp(HPidHitData)

// -----------------------------------------------------------------------------

void HPidHitData::reset(void)
{
// Set all memebers to 0

    memset(((char *) this) + sizeof(TObject),
            0, sizeof(*this) - sizeof(TObject));

    clearFlags();
}

// -----------------------------------------------------------------------------

void HPidHitData::fill(HPidTrackCand *pTrack)
{
    if(pTrack == NULL)
    {
        ::Error("HPidHitData::fill", "pTack==NULL");
        return;
    }

    HRichHit *pRichHit;

    if((pRichHit = pTrack->getRichHit()) != NULL)
    {
        nRingPadNr     = pRichHit->getRingPadNr();
        fRingCentroid  = pRichHit->getCentroid();
        fRichTheta     = pRichHit->getTheta();
        fRichPhi       = pRichHit->getPhi();
        nRingPatMat    = pRichHit->getRingPatMat();
        nRingHouTra    = pRichHit->getRingHouTra();
        nRingAmplitude = pRichHit->getRingAmplitude();
        nRingLocalMax4 = pRichHit->getRingLocalMax4();
        //fRingFitVar    = pRichHit->;
        //fRingFitRad    = pRichHit->;
        //fRingFitTheta  = pRichHit->;
        //fRingFitPhi    = pRichHit->;

        setFlagRICH();
    }

    HShowerHitTofTrack* pShowerHitTofTrack;
    HShowerHitTof* pShowerHitTof;
    
    if(pTrack->isSim()){
      if((pShowerHitTofTrack=pTrack->getShowerHitTofTrack())!=NULL)
	{
	  tof=pShowerHitTofTrack->getTof();
	  sum0=pShowerHitTofTrack->getSum(0);	
	  sum1=pShowerHitTofTrack->getSum(1);	
	  sum2=pShowerHitTofTrack->getSum(2);	
	}
      else
	{
	  if(pTrack->getSystem()==0)
	    {
	      cout << pTrack->getMetaId()<<endl;
	      tof=-2.;
	      sum0=-2.;
	      sum1=-2.;
	      sum2=-2.;
	    }
	}
    }
    else
      {
	if((pShowerHitTof=pTrack->getShowerHitTof())!=NULL)
	{
	  tof=pShowerHitTof->getTof();
	  sum0=pShowerHitTof->getSum(0);	
	  sum1=pShowerHitTof->getSum(1);	
	  sum2=pShowerHitTof->getSum(2);	
	}
      else
	{
	  if(pTrack->getSystem()==0)
	    {
	      cout << pTrack->getMetaId()<<endl;
	      tof=-2.;
	      sum0=-2.;
	      sum1=-2.;
	      sum2=-2.;
	    }
	}
      }
    
    HKickTrack *pKickTrack;

    if((pKickTrack = pTrack->getKickTrack()) != NULL)
    {
        fMetaEloss = pKickTrack->getMetaEloss();
        setFlagTOF();
    }

    iMdcClsSize0  = HPidMdcFunc::getMdcClsSize(pTrack,  0);
    iMdcNWires0   = HPidMdcFunc::getMdcNWires(pTrack,   0);
    iMdcLevelCls0 = HPidMdcFunc::getMdcLevelCls(pTrack, 0);
    iMdcClsSize1  = HPidMdcFunc::getMdcClsSize(pTrack,  1);
    iMdcNWires1   = HPidMdcFunc::getMdcNWires(pTrack,   1);
    iMdcLevelCls1 = HPidMdcFunc::getMdcLevelCls(pTrack, 1);
    iMdcChiSquare = HPidMdcFunc::getMdcChiSquare(pTrack  );

    if(pTrack->getMdcSeg() != NULL)
        setFlagMDC();
}

// -----------------------------------------------------------------------------

void HPidHitData::print(void) const
{
    printf("HPidHitData\n");
    printf("nRingPadNr: %d\n", nRingPadNr);
    printf("fRingCentroid: %f\n", fRingCentroid);
    printf("fRichTheta: %f\n", fRichTheta);
    printf("fRichPhi: %f\n", fRichPhi);
    printf("nRingPatMat: %d\n", nRingPatMat);
    printf("nRingHouTra: %d\n", nRingHouTra);
    printf("nRingAmplitude: %d\n", nRingAmplitude);
    printf("nRingLocalMax4: %d\n", nRingLocalMax4);
    printf("fRingFitVar: %f\n", fRingFitVar);
    printf("fRingFitRad: %f\n", fRingFitRad);
    printf("fRingFitTheta: %f\n", fRingFitTheta);
    printf("fRingFitPhi: %f\n", fRingFitPhi);

    printf("fMetaEloss: %f\n", fMetaEloss);

    printf("iMdcClsSize0: %i\n", iMdcClsSize0);
    printf("iMdcNWires0: %i\n", iMdcNWires0);
    printf("iMdcLevelCls0: %i\n", iMdcLevelCls0);
    printf("iMdcClsSize1: %i\n", iMdcClsSize1);
    printf("iMdcNWires1: %i\n", iMdcNWires1);
    printf("iMdcLevelCls1: %i\n", iMdcLevelCls1);
}

// -----------------------------------------------------------------------------

void HPidHitData::Streamer(TBuffer &R__b)
{
// Stream an object of class HPidHitData.

UInt_t R__s, R__c;

    if (R__b.IsReading())
    {
        Version_t R__v = R__b.ReadVersion(&R__s, &R__c);

        TObject::Streamer(R__b);
        R__b >> nRingPadNr;
        R__b >> fRingCentroid;
        R__b >> fRichTheta;
        R__b >> fRichPhi;
        R__b >> nRingPatMat;
        R__b >> nRingHouTra;
        R__b >> nRingAmplitude;
        R__b >> nRingLocalMax4;
        R__b >> fRingFitVar;
        R__b >> fRingFitRad;
        R__b >> fRingFitTheta;
        R__b >> fRingFitPhi;

        if(R__v > 1)
        {
            R__b >> fMetaEloss;

            R__b >> iMdcClsSize0;
            R__b >> iMdcNWires0;
            R__b >> iMdcLevelCls0;
            R__b >> iMdcClsSize1;
            R__b >> iMdcNWires1;
            R__b >> iMdcLevelCls1;
        }

	if(R__v > 2)
	  {
	    R__b >> iMdcChiSquare;
	  }
	if(R__v > 3)
	  {
	    R__b >> tof;
	    R__b >> sum0;
	    R__b >> sum1;
	    R__b >> sum2;
	  }

        R__b.CheckByteCount(R__s, R__c, HPidHitData::IsA());
    }
    else
    {
        R__c = R__b.WriteVersion(HPidHitData::IsA(), kTRUE);
        TObject::Streamer(R__b);

        R__b << nRingPadNr;
        R__b << fRingCentroid;
        R__b << fRichTheta;
        R__b << fRichPhi;
        R__b << nRingPatMat;
        R__b << nRingHouTra;
        R__b << nRingAmplitude;
        R__b << nRingLocalMax4;
        R__b << fRingFitVar;
        R__b << fRingFitRad;
        R__b << fRingFitTheta;
        R__b << fRingFitPhi;

        R__b << fMetaEloss;

        R__b << iMdcClsSize0;
        R__b << iMdcNWires0;
        R__b << iMdcLevelCls0;
        R__b << iMdcClsSize1;
        R__b << iMdcNWires1;
        R__b << iMdcLevelCls1;
	R__b << iMdcChiSquare;

	R__b << tof;
	R__b << sum0;
	R__b << sum1;
	R__b << sum2;

        R__b.SetByteCount(R__c, kTRUE);
    }
}

