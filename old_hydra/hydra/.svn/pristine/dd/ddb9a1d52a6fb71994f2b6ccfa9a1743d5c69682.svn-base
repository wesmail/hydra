//*-- AUTHOR : Ilse Koenig
//*-- Modified : 02/11/98

/////////////////////////////////////////////////////////////
//HRuntimeDb
//
//  Administration class for parameter input/output
//
/////////////////////////////////////////////////////////////

#include <iostream.h>
#include "hades.h"
#include "hruntimedb.h"
#include "hparset.h"
#include "hpario.h"
#include "hparfileio.h"
#include "hspectrometer.h"
#include "heventfile.h"
#include "TKey.h"
#include "TClass.h"

ClassImp(HRuntimeDb)

HRuntimeDb::HRuntimeDb() {
  // constructor creates an empty list for parameter containers
  // and an empty list of event files for the version management
  containerList=new TList();
  eventFiles=new TList();
  firstInput=0;
  secondInput=0;
  output=0;
  currentFilePos=-1;
  versionsChanged=kFALSE;
}

HRuntimeDb::~HRuntimeDb() {
  // destructor
  // deletes the list of event files and all containers
  containerList->Delete();
  delete containerList;
  eventFiles->Delete();
  delete eventFiles;
}

Bool_t HRuntimeDb::addContainer(HParSet* container) {
  // adds a container to the list of containers
  Text_t* name=container->GetName();
  if (!containerList->FindObject(name)) {
    containerList->Add(container);
    TIter next(eventFiles);
    HEventFile* file;
    HParVersion* vers;
    while ((file=(HEventFile*)next())) {
      if (!file->getParVersion(name)) {
        vers=new HParVersion(name);
        file->addParVersion(vers);
      }
    }      
    return kTRUE;
  }
  cerr<<"Container with name "<<name<<" exists already!"<<endl;
  return kFALSE;
}

HParSet* HRuntimeDb::getContainer(Text_t* name) {
  // returns a pointer to the container called by name
  return (HParSet*)(containerList->FindObject(name));
}

void HRuntimeDb::removeContainer(Text_t* name) {
  // removes the container from the list and deletes it
  TObject* c=containerList->FindObject(name);
  if (c) {
    containerList->Remove(c);
    delete c;
  }
}

void HRuntimeDb::clearContainerList() {
  containerList->Clear();
}

void HRuntimeDb::addEventFile(Text_t* name,Text_t* refFile="") {
  // adds an event file at the end of the list of event files
  HEventFile* file=new HEventFile(name,refFile);
  TIter next(containerList);
  HParSet* cont;
  HParVersion* vers;
  while ((cont=(HParSet*)next())) {
    Text_t* name=cont->GetName();
    vers=new HParVersion(name);
    file->addParVersion(vers);
  }    
  eventFiles->Add(file);
  currentFilePos++;
}

void HRuntimeDb::insertEventFile(Text_t* name,Int_t pos,
                                 Text_t* refFile="") {
  // inserts an event file in the list of event files at position pos
  HEventFile* file=new HEventFile(name,refFile);
  TIter next(containerList);
  HParSet* cont;
  HParVersion* vers;
  while ((cont=(HParSet*)next())) {
    Text_t* name=cont->GetName();
    vers=new HParVersion(name);
    file->addParVersion(vers);
  }
  eventFiles->AddAt(file,pos);
  currentFilePos=pos;
}

Int_t HRuntimeDb::getEventFilePosition(Text_t* name) {
  // returns the position of the event file in the list of event files
  // returns -1 if not found
  TIter next(eventFiles);
  TObject* obj;
  int n=0;
  while ((obj=next())) {
    if (strcmp(obj->GetName(),name)==0) return n;
    n++;
  }
  return -1;
}

Int_t HRuntimeDb::getNumEventFiles() {
  // returns number of event files
  return eventFiles->GetSize();
}

TList* HRuntimeDb::getEventFileList() {
  return eventFiles;
}

void HRuntimeDb::clearEventFileList() {
  eventFiles->Delete();
}

void HRuntimeDb::setCurrentEventFile(Int_t n=-1) {
  // sets the current event file pointer to position n
  // ATTENTION: This number must be 1 less then the file to be
  // analyzed because the function getNextEventFile() increments
  // the position by 1 internally!
  if (n>=-1 && n<=eventFiles->LastIndex()) currentFilePos=n;
  else {
    cerr<<"event file number "<<n<<" doesn't exist!"<<endl;
  }
}

