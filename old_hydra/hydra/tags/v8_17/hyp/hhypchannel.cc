//*-- AUTHOR : I. Froehlich
//*-- Modified : 21/04/2005 by B. Spruck
//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////////////
//
// HHypChannel
//
// Holds the list numbers for the channels                  
// 
// List namings follow the reaction naming scheme (see wiki entry)
// There are 2 ways:
//     i) you use the standard naming schem
//        this means this class has to be updated from time to time
//    ii) you use you own name
//        This is at your own risk. Nobody can read your data when
//        you make a Micro-Hyp-DST (Even not yourself)
//        It is only recomended for in-mid-selectors where storing
//        data is not so important
//
////////////////////////////////////////////////////////////////////////

using namespace std;

#include <iostream>
#include "hhypchannel.h"

ClassImp(HHypChannel)

Bool_t HHypChannel::Add(const Char_t *name, Int_t major, Int_t minor)
{
  // Add one more channel to the list of known
  // Input
  // name: Name of the channel, see HadesHypNamingConvention in the wiki
  // major: Main channel
  // minor: subchannel (e.g. different algorithm selection)
  if (add_pointer == HHYP_MAX_HYPCHANNELS) {
    cout << "channels empty" << endl;
    return kFALSE;
  }
  hypchannel_numbers[add_pointer] = major * HHYP_SUBCHANNELS + minor;
  hypchannel_names[add_pointer] = name;
  add_pointer++;
  return kTRUE;
}

HHypChannel::HHypChannel(void)
{
  add_pointer = 0;
  dynamic_pointer = HHYP_SUBCHANNELS * 1000;

  Add("raw", 0, -1);            //raw event (better to read...)  
  Add("empty", 0, -2);          //empty event
  Add("ntuple", 0, -3);         //only ntuple
  Add("dummy", 0, 1);

  //////////////////////////////////
  // pion production              //
  //////////////////////////////////

  Add("PPPipPim",         10, 0);       // 4 Tracks with correct ID, no cut
  Add("PPPipPim_dtof",    10, 1);       // 4 Tracks with correct ID, dtof cut
  Add("PPPipPim_hard",    10, 2);       // 4 Tracks with correct ID, hard cut
  Add("PPPipPim_Pi0kine", 10, 3);       // 4 Tracks with correct ID, pi0-kine

  Add("PPPipPim_Pi0",         11, 0);   // Cut on missing Pi0
  Add("PPPipPim_Pi0_dtof",    11, 1);   // Cut on missing Pi0, dtof cut
  Add("PPPipPim_dtof_Pi0",    11, 2);   // Cut on missing Pi0, dtof cut
  Add("PPPipPim_Pi0kine",     11, 3);   // Cut on missing Pi0 with kine refit
  Add("PPPipPim_Pi0_Pi0kine", 11, 4);   // Cut on missing Pi0 with kine refit
  Add("PPPipPim_Pi0_hard",    11, 5);   // Cut on missing Pi0, hard cut
  Add("PPPipPim_hard_Pi0",    11, 6);   // Cut on missing Pi0, hard cut

  Add("PPPipPim_Miss0", 12, 0); // Cut on no missing particle

  //////////////////////////////////
  // dilepton production          //
  //////////////////////////////////

  Add("PPEpEm", 20, 0);         // 4 Tracks with correct ID, no cut
  Add("PPEpEm_dtof", 20, 1);
  Add("PPEpEm_hard", 20, 2);

  Add("PPEpEm_Gamma", 21, 0);   // 
  Add("PPEpEm_hard_Gamma", 21, 1);   // 


  //////////////////////////////////
  // proton scattering            //
  //////////////////////////////////

  Add("PP", 30, 0);             // 4 Tracks with correct ID, no cut
  Add("PP_hard", 30, 2);
  Add("PP_elastic", 31, 0);
  Add("PP_Pi0", 32, 0);


  //////////////////////////////////
  // Lambda reco                  //
  //////////////////////////////////

  Add("PPKpPim",            40,0);  // 4 Tracks with correct ID, no cut
  Add("PPKpPim_dtof",       41,0);
  Add("PPKpPim_dtof_Miss0", 42,0);
  Add("PPKpPim_dtof_Miss0_Lambda", 43,0);


}

HHypChannel::~HHypChannel(void)
{
}

Int_t HHypChannel::Get(const Char_t *name)
{
  //Get back the number from a name
  for (Int_t i = 0; i < add_pointer; i++) {
    if (strcmp(name, hypchannel_names[i]) == 0)
      return (hypchannel_numbers[i]);
  }

  Add(name, 0, dynamic_pointer);
  dynamic_pointer++;

  cout << "WARNING: you are using the non-standard channel " << name << endl;
  return dynamic_pointer - 1;

}

const Char_t *HHypChannel::Get(Int_t num)
{
  //Get back the name from the number
  if (num == -1)
    return hypchannel_names[0];

  for (Int_t i = 0; i < add_pointer; i++) {
    if (num == hypchannel_numbers[i])
      return (hypchannel_names[i]);
  }
  return "unknown";
}
