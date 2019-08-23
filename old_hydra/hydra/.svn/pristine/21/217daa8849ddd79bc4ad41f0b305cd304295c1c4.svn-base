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
 *                                                                         *
 *            								   *
 ***************************************************************************/


using namespace std;

#include "hades.h"
#include <TObject.h>
#include <TClass.h>
//#include "hreconstructor.h"

#include <iostream>
#include <iomanip>
#include "hhypreconstructor.h"
#include "hhypbasealgorithm.h"


ClassImp(HHypReconstructor)

HHypReconstructor::HHypReconstructor(void) : numOfAlgorithms(0)
{}
HHypReconstructor::~HHypReconstructor(void)
{}

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
    cout << "---------------------" << endl;
    HHypReconstructor::executeInt();
    cout<<"\t\t\t0\t1\t2\t3"<<endl;
    for (i = 0; i< numOfAlgorithms; i++)
    {
        cout<<"controlMatrix["<<i<<"]\t"<<controlMatrix[i][0]<<"\t"<<controlMatrix[i][1]<<"\t"
        <<controlMatrix[i][2]<<"\t"<<controlMatrix[i][3]<<endl;
    }
    return 1;
}// END HHypReconstructor::execute()

Bool_t HHypReconstructor::init()
{
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


	/* init algorithms: very important (Ingo) */
	hHypAlgList[i]->init();
    }
    return kTRUE;
}

Bool_t HHypReconstructor::reinit()
{
    return kTRUE;
}

Bool_t HHypReconstructor::finalize()
{
    return kTRUE;
}

//PRIVATE
Bool_t HHypReconstructor::orderBeforeExecute()
{
    // We have to order the algos inputed via HHypReconstructor::AddAlgorithm method
    // to ensure that they are executed in the propper order. Otherwise we may try to execute an
    // algo which need as input List(i) before we even have genereted it.
    // Options to solve it:
    // 1. Order Twice : THIS IS USED. 
    //                  MAY BE NOT VERY PERFORMING BUT THERE ARE ONLY %= ALGOS 
    //		1st loop for inputs
    // 		2nd loop for outputs
    // 2. Order "in chains"  -1->1->3->4->X  3->8 -1->5->6 -1->7...
    // 		More complicated et first glance. Can be done if performance could be expected
    // 3  We could also loop over all the not executed algos an execute what is possible.
    // 	I think that is the worse option an forces to loop too much.
    // 	Less elegant of all. ;-)
    //
    // The output of this should be the matrix  explain in HHypReconstructor.h
    //		Matrix[hHypMaxNumOfAlgos][4]:: Alg,TheoInt,TheoOut,RealOut
    Int_t i, orderIn = 0, orderOut = 0;
    HHypBaseAlgorithm * delme;
 
    while (0 == orderIn)
    {// Quicksort...
        orderIn = 1;
        for (i = 0; i < (numOfAlgorithms - 1) ;i ++) //Loop 1 on the inputs
        {
            if ( controlMatrix[i+1][1] < controlMatrix[i][1])
            {// Exchange order of algos
                orderIn = 0;
		delme = hHypAlgList[i];
		hHypAlgList[i]=hHypAlgList[i+1];
		hHypAlgList[i+1]=delme;
		for (int loop=0; loop < 4;loop++) {
		    Int_t swap;
		    swap = controlMatrix[i][loop];
		    controlMatrix[i][loop] = controlMatrix[i+1][loop];
		    controlMatrix[i+1][loop] = swap;
		}
            }
        }
    }

    while (0 == orderOut)
    {// Quicksort
        orderOut = 1;
        for (i = 0; i < (numOfAlgorithms - 1) ;i ++) //Loop 2 on the outputs
        {
            if ( controlMatrix[i+1][2] < controlMatrix[i][2])
            {// Exchange order of algos
              orderOut = 0;
	      delme = hHypAlgList[i];
	      hHypAlgList[i]=hHypAlgList[i+1];
	      hHypAlgList[i+1]=delme;
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

      cout << "executing : " << hHypAlgList[i]->GetName() << endl;

        for (j = 0; j<=i; j++)
        {

	  if (controlMatrix[i][1] < 0 || // No source needed or...
	      (controlMatrix[i][1] == controlMatrix[j][2] 
	       // ...Source for This Algo already Exist
	       && (controlMatrix[j][3]>=0)))  // ...AND is OK.
	    {
	      // set the source idx number
	      hHypAlgList[i]->SetSourceChannelAlgIdx(controlMatrix[j][3]);

	      if (kTRUE == hHypAlgList[controlMatrix[i][0]]->execute()) // EXECUTES COMPLETES
                {
		  controlMatrix[i][3]= hHypAlgList[i]->GetExitChannelAlgIdx();
                }
	      else // EXECUTION FAILS
                {
		  cout<<"\t[Debug] HHypReconstructor::executeInt(): else EXECUTION FAILS "<<endl;
		  controlMatrix[i][3]= ERROR_IDX;
                }
	      break;
            }
        }
	if ((i==j) && (controlMatrix[i][1] >= 0))
	  cout << "WARNING: No source for " << i << endl;
    }
    return kTRUE;
}// END HHypReconstructor::executeInt()


