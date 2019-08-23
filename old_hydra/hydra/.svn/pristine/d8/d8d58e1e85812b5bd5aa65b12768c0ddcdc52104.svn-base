//*-- AUTHOR : Marcin Wisniowski 
//*-- Modified : 09.02.2005 by Marcin Wisniowski 
//*-- Modified : 27/01/2005 by T. Perez 
//*-- Modified : 21/04/2005 by I. Froehlich

//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////////// 
// 
// HHypReconstructor
//
// This is the only one reconstructor for the HYP project
// (at the moment). It controles all algorithms.
//
// Algorithms are attached using the "AddAlgorithm" method
// Each attached algorithm has one input list and one output list
// For a description of the lists see HHypChannel and the wiki book
//
// Before Init() the algorithms are ordered and form a tree
// If a list is not available by another algorithm, the
// reconstructor tries to get is from the DST ("Micro-Hyp-DST")
//
////////////////////////////////////////////////////////////////////// 

using namespace std;

#include "hhypreconstructor.h"
#include "hiterator.h"

ClassImp(HHypReconstructor)

  HHypReconstructor::HHypReconstructor(Text_t * name, Text_t * title,
                                       TString opt)
:HReconstructor(name, title)
{
  numOfAlgorithms = 0;
  beam = NULL;
  hypqa = NULL;
  num_accepted_events = 0;

  for (int i = 0; i < hHypMaxNumOfAlgos; i++) {
    hHypAlgList[i] = NULL;
    histofiles[i] = NULL;
  }
  skip_event_enable = 0;
  has_build_cat = 0;
  if (opt.Contains("skipempty"))
    skip_event_enable = 1;

}

HHypReconstructor::HHypReconstructor(void)
{
  HHypReconstructor("name", "title", "");
}

HHypReconstructor::~HHypReconstructor(void)
{
}

Bool_t HHypReconstructor::AddAlgorithm(const char *in,
                                       HHypBaseAlgorithm * hHypBaseAlgoIn,
                                       const char *out, TFile * file)
{
  Bool_t tmp = AddAlgorithm(in, hHypBaseAlgoIn, out);

  hHypBaseAlgoIn->SetHFile(file);
  if (tmp == kFALSE)
    return tmp;
  histofiles[numOfAlgorithms - 1] = file;
  return tmp;
}

Bool_t HHypReconstructor::AddAlgorithm(const char *in,
                                       HHypBaseAlgorithm * hHypBaseAlgoIn,
                                       const char *out)
{
  Int_t listIn = channel.Get(in);       //get (or set) input channel
  Int_t listOut = channel.Get(out);     //get (or set) output channel

  hHypBaseAlgoIn->SetSourceChannelAlgIdx(UNDEFINED_IDX);
  hHypBaseAlgoIn->SetExitChannelAlgIdx(UNDEFINED_IDX);
  hHypBaseAlgoIn->SetHypChannel(&channel);

  controlMatrix[numOfAlgorithms][3] = UNDEFINED_IDX;

  hHypAlgList[numOfAlgorithms] = hHypBaseAlgoIn;
  controlMatrix[numOfAlgorithms][0] = numOfAlgorithms;
  controlMatrix[numOfAlgorithms][1] = listIn;
  controlMatrix[numOfAlgorithms][2] = listOut;
  hHypBaseAlgoIn->SetExitList(listOut);
  hHypBaseAlgoIn->SetInitList(listIn);

  numOfAlgorithms++;
  if (numOfAlgorithms >= hHypMaxNumOfAlgos) {
    cout << " ERROR" << endl;
    cout << "TOO MANY ALGOS IN HHypReconstructor" << endl;
    cout << "contact Tiago Perez <tiago.perez@exp2.physik.uni-giessen.de>" <<
      endl;
    return kFALSE;
  } else
    return kTRUE;
}                               // END  HHypReconstructor::AddAlgorithm()

Bool_t HHypReconstructor::AddQA(TFile * file, Option_t par[])
{
  // Adding QA algorithms to all known algorithms
  // ntuple output is written to "file"

  Int_t num = numOfAlgorithms;

  hypqa = new HHypQA("QA_raw", par);
  AddAlgorithm("raw", hypqa, "empty", file);
  //always use raw input

  for (int i = 0; i < num; i++) {
    //going through the list of algo's
    //what is the output list?
    Int_t exit_list = controlMatrix[i][2];

    if (exit_list > 0) {        //valid output
      AddAlgorithm(channel.Get(exit_list),
                   new HHypQA("QA", par), "ntuple", file);
      //      cout << "Added HypQA for " << channel.Get(exit_list) << endl;
    }

  }
  return kTRUE;
}

