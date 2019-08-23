#pragma implementation


#include "hhyplistreader.h"

HAnaDST::HAnaDST(const char *pFileName): HSUDummyRec(pFileName)
{
}


//---------------------------------------------------------------
Bool_t HAnaDST::init(void)
{
    setInitOk(kFALSE);

	Idx = new TH2F("Idx","nconb, npart",5,0,5,5,0,5);
	
	cout<<"\n\n  ***********************INIT***********************************\n\n";

    if(openOutFile() == NULL)
    {
         return kFALSE;
    }

    if((pHypList = getCategory(catHypList)) == NULL)
    {
        Error("init", "Cannot get catStartCal cat");
        goto lab_End;
    }

    if((pitHypList = getIterator(catHypList)) == NULL)
    {
        Error("init", "Cannot get catStartCal iterator");
        goto lab_End;
    }

    if(openOutFile() == NULL)
    {
        return kFALSE;
    }

    setInitOk();

lab_End:

    return getInitOk();
}

//---------------------------------------------------------------

Bool_t HAnaDST::finalize(void)
{
	return writeAndCloseOutFile();

	cout<<"\n\n  ***********************FINALIZE***********************************\n\n";
   TFile *f=new TFile("/home/wisnia/hydra/rootfiles/hyplist_out.root","recreate");
   Idx->Write();
   f->Close();
   return kTRUE;

}

//---------------------------------------------------------------

Int_t HAnaDST::execute(void)
{
	cout<<"\n\n  ***********************EXECUTE***********************************\n\n";
    HHypList *pHypCat = NULL;

	pitHypList->Reset();
	while((pHypCat = (HHypList*)pitHypList->Next()) != NULL)
	{    
		for(Int_t i=0; i<pHypCat->getNcomb(); i++ )
		{
		    for(Int_t j=0; j<pHypCat->getNpart(); j++ )
			{
				Int_t IDX = pHypCat->getIdx(i,j);
				//cout<<"Idx("<<i<<","<<j<<") = "<<IDX<<endl;
                Idx->Fill(i,j,IDX);
				//pHypCat->Print();
			}
		}
	}
   return 0;
}