HEventFile* HRuntimeDb::getCurrentEventFile() {
  // returns a pointer to the current event file
  // (without calling the init() funtions of the containers)
  if (currentFilePos>=0 && currentFilePos<=eventFiles->LastIndex())
     return ((HEventFile*)eventFiles->At(currentFilePos));
  else return 0;
}

HEventFile* HRuntimeDb::getEventFile(Int_t n=0) {
  // returns a pointer to the event file position n in list
  if (n>=0 && n<=eventFiles->LastIndex())
     return ((HEventFile*)eventFiles->At(n));
  else return 0;
}

HEventFile* HRuntimeDb::getEventFile(Text_t* name) {
  // returns a pointer to the event file called by name
  return (HEventFile*)(eventFiles->FindObject(name));
}

void HRuntimeDb::removeEventFile(Text_t* name) {
  // removes the event file from the list and deletes it
  TObject* c=eventFiles->FindObject(name);
  if (c) {
    eventFiles->Remove(c);
    delete c;
  }
}

void HRuntimeDb::removeEventFile(Int_t n=0) {
  // removes the event file at position n from the list and deletes it
  if (n>=0 && n<=eventFiles->LastIndex()) {
    TObject* c=eventFiles->At(n);
    eventFiles->Remove(c);
    delete c;
  }
}

void HRuntimeDb::writeSetup() {
  // writes the setup to the output ever time the setup has changed
  HSpectrometer* spec=gHades->getSetup();
  if (spec->hasChanged() && getOutput() && output->check() && firstInput!=output) 
      spec->write(output);
}

void HRuntimeDb::writeVersions() {
  // writes the event file versions to the output
  if (getOutput() && output->check()) {
    if (versionsChanged &&
          (strcmp(output->IsA()->GetName(),"HParFileIo")==0)) {
      output->cd();
      if (gFile->IsWritable()) eventFiles->Write();
      versionsChanged=kFALSE;
    }
  }
}


Bool_t HRuntimeDb::writeContainers() {
  // writes all containers to the output
  // loops over the list of containers and calls for each the
  // function writeContainer(...)
  TIter next(containerList);
  HParSet* cont;
  Bool_t rc=kTRUE;
  HEventFile* file=0;
  if ((file=getCurrentEventFile())) {
    while ((cont=(HParSet*)next()) && rc) {
      rc=writeContainer(cont,file);
    }
  }
  if (!rc) cerr<<"error while writing container to output"<<endl;
  return rc;
}


Int_t HRuntimeDb::findOutputVersion(HParSet* cont) {
  Int_t in1=cont->getInputVersion(1);
  Int_t in2=cont->getInputVersion(2);
  HEventFile* file;
  HParVersion* vers;
  Text_t* name=cont->GetName();
  Int_t v=0;
  if (in1==-1 && in2==-1) {
    if (cont->hasChanged()) return 0;
    else {
      Int_t i=currentFilePos;
      while (i>=0) {
        file=(HEventFile*)eventFiles->At(i);
        vers=file->getParVersion(name);
        if (vers->getInputVersion(1)==in1 && vers->getInputVersion(2)==in2) {
          if ((v=vers->getRootVersion())!=0) return v;
        }
        --i;
      }
      return 0;
    }
  }
  if ((firstInput==output) && (in1>0 && in2==-1)) return in1;
  TIter next(eventFiles);
  v=0;
  while ((file=(HEventFile*)next())) {
    vers=file->getParVersion(name);
    if (vers->getInputVersion(1)==in1 && vers->getInputVersion(2)==in2) {
      if ((v=vers->getRootVersion())!=0) return v;
    }
  }
  return 0;
}


Bool_t HRuntimeDb::writeContainer(HParSet* cont, HEventFile* file) {
  // writes a container to the output if the containers has changed
  // The output might be suppressed if the changes is due an
  // initialisation from a ROOT file which serves also as output
  // or if it was already written
  Text_t* c=cont->GetName();
  HParVersion* vers=file->getParVersion(c);
  Bool_t rc=kTRUE;
  if (getOutput() && output->check()) {
    Int_t cv=0;
    if (strcmp(output->IsA()->GetName(),"HParFileIo")==0) {
      if (cont->hasChanged()) {
        cv=findOutputVersion(cont);
        if (cv==0){
          cv=cont->write(output);
          if (cv>0) {
            cout<<"***  "<<c<<" written to ROOT file   version: "<<cv<<'\n';
          }
          else return kFALSE;
        }
        vers->setRootVersion(cv);
      } else {
        if (vers->getRootVersion()==0)
            vers->setRootVersion(findOutputVersion(cont));
      }
    } else {   // might be e.g. Asci I/O
      if (cont->hasChanged()) cv=cont->write(output);
      if (cv<0) return kFALSE;
      cout<<"***  "<<c<<" written to output"<<'\n';
      vers->setRootVersion(cv);
    }
  }
  vers->setInputVersion(cont->getInputVersion(1),1);
  vers->setInputVersion(cont->getInputVersion(2),2);
  cont->setChanged(kFALSE);
  return rc;
}


