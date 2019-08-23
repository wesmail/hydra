//*-- Authors: 

//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////////////
//
// HHypMultiChannelFiller
//
// Please add some statements here
//
////////////////////////////////////////////////////////////////////////

#include "hhypmultichannelfiller.h"

using namespace std;

ClassImp(HHypMultiChannelFiller)

HHypMultiChannelFiller::HHypMultiChannelFiller(Text_t * name, Text_t * title):
HReconstructor(name, title)
{
  nPlus = 0;
  nMinus = 0;
  missing_array = 0;
  pid_real_array = 0;
  numberOfParticles = 0;
  particleArrayPlus.Set(0);
  particleArrayMinus.Set(0);
}

//-----------------------------------------------------------------
HHypMultiChannelFiller::HHypMultiChannelFiller()
{
  nPlus = 0;
  nMinus = 0;
  missing_array = 0;
  pid_real_array = 0;
  numberOfParticles = 0;
  particleArrayPlus.Set(0);
  particleArrayMinus.Set(0);
}

//-----------------------------------------------------------------
HHypMultiChannelFiller::~HHypMultiChannelFiller(void)
{
}

//-----------------------------------------------------------------
Bool_t HHypMultiChannelFiller::SetExitList(Int_t e_list)
{
  exitList = e_list;
  return kTRUE;
}

//-----------------------------------------------------------------
Bool_t HHypMultiChannelFiller::init()
{
  // Purpose: 
  // 1.) Calculate the number of possible combinations
  // 2.) Prepare the pid table with all permutations is created. 
  // 3.) init HypComb, HypList and PidParticle Containers

  generateNumberOfCombinations();
  create_pid_table();

  //---------- Initialization of HHypComb Container -----------------------
  m_pContCatComb = gHades->getCurrentEvent()->getCategory(catHypComb);

  if (!m_pContCatComb) {
    m_pContCatComb = HHypComb::buildLinearCat("HHypComb");

    if (!m_pContCatComb)
      return kFALSE;
    // else
    gHades->getCurrentEvent()->addCategory(catHypComb, m_pContCatComb,
                                           "HypComb");
  }
  //-----------------------------------------------------------------------

  //---------- Initialization of HHypList Container -----------------------
  m_pContCatList = gHades->getCurrentEvent()->getCategory(catHypList);

  if (!m_pContCatList) {
    m_pContCatList = HHypList::buildLinearCat("HHypList");

    if (!m_pContCatList)
      return kFALSE;
    // else
    gHades->getCurrentEvent()->addCategory(catHypList, m_pContCatList,
                                           "HypList");
  }
  //-----------------------------------------------------------------------

  //---------- Initialization of HPidParticle Container -----------------------
  m_pContItPart = NULL;         // Iterator
  m_pContCatPart = NULL;        // Category

  if ((m_pContCatPart =
       gHades->getCurrentEvent()->getCategory(catPidPart)) == NULL) {
    Error("init", "Cannot get catPidPart cat");
    return kFALSE;
  }
  m_pContItPart = (HIterator *) m_pContCatPart->MakeIterator();
  //-----------------------------------------------------------------------
  return kTRUE;
}                               // End Bool_t HHypMultiChannelFiller::init() 

