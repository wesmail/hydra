// File: hrichpadgraph.h
// ****************************************************************************
//
// HRichPadGraph
//
// Author: Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
// Modified: 1999/12/04 by Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
// ----------------------------------------------------------------------------
//
// ****************************************************************************


#ifndef HRICHPADGRAPH_H
#define HRICHPADGRAPH_H

#include "TObject.h"


// ****************************************************************************

class HRichPadGraph: public TObject  {

// ****************************************************************************

  private:

 Int_t iNodeListLen;
 Int_t iNodeNr;
 Int_t iLinkNodeNr;
 
  public:

 Int_t iLabel;

 HRichPadGraph **pNodeList; //!

 HRichPadGraph();
 virtual ~HRichPadGraph();
 
 HRichPadGraph(const HRichPadGraph& source);
 HRichPadGraph& operator=(const HRichPadGraph& source);

 void InitGraph(Int_t nodenr, Int_t listlen);
 void AddNode(HRichPadGraph* nodeptr);
 Int_t MarkNodes(Int_t labelnr);

 ClassDef(HRichPadGraph,1)
};

//============================================================================


#endif // HRICHPADGRAPH_H