Bool_t HRuntimeDb::initContainers() {
  // loops over the list of containers and calls the init()
  // function of each container if it is not static
  if (currentFilePos==-1) return kTRUE;
  HEventFile* file=getCurrentEventFile();
  Text_t* refFileName=file->getRefFileName();
  if (strlen(refFileName)<=1) {
    if (firstInput) firstInput->readVersions(file);
    if (secondInput) secondInput->readVersions(file);
  } else {
    HEventFile* refFile=getEventFile(refFileName);
    if (firstInput) firstInput->readVersions(refFile);    
    if (secondInput) secondInput->readVersions(refFile);
  }
  TIter next(containerList);
  HParSet* cont;
  Bool_t rc=kTRUE;
  cout<<'\n'<<"*************************************************************"<<'\n';
  cout<<"     initialisation for event file "<<file->GetName()<<'\n';
  cout<<"*************************************************************"<<'\n';
  while ((cont=(HParSet*)next()) && rc) {
    if (!cont->isStatic()) rc=cont->init();
    cont->print();
  }
  if (!rc) cerr<<"error occured during initialization"<<endl;
  return rc;
}


void HRuntimeDb::setContainersStatic(Bool_t f=kTRUE) {
  // sets the status flag in all containers
  // flag kTRUE sets the all 'static'
  // flag kFALSE sets the all not 'static'
  TIter next(containerList);
  HParSet* cont;
  while ((cont=(HParSet*)next())) {
    cont->setStatic(f);
  }
}


HEventFile* HRuntimeDb::getNextEventFile() {
  // checks first if the containers have changed during the analysis
  // of the last event file; the containers which have changed are
  // written to the output
  // Each container is reinitialized if necessary with the version
  // needed for the next event file.
  // return a pointer to this 'event file'.
  Bool_t rc=kTRUE;
//  if (currentFilePos==0) writeSetup();
  if (currentFilePos>=0) rc=writeContainers();
  if (!rc) return 0;
  // now get the new file
  currentFilePos++;
  if (currentFilePos<=eventFiles->LastIndex()) {
    rc=initContainers();
  }
  else saveOutput();
//  else writeVersions();
  if (rc) return (getCurrentEventFile());
  else return 0;
}  


Bool_t HRuntimeDb::readAll() {
  // reads all containers with all versions for all event files and
  // writes the containers to the output
  if (!(getOutput() && output->check())) {
    cout<<"***********************************************************"<<endl;
    cout<<"*********************  W A R N I N G  *********************"<<endl;
    cout<<"*********************   no output !   *********************"<<endl;
    cout<<"***********************************************************"<<endl;
  }
  setCurrentEventFile(-1);
  HEventFile* f;
  Int_t i=0;
  while ((f=getNextEventFile())) { i++; }
  if (i==(getNumEventFiles())) return kTRUE;
  return kFALSE;
}


Bool_t HRuntimeDb::setInputVersion(Text_t* eventFileName,Text_t* container,
                               Int_t version,Int_t inp) {
  // sets the input version of a container defined by its name and an
  // event file defined by its name taken from input with inputNumber inp
  // (1 for first input and 2 for second input)
  HEventFile* f=(HEventFile*)eventFiles->FindObject(eventFileName);
  if (f) {
    HParVersion* v=f->getParVersion(container);
    if (v) {
      v->setInputVersion(version,inp);
      return kTRUE;
    }
    else cerr<<"Container not found"<<endl;
  }
  else cerr<<"event file not found"<<endl;
  return kFALSE;
}


Bool_t HRuntimeDb::setRootOutputVersion(Text_t* eventFileName,Text_t* container,
                               Int_t version) {
  // sets the Root file output version of a container defined by its name
  // and an event file defined by its name
  // should only be used after initialization 'by hand' on the interpreter level
  HEventFile* f=(HEventFile*)eventFiles->FindObject(eventFileName);
  if (f) {
    HParVersion* v=f->getParVersion(container);
    if (v) {
      v->setRootVersion(version);
      return kTRUE;
    }
    else cerr<<"Container not found"<<endl;
  }
  else cerr<<"File not found"<<endl;
  return kFALSE;
}

