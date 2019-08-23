//*-- Authors: Tiago Perez & Marcin Wisniowski
//*-- Modified: 10.04.05 filler completly rewritten --- Bjoern Spruck
//*-- Modified: 7 Nov 2005, Fake-Throwout  --- B. Spruck
//*-- Modified: 15.04.2008, PidParticle->PidTrackCand, B. Spruck
//
//_HADES_CLASS_DESCRIPTION
////////////////////////////////////////////////////////////////////////
//
// HHypListFiller
//
// HypListFiller is creating a permutated list for all particle types
// which are needed for the Algorithm. This list is created once at
// init time and then copied to HypComp for every event on execute.
//
// Number of particles in list is limited to 10
//
// Track filter is checking:
// Checks isUsed flag of PidTrackCleaner
// You have to take care yourself that PidTrackCleaner or any other
// task setting isUsed flag is called BEFORE starting Hyp
//
////////////////////////////////////////////////////////////////////////

#include "hhyplistfiller.h"

// the following are needed ONLY for trackcleaner
#include "hrktrackB.h"
#include "hmdctrackgdef.h"

using namespace std;

ClassImp(HHypListFiller)

HHypListFiller::HHypListFiller(const Text_t * name,const Text_t * title):
HReconstructor(name, title)
{
  nPlus = 0;
  nMinus = 0;
  numberOfParticles = 0;
  nPlusReal = 0;
  nMinusReal = 0;
  numberOfRealParticles = 0;
  numberOfPossibleMissing = 0;
  pid_real_array = 0;
  pid_full_array = 0;
  particleArrayPlus.Set(0);
  particleArrayMinus.Set(0);
  particleArrayMissing.Set(0);
}

//-----------------------------------------------------------------
HHypListFiller::HHypListFiller()
{
  nPlus = 0;
  nMinus = 0;
  numberOfParticles = 0;
  nPlusReal = 0;
  nMinusReal = 0;
  numberOfRealParticles = 0;
  numberOfPossibleMissing = 0;
  pid_real_array = 0;
  pid_full_array = 0;
  particleArrayPlus.Set(0);
  particleArrayMinus.Set(0);
  particleArrayMissing.Set(0);
}

//-----------------------------------------------------------------
HHypListFiller::~HHypListFiller(void)
{
}

//-----------------------------------------------------------------
Bool_t HHypListFiller::SetExitList(Int_t e_list)
{
  exitList = e_list;
  return kTRUE;
}

//-----------------------------------------------------------------
Bool_t HHypListFiller::init()
{
  // Before init is called, all the tracks need to be added.
  // Next the number of possible combinations is computed and a pid
  // table with all permutaions is created. Next steps done are to init
  // HypComb, HypList and PidTrackCand Containers

  if (numberOfPossibleMissing == 0) {
    // no missing particle defined? then nothing ("dummy") is missing
    AddOneMissing(HPidPhysicsConstants::pid("dummy"));
  }

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

  //---------- Initialization of HPidTrackCand Container -----------------------
  m_pContItTrackCand = NULL;         // Iterator
  m_pContCatTrackCand = NULL;        // Category

  if ((m_pContCatTrackCand =
       gHades->getCurrentEvent()->getCategory(catPidTrackCand)) == NULL) {
    Error("init", "Cannot get catPidTrackCand cat");
    return kFALSE;
  }
  m_pContItTrackCand = (HIterator *) m_pContCatTrackCand->MakeIterator();
  if (m_pContItTrackCand == NULL) {
    Error("init", "Cannot get catPidTrackCand Iterator");
    return kFALSE;
  }
  //-----------------------------------------------------------------------

  return kTRUE;
}                               // End Bool_t HHypListFiller::init()

