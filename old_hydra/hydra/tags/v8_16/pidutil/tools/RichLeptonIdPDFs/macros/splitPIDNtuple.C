// This macro makes 4 NTuples for singles, doubles, fakes and hadrons from the joined common
// Ntuple which was written by the PDF-maker class. Those four NTuples are the input to the
// PDF-construction macro. The macro opens all pdf-files in the list given as third argument
// and retrieves the ntuple with the relevant data.


  enum enumData{
    f_kick_mom_exp,f_spline_mom_exp,f_rk_mom_exp,f_kick123_mom_exp,f_reft_mom_exp,n_sector,f_mom_geant,f_track_theta,f_track_phi,f_deltatheta,f_deltaphi,n_process,b_ring,b_kicktrack,b_splinetrack,b_rktrack,b_kick123track,b_reftrack,b_segments,f_shw_sum0,f_shw_sum1,f_shw_sum2,n_shw_cs0,n_shw_cs1,n_shw_cs2,f_shw_tof,f_tof_tof,n_shw_mult,i_tracktype,i_ringtype,n_mdc_nw0,n_mdc_nw1,n_mdc_cfl0,n_mdc_cfl1,n_mdc_cls0,n_mdc_cls1,f_mdc_chi2,n_rich_patmatrix,n_rich_npads,f_rich_centroid,f_rich_avcharge,n_rich_locmax,i_geant_pid,n_system,f_metamatch_quality,f_kick_pull,f_spline_quality,f_distance_to_vertex,f_track_r_spline,f_track_z_spline, n_medium, b_corrRingSpline,b_corrRingKick, f_vertex_zcoord,n_charge_kick,n_charge_spline,f_track_z_kick,f_track_r_kick, i_geant_corrflag};


#define MAX_NTUPLE_SIZE 200 
#define NTUPLE_VARS1 "f_kick_mom_exp:f_spline_mom_exp:f_rk_mom_exp:f_kick123_mom_exp:f_reft_mom_exp:n_sector:f_mom_geant:f_track_theta:f_track_phi:f_deltatheta:f_deltaphi:n_process:b_ring:b_kicktrack:b_splinetrack:b_rktrack:b_kick123track:b_reftrack:b_segments:f_shw_sum0:f_shw_sum1:f_shw_sum2:n_shw_cs0:n_shw_cs1:n_shw_cs2:f_shw_tof:f_tof_tof:n_shw_mult:i_tracktype:i_ringtype"
#define NTUPLE_VARS2 ":n_mdc_nw0:n_mdc_nw1:n_mdc_cfl0:n_mdc_cfl1:n_mdc_cls0:n_mdc_cls1:f_mdc_chi2:n_rich_patmatrix:n_rich_npads:f_rich_centroid:f_rich_avcharge:n_rich_locmax:i_geant_pid:n_system:f_metamatch_quality:f_kick_pull:f_spline_quality:f_distance_to_vertex:f_track_r_spline:f_track_z_spline:n_medium:b_corrRingSpline:b_corrRingKick:f_vertex_zcoord:n_charge_kick:n_charge_spline:f_track_z_kick:f_track_r_kick:i_geant_corrflag"

