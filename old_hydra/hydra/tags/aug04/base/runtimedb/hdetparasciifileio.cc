//*-- AUTHOR : Ilse Koenig
//*-- modified : 17/01/2003 by Ilse Koenig


/////////////////////////////////////////////////////////////
// HDetParAsciiFileIo
//
// Class for parameter input/output from/into Ascii file
// Base class for Ascii I/O used by MDC, START detector 
//
/////////////////////////////////////////////////////////////

#include "hdetparasciifileio.h"
#include "hdetgeompar.h"
#include "hgeomcompositevolume.h"
#include "hgeomshapes.h"
#include "hparcond.h"
#include "hparamlist.h"
#include "TClass.h"

ClassImp(HDetParAsciiFileIo)

HDetParAsciiFileIo::HDetParAsciiFileIo(fstream* f) {
  // constructor
  pFile=f;
  sepLine=
    "##############################################################################\n";
}


Bool_t HDetParAsciiFileIo::findContainer(const Text_t* name) {
  // searches the container in the file
  const Int_t maxbuf=4000;
  Text_t buf[maxbuf];
  Text_t buf2[maxbuf];
  sprintf(buf2,"%s%s%s","[",name,"]");
  pFile->clear();
  pFile->seekg(0,ios::beg);
  while (!pFile->eof()) {
    pFile->getline(buf,maxbuf);
    if (buf[0]!='[') continue;
    if (!strncmp(buf,buf2,strlen(buf2))) break;
  }
  if (pFile->eof()) return kFALSE;
  return kTRUE;
}


Bool_t HDetParAsciiFileIo::checkAllFound(Int_t* set, Int_t setSize) {
  // checks if all modules have been initialized
  Bool_t allFound=kTRUE;
  for(Int_t i=0; i<setSize; i++) {
    if (set[i]==999) {
      set[i]=0;
      printf(" %i",i);
    }
    if (set[i]) allFound=kFALSE;
  }
  printf("\n");
  return allFound;
}


void HDetParAsciiFileIo::writeHeader(const Text_t* name, const Text_t* context,
                                     const Text_t* author, const Text_t* description) {
  // calls the function putAsciiHeader(...) of the parameter container
  // class and writes the header to the file
  pFile->write(sepLine,strlen(sepLine));
  pFile->write(fHeader.Data(),fHeader.Length());
  pFile->write(sepLine,strlen(sepLine));
  *pFile<<"["<<name<<"]\n";
  if (strlen(context)>0) *pFile<<"// Parameter Context: "<<context<<"\n";
  if (strlen(author)>0) *pFile<<"author:      "<<author<<"\n";
  TString t=description;
  if (!t.IsNull()) {
    t=t.ReplaceAll("\n"," \\\n");
    *pFile<<"description: "<<t<<"\n";
  }
  *pFile<<"//-----------------------------------------------------------"
            "-----------------\n";
}


void HDetParAsciiFileIo::readComment(const char* lastBuf, HParSet* pPar) {
  // reads the author or the description
  const Int_t maxbuf=4000;
  Text_t buf[maxbuf];
  TString s=lastBuf;
  TString pName, pVal;
  Ssiz_t n=s.First(':');
  Ssiz_t m=s.Last('\\');
  pName=s(0,n);
  if (m>0) pVal=s(n+1,m-n-1);
  else pVal=s(n+1,s.Length()-n-1);
  pVal=pVal.Strip(pVal.kLeading);
  while (m>0) {
    pVal+="\n";
    pFile->getline(buf,maxbuf);
    if (buf[0]!='/') {
      s=buf;
      m=s.Last('\\');
      if (m>0) pVal+=s(0,m);
      else pVal+=s;
    }
  }
  if (pName.CompareTo("author")==0) pPar->setAuthor(pVal.Data());
  else if ( pName.CompareTo("description")==0) pPar->setDescription(pVal.Data());
}

