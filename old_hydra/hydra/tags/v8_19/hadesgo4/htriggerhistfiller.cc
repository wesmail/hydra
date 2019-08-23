//*-- AUTHOR : J. Markert

//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////////////////
// HTriggerHistFiller
//
// Container class for the trigger hists
////////////////////////////////////////////////////////////////////////////

#include "htriggerhistfiller.h"
#include "htriggerhistfillerutil.h"
#include "hparamlist.h"
#include "hmonhist.h"
#include "htrendhist.h"
#include "hhistarray.h"

#include "triggerinfodef.h"
#include "hmatchurich.h"
#include "hmatchutof.h"
#include "hmatchushower.h"
#include "hmuleptons.h"
#include "hmuEmuleptons.h"

#include "htofhit.h"
#include "hshowerhitIPU.h"
#include "hrichhitIPU.h"
#include "hiterator.h"
#include "htriggerparshowermap.h"

#include "hades.h"
#include "hevent.h"
#include "hcategory.h"
#include "TList.h"
#include "TArrayI.h"


#include <cmath>


// Color definitions
TArrayI HTriggerHistFiller::colorsModule;
TArrayI HTriggerHistFiller::colorsCuts;
TArrayI HTriggerHistFiller::colorsIPUtrend;

// Hist Pointers
HMonHist*        HTriggerHistFiller::htof=0;
HHistArray*      HTriggerHistFiller::htriggermultdistrib=0;
HHistArray*      HTriggerHistFiller::htriggerrichringmultdistrib=0;
HHistArray*      HTriggerHistFiller::htriggertofhitmultdistrib=0;
HHistArray*      HTriggerHistFiller::htriggershowerhitmultdistrib=0;
HHistArray*      HTriggerHistFiller::htriggermatchuhitmultdistrib=0;
HHistArray2*     HTriggerHistFiller::hrichCorrPhi=0;
HHistArray2*     HTriggerHistFiller::hrichCorrTheta=0;
HHistArray2*     HTriggerHistFiller::htofCorrPhi=0;
HHistArray2*     HTriggerHistFiller::htofCorrTheta=0;
HHistArray2*     HTriggerHistFiller::hshowerCorrPhi=0;
HHistArray2*     HTriggerHistFiller::hshowerCorrTheta=0;
HHistArray2*     HTriggerHistFiller::hmuCorrPhi=0;
HHistArray2*     HTriggerHistFiller::hmuCorrTheta=0;
HMonHist*        HTriggerHistFiller::htofnumbers=0;
HMonHist*        HTriggerHistFiller::hrichnumbers=0;
HTrendArray*     HTriggerHistFiller::hiputrendRich=0;
HTrendArray*     HTriggerHistFiller::hiputrendTof=0;
HTrendArray*     HTriggerHistFiller::hiputrendShower=0;
HTrendArray*     HTriggerHistFiller::hiputrendMatchU=0;
HTrendArray*     HTriggerHistFiller::htriggerTrendTime=0;
HTrendArray*     HTriggerHistFiller::hiputrendRichTime=0;
HTrendArray*     HTriggerHistFiller::hiputrendTofTime=0;
HTrendArray*     HTriggerHistFiller::hiputrendShowerTime=0;
HTrendArray*     HTriggerHistFiller::hiputrendMatchUTime=0;
HMonHist2*       HTriggerHistFiller::hipubutterfly=0;
HMonHist*        HTriggerHistFiller::hmulepmom=0;
HMonHist2*       HTriggerHistFiller::hmuCorrAll=0;

// Hist Addon Pointers
HMonHistAddon* HTriggerHistFiller::htofA=0;
HMonHistAddon* HTriggerHistFiller::htriggermultdistribA=0;
HMonHistAddon* HTriggerHistFiller::htriggerrichringmultdistribA=0;
HMonHistAddon* HTriggerHistFiller::htriggertofhitmultdistribA=0;
HMonHistAddon* HTriggerHistFiller::htriggershowerhitmultdistribA=0;
HMonHistAddon* HTriggerHistFiller::htriggermatchuhitmultdistribA=0;
HMonHistAddon* HTriggerHistFiller::hrichCorrPhiA=0;
HMonHistAddon* HTriggerHistFiller::hrichCorrThetaA=0;
HMonHistAddon* HTriggerHistFiller::htofCorrPhiA=0;
HMonHistAddon* HTriggerHistFiller::htofCorrThetaA=0;
HMonHistAddon* HTriggerHistFiller::hshowerCorrPhiA=0;
HMonHistAddon* HTriggerHistFiller::hshowerCorrThetaA=0;
HMonHistAddon* HTriggerHistFiller::hmuCorrPhiA=0;
HMonHistAddon* HTriggerHistFiller::hmuCorrThetaA=0;
HMonHistAddon* HTriggerHistFiller::htofnumbersA=0;
HMonHistAddon* HTriggerHistFiller::hrichnumbersA=0;
HMonHistAddon* HTriggerHistFiller::hiputrendRichA=0;
HMonHistAddon* HTriggerHistFiller::hiputrendTofA=0;
HMonHistAddon* HTriggerHistFiller::hiputrendShowerA=0;
HMonHistAddon* HTriggerHistFiller::hiputrendMatchUA=0;
HMonHistAddon* HTriggerHistFiller::htriggerTrendTimeA=0;
HMonHistAddon* HTriggerHistFiller::hiputrendRichTimeA=0;
HMonHistAddon* HTriggerHistFiller::hiputrendTofTimeA=0;
HMonHistAddon* HTriggerHistFiller::hiputrendShowerTimeA=0;
HMonHistAddon* HTriggerHistFiller::hiputrendMatchUTimeA=0;
HMonHistAddon* HTriggerHistFiller::hipubutterflyA=0;
HMonHistAddon* HTriggerHistFiller::hmulepmomA=0;
HMonHistAddon* HTriggerHistFiller::hmuCorrAllA=0;

ClassImp(HTriggerHistFiller)

  HTriggerHistFiller::HTriggerHistFiller(const Char_t* name,const Char_t* title)
    : TNamed(name,title)
{
  colorsModule.Set(2);
  colorsCuts.Set(4);
  colorsIPUtrend.Set(7);
  resetCounter();
  resetTimeCounter();
  for(Int_t i=0; i<4; i++){
    ipuTimeOld[i]=0;
    for(Int_t j=0; j<7; j++){
      ipuTimeEventCounter[i][j]=0;
    }
  }
  for(Int_t i=0; i<3; i++){
    for(Int_t j=0; j<3; j++){
      richCounter[i][j]=0;
    }
  }
}
HTriggerHistFiller::~HTriggerHistFiller()
{
}

void HTriggerHistFiller::fillRich(HIterator* iterrich)
{
  HMatchURich* rich=0;
  Int_t s=0;
  Int_t count[7]; //count number of rings per sector (0-5) and global (6)
  for(Int_t i=0;i<7;i++)
    {
      count[i]=0;
    }
  while ((rich=(HMatchURich *)iterrich->Next())!=0)
    {
      s=rich->getSegmentId();
      iputrendhit[0][s]++;
      iputrendhit[0][6]++;
      count[6]++;
      if((s>=0) && (s<=5))
	{
	  count[s]++;
	}
      else
	{
	  cout << "---trigger: ---fillRich: ---Sector out of bounds... should be 0-5 but is: " << s << endl;
	}
    }
  for(Int_t i=0;i<7;i++)
    {
      iputrendevent[0][i]++;
    }
  for(Int_t i=0;i<3;i++)
    {
      richCounter[0][i]++;
    }
  if(count[6]>=2)
    {
      for(Int_t i=0;i<3;i++)
	{
	  richCounter [1][i]++;
	  richCounter [2][i]++;
	}
    }
  else
    {
      if(count[6]>=1)
	{
	  for(Int_t i=0;i<3;i++)
	    {
	      richCounter [1][i]++;
	    }
	}
    }
  if(richCounter[0][0]==100)
    {
      richCounter[0][0]=0;
      for(Int_t i=0;i<2;i++)
	{
	  richResult[i][0]=(Float_t)richCounter[i+1][0]/100.0;
	  richCounter[i+1][0]=0;
	}
    }
  if(richCounter[0][1]==1000)
    {
      richCounter[0][1]=0;
      for(Int_t i=0;i<2;i++)
	{
	  richResult[i][1]=(Float_t)richCounter[i+1][1]/1000.0;
	  richCounter[i+1][1]=0;
	}
    }
  if(richCounter[0][2]==10000)
    {
      richCounter[0][2]=0;
      for(Int_t i=0;i<2;i++)
	{
	  richResult[i][2]=(Float_t)richCounter[i+1][2]/10000.0;
	  richCounter[i+1][2]=0;
	}
    }

  if(htriggerrichringmultdistrib)
    {
      for(Int_t i=0;i<6;i++)
	{
	  htriggerrichringmultdistrib->fill(0,i,count[i]);
	}
    }
  if(htriggermultdistrib)
    {
      htriggermultdistrib->fill(0,0,count[6]);
    }
}

void HTriggerHistFiller::fillTof(HIterator* itertof)
{
  HMatchUTof * tof=0;
  Int_t s=0;
  Int_t count[7]; //count number of hits per sector (0-5) and global (6)
  for(Int_t i=0;i<7;i++)
    {
      count[i]=0;
    }
  Int_t pidTof=0;
  Float_t time;
  while ((tof=(HMatchUTof *)itertof->Next())!=0)
    {
      time=tof->getTime();
      if(htof)
	{
	  htof->getP()->Fill(time);
	}
      s = tof->getSector();
      pidTof = tof->getPID();
      iputrendhit[1][s]++;
      iputrendhit[1][6]++;
      if(pidTof==1)
	{
	  tofLeptons[s]++;
	  tofLeptons[6]++;
	}
      count[6]++;
      if((s>=0) && (s<=5))
	{
	  count[s]++;
	}
      else
	{
	  cout << "---trigger: ---fillTof: ---Sector out of bounds... should be 0-5 but is: " << s << endl;
	}
    }
  for(Int_t i=0;i<7;i++)
    {
      iputrendevent[1][i]++;
    }

  if(htriggertofhitmultdistrib)
    {
      for(Int_t i=0;i<6;i++)
	{
	  htriggertofhitmultdistrib->fill(0,i,count[i]);
	}
    }
  if(htriggermultdistrib)
    {
      htriggermultdistrib->fill(0,1,count[6]);
    }
}

