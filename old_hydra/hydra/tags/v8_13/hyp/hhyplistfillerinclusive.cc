// dedicated for : 1) p p
//                 2) p p e+ e- 

// filler creates all possible combinations if we have more particle than expected
// run with DEBUG=1 to see details

// Does NOT work for p p pi+ pi- !!!!!!!!!!!!!!!! -> needs upgrade

//*-- Authors: Tiago Perez & Marcin Wisniowski
//*-- Modified: 31.02.2006 Marcin Wisniowski 
//*-- Modified: 10.05.2006 Marcin Wisniowski
//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////////////
//
// HHypListFillerInclusive
//
// HypListFiller is creating a permutated list for all particle types
// which are needed for the Algorithm. This list is created once at
// init time and then copied to HypComp for every event on execute.
// List of combination of 2(4) tracks out of N tracks
//
//  t1   t2   t3   t4   N=4, Ncomb=6, all tracks are taken as protons 
//                           assumed id=14
//	
//	0:   t1   t2
//	1:   t1   t3
//	2:   t1   t4
//	3:   t2   t3
//	4:   t2   t4
//	5:   t3   t4
//
////////////////////////////////////////////////////////////////////////

#include "hhyplistfillerinclusive.h"

#define DEBUG 0 
#define N_ALG 4 // Runge Kutta
#define MAX_N_PAR  8  // maximal number of one-sign leptons/hadrons
using namespace std;

ClassImp(HHypListFillerInclusive)

HHypListFillerInclusive::HHypListFillerInclusive(Text_t * name, Text_t * title):
HReconstructor(name, title)
{
  nElectron  = 0;
  nPositron = 0;
  nPosPion = 0;
  nPosPion = 0;
  nProton  = 0;
  numberOfParticles = 0;
}

//-----------------------------------------------------------------
HHypListFillerInclusive::HHypListFillerInclusive()
{
  nElectron  = 0;
  nPositron = 0;
  nPosPion = 0;
  nPosPion = 0;
  nProton  = 0;
}

//-----------------------------------------------------------------
HHypListFillerInclusive::~HHypListFillerInclusive(void)
{
}

//-----------------------------------------------------------------
Bool_t HHypListFillerInclusive::SetExitList(Int_t e_list)
{
	exitList = e_list;
	return kTRUE;
}

//-----------------------------------------------------------------
Bool_t HHypListFillerInclusive::init()
{ 

    cerr<<endl<<" Using filler : HHypListFillerInclusive  "<<endl<<endl;
	// Before init is called, all the tracks need to be added.
	// Next the number of possible combinations is computed and a pid
	// table with all permutaions is created. Next steps done are to init
	// HypComb, HypList and PidParticle Containers
	
	//---------- Initialization of HHypComb Container -----------------------
	m_pContCatComb = gHades->getCurrentEvent()->getCategory(catHypComb);
	
	if (!m_pContCatComb) {
		m_pContCatComb = HHypComb::buildLinearCat("HHypComb");
	
		if (!m_pContCatComb)
		return kFALSE;
		// else
		gHades->getCurrentEvent()->addCategory(catHypComb, m_pContCatComb,
						       "HypComb");
	}
	//-----------------------------------------------------------------------
	
	//---------- Initialization of HHypList Container -----------------------
	m_pContCatList = gHades->getCurrentEvent()->getCategory(catHypList);
	
	if (!m_pContCatList) {
		m_pContCatList = HHypList::buildLinearCat("HHypList");
	
		if (!m_pContCatList)
		return kFALSE;
		// else
		gHades->getCurrentEvent()->addCategory(catHypList, m_pContCatList,
						       "HypList");
	}
	//-----------------------------------------------------------------------
	
	//---------- Initialization of HPidParticle Container -----------------------
	m_pContItPart = NULL;         // Iterator
	m_pContCatPart = NULL;        // Category
	
	if ((m_pContCatPart =
		gHades->getCurrentEvent()->getCategory(catPidPart)) == NULL) {
		Error("init", "Cannot get catPidPart cat");
		return kFALSE;
	}
	m_pContItPart = (HIterator *) m_pContCatPart->MakeIterator(); 
	
	nHadronPlus  = nPosPion + nProton;
	nHadronMinus = nNegPion;
	nLeptonPlus  = nPositron;
	nLeptonMinus = nElectron;

	return kTRUE;
}// End Bool_t HHypListFillerInclusive::init() 