void HDetParAsciiFileIo::writeComment(HParSet* pPar) {
  TString t=pPar->getAuthor();
  if (!t.IsNull()) *pFile<<"author:      "<<t<<"\n";
  t=pPar->getDescription();
  if (!t.IsNull()) {
    t=t.ReplaceAll("\n"," \\\n");
    *pFile<<"description: "<<t<<"\n";
  }
}

Int_t HDetParAsciiFileIo::writeFile(HDetGeomPar* pPar) {
  if (pFile) {
    Text_t buf[4000];
    const Text_t* detName=pPar->getDetectorName();
    Text_t com1[]=
        "# Parameters describing the geometry of the ";
    Text_t com2[]="\n"
        "#\n"
        "# Format:\n"
        "#   [";
    Text_t com3[]="GeomLabPositions] lab positions of the modules:\n"
        "#      name of volume   name of reference module\n"
        "#      rotation matrix\n"
        "#      translation vector\n"
        "#\n"
        "#   [";
    Text_t com4[]="GeomVolumes] parameters of the volumes:\n"
        "#      name of volume   shape   name of mother\n"
        "#      x, y and z components of points \n"
        "#      rotation matrix\n"
        "#      translation vector\n"
        "#\n";
    pFile->write(sepLine,strlen(sepLine));
    sprintf(buf,"%s%s%s%s%s%s%s",
         com1,detName,com2,detName,com3,detName,com4);
    pFile->write(buf,strlen(buf));
    pFile->write(sepLine,strlen(sepLine));
    sprintf(buf,"[%s%s]\n",detName,"GeomLabPositions");
    pFile->write(buf,strlen(buf));
    writeComment(pPar);
    const char* sl="// ------------------------------------------------------\n";
    for(Int_t m=0;m<pPar->getNumModules();m++) {
      HModGeomPar* mod=pPar->getModule(m);
      if (mod) {
        pFile->write(sl,strlen(sl));
        sprintf(buf,"%s  %s\n",mod->GetName(),mod->getRefName());
        pFile->write(buf,strlen(buf));
        writeTransform(mod->getLabTransform());
      }
    }
    pFile->write(sepLine,strlen(sepLine));
    sprintf(buf,"[%s%s]\n",detName,"GeomVolumes");
    pFile->write(buf,strlen(buf));
    HGeomShapes* shapes=pPar->getShapes();
    for(Int_t m=0;m<pPar->getNumRefModules();m++) {
      HGeomCompositeVolume* mod=pPar->getRefVolume(m);
      if (mod) {
        writeVolume(mod,shapes);
        for(Int_t c=0;c<mod->getNumComponents();c++) {
          HGeomVolume* vol=mod->getComponent(c);
          if (vol) writeVolume(vol,shapes);
        }
      }
    }
    pFile->write(sepLine,strlen(sepLine));
    pPar->setChanged(kFALSE);
    return 1;
  }
  cerr<<"Output is not writable"<<endl;
  return -1;
}  


Bool_t HDetParAsciiFileIo::read(HDetGeomPar* pPar,Int_t* set) {
  if (!pFile) return kFALSE;
  pFile->clear();
  pFile->seekg(0,ios::beg);
  const char* detName=pPar->getDetectorName();
  TString key1(detName);
  key1+="GeomLabPositions";
  Bool_t allFound=readLabPositions(key1,pPar,set,pPar->getMaxSectors(),
                                   pPar->getMaxModules());
  if (pPar->isFirstInitialization() && allFound) {
    TString key2(detName);
    key2+="GeomVolumes";
    if (readVolumes(key2,pPar)==kFALSE) allFound=kFALSE;
  }
  if (allFound) {
    pPar->setInputVersion(1,inputNumber);
    pPar->setChanged();
    pPar->setNotFirstInit();
    printf("%s initialized from Ascii file\n",pPar->GetName());
  } else pPar->clear();
  return allFound;  
}


