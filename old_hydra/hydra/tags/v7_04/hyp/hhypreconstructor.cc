/***************************************************************************
 *   Author :  Tiago Perez <tiago.perez@exp2.physik.uni-giessen.de>        *
 *             II Physikalisches Institut                                  *
 *             Universitaet Giessen                                        *
 *            								   *
 *   Title  :  HHypReconstructor.cc                                        *
 *   Project:  HYDRA Event Hypothesys  (HADES)                             *
 *   Description:   						           *
 *            								   *
 *   Modified :                                                            *
 *             2005.01.27   T. Perez  OrderBeforeExecute                   *
 *                                                                         *
 *            								   *
 ***************************************************************************/


using namespace std;

// #include "hades.h"
// #include <TObject.h>
// #include <TClass.h>

// #include <iostream>
// #include <iomanip>
#include "hhypreconstructor.h"
//#include "hhypbasealgorithm.h"


ClassImp(HHypReconstructor)

HHypReconstructor::HHypReconstructor(void) : numOfAlgorithms(0)
{
  beam = NULL;
  for (int i=0; i<hHypMaxNumOfAlgos; i++)
    {
      hHypAlgList[i]=NULL;
      histofiles[i]=NULL;
    }
}

HHypReconstructor::~HHypReconstructor(void)
{}

Bool_t HHypReconstructor::AddAlgorithm(Int_t listIn, HHypBaseAlgorithm *hHypBaseAlgoIn, Int_t listOut, TFile * file) {
  Bool_t tmp = AddAlgorithm(listIn, hHypBaseAlgoIn, listOut);
  if (tmp == kFALSE) return tmp;
  histofiles[numOfAlgorithms-1]=file;
  return tmp;
}

Bool_t HHypReconstructor::AddAlgorithm(Int_t listIn, HHypBaseAlgorithm *hHypBaseAlgoIn, Int_t listOut)
{
    hHypBaseAlgoIn->SetSourceChannelAlgIdx(UNDEFINED_IDX);
    hHypBaseAlgoIn->SetExitChannelAlgIdx(UNDEFINED_IDX);
    controlMatrix[numOfAlgorithms][3] = UNDEFINED_IDX;

    hHypAlgList[numOfAlgorithms] = hHypBaseAlgoIn;
    controlMatrix[numOfAlgorithms][0] = numOfAlgorithms;
    controlMatrix[numOfAlgorithms][1] = listIn;
    controlMatrix[numOfAlgorithms][2] = listOut;
    hHypBaseAlgoIn->SetExitList(listOut);
    hHypBaseAlgoIn->SetInitList(listIn);

    numOfAlgorithms ++;
    if (numOfAlgorithms >= hHypMaxNumOfAlgos)
    {
        cout<<" ERROR"<<endl;
        cout<<"TOO MANY ALGOS IN HHypReconstructor"<<endl;
        cout<<"contact Tiago Perez <tiago.perez@exp2.physik.uni-giessen.de>"<<endl;
        return kFALSE;
    }
    else
        return kTRUE;
}  // END  HHypReconstructor::AddAlgorithm()


Int_t HHypReconstructor::execute()
{
    Int_t i;
    //cout << "---------------------" << endl;
    HHypReconstructor::executeInt();


//     cout<<"\t\t\t0\t1\t2\t3"<<endl;
//     for (i = 0; i< numOfAlgorithms; i++)
//     {
//         cout<<"controlMatrix["<<i<<"]\t"<<controlMatrix[i][0]<<"\t"<<controlMatrix[i][1]<<"\t"
//         <<controlMatrix[i][2]<<"\t"<<controlMatrix[i][3]<<endl;
//     }

    return kTRUE;

}// END HHypReconstructor::execute()

Bool_t HHypReconstructor::init()
{

  cout<<"initialization of HHypReconstructor"<<endl;
  
  //---------- Initialization of HHypComb Container -----------------------
  HCategory * m_pContCatComb=gHades->getCurrentEvent()->getCategory(catHypComb);
  
  if (!m_pContCatComb) 
    {
      m_pContCatComb = HHypComb::buildLinearCat("HHypComb");
      
      if (!m_pContCatComb) return kFALSE;
      else gHades->getCurrentEvent()
	     ->addCategory(catHypComb, m_pContCatComb, "HypComb");
    }
  //-----------------------------------------------------------------------
  
  //---------- Initialization of HHypList Container -----------------------
  HCategory * m_pContCatList=gHades->getCurrentEvent()->getCategory(catHypList);
  
  if (!m_pContCatList) 
    {
      m_pContCatList = HHypList::buildLinearCat("HHypList");
      
      if (!m_pContCatList) return kFALSE;
      else gHades->getCurrentEvent()
	     ->addCategory(catHypList, m_pContCatList, "HypList");
    }
  //-----------------------------------------------------------------------
  



    Int_t i;
    cout<<"\t\t\t0\t1\t2\t3"<<endl;
    for (i = 0; i< numOfAlgorithms; i++)
      {
	cout<<"controlMatrix["<<i<<"]\t"<<controlMatrix[i][0]<<"\t"<<controlMatrix[i][1]<<"\t"
	    <<controlMatrix[i][2]<<"\t"<<controlMatrix[i][3]<<endl;
     }
    HHypReconstructor::orderBeforeExecute();
    
    cout<<"\n\n\t\t\t0\t1\t2\t3"<<endl;
    for (i = 0; i< numOfAlgorithms; i++)
    {
         cout<<"controlMatrix["<<i<<"]\t"<<controlMatrix[i][0]<<"\t"<<controlMatrix[i][1]<<"\t"
         <<controlMatrix[i][2]<<"\t"<<controlMatrix[i][3]<<endl;

	TFile * new_file=histofiles[controlMatrix[i][0]];
	TFile * current_file=gFile;
	if (new_file) {
	  new_file->cd();
	}

	/* init algorithms: very important (Ingo) */
	
	hHypAlgList[i]->SetBeam(beam);
	hHypAlgList[i]->init();

	if (new_file && current_file) {
	  current_file->cd();
	}
    }
    return kTRUE;
}

