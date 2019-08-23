/**
 * @file   hdstsimulationsep05.h
 * @author Simon Lang
 * @date   Thu Oct  5 13:59:12 2006
 * 
 * @brief  This Class provides Functions for a DST Production from 
 *         simulated Data of Sep05 Beamtime
 * 
 * $Id: hdstsimulationsep05.h,v 1.1 2006-10-05 12:57:45 slang Exp $
 */

#ifndef HDSTSIMULATIONSEP05_H
#define HDSTSIMULATIONSEP05_H


#include "hdstsimulation.h"


class HDstSimulationSep05 : public HDstSimulation
{
   public:
      HDstSimulationSep05() : HDstSimulation() {};
      ~HDstSimulationSep05() {};

      void setupTaskListGen0();
      void setupTaskListGen1();
      
      ClassDef( HDstSimulationSep05, 0 ) // Class used for DST Production from simulated Data of Sep05 beamtime
};


#endif   // HDSTSIMULATIONSEP05_H