void HRuntimeDb::print() {
  // prints the list of the actuzal containers, the list of the
  // event files/versions and information about input/output
  cout<<"-----------------------------------------------------------"<<'\n';
  cout<<"---------  actual containers in runtime database  ---------"<<'\n';
  containerList->ls();
  TIter next(eventFiles);
  HEventFile* file;
  cout<<"-----------------  event files, versions  -----------------"<<'\n';
  cout<<"file name"<<'\n';
  cout<<"  container  1st-inp-version  2nd-inp-version  output-version"<<'\n';
  while ((file=(HEventFile*)next())) {
    file->print();
  }
  cout<<"---------------------  input/output  ----------------------"<<'\n';
  cout<<"first Input:"<<'\n';
  if (firstInput) firstInput->print();
  else cout<<"  none"<<'\n';
  cout<<'\n'<<"second Input:"<<'\n';
  if (secondInput) secondInput->print();
  else cout<<" none"<<'\n';
  cout<<'\n'<<"Output:"<<'\n';
  if (output) output->print();
  else cout<<" none"<<'\n';
  cout<<"-----------------------------------------------------------"<<'\n';
}

void HRuntimeDb::resetInputVersions() {
  // resets all input versions in the event file list and in all
  // containers which are not static
  // is called each time a new input is set
  TIter nextFile(eventFiles);
  HEventFile* file;
  while ((file=(HEventFile*)nextFile())) {
    file->resetInputVersions();
  }
  TIter nextCont(containerList);
  HParSet* cont;
  while ((cont=(HParSet*)nextCont())) {
    if (!cont->isStatic()) cont->resetInputVersions();
  }
}

void HRuntimeDb::resetOutputVersions() {
  // resets all output versions in the event file list
  // is called each time a new output is set
  // is called also each time a new input is set which is not
  // identical with the output
  TIter nextFile(eventFiles);
  HEventFile* file;
  while ((file=(HEventFile*)nextFile())) {
    file->resetOutputVersions();
  }
}

void HRuntimeDb::resetAllVersions() {
  // resets all input and output versions in the event file list
  // and in all containers which are not static
  resetInputVersions();
  resetOutputVersions();
}

Bool_t HRuntimeDb::setFirstInput(HParIo* inp1) {
  // sets the first input pointer
  firstInput=inp1;
  if (inp1->check()==kTRUE) {
    inp1->setInputNumber(1);
    resetInputVersions();
    if (output && firstInput!=output) resetOutputVersions();
    return kTRUE;
  }
  else cerr<<"no connection to input"<<endl;
  return kFALSE;
}

Bool_t HRuntimeDb::setSecondInput(HParIo* inp2) {
  // sets the second input pointer
  secondInput=inp2;
  if (inp2->check()==kTRUE) {
    inp2->setInputNumber(2);
    resetInputVersions();
    if (output && firstInput!=output) resetOutputVersions();
    return kTRUE;
  }
  else cerr<<"no connection to input"<<endl;
  return kFALSE;
}

Bool_t HRuntimeDb::setOutput(HParIo* op) {
  // sets the output pointer
  output=op;
  if (output->check()==kTRUE) {
    resetOutputVersions();
    return kTRUE;
  }
  else cerr<<"no connection to output"<<endl;
  return kFALSE;
}

HParIo* HRuntimeDb::getFirstInput() {
  // return a pointer to the first input
  if (firstInput) firstInput->cd();
  return firstInput;
}

HParIo* HRuntimeDb::getSecondInput() {
  // return a pointer to the second input
  if (secondInput) secondInput->cd();
  return secondInput;
}
HParIo* HRuntimeDb::getOutput() {
  // return a pointer to the output
  if (output) output->cd();
  return output;
}

void HRuntimeDb::closeFirstInput() {
  if (firstInput) {
    firstInput->cd();
    firstInput->close();
    firstInput=0;
  }
}

void HRuntimeDb::closeSecondInput() {
  if (secondInput) {
    secondInput->cd();
    secondInput->close();
    secondInput=0;
  }
}

void HRuntimeDb::saveOutput() {
  // writes the setup, the versions and the containers (if not
  // yet written out) 
  // without the setup and version information the containers cannot
  // be read again!
  Bool_t rc=kTRUE;
  writeSetup();
  if (currentFilePos>=0) rc=writeContainers();
  writeVersions();
  if (!rc) cerr<<"error occured during write"<<endl;
}

void HRuntimeDb::closeOutput() {
  // calls saveOutput() and deletes then the output
  if (output) {
    saveOutput();
    resetOutputVersions();
    output=0;
  }
}



//////////////////////////////////////////////////////////////////////
/////////      code not used anymore      ////////////////////////////
//////////////////////////////////////////////////////////////////////