void HTriggerHistFiller::fillRichCorrelation(HIterator *iterHMatchURich, HIterator* iterHRichHitIPU)
{
  typedef struct
  {
    Int_t X;
    Int_t Y;
    Int_t MF;
  } RichIPUtype;
  
  typedef struct
  {
    Int_t X;
    Int_t Y;
    Int_t RQ;
    Int_t VQ;
    Int_t MF;
  } RichAlgotype;

      Int_t ipuword[6][96][12];
      Int_t numberOfIpuWords[6];
      Int_t ipuHitsSec[6];	// Number of hits from the IPU per sector
      Int_t anaHitsSec[6];	// Number of hits from the ana per sector

      Int_t skipEventFlag;
#define MAX_IPUHITSEC 400
#define MAX_IPUHIT  MAX_IPUHITSEC*6
      RichIPUtype ipucoord[6][MAX_IPUHITSEC];	// Contains all IPU rings coordinates (HMatchURich)
      RichAlgotype anacoord[6][MAX_IPUHITSEC];	// Contains all ana rings coordinates (HRichHitIPU) and quality for veto e ring

      Int_t myX, myY, mySect;

      Int_t diffX, diffY, diffSquared;
      Int_t diffList[MAX_IPUHIT][3];
      Int_t diffMatrix[MAX_IPUHITSEC][MAX_IPUHITSEC];
      Int_t SqrDiff;
      Int_t corrList[MAX_IPUHIT][5];
      Int_t nonCorrelatedIpu, nonCorrelatedAna;
      Int_t listLen, listPos, listPos2;
  
//  listPos = 0;
//  listPos2 = 0;
      
//      if ((fEventHead)->getId () == 1)
  if(1)
  {

    for (Int_t j = 0; j < 6; j++)
      {
        numberOfIpuWords[j] = 0;
        for (Int_t ii = 0; ii < 96; ii++)
      for (Int_t jj = 0; jj < 12; jj++)
        ipuword[j][ii][jj] = 0;
      }

    for (Int_t j = 0; j < 6; j++)
      {
        ipuHitsSec[j] = 0;
        anaHitsSec[j] = 0;
      }

    iterHRichHitIPU->Reset ();
    iterHMatchURich->Reset ();

    skipEventFlag = 0;

    HMatchURich *ptlHMatchURich;
    while ((ptlHMatchURich =
        (HMatchURich *) iterHMatchURich->Next ()) != 0)
      {			// get all IPU rings   

        myX = ptlHMatchURich->getRow ();	// -3 for new FPGA design (Jan04) !!!!!!
        myY = ptlHMatchURich->getColumn ();
        mySect = ptlHMatchURich->getSegmentId ();

        if(ipuHitsSec[mySect]==MAX_IPUHITSEC) continue;// Overflow
        
        ipucoord[mySect][ipuHitsSec[mySect]].X = myX;
        ipucoord[mySect][ipuHitsSec[mySect]].Y = myY;
        ipucoord[mySect][ipuHitsSec[mySect]].MF = 0;

        ipuword[mySect][myX][(Int_t) myY / 8]++;
        if (ipuword[mySect][myX][(Int_t) myY / 8] == 1)
      numberOfIpuWords[mySect]++;

        if (numberOfIpuWords[mySect] == 8)
      skipEventFlag++;

        ipuHitsSec[mySect]++;
      }


    Int_t RingsumPattern[6][96][96];	// Contains the quality of the ring for each sector and each ring center
    for (Int_t s = 0; s < 6; s++)
      for (Int_t r = 0; r < 96; r++)
        for (Int_t c = 0; c < 96; c++)
      RingsumPattern[s][r][c] = 0;




    Int_t version = 2;	// 1=old, 2=new

    HRichHitIPU *ptlHRichHitIPU;
    
    iterHRichHitIPU->Reset ();
    while ((ptlHRichHitIPU =
        (HRichHitIPU *) iterHRichHitIPU->Next ()) != 0)
      {
        if ((version == 1) ||
        ((version == 2)
        && ((ptlHRichHitIPU->getQuality () > 8)
            && (ptlHRichHitIPU->getVetoQuality () < 4))))
      {
        mySect = ptlHRichHitIPU->getSector ();
        myY = ptlHRichHitIPU->getY ();
        myX = ptlHRichHitIPU->getX ();
        if ((myX == 0) || (myY == 0))
          continue;
        RingsumPattern[mySect][myX][myY] =
          ptlHRichHitIPU->getQuality ();
      }

      }
    iterHRichHitIPU->Reset ();
    while ((ptlHRichHitIPU =
        (HRichHitIPU *) iterHRichHitIPU->Next ()) != 0)
      {
        mySect = ptlHRichHitIPU->getSector ();
        myY = ptlHRichHitIPU->getY ();
        myX = ptlHRichHitIPU->getX ();
        if(anaHitsSec[mySect]==MAX_IPUHITSEC) continue;
        
        if ((myX == 0) || (myY == 0))
      continue;
        if ((ptlHRichHitIPU->getQuality () > 8)
        && (ptlHRichHitIPU->getVetoQuality () < 4))
      {
        Int_t CurrentSum = ptlHRichHitIPU->getQuality ();
        if (CurrentSum >= RingsumPattern[mySect][myX + 1][myY] &&
            CurrentSum >= RingsumPattern[mySect][myX][myY + 1] &&
            CurrentSum > RingsumPattern[mySect][myX - 1][myY] &&
            CurrentSum > RingsumPattern[mySect][myX][myY - 1])
          {		// Local maximun search

            anacoord[mySect][anaHitsSec[mySect]].X = myX;
            anacoord[mySect][anaHitsSec[mySect]].Y = myY;
            anacoord[mySect][anaHitsSec[mySect]].RQ =
        ptlHRichHitIPU->getQuality ();
            anacoord[mySect][anaHitsSec[mySect]].VQ =
        ptlHRichHitIPU->getVetoQuality ();
            anacoord[mySect][anaHitsSec[mySect]].MF = 0;

            anaHitsSec[mySect]++;
          }
      }
      }


    //- ****** Correlate closest ipu/ana pairs *****

    for (Int_t sec = 0; sec < 6; sec++)
      {			// loop over sector
        //if (ipuHitsSec[sec] ==1 ) { // restrict to multiplicity 1 in IPU
        if ((anaHitsSec[sec] > 7) || (ipuHitsSec[sec] > 7))
      continue;	// If there is more than 7 hits per sector skip!

        listLen = 0;	// current number of array elements in difflist
        listPos = 0;

        //-** Create sorted list (diffList) of squared distances for all ipu-ana-hit combinations **
        //     Sorted by increasing squared distance

        // Reset difflist (all array elements set to zero)
        for (Int_t k = 0; k < MAX_IPUHIT; k++) diffList[k][0] = 0;

        for (Int_t actAna = 0; actAna < anaHitsSec[sec]; actAna++)
      {		// loop over  softrings
        for (Int_t actIpu = 0; actIpu < ipuHitsSec[sec]; actIpu++)
          {		// loop over  hardrings


            diffX =
        ipucoord[sec][actIpu].X - anacoord[sec][actAna].X;
            diffY =
        ipucoord[sec][actIpu].Y - anacoord[sec][actAna].Y;
            diffSquared = diffX * diffX + diffY * diffY;	// squared distance ana-ipu

            // Now determine in which position (listPos) the new value 
            // must be placed in the SORTED list

            if (listLen == 0)
        {
          listPos = 0;	// special case: first entry goes to position zero in difflist
        }
            else
        {
          // loop over existing entries in difflist until new value 
          // diffSquared is bigger
          while ((diffSquared > diffList[listPos][0])
            && (listPos < listLen))
            {
              listPos++;
            }

          // now we found the position where the new array element 
          // has to be placed in difflist
          // and we copy all following element to the next position 
          // in order to get a free array element for the new entry
          for (Int_t listPos2 = listLen; listPos2 > listPos;
                listPos2--)
            {
              diffList[listPos2][0] =
            diffList[listPos2 - 1][0];
              diffList[listPos2][1] =
            diffList[listPos2 - 1][1];
              diffList[listPos2][2] =
            diffList[listPos2 - 1][2];
            }
        }

            // Now we can fill the new array elements at position listPos           
            listLen++;	// increase number of array elements by one             
            
            SqrDiff = diffX * diffX + diffY * diffY;
            diffMatrix[actAna][actIpu] = SqrDiff;
            diffList[listPos][0] = SqrDiff;	// squared distance
            diffList[listPos][1] = actAna;	//  corresponding ana ring (anacoord)   
            diffList[listPos][2] = actIpu;	//  corresponding ipu ring (ipucoord)
            listPos = 0;

          }		// end of loop over hardrings
      }		// end of loop over softrings 


        //- ** Now we can start to correlate ipu and ana rings based on diffList
        //-** group ipu-ana hit pairs with smallest distances **

        nonCorrelatedIpu = ipuHitsSec[sec];
        nonCorrelatedAna = anaHitsSec[sec];
        listPos = 0;
        listPos2 = 0;
        while ((nonCorrelatedIpu > 0) && (nonCorrelatedAna > 0)
          && (listPos < listLen))
      {
        Int_t actAna = diffList[listPos][1];
        Int_t actIpu = diffList[listPos][2];
        if ((ipucoord[sec][actIpu].MF == 0)
            && (anacoord[sec][actAna].MF == 0))
          {
            corrList[listPos2][0] = sec;
            corrList[listPos2][1] = anacoord[sec][actAna].X;
            corrList[listPos2][2] = anacoord[sec][actAna].Y;
            corrList[listPos2][3] = ipucoord[sec][actIpu].X;
            corrList[listPos2][4] = ipucoord[sec][actIpu].Y;
            listPos2++;
            anacoord[sec][actAna].MF++;
            anacoord[sec][actAna].MF++;
            ipucoord[sec][actIpu].MF++;
            ipucoord[sec][actIpu].MF++;
            nonCorrelatedIpu--;
            nonCorrelatedAna--;
          }
        listPos++;
      }


        //-** group all remaining ana hits with (0,0) **
        for (Int_t actAna = 0; actAna < anaHitsSec[sec]; actAna++)
      {
        if ((anacoord[sec][actAna].MF == 0))
          {		// if it has not been correlated yet

            Int_t NeighborFlag = 0;
            for (Int_t actIpu = 0; actIpu < ipuHitsSec[sec]; actIpu++)
        {
          if (diffMatrix[actAna][actIpu] <= 2)
            NeighborFlag = 1;
        }

            corrList[listPos2][0] = sec;
            corrList[listPos2][1] = anacoord[sec][actAna].X;
            corrList[listPos2][2] = anacoord[sec][actAna].Y;
            corrList[listPos2][3] = 0;
            corrList[listPos2][4] = 0;

            listPos2++;
          }
      }

        //-** group all remaining ipu hits with (0,0) **
        for (Int_t actIpu = 0; actIpu < ipuHitsSec[sec]; actIpu++)
      {
        if ((ipucoord[sec][actIpu].MF == 0))
          {		// if it has not been correlated yet


            Int_t NeighborFlag = 0;
            for (Int_t actAna = 0; actAna < anaHitsSec[sec]; actAna++)
        {
          if (diffMatrix[actAna][actIpu] <= 2)
            NeighborFlag = 1;
        }

            corrList[listPos2][0] = sec;
            corrList[listPos2][1] = 0;
            corrList[listPos2][2] = 0;
            corrList[listPos2][3] = ipucoord[sec][actIpu].X;
            corrList[listPos2][4] = ipucoord[sec][actIpu].Y;

            listPos2++;
          }
      }

        //-** fill histograms from list of correlated pairs (corrList) **
        for (listPos = 0; listPos < listPos2; listPos++)
      {
        sec = corrList[listPos][0];
/*        hshcx[sec]->Fill ((Float_t) corrList[listPos][1],
                (Float_t) corrList[listPos][3],
                (Stat_t) 1);
        hshcy[sec]->Fill ((Float_t) corrList[listPos][2],
                (Float_t) corrList[listPos][4],
                (Stat_t) 1);*/
	  hrichCorrPhi->fill(0,sec,(Float_t) corrList[listPos][1],
                (Float_t) corrList[listPos][3]);
	  hrichCorrTheta->fill(0,sec,(Float_t) corrList[listPos][2],
                (Float_t) corrList[listPos][4]);
      }


        //- *********** End of Correlation ************

        //}

      }			// end of loop over sector
  }			// real events
}
void HTriggerHistFiller::fillMatchUCorrelation(
   HIterator* iterHMatchURich,HIterator *iterHMatchUTof,HIterator *iterHMatchUShower,
   HIterator *iterHMULeptons,HIterator *iterHMUEMULeptons)
{
typedef struct {
  Float_t rth;
  Float_t rph;
  Float_t mth;
  Float_t mph;
  Float_t p;
  Float_t DP;
  Float_t DT;
  Int_t eleflag;
  Int_t MF;
  Int_t sec;
} MULeptontype;

typedef struct {
  Float_t m_a;
  Float_t opangle_a;
  Float_t m_u;
  Float_t opangle_u;
  Float_t m_l;
  Float_t opangle_l;
} MUEmuDiLeptontype;


  HMatchURich* ptlHMatchURich= 0;
  HMatchUTof* ptlHMatchUTof= 0;
  HMatchUShower* ptlHMatchUShower= 0;
  HMULeptons* ptlHMULeptons= 0;
  HMUEMULeptons* ptlHMUEMULeptons= 0;

  Int_t rRichIPU, cRichIPU, sRichIPU;
  Int_t nLeptons, leptperevt, emuleptperevt, dileptperevt_u, dileptperevt_l, dileptperevt_a;
  Float_t diffTheta, diffPhi;
  Float_t diffPhi_Rich, diffTheta_Rich, diffPhi_Meta, diffTheta_Meta, diffSquared;


  Int_t EPS_MU, EPS_EMU;

#define MU_MAX_LIST  1000   // for savety reason --> better check overflow in code
#define MU_MAX_HITS  200
  Float_t diffList[MU_MAX_LIST][3];
  Float_t corrList[MU_MAX_LIST][8];
  Int_t nonCorrelatedIpu, nonCorrelatedAna;
  Int_t listLen, listPos, listPos2;

  MULeptontype   MU_EmuLeptonHits[MU_MAX_HITS];
  MULeptontype      MU_LeptonHits[MU_MAX_HITS];

  Int_t MU_nLeptons;
  Int_t MU_nEmuLeptons;

  Int_t ipuword[6][96][12];
  Int_t numberOfIpuWords[6];

	nLeptons = leptperevt = emuleptperevt = 0;
	dileptperevt_a = dileptperevt_u = dileptperevt_l = 0;
	
	EPS_MU = EPS_EMU = 0;

	MU_nLeptons=0;
	MU_nEmuLeptons=0;

	Int_t TotalNumberOfRichIpuWords = 0;
	Int_t TotalNumberOfTofHits = 0;
	Int_t TotalNumberOfShowerIpuWords = 0;
	Int_t ipuwordSho[6][32][2];
      
	for (Int_t ii =0; ii<6; ii++) {
	  numberOfIpuWords[ii]=0;
	  for (Int_t iic = 0; iic<32; iic++)
	    {
	      for (Int_t iim = 0; iim<2; iim++)
		ipuwordSho[ii][iic][iim] = 0;
	    }
	  for (Int_t iic = 0; iic<96; iic++)
	    {
	      for (Int_t iim = 0; iim<12; iim++)
		ipuword[ii][iic][iim] = 0;
	    }
	}
	
	//// RICH
	while ( (ptlHMatchURich = (HMatchURich *)iterHMatchURich->Next() ) != 0  )
	  {
	    rRichIPU = ptlHMatchURich->getRow();
	    cRichIPU = ptlHMatchURich->getColumn();
	    sRichIPU = ptlHMatchURich->getSegmentId();
	    
	    ipuword[sRichIPU][rRichIPU ][(Int_t)cRichIPU/8]++;
	    if ( ipuword[sRichIPU][rRichIPU][(Int_t)cRichIPU/8] == 1 )
	      {
		numberOfIpuWords[sRichIPU]++;
		TotalNumberOfRichIpuWords++;
	      }
	  
//	    if(verbose) cout << "RICH: sec " << sRichIPU << " theta " << ptlHMatchURich->getTheta() << " phi "<< ptlHMatchURich->getPhi() << " row " << rRichIPU << " col " << cRichIPU <<  endl;
	  } // End while
	if (TotalNumberOfRichIpuWords>=6) 
	  return;

	while ((ptlHMatchUTof = (HMatchUTof *)iterHMatchUTof->Next()) != 0) {
//	  if(verbose) cout << "Tof: sec " << ptlHMatchUTof->getSector() << " theta " << ptlHMatchUTof->getTheta() 
// 	       << " phi " << ptlHMatchUTof->getPhi() << " time " << ptlHMatchUTof->getTime()
// 	       << " PID " << ptlHMatchUTof->getPID() << endl;
	  TotalNumberOfTofHits++;
	}
 Int_t rShoIPU, cShoIPU, sShoIPU;
	while ((ptlHMatchUShower = (HMatchUShower *)iterHMatchUShower->Next()) != 0) {
//	  if(verbose) cout << "Shower: sec " << ptlHMatchUShower->getSector() << " theta " << ptlHMatchUShower->getTheta()
//	       << " phi " << ptlHMatchUShower->getPhi() << endl;

	  rShoIPU = ptlHMatchUShower->getRow();
	  cShoIPU = ptlHMatchUShower->getColumn();
	  sShoIPU = ptlHMatchUShower->getSector();

	  ipuwordSho[sShoIPU][rShoIPU][(Int_t)cShoIPU/16]++;
	  if (ipuwordSho[sShoIPU][rShoIPU][(Int_t)cShoIPU/16]==1)
	    {
	      TotalNumberOfShowerIpuWords++;
	    }

	}

      if (TotalNumberOfRichIpuWords * (TotalNumberOfShowerIpuWords+TotalNumberOfTofHits)>=50) return;

//      cout << endl;

      Int_t MaxWords = 8;
      Int_t toomanyringsinevt=0;
      Int_t toomanyringsinsec[6];



      for (Int_t ii =0; ii<6; ii++) {
	toomanyringsinsec[ii] =0;
	if ( numberOfIpuWords[ii]>= MaxWords ) {
	  //cout << " numberOfIpuWords: " <<  numberOfIpuWords[ii] << endl;
	  toomanyringsinsec[ii] =1;
	  //cout << "toomanyringsinsec[" << ii << "]= " << toomanyringsinsec[ii] << endl;
	}

	if (toomanyringsinsec[ii]){
	  toomanyringsinevt =1;
	  //cout << "toomanyringsinevt " << toomanyringsinevt << endl;
	}
      }

      if (toomanyringsinevt){
	cout << "skip the evt (toomanyringsinevt)" << endl;
	return;
	// if the rich has exceeded the max number of data words, the matching unit triggers the event positively,
	// without even trying a match
	// therefore we skip the event without trying a correlation with the emulation
      }

      Float_t phi = 0.; 
      //// LEPTONS
      while (( ptlHMULeptons  = (HMULeptons *)iterHMULeptons->Next()) != 0) {
	Int_t sector = ptlHMULeptons->getSector();

	MU_LeptonHits[MU_nLeptons].sec = sector;

	//// rich in LEPTON
	MU_LeptonHits[MU_nLeptons].rth = ptlHMULeptons->getThetaRich();
	MU_LeptonHits[MU_nLeptons].rph = ptlHMULeptons->getPhiRich();
	if (sector<5)
	  MU_LeptonHits[MU_nLeptons].rph -= 60*(sector+1);

	//// meta in LEPTON
	MU_LeptonHits[MU_nLeptons].mth = ptlHMULeptons->getThetaMeta();
	MU_LeptonHits[MU_nLeptons].mph = ptlHMULeptons->getPhiMeta(); 
	if (sector<5)
	  MU_LeptonHits[MU_nLeptons].mph -= 60*(sector+1);

	MU_LeptonHits[MU_nLeptons].p = ptlHMULeptons->getMom();
	MU_LeptonHits[MU_nLeptons].eleflag = ptlHMULeptons->getFlag();
	MU_LeptonHits[MU_nLeptons].DP = MU_LeptonHits[MU_nLeptons].rph-MU_LeptonHits[MU_nLeptons].mph;
	MU_LeptonHits[MU_nLeptons].DT = MU_LeptonHits[MU_nLeptons].rth-MU_LeptonHits[MU_nLeptons].mth;
	MU_LeptonHits[MU_nLeptons].MF = 0;

	diffTheta = MU_LeptonHits[MU_nLeptons].DT;
	diffPhi = MU_LeptonHits[MU_nLeptons].DP;


	phi = MU_LeptonHits[MU_nLeptons].rph;
	if (sector < 5) phi -= 60*(sector+1);
//////	hmuhits->Fill(phi,MU_LeptonHits[MU_nLeptons].DP);

/*	if(verbose){
		 cout << i <<  " seq num " <<(fEventHead)->getEventSeqNumber() <<
		 	"   ****************Found Lepton in MU." << endl;
		cout << "Lept:  sec " << ptlHMULeptons->getSector()
	     		<< "  detector bit  " << ptlHMULeptons->getDetBit() << endl;
		cout << "RICH: phi " << MU_LeptonHits[MU_nLeptons].rph
	     		<< "  theta  " << ptlHMULeptons->getThetaRich() << " nr " << ptlHMULeptons->getRichNr() << endl;
		cout << "META: phi  " <<  MU_LeptonHits[MU_nLeptons].mph
	     		<< "  theta  " <<   ptlHMULeptons->getThetaMeta() << " nr " << ptlHMULeptons->getMetaNr()
	     		<< "  momentum  " << MU_LeptonHits[MU_nLeptons].p << endl;
		cout << "***********************PHI DIFFERENCE " << diffPhi << endl;
	}*/

	//    if (diffTheta>0&&MU_LeptonHits[MU_nLeptons].eleflag==1) cout << "***************************evt " << i
	// 								<<"WRONG ELE FLAG!!!" << endl;
	//    if (diffTheta<0&&MU_LeptonHits[MU_nLeptons].eleflag==0) cout << "***************************evt " << i
	// 								<< "WRONG ELE FLAG!!!" << endl;
	//    cout << "---------------------------------------" << endl;

	MU_nLeptons++;
	leptperevt++;
      }
//      cout << endl;
//      cout << endl;

      // --------------------- EMU LEPTONS ------------------------------------
      while ((ptlHMUEMULeptons = (HMUEMULeptons *)iterHMUEMULeptons->Next()) != 0) {  // get allemu leptons
	if (MU_nEmuLeptons<7) {
	  MU_EmuLeptonHits[MU_nEmuLeptons].sec  = ptlHMUEMULeptons->getSector();
	  MU_EmuLeptonHits[MU_nEmuLeptons].p    = ptlHMUEMULeptons->getMom();
	  MU_EmuLeptonHits[MU_nEmuLeptons].rth  = ptlHMUEMULeptons->getThetaRich();
	  MU_EmuLeptonHits[MU_nEmuLeptons].rph  = ptlHMUEMULeptons->getPhiRich();
	  MU_EmuLeptonHits[MU_nEmuLeptons].mth  = ptlHMUEMULeptons->getThetaMeta();
	  MU_EmuLeptonHits[MU_nEmuLeptons].mph  = ptlHMUEMULeptons->getPhiMeta();
	  MU_EmuLeptonHits[MU_nEmuLeptons].eleflag = ptlHMUEMULeptons->getFlag();
	  MU_EmuLeptonHits[MU_nEmuLeptons].DP = MU_EmuLeptonHits[MU_nEmuLeptons].rph-MU_EmuLeptonHits[MU_nEmuLeptons].mph;
	  MU_EmuLeptonHits[MU_nEmuLeptons].DT = MU_EmuLeptonHits[MU_nEmuLeptons].rth-MU_EmuLeptonHits[MU_nEmuLeptons].mth;
	  MU_EmuLeptonHits[MU_nEmuLeptons].MF = 0;


/*	if(verbose){
	  cout << i << " seq num " <<(fEventHead)->getEventSeqNumber() <<"   ****************Found Lepton in MU- EMULATION." << endl;
	  cout << "Lept:  sec "<< ptlHMUEMULeptons->getSector() << endl;
	  cout << "RICH: phi " << ptlHMUEMULeptons->getPhiRich()
	       << "  theta  "  << ptlHMUEMULeptons->getThetaRich() << endl;
	  cout << "META: phi " << ptlHMUEMULeptons->getPhiMeta()
	       << "  theta  "  << ptlHMUEMULeptons->getThetaMeta()
	       << "  momentum  " << MU_EmuLeptonHits[MU_nEmuLeptons].p << endl;

	  cout << "***********************PHI DIFFERENCE " << MU_EmuLeptonHits[MU_nEmuLeptons].DP << endl;
	  cout << "---------------------------------------" << endl;
	}*/
	  MU_nEmuLeptons++;
	  emuleptperevt++;
	}
      }

//      cout << endl;



      //- ************************** Correlate closest ipu/ana pairs *****************************

      // MU_LeptonHits[i][] contains hits from real hardware
      // MU_EmuLeptonHits[i][] contains hits from offline analysis

      listLen = 0;   // current number of array elements in difflist
      listPos = 0;

      Int_t actAna, actIpu;    
      //-** Create sorted list (diffList) of squared distances for all ipu-ana-hit combinations **
      //     Sorted by increasing squared distance

      // Reset difflist (all array elements set to zero)
// if this is really needed, the code is shit. takes awful lot of time
/*      for ( Int_t k=0; k<30000;k++) { //100 arbitrary value,1600 would be consistent
	for(Int_t kkk=0; kkk<3; kkk++)
	  { diffList[k][kkk] = 0;
	  }
	for(Int_t kkk=0; kkk<8; kkk++)//was 7
	  { corrList[k][kkk] = 0;
	  }
      }*/

      if (leptperevt>30) return; // i do not any correlation for events with cutoff

      for( actAna=0; actAna<emuleptperevt;actAna++) {    // loop over  softhits
	for( actIpu=0; actIpu<leptperevt;actIpu++) {   // loop over  hardhits

	  //       if ( (MU_LeptonHits[actIpu].sec == MU_EmuLeptonHits[actAna].sec ) &&
	  //  	  (MU_LeptonHits[actIpu].sec>1) ) {

	  diffPhi_Rich   = MU_LeptonHits[actIpu].rph - MU_EmuLeptonHits[actAna].rph;
	  diffTheta_Rich = MU_LeptonHits[actIpu].rth - MU_EmuLeptonHits[actAna].rth;
	  diffPhi_Meta   = MU_LeptonHits[actIpu].mph - MU_EmuLeptonHits[actAna].mph;
	  diffTheta_Meta = MU_LeptonHits[actIpu].mth - MU_EmuLeptonHits[actAna].mth;
	  diffSquared    = diffPhi_Rich*diffPhi_Rich + diffTheta_Rich*diffTheta_Rich +
	    diffPhi_Meta*diffPhi_Meta + diffTheta_Meta*diffTheta_Meta;   // squared distance ana-ipu
//////	  diff->Fill(diffSquared);
	  //      cout << "Calc " << listPos << " " << diffSquared << " " << actAna
	  // 	  << " " << actIpu << endl; 
	  // Now determine in which position (listPos) the new value 
	  // must be placed in the SORTED list
	  if ( listLen == 0 ) {
	    listPos = 0;  // special case: first entry goes to position zero in difflist
	  } else {
	    // loop over existing entries in difflist until new value
	    // diffSquared is bigger
	    while ( (diffSquared > diffList[listPos][0]) && ( listPos < listLen) ) {
	      listPos++;
	    }
	    // now we found the position where the new array element 
	    // has to be placed in difflist
	    // and we copy all following element to the next position 
	    // in order to get a free array element for the new entry
	    for(Int_t listPos2=listLen; listPos2>listPos;listPos2--) {
	      diffList[listPos2][0] = diffList[listPos2-1][0];
	      diffList[listPos2][1] = diffList[listPos2-1][1];
	      diffList[listPos2][2] = diffList[listPos2-1][2];
	    }
	  }                                                 

	  // Now we can fill the new array elements at position listPos

	  listLen++;       // increase number of array elements by one

	  // diffList contains:


	  //      cout << "Fill " << listPos << " " << diffSquared << " "
	  // 	  << actAna << " " << actIpu << endl; 
	  diffList[listPos][0] = diffSquared;    // squared distance
	  diffList[listPos][1] = actAna;  //  corresponding ana ring (MU_EmuLeptonHits)
	  diffList[listPos][2] = actIpu;  //  corresponding ipu ring (MU_LeptonHits)
	  listPos = 0;


            if(listLen>=1000) return;
	  //} // end of sector condition
	} // end of loop over hardhits
      } // end of loop over softhits 


	//- ** Now we can start to correlate ipu and ana rings based on diffList

	//-** group ipu-ana hit pairs with smallest distances **
      nonCorrelatedIpu = leptperevt;
      nonCorrelatedAna = emuleptperevt;
      //  cout << " non correlated: " << nonCorrelatedIpu << " " << nonCorrelatedAna << endl;

      listPos = 0; listPos2 = 0;
   while ( ( nonCorrelatedIpu > 0 ) && ( nonCorrelatedAna > 0 ) && ( listPos<listLen) ) {
      actAna = (Int_t)diffList[listPos][1];
      actIpu = (Int_t)diffList[listPos][2];
      //     cout << "act ana " << actAna << MU_EmuLeptonHits[actAna].rth << "  " << MU_EmuLeptonHits[actAna].rph << endl;
      //     cout << "act ipu " << actIpu << MU_LeptonHits[actIpu].rth << "  " << MU_LeptonHits[actIpu].rph << endl;
      //     cout << "matching flag: " << MU_LeptonHits[actIpu].MF << "  " << MU_EmuLeptonHits[actAna].MF << endl;

      if ( (MU_LeptonHits[actIpu].MF==0) && (MU_EmuLeptonHits[actAna].MF==0) ) {
	//      cout << "matching flag: " << MU_LeptonHits[actIpu].MF << "  " << MU_EmuLeptonHits[actAna].MF << endl;
        corrList[listPos2][0] = MU_LeptonHits[actIpu].sec;
	corrList[listPos2][1] = MU_EmuLeptonHits[actAna].DP;
	corrList[listPos2][2] = MU_EmuLeptonHits[actAna].DT;
	corrList[listPos2][3] = MU_LeptonHits[actIpu].DP;
	corrList[listPos2][4] = MU_LeptonHits[actIpu].DT;
	corrList[listPos2][5] = MU_EmuLeptonHits[actAna].p;
	corrList[listPos2][6] = MU_LeptonHits[actIpu].p;
        corrList[listPos2][7] = MU_LeptonHits[actIpu].mth;

	if (  fabs(MU_EmuLeptonHits[actAna].DT-MU_LeptonHits[actIpu].DT) > 0.1 ||
	      fabs(MU_EmuLeptonHits[actAna].DP-MU_LeptonHits[actIpu].DP) > 0.1 ) {
// 	  cout << "*********************evt " << i << " seq num " <<(fEventHead)->getEventSeqNumber() << endl;
// 	  cout << "  BAD CORRELATION: Delta phi Emu " << corrList[listPos2][1]
// 	       << "  Delta theta Emu "  << corrList[listPos2][2] << "  Delta phi MU "
// 	       << corrList[listPos2][3] << "  Delta theta MU "  << corrList[listPos2][4] << endl;
	}

	listPos2++;
	MU_EmuLeptonHits[actAna].MF++;
	MU_LeptonHits[actIpu].MF++;
	nonCorrelatedIpu--; nonCorrelatedAna--;	  
	//cout << " NCI: " << nonCorrelatedIpu << "  NCA " << nonCorrelatedAna << endl;
      }
      listPos++;
      }// end while

      //cout << " non correlated: " << nonCorrelatedIpu << " " << nonCorrelatedAna << endl;

      //-** group all remaining ipu (or ana) hits with (0,0) **
      /*
	for( actAna=0; actAna<leptperevt; actAna++ ) {
	if ( MU_EmuLeptonHits[actAna].MF==0 ) {

	Float_t phi_Rich = MU_EmuLeptonHits[actAna].rph;
	if (MU_EmuLeptonHits[actAna].sec < 5)
	phi_Rich -= 60* (MU_EmuLeptonHits[actAna].sec+1);

	corrList[listPos2][1] = MU_EmuLeptonHits[actAna].DP;
	corrList[listPos2][2] = MU_EmuLeptonHits[actAna].DT;
	corrList[listPos2][3] = 0;
	corrList[listPos2][4] = 0;
	hnca->Fill((Float_t)corrList[listPos2][2],(Float_t)corrList[listPos2][1],(Stat_t)1);
	hnca_2->Fill((Float_t) phi_Rich,corrList[listPos2][1],(Stat_t)1);

	//if ( fabs ( fabs(corrList[listPos2][1]) - (8+0.25* fabs(phi_Rich-30) )  ) >0.5    ) {
	cout << "*********************evt " << i <<  " seq num " <<(fEventHead)->getEventSeqNumber() <<endl;
	cout << "  NON CorrAna: Delta phi " << corrList[listPos2][1]
	<< "  Delta theta " <<corrList[listPos2][2] << " " << corrList[listPos2][3] << " " 
	<< corrList[listPos2][4] << " phi rich " << MU_EmuLeptonHits[actAna].rph << endl;
	cout << endl;
	//}
	listPos2++;
	}
	}



	for( actIpu=0; actIpu<leptperevt;actIpu++) {
	if ( MU_LeptonHits[actIpu].MF==0 ) {

	Float_t phi_Rich = MU_LeptonHits[actIpu].rph;
	if (MU_LeptonHits[actIpu].sec < 5)
	phi_Rich -= 60* (MU_LeptonHits[actIpu].sec+1);

	corrList[listPos2][1] = 0;
	corrList[listPos2][2] = 0;
	corrList[listPos2][3] = MU_LeptonHits[actIpu].DP;
	corrList[listPos2][4] = MU_LeptonHits[actIpu].DT;
	hnci->Fill((Float_t)corrList[listPos2][4],(Float_t)corrList[listPos2][3],(Stat_t)1);
	hnci_2->Fill(phi_Rich,(Float_t)corrList[listPos2][3],(Stat_t)1);

	if ( fabs ( fabs(corrList[listPos2][3]) - (8+0.25* fabs(phi_Rich-30) )  ) >0.5    ) {
	cout << "*********************evt " << i <<  " seq num " <<(fEventHead)->getEventSeqNumber() <<endl;
	cout << "  NON CorrIPU: Delta phi " << corrList[listPos2][3]
	<< "  Delta theta "  << corrList[listPos2][4] << " " << corrList[listPos2][1] << " " 
	<< corrList[listPos2][2] << endl;
	cout << endl;
	}
	listPos2++;
	}
	}
      */
        
      //    cout << " ListPos2: " << listPos2 << endl;
      //-** fill histograms from list of correlated pairs (corrList) **
      for ( listPos = 0; listPos<listPos2; listPos++ ) {
	//     cout << "Corr " << corrList[listPos][1] << " "
	// 	 << corrList[listPos][2] << " " << corrList[listPos][3] << " "   
	// 	 << corrList[listPos][4] << endl;
	  hmuCorrPhi->fill(0,(Int_t)corrList[listPos][0],(Float_t) corrList[listPos][1],(Float_t) corrList[listPos][3]);
	  hmuCorrTheta->fill(0,(Int_t)corrList[listPos][0],(Float_t) corrList[listPos][2],(Float_t) corrList[listPos][4]);

	if(hmuCorrAll){
            if(fabs(corrList[listPos][2]- corrList[listPos][4])<1.){
               hmuCorrAll->getP()->Fill(corrList[listPos][7],corrList[listPos][0]);
            }
        }

//////	hshcx->Fill((Float_t) corrList[listPos][1],(Float_t) corrList[listPos][3],(Stat_t)1);
//////	hshcy->Fill((Float_t) corrList[listPos][2],(Float_t) corrList[listPos][4],(Stat_t)1);
//////	hshcp->Fill((Float_t) corrList[listPos][5],(Float_t) corrList[listPos][6],(Stat_t)1);
//	if (corrList[listPos][5]!=corrList[listPos][6]) {
// 	  cout << "***************** UNCORRELATED MOMENTUM in evt " << i << endl;
// 	  cout << corrList[listPos][5] << " " << corrList[listPos][6] << endl;
//	}
//	if ((corrList[listPos][1]!=corrList[listPos][3]) || //(corrList[listPos][2]!=corrList[listPos][4])) {
// 	  cout << "***************** UNCORRELATED ANGLE in evt " << i << endl;
// 	  cout << corrList[listPos][1] << " " << corrList[listPos][3] << "----"
// 	       << corrList[listPos][2] << " " << corrList[listPos][4] << endl;
//	}

      }

}
void HTriggerHistFiller::fillTofCorrelation(HIterator *itertof, HIterator* itertofhit)
{
  HTofHit *tofHit;
  HMatchUTof *ipuHit;
  Float_t phiTof, thetaTof;
  Float_t phiMatch, thetaMatch;
  TList *ipucoord[6];
  TList *anacoord[6];
  Int_t anaHitsSec[6];
  Int_t ipuHitsSec[6];
  Int_t sectorMatch;
  Int_t sectorTof;
  Float_t timeTof;
  Float_t timeMatch;
  Float_t diffPhi, diffTheta, diffSquared;
  Int_t listPos, listPos2;
  Int_t actAna, actIpu;
  Int_t nonCorrelatedIpu, nonCorrelatedAna;

  TList *diffList;
  TList *corrList;

  for(Int_t i=0;i<6;i++)
    {
      ipucoord[i] = new TList();
      anacoord[i] = new TList();
      anaHitsSec[i] = 0;
      ipuHitsSec[i] = 0;
    }
  diffList = new TList();
  corrList = new TList();
  listPos = 0;
  listPos2 = 0;
  actAna = 0;
  actIpu = 0;

  while((tofHit = (HTofHit *)itertofhit->Next()) != 0)
    {
      tofHit->getPhi(phiTof);
      tofHit->getTheta(thetaTof);
      sectorTof = tofHit->getSector();
      timeTof = tofHit->getTof();

      if(sectorTof < 5)
	phiTof -= (sectorTof + 1) * 60;

      if(((phiTof > 0.) && (phiTof < 60.)) && ((thetaTof > 43.) && (thetaTof < 90.)))
	{
	  TofTypePrivat *th = new TofTypePrivat(phiTof, thetaTof, timeTof,tofHit->getModule(),
						tofHit->getCell(),tofHit->getModule()*8+tofHit->getCell());
	  anacoord[sectorTof]->Add(th);
	  anaHitsSec[sectorTof]++;
	}
    }

  while((ipuHit = (HMatchUTof *)itertof->Next()) != 0)
    {
      phiMatch = ipuHit->getPhi();
      thetaMatch = ipuHit->getTheta();
      timeMatch = ipuHit->getTime();
      sectorMatch = ipuHit->getSector();
      if(sectorMatch < 5)
	{
	  phiMatch -= (sectorMatch + 1) * 60;
	}
      TofTypePrivat *mh = new TofTypePrivat(phiMatch, thetaMatch, timeMatch, 0, 0, sectorMatch);
      ipucoord[sectorMatch]->Add(mh);

      ipuHitsSec[sectorMatch]++;
    }
  for(Int_t sec = 0; sec < 6; sec++)
    {
      listPos = 0;
      for(actAna = 0; actAna < anaHitsSec[sec]; actAna++)
	{
	  for(actIpu = 0; actIpu < ipuHitsSec[sec]; actIpu++)
	    {
	      diffPhi = ((TofTypePrivat*)ipucoord[sec]->At(actIpu))->getPhi() - ((TofTypePrivat*)anacoord[sec]->At(actAna))->getPhi();
	      diffTheta = ((TofTypePrivat*)ipucoord[sec]->At(actIpu))->getTheta() - ((TofTypePrivat*)anacoord[sec]->At(actAna))->getTheta();
	      diffSquared = diffPhi * diffPhi + diffTheta * diffTheta;

	      if(diffList->GetSize() == 0)
		listPos = 0;
	      else
		{
		  while((diffSquared > ((ThetaCorrPrivat*)diffList->At(listPos))->getThetaDiff2()) && (listPos < diffList->GetSize()))
		    {
		      listPos++;
		      if(listPos == diffList->GetSize())
			break;
		    }
		}
	      ThetaCorrPrivat *tc = new ThetaCorrPrivat(diffSquared, actAna, actIpu);
	      diffList->AddAt(tc, listPos);
	    }
	}
      nonCorrelatedIpu = ipuHitsSec[sec];
      nonCorrelatedAna = anaHitsSec[sec];

      listPos = 0;
      listPos2 = 0;
      while((nonCorrelatedIpu > 0) && (nonCorrelatedAna > 0) && (listPos < diffList->GetSize()))
	{
	  actAna = ((ThetaCorrPrivat*)diffList->At(listPos))->getAnaHit();
	  actIpu = ((ThetaCorrPrivat*)diffList->At(listPos))->getIpuHit();
	  if((((TofTypePrivat*)ipucoord[sec]->At(actIpu))->getPhi() > 0) && (((TofTypePrivat*)anacoord[sec]->At(actAna))->getPhi() > 0))
	    {
	      CorrPrivat *cp = new CorrPrivat(sec,((TofTypePrivat*)anacoord[sec]->At(actAna))->getPhi(),
					      ((TofTypePrivat*)anacoord[sec]->At(actAna))->getTheta(),
					      ((TofTypePrivat*)ipucoord[sec]->At(actIpu))->getPhi(),
					      ((TofTypePrivat*)ipucoord[sec]->At(actIpu))->getTheta(),
					      ((TofTypePrivat*)anacoord[sec]->At(actAna))->getTime(),
					      ((TofTypePrivat*)ipucoord[sec]->At(actIpu))->getTime(),
					      ((TofTypePrivat*)anacoord[sec]->At(actAna))->getSector());
	      corrList->AddAt(cp,listPos2);
	      listPos2++;
	      nonCorrelatedIpu--;
	      nonCorrelatedAna--;
	      ((TofTypePrivat*)anacoord[sec]->At(actAna))->clear();
	      ((TofTypePrivat*)ipucoord[sec]->At(actIpu))->clear();
	    }
	  listPos++;
	}
      for(actAna = 0; actAna < anaHitsSec[sec]; actAna++)
	{
	  if((((TofTypePrivat*)anacoord[sec]->At(actAna))->getPhi() != 0) && (((TofTypePrivat*)anacoord[sec]->At(actAna))->getTheta() != 0))
	    {
	      CorrPrivat *cp = new CorrPrivat(sec,
					      ((TofTypePrivat*)anacoord[sec]->At(actAna))->getPhi(),
					      ((TofTypePrivat*)anacoord[sec]->At(actAna))->getTheta(),
					      0,
					      40,
					      ((TofTypePrivat*)anacoord[sec]->At(actAna))->getTime(),
					      0,
					      0);
	      corrList->AddAt(cp,listPos2);
	      listPos2++;
	    }
	}
      for(actIpu = 0; actIpu < ipuHitsSec[sec]; actIpu++)
	{
	  if((((TofTypePrivat*)ipucoord[sec]->At(actIpu))->getPhi() != 0) && (((TofTypePrivat*)ipucoord[sec]->At(actIpu))->getTheta() != 0))
	    {
	      CorrPrivat *cp = new CorrPrivat(sec,
					      ((TofTypePrivat*)ipucoord[sec]->At(actIpu))->getPhi(),
					      ((TofTypePrivat*)ipucoord[sec]->At(actIpu))->getTheta(),
					      0,
					      40,
					      ((TofTypePrivat*)ipucoord[sec]->At(actIpu))->getTime(),
					      0,
					      0);
	      corrList->AddAt(cp,listPos2);
	      listPos2++;
	    }
	}
      for(listPos=0;listPos<listPos2;listPos++)
	{
	  htofCorrPhi->fill(0,sec,((CorrPrivat*)corrList->At(listPos))->getPhiAna(),
			    ((CorrPrivat*)corrList->At(listPos))->getPhiIPU());
	  htofCorrTheta->fill(0,sec,((CorrPrivat*)corrList->At(listPos))->getThetaAna(),
			      ((CorrPrivat*)corrList->At(listPos))->getThetaIPU());
	}
      diffList->Delete();
      corrList->Delete();
    }
  for(Int_t i=0;i<6;i++)
    {
      ipucoord[i]->Delete();
      delete ipucoord[i];
      anacoord[i]->Delete();
      delete anacoord[i];
    }
  delete diffList;
  delete corrList;
}
void HTriggerHistFiller::fillShower(HIterator* itershower)
{
  HMatchUShower* shower=0;
  Int_t s=0;
  Int_t count[7]; //count number of hits per sector (0-5) and global (6)
  for(Int_t i=0;i<7;i++)
    {
      count[i]=0;
    }
  while ((shower=(HMatchUShower*)itershower->Next())!=0)
    {
      s=shower->getSector();
      iputrendhit[2][s]++;
      iputrendhit[2][6]++;
      count[6]++;
      if((s>=0) && (s<=5))
	{
	  count[s]++;
	}
      else
	{
	  cout << "---trigger: ---fillShower: ---Sector out of bounds... should be 0-5 but is: " << s << endl;
	}
    }
  for(Int_t i=0;i<7;i++)
    {
      iputrendevent[2][i]++;
    }

  if(htriggershowerhitmultdistrib)
    {
      for(Int_t i=0;i<6;i++)
	{
	  htriggershowerhitmultdistrib->fill(0,i,count[i]);
	}
    }
  if(htriggermultdistrib)
    {
      htriggermultdistrib->fill(0,2,count[6]);
    }
}
void HTriggerHistFiller::fillShowerCorrelation(HIterator* itershower, HIterator*itershowerhit,HTriggerParShowerMap *showerMap)
{
  TList *MUShoHit[6];
  TList *ShowerHit[6];
  TList *diffList;
  HShowerHitIPU *hitshower;
  HMatchUShower *hitmatch;
  Int_t MUShoHitsSec[6]; 			// Number of hits from MU per sector
  Int_t ShowerHitsSec[6];			// Number of hits from Shower per sector

  Int_t sec;					// counter: sector

  Float_t pphi, ptheta;				//used only to fill the number of parameters requested
  Int_t listLen, listPos, listPos2;
  Int_t MUSho,Shower;
  Int_t nonCorrelatedMUSho, nonCorrelatedShower;
  Float_t diffSquared;

  for (sec=0;sec<6;sec++)
    {
      MUShoHitsSec[sec] = 0;
      ShowerHitsSec[sec] = 0;
      MUShoHit[sec] = new TList();
      ShowerHit[sec] = new TList();
    }
  diffList = new TList();

  while((hitshower = (HShowerHitIPU *)itershowerhit->Next()) != NULL)
  {
      sec = hitshower->getSector();
      //hitshower->getSphereCoord(&pr, &pphi, &ptheta);
      pphi = showerMap->getShowerPhi(hitshower->getCol(),hitshower->getRow());
      ptheta = showerMap->getShowerTheta(hitshower->getCol(),hitshower->getRow());
      /*    if(sec < 5)
	    pphi -= 60*(sec+1);*/
      CorrShowerCord *sc = new CorrShowerCord(pphi, ptheta,0);
      ShowerHit[sec]->Add(sc);
      ShowerHitsSec[sec]++;
    }
  while((hitmatch = (HMatchUShower *)itershower->Next()) != NULL)
  {
      sec = hitmatch->getSector();
      pphi = hitmatch->getPhi();
      ptheta = hitmatch->getTheta();
      if(sec < 5)
	pphi -= 60*(sec+1);
      CorrShowerCord *sc = new CorrShowerCord(pphi, ptheta,0);
      MUShoHit[sec]->Add(sc);
      MUShoHitsSec[sec]++;
    }
  for (sec=0; sec<6; sec++)
    {
      listLen = 0;   // current number of array elements in difflist
      listPos = 0;

      for( MUSho=0; MUSho<MUShoHitsSec[sec];MUSho++)
	{
	  // loop over shower hits
	  for( Shower=0; Shower<ShowerHitsSec[sec];Shower++)
	    {
	      pphi = ((CorrShowerCord*)(MUShoHit[sec]->At(MUSho)))->getPhi() - ((CorrShowerCord*)(ShowerHit[sec]->At(Shower)))->getPhi();
	      ptheta = ((CorrShowerCord*)(MUShoHit[sec]->At(MUSho)))->getTheta() - ((CorrShowerCord*)(ShowerHit[sec]->At(Shower)))->getTheta();
	      diffSquared = pphi * pphi + ptheta * ptheta;
	      if(diffList->GetSize() == 0)
		listPos = 0;
	      else
		{
		  while((diffSquared > ((CorrShowerDiff*)diffList->At(listPos))->getDiff()) && (listPos < diffList->GetSize()))
		    {
		      listPos++;
		      if(listPos == diffList->GetSize())
			break;
		    }
		}
	      CorrShowerDiff *tc = new CorrShowerDiff(diffSquared, MUSho, Shower);
	      diffList->AddAt(tc, listPos);
	    }
	}
      nonCorrelatedMUSho = MUShoHitsSec[sec];
      nonCorrelatedShower = ShowerHitsSec[sec];
      listPos = 0; listPos2 = 0;

      while ( ( nonCorrelatedMUSho > 0 ) && ( nonCorrelatedShower > 0 ) && ( listPos<diffList->GetSize()) )
	{
	  MUSho = ((CorrShowerDiff *)diffList->At(listPos))->getMU();
	  Shower = ((CorrShowerDiff *)diffList->At(listPos))->getShower();
	  if ( (((CorrShowerCord*)(MUShoHit[sec]->At(MUSho)))->getMF()==0) &&
	       (((CorrShowerCord*)(ShowerHit[sec]->At(Shower)))->getMF()==0) )
	    {
	      hshowerCorrPhi->fill(0,sec,((CorrShowerCord*)(ShowerHit[sec]->At(Shower)))->getPhi(),
				   ((CorrShowerCord*)(MUShoHit[sec]->At(MUSho)))->getPhi());
	      hshowerCorrTheta->fill(0,sec,((CorrShowerCord*)(ShowerHit[sec]->At(Shower)))->getTheta(),
				     ((CorrShowerCord*)(MUShoHit[sec]->At(MUSho)))->getTheta());
	      ((CorrShowerCord*)(MUShoHit[sec]->At(MUSho)))->setMF(1);
	      ((CorrShowerCord*)(ShowerHit[sec]->At(Shower)))->setMF(1);
	      nonCorrelatedMUSho--;
	      nonCorrelatedShower--;
	    }
	  listPos++;
	}
      diffList->Delete();
    }
  for(sec=0;sec<6;sec++)
    {
      MUShoHit[sec]->Delete();
      delete MUShoHit[sec];
      ShowerHit[sec]->Delete();
      delete ShowerHit[sec];
    }
  delete diffList;
}
void HTriggerHistFiller::fillMatchU(HIterator* itermatchu )
{
  HMULeptons* matchu=0;
  Int_t s=0;
  Float_t phi;
  Int_t count[7]; //count number of hits per sector (0-5) and global (6)
  for(Int_t i=0;i<7;i++)
    {
      count[i]=0;
    }
  while ((matchu=(HMULeptons*)itermatchu->Next())!=0)
    {
      phi = matchu->getPhiRich();
      if (matchu->getSector() < 5)  phi -= 60*(matchu->getSector()+1);

      if(hipubutterfly){
	hipubutterfly->getP()->Fill(phi,(matchu->getPhiRich()-matchu->getPhiMeta()));
      }
      if(hmulepmom){
	hmulepmom->getP()->Fill(matchu->getMom());
      }

      s=matchu->getSector();
      iputrendhit[3][s]++;
      iputrendhit[3][6]++;
      count[6]++;
      if((s>=0) && (s<=5))
	{
	  count[s]++;
	}
      else
	{
	  cout << "---trigger: ---fillMatchU: ---Sector out of bounds... should be 0-5 but is: " << s << endl;
	}
    }
  for(Int_t i=0;i<7;i++)
    {
      iputrendevent[3][i]++;
    }

  if(htriggermatchuhitmultdistrib)
    {
      for(Int_t i=0;i<6;i++)
	{
	  htriggermatchuhitmultdistrib->fill(0,i,count[i]);
	}
    }
  if(htriggermultdistrib)
    {
      htriggermultdistrib->fill(0,3,count[6]);
    }
}