Bool_t HDetParAsciiFileIo::readLabPositions(const Text_t* keyName,
            HDetGeomPar* pPar,Int_t* set, Int_t maxSec, Int_t maxMod) {
  // reads the lab positions of all modules
  if (!findContainer(keyName)) return kFALSE;
  const Int_t maxbuf=4000;
  Text_t buf[maxbuf], name[10], ref[10];
  Int_t s=-1,m,n;
  HGeomTransform transform;
  Bool_t all=kFALSE;
  while (!all && !pFile->eof()) {             
    pFile->getline(buf,maxbuf);
    if (buf[0]=='#') all=kTRUE;
    else {
      if (buf[0]!='/') {
        if (strstr(buf,":")) readComment(buf,pPar);
        else {
          sscanf(buf,"%s%s",name,ref);
          readTransform(transform);
          if (maxSec>0) s=pPar->getSecNum(name);
          m=pPar->getModNumInMod(name);
          if (maxSec>0) n=s*maxMod+m;
          else n=m;
          HModGeomPar* mod=0;
          if (set[n] && (mod=pPar->getModule(s,m))) {
            mod->getLabTransform().setTransform(transform);
            if (pPar->isFirstInitialization()) {
              mod->SetName(name);
              mod->setRefName(ref);
              Int_t mr=pPar->getModNumInMod(ref);
              HGeomCompositeVolume* refMod=pPar->getRefVolume(mr);
              if (refMod==0) {
                Int_t num=pPar->getNumComponents();
                refMod=new HGeomCompositeVolume(num);
                refMod->SetName(ref);
                pPar->addRefVolume(refMod,mr);
              }
              mod->setVolume(refMod);
            }
            set[n]=999;
          }
        }
      }
    }
  }
  Int_t setSize=0;
  if (maxSec>0) setSize=maxSec*maxMod;
  else setSize=maxMod;
  return checkAllFound(set,setSize);
}


Bool_t HDetParAsciiFileIo::readVolumes(const Text_t* keyName,HDetGeomPar* pPar) {
  // reads the geometry parameters of all volumes
  if (!findContainer(keyName)) return kFALSE;
  const Int_t maxbuf=4000;
  Text_t buf[maxbuf], name[10], mother[10], shape[10];
  HGeomVolume volume;
  HGeomShapes* shapes=pPar->getShapes();
  Bool_t all=kFALSE;
  while (!all && !pFile->eof()) {             
    pFile->getline(buf,maxbuf);
    if (buf[0]=='#') all=kTRUE;
    else {
      if (buf[0]!='/') {
        sscanf(buf,"%s%s%s",name,shape,mother);
        TString s(shape);
        s.ToUpper();
        if (s.Length()==3) s.Append(' ');
        volume.setShape(s);
        volume.setMother(mother);
        Int_t nPoints=shapes->readPoints(pFile,&volume);
        if (nPoints==0) return kFALSE;
        HGeomTransform& t=volume.getTransform();
        readTransform(t);
        Int_t m, l;
        if (strcmp(mother,"CAVE")==0) {
          m=pPar->getModNumInMod(name);
          l=-1;
        } 
        else {
          m=pPar->getModNumInComp(name);
          l=pPar->getCompNum(name);
        }
        HGeomCompositeVolume* volu=pPar->getRefVolume(m);
        if (volu) {
          if (l==-1) {
            if (volu->getNumPoints()==0) volu->setVolumePar(volume);
          } else {
            HGeomVolume* v=volu->getComponent(l);
            if (v->getNumPoints()==0) {
              v->SetName(name);
              v->setVolumePar(volume);
            } 
          }
        }
      }
    }
  }
  Bool_t allFound=kTRUE;
  HGeomCompositeVolume* v;
  for(Int_t i=0;i<pPar->getNumRefModules();i++) {
    v=pPar->getRefVolume(i);
    if (v && v->getNumPoints()==0) allFound=kFALSE;
  }
  return allFound;
}


