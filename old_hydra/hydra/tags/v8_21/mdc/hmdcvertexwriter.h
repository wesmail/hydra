#ifndef HMDCVERTEXWRITER_H
#define HMDCVERTEXWRITER_H

#include "hreconstructor.h"
#include "TString.h"
#include "TFile.h"
#include "TNtuple.h"


class HMdcVertexWriter : public HReconstructor {
protected:
    TFile*        outfile;           // pointer to root file
    TNtuple*      vertex;            // pointer to vertex ntuple
    TString       filename;          //
    Bool_t        fileNameFromInput; // flag to remember if filename was created from input or outputfile
    Bool_t        isEmbedding;       // flag to check if embedding mode is on
    static Bool_t doSkipNoVertex;    // == kTRUE skip no vertex even if it is not calculated (default: KFALSE)
public:
  HMdcVertexWriter(void);
  HMdcVertexWriter(const Text_t* name,const Text_t* title);
  ~HMdcVertexWriter(void);
  Bool_t init(void);
  Bool_t finalize(void);
  Int_t  execute(void);
  static void setSkipNoVertex(Bool_t skip = kTRUE){doSkipNoVertex = skip; }

  ClassDef(HMdcVertexWriter,0) // Writes the event vertex pointer to ascii file
};

#endif /* !HMDCVERTEXWRITER_H */