void HTriggerHistFiller::fillTrend()
{
  for(Int_t i=0;i<4;i++)
    {
      for(Int_t j=0;j<7;j++)
	{
	  if(iputrendevent[i][j]!=0)
	    {
	      if(hiputrendRich  &&i==0)
		{
		  hiputrendRich->fill(0,j,((Float_t)iputrendhit[i][j])/((Float_t)iputrendevent[i][j]));
		}
	      if(hiputrendTof   &&i==1)
		{
		  // hiputrendTof->fill(0,j,((Float_t)iputrendhit[i][j])/((Float_t)iputrendevent[i][j]));
		  hiputrendTof->fill(0,j,((Float_t)tofLeptons[j])/((Float_t)iputrendevent[i][j]));
		}
	      if(hiputrendShower&&i==2)
		{
		  hiputrendShower->fill(0,j,((Float_t)iputrendhit[i][j])/((Float_t)iputrendevent[i][j]));
		}
	      if(hiputrendMatchU&&i==3)
		{
		  hiputrendMatchU->fill(0,j,((Float_t)iputrendhit[i][j])/((Float_t)iputrendevent[i][j]));
		}
	    }
	}
    }
  if(htofnumbers!=0)
    {
      for(Int_t i=0;i<7;i++)
	{
	  if(iputrendhit[1][i] != 0)
	    {
	      htofnumbers->getP()->SetBinContent( i+1, ((Float_t)tofLeptons[i])/((Float_t)iputrendhit[1][i]) );
	    }
	}
    }
  if(hrichnumbers!=0)
    {
      for(Int_t i=0;i<3;i++)
	{
	  hrichnumbers->getP()->SetBinContent( i+1  , richResult[0][i] );
	  hrichnumbers->getP()->SetBinContent( i+1+3, richResult[1][i] );
	}
    }
  resetCounter();
}
void HTriggerHistFiller::fillTime(UInt_t currentTime)
{
  //cout << "--------------------------------------------------------------------------" << endl;
  //cout << "-----" << timeOld  << " \t: " << currentTime << endl;

  if(timeOld >= currentTime)
    {
      //cout << "hitcounter++ = " << timeHitCounter << endl;
      timeEventCounter++;
    }
  else
    {
      timeOffset = (currentTime - timeOld);
      //cout << "timeOffset = " << timeOffset << endl;
      timeOld    = currentTime;
      if(htriggerTrendTime)
	{
	  //cout<<"fill "<<timeHitCounter<<endl;
	  htriggerTrendTime->fill(0, 0, timeEventCounter);
	  for(Int_t i=0;i<timeOffset-1;i++)
	    {
	      /*cout<<"fill 0"<<endl;*/
	      htriggerTrendTime->fill(0, 0, 0);
	    } // fill zeros for the seconds without events
	}
      timeEventCounter = 1;
    }

  //cout << "-----" << timeOld << " \t: " << currentTime << endl;
  //cout << "--------------------------------------------------------------------------" << endl;
}
void HTriggerHistFiller::fillRichTime(HIterator* iterrich, UInt_t currentTime)
{
  HMatchURich* rich=0;
  Int_t s=0;


  if(ipuTimeOld[0] > currentTime)
    {
      cout << "--- trigger: --fillRichTime ipuTimeOld[0] :" << ipuTimeOld[0] << " > currentTime :" << currentTime << endl;
      cout << "reset ipuTimeOld..." << endl;
      ipuTimeOld[0]=0;
    }
  if(ipuTimeOld[0] == currentTime)
    {

      while ((rich=(HMatchURich *)iterrich->Next())!=0)
	{
	  s=rich->getSegmentId();
	  ipuTimeHitCounter[0][s]++;
	  ipuTimeHitCounter[0][6]++;
	}
      for(Int_t i=0;i<7;i++)
	{
	  ipuTimeEventCounter[0][i]++;
	}
    }
  if(ipuTimeOld[0] < currentTime)
    {
      fillTrendTime(0, (currentTime - ipuTimeOld[0]) );
      ipuTimeOld[0] = currentTime;
      while ((rich=(HMatchURich *)iterrich->Next())!=0)
	{
	  s=rich->getSegmentId();
	  ipuTimeHitCounter[0][s]++;
	  ipuTimeHitCounter[0][6]++;
	}
      for(Int_t i=0;i<7;i++){ipuTimeEventCounter[0][i]++;}
    }
}
void HTriggerHistFiller::fillTofTime(HIterator* itertof, UInt_t currentTime)
{
  HMatchUTof* tof=0;
  Int_t s=0;


  if(ipuTimeOld[1] > currentTime)
    {
      cout << "--- trigger: --fillTofTime ipuTimeOld[1] :" << ipuTimeOld[1] << " > currentTime :" << currentTime << endl;
      cout << "reset ipuTimeOld..." << endl;
      ipuTimeOld[1]=0;
    }
  if(ipuTimeOld[1] == currentTime)
    {

      while ((tof=(HMatchUTof *)itertof->Next())!=0)
	{
	  s=tof->getSector();
	  ipuTimeHitCounter[1][s]++;
	  ipuTimeHitCounter[1][6]++;
	}
      for(Int_t i=0;i<7;i++){ipuTimeEventCounter[1][i]++;}
    }
  if(ipuTimeOld[1] < currentTime)
    {
      fillTrendTime(1, (currentTime - ipuTimeOld[1]) );
      ipuTimeOld[1] = currentTime;
      while ((tof=(HMatchUTof *)itertof->Next())!=0)
	{
	  s=tof->getSector();
	  ipuTimeHitCounter[1][s]++;
	  ipuTimeHitCounter[1][6]++;
	}
      for(Int_t i=0;i<7;i++)
	{
	  ipuTimeEventCounter[1][i]++;
	}
    }
}
void HTriggerHistFiller::fillShowerTime(HIterator* itershower, UInt_t currentTime)
{
  HMatchUShower* shower=0;
  Int_t s=0;


  if(ipuTimeOld[2] > currentTime)
    {
      cout << "--- trigger: --fillShowerTime ipuTimeOld[2] :" << ipuTimeOld[2] << " > currentTime :" << currentTime << endl;
      cout << "reset ipuTimeOld..." << endl;
      ipuTimeOld[2]=0;
    }
  if(ipuTimeOld[2] == currentTime)
    {

      while ((shower=(HMatchUShower *)itershower->Next())!=0)
	{
	  s=shower->getSector();
	  ipuTimeHitCounter[2][s]++;
	  ipuTimeHitCounter[2][6]++;
	}
      for(Int_t i=0;i<7;i++)
	{
	  ipuTimeEventCounter[2][i]++;
	}
    }
  if(ipuTimeOld[2] < currentTime)
    {
      fillTrendTime(2, (currentTime - ipuTimeOld[2]) );
      ipuTimeOld[2] = currentTime;
      while ((shower=(HMatchUShower *)itershower->Next())!=0)
	{
	  s=shower->getSector();
	  ipuTimeHitCounter[2][s]++;
	  ipuTimeHitCounter[2][6]++;
	}
      for(Int_t i=0;i<7;i++)
	{
	  ipuTimeEventCounter[2][i]++;
	}
    }
}
void HTriggerHistFiller::fillMatchUTime(HIterator* itermatchu, UInt_t currentTime)
{
  HMULeptons* matchu=0;
  Int_t s=0;


  if(ipuTimeOld[3] > currentTime)
    {
      cout << "--- trigger: --fillMatchUTime ipuTimeOld[3] :" << ipuTimeOld[3] << " > currentTime :" << currentTime << endl;
      cout << "reset ipuTimeOld..." << endl;
      ipuTimeOld[3]=0;
    }
  if(ipuTimeOld[3] == currentTime)
    {

      while ((matchu=(HMULeptons *)itermatchu->Next())!=0)
	{
	  s=matchu->getSector();
	  ipuTimeHitCounter[3][s]++;
	  ipuTimeHitCounter[3][6]++;
	}
      for(Int_t i=0;i<7;i++)
	{
	  ipuTimeEventCounter[3][i]++;
	}
    }
  if(ipuTimeOld[3] < currentTime)
    {
      fillTrendTime(3, (currentTime - ipuTimeOld[3]) );
      ipuTimeOld[3] = currentTime;
      while ((matchu=(HMULeptons *)itermatchu->Next())!=0)
	{
	  s=matchu->getSector();
	  ipuTimeHitCounter[3][s]++;
	  ipuTimeHitCounter[3][6]++;
	}
      for(Int_t i=0;i<7;i++)
	{
	  ipuTimeEventCounter[3][i]++;
	}
    }
}
void HTriggerHistFiller::fillTrendTime(UInt_t ipu, UInt_t offset)
{
  for(Int_t j=0;j<7;j++)
    {
      if( (ipuTimeEventCounter[ipu][j]!=0) && (offset>=1) )
	{
	  if(hiputrendRichTime  &&ipu==0)
	    {
	      hiputrendRichTime  ->fill( 0, j, ((Float_t)ipuTimeHitCounter[ipu][j]) / ((Float_t)ipuTimeEventCounter[ipu][j]) );
	      for(UInt_t i=0;i<offset-1;i++)
		{
		  hiputrendRichTime  ->fill( 0, j, 0 );
		}
	    }
	  if(hiputrendTof   &&ipu==1)
	    {
	      hiputrendTofTime   ->fill( 0, j, ((Float_t)ipuTimeHitCounter[ipu][j]) / ((Float_t)ipuTimeEventCounter[ipu][j]) );
	      for(UInt_t i=0;i<offset-1;i++)
		{
		  hiputrendTofTime   ->fill( 0, j, 0 );
		}
	    }
	  if(hiputrendShower&&ipu==2)
	    {
	      hiputrendShowerTime->fill( 0, j, ((Float_t)ipuTimeHitCounter[ipu][j]) / ((Float_t)ipuTimeEventCounter[ipu][j]) );
	      for(UInt_t i=0;i<offset-1;i++)
		{
		  hiputrendShowerTime->fill( 0, j, 0 );
		}
	    }
	  if(hiputrendMatchU&&ipu==3)
	    {
	      hiputrendMatchUTime->fill( 0, j, ((Float_t)ipuTimeHitCounter[ipu][j]) / ((Float_t)ipuTimeEventCounter[ipu][j]) );
	      for(UInt_t i=0;i<offset-1;i++)
		{
		  hiputrendMatchUTime->fill( 0, j, 0 );
		}
	    }
	}
    }
  resetSingleIpuTimeCounter(ipu);
}
void   HTriggerHistFiller::putTriggerParams(HParamList* l,TList* histpool)
{
  // Puts all params of HTriggerHistFiller to the parameter list of
  // HParamList (which ist used by the io);

  Text_t mytemp[500];
  l->add("colorsModule"           ,colorsModule    );
  l->add("colorsCuts"             ,colorsCuts      );
  l->add("colorsIPUtrend"         ,colorsIPUtrend  );

  htofA->getText(mytemp);
  l->add("htof"                          ,mytemp,'s',500);

  hiputrendRichA->getText(mytemp);
  l->add("hiputrendRich"                 ,mytemp,'s',500);

  hiputrendTofA->getText(mytemp);
  l->add("hiputrendTof"                  ,mytemp,'s',500);

  hiputrendShowerA->getText(mytemp);
  l->add("hiputrendShower"               ,mytemp,'s',500);

  hiputrendMatchUA->getText(mytemp);
  l->add("hiputrendMatchU"               ,mytemp,'s',500);

  htriggerTrendTimeA->getText(mytemp);
  l->add("htriggerTrendTime"             ,mytemp,'s',500);

  hiputrendRichTimeA->getText(mytemp);
  l->add("hiputrendRichTime"             ,mytemp,'s',500);

  hiputrendTofTimeA->getText(mytemp);
  l->add("hiputrendTofTime"              ,mytemp,'s',500);

  hiputrendShowerTimeA->getText(mytemp);
  l->add("hiputrendShowerTime"           ,mytemp,'s',500);

  hiputrendMatchUTimeA->getText(mytemp);
  l->add("hiputrendMatchUTime"           ,mytemp,'s',500);

  htofnumbersA->getText(mytemp);
  l->add("htofnumbers"                   ,mytemp,'s',500);

  hrichCorrPhiA->getText(mytemp);
  l->add("hrichCorrPhi"                  ,mytemp,'s',500);

  hrichCorrThetaA->getText(mytemp);
  l->add("hrichCorrTheta"                ,mytemp,'s',500);

  htofCorrPhiA->getText(mytemp);
  l->add("htofCorrPhi"                   ,mytemp,'s',500);

  htofCorrThetaA->getText(mytemp);
  l->add("htofCorrTheta"                 ,mytemp,'s',500);

  hshowerCorrPhiA->getText(mytemp);
  l->add("hshowerCorrPhi"                ,mytemp,'s',500);

  hshowerCorrThetaA->getText(mytemp);
  l->add("hshowerCorrTheta"              ,mytemp,'s',500);
  
  hmuCorrPhiA->getText(mytemp);
  l->add("hmuCorrPhi"                    ,mytemp,'s',500);

  hmuCorrThetaA->getText(mytemp);
  l->add("hmuCorrTheta"                  ,mytemp,'s',500);

  hrichnumbersA->getText(mytemp);
  l->add("hrichnumbers"                  ,mytemp,'s',500);

  htriggermultdistribA->getText(mytemp);
  l->add("htriggermultdistrib"           ,mytemp,'s',500);

  htriggerrichringmultdistribA->getText(mytemp);
  l->add("htriggerrichringmultdistrib"   ,mytemp,'s',500);

  htriggertofhitmultdistribA->getText(mytemp);
  l->add("htriggertofhitmultdistrib"     ,mytemp,'s',500);

  htriggershowerhitmultdistribA->getText(mytemp);
  l->add("htriggershowerhitmultdistrib"  ,mytemp,'s',500);

  htriggermatchuhitmultdistribA->getText(mytemp);
  l->add("htriggermatchuhitmultdistrib"  ,mytemp,'s',500);

  hipubutterflyA->getText(mytemp);
  l->add("hipubutterfly"                 ,mytemp,'s',500);

  hmulepmomA->getText(mytemp);
  l->add("hmulepmon"                     ,mytemp,'s',500);
  
  hmuCorrAllA->getText(mytemp);
  l->add("hmucorrall"                    ,mytemp,'s',500);
}
Bool_t HTriggerHistFiller::getTriggerParams(HParamList* l,TList* histpool,TList* activeHists)
{
  if(!(l->fill("colorsModule"  ,&colorsModule)))     return kFALSE;
  if(!(l->fill("colorsCuts"    ,&colorsCuts  )))     return kFALSE;
  if(!(l->fill("colorsIPUtrend",&colorsIPUtrend  ))) return kFALSE;

  Text_t mytemp[500];
  if(!(l->fill("htof",mytemp,500))) {return kFALSE;}
  else
    {
      histpool->Add(htofA=new HMonHistAddon(mytemp));
      if(htofA->getActive()==1)
	{
	  activeHists->Add(htof=new HMonHist(*htofA));
	}
    }
  if(!(l->fill("hiputrendRich",mytemp,500))) {return kFALSE;}
  else
    {
      histpool->Add(hiputrendRichA=new HMonHistAddon(mytemp));
      if(hiputrendRichA->getActive()==1)
	{
	  activeHists->Add(hiputrendRich=new HTrendArray(*hiputrendRichA));
	  for(Int_t row=0;row<1;row++)
	    {
	      for(Int_t sec=0;sec<7;sec++)
		{
		  hiputrendRich->getP(row,sec)->SetLineColor(colorsIPUtrend[sec]);
		}
	    }
	}
    }
  if(!(l->fill("hiputrendTof",mytemp,500))) {return kFALSE;}
  else
    {
      histpool->Add(hiputrendTofA=new HMonHistAddon(mytemp));
      if(hiputrendTofA->getActive()==1)
	{
	  activeHists->Add(hiputrendTof=new HTrendArray(*hiputrendTofA));
	  for(Int_t row=0;row<1;row++)
	    {
	      for(Int_t sec=0;sec<7;sec++)
		{
		  hiputrendTof->getP(row,sec)->SetLineColor(colorsIPUtrend[sec]);
		}
	    }
	}
    }
  if(!(l->fill("hiputrendShower",mytemp,500))) {return kFALSE;}
  else
    {
      histpool->Add(hiputrendShowerA=new HMonHistAddon(mytemp));
      if(hiputrendShowerA->getActive()==1)
	{
	  activeHists->Add(hiputrendShower=new HTrendArray(*hiputrendShowerA));
	  for(Int_t row=0;row<1;row++)
	    {
	      for(Int_t sec=0;sec<7;sec++)
		{
		  hiputrendShower->getP(row,sec)->SetLineColor(colorsIPUtrend[sec]);
		}
	    }
	}
    }
  if(!(l->fill("hiputrendMatchU",mytemp,500))) {return kFALSE;}
  else
    {
      histpool->Add(hiputrendMatchUA=new HMonHistAddon(mytemp));
      if(hiputrendMatchUA->getActive()==1)
	{
	  activeHists->Add(hiputrendMatchU=new HTrendArray(*hiputrendMatchUA));
	  for(Int_t row=0;row<1;row++)
	    {
	      for(Int_t sec=0;sec<7;sec++)
		{
		  hiputrendMatchU->getP(row,sec)->SetLineColor(colorsIPUtrend[sec]);
		}
	    }
	}
    }
  if(!(l->fill("htriggerTrendTime",mytemp,500))) {return kFALSE;}
  else
    {
      histpool->Add(htriggerTrendTimeA=new HMonHistAddon(mytemp));
      if(htriggerTrendTimeA->getActive()==1)
	{
	  activeHists->Add(htriggerTrendTime=new HTrendArray(*htriggerTrendTimeA));
	  for(Int_t sec=0;sec<1;sec++){
	    htriggerTrendTime->getP(0,sec)->SetLineColor(17);
	  }
	}
    }
  if(!(l->fill("hiputrendRichTime",mytemp,500))) {return kFALSE;}
  else
    {
      histpool->Add(hiputrendRichTimeA=new HMonHistAddon(mytemp));
      if(hiputrendRichTimeA->getActive()==1)
	{
	  activeHists->Add(hiputrendRichTime=new HTrendArray(*hiputrendRichTimeA));
	  for(Int_t row=0;row<1;row++)
	    {
	      for(Int_t sec=0;sec<7;sec++)
		{
		  hiputrendRichTime->getP(row,sec)->SetLineColor(colorsIPUtrend[sec]);
		}
	    }
	}
    }
  if(!(l->fill("hiputrendTofTime",mytemp,500))) {return kFALSE;}
  else
    {
      histpool->Add(hiputrendTofTimeA=new HMonHistAddon(mytemp));
      if(hiputrendTofTimeA->getActive()==1)
	{
	  activeHists->Add(hiputrendTofTime=new HTrendArray(*hiputrendTofTimeA));
	  for(Int_t row=0;row<1;row++)
	    {
	      for(Int_t sec=0;sec<7;sec++)
		{
		  hiputrendTofTime->getP(row,sec)->SetLineColor(colorsIPUtrend[sec]);
		}
	    }
	}
    }
  if(!(l->fill("hiputrendShowerTime",mytemp,500))) {return kFALSE;}
  else
    {
      histpool->Add(hiputrendShowerTimeA=new HMonHistAddon(mytemp));
      if(hiputrendShowerTimeA->getActive()==1)
	{
	  activeHists->Add(hiputrendShowerTime=new HTrendArray(*hiputrendShowerTimeA));
	  for(Int_t row=0;row<1;row++)
	    {
	      for(Int_t sec=0;sec<7;sec++)
		{
		  hiputrendShowerTime->getP(row,sec)->SetLineColor(colorsIPUtrend[sec]);
		}
	    }
	}
    }
  if(!(l->fill("hrichCorrPhi",mytemp,500))) {return kFALSE;}
  else
    {
      histpool->Add(hrichCorrPhiA=new HMonHistAddon(mytemp));
      if(hrichCorrPhiA->getActive()==1)
	{
	  activeHists->Add(hrichCorrPhi=new HHistArray2(*hrichCorrPhiA));
	}
    }
  if(!(l->fill("hrichCorrTheta",mytemp,500))) {return kFALSE;}
  else
    {
      histpool->Add(hrichCorrThetaA=new HMonHistAddon(mytemp));
      if(hrichCorrThetaA->getActive()==1)
	{
	  activeHists->Add(hrichCorrTheta=new HHistArray2(*hrichCorrThetaA));
	}
    }
  if(!(l->fill("htofCorrPhi",mytemp,500))) {return kFALSE;}
  else
    {
      histpool->Add(htofCorrPhiA=new HMonHistAddon(mytemp));
      if(htofCorrPhiA->getActive()==1)
	{
	  activeHists->Add(htofCorrPhi=new HHistArray2(*htofCorrPhiA));
	}
    }
  if(!(l->fill("htofCorrTheta",mytemp,500))) {return kFALSE;}
  else
    {
      histpool->Add(htofCorrThetaA=new HMonHistAddon(mytemp));
      if(htofCorrThetaA->getActive()==1)
	{
	  activeHists->Add(htofCorrTheta=new HHistArray2(*htofCorrThetaA));
	}
    }
  if(!(l->fill("hshowerCorrPhi",mytemp,500))) {return kFALSE;}
  else
    {
      histpool->Add(hshowerCorrPhiA=new HMonHistAddon(mytemp));
      if(hshowerCorrPhiA->getActive()==1)
	{
	  activeHists->Add(hshowerCorrPhi=new HHistArray2(*hshowerCorrPhiA));
	}
    }
  if(!(l->fill("hshowerCorrTheta",mytemp,500))) {return kFALSE;}
  else
    {
      histpool->Add(hshowerCorrThetaA=new HMonHistAddon(mytemp));
      if(hshowerCorrThetaA->getActive()==1)
	{
	  activeHists->Add(hshowerCorrTheta=new HHistArray2(*hshowerCorrThetaA));
	}
    }
  if(!(l->fill("hmuCorrPhi",mytemp,500))) {return kFALSE;}
  else
    {
      histpool->Add(hmuCorrPhiA=new HMonHistAddon(mytemp));
      if(hmuCorrPhiA->getActive()==1)
	{
	  activeHists->Add(hmuCorrPhi=new HHistArray2(*hmuCorrPhiA));
	}
    }
  if(!(l->fill("hmuCorrTheta",mytemp,500))) {return kFALSE;}
  else
    {
      histpool->Add(hmuCorrThetaA=new HMonHistAddon(mytemp));
      if(hmuCorrThetaA->getActive()==1)
	{
	  activeHists->Add(hmuCorrTheta=new HHistArray2(*hmuCorrThetaA));
	}
    }
  if(!(l->fill("hiputrendMatchUTime",mytemp,500))) {return kFALSE;}
  else
    {
      histpool->Add(hiputrendMatchUTimeA=new HMonHistAddon(mytemp));
      if(hiputrendMatchUTimeA->getActive()==1)
	{
	  activeHists->Add(hiputrendMatchUTime=new HTrendArray(*hiputrendMatchUTimeA));
	  for(Int_t row=0;row<1;row++)
	    {
	      for(Int_t sec=0;sec<7;sec++)
		{
		  hiputrendMatchUTime->getP(row,sec)->SetLineColor(colorsIPUtrend[sec]);
		}
	    }
	}
    }
  if(!(l->fill("htofnumbers",mytemp,500))) {return kFALSE;}
  else
    {
      histpool->Add(htofnumbersA=new HMonHistAddon(mytemp));
      if(htofnumbersA->getActive()==1)
	{
	  activeHists->Add(htofnumbers=new HMonHist(*htofnumbersA));
	}
    }
  if(!(l->fill("hrichnumbers",mytemp,500))) {return kFALSE;}
  else
    {
      histpool->Add(hrichnumbersA=new HMonHistAddon(mytemp));
      if(hrichnumbersA->getActive()==1)
	{
	  activeHists->Add(hrichnumbers=new HMonHist(*hrichnumbersA));
	}
    }
  if(!(l->fill("htriggermultdistrib",mytemp,500))) {return kFALSE;}
  else
    {
      histpool->Add(htriggermultdistribA=new HMonHistAddon(mytemp));
      if(htriggermultdistribA->getActive()==1)
	{
	  activeHists->Add(htriggermultdistrib=new HHistArray(*htriggermultdistribA));
	  for(Int_t sec=0;sec<4;sec++)
	    {
	      htriggermultdistrib->getP(0,sec)->SetLineColor(17);
	    }
	}
    }
  if(!(l->fill("htriggerrichringmultdistrib",mytemp,500))) {return kFALSE;}
  else
    {
      histpool->Add(htriggerrichringmultdistribA=new HMonHistAddon(mytemp));
      if(htriggerrichringmultdistribA->getActive()==1)
	{
	  activeHists->Add(htriggerrichringmultdistrib=new HHistArray(*htriggerrichringmultdistribA));
	  for(Int_t sec=0;sec<6;sec++)
	    {
	      htriggerrichringmultdistrib->getP(0,sec)->SetLineColor(17);
	    }
	}
    }
  if(!(l->fill("htriggertofhitmultdistrib",mytemp,500))) {return kFALSE;}
  else
    {
      histpool->Add(htriggertofhitmultdistribA=new HMonHistAddon(mytemp));
      if(htriggertofhitmultdistribA->getActive()==1)
	{
	  activeHists->Add(htriggertofhitmultdistrib=new HHistArray(*htriggertofhitmultdistribA));
	  for(Int_t sec=0;sec<6;sec++)
	    {
	      htriggertofhitmultdistrib->getP(0,sec)->SetLineColor(17);
	    }
	}
    }
  if(!(l->fill("htriggershowerhitmultdistrib",mytemp,500))) {return kFALSE;}
  else
    {
      histpool->Add(htriggershowerhitmultdistribA=new HMonHistAddon(mytemp));
      if(htriggershowerhitmultdistribA->getActive()==1)
	{
	  activeHists->Add(htriggershowerhitmultdistrib=new HHistArray(*htriggershowerhitmultdistribA));
	  for(Int_t sec=0;sec<6;sec++)
	    {
	      htriggershowerhitmultdistrib->getP(0,sec)->SetLineColor(17);
	    }
	}
    }
  if(!(l->fill("htriggermatchuhitmultdistrib",mytemp,500))) {return kFALSE;}
  else
    {
      histpool->Add(htriggermatchuhitmultdistribA=new HMonHistAddon(mytemp));
      if(htriggermatchuhitmultdistribA->getActive()==1)
	{
	  activeHists->Add(htriggermatchuhitmultdistrib=new HHistArray(*htriggermatchuhitmultdistribA));
	  for(Int_t sec=0;sec<6;sec++)
	    {
	      htriggermatchuhitmultdistrib->getP(0,sec)->SetLineColor(17);
	    }
	}
    }

  if(!(l->fill("hipubutterfly",mytemp,500))) {return kFALSE;}
  else
    {
      histpool->Add(hipubutterflyA=new HMonHistAddon(mytemp));
      if(hipubutterflyA->getActive()==1)
	{
	  activeHists->Add(hipubutterfly=new HMonHist2(*hipubutterflyA));
	}
    }

  if(!(l->fill("hmulepmom",mytemp,500))) {return kFALSE;}
  else
    {
      histpool->Add(hmulepmomA=new HMonHistAddon(mytemp));
      if(hmulepmomA->getActive()==1)
	{
	  activeHists->Add(hmulepmom=new HMonHist(*hmulepmomA));
	}
    }
  
  if(!(l->fill("hmucorrall",mytemp,500))) {return kFALSE;}
  else
    {
      histpool->Add(hmuCorrAllA=new HMonHistAddon(mytemp));
      if(hmuCorrAllA->getActive()==1)
	{
	  activeHists->Add(hmuCorrAll=new HMonHist2(*hmuCorrAllA));
	}
    }
  return kTRUE;
}