void HDetParAsciiFileIo::readTransform(HGeomTransform& tf) {
  const Int_t maxbuf=4000;
  Text_t buf[maxbuf];
  Double_t r[9], t[3];
  pFile->getline(buf,maxbuf);
  sscanf(buf,"%lf%lf%lf%lf%lf%lf%lf%lf%lf",&r[0],&r[1],&r[2],
         &r[3],&r[4],&r[5],&r[6],&r[7],&r[8]);
  pFile->getline(buf,maxbuf);
  sscanf(buf,"%lf%lf%lf",&t[0],&t[1],&t[2]);
  tf.setRotMatrix(r);
  tf.setTransVector(t);
}


void HDetParAsciiFileIo::writeTransform(const HGeomTransform& tf) {
  Text_t buf[155];
  const HGeomRotation& r=tf.getRotMatrix();
  sprintf(buf,"%10.7f%11.7f%11.7f%11.7f%11.7f%11.7f%11.7f%11.7f%11.7f\n",
            r(0),r(1),r(2),r(3),r(4),r(5),r(6),r(7),r(8));
  pFile->write(buf,strlen(buf));
  const HGeomVector& v=tf.getTransVector();
  sprintf(buf,"%9.3f%10.3f%10.3f\n",v(0),v(1),v(2));
  pFile->write(buf,strlen(buf));
}


Bool_t HDetParAsciiFileIo::readVolume(HGeomVolume* volu,HGeomShapes* shapes,
                                    Text_t* buf) {
  Text_t name[10], mother[10], shape[10];
  sscanf(buf,"%s%s%s",name,shape,mother);
  volu->SetName(name);
  volu->setMother(mother);
  TString s(shape);
  s.ToUpper();
  if (s.Length()==3) s.Append(' ');
  volu->setShape(s);
  Int_t nPoints=shapes->readPoints(pFile,volu);
  if (nPoints==0) return kFALSE;
  HGeomTransform& t=volu->getTransform();
  readTransform(t);
  return kTRUE;
}


void HDetParAsciiFileIo::writeVolume(HGeomVolume* volu,HGeomShapes* shapes) {
  Text_t buf[120];
  sprintf(buf,"%s\n",
          "// ------------------------------------------------------");
  pFile->write(buf,strlen(buf));
  sprintf(buf,"%s  %s  %s\n",volu->GetName(),
            (const char*)volu->getShape(),(const char*)volu->getMother());
  pFile->write(buf,strlen(buf));
  shapes->writePoints(pFile,volu);
  writeTransform(volu->getTransform());
}