Int_t HHypListFillerInclusive::execute()
{

	Int_t index_comb = 0;
	Int_t index_list = 0;
	
	HLocation locDummy;
	
	counterHadronPlus  = 0;
	counterHadronMinus = 0;
	counterLeptonPlus  = 0;
	counterLeptonMinus = 0;
	
	Int_t counter = 0;
	Int_t Charge[10];

    for(int kk=0;kk<20;kk++) 
	{
	    PosHadronIdx[kk] = -1;
	    NegHadronIdx[kk] = -1;
	    PositronIdx[kk]  = -1;
	    ElectronIdx[kk]  = -1;
	}

	HPidParticle  *pidPart    = NULL;
	HPidHitData   *pidHitData = NULL;
	HPidTrackData *PidTrack   = NULL;
	
	m_pContItPart->Reset();
	while (((pidPart = (HPidParticle *) m_pContItPart->Next()) != NULL)
			&& (counter < 10)) 
	{
		pidHitData    = (HPidHitData*  ) pidPart -> getHitData();
		PidTrack      = (HPidTrackData*) pidPart -> getTrackData();

		Charge[counter] = pidPart -> getCharge();
	
		if (Charge[counter]==1 /*&& pidPart->getValidFlag()==1*/)
		{
	        if(pidHitData->hasRingCorrelation[N_ALG])
			{
			    PositronIdx[counterLeptonPlus] = counter;
		        counterLeptonPlus++;
			}
	        else
			{
			    PosHadronIdx[counterHadronPlus] = counter;
	            counterHadronPlus++;
			}
		}
		else if (Charge[counter]==-1 /*&& pidPart->getValidFlag()==1*/)
		{
            if(pidHitData->hasRingCorrelation[N_ALG])
			{
			    ElectronIdx[counterLeptonMinus] = counter;
                counterLeptonMinus++;
			}
	        else
			{
			    NegHadronIdx[counterHadronMinus] = counter;
		        counterHadronMinus++;
			}
		}
		counter++;
	}
	
    if ( counterHadronPlus >= nHadronPlus  && counterHadronPlus < MAX_N_PAR &&
	     counterHadronMinus>= nHadronMinus && counterHadronMinus< MAX_N_PAR &&
		 counterLeptonPlus >= nLeptonPlus  && counterLeptonPlus < MAX_N_PAR &&
	     counterLeptonMinus>= nLeptonMinus && counterLeptonMinus< MAX_N_PAR ) 
	{

		//=========================== Create HHypComb Container ================
		HHypComb *hypcomb = NULL;

        if(DEBUG) cerr<<"   counterHadronPlus: "<<counterHadronPlus<<
		                "   counterHadronMinus: "<<counterHadronMinus<<
						"   counterLeptonPlus: "<<counterLeptonPlus<<
						"   counterLeptonMinus: "<<counterLeptonMinus<<endl;
						
        numberOfCombinations  = CalculateNComb(counterHadronPlus,  nHadronPlus,  nProton);
	    numberOfCombinations *= CalculateNComb(counterLeptonMinus, nLeptonMinus, nElectron);
	    numberOfCombinations *= CalculateNComb(counterLeptonPlus,  nLeptonPlus,  nPositron);
	
        if(DEBUG) cerr<<"   numberOfCombinations: "<<numberOfCombinations<<
		                "   nHadronPlus: "<<nHadronPlus<<
						"   nHadronMinus: "<<nHadronMinus<<
						"   nLeptonPlus: "<<nLeptonPlus<<
						"   nLeptonMinus: "<<nLeptonMinus<<endl;
	
		hypcomb = (HHypComb *) m_pContCatComb->getNewSlot(locDummy, &index_comb);
		if (hypcomb != 0)
			hypcomb = new(hypcomb) HHypComb(numberOfCombinations,
			              nHadronPlus + nHadronMinus + nLeptonPlus + nLeptonMinus);
		else
			return ERROR_IDX;
		//=========================== Create HHypComb Container END =============


		//========================== Fill PID Table in HHypComb =================
		fill_pid_table(hypcomb);    // just copy from pid_real_array

	
		//========================== Fill Idx Table in HHypComb =================
		fill_pid_idx(hypcomb);


		//=========================== Create HHypList Container ==================
		HHypList *hyplist = NULL;
	
		hyplist = (HHypList *) m_pContCatList->getNewSlot(locDummy, &index_list);
		if (hyplist != 0)
			hyplist = new(hyplist) HHypList(index_comb);
		else
			return ERROR_IDX;
		//=========================== Create HHypList Container END ==============
	
		hyplist->setListId(exitList);
	
		/*
		for (Int_t Icomb = 0; Icomb < numberOfCombinations; Icomb++) {
			//this is to keep PIDTRACK INFO
			if (Icomb < MAX_USER_VALUES)
				hyplist->setUserValue(FILLER_VALID_PIDTRACKS, numpidtracks[Icomb],
								Icomb);
		}
		*/
	
	}//if(counterPlus >= nPlus)
	else {
		return ERROR_IDX;
	}//if( counterPlus==nPlus && counterMinus==nMinus )
	
	return index_list;
}

