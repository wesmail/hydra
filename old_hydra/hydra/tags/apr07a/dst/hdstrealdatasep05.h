/**
 * @file   hdstrealdatasep05.h
 * @author Simon Lang
 * @date   Thu Oct  5 14:00:03 2006
 * 
 * @brief  This Class provides Functions for a DST Production from real Data
 *         of Sep05 Beamtime
 * 
 * $Id: hdstrealdatasep05.h,v 1.3 2006-12-15 14:20:08 slang Exp $
 */

#ifndef HDSTREALDATASEP05_H
#define HDSTREALDATASEP05_H


#include "hdstrealdata.h"


class HDstRealDataSep05 : public HDstRealData
{
   public:
      HDstRealDataSep05() : HDstRealData() {};
      ~HDstRealDataSep05() {};

      void setupTaskListGen0();
      void setupTaskListPostGen0();
      void setupTaskListGen1();

   ClassDef(HDstRealDataSep05, 0) // Class used for DST Production from real data of Sep05 beamtime
};


#endif   // HDSTREALDATASEP05_H
