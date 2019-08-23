#include "hhyplistfiller.h"

using namespace std;

ClassImp(HHypListFiller)

///////////////////////////////////////////////////////////////////////
//HHypListFiller
//
// HHypListFiller 
//
//////////////////////////////////////////////////////////////////////

HHypListFiller::HHypListFiller(Text_t *name,Text_t *title) :
  HReconstructor(name,title)
{
  nPlus =0;
  nMinus = 0;
  numberOfParticles = 0;
  particleArrayPlus.Set(0); 
  particleArrayMinus.Set(0); 
}

HHypListFiller::HHypListFiller()
{
  nPlus =0;
  nMinus = 0;
  numberOfParticles = 0;
  particleArrayPlus.Set(0); 
  particleArrayMinus.Set(0); 
}


HHypListFiller::~HHypListFiller(void) {
}

Bool_t HHypListFiller::SetExitList(Int_t e_list)
{
    exitList = e_list;
    return kTRUE;
}



Bool_t HHypListFiller::init() 
{
  cout<<"initialization of HHypListFiller "<<endl;
  generateNumberOfCombinations();   
  
  //---------- Initialization of HHypComb Container -----------------------
  m_pContCatComb=gHades->getCurrentEvent()->getCategory(catHypComb);
    	
  if (!m_pContCatComb) 
    {
      m_pContCatComb = HHypComb::buildLinearCat("HHypComb");

      if (!m_pContCatComb) return kFALSE;
      else gHades->getCurrentEvent()
	     ->addCategory(catHypComb, m_pContCatComb, "HypComb");
    }
  //-----------------------------------------------------------------------
    
  //---------- Initialization of HHypList Container -----------------------
  m_pContCatList=gHades->getCurrentEvent()->getCategory(catHypList);
	
  if (!m_pContCatList) 
    {
      m_pContCatList = HHypList::buildLinearCat("HHypList");

      if (!m_pContCatList) return kFALSE;
      else gHades->getCurrentEvent()
	     ->addCategory(catHypList, m_pContCatList, "HypList");
    }
  //-----------------------------------------------------------------------
  
  //---------- Initialization of HPidParticle Container -----------------------
  m_pContItPart = NULL;    // Iterator
  m_pContCatPart = NULL;   // Category

  if((m_pContCatPart = gHades->getCurrentEvent()->getCategory( catPidPart )) == NULL) 
    {
      Error("init", "Cannot get catPidPart cat");
      return kFALSE;
    }
  m_pContItPart = (HIterator*)m_pContCatPart->MakeIterator();
  //-----------------------------------------------------------------------
  return kTRUE;
}// End Bool_t HHypListFiller::init() 