/* ----------------  must be changed!!!!!!  --------------------------
Bool_t HRuntimeDb::init(HParFileIo* file) {
  if (file->check()) {
    TList* list=file->getList();
    Text_t* name;
    TKey* key;
    HParSet* container;
    TIter next(list);
    while ((key=(TKey*)next())) {
      name=key->GetName();
      container=(HParSet*)key->ReadObj();
      addContainer(container);
    }
    return kTRUE;
  }
  cerr<<"cannot initialize run time database from file"<<endl;
  return kFALSE;
}
------------------------------------------------------------------  */

//Bool_t HRuntimeDb::getWriteFlag() {
//  if (!getOutput() || !output->check() || !(firstInput && secondInput)
//      && (firstInput==output || secondInput==output)) return kFALSE;
//  else return kTRUE;
////  if !(getOutput() && output->check()) return kFALSE;
////  if (vers->getRootVersion()!=cont->getRootInputVersion()
////        || vers->getOraVersion()!=cont->getOraVersion()
////        || !(firstInput==output || secondInput==output)) return kTRUE;
////  else return kFALSE;
//}

//Bool_t HRuntimeDb::write() {
//  output->cd();
//  if (output->check()==kFALSE) {
//    cerr<<"no output open"<<endl;
//    return kFALSE;
//  }
//  Bool_t rc=kTRUE;
//  if (gHades->getSetup()->write(output)==kFALSE) rc=kFALSE;
//  TIter next(containerList);
//  HParSet* cont;
//  while ((cont=(HParSet*)next())) {
//    if (cont->write(output)==kFALSE) rc=kFALSE;
//  }
//  if (rc==kFALSE) cerr<<"error occured while writing runtime database"<<endl;
//  return rc;
//}

/*  ---------------------  not used  -----------------------------
Bool_t HRuntimeDb::initAll() {
  Bool_t rc=kTRUE;
  currentFilePos=-1;  
  TIter next(eventFiles);
  HEventFile* file;
  HParSet* cont;
  HParVersion* vers;
  Int_t v;
  Text_t* c;
  while ((file=(HEventFile*)next())) {
    if (firstInput) firstInput->readVersions(file);
    if (secondInput) secondInput->readVersions(file);
    currentFilePos++;
    TIter next(containerList);
    while ((cont=(HParSet*)next()) && rc) {
      if (!cont->isStatic()) rc=cont->init();
      if (cont->hasChanged() && getOutput() && output->check()) {
        if(cont->write(output)==kFALSE) rc=kFALSE;
      }
      v=cont->getRootOutputVersion();
      c=cont->GetName();
      vers=file->getParVersion(c);
      vers->setRootVersion(v);
    }      
  }
  if (getOutput() && output->check()) {  
    if (gHades->getSetup()->write(output)==kFALSE) rc=kFALSE;
    if (strcmp(output->IsA()->GetName(),"HParFileIo")==0) {
      if (gFile->IsWritable()) eventFiles->Write();
    }
  }
  if (rc==kFALSE) cerr<<"error occured during initialization"<<endl;
  return rc;
}
------------------------------------------------------------------  */

/*  -------------------------- not used  ------------------------
HEventFile* HRuntimeDb::getNextEventFile() {
  TIter next(containerList);
  HParSet* cont;
  Bool_t rc=kTRUE;
  HEventFile* file=0;
  if (currentFilePos>=0) {
    while ((cont=(HParSet*)next()) && rc) {
      if ((file=getCurrentEventFile())) rc=writeContainer(cont,file);
    }
  }
  if (!rc) {
    cerr<<"error while writing container to output"<<endl;
    return 0;
  }
  currentFilePos++;  
  if (currentFilePos>eventFiles->LastIndex()) {
    if (getWriteFlag()) {  
      HSpectrometer* spec=gHades->getSetup();
      if (spec->hasChanged()) spec->write(output);
    }
    if (getOutput() && output->check()) {
      if (strcmp(output->IsA()->GetName(),"HParFileIo")==0) {
        if (gFile->IsWritable()) eventFiles->Write();
      }
    }
    currentFilePos=-1;
    return 0;
  }
  file=(HEventFile*)eventFiles->At(currentFilePos);
  if (firstInput) firstInput->readVersions(file);
  if (secondInput) secondInput->readVersions(file);
  next.Reset();
  rc=kTRUE;
  while ((cont=(HParSet*)next()) && rc) {
    if (!cont->isStatic()) rc=cont->init();
//    if (rc) rc=writeContainer(cont,file);
  }
  if (rc) return file;
  else cerr<<"error occured during initialization"<<endl;
  return 0;
}
----------------------------------------------------------------  */