Bool_t HHypReconstructor::init()
{

  cout << "initialization of HHypReconstructor" << endl;

  if ((pParams = (HHypRecPar *) gHades->getRuntimeDb()
       ->getContainer("HypRecPar")) == NULL) {
    Error("init", "Cannot get parameters for HypReconstructor");
    return kFALSE;
  }
  //---------- Initialization of HHypComb Container -----------------------
  m_pContCatComb = gHades->getCurrentEvent()->getCategory(catHypComb);

  if (!m_pContCatComb) {
    m_pContCatComb = HHypComb::buildLinearCat("HHypComb");

    if (!m_pContCatComb)
      return kFALSE;
    else
      gHades->getCurrentEvent()
        ->addCategory(catHypComb, m_pContCatComb, "Hyp");
    has_build_cat = 1;
  }
  //-----------------------------------------------------------------------

  //---------- Initialization of HHypList Container -----------------------
  m_pContCatList = gHades->getCurrentEvent()->getCategory(catHypList);

  if (!m_pContCatList) {
    m_pContCatList = HHypList::buildLinearCat("HHypList");

    if (!m_pContCatList)
      return kFALSE;
    else
      gHades->getCurrentEvent()
        ->addCategory(catHypList, m_pContCatList, "Hyp");
    has_build_cat = 1;
  }
  //-----------------------------------------------------------------------

  //---------- Initialization of HHypKine Container -----------------------
  m_pContCatKine = gHades->getCurrentEvent()->getCategory(catHypKine);

  if (!m_pContCatKine) {
    m_pContCatKine = HHypKine::buildLinearCat("HHypKine");

    if (!m_pContCatKine)
      return kFALSE;
    else
      gHades->getCurrentEvent()
        ->addCategory(catHypKine, m_pContCatKine, "Hyp");
    has_build_cat = 1;
  }
  //-----------------------------------------------------------------------

  Int_t i;

  orderBeforeExecute();

  //cout<<"\n\n\t\t\t0\t1\t2\t3"<<endl;
  for (i = 0; i < numOfAlgorithms; i++) {
    //   cout<<"controlMatrix["<<i<<"]\t"<<controlMatrix[i][0]<<"\t"<<controlMatrix[i][1]<<"\t"
    //   <<controlMatrix[i][2]<<"\t"<<controlMatrix[i][3]<<endl;

    TFile          *new_file = histofiles[i];
    TFile          *current_file = gFile;

    if (new_file) {
      new_file->cd();
    }

    /* init algorithms: very important (Ingo) */

    hHypAlgList[i]->SetBeam(beam);
    hHypAlgList[i]->SetBeamPID(beam_pid);
    hHypAlgList[i]->SetTargetPID(target_pid);
    hHypAlgList[i]->SetParams(pParams);

    if (hHypAlgList[i]->base_init() == kTRUE)
      hHypAlgList[i]->init();

    if (new_file && current_file) {
      current_file->cd();
    }
  }

  if ((pitHypList = (HIterator *) m_pContCatList->MakeIterator()) == NULL) {
    std::cout << "Hyp list gets no iterator..." << std::endl;
    exit(1);

  }

  return kTRUE;
}

Bool_t HHypReconstructor::reinit()
{
  //reinit algorithms to init params

  for (int i = 0; i < numOfAlgorithms; i++)
    if (hHypAlgList[i]->base_reinit() == kTRUE)
      hHypAlgList[i]->reinit();

  return kTRUE;
}

Bool_t HHypReconstructor::finalize()
{

  for (int i = 0; i < numOfAlgorithms; i++) {

    TFile          *new_file = histofiles[i];
    TFile          *current_file = gFile;

    if (new_file) {
      new_file->cd();
    }

    /* fin algorithms: very important (Ingo) */
    if (hHypAlgList[i]->base_finalize() == kTRUE)
      hHypAlgList[i]->finalize();

    if (new_file && current_file) {
      current_file->cd();
    }

  }

  if (hypqa)
    hypqa->Dump();

  if (skip_event_enable) {
    cout << "The Micro-HYP-DST has been filled with " << num_accepted_events <<
      " events" << endl;
  }

  return kTRUE;

}