Int_t HHypListFiller::execute()
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
  Int_t Charge[HYP_MAX_PARTICLES];
  Int_t numpidtracks[MAX_USER_VALUES];

  HPidTrackCand   *pPidTrackCand = NULL;

  m_pContItTrackCand->Reset();

  Int_t iCount = 0;

  while (((pPidTrackCand = (HPidTrackCand *) m_pContItTrackCand->Next()) != NULL)
         && (counter < HYP_MAX_PARTICLES) && (iCount < HYP_MAX_TRACKCLEAN)) {
    Int_t charge;
    charge=pPidTrackCand->getTrackData()->getPolarity(ALG_RUNGEKUTTA);
    is_valid[iCount]=pPidTrackCand->isFlagBit(HPidTrackCand::kIsUsed);

    if(is_valid[iCount] && charge==0){// recheck filters
      cerr << "valid particle without charge???" << endl;
      is_valid[iCount]=false;
    }

    // do not change!!! is valid is THE criteria used in following steps
    if (!is_valid[iCount++])
      continue;

    Charge[counter] = charge;

    if (Charge[counter] == 1){
      counterPlus++;
    }else if (Charge[counter] == -1){
      counterMinus++;
    }

    counter++;
  }
  //========= Counting Number of ch. positive and negative particles END =====

  // Number of particles in event must be number of particles INCLUDING fakes
  if (counterPlus == nPlus && counterMinus == nMinus) {

    //=========================== Create HHypComb Container ================
    HHypComb       *hypcomb = NULL;

    hypcomb = (HHypComb *) m_pContCatComb->getNewSlot(locDummy, &index_comb);
    if (hypcomb != 0)
      hypcomb =
        new(hypcomb) HHypComb(numberOfCombinations * numberOfPossibleMissing,
                              numberOfRealParticles);
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
      if (Icomb < MAX_USER_VALUES) {
        for (Int_t k = 0; k < numberOfPossibleMissing; k++) {
//          hyplist->setUserValue(FILLER_VALID_PIDTRACKS, numpidtracks[Icomb],
//                                Icomb + k * numberOfCombinations);
          hyplist->setUserValue(FILLER_VALID_PIDTRACKS, numberOfParticles,
                                Icomb + k * numberOfCombinations);
          hyplist->setUserValue(FILLER_MISSING_PID, particleArrayMissing[k],
                                Icomb + k * numberOfCombinations);
        }
      } else {
        Error("wasauchimmer", "mist");
        exit(0);
      }
    }

    //      cout<<"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"<<endl;
    //      hyplist->print();

  }                             //if( counterPlus==nPlus && counterMinus==nMinus )
  else {
    return ERROR_IDX;
  }                             //if( counterPlus==nPlus && counterMinus==nMinus )

  return index_list;
}

Bool_t HHypListFiller::finalize()
{
  // cout<<" \n\n\n HHypListFiller::finalize() \n\n";

  if (pid_real_array) {         // if init was called this is ALWAYS true
    for (Int_t i = 0; i < numberOfCombinations; i++)
      delete[]pid_real_array[i];
    delete[]pid_real_array;
  }

  if (pid_full_array) {         // if init was called this is ALWAYS true
    for (Int_t i = 0; i < numberOfCombinations; i++)
      delete[]pid_full_array[i];
    delete[]pid_full_array;
  }

  return kTRUE;
}                               // End HHypListFiller::finalize()

Bool_t HHypListFiller::AddTrack(Int_t particle)
{
  // Adds a particle type track to filler
  //      must be done before init is called
  Int_t q;

  q = HPidPhysicsConstants::charge(particle);
  if (-1 == q) {
    particleArrayMinus.Set(nMinus + 1);
    particleArrayMinus.AddAt(particle, nMinus);
    nMinus++;
    if (particle != HPidPhysicsConstants::fakeNeg())
      nMinusReal++;
  } else if (1 == q) {
    particleArrayPlus.Set(nPlus + 1);
    particleArrayPlus.AddAt(particle, nPlus);
    nPlus++;
    if (particle != HPidPhysicsConstants::fakePos())
      nPlusReal++;
  } else {
    return kFALSE;
  }

  return kTRUE;
}

Bool_t HHypListFiller::AddOneMissing(Int_t particle)
{
// Adds a missing particle to filler
//      must be done before init is called

  particleArrayMissing.Set(numberOfPossibleMissing + 1);
  particleArrayMissing.AddAt(particle, numberOfPossibleMissing);
  numberOfPossibleMissing++;

  return kTRUE;
}

