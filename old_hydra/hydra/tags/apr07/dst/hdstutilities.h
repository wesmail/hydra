/**
 * @file   hdstutilities.h
 * @author Simon Lang
 * @date   Fri May  5 15:26:25 2006
 * 
 * @brief  This Class provides Utility Functions for a DST Production
 * 
 * $Id: hdstutilities.h,v 1.2 2006-12-15 14:12:23 slang Exp $
 *
 */


#ifndef HDSTUTILITIES_H
#define HDSTUTILITIES_H


#include "TObject.h"


class HDstUtilities : public TObject
{
   private:
      static const Int_t MAX_LINE_LENGTH = 1024;

   private:
      HDstUtilities() {};

   public:
      ~HDstUtilities() {};

      static Bool_t readCmdLineArgsFromFile(const char* filename,
					    int* argc, char*** argv);
      static void  str2CmdLineArgs(const char* input, int* argc, char*** argv);
      static void  freeCmdLineArgs(int argc, char** argv);
      static char* extractArg(int* argc, char*** argv, const char* arg);

   ClassDef( HDstUtilities, 0 ) // Utility Functions for a DST Production
};


#endif   // HDSTUTILITIES_H