//PRIVATE
Bool_t HHypReconstructor::orderBeforeExecute()
{
  // We have to order the algos inputed via HHypReconstructor::AddAlgorithm method
  // to ensure that they are executed in the propper order. Otherwise we may try to execute an
  // algo which need as input List(i) before we even have genereted it.
  // 
  // Lets check this until everything is fine....
  //
  // DO NOT TOUCH THIS FUNCTION (I. Froehlich)
  // I GUESS IT IS THE FINAL SOLUTION

  Int_t i, j, orderIn = 0;
  HHypBaseAlgorithm *delme;

  while (0 == orderIn) {        // Quicksort...
    // cout<<"\t[DEBUG] ----------------hello-----------------"<<endl;
    orderIn = 1;
    for (i = 0; i < (numOfAlgorithms - 1); i++) //Loop 1 on the inputs
    {
      for (j = (i + 1); j < (numOfAlgorithms); j++)     //Loop 2 on the outputs
      {
        if ((controlMatrix[i][1] == controlMatrix[j][2]) && (controlMatrix[i][1] > 0)) {        // Exchange order of algos
          //  cout << "exchange " << i<< " with " << j << endl;
          orderIn = 0;
          delme = hHypAlgList[i];
          TFile          *file = histofiles[i];

          hHypAlgList[i] = hHypAlgList[j];
          histofiles[i] = histofiles[j];
          hHypAlgList[j] = delme;
          histofiles[j] = file;
          for (int loop = 0; loop < 4; loop++) {
            Int_t swap;

            swap = controlMatrix[i][loop];
            controlMatrix[i][loop] = controlMatrix[j][loop];
            controlMatrix[j][loop] = swap;
          }
        }
      }
    }
  }

  //now check that input is existing. If not, mark it....
  //if output is existing more than one time, thus must be an error
  Int_t input_counter = 0;

  for (i = 0; i < (numOfAlgorithms); i++)       //Loop 1 on the inputs
  {
    if ((controlMatrix[i][1] < 0))
      input_counter = 1;        //empty input, no problem
    else {
      input_counter = 0;
      for (j = 0; j < i; j++)   //Loop 2 on the outputs
      {
        if ((controlMatrix[i][1] == controlMatrix[j][2])
            && (controlMatrix[i][1] > 0))
          input_counter++;
      }
    }
    if (input_counter == 0)     //no input
    {
      controlMatrix[i][4] = FROM_TREE;
      if (has_build_cat) {
        std::
          cout <<
          "HHypReconstructor::orderBeforeExecute(): Hyp list from tree required, but no category found...(maybe you misspelled the input channel name?)"
          << std::endl;
        exit(1);
      }
    } else if (input_counter == 1)
      controlMatrix[i][4] = CURRENT_EVENT;
    else {
      std::
        cout <<
        "HHypReconstructor::orderBeforeExecute(): More than one input list... (Maybe two algos feeding same output channel?)"
        << std::endl;
      exit(1);
    }
  }

  //THIS IS JUST FOR FUN.....
  std::
    cout << "***********************************************************" <<
    std::endl;
  std::cout << "* TREE structure for the HYP reconstructor " << std::endl;
  std::cout << "* NEW v2 " << std::endl;
  std::
    cout << "***********************************************************" <<
    std::endl;

  for (i = 0; i < (numOfAlgorithms); i++)       //Loop 1 on the inputs
  {
    if (controlMatrix[i][1] < 0)
      Dump(i, TString("* "));
    if (controlMatrix[i][4] == FROM_TREE) {
      {
        std::cout << "* " << channel.Get(controlMatrix[i][1]) << std::endl;
        Dump(i, TString("*    "));
      }
    }
  }
  std::
    cout << "***********************************************************" <<
    std::endl;

  return kTRUE;
}                               // END HHypReconstructor::orderBeforeExecute()

void HHypReconstructor::Dump(Int_t alg, TString space)
{
  Int_t output = controlMatrix[alg][2];

  if (output == channel.Get("ntuple")) {
    std::cout << space << hHypAlgList[alg]->
      GetName() << " --> Ntuple " << std::endl;
    return;
  }
  std::cout << space << hHypAlgList[alg]->GetName() << std::endl;
  for (Int_t i = (alg + 1); i < (numOfAlgorithms); i++) //Loop 1 on the inputs
  {
    if (controlMatrix[i][1] == output) {
      Dump(i, space + TString("   "));
    }
  }
}