#if 1
void HHypListFiller::just_a_test_for_debugging(void)
{
  // just_a_test_for_debugging - the name says it
  // -> creates pid_table
  // -> copy to dummy hypcomb
  // -> prints content

  if (numberOfPossibleMissing == 0) {
    cout <<
      "[Debug]\tno missing particle defined? Lets add a \"dummy\" particle!" <<
      endl;
    AddOneMissing(HPidPhysicsConstants::pid("dummy"));
  }

  cout << "[Debug]\tnMinus = " << nMinus << "\n\tnPlus = " << nPlus << endl;

  cout << "[Debug]\tnMinusReal = " << nMinusReal << "\n\tnPlusReal = " <<
    nPlusReal << endl;

  generateNumberOfCombinations();

  cout << "[Debug]\tnumberOfPart = " << numberOfParticles << endl;
  cout << "[Debug]\tnumberOfRealPart = " << numberOfRealParticles << endl;
  cout << "[Debug]\tnumberOfPossibleMissing = " << numberOfPossibleMissing <<
    endl;

  cout << "[Debug]\tnegCombin = " << numberOfNegCombinations <<
    "\n\tposCombin = " << numberOfPosCombinations << "\n\tallCombin = " <<
    numberOfCombinations << endl;

  create_pid_table();

  cout << "--- PID REAL Array ---" << endl;
  for (Int_t i = 0; i < numberOfCombinations; i++) {
    for (Int_t j = 0; j < numberOfRealParticles; j++) {
      cout << "\t" << pid_real_array[i][j];
    }
    cout << endl;
  }
  cout << "--- END ---" << endl;
  cout << "--- PID FULL Array ---" << endl;
  for (Int_t i = 0; i < numberOfCombinations; i++) {
    for (Int_t j = 0; j < numberOfParticles; j++) {
      cout << "\t" << pid_full_array[i][j];
    }
    cout << endl;
  }
  cout << "--- END ---" << endl;

  HHypComb       *hc =
    new HHypComb(numberOfCombinations * numberOfPossibleMissing,
                 numberOfRealParticles);

  fill_pid_table(hc);
  hc->print();
  delete hc;

  // clean up
  if (pid_real_array) {         // if create_pid_table was called this is ALWAYS true
    for (Int_t i = 0; i < numberOfCombinations; i++)
      delete[]pid_real_array[i];
    delete[]pid_real_array;
  }
  if (pid_full_array) {         // if create_pid_table was called this is ALWAYS true
    for (Int_t i = 0; i < numberOfCombinations; i++)
      delete[]pid_full_array[i];
    delete[]pid_full_array;
  }
}
#endif

// -----------------------------------------------------------------------
// ----------       Private methods --------------------------------------
// -----------------------------------------------------------------------
Bool_t HHypListFiller::generateNumberOfCombinations()
{
  // computes number of combinations and number of pos/neg particle combination
  // This function is normally called by init.
  //
  // Mathematics (Bronstein ;-)):
  // Permutation of n elements in m groups with k1 to km elements each
  // P=n! / ( k1! * k2! *...* km! )
  // this is true for positive and negative independently
  // -> everything is done for positive AND negativ particles
  //
  // the following things happen here:
  // 1) sort positive and negative particle IDs
  //   -> now every "difference" between two particles is one additional type
  // 2) now k1! * k2! ... is calculated
  //   -> if particle type changes, loop-last_index is k1, next is k2... last is km
  // 3) result is produkt of ks
  // 3) from these differences calculate number of combinations allcombination/doubles
  // 4) numberofcombinations is numberofpositive times numberofnegative

  numberOfParticles = nMinus + nPlus;
  numberOfRealParticles = nMinusReal + nPlusReal;

  if (numberOfParticles > HYP_MAX_PARTICLES) {
    Error("generateNumberOfCombinations",
          "HHypListFiller: we do not support more than HYP_MAX_PARTICLES particles in one reaction");

    numberOfCombinations = 0;
    //              numberOfPosCombinations=0;// not needed
    //              numberOfNegCombinations=0;// not needed
    return kFALSE;
  }

  Int_t sort;

  // sort positive
  do {
    sort = kFALSE;
    for (Int_t loop = 1; loop < nPlus; loop++) {
      if (particleArrayPlus.At(loop - 1) > particleArrayPlus.At(loop)) {
        Int_t swap;

        swap = particleArrayPlus.At(loop - 1);
        particleArrayPlus.AddAt(particleArrayPlus.At(loop), (loop - 1));
        particleArrayPlus.AddAt(swap, loop);
        sort = kTRUE;
      }
    }
  } while (sort /*==kTRUE*/ );

  // sort negative
  do {
    Int_t swap;

    sort = kFALSE;
    for (Int_t loop = 1; loop < nMinus; loop++) {
      if (particleArrayMinus.At(loop - 1) > particleArrayMinus.At(loop)) {
        swap = particleArrayMinus.At(loop - 1);
        particleArrayMinus.AddAt(particleArrayMinus.At(loop), (loop - 1));
        particleArrayMinus.AddAt(swap, loop);
        sort = kTRUE;
      }
    }
  } while (sort /*==kTRUE*/ );

  // we do not support more than 10 particles in one reaction ... see Charge[] upthepage
  // too lazy to calculate here... just use a table
#if HYP_MAX_PARTICLES!=10
#error "Filler: only HYP_MAX_PARTICLES=10 supported here!!!"
#endif
  static Int_t faculties[11] = {
    1, 1, 2, 6,                 // 0,1,2,3
    24,                         // 4
    120,                        // 5
    720,                        // 6
    5040,                       // 7
    40320,                      // 8
    362880,                     // 9
    3628800                     // 10
  };

  Int_t product_ks_pos = 1;
  Int_t product_ks_neg = 1;
  Int_t last_index;

  // check how many Plus are diff
  last_index = 0;
  for (Int_t loop = 1; loop < nPlus; loop++) {
    if (particleArrayPlus.At(loop - 1) != particleArrayPlus.At(loop)) {
      product_ks_pos *= faculties[loop - last_index];
      last_index = loop;
    }
  }
  product_ks_pos *= faculties[nPlus - last_index];

  // check how many Minus are diff
  last_index = 0;
  for (Int_t loop = 1; loop < nMinus; loop++) {
    if (particleArrayMinus.At(loop - 1) != particleArrayMinus.At(loop)) {
      product_ks_neg *= faculties[loop - last_index];
      last_index = loop;
    }
  }
  product_ks_neg *= faculties[nMinus - last_index];

  // positive comb's times negative comb's
  numberOfNegCombinations = faculties[nMinus] / product_ks_neg;
  numberOfPosCombinations = faculties[nPlus] / product_ks_pos;
  numberOfCombinations = numberOfNegCombinations * numberOfPosCombinations;

  //      cout<<"[Debug]\tnegCombin = "<<numberOfNegCombinations<<"\n\tposCombin = "<<numberOfPosCombinations
  //              <<"\n\tallCombin = "<<numberOfCombinations<<endl;

  return kTRUE;

}                               // End Bool_t HHypListFiller::generateNumberOfCombinations()

