/**
 * @file   hdstrealdata.h
 * @author Simon Lang
 * @date   Fri May  5 15:26:25 2006
 * 
 * @brief  This Class provides Functions for a DST Production from real Data
 * 
 * $Id: hdstembedding.h,v 1.1 2007-06-25 19:00:39 slang Exp $
 *
 */

#ifndef HDSTEMBEDDING_H
#define HDSTEMBEDDING_H


#include "hdstproduction.h"


class HDstEmbedding : public HDstProduction
{
   protected:
      Int_t richUnpackers[12];
      Int_t mdcUnpackers[24];
      Int_t showerUnpackers[12];
      Int_t tofUnpackers[1];
      Int_t tofinoUnpackers[1];
      Int_t startUnpackers[1];
      Int_t latchUnpackers[1];
      Int_t tboxUnpackers[1];

   public:
      HDstEmbedding();
      ~HDstEmbedding() {};

      void setupInput();
      void setupTaskList();

      ClassDef( HDstEmbedding, 0 ) // Base class used for DST Production from real data with admixture of simulated tracks
};


#endif   // HDSTEMBEDDING_H