Int_t HHypMultiChannelFiller::execute()
{
  Int_t index_comb = 0;
  Int_t index_list = 0;

  //  Int_t i,j;
  HLocation locDummy;

  // Get PID
  //      Check Nr of Particle
  //    Check Nr of + and -Track
  // Compare NrParticlesPiD with Table ifnot return ERROR_IDX
  // Compare NrParticlesPiD- with Table
  // Compare NrParticlesPiD+ with Table
  //==========================================================================

  //========= Counting Number of ch. positive and negative particles =========
  Int_t counterMinus = 0;
  Int_t counterPlus = 0;
  Int_t counter = 0;
  Int_t Charge[10];
  Int_t numpidtracks[MAX_USER_VALUES];

  HPidParticle   *pidPart = NULL;

  m_pContItPart->Reset();
  while (((pidPart = (HPidParticle *) m_pContItPart->Next()) != NULL)
         && (counter < 10)) {
    Charge[counter] = pidPart->getCharge();

    if (Charge[counter] == 1)
      counterPlus++;
    else if (Charge[counter] == -1)
      counterMinus++;
    counter++;
  }
  //========= Counting Number of ch. positive and negative particles END =====

  // Number of particles in event must be number of particles INCLUDING fakes
  if (counterPlus == nPlus && counterMinus == nMinus) {

    //=========================== Create HHypComb Container ================
    HHypComb       *hypcomb = NULL;

    hypcomb = (HHypComb *) m_pContCatComb->getNewSlot(locDummy, &index_comb);
    if (hypcomb != 0)
      hypcomb = new(hypcomb) HHypComb(numberOfCombinations, numberOfParticles);
    else
      return ERROR_IDX;
    //=========================== Create HHypComb Container END =============

    //========================== Fill PID Table in HHypComb =================
    fill_pid_table(hypcomb);    // just copy from pid_real_array

    //========================== Fill Idx Table in HHypComb =================
    fill_pid_idx(hypcomb);

    //========================== Fill HHypComb: fprobcomb ===================
    if (!fill_pid_fprob(hypcomb, numpidtracks))
      return (ERROR_IDX);

    //=========================== Create HHypList Container ==================
    HHypList       *hyplist = NULL;

    hyplist = (HHypList *) m_pContCatList->getNewSlot(locDummy, &index_list);
    if (hyplist != 0)
      hyplist = new(hyplist) HHypList(index_comb);
    else
      return ERROR_IDX;
    //=========================== Create HHypList Container END ==============

    hyplist->setListId(exitList);

    for (Int_t Icomb = 0; Icomb < numberOfCombinations; Icomb++) {
      //this is to keep PIDTRACK INFO
      if (Icomb < MAX_USER_VALUES)
        hyplist->setUserValue(FILLER_VALID_PIDTRACKS, numpidtracks[Icomb],
                              Icomb);

      hyplist->setUserValue(FILLER_MISSING_PID,missing_array[Icomb],Icomb);
	
    }

    //    cout << "MC filler executed" << endl;

  }                             //if( counterPlus==nPlus && counterMinus==nMinus )
  else {
    return ERROR_IDX;
  }                             //if( counterPlus==nPlus && counterMinus==nMinus )

  return index_list;
}

Bool_t HHypMultiChannelFiller::finalize()
{
  // cout<<" \n\n\n HHypListFiller::finalize() \n\n";

  if (pid_real_array) {         // if init was called this is ALWAYS true
    for (Int_t i = 0; i < numberOfCombinations; i++)
      delete[]pid_real_array[i];
    delete[]pid_real_array;
  }
  if (missing_array)
    delete missing_array;

  return kTRUE;
}                               // End HHypMultiChannelFiller::finalize()

// -----------------------------------------------------------------------
// ----------       Private methods --------------------------------------
// -----------------------------------------------------------------------

Bool_t HHypMultiChannelFiller::generateNumberOfCombinations()
{
  // computes number of combinations and number of pos/neg particle combination
  // This function is normally called by init.
  //      
  // This should be done by the MultiChannel program here

  numberOfParticles = nMinus + nPlus;

  if (numberOfParticles > 10) {
    Error("generateNumberOfCombinations",
          "HHypMultiChannelFiller: we do not support more than 10 particles in one reaction");

    numberOfCombinations = 0;
    //              numberOfPosCombinations=0;// not needed
    //              numberOfNegCombinations=0;// not needed
    return kFALSE;
  }

  Int_t sort;

  numberOfCombinations = 123;   //THIS HAS TO BE CHANGED

  return kTRUE;

}                               // End Bool_t HHypMultiChannelFiller::generateNumberOfCombinations()

void HHypMultiChannelFiller::create_pid_table(void)
{
  // This function create all possible combinations
  // For each combination, wich is one line in the final pid
  // table, the array of negative and postive particles have to be set
  // up. In addition we need one (=exactly one) particle which will be
  // the missing particle

  // Input: nPlus and nMinus (Set from outside)
  // Beam energy -> ToDo

  pid_real_array = new Int_t *[numberOfCombinations];
  for (Int_t i = 0; i < numberOfCombinations; i++)
    pid_real_array[i] = new Int_t[numberOfParticles];

  missing_array = new Int_t[numberOfCombinations];

  // Alexander: Please add your algorithm here#
  // For each combination, the positive particles should appear FIRST
  // e.g. +++-
  // or ++++--

  // The next lines are JUST for testing

  for (Int_t i = 0; i < numberOfCombinations; i++) {

    pid_real_array[i][0] = HPidPhysicsConstants::pid("p");
    pid_real_array[i][1] = HPidPhysicsConstants::pid("p");
    pid_real_array[i][2] = HPidPhysicsConstants::pid("pi+");
    pid_real_array[i][3] = HPidPhysicsConstants::pid("pi-");
    missing_array[i] = HPidPhysicsConstants::pid("pi0");
  }

}

void HHypMultiChannelFiller::fill_pid_table(HHypComb * hypcomb)
{
  for (Int_t i = 0; i < numberOfCombinations; i++) {
    for (Int_t j = 0; j < numberOfParticles; j++) {
      hypcomb->setPid(i, j, pid_real_array[i][j]);
    }
    //    hyplist->setUserValue(FILLER_MISSING_PID,missing_array[i],i);
  }
}