void HHypListFiller::PermSwapper(Int_t * array, Int_t a, Int_t b)
{
  // Swap array-values for indizes a,b
  // only used for permutations by FindNextPermutation
  // and PermReverser
  Int_t s;

  s = array[a];
  array[a] = array[b];
  array[b] = s;
}

void HHypListFiller::PermReverser(Int_t * array, Int_t a, Int_t b)
{
  // Swap array-values from indizes a to b, meaning [a]<->[b] [a-1]<->[b-1] ...
  // a MUST be smaller or equal than b or nothing will be done
  // only used for permutations by FindNextPermutation

  while (a < b) {
    PermSwapper(array, a, b);
    a++;
    b--;
  }
}

#if 0
void HHypListFiller::print_array(Int_t * array, Int_t count)
{
  // Output content of permutation array
  // for debugging purpose only
  for (Int_t i = 0; i < count; i++) {
    cout << array[i] << ",";
  }
  cout << endl;
}
#endif

Bool_t HHypListFiller::FindNextPermutation(Int_t * array, Int_t count)
{
  // Finds next permution of array. perms are deivered in right order
  // starting from 11223 to 32211. If last one is reached, return false
  // for the first call, array values MUST be in the right order!!!
  // This is done by init() -> generateNumberOfCombinations
  // Array values can be ANY number... negativ, 0, 1, 9999... dont care about this

  Int_t next = count - 1;

  if (count == 0 || next == 0)
    return (false);             // zero or one elements ... nothing to do

  do {
    // find rightmost element smaller than successor
    Int_t next1 = next;

    next--;
    if (array[next] < array[next1]) {   // swap with rightmost element that's smaller
      Int_t mid = count;

      while (!(array[next] < array[--mid]));
      PermSwapper(array, next, mid);
      PermReverser(array, next1, count - 1);
      return (true);
    }
  } while (next);

  // next==0
  PermReverser(array, 0, count - 1);    // restore to start order

  return (false);
}                               // End Bool_t HHypListFiller::FindNextPermutation