Bool_t HHypListFillerInclusive::finalize()
{
	return kTRUE;
}  // End HHypListFillerInclusive::finalize()

Bool_t HHypListFillerInclusive::AddTrack(Int_t particle)
{
	// Adds a particle type track to filler
	//      must be done before init is called

    switch(particle)
	{
	    case 2 :  nPositron++;   break;
		case 3 :  nElectron++;   break;
		case 8 :  nPosPion++;    break;
		case 9 :  nNegPion++;    break;
		case 14 : nProton++;     break;
        default : cerr<<"HHypListFillerInclusive::AddTrack particle witch ID= "<<particle
		              <<" not implemented"<<endl; break;
	}
	return kTRUE;
}

// -----------------------------------------------------------------------
// ----------       Private methods --------------------------------------
// -----------------------------------------------------------------------

void HHypListFillerInclusive::fill_pid_table(HHypComb * hypcomb)
{
 for (Int_t i = 0; i < numberOfCombinations; i++) {

        int j=0, jj=0, jjj=0, jjjj=0, jjjjj=0;
        // proton
        for (j = 0; j < nProton; j++) {
            hypcomb->setPid(i, j, 14);
        }
        // pi+
        for (jj = j; jj < nPosPion+j; jj++) {
            hypcomb->setPid(i, jj, 8);
        }
        // e+ 
        for (jjj = jj; jjj < nPositron+jj; jjj++) {
            hypcomb->setPid(i, jjj, 2);
        }
        // pi-
        for (jjjj = jjj; jjjj < nNegPion+jjj; jjjj++) {
            hypcomb->setPid(i, jjjj, 9);
        }
        // e-
        for (jjjjj = jjjj; jjjjj < nElectron+jjjj; jjjjj++) {
            hypcomb->setPid(i, jjjjj, 3);
        }
    }
}

Int_t HHypListFillerInclusive::fill_pid_idx(HHypComb * hypcomb)
{
    int NComb=0, MComb=0;
	if(nHadronPlus)
      for(int i=0; i<numberOfCombinations; i+=CalculateNComb(counterHadronPlus,  nHadronPlus,  nProton))
        NComb=fill_pid_idx_double_part(hypcomb, counterHadronPlus,0 , i, PosHadronIdx);	
	if(nLeptonPlus)
	  for(int i=0; i<NComb*counterLeptonPlus*counterLeptonMinus;i+=NComb*counterLeptonPlus)	
        MComb=fill_pid_idx_singiel_part(hypcomb, counterLeptonPlus, 2, i, NComb, PositronIdx);	
	if(nLeptonMinus)
      NComb=fill_pid_idx_singiel_part(hypcomb, counterLeptonMinus,3, 0, MComb, ElectronIdx);	
	
	if(DEBUG) hypcomb->print();
	return NComb;	
}

Int_t HHypListFillerInclusive::fill_pid_idx_double_part(HHypComb * hypcomb, 
                                   Int_t nTracks, Int_t iParPosition, Int_t iStartComb, Int_t *pidIdx)
{
    Int_t i=0;
	Int_t iComb = 0;
	Int_t idx = 0;           // idx nnumber of particle in contPid

    for(Int_t j = 0; j < nTracks-1; j++)
	{ 
	    for (i = 0; i < nTracks-idx-1; i++)
	    {
			hypcomb->setIdxPidPart( iStartComb + iComb,  iParPosition, pidIdx[idx]);
		    iComb++;
		} 
		idx++;
	}

	iComb = 0;
	idx = 1;
		
	for(Int_t j = 0; j < nTracks-1; j++)
	{
	    for (i = idx; i < nTracks; i++)
	    {
			hypcomb->setIdxPidPart( iStartComb + iComb,  iParPosition + 1, pidIdx[i]);
			iComb++;
		}
		idx++;
	}
    return  iComb;	
}