Bool_t HDetParAsciiFileIo::readCond(HParCond* pPar) {
  if (!pFile) return kFALSE;
  pFile->clear();
  pFile->seekg(0,ios::beg);
  Text_t* name=(char*)pPar->GetName();
  if (!findContainer(name)) return kFALSE;
  HParamList* paramList = new HParamList;
  const Int_t maxbuf=8000;
  Text_t buf[maxbuf];
  buf[0]='\0';
  TString s, pName, pVal, pType;
  Bool_t isBinary=kFALSE;
  while (buf[0]!='#' && !pFile->eof()) {             
    pFile->getline(buf,maxbuf);
    if (buf[0]!='/' && buf[0]!='#') {
      s=buf;
      Ssiz_t n=s.First(':');
      if (n==-1) {
        cerr<<"Error in Ascii File input for parameter container "
            <<name<<":"<<endl<<"Missing backslash for parameter "<<pName<<endl;
        return  kFALSE;
      } 
      Ssiz_t m=s.Last('\\');
      pName=s(0,n);
      if (m>0) s=s(n+1,m-n-1);
      else s=s(n+1,s.Length()-n-1);
      s=s.Strip(s.kLeading);
      if (pName.CompareTo("author")==0
          || pName.CompareTo("description")==0) pType='s';
      else {
        pType=s[0];
        if (s.Length()>1 && s[1]=='B') {
          isBinary=kTRUE;
          pType+="Bin";
        } else {
          s=s(1,s.Length()-1);
          s=s.Strip(s.kLeading);
        }
      }
      if (!isBinary) {
        if (!(pType[0]=='i'||pType[0]=='f'||pType[0]=='d'
	      ||pType[0]=='l'||pType[0]=='s')) {
	  cerr<<pName<<": Unsupported type "<<pType<<endl;
          delete paramList;
          return kFALSE;
        }
        if (pType[0]=='s') {
          pVal=s;
          if (m>0) pVal+="\n";
        }
        else pVal=s.ReplaceAll(" ","");
        while (m>0) {
          pFile->getline(buf,maxbuf);
          if (buf[0]!='/') {
            s=buf;
            if (pType[0]!='s') s=s.ReplaceAll(" ","");
            m=s.Last('\\');
            if (m>0) {
              pVal+=s(0,m);
              if (pType[0]=='s') pVal+="\n";
            }
            else pVal+=s;
          }
        }
        Int_t nVal=1;
        if (pType[0]!='s' )
          for(Int_t i=0;i<pVal.Length();i++) { if (pVal[i]==',') nVal++; }
        paramList->add(pName.Data(),pVal.Data(),pType[0],nVal);
      } else {
        UChar_t* val=0;
        Int_t length=0;
    	if (pType.CompareTo("hBin")==0) {
            Short_t v=0;
            val=readArray(v,"%hi",length);
            if (val && length>0)
              paramList->addBinary(pName.Data(),(Short_t*)val,length/sizeof(Short_t));
        } else if (pType.CompareTo("iBin")==0) {
            Int_t v=0;
            val=readArray(v,"%i",length);
            if (val && length>0)
              paramList->addBinary(pName.Data(),(Int_t*)val,length/sizeof(Int_t));
        } else if  (pType.CompareTo("fBin")==0){
            Float_t v=0;
            val=readArray(v,"%f",length);
            if (val && length>0)
              paramList->addBinary(pName.Data(),(Float_t*)val,length/sizeof(Float_t));
        } else if (pType.CompareTo("dBin")==0) {
            Double_t v=0;
            val=readArray(v,"%lf",length);
            if (val && length>0)
              paramList->addBinary(pName.Data(),(Double_t*)val,length/sizeof(Double_t));
        } else {
	    cerr<<pName<<": Unsupported type "<<pType<<endl;
            delete paramList;
            return kFALSE;
	}
        if (val) delete [] val;
      }
    }
  }
  HParamObj* p=paramList->find("author");
  if (p) pPar->setAuthor(p->getParamValue());
  p=paramList->find("description");
  if (p) pPar->setDescription(p->getParamValue());
  Bool_t allFound=pPar->getParams(paramList);
  if (allFound) { 
    pPar->setInputVersion(1,inputNumber);
    pPar->setChanged();
    printf("%s initialized from Ascii file\n",name);
  } else pPar->setInputVersion(-1,inputNumber);
  delete paramList;
  return allFound;  
}


template <class type> UChar_t* HDetParAsciiFileIo::readArray(
              type t,char* format,Int_t& length) {
  const Int_t st=sizeof(t);
  const Int_t maxbuf=8000;
  const Int_t bufSizeExt=10000*st;
  const char d[]=",";
  Text_t buf[maxbuf];
  Int_t m=1, l=0, bufSize=bufSizeExt ;
  UChar_t* val=new UChar_t[bufSize];
  do {             
    pFile->getline(buf,maxbuf);
    if (buf[0]!='/' && buf[0]!='#') {
      TString s=buf;
      s=s.ReplaceAll(" ","");
      m=s.Last('\\');
      if (m>0) s=s(0,s.Length()-2);
      if ((bufSize-1000)<l) {
        bufSize+=bufSizeExt;
        UChar_t* va=new UChar_t[bufSize];
        memcpy(va,val,l);
        delete [] val;
        val=va;
      }
      char* ss=strtok((char*)s.Data(),d);
      while (ss!=0) {
        sscanf(ss,format,&t);
        memcpy(&val[l],&t,st);
        l+=st;
        ss=strtok(NULL,d);
      }
    }
  } while (buf[0]!='#' && !pFile->eof() && m>0);
  length=l;
  return val;
}