void HHypListFiller::create_pid_table(void)
{
  // This function create all possible permutation for the given
  // particles and stores it into pid_array, which is then copied
  // to hypcomb for every event by fill_pid_table.
  // number of possible combinations must be calculated before by
  // generateNumberOfCombinations ...
  // function is normally called by init

//      cout << "[DEBUG] --- Create PID Table ----" << endl;
//      pid_array=0;// prevents freeing of unalloced memory
  pid_real_array = new Int_t *[numberOfCombinations];
  for (Int_t i = 0; i < numberOfCombinations; i++)
    pid_real_array[i] = new Int_t[numberOfRealParticles];

  pid_full_array = new Int_t *[numberOfCombinations];
  for (Int_t i = 0; i < numberOfCombinations; i++)
    pid_full_array[i] = new Int_t[numberOfParticles];

  // we dont support more than HYP_MAX_PARTICLES particles per event...
  Int_t array_pos[HYP_MAX_PARTICLES], array_neg[HYP_MAX_PARTICLES];

  // copy arrays for permutation
  for (Int_t j = 0; j < nMinus; j++) {
    array_neg[j] = particleArrayMinus.At(j);
  }
  for (Int_t j = 0; j < nPlus; j++) {
    array_pos[j] = particleArrayPlus.At(j);
  }

  // Ordering:
  // first neg is permutated and then copied numofPosCom times
  // then pos is permutated and copied numofNegCom times
  // -2 -1 . 1 1 2
  // -1 -2 . 1 1 2
  // -2 -1 . 1 2 1 + numNeg
  // -1 -2 . 1 2 1
  // -2 -1 . 2 1 1 + numNeg*2
  // -1 -2 . 2 1 1

  Int_t i;

//      cout << "Neg Permutations..." << endl;
  i = 0;
  do {                          // for all negative particles combis
    Int_t i2;

    i2 = i;

    // print_array(array_neg,nMinus);
    // avoid permutating twice ... thus copying neg numOfPosCom times
    // with offset of numberOfNegCombinations
    for (Int_t k = 0; k < numberOfPosCombinations;
         k++, i2 += numberOfNegCombinations) {
      if (i2 > numberOfCombinations) {  // should NEVER happen
        Error("create_pid_table", "HHypListFiller: neg i2 overflow");
        break;
      }
      for (Int_t j = 0; j < nMinus; j++) {
        pid_full_array[i2][(numberOfParticles - 1) - j] = array_neg[j];
      }
    }
    i++;
    if (i > numberOfNegCombinations) {  // should NEVER happen
      Error("create_pid_table", "HHypListFiller: neg i overflow");
      break;
    }
  } while (FindNextPermutation(array_neg, nMinus));     // for all negative particles combis

//      cout << "Found: " << i << " -> ";
//      print_array(array_neg,nMinus);
//      cout << "Last permutation should be identical to first" << endl;

  // crosscheck!!! should NEVER happen
  if (i != numberOfNegCombinations) {
    Error("create_pid_table",
          "crosscheck: numberOfNegCombinations something fishy here; eigther permuations or calculations wrong!");
  }
//      cout << "Pos Permutations..." << endl;
  i = 0;
  do {                          //for all positive particles combis
    Int_t i2;

    // print_array(array_pos,nPlus);
    // avoid permutating twice ... thus copying pos numOfNegCom times
    // with offset ONE but at position i*numberOfNegCombinations
    i2 = i * numberOfNegCombinations;
    for (Int_t k = 0; k < numberOfNegCombinations; k++, i2++) {
      if (i2 > numberOfCombinations) {  // should NEVER happen
        Error("create_pid_table", "HHypListFiller: pos i2 overflow");
        break;
      }
      for (Int_t j = 0; j < nPlus; j++) {
        pid_full_array[i2][j] = array_pos[j];
      }
    }
    i++;
    if (i > numberOfPosCombinations) {  // should NEVER happen
      Error("create_pid_table", "HHypListFiller: pos i overflow");
      break;
    }
  } while (FindNextPermutation(array_pos, nPlus));      //for all positive particles combis

  //      cout << "Found: " << i << " -> ";
  // print_array(array_pos,nPlus);
  //      cout << "Last permutation should be identical to first" << endl;

  // crosscheck!!! should NEVER happen
  if (i != numberOfPosCombinations) {
    Error("create_pid_table",
          "crosscheck: numberOfPosCombinations something fishy here; eigther permuations or calculations wrong!");
  }

  remove_fakes_pid_table();
  //      cout << "[DEBUG] --- Create PID Table END ----" << endl;
}

