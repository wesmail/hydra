//*-- Authors: Tiago Perez & Marcin Wisniowski
//*-- Modified: 10.04.05 filler completly rewritten --- Bjoern Spruck
////////////////////////////////////////////////////////////////////////
//
// HHypListFiller
//
// HypListFiller is creating a permutated list for all particle types
// which are needed for the Algorithm. This list is created once at
// init time and then copied to HypComp for every event on execute.
//
////////////////////////////////////////////////////////////////////////

#include "hhyplistfiller.h"

using namespace std;

ClassImp(HHypListFiller)

HHypListFiller::HHypListFiller(Text_t * name, Text_t * title):
HReconstructor(name, title)
{
  nPlus = 0;
  nMinus = 0;
  numberOfParticles = 0;
  particleArrayPlus.Set(0);
  particleArrayMinus.Set(0);
}

//-----------------------------------------------------------------
HHypListFiller::HHypListFiller()
{
  nPlus = 0;
  nMinus = 0;
  numberOfParticles = 0;
  particleArrayPlus.Set(0);
  particleArrayMinus.Set(0);
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
// HypComb, HypList and PidParticle Containers

  // cout<<"initialization of HHypListFiller "<<endl;

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
}                               // End Bool_t HHypListFiller::init() 

Int_t HHypListFiller::execute()
{
  // cout<<"[Debug] HHypListFiller::execute()"<<endl;
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

  HPidParticle *pidPart = NULL;

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

  if (counterPlus == nPlus && counterMinus == nMinus) {
    //       cout << "PLUS: " << counterPlus << " MINUS: " << counterMinus << endl;

    //=========================== Create HHypComb Container ================
    HHypComb *hypcomb = NULL;

    hypcomb = (HHypComb *) m_pContCatComb->getNewSlot(locDummy, &index_comb);
    if (hypcomb != 0)
      hypcomb = new(hypcomb) HHypComb(numberOfCombinations, numberOfParticles);
    else
      return ERROR_IDX;
    //=========================== Create HHypComb Container END =============

    //========================== Fill Idx Table in HHypComb =================
    Int_t idx = 0;              // idx nnumber of particle in contPid
    Int_t iPosPlus = 0;         // position of positive Particle in Idx table
    Int_t iPosMinus = 0;        // position of negative Particle in Idx table

    //cout << "enter PID loop" << endl;

    m_pContItPart->Reset();
    while ((pidPart = (HPidParticle *) m_pContItPart->Next()) != NULL) {

#if 0
      cout << "-------------" << endl;
      cout << "2: " <<  pidPart->getWeightForPID(2) << endl;
      cout << "3: " <<  pidPart->getWeightForPID(3) << endl;
      cout << "8: " <<  pidPart->getWeightForPID(8) << endl;
      cout << "9: " <<  pidPart->getWeightForPID(9) << endl;
      cout << "14: " <<  pidPart->getWeightForPID(14) << endl;
#endif

      if (pidPart->getCharge() == -1)   // set Idx for negative particle
      {
        for (Int_t i = 0; i < numberOfCombinations; i++) {
          hypcomb->setIdxPidPart(i, (numberOfParticles - 1) - iPosMinus, idx);
        }
        iPosMinus++;
      } else if (pidPart->getCharge() == 1)     // set Idx for positive particle
      {
        for (Int_t i = 0; i < numberOfCombinations; i++) {
          hypcomb->setIdxPidPart(i, iPosPlus, idx);
        }
        iPosPlus++;
      }
      idx++;
    }
    //========================== Fill Idx Table in HHypComb END =============

    //========================== Fill PID Table in HHypComb =================

    fill_pid_table(hypcomb);    // just copy from pid_array

    //========================== Fill PID Table in HHypComb END =============

    //========================== Fill HHypComb: fprobcomb ===================
    HPidParticle *PidPart = NULL;

    HCategory *pidpartCat = gHades->getCurrentEvent()->getCategory(catPidPart);

    for (Int_t Icomb = 0; Icomb < numberOfCombinations; Icomb++) {
      Float_t ProbComb = 1.0;

      if (Icomb < MAX_USER_VALUES)
        numpidtracks[Icomb] = 0;
      for (Int_t Ipart = 0; Ipart < numberOfParticles; Ipart++) {
        Int_t Pid = hypcomb->getPid(Icomb, Ipart);
        Int_t Idx = hypcomb->getIdxPidPart(Icomb, Ipart);

        PidPart = (HPidParticle *) pidpartCat->getObject(Idx);

        if (PidPart == NULL) {
          cout << "\n ERROR : HHypListFiller::execute() setProbComb " << endl;
          return ERROR_IDX;
          break;
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
        } else if (Icomb < MAX_USER_VALUES)
          numpidtracks[Icomb]++;

        ProbComb *= Prob;
      }
      //std::cout << "setting prob " << ProbComb << std::endl;
      if (ProbComb < 0)
        ProbComb = 0.0;         //lower than zero is very strange

      hypcomb->setProbComb(Icomb, ProbComb);
    }
    //=========================== Fill HHypComb: fprobcomb END ===============

    //=========================== Create HHypList Container ==================
    HHypList *hyplist = NULL;

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

  if (pid_array) {              // if init was called this is ALWAYS true
    for (Int_t i = 0; i < numberOfCombinations; i++)
      delete[]pid_array[i];
    delete[]pid_array;
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
  } else if (1 == q) {
    particleArrayPlus.Set(nPlus + 1);
    particleArrayPlus.AddAt(particle, nPlus);
    nPlus++;
  } else
    return kFALSE;
  //[Debug]
  /*
     numberOfParticles = nPlus + nMinus;
     cout<<" HHypListFiller::AddTrack(Int_t) Part = "<<particle<<endl;
     cout<<" nMinus = "<<nMinus<<"  nPlus = "<<nPlus
     <<" NumOfPart = "<<numberOfParticles<<endl;
   */
  return kTRUE;
}


#if 0
void HHypListFiller::just_a_test_for_debugging(void)
{
// just_a_test_for_debugging - the name says it
// -> creates pid_table
// -> copy to dummy hypcomb
// -> prints content

  cout << "[Debug]\tnMinus = " << nMinus << "\n\tnPlus = " << nPlus
    << "\n\tnumberOfPart = " << numberOfParticles << endl;

  generateNumberOfCombinations();

  cout << "[Debug]\tnegCombin = " << numberOfNegCombinations <<
    "\n\tposCombin = " << numberOfPosCombinations << "\n\tallCombin = " <<
    numberOfCombinations << endl;

  create_pid_table();

  cout << "--- PID Array ---" << endl;
  for (Int_t i = 0; i < numberOfCombinations; i++) {
    for (Int_t j = 0; j < numberOfParticles; j++) {
      cout << "\t" << pid_array[i][j];
    }
    cout << endl;
  }
  cout << "--- END ---" << endl;

  HHypComb *hc = new HHypComb(numberOfCombinations, numberOfParticles);

  fill_pid_table(hc);
  hc->print();
  delete hc;

  // clean up
  if (pid_array) {              // if create_pid_table was called this is ALWAYS true
    for (Int_t i = 0; i < numberOfCombinations; i++)
      delete[]pid_array[i];
    delete[]pid_array;
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

//   cout<<"[Debug] HHypListFiller::generateNumberOfCombinations()"<<endl;
//   cout<<"[Debug]\tnMinus = "<<nMinus<<"\n\tnPlus = "<<nPlus
//       <<"\n\tnumberOfPart = "<<numberOfParticles<<endl;

//      cout<<"[Debug]\tnMinus = "<<nMinus<<"\n\tnPlus = "<<nPlus
//                      <<"\n\tnumberOfPart = "<<numberOfParticles<<endl;

  if (numberOfParticles > 10) {
    cout <<
      "[Debug] HHypListFiller: we do not support more than 10 particles in one reaction"
      << endl;

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

void HHypListFiller::print_array(Int_t * array, Int_t count)
{
// Output content of permutation array
// for debugging purpose only
  for (Int_t i = 0; i < count; i++) {
    cout << array[i] << ",";
  }
  cout << endl;
}

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
  pid_array = new Int_t *[numberOfCombinations];
  for (Int_t i = 0; i < numberOfCombinations; i++)
    pid_array[i] = new Int_t[numberOfParticles];

  // we dont support more than 10 particles per event...
  Int_t array_pos[10], array_neg[10];

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
      if (i2 > numberOfCombinations) {
        cout << "[ERROR] neg i2 overflow" << endl;
        break;
      }                         // should NEVER happen
      for (Int_t j = 0; j < nMinus; j++) {
        pid_array[i2][(numberOfParticles - 1) - j] = array_neg[j];
//                              hypcomb->setPid(i2, (numberOfParticles-1)-j, array_neg[j] );
      }
    }
    i++;
    if (i > numberOfNegCombinations) {
      cout << "[ERROR] neg i overflow" << endl;
      break;
    }                           // should NEVER happen
  } while (FindNextPermutation(array_neg, nMinus));     // for all negative particles combis

  //      cout << "Found: " << i << " -> ";
  //      print_array(array_neg,nMinus);
//      cout << "Last permutation should be identical to first" << endl;

  // crosscheck!!! should NEVER happen
  if (i != numberOfNegCombinations) {
    cout <<
      "[ERROR] crosscheck: numberOfNegCombinations something fishy here; eigther permuations or calculations wrong "
      << i << endl;
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
      if (i2 > numberOfCombinations) {
        cout << "[ERROR] pos i2 overflow" << endl;
        break;
      }                         // should NEVER happen
      for (Int_t j = 0; j < nPlus; j++) {
        pid_array[i2][j] = array_pos[j];
//                              hypcomb->setPid(i2, j, array_pos[j] );
      }
    }
    i++;
    if (i > numberOfPosCombinations) {
      cout << "[ERROR] pos i overflow" << endl;
      break;
    }                           // should NEVER happen
  } while (FindNextPermutation(array_pos, nPlus));      //for all positive particles combis

  //      cout << "Found: " << i << " -> ";
  // print_array(array_pos,nPlus);
//      cout << "Last permutation should be identical to first" << endl;

  // crosscheck!!! should NEVER happen
  if (i != numberOfPosCombinations) {
    cout <<
      "[ERROR] crosscheck: numberOfPosCombinations something fishy here; eigther permuations or calculations wrong "
      << i << endl;
  }
  //      cout << "[DEBUG] --- Create PID Table END ----" << endl;
}

void HHypListFiller::fill_pid_table(HHypComb * hypcomb)
{
  for (Int_t i = 0; i < numberOfCombinations; i++) {
    for (Int_t j = 0; j < numberOfParticles; j++) {
      hypcomb->setPid(i, j, pid_array[i][j]);
    }
  }

//      hypcomb->print();// Show me
}
