/**
 * @file   hdstrealdata.h
 * @author Simon Lang
 * @date   Fri May  5 15:26:25 2006
 * 
 * @brief  This Class provides Functions for a DST Production from real Data
 * 
 * $Id: hdstrealdata.h,v 1.3 2007-02-24 17:46:00 jurkovic Exp $
 *
 */

#ifndef HDSTREALDATA_H
#define HDSTREALDATA_H


#include "hdstproduction.h"


class HDstRealData : public HDstProduction
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
      HDstRealData();
      ~HDstRealData() {};

      void runNTupleMaker();
      void runPdfMaker();
      void setupInput();
      void setupTaskList();

      ClassDef( HDstRealData, 0 ) // Base class used for DST Production from real data
};


#endif   // HDSTREALDATA_H