void HHypListFiller::remove_fakes_pid_table(void)
{
  for (Int_t i = 0; i < numberOfCombinations; i++) {
    Int_t j2;

    j2 = 0;
    for (Int_t j = 0; j < numberOfParticles; j++) {
      if (pid_full_array[i][j] != HPidPhysicsConstants::fakeNeg()
          && pid_full_array[i][j] != HPidPhysicsConstants::fakePos()) {
        pid_real_array[i][j2] = pid_full_array[i][j];
        j2++;
      }
    }
    // crosscheck!!! should NEVER happen
    if (j2 != numberOfRealParticles) {
      Error("remove_fakes_pid_table",
            "[ERROR] crosscheck: number of REAL particles (j2) != numberOfRealParticles");
      cerr <<
        "[ERROR] crosscheck: number of REAL particles (j2) != numberOfRealParticles "
        << j2 << " " << numberOfRealParticles << endl;
    }
  }

#if 0
  cout << "----------------------------------------" << endl;
  for (Int_t i = 0; i < numberOfCombinations; i++) {
    for (Int_t j = 0; j < numberOfParticles; j++) {
      cout << pid_full_array[i][j] << "\t";
    }
    cout << endl;
    for (Int_t j = 0; j < numberOfRealParticles; j++) {
      cout << pid_real_array[i][j] << "\t";
    }
    cout << endl;
  }

  cout << "----------------------------------------" << endl;
#endif
}

void HHypListFiller::fill_pid_table(HHypComb * hypcomb)
{
  for (Int_t i = 0; i < numberOfCombinations; i++) {
    for (Int_t k = 0; k < numberOfPossibleMissing; k++) {
      // Add missing particle ID as variable
      // this has to be done after hyplist is created!
      for (Int_t j = 0; j < numberOfRealParticles; j++) {
        hypcomb->setPid(i + k * numberOfCombinations, j, pid_real_array[i][j]);
      }
    }
  }
}

void HHypListFiller::fill_pid_idx(HHypComb * hypcomb)
{
  Int_t idx = 0;                // idx nnumber of particle in contPidTrackCand
  Int_t          *iPosPlus;     // position of positive Particle in REAL Idx table
  Int_t          *iPosMinus;    // position of negative Particle in REAL Idx table
  Int_t          *iPosPlusF;    // position of positive Particle in FULL Idx table
  Int_t          *iPosMinusF;   // position of negative Particle in FULL Idx table
  HPidTrackCand   *pPidTrackCand = NULL;

	if( numberOfParticles >HYP_MAX_PARTICLES){
		cerr << "!!!!fill_pid_idx: numberOfParticles >HYP_MAX_PARTICLES!!"<<endl;
		exit(0);
	}
	if(pid_full_array==0){
		cerr << "!!!!fill_pid_idx: pid_full_array==0!!"<<endl;
		exit(0);
	}

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

  m_pContItTrackCand->Reset();
  while ((pPidTrackCand = (HPidTrackCand *) m_pContItTrackCand->Next()) != NULL && idx<HYP_MAX_TRACKCLEAN) {
    if (is_valid[idx]) {
      if (pPidTrackCand->getTrackData()->getPolarity(ALG_RUNGEKUTTA) == -1)   // set Idx for negative particle
      {
        for (Int_t i = 0; i < numberOfCombinations; i++) {
          if (pid_full_array[i][(numberOfParticles - 1) - iPosMinusF[i]] !=
              HPidPhysicsConstants::fakeNeg()) {
            for (Int_t k = 0; k < numberOfPossibleMissing; k++) {
              hypcomb->setIdxPidTrackCand(i + k * numberOfCombinations,
                                     (numberOfRealParticles - 1) - iPosMinus[i],
                                     idx);
            }
            iPosMinus[i]++;
          }
          iPosMinusF[i]++;
        }
      } else if (pPidTrackCand->getTrackData()->getPolarity(ALG_RUNGEKUTTA) == 1)     // set Idx for positive particle
      {
        for (Int_t i = 0; i < numberOfCombinations; i++) {
          if (pid_full_array[i][iPosPlusF[i]] !=
              HPidPhysicsConstants::fakePos()) {
            for (Int_t k = 0; k < numberOfPossibleMissing; k++) {
              hypcomb->setIdxPidTrackCand(i + k * numberOfCombinations, iPosPlus[i],
                                     idx);
            }
            iPosPlus[i]++;
          }
          iPosPlusF[i]++;
        }
      } else {// crosscheck, neutrals should not have survived!
				Error("fill_pid_idx", "crosscheck: neutral particles not allowed");
				cerr << "[ERROR] crosscheck: neutral particles not allowed"<< endl;
      }
    }
    idx++;
  }                             // end while

  // crosscheck!!! should NEVER happen
  for (Int_t i = 0; i < numberOfCombinations; i++) {
    if (iPosPlus[i] != nPlusReal) {
      Error("fill_pid_idx", "crosscheck: iPosPlus[i] != nPlusReal");
      cerr << "[ERROR] crosscheck: iPosPlus[i] != nPlusReal "
        << iPosPlus[i] << " " << nPlusReal << endl;
    }
    if (iPosMinus[i] != nMinusReal) {
      Error("fill_pid_idx", "crosscheck: iPosMinus[i] != nMinusReal");
      cerr << "[ERROR] crosscheck: iPosMinus[i] != nMinusReal "
        << iPosMinus[i] << " " << nMinusReal << endl;
    }
    if (iPosPlusF[i] != nPlus) {
      Error("fill_pid_idx", "crosscheck: iPosPlusF[i] != nPlus");
      cerr << "[ERROR] crosscheck: iPosPlusF[i] != nPlus "
        << iPosPlusF[i] << " " << nPlus << endl;
    }
    if (iPosMinusF[i] != nMinus) {
      Error("fill_pid_idx", "crosscheck: iPosMinusF[i] != nMinus");
      cerr << "[ERROR] crosscheck: iPosMinusF[i] != nMinus "
        << iPosMinusF[i] << " " << nMinus << endl;
    }
  }

  delete[]iPosMinus;
  delete[]iPosPlus;
  delete[]iPosMinusF;
  delete[]iPosPlusF;
}

