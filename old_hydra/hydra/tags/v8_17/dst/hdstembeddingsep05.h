/**
 * @file   hdstembeddingsep05.h
 * @author Simon Lang
 * @date   Wed Nov 14 14:44:11 2007
 * 
 * @brief  This Class provides Functions for a DST Production from 
 *         embedded Data of Sep05 Beamtime
 * 
 * $Id: hdstembeddingsep05.h,v 1.1 2007-11-14 14:13:42 slang Exp $
 */

#ifndef HDSTEMBEDDINGSEP05_H
#define HDSTEMBEDDINGSEP05_H


#include "hdstembedding.h"


class HDstEmbeddingSep05 : public HDstEmbedding
{
   public:
      HDstEmbeddingSep05() : HDstEmbedding() {};
      ~HDstEmbeddingSep05() {};

      void setupTaskListGen1();
      
      ClassDef( HDstEmbeddingSep05, 0 ) // Class used for DST Production from embedded Data of Sep05 beamtime
};


#endif   // HDSTEMBEDDINGSEP05_H