Int_t HDetParAsciiFileIo::writeCond(HParCond* pPar) {
  if (pFile) {
    const Text_t* name=pPar->GetName();
    const Text_t* context=pPar->getParamContext();
    *pFile<<sepLine;
    *pFile<<"# Class:   "<<name<<"\n# Context: "<<context<<"\n";
    *pFile<<sepLine;
    *pFile<<"["<<name<<"]\n";
    writeComment(pPar);
    *pFile<<"//-----------------------------------------------------------"
            "-----------------\n";
    HParamList* paramList = new HParamList;
    pPar->putParams(paramList);
    TList* pList=paramList->getList();
    TIter next(pList);
    HParamObj* po;
    while ((po=(HParamObj*)next())) {
      char pType=po->getParamType();
      if (po->getNumParams() == 1) {
        *pFile<<po->GetName()<<":  "<<pType<<"  "<<po->getParamValue()<<"\n";
      } else {
        *pFile<<po->GetName()<<":  "<<pType<<" \\\n";
        TString s=po->getParamValue();
        if (pType=='s') s.ReplaceAll("\n"," \\\n");
        else {
          s.ReplaceAll(",",", ");
          TString ss;
          Ssiz_t l;
          do {
            l=s.Length();
            if (l>76) {
              ss=s(0,76);
              Ssiz_t n=ss.Last(' '); 
              ss=s(0,n);                  
              s=s(n+1,l);
              ss+=" \\";
            } else ss=s;
            *pFile<<"  "<<ss<<"\n";
          } while (l>76);
        }
      }
    }
    TList* pBinList=paramList->getBinaryList();
    TIter nextBin(pBinList);
    HParamBinObj* pbo;
    while ((pbo=(HParamBinObj*)nextBin())) {
      TString paramType=pbo->getParamType();
      *pFile<<pbo->GetName()<<":  "<<paramType<<" \\\n";
      Int_t l=0, i=0, k=0;
      TString format;
      if (paramType.CompareTo("hBin")==0) {
        l=pbo->getLength()/sizeof(Int_t);
        Int_t* paramValue=(Int_t*)pbo->getParamValue();
        while (k<l-1) {
          *pFile<<paramValue[k]<<", ";
          i++;
          k++;  
          if (i==10) {
            *pFile<<"\\\n";
            i=0;
          }
        }
        *pFile<<paramValue[l-1]<<"\n";
      } else if (paramType.CompareTo("iBin")==0) {
        l=pbo->getLength()/sizeof(Int_t);
        Int_t* paramValue=(Int_t*)pbo->getParamValue();
        while (k<l-1) {
          *pFile<<paramValue[k]<<", ";
          i++;
          k++;  
          if (i==10) {
            *pFile<<"\\\n";
            i=0;
          }
        }
        *pFile<<paramValue[l-1]<<"\n";
      } else if (paramType.CompareTo("fBin")==0) {
        l=pbo->getLength()/sizeof(Float_t);
        Float_t* paramValue=(Float_t*)pbo->getParamValue();
        while (k<l-1) {
          *pFile<<paramValue[k]<<", ";
          i++;
          k++;
          if (i==10) {
            *pFile<<"\\\n";
            i=0;
          }
        }
        *pFile<<paramValue[l-1]<<"\n";
      } else if (paramType.CompareTo("dBin")==0) {
        l=pbo->getLength()/sizeof(Double_t);
        Double_t* paramValue=(Double_t*)pbo->getParamValue();
        while (k<l-1) {
          *pFile<<paramValue[k]<<", ";
          i++;
          k++;  
          if (i==10) {
            *pFile<<"\\\n";
            i=0;
          }
        }
        *pFile<<paramValue[l-1]<<"\n";
      } else {
        Error("HDetParAsciiFileIo::writeCond(HParCond*)",
              "Type %s of parameter %s not supported by ASCII I/O",
              paramType.Data(),pbo->GetName());
        return kFALSE;
      }
    }
    *pFile<<sepLine;
    delete paramList;
    pPar->setChanged(kFALSE);
    return 1;
  }
  cerr<<"Output is not writable"<<endl;
  return -1;
}  