Bool_t HHypReconstructor::reinit()
{
    return kTRUE;
}

Bool_t HHypReconstructor::finalize()
{

  for (int i = 0; i< numOfAlgorithms; i++)
    {
   
      
      TFile * new_file=histofiles[controlMatrix[i][0]];
      TFile * current_file=gFile;
      if (new_file) {
	new_file->cd();
      }
      
      /* fin algorithms: very important (Ingo) */
      hHypAlgList[i]->finalize();


      if (new_file) {
	current_file->cd();
      }

    }
  return kTRUE;

}

//PRIVATE
Bool_t HHypReconstructor::orderBeforeExecute()
{
    // We have to order the algos inputed via HHypReconstructor::AddAlgorithm method
    // to ensure that they are executed in the propper order. Otherwise we may try to execute an
    // algo which need as input List(i) before we even have genereted it.
    // Options to solve it:
    // 1. Order One Loop : THIS IS USED. 
    //		Swaps if input i+1 < input i OR
    // 		         output(i+1)< output(i) AND input i+1 <= input i
    // 2. Order "in chains"  -1->1->3->4->X  3->8 -1->5->6 -1->7...
    // 		More complicated et first glance. Can be done if performance could be expected
    // 3  We could also loop over all the not executed algos an execute what is possible.
    // 	I think that is the worse option an forces to loop too much.
    // 	Less elegant of all. ;-)
    //
    // The output of this should be the matrix  explain in HHypReconstructor.h
    //		Matrix[hHypMaxNumOfAlgos][4]:: Alg,TheoInt,TheoOut,RealOut
    Int_t i, orderIn = 0;
    HHypBaseAlgorithm * delme;
 
    while (0 == orderIn)
    {// Quicksort...
     // cout<<"\t[DEBUG] ----------------hello-----------------"<<endl;
        orderIn = 1;
        for (i = 0; i < (numOfAlgorithms - 1) ;i ++) //Loop 1 on the inputs
        {
	  if ( controlMatrix[i+1][1] < controlMatrix[i][1] ||
	       (controlMatrix[i+1][2] < controlMatrix[i][2] && 
		controlMatrix[i+1][1] <= controlMatrix[i][1]))
	    {// Exchange order of algos
	      orderIn = 0;
	      delme = hHypAlgList[i];
	      TFile * file = histofiles[i];
	      hHypAlgList[i]=hHypAlgList[i+1];
	      histofiles[i]=histofiles[i+1];
	      hHypAlgList[i+1]=delme;
	      histofiles[i+1]=file;
	      for (int loop=0; loop < 4;loop++) {
		Int_t swap;
		swap = controlMatrix[i][loop];
		controlMatrix[i][loop] = controlMatrix[i+1][loop];
		controlMatrix[i+1][loop] = swap;
	      }
	    }
        }
    }
    return kTRUE;
}// END HHypReconstructor::orderBeforeExecute()

Bool_t HHypReconstructor::executeInt()
{
  Int_t i, j;

  for( i = 0;i<numOfAlgorithms ;i++ )
    {
      // resetting this algo
      hHypAlgList[i]->SetSourceChannelAlgIdx(UNDEFINED_IDX);
      hHypAlgList[i]->SetExitChannelAlgIdx(UNDEFINED_IDX);
      controlMatrix[i][3] = UNDEFINED_IDX;

      //cout << "executing : " << hHypAlgList[i]->GetName() << endl;

      for (j = 0; j<=i; j++)
        {
	  if (controlMatrix[i][1] < 0 || // No source needed or...
	      (controlMatrix[i][1] == controlMatrix[j][2] 
	       // ...Source for This Algo already Exist
	       && (controlMatrix[j][3]>=0)))  // ...AND is OK.
	    {
	      //	      cout<<"\t\t[Debug]HHypReconstructor::executeInt()"<<endl;
	      // set the source idx number
	      hHypAlgList[i]->SetSourceChannelAlgIdx(controlMatrix[j][3]);

	      TFile * new_file=histofiles[controlMatrix[i][0]];
	      TFile * current_file=gFile;
	      if (new_file) {
		new_file->cd();
	      }

	      if (kTRUE == hHypAlgList[controlMatrix[i][0]]->execute()) // EXECUTES COMPLETES
                {
		  controlMatrix[i][3]= hHypAlgList[i]->GetExitChannelAlgIdx();
                }
	      else // EXECUTION FAILS
                {
		  //cout<<"\t[Debug] HHypReconstructor::executeInt(): else EXECUTION FAILS "<<endl;
		  controlMatrix[i][3]= ERROR_IDX;
                }
	      if (new_file) {
                current_file->cd();
              }


	      break;
            }
        }
      if ((i==j) && (controlMatrix[i][1] >= 0))
	cout << "WARNING: No source for " << i << endl;
    }
  return kTRUE;
}// END HHypReconstructor::executeInt()