void HHypMultiChannelFiller::fill_pid_idx(HHypComb * hypcomb)
{
  Int_t idx = 0;                // idx nnumber of particle in contPid
  Int_t          *iPosPlus;     // position of positive Particle in REAL Idx table
  Int_t          *iPosMinus;    // position of negative Particle in REAL Idx table
  Int_t          *iPosPlusF;    // position of positive Particle in FULL Idx table
  Int_t          *iPosMinusF;   // position of negative Particle in FULL Idx table
  HPidParticle   *pidPart = NULL;

  iPosPlus = new Int_t[numberOfCombinations];
  iPosMinus = new Int_t[numberOfCombinations];
  iPosPlusF = new Int_t[numberOfCombinations];
  iPosMinusF = new Int_t[numberOfCombinations];

  for (Int_t i = 0; i < numberOfCombinations; i++) {
    iPosPlus[i] = 0;
    iPosMinus[i] = 0;
    iPosPlusF[i] = 0;
    iPosMinusF[i] = 0;
  }

  m_pContItPart->Reset();
  while ((pidPart = (HPidParticle *) m_pContItPart->Next()) != NULL) {

#if 0
    cout << "-------------" << endl;
    cout << "2: " << pidPart->getWeightForPID(2) << endl;
    cout << "3: " << pidPart->getWeightForPID(3) << endl;
    cout << "8: " << pidPart->getWeightForPID(8) << endl;
    cout << "9: " << pidPart->getWeightForPID(9) << endl;
    cout << "14: " << pidPart->getWeightForPID(14) << endl;
#endif

    for (Int_t i = 0; i < numberOfCombinations; i++) {
      if (pidPart->getCharge() == -1) { // set Idx for negative particle
        hypcomb->setIdxPidPart(i, (numberOfParticles - 1) - iPosMinus[i], idx);
        iPosMinus[i]++;
      } else if (pidPart->getCharge() == 1) {   // set Idx for positive particle
        hypcomb->setIdxPidPart(i, iPosPlus[i], idx);
        iPosPlus[i]++;
      }
    }
    idx++;
  }                             // end while

  delete[]iPosMinus;
  delete[]iPosPlus;
  delete[]iPosMinusF;
  delete[]iPosPlusF;
}

Bool_t HHypMultiChannelFiller::fill_pid_fprob(HHypComb * hypcomb,
                                              Int_t * numpidtracks)
{
  HPidParticle   *PidPart = NULL;

  HCategory      *pidpartCat =
    gHades->getCurrentEvent()->getCategory(catPidPart);

  for (Int_t Icomb = 0; Icomb < numberOfCombinations; Icomb++) {
    Float_t ProbComb = 1.0;

    if (Icomb < MAX_USER_VALUES)
      numpidtracks[Icomb] = 0;
    for (Int_t Ipart = 0; Ipart < numberOfParticles; Ipart++) {
      Int_t Pid = hypcomb->getPid(Icomb, Ipart);
      Int_t Idx = hypcomb->getIdxPidPart(Icomb, Ipart);

      PidPart = (HPidParticle *) pidpartCat->getObject(Idx);

      if (PidPart == NULL) {
        Error("fill_pid_fprob",
              "[ERROR] HHypMultiChannelFiller::execute() setProbComb");
        return kFALSE;
      }

      Float_t Prob = PidPart->getWeightForPID(Pid);
      Float_t Pid_from_PID = PidPart->getPid();

      //cout << "Our Pid " << Pid << " Prob " << Prob << " Pid_from_PID " << Pid_from_PID << endl;

      // To distinguish this to cases (below)
      // Pid_from_PID=-3  Prob(p)=-1  Prob(pi+)=-1     Prob(pi-)=-1   Charge=-1
      // Pid_from_PID= 8  Prob(p)=-1  Prob(pi+)=0.999  Prob(pi-)=-1   Charge= 1
      if (Prob == -1.0 && Pid_from_PID < 0) {
        Prob = 1.0;
      } else if (Prob == -1.0 && Pid_from_PID > 0) {
        Prob = 0.0;
      } else if (Icomb < MAX_USER_VALUES) {
        numpidtracks[Icomb]++;
      }

      ProbComb *= Prob;
    }
    //std::cout << "setting prob " << ProbComb << std::endl;
    if (ProbComb < 0)
      ProbComb = 0.0;           //lower than zero is very strange

    hypcomb->setProbComb(Icomb, ProbComb);
  }                             // end for

  return kTRUE;
}
