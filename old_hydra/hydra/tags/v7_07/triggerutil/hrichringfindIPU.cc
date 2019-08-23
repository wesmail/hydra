//*-- Author: Alberica Toia
//*-- Modified: Joern Wuestenfeld 05/17/2005
//*-- Version: $Id: hrichringfindIPU.cc,v 1.9 2005-07-15 09:02:50 wuestenf Exp $

#include "hades.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hrichringfindIPU.h"
#include "hrichanalysisIPU.h"
#include "hrichIPUparpattern.h"
#include "hrichIPUparthresholds.h"
#include "hrichIPUparlocmax.h"
#include "hrichhitIPU.h"
#include "htriggersetup.h"
#include "hmessagemgr.h"


//******************************************************************************
//
// HRichRingFindIPU
//
// Rich ring finder part of the IPU emulation.
//
//******************************************************************************

ClassImp(HRichRingFindIPU)
//----------------------------------------------------------------------------

const char* HRichRingFindIPU::checkParamContext(void)
{
  // Arbitrary choice to get the context from fRichIPUParThr!
  //
  // Input Parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   Name of the parameter context.
  if (fRichIPUParThr)
    {
      return fRichIPUParThr->getParamContext();
    }
  else
    {
      return NULL;
    }
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichRingFindIPU::initParContainer(HSpectrometer *spec, HRuntimeDb *rtdb)
{
  // Adds to the runtime database "rtdb" the containers needed by the RICH IPU
  // spec is used to get information of the spectrometer setup.
  //
  // Input parameters:
  //   HSpectrometer *spec
  //     Pointer to the dectector setup
  //   HRuntimeDb *rtdb
  //     Pointer to the HADES runtime database.
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   Returns kTRUE if all parameter containers are found, kFALSE otherwise.
  fRichIPUParPat=(HRichIPUParPattern *)rtdb->getContainer("RichIPUParPattern");
  if (fRichIPUParPat)
    {
      INFO_msg(10,HMessageMgr::DET_TRIGGER,"RichIPU Parameters Pattern initialized");
    }
  else
    {
      ERROR_msg(HMessageMgr::DET_TRIGGER,"RichIPU Parameters Pattern Initialization failed!");
      return kFALSE;
    }

  fRichIPUParThr=(HRichIPUParThresholds *)rtdb->getContainer("RichIPUParThresholds");
  if (fRichIPUParThr)
    {
      INFO_msg(10,HMessageMgr::DET_TRIGGER,"RichIPU Parameters Thresholds initialized");
    }
  else
    {
      ERROR_msg(HMessageMgr::DET_TRIGGER,"RichIPU Parameters Thresholds Initialization failed!");
      return kFALSE;
    }

  fRichIPUParLmax=(HRichIPUParLocMax *)rtdb->getContainer("RichIPUParLocMax");
  if (fRichIPUParLmax)
    {
      INFO_msg(10,HMessageMgr::DET_TRIGGER,"RichIPU Parameters LocMax initialized");
    }
  else
    {
      ERROR_msg(HMessageMgr::DET_TRIGGER,"RichIPU Parameters LocMax Initialization failed!");
      return kFALSE;
    }
  // fRichSetup = (HTriggerSetup *)rtdb->getContainer("TriggerSetup");
// 	if(!fRichSetup)
// 		{
// 			ERROR_msg(HMessageMgr::DET_TRIGGER,"RICH ring finder version (TriggerSetup) not available!");
// 			return kFALSE;
// 		}
  return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
HRichRingFindIPU::HRichRingFindIPU(void)
{
  // Default constructor
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   none
  iRingNr = 0;
  pRings = new TObjArray(2);
  pRings->SetOwner(kTRUE);
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichRingFindIPU::init(HRichAnalysisIPU* showMe)
{
  // Initialize task.
  //
  // Input parameters:
  //   HRichAnalysisIPU *showMe
  //      ?????
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   Returns kTRUE if all parameter container found, kFALSE otherwise..
  SEPERATOR_msg("-",60);
  INFO_msg(10,HMessageMgr::DET_TRIGGER,"INIT of HRichRingFindIPU()");
  HSpectrometer *spec = gHades->getSetup();
  HRuntimeDb *rtdb = gHades->getRuntimeDb();
  HRichDetector * pRichDet = (HRichDetector*)gHades->getSetup()->getDetector("Rich");
  // initialize parameters
  if (pRichDet)
    {
      if(!initParContainer(spec,rtdb))
        {
          return kFALSE;
        }
    }

  if (pRings)pRings->Clear();
  showMe->setRings(pRings);
  SEPERATOR_msg("-",60);
  flag =0;
  version =0;
  return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
HRichRingFindIPU::~HRichRingFindIPU()
{
  // Destructor
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   none
  if (pRings)
    {
      delete pRings;
      pRings = NULL;
    }
}
//============================================================================

//----------------------------------------------------------------------------
HRichRingFindIPU::HRichRingFindIPU(const HRichRingFindIPU& source)
{
  // Copy constructor.
  //   Can not be used, call default constructor instead!
  //
  // Input parameters:
  //   HRichRingFindIPU &source
  //     Pointer to object from which to copy, neglected in this case.
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   none
  ERROR_msg(HMessageMgr::DET_TRIGGER,"HRichRingFindIPU object can not be initialized with values of another object!");
  ERROR_msg(HMessageMgr::DET_TRIGGER,"Default constructor will be called.");
  HRichRingFindIPU();
}
//============================================================================

//----------------------------------------------------------------------------
HRichRingFindIPU& HRichRingFindIPU::operator=(const HRichRingFindIPU& source)
{
  // Operator =
  //  Assigns the values of anothe HRichRingFind object to this one.
  //  Can not be used, due to technical reasons, calls default constructor instead.
  //
  // Input parameters:
  //   const HRichRingFindIPU& source
  //     Pointer to object from which to assign values.
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   Pointer to newly created object, or this object.
  if (this != &source)
  {
    ERROR_msg(HMessageMgr::DET_TRIGGER,"HRichRingFindIPU object can not be assigned!");
    ERROR_msg(HMessageMgr::DET_TRIGGER,"Default constructor will be called.");
    return *(new HRichRingFindIPU());
  }
  return *this;
}
//============================================================================

//----------------------------------------------------------------------------
void HRichRingFindIPU::makeLabelPattern(void)
{
  // Derivation of the label mask from the pattern search mask.
  // The labelling mask corresponds to the ring region of the pattern
  // search mask, where each pad is counted only once
  //
  // Input Parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   none
  Int_t nn=0;
  for (Int_t xx=0; xx < fRichIPUParPat->getNGroups(); xx++)
    { // loop over groups
      if (fRichIPUParPat->getPattern(xx,0,1)==0)
        { // ring region
          for (Int_t yy=1; yy <= fRichIPUParPat->getPattern(xx,0,0); yy++)
            { // loop over group lenght
              Int_t flag2 =0;
              for (Int_t zz=0; zz < fRichIPUParPat->getNPadDimensions(); zz++)
                { // loop over x,y
                  if(flag2==0)
                    {
                      nn++;
                      flag2=1;
                    }
                  if ( (nn>=0) && (nn<65) && (zz>=0) && (zz<2) )
                    {
                      setLabelPattern(nn-1,zz,fRichIPUParPat->getPattern(xx,yy,zz)); // each pad should enter twice
                    }
                  else
                    {
                      ERROR_msg(HMessageMgr::DET_TRIGGER,"ARRAY OUT OF SIZE (you never should read this!!)");
                    }
                }
            }
        }
    }

  label_size=0;
  for (Int_t ii=0; ii<nn; ii++)
  {
    Int_t flag4=0;
    for (Int_t k=0; k<fRichIPUParPat->getNPadDimensions(); k++)
      {
        Int_t flag3=0;
        for (Int_t j=1; j<=ii; j++ )
          {
            // if the pad is already labelled
            if (  (( (getLabelPattern(ii,0)) == (getLabelPattern(ii-j,0))) &&
                  ( (getLabelPattern(ii,1)) == (getLabelPattern(ii-j,1))) )  )
              {
                flag3=1;
              }
          } // j loop
        if (flag3==0)
          {
            // i write the pad into the label patter only the first time it appears
            // (when flag3 = 0)
            if(flag4==0)
              {
                label_size++;
                flag4=1;
              }
            setLabelPattern(label_size-1,k,getLabelPattern(ii,k));
          }
      } // k loop
  } // ii loop
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HRichRingFindIPU::Execute(void)
{
  // Do the job!
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   Returns the number of Rings.

  // i make the label pattern once and forever (only when flag = 0) could also be done during initialization.
  if (flag ==0)
    {
      makeLabelPattern();
      flag=1;
    }

  pRings->Delete();
  pRings->Expand(2);

  // initialisation of the variables to 0
  iRingNr = 0;
  for (lr=0; lr<96;lr++)
    {
      for (lc=0; lc<96;lc++)
        {
          label[lr][lc]=0;
        }
    }
  // loop over the pad plane
  for(int r=0; r<96; r++)
    {
      for(int c=0; c<96; c++)
        {
          if(pads[Sector][r][c])
            {
              //----------------------------- Labeling -------------------------------------
              // potential ring centers are all the pads lying on a ring around any fired pad,
              // weighted by the number of times they have been found

              for(Int_t lnr=0; lnr < label_size; lnr++)
                {
                  lr = r + getLabelPattern(lnr,0);
                  lc = c + getLabelPattern(lnr,1);
                  if (lr>=0&&lr<=95&&lc>=0&&lc<=95 )
                    { // boundaries of the padplane
                      label[lr][lc]++;
                    }
                }
            }
        }
    }
  //----------------------------- Ring Finder -----------------------------------
  RingSearch();
  MaxSearch();

//   for(Int_t i=0;i<pRings->GetEntries();i++)
//   {
//     gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"bevore %d %d",((HRichHitIPU*)pRings->At(i))->getX(), ((HRichHitIPU*)pRings->At(i))->getY());
//   }

  return (iRingNr);
}
//============================================================================


//----------------------------------------------------------------------------
void HRichRingFindIPU::ZeroArray()
{
  // Clear the pads array.
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   none
  for(Int_t i=0; i<6; i++)
    {
      for(Int_t j=0; j<96; j++)
        {
          for (Int_t k=0; k<96; k++)
            {
              pads[i][j][k] = 0;
            }
        }
    }
}
//============================================================================

//----------------------------------------------------------------------------	
void HRichRingFindIPU::RingSearch(void)
{
  //------------------------- Ring Recognition ---------------------------------
  // A pattern (patterCoord) with a ring region and a veto region is applied
  // to each pad found in the labeling process (potential ring center).
  // The ring and veto regions of the pattern consist of groups.
  // In each group it is enough if 1 pad is fired ("logical OR") to consider
  // the group fired.
  // The groups in each region are summed up.
  // A threshold (fRichIPUParThr->getThresholds(re,sec)) is set independently
  // for each of the two regions.
  //
  // version 1: local maximum search on ringsum for ALL pads
  //             (used in hardware until beginning of 04)
  // version 2: local maximum search on ringsum for pads over threshold
  // version 3: combined thresholds
  //
  //  WARNING: hardcoded version = 2
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   none
#warning "The ringfinder version is hardcoded here!"
  version =2;

  Int_t iNumberOfGroups = fRichIPUParPat->getNGroups();
  Int_t iNumberOfRegions = 2;

  Int_t iGroupsInRegion[iNumberOfRegions];
  Int_t px, py;
  Int_t iPadsInGroup;
  Int_t iTotPads;
  Int_t iNumberOfValidRegions;

  Int_t patternHit[iNumberOfGroups];

  // initialisation of centers to 0
  for(Int_t j=0; j<96; j++)
    {
      for(Int_t k=0; k<96; k++)
        { 
          centers[j][k] = 0;
          ringsum[j][k] = 0;
          vetosum[j][k] = 0;
          nPads[j][k] = 0;
        }
    }

  // loop over potential ring centers
  for(lr=0; lr<96; lr++)
    {
      for(lc=0; lc<96; lc++)
        {
          if(label[lr][lc] >0)
            {  // labelling condition

              // initialisation of variables to 0
              iNumberOfValidRegions = 0;

              for (Int_t j=0; j< iNumberOfRegions; j++)
                {
                  iGroupsInRegion[j]=0;
                }
              iTotPads = 0;

              // loop over groups
              for (Int_t i=0; i< iNumberOfGroups; i++){
                patternHit[i] = 0;
                iPadsInGroup = 0;

                // loop over pads in group
                for(Int_t j=1; j<= fRichIPUParPat->getPattern(i,0,0) ; j++)
                  {

                    px = lr + fRichIPUParPat->getPattern(i,j,0);
                    py = lc - fRichIPUParPat->getPattern(i,j,1);
                    if(px>=0 && px <=95 && py>=0 && py<=95)
                      { // boundaries of pad plane
                      if (pads[Sector][px][py])
                        { // if pad is fired

                          iPadsInGroup++; 
                          if ( fRichIPUParPat->getPattern(i,0,1)  == 0 )
                            { // only pads in groups of ring region
                              iTotPads++;                                  // are counted to get Npads
                            }
                        }
                      }
                  } // end of loop over pads in group

                if ( iPadsInGroup > 0 )
                  {    // at least one pad fired (logical OR)
                    iGroupsInRegion[fRichIPUParPat->getPattern(i,0,1)]++;
                    patternHit[i] = 1;
                  }
              } // end of loop over groups

              //cout << "RING FIND: thresholds " << fRichIPUParThr->getThresholds(0,Sector) << " " << fRichIPUParThr->getThresholds(1,Sector) << endl;
              if (version==1)
                {
                  for (Int_t j=0; j< iNumberOfRegions; j++)
                    {
                      // here thresholds are applied independently for ring and veto region
                      if (  (fRichIPUParThr->getThresholds(j,Sector)) >= 0
                            && iGroupsInRegion[j] > (fRichIPUParThr->getThresholds(j,Sector)) ||
                            (fRichIPUParThr->getThresholds(j,Sector)) < 0
                            && iGroupsInRegion[j] < -(fRichIPUParThr->getThresholds(j,Sector)) )
                        {
                          iNumberOfValidRegions++;
                        }
                    }
                  ringsum[lr][lc]  = iGroupsInRegion[0];
                  vetosum[lr][lc]  = iGroupsInRegion[1];

                if ( iNumberOfValidRegions ==  iNumberOfRegions )
                  {  // We  have found a ring candidate!!!!
                    centers[lr][lc] = 1;
                    nPads[lr][lc] = iTotPads/2;
                  }
                }

              else
                {
                  if (version==2)
                    {
                      for (Int_t j=0; j< iNumberOfRegions; j++)
                        {
                          // here thresholds are applied independently for ring and veto region
                          if (  (fRichIPUParThr->getThresholds(j,Sector)) >= 0
                                && iGroupsInRegion[j] > (fRichIPUParThr->getThresholds(j,Sector)) ||
                                (fRichIPUParThr->getThresholds(j,Sector)) < 0
                                && iGroupsInRegion[j] < -(fRichIPUParThr->getThresholds(j,Sector)) )
                            {
                              iNumberOfValidRegions++;
                            }
                        }

                      if ( iNumberOfValidRegions ==  iNumberOfRegions ) {  //We  have found a ring candidate!!!!
                        centers[lr][lc] = 1;
                        nPads[lr][lc] = iTotPads/2;
                        ringsum[lr][lc]  = iGroupsInRegion[0];
                        vetosum[lr][lc]  = iGroupsInRegion[1];
                      }
                    }
                  else
                    {
                      if (version==3)
                        {
                          iNumberOfValidRegions=1;
                          // here thresholds are applied together for ring and veto region
                          if (iGroupsInRegion[0]-iGroupsInRegion[1] > fRichIPUParThr->getThresholds(0,Sector))
                            {
                              iNumberOfValidRegions++;
                            }

                          if ( iNumberOfValidRegions ==  iNumberOfRegions )
                            {  // We  have found a ring candidate!!!!
                              centers[lr][lc] = 1;
                              nPads[lr][lc] = iTotPads/2;
                              ringsum[lr][lc]  = iGroupsInRegion[0];
                              vetosum[lr][lc]  = iGroupsInRegion[1];
                            }
                        }
                    }

                }
            }
        }
    }
}
//============================================================================


//----------------------------------------------------------------------------

void HRichRingFindIPU::MaxSearch(void)
{
  //------------------------- Local Max Search ---------------------------------
  // The ring search algo is likely to identify one or more neighbouring pads
  // as the center of a potential ring candidate.
  // Therefore, a local maximum search over 4/ 8 directly neighbouring
  // pads is performed to determine the center.   
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   none

  // loop over found ring centers
  for(lr=0; lr<96; lr++)
    {
      for(lc=0; lc<96; lc++)
        {
          if(centers[lr][lc] >0)
            {
              // search over 4 or 8 neighbouring pads
              Int_t fulfill=0;
              if  (fRichIPUParLmax->getLocMaxFlag()==1)
                {
                  if ( (fRichIPUParLmax->getLocMax()==4)||(fRichIPUParLmax->getLocMax()==8) )
                    {
                      if(   ((version==1)&&
                            ( ringsum[lr][lc] >= ringsum[lr+1][lc] &&
                              ringsum[lr][lc] >= ringsum[lr][lc+1] &&
                              ringsum[lr][lc] >  ringsum[lr-1][lc] &&
                              ringsum[lr][lc] >  ringsum[lr][lc-1]   ) ) ||
                            ((version==2)&&
                            ( ringsum[lr][lc] > ringsum[lr+1][lc] &&
                              ringsum[lr][lc] >= ringsum[lr][lc+1] &&
                              ringsum[lr][lc] >=  ringsum[lr-1][lc] &&
                              ringsum[lr][lc] >  ringsum[lr][lc-1]   ) ) )
                        {

                          if (fRichIPUParLmax->getLocMax()==8)
                            {
                              if( ringsum[lr][lc] >= ringsum[lr+1][lc+1] &&
                                  ringsum[lr][lc] >= ringsum[lr-1][lc+1] &&
                                  ringsum[lr][lc] >  ringsum[lr-1][lc-1] &&
                                  ringsum[lr][lc] >  ringsum[lr+1][lc-1] )
                                {
                                  fulfill=1;
                                }
                            }
                          else
                            {
                              fulfill=1;
                            }
                        }
                    }
                  else
                    {
                      ERROR_msg(HMessageMgr::DET_TRIGGER,"wrong parameter value!!!");
                    }
                }
              else
                {
                  fulfill=1;
                }

              if ( fulfill==1 )
                { // fill the ring container

                  fillHit(lr,lc,ringsum[lr][lc],vetosum[lr][lc],nPads[lr][lc]);
/*                  HRichHitIPU  *ring = new HRichHitIPU;
                  if(ring == NULL)
                    {
                      ERROR_msg(HMessageMgr::DET_TRIGGER,"No entry at the position!");
                      continue;
                    }
                  if ( iRingNr < 4999 )
                    {
                      iRingNr++;
                    }
                  else
                    {
                      cout << "iRingNr reached MAXIMUM!!!" << endl;
                    }
                  // now i store the ring info
                  cout << lc << " " << lr << endl;
                  ring->setX(lc);
                  ring->setY(lr);
                  ring->setQuality(ringsum[lr][lc]);
                  ring->setVetoQuality(vetosum[lr][lc]);
                  ring->setNPads(nPads[lr][lc]);
                  pRings->AddLast(ring);*/
                }
            }
        }
    }
}
//============================================================================
void HRichRingFindIPU::fillHit(Int_t row, Int_t column, Int_t quality, Int_t vetoQuality, Int_t nPads)
{
  HRichHitIPU  *ring = new HRichHitIPU;
  if(ring == NULL)
  {
    ERROR_msg(HMessageMgr::DET_TRIGGER,"No entry at the position!");
    return;
  }
  if ( iRingNr < 4999 )
  {
    iRingNr++;
  }
  else
  {
    INFO_msg(10,HMessageMgr::DET_TRIGGER,"iRingNr reached MAXIMUM!!!");
  }
  // now i store the ring info
  ring->setX(column);
  ring->setY(row);
  ring->setQuality(quality);
  ring->setVetoQuality(vetoQuality);
  ring->setNPads(nPads);
  pRings->AddLast(ring);
}
