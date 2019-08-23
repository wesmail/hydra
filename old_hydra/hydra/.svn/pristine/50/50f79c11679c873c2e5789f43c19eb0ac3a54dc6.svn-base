//Macro for merging ntuples
{
 
    char *ntuples[] =
      {
	"raw_qa",
	"PPPipPim_qa"
      };

    char *cFiles[]=
      { 
	"be04042001512_dstgen1_hyp_ntup.root",
	"be04042003124_dstgen1_hyp_ntup.root",
	"be04042004826_dstgen1_hyp_ntup.root"
      };

    TChain *pChain[sizeof(ntuples)/sizeof(char*)];
    for(Int_t i=0;i<sizeof(ntuples)/sizeof(char*);i++)
      pChain[i] = new TChain(ntuples[i]);
    
    for(Int_t i=0;i<sizeof(cFiles)/sizeof(char*);i++)
      for(Int_t j=0;j<sizeof(ntuples)/sizeof(char*);j++)
	pChain[j]->Add(cFiles[i]);
    
    printf("Merging ntpules....\n");
    for(Int_t j=0;j<sizeof(ntuples)/sizeof(char*);j++)
      pChain[j]->Merge(TString(ntuples[j])+TString(".root"));    
}