Int_t HHypListFiller::execute()
{
  // cout<<"[Debug] HHypListFiller::execute()"<<endl;
  Int_t index_comb=0;
  Int_t index_list=0;
  //  Int_t i,j;
  HLocation locDummy;
  // Get PID
  //	Check Nr of Particle
  //    Check Nr of + and -Track
  // Comapre NrParticlesPiD with Table ifnot return ERROR_IDX
  // Comapre NrParticlesPiD- with Table
  // Comapre NrParticlesPiD+ with Table
  //=======================================================================
   
  // get PID  
  Int_t counterMinus = 0;
  Int_t counterPlus = 0;
  Int_t counter = 0;
  Float_t Pz[10],Prob[10];
  Int_t PID[10], Charge[10];
  Float_t ProbId8[10], ProbId9[10], ProbId14[10];

  HPidParticle *pidPart = NULL;
  m_pContItPart->Reset();   
  while( (pidPart=(HPidParticle*)m_pContItPart->Next())!=NULL )
    {  
      Pz[counter]  = pidPart->Pz();
      PID[counter] = pidPart-> getPid();

      Prob[counter] = pidPart-> getWeight();
      ProbId8[counter]  = pidPart-> getWeightForPID(8);
      ProbId9[counter]  = pidPart-> getWeightForPID(9);
      ProbId14[counter] = pidPart-> getWeightForPID(14);
      Charge [counter]= pidPart-> getCharge();
       
      if( Charge[counter] ==  1) counterPlus++;
      else if( Charge[counter] == -1) counterMinus++;

      if (8 < counterPlus || 8 < counterMinus) {
	cerr<<"[Error] too many Particles\t cPlus = "<<counterPlus
	    <<"  cMinus = "<< counterMinus <<endl;
	break;
	
      }
      
      counter++; 
    }

  if( counterPlus==nPlus && counterMinus==nMinus )
    {  
      //      cout << "PLUS: " << counterPlus << " MINUS: " << counterMinus << endl;
      //=========================== Create HHypComb Container ================
      HHypComb *hypcomb = NULL;
       
      hypcomb = (HHypComb*) m_pContCatComb->getNewSlot(locDummy,&index_comb);
      if ( hypcomb != 0 ) hypcomb = new(hypcomb) HHypComb(numberOfCombinations ,numberOfParticles);
      else return ERROR_IDX;
      //=======================================================================       
       
      //========================== Fill Idx Table in HHypComb =================
      Int_t  idx = 0;      // idx nnumber of particle in contPid
      Int_t iPosPlus  = 0; // position of positive Particle in Idx table
      Int_t iPosMinus = 0; // position of negative Particle in Idx table
      
      m_pContItPart->Reset();
      while( (pidPart=(HPidParticle*)m_pContItPart->Next())!=NULL )
	{   
	  if( pidPart->getCharge() == -1 )      // set Idx for negative particle
	    {
	      for(Int_t i=0; i<numberOfCombinations; i++)
	        {
	          hypcomb->setIdxPidPart( i, (numberOfParticles-1)-iPosMinus, idx);
	        }
	      iPosMinus++;
	    }
	  else if(pidPart->getCharge() ==  1 )  // set Idx for positive particle
	    {	       
	      for(Int_t i=0; i<numberOfCombinations; i++)
	        {
	          hypcomb->setIdxPidPart( i, iPosPlus, idx);
		}  
	      iPosPlus++;
	    }
	  idx++;
	}
      //========================== Fill Idx Table in HHypComb END =============

      //========================== Fill PID Table in HHypComb =================
      // Fill Negative tracks
      for (Int_t i = 0; i < numberOfCombinations; i++) {
	for (Int_t j = 0; j < nMinus; j++)
	  hypcomb->setPid(i, (numberOfParticles-1)-j, particleArrayMinus.At(j) );
	hypcomb->setProbComb(i,0.1);
      }
      // Fill Positive tracks
      for (Int_t i = 0; i < numberOfCombinations; i++) {
	for (Int_t j = 0; j < nPlus; j++) {
	  Int_t k;
	  k = (j+i)%nPlus;
	  hypcomb->setPid(i, j, particleArrayPlus.At(k) );
	}  
	hypcomb->setProbComb(i,0.1);
      }
      //========================== Fill PID Table in HHypComb END =============

      //---------------------- setProbComb ---------------------------------
      for (Int_t i = 0; i < numberOfCombinations; i++) 
      {
	for (Int_t j = 0; j < nPlus; j++) 
	{
          //Int_t Pid = hypcomb->getPid( i, (numberOfParticles-1)-iPosMinus );
          //hypcomb->setProbComb( i, (numberOfParticles-1)-iPosMinus, pidPart->);
	}
      }	
		  
      //---------------------- setProbComb ---------------------------------

      //=========================== Fill PID Table in HHypComb END =============
   
//       cout<<"*******************************"<<endl;
//       cout<<"counter = "<<counter<<endl;
//       for(Int_t i=0; i<counter; i++)
//         cout<<"  PID="<<PID[i]<<" Prob(p)="<< ProbId14[i]
// 	    <<" Prob(pi+)="<< ProbId8[i]
// 	    <<" Prob(pi-)="<< ProbId9[i]<<"  Prob="
// 	    <<Prob[i]<<"  Charge="<<Charge[i]<<endl;
      //=========================== Create HHypList Container =================
      HHypList *hyplist = NULL;
      
      hyplist = (HHypList*) m_pContCatList->getNewSlot(locDummy,&index_list);
      if ( hyplist != 0 ) hyplist = new(hyplist) HHypList(index_comb);
      else return ERROR_IDX;
      //=========================== Create HHypList Container END ==============

      hyplist->setListId(exitList);
      hyplist->setProbAlg(0,0.1);
      hyplist->setProbAlg(1,0.2);
      hyplist->setProbAlg(2,0.3);
      //      hyplist->print();
    }
  else return ERROR_IDX;
  return index_list;
}

