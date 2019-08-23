#ifndef HMDCEXTRACTOR_H
#define HMDCEXTRACTOR_H

#include "hreconstructor.h"
#include "TString.h"

class HCategory;
class HLocation;
class HIterator;
class HMdcTrkCandExtractor;
class HMdcWireInfoExtractor;
class TFile;
class HTree;
class HRecEvent;
class HMdcFunc1;

class HMdcExtractor : public HReconstructor {
protected:
    HMdcTrkCandExtractor* trkcandext;   //! pointer to the trk candidate extractor
    HMdcWireInfoExtractor* wireinfoext; //! pointer to the wireinfo extractor
    HCategory* segCat;             //! pointer to the seg data
    HIterator* iterseg;            //! iterator on seg data.
    HLocation locseg;              //! location in segment category
    HCategory* trkcandCat;         //! pointer to the HMdcTrkCand data
    HIterator* itertrkcand;        //! Iterator to the HMdcTrkCand data

    HCategory* kickCat;            //! pointer to the seg data
    HIterator* iterkick;           //! iterator on kicktrack  data.
    HLocation lockick;             //! location in kicktrack category

    HCategory* pidpartCat;         //! pointer to HPidParticle category
    HIterator* iterpidpart;        //! Iterator to HPidParticle category
    HCategory* pidcandCat;         //! pointer to HPidCandidate category
    HCategory* pidtrackCat;        //! pointer to HPidTrackCand category
    HMdcFunc1* func1;              //! Jaro's helper class for single/double tracks (CPR)
    TString filenameCPR;           //! file name of CPR parameters
    TString conditionCPR;          //! condition or/and of both inner mdc modules probalitty > prob (default OR)
    Bool_t  cpr;                   //! switch on CPR for single/double investigation
    Float_t probSingle;            //! probability threshold for singles
    Int_t cprct[2];                //! counters for singles/doubles

    Int_t counter;
    Int_t lookupseg[500];         //! bookkeeping of segments
    Int_t multseg[500];           //! multiplicity of one segment
    Int_t segctr;                 //! multiplicity of one segment
    HRecEvent* event;             //! copy event

    void  initLookupSeg()  {for(Int_t i=0;i<500;i++)    {lookupseg[i]=-1;multseg[i]=0;}}
    void  resetLookupSeg(){for(Int_t i=0;i<segctr;i++){lookupseg[i]=-1;multseg[i]=0;}segctr=0;}
    Int_t checkMultSeg(Int_t index) {for(Int_t i=0;i<segctr;i++){if(lookupseg[i]==index) return multseg[i];} return 0;}
    Int_t registerSeg (Int_t index)
    {
	for(Int_t i=0;i<segctr;i++)
	{
	    if(lookupseg[i]==index)
	    {
		multseg[i]++;
                return multseg[i];
	    }
	}
	lookupseg[segctr]=index;
	multseg[segctr]++;
        segctr++;
        return multseg[segctr-1];
    }
public:
    HMdcExtractor(void);
    HMdcExtractor(const Text_t* name,const Text_t* title);
    ~HMdcExtractor(void);
    void   setFileNameCPR(TString namefile="");
    void   doCPR(Bool_t ok=kTRUE) {cpr=ok;}
    void   setProbSingle(Float_t prob=0.9) {probSingle=prob;}
    Bool_t init(void);
    void   initVariables();
    void   setParContainers(void);
    Bool_t finalize(void);
    Int_t  execute(void);
    ClassDef(HMdcExtractor,1) // Extracts Mdc data.
};

#endif /* !HMDCEXTRACTOR_H */