Int_t HHypReconstructor::execute()
{
  Int_t i, j;

  Int_t kReturnValue = 0;       //By default continue

  if (skip_event_enable)
    kReturnValue = kSkipEvent;  //...or skip event

  for (i = 0; i < numOfAlgorithms; i++) {
    //cout << "exe " << i << endl;
    // resetting this algo
    hHypAlgList[i]->SetSourceChannelAlgIdx(UNDEFINED_IDX);
    hHypAlgList[i]->SetExitChannelAlgIdx(UNDEFINED_IDX);
    if (controlMatrix[i][4] == CURRENT_EVENT)
      controlMatrix[i][3] = UNDEFINED_IDX;
    else {
      // try to find object from tree
      controlMatrix[i][4] = FROM_TREE;  //reset....

      pitHypList->Reset();
      Int_t idx = 0;
      HHypList       *list = NULL;

      //cout << "looking for " << controlMatrix[i][1] << endl;
      while ((list = (HHypList *) pitHypList->Next()) != NULL) {
        //cout << "found is " << list->getListId() << endl;
        if (list->getListId() == controlMatrix[i][1]) {
          //I found something....
          //std::cout << "found " << idx << std::endl;
          controlMatrix[i][4] = idx;
          list->Restore();
        }
        idx++;
      }
      if (controlMatrix[i][4] == FROM_TREE) {   //bad luck...
        //std::cout << "found nothing"  << std::endl;
        controlMatrix[i][3] = UNDEFINED_IDX;
      }
    }

    Bool_t got_source = kFALSE;

    if (controlMatrix[i][4] == CURRENT_EVENT) {
      if (controlMatrix[i][1] < 0) {    // No source needed
        got_source = kTRUE;
      } else
        for (j = 0; j < i; j++) {
          if ((controlMatrix[i][1] == controlMatrix[j][2])
              // ...Source for This Algo already Exist
              && (controlMatrix[j][3] >= 0))    // ...AND is OK.
          {
            //cout << i << ":" << j << ":" << controlMatrix[i][1]  << ":" <<  controlMatrix[j][2] << endl; 
            //cout << "CURRENT_EVENT is " << controlMatrix[j][3] << endl; 
            hHypAlgList[i]->SetSourceChannelAlgIdx(controlMatrix[j][3]);
            got_source = kTRUE;
          }
        }                       // end for
    }                           //end current event

    else {                      //from tree
      if (controlMatrix[i][4] >= 0)     // we have source 
      {
        hHypAlgList[i]->SetSourceChannelAlgIdx(controlMatrix[i][4]);
        got_source = kTRUE;
        //cout << "got_source" << endl;
      }
    }

    if (got_source) {
      // MM must be below
      //cout << "got_source2" << endl;
      TFile          *new_file = histofiles[controlMatrix[i][0]];
      TFile          *current_file = gFile;

      if (new_file) {
        new_file->cd();
      }

      if ((kTRUE == hHypAlgList[i]->base_execute()) && (kTRUE == hHypAlgList[i]->execute()))    // EXECUTES COMPLETES
      {
        controlMatrix[i][3] = EMPTY_IDX;

        //Check if there should be some output
        if ((controlMatrix[i][2] > 0) && (hHypAlgList[i]->GetList())) {
          // Write cache data to the hyplist 
          hHypAlgList[i]->GetList()->writeUserValues();

          // Here I check if at least one combination of the output is valid
          // Only in that case, EMPTY_IDX is overwritten
          for (int combloop = 0;
               combloop < hHypAlgList[i]->GetList()->getNcomb(); combloop++) {

            if (hHypAlgList[i]->GetList()->getProbAlg(combloop) >= 0) {
              controlMatrix[i][3] = hHypAlgList[i]->GetExitChannelAlgIdx();
              if (hHypAlgList[i]->skip == kFALSE)
                kReturnValue = 0;
            }
          }
        }
      } else                    // EXECUTION FAILS
      {
        //cout<<"\t[Debug] HHypReconstructor::executeInt(): else EXECUTION FAILS "<<endl;
        controlMatrix[i][3] = ERROR_IDX;
      }
      if (new_file) {
        current_file->cd();
      }

    }                           //END got_source

  }

  if (kReturnValue != kSkipEvent)
    num_accepted_events++;
  //cout << "hyp exit with " << kReturnValue << endl;
  return kReturnValue;
}                               // END HHypReconstructor::execute()
