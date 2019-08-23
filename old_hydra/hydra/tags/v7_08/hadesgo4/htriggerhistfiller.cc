//*-- AUTHOR : J. Markert

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
#include "htofhit.h"
#include "hshowerhitIPU.h"
#include "hiterator.h"
#include "htriggerparshowermap.h"

#include "hades.h"
#include "hevent.h"
#include "hcategory.h"
#include <TList.h>
#include <TArrayI.h>

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
HHistArray2*     HTriggerHistFiller::htofCorrPhi=0;
HHistArray2*     HTriggerHistFiller::htofCorrTheta=0;
HHistArray2*     HTriggerHistFiller::hshowerCorrPhi=0;
HHistArray2*     HTriggerHistFiller::hshowerCorrTheta=0;
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

// Hist Addon Pointers
HMonHistAddon* HTriggerHistFiller::htofA=0;
HMonHistAddon* HTriggerHistFiller::htriggermultdistribA=0;
HMonHistAddon* HTriggerHistFiller::htriggerrichringmultdistribA=0;
HMonHistAddon* HTriggerHistFiller::htriggertofhitmultdistribA=0;
HMonHistAddon* HTriggerHistFiller::htriggershowerhitmultdistribA=0;
HMonHistAddon* HTriggerHistFiller::htriggermatchuhitmultdistribA=0;
HMonHistAddon* HTriggerHistFiller::htofCorrPhiA=0;
HMonHistAddon* HTriggerHistFiller::htofCorrThetaA=0;
HMonHistAddon* HTriggerHistFiller::hshowerCorrPhiA=0;
HMonHistAddon* HTriggerHistFiller::hshowerCorrThetaA=0;
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

ClassImp(HTriggerHistFiller)

HTriggerHistFiller::HTriggerHistFiller(const char* name,const char* title)
    : TNamed(name,title)
{
  colorsModule.Set(2);
  colorsCuts.Set(4);
  colorsIPUtrend.Set(7);
  resetCounter();
  resetTimeCounter();
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

	Int_t sec;						// counter: sector

	Float_t pphi, ptheta;						//used only to fill the number of parameters requested
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
  l->add("htof"                          ,mytemp,500);

  hiputrendRichA->getText(mytemp);
  l->add("hiputrendRich"                 ,mytemp,500);

  hiputrendTofA->getText(mytemp);
  l->add("hiputrendTof"                  ,mytemp,500);

  hiputrendShowerA->getText(mytemp);
  l->add("hiputrendShower"               ,mytemp,500);

  hiputrendMatchUA->getText(mytemp);
  l->add("hiputrendMatchU"               ,mytemp,500);

  htriggerTrendTimeA->getText(mytemp);
  l->add("htriggerTrendTime"             ,mytemp,500);

  hiputrendRichTimeA->getText(mytemp);
  l->add("hiputrendRichTime"             ,mytemp,500);

  hiputrendTofTimeA->getText(mytemp);
  l->add("hiputrendTofTime"              ,mytemp,500);

  hiputrendShowerTimeA->getText(mytemp);
  l->add("hiputrendShowerTime"           ,mytemp,500);

  hiputrendMatchUTimeA->getText(mytemp);
  l->add("hiputrendMatchUTime"           ,mytemp,500);

  htofnumbersA->getText(mytemp);
  l->add("htofnumbers"                   ,mytemp,500);

  htofCorrPhiA->getText(mytemp);
  l->add("htofCorrPhi"                   ,mytemp,500);

  htofCorrThetaA->getText(mytemp);
  l->add("htofCorrTheta"                 ,mytemp,500);

  hshowerCorrPhiA->getText(mytemp);
  l->add("hshowerCorrPhi"                ,mytemp,500);

  hshowerCorrThetaA->getText(mytemp);
  l->add("hshowerCorrTheta"              ,mytemp,500);

  hrichnumbersA->getText(mytemp);
  l->add("hrichnumbers"                  ,mytemp,500);

  htriggermultdistribA->getText(mytemp);
  l->add("htriggermultdistrib"           ,mytemp,500);

  htriggerrichringmultdistribA->getText(mytemp);
  l->add("htriggerrichringmultdistrib"   ,mytemp,500);

  htriggertofhitmultdistribA->getText(mytemp);
  l->add("htriggertofhitmultdistrib"     ,mytemp,500);

  htriggershowerhitmultdistribA->getText(mytemp);
  l->add("htriggershowerhitmultdistrib"  ,mytemp,500);

  htriggermatchuhitmultdistribA->getText(mytemp);
  l->add("htriggermatchuhitmultdistrib"  ,mytemp,500);

  hipubutterflyA->getText(mytemp);
  l->add("hipubutterfly"  ,mytemp,500);

  hmulepmomA->getText(mytemp);
  l->add("hmulepmon"  ,mytemp,500);
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
  return kTRUE;
}