splitPIDNtuple(Char_t* targetdirectory, Char_t* targetfile, Char_t* ntuplefiles)
{ 
  
  TString NTUPLE_VARS1("f_kick_mom_exp:f_spline_mom_exp:f_rk_mom_exp:f_kick123_mom_exp:f_reft_mom_exp:n_sector:f_mom_geant:f_track_theta:f_track_phi:f_deltatheta:f_deltaphi:n_process:b_ring:b_kicktrack:b_splinetrack:b_rktrack:b_kick123track:b_reftrack:b_segments:f_shw_sum0:f_shw_sum1:f_shw_sum2:n_shw_cs0:n_shw_cs1:n_shw_cs2:f_shw_tof:f_tof_tof:n_shw_mult:i_tracktype:i_ringtype");
  TString NTUPLE_VARS2(":n_mdc_nw0:n_mdc_nw1:n_mdc_cfl0:n_mdc_cfl1:n_mdc_cls0:n_mdc_cls1:f_mdc_chi2:n_rich_patmatrix:n_rich_npads:f_rich_centroid:f_rich_avcharge:n_rich_locmax:i_geant_pid:n_system:f_metamatch_quality:f_kick_pull:f_spline_quality:f_distance_to_vertex:f_track_r_spline:f_track_z_spline:n_medium:b_corrRingSpline:b_corrRingKick:f_vertex_zcoord:n_charge_kick:n_charge_spline:f_track_z_kick:f_track_r_kick");
  
  
  
  TString targetFile(targetdirectory);
  targetFile.Append(targetfile);
  cout << "Writing to: " << targetFile.Data() << endl;
  TNtuple* N=NULL;
  
  TFile* inputFile[2000];
  TNtuple* ntup[2000];
  Char_t* names[2000];
  Int_t nFiles=0;
  
  for(Int_t i=0;i<2000;i++)
    {
      inputFile[i]=NULL;
      ntup[i]=NULL;
      names[i]=new Char_t[265];
    }
  
  ifstream nTupleList(ntuplefiles);
  Char_t name[256];
  Int_t wait;
  Int_t k=0;
  
  while(nTupleList.getline(name,265) &&nFiles<2000)
    { 
      cout << "opening file: " << name<< endl;
      inputFile[k] = TFile::Open(name);
      ntup[k]=(TNtuple*)inputFile[k]->FindObjectAny("N");
      //ntup[k]->SetDirectory(0);
      sprintf(names[k],"%s",name);
      k++;
      nFiles++;
      
    }

  //The four separate ntuples are empty copies of the original one
  
  TFile* pidNtuples = new TFile(targetFile.Data(),"RECREATE");

  TString ntuple_str(NTUPLE_VARS1.Data());
  ntuple_str.Append(NTUPLE_VARS2.Data());

  
  TNtuple* SingleNtuple = new TNtuple("SingleProperties","SingleProperties",ntuple_str);
  TNtuple* DoubleNtuple = new TNtuple("DoubleProperties","DoubleProperties",ntuple_str);
  TNtuple* FakeNtuple = new TNtuple("FakeProperties","FakeProperties",ntuple_str);
  TNtuple* HadronNtuple = new TNtuple("HadronProperties","HadronProperties",ntuple_str);
  



  
  
  TObjArray* listOfBranches==NULL;
  TIterator* branchIterator=NULL;
  TBranch* bbr==NULL;

  //Make a place to store the data read from the tree
  Float_t PI[58]; //Short for PatternInstance
  for(Int_t a=0;a<nFiles;a++)
    {

      cout << "filename/number: " << names[a] << " " << a << endl;
      listOfBranches = ntup[a]->GetListOfBranches();
      cout <<"List of branches found!" << endl;
      branchIterator = listOfBranches->MakeIterator();

      
      branchIterator->Reset();
      
      Int_t branchcount=0;
      cout << "branchcount is now: " << branchcount << endl;

      while((bbr=(TBranch*)branchIterator->Next())!=NULL)
	{
	  ntup[a]->SetBranchAddress(bbr->GetName(),&PI[branchcount]);
	  //cout << bbr->GetName() << endl;
	  //cout << branchcount << endl;
	  branchcount++;
	}
     

      //#warning Here a check for the b_corrRingSpline variable is required ! It can be negative! - - FIX this
      //Fill all single patterns to the ntuples
      Int_t nPatterns = ntup[a]->GetEntries();
      for(Int_t i=0;i<nPatterns;i++)
	{
	  ntup[a]->GetEntry(i);
	  if (PI[b_corrRingKick]==1 || (PI[b_corrRingSpline]==1 && (PI[f_shw_tof]>-1.0 || PI[f_tof_tof]>-1.0)))
	    {//correlated KickTrack or SplineTrack with Meta hit!
	      if(PI[i_tracktype]>=2 && PI[i_ringtype]>=0) DoubleNtuple->Fill(PI);
	      if(PI[i_tracktype]==1 && PI[i_ringtype]>=0) SingleNtuple->Fill(PI);
	      if(PI[i_tracktype]==0 && PI[i_ringtype]>=0) HadronNtuple->Fill(PI);	
	      if(PI[i_tracktype]==-1 && PI[i_ringtype]>=0) FakeNtuple->Fill(PI);	
	     	    }	  
	  if(!(i%50000))
	    cout << i << endl;
	}

      cout << "Number of singles after copy is: " << SingleNtuple->GetEntries()<< endl;
      cout << "Number of doubles after copy is: " << DoubleNtuple->GetEntries()<< endl;
      cout << "Number of fakes after copy is: " << FakeNtuple->GetEntries()<< endl;
      cout << "Number of hadrons after copy is: " << HadronNtuple->GetEntries()<< endl;
      inputFile[a]->Close();
      cout << "counter: " << a << endl;
    }

  
  SingleNtuple->Write();
  DoubleNtuple->Write();
  FakeNtuple->Write();
  HadronNtuple->Write();

  cout << "closing!" << endl;
  pidNtuples->Close();
  
  //Now we have the Ntuples with singles, doubles and fakes, required by the advanced_pid_analysis Macro
  //In this case the analysis is finished here!
}  