Int_t HHypListFillerInclusive::fill_pid_idx_singiel_part(HHypComb * hypcomb, Int_t nTracks, Int_t iParPosition,
                                               Int_t iStartComb, Int_t iNComb,Int_t *pidIdx )
{
     Int_t iComb=0;

     for(Int_t idx=0; idx<nTracks; idx++)
	 {
         for(int i=0; i<iNComb; i++)
         {
              hypcomb->setIdxPidPart( iStartComb + iComb,  iParPosition , pidIdx[idx]);
			  iComb++;
         }
	 }
	 return  iComb;
}

Bool_t HHypListFillerInclusive::fill_pid_fprob(HHypComb * hypcomb,Int_t *numpidtracks)
{
	/*
	HPidParticle *PidPart = NULL;

	HCategory *pidpartCat = gHades->getCurrentEvent()->getCategory(catPidPart);

	for (Int_t Icomb = 0; Icomb < numberOfCombinations; Icomb++) {
		Float_t ProbComb = 1.0;
	
		if (Icomb < MAX_USER_VALUES)
			numpidtracks[Icomb] = 0;
		for (Int_t Ipart = 0; Ipart < numberOfRealParticles; Ipart++) {
			Int_t Pid = hypcomb->getPid(Icomb, Ipart);
			Int_t Idx = hypcomb->getIdxPidPart(Icomb, Ipart);
	
			PidPart = (HPidParticle *) pidpartCat->getObject(Idx);
	
			if (PidPart == NULL) {
				Error("fill_pid_fprob","[ERROR] HHypListFillerInclusive::execute() setProbComb");
				return kFALSE;
			}
	
			Float_t Prob = PidPart->getWeightForPID(Pid);
			Float_t Pid_from_PID = PidPart->getPid();
	
			//cout << "Our Pid " << Pid << " Prob " << Prob << " Pid_from_PID " << Pid_from_PID << endl;
	
			// To distinguish this to cases (below)
			// Pid_from_PID=-3  Prob(p)=-1  Prob(pi+)=-1     Prob(pi-)=-1   Charge=-1
			// Pid_from_PID= 8  Prob(p)=-1  Prob(pi+)=0.999  Prob(pi-)=-1   Charge= 1
			if (Prob == -1.0 && Pid_from_PID < 0) {
				Prob = 1.0;
			} else if (Prob == -1.0 && Pid_from_PID > 0) {
				Prob = 0.0;
			} else if (Icomb < MAX_USER_VALUES){
				numpidtracks[Icomb]++;
			}
			
			ProbComb *= Prob;
		}
		//std::cout << "setting prob " << ProbComb << std::endl;
		if (ProbComb < 0)
			ProbComb = 0.0;         //lower than zero is very strange
	
		hypcomb->setProbComb(Icomb, ProbComb);
	}// end for
	*/
	for(Int_t i=0; i<numberOfCombinations; i++)
	    hypcomb->setProbComb(i, 1);

	return kTRUE;
}

Int_t HHypListFillerInclusive::CalculateNComb(Int_t nTracksPlus,Int_t nPartPlus,Int_t nSamePartPlus)
// number of positiv tracks type X, number of positive particles X, number of identicel particles X
// X i.e positive hadron
// if we want to analize p p cases and we find out 3 good tracks (X) : CalculateNComb(3,2,2)
// if we want to analize p p pip and we find out 4 good tracks (X) : CalculateNComb(4,3,2)

{
	static Int_t faculties[11] = {
		1, 1, 2, 6,                 // 0,1,2,3
		24,                         // 4
		120,                        // 5
		720,                        // 6
		5040,                       // 7
		40320,                      // 8
		362880,                     // 9
		3628800                     // 10
	};
    
	return faculties[nTracksPlus]/(faculties[nTracksPlus-nPartPlus]*faculties[nSamePartPlus]);
}
