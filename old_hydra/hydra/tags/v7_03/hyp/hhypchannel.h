/***************************************************************************
 *   Author :  Ingo Froehlich <ingo.froehlich@exp2.physik.uni-giessen.de>  *
 *             II Physikalisches Institut                                  *
 *             Universitaet Giessen                                        *
 *            								   *
 *   Project:  HYDRA Event Hypothesys  (HADES)                             *
 *   Description: Holds the list numbers for the channels                  *
 *                at the moment everything is hardcoded, later dynamic     *
 *                allocation and storing in the root file is planned 	   *
 *            								   *
 *   Modified :  2005-01-11 Created: IF                                    *
 *            								   *
 ***************************************************************************/

#ifndef HHYPCHANNEL_H
#define HHYPCHANNEL_H



#include <TObject.h>
#include <TArrayI.h>

class HCategory;
class HIterator;
class HSpectrometer;
class HRuntimeDb;

class HHypChannel 
{

public:

    HHypChannel();
    virtual ~HHypChannel();
    
    Int_t Get(char *);

    ClassDef(HHypChannel,1)
};

//============================================================================

#endif 