Bool_t HHypListFiller::fill_pid_fprob(HHypComb * hypcomb, Int_t * numpidtracks)
{
  HPidTrackCand   *pPidTrackCand = NULL;

  HCategory      *PidTrackCandCat =
    gHades->getCurrentEvent()->getCategory(catPidTrackCand);

  for (Int_t Icomb = 0; Icomb < numberOfCombinations; Icomb++) {
    Float_t ProbComb = 1.0;

    if (Icomb < MAX_USER_VALUES)
      numpidtracks[Icomb] = 0;
    for (Int_t Ipart = 0; Ipart < numberOfRealParticles; Ipart++) {
      //Int_t Pid = hypcomb->getPid(Icomb, Ipart);
      Int_t Idx = hypcomb->getIdxPidTrackCand(Icomb, Ipart);

      pPidTrackCand = (HPidTrackCand *) PidTrackCandCat->getObject(Idx);

      if (pPidTrackCand == NULL) {
        Error("fill_pid_fprob",
              "[ERROR] HHypListFiller::execute() setProbComb");
        return kFALSE;
      }

      Float_t Prob = 1.0;
			// Probabilities were never used ...
			// future plan is to move this completly to teh algos
			// therfore code is removed here!
/*      Float_t Prob = pPidTrackCand->getWeightForPID(Pid);
      Float_t Pid_from_PID = pPidTrackCand->getPid();

      //cout << "Our Pid " << Pid << " Prob " << Prob << " Pid_from_PID " << Pid_from_PID << endl;

      // To distinguish this to cases (below)
      // Pid_from_PID=-3  Prob(p)=-1  Prob(pi+)=-1     Prob(pi-)=-1   Charge=-1
      // Pid_from_PID= 8  Prob(p)=-1  Prob(pi+)=0.999  Prob(pi-)=-1   Charge= 1
      if (Prob == -1.0 && Pid_from_PID < 0) {
        Prob = 1.0;
      } else if (Prob == -1.0 && Pid_from_PID > 0) {
        Prob = 0.0;
      }*/

      if (Icomb < MAX_USER_VALUES) {
        numpidtracks[Icomb]++;
      }

      ProbComb *= Prob;
    }
    //std::cout << "setting prob " << ProbComb << std::endl;
    if (ProbComb < 0)
      ProbComb = 0.0;           //lower than zero is very strange

    for (Int_t k = 0; k < numberOfPossibleMissing; k++) {
      hypcomb->setProbComb(Icomb + k * numberOfCombinations, ProbComb);
    }
  }                             // end for

  return kTRUE;
}