Bool_t HHypListFiller::finalize()
{
  cout<<" \n\n\n HHypListFiller::finalize() \n\n";
  return kTRUE;
} // End HHypListFiller::finalize()

Bool_t
HHypListFiller::AddTrack(Int_t particle)
{

  Int_t q;  
  q = HPidPhysicsConstants::charge(particle);
  if (-1 == q) {
    particleArrayMinus.Set(nMinus + 1);
    particleArrayMinus.AddAt(particle, nMinus);
    nMinus++;
  }
  else if (1 == q) {
    particleArrayPlus.Set(nPlus + 1);
    particleArrayPlus.AddAt(particle, nPlus);
    nPlus++;
  }
  else 
    return kFALSE;
  //[Debug]
  /*
  numberOfParticles = nPlus + nMinus;
  cout<<" HHypListFiller::AddTrack(Int_t) Part = "<<particle<<endl;
  cout<<" nMinus = "<<nMinus<<"  nPlus = "<<nPlus
      <<" NumOfPart = "<<numberOfParticles<<endl;
      */
  return kTRUE;
}

// -----------------------------------------------------------------------
// ----------       Private methods --------------------------------------
// -----------------------------------------------------------------------
Bool_t 
HHypListFiller::generateNumberOfCombinations() {
//   cout<<"[Debug] HHypListFiller::generateNumberOfCombinations()"<<endl;
//   cout<<"[Debug]\tnMinus = "<<nMinus<<"\n\tnPlus = "<<nPlus
//       <<"\n\tnumberOfPart = "<<numberOfParticles<<endl;
  Int_t loop;
  Int_t diff = 0;

  numberOfParticles = nMinus + nPlus;  

  if (nMinus != 1)    {
    cerr<< "\n ERROR HHypListFiller::generateNumberOfCombinations()"
	<< "\n only works for 1 neg track. nMinus = "<<nMinus
	<< "\n Contact : tiago.perez@exp2.physik.uni-giessen.de\n"<<endl;
    return kFALSE;
  }
  else if (nPlus != 3)    {
    cerr<< "\n ERROR HHypListFiller::generateNumberOfCombinations()"
	<< "\n only works for 3 POSITIVE TRACKS. nPlus = "<<nPlus
	<< "\n Contact : tiago.perez@exp2.physik.uni-giessen.de"<<endl;
    return kFALSE;
  }
  diff = 0;
  for (loop=1; loop< nPlus; loop++) {
    if (particleArrayPlus.At(loop-1)!=particleArrayPlus.At(loop))
      diff++;
  } 
  if (0 == diff)
    numberOfCombinations = 1;
  else if (1 == diff)
    numberOfCombinations = 3;
  else if (1 < diff)
    numberOfCombinations = 6;
  else 
    numberOfCombinations = 0;
  generateMatrix();
  return kTRUE;

} // End Bool_t HHypListFiller::generateNumberOfCombinations()

Bool_t 
HHypListFiller::generateMatrix() {
  // I don't need that any more..
  return kTRUE;
} // End HHypListFiller::generateMatrix()
