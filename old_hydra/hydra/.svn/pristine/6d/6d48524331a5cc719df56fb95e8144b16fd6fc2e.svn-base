//*-- AUTHOR : Denis Bertini
//*-- modified : 28/05/99 by Ilse Koenig

/////////////////////////////////////////////////////////////
// HMdcParAsciiFileIo
//
// Class for Mdc parameter input/output from/into Ascii file
//
//
// This class replaces the former class HMdcParAsciIo.
/////////////////////////////////////////////////////////////

#include "hmdcparasciifileio.h"
#include "hades.h"
#include "hspectrometer.h"
#include "hmdcdetector.h"
#include "hmdcrawstruct.h"
#include "hmdcgeomstruct.h"
#include "hmdcmboreadout.h"
#include "hmdccalpar.h"
#include "hmdclookupgeom.h"
#include "hmdclookupraw.h"
#include "hmdcdigitpar.h"
#include "hmdclayergeompar.h"
#include "hmdcgeompar.h"

ClassImp(HMdcParAsciiFileIo)

HMdcParAsciiFileIo::HMdcParAsciiFileIo(fstream* f) : HDetParAsciiFileIo(f) {
  // constructor calls the base class constructor
  fName="HMdcParIo";
}


Bool_t HMdcParAsciiFileIo::init(HParSet* pPar,Int_t* set) {
  // calls the appropriate read function of the container
  const  Text_t* name=pPar->GetName();
  if (pFile) {
    pFile->clear();
    pFile->seekg(0,ios::beg);
    if (strcmp(name,"MdcRawStruct")==0) return read((HMdcRawStruct*)pPar,set);
    if (strcmp(name,"MdcGeomStruct")==0) return read((HMdcGeomStruct*)pPar,set);
    if (strcmp(name,"MdcMboReadout")==0) return read((HMdcMboReadout*)pPar,set);
    if (strcmp(name,"MdcCalPar")==0) return read((HMdcCalPar*)pPar,set);
    if (strcmp(name,"MdcLookupGeom")==0) return read((HMdcLookupGeom*)pPar,set);
    if (strcmp(name,"MdcDigitPar")==0) return read((HMdcDigitPar*)pPar,set);
    if (strcmp(name,"MdcLayerGeomPar")==0)
        return read((HMdcLayerGeomPar*)pPar,set);
    if (strcmp(name,"MdcGeomPar")==0)
        return HDetParAsciiFileIo::read((HDetGeomPar*)pPar,set);
  }
  cerr<<"initialization of "<<name<<" not possible from file!"<<endl;
  return kFALSE;
}


Int_t HMdcParAsciiFileIo::write(HParSet* pPar) {
  // calls the appropriate write function of the container
  const  Text_t* name=pPar->GetName();
  if (pFile) {
    if (strcmp(name,"MdcRawStruct")==0) return writeFile((HMdcRawStruct*)pPar);
    if (strcmp(name,"MdcGeomStruct")==0) return writeFile((HMdcGeomStruct*)pPar);
    if (strcmp(name,"MdcMboReadout")==0) return writeFile((HMdcMboReadout*)pPar);
    if (strcmp(name,"MdcCalPar")==0) return writeFile((HMdcCalPar*)pPar);
    if (strcmp(name,"MdcLookupGeom")==0) return writeFile((HMdcLookupGeom*)pPar);
    if (strcmp(name,"MdcLookupRaw")==0) return writeFile((HMdcLookupRaw*)pPar);
    if (strcmp(name,"MdcDigitPar")==0) return writeFile((HMdcDigitPar*)pPar);
    if (strcmp(name,"MdcLayerGeomPar")==0)
        return writeFile((HMdcLayerGeomPar*)pPar);
    if (strcmp(name,"MdcGeomPar")==0) return HDetParAsciiFileIo::writeFile((HDetGeomPar*)pPar);
  }
  cerr<<name<<" could not be written to Ascii file"<<endl;
  return kFALSE;
}


Bool_t HMdcParAsciiFileIo::write(HDetector*) {
  return kTRUE;  // not implemented!
}


Int_t HMdcParAsciiFileIo::writeFile(HMdcRawStruct* pPar) {
  if (pFile) {
    Text_t buf[155];
    Text_t com[]="########################################################\n"
               "# Parameters describing the hardware of the MDC\n"
               "# Format:\n"
               "# sector   module   motherboard index   motherboard name\n"
               "#    number of TDC channels on the board\n"
               "########################################################\n";
    pFile->write(com,strlen(com));
    sprintf(buf,"[%s]\n",pPar->GetName());
    pFile->write(buf,strlen(buf));
    HMdcRawStruct& pp = *pPar;
    Int_t nTdcs;
    for(Int_t s=0;s<6;s++) {
      HMdcRawSecStru& sec= pp[s];
      for(Int_t m=0;m<4;m++) {
        HMdcRawModStru& mod=sec[m];
        for(Int_t l=0;l<16;l++) {
          HMdcRawMothStru& moth=mod[l];
          nTdcs=moth.getNTdcs();
          if (nTdcs>0) {
            sprintf(buf,"%3i%3i%4i  %s%6i\n",
                    s,m,l,moth.GetName(),nTdcs);
            pFile->write(buf,strlen(buf));
          }
        }
      }
    }
    sprintf(buf,"########################################################\n");
    pFile->write(buf,strlen(buf));
    pPar->setChanged(kFALSE);
    return 1;
  }
  cerr<<"Output is not writable"<<endl;
  return -1;
}  


Int_t HMdcParAsciiFileIo::writeFile(HMdcMboReadout* pPar) {
  if (pFile) {
    Text_t buf[155];
    Text_t com[]="########################################################\n"
               "# Readout addresses of all MDC motherboards\n"
               "# Format:\n"
               "# sector   module   MBO index   MBO name\n"
               "#    BUS   BUS position   ROC   SAM position   SAM\n"
               "########################################################\n";
    pFile->write(com,strlen(com));
    sprintf(buf,"[%s]\n",pPar->GetName());
    pFile->write(buf,strlen(buf));
    HMdcMboReadout& pp = *pPar;
    for(Int_t s=0;s<pp.getSize();s++) {
      HMdcMboReadoutSec& sec= pp[s];
      for(Int_t m=0;m<sec.getSize();m++) {
        HMdcMboReadoutMod& mod=sec[m];
        for(Int_t l=0;l<mod.getSize();l++) {
          HMdcMboReadoutMoth& moth=mod[l];
          Int_t bus=moth.getBus();
          if (bus!=-1) {
            sprintf(buf,"%3i%3i%4i  %s%6i%3i%3i%4i%3i\n",s,m,l,moth.GetName(),
                bus,moth.getBusPos(),moth.getRoc(),moth.getSamPos(),
                moth.getSam());
            pFile->write(buf,strlen(buf));
          }
        }
      }
    }
    sprintf(buf,"########################################################\n");
    pFile->write(buf,strlen(buf));
    pPar->setChanged(kFALSE);
    return 1;
  }
  cerr<<"Output is not writable"<<endl;
  return -1;
}  


Int_t HMdcParAsciiFileIo::writeFile(HMdcLookupGeom* pPar) {
  if (pFile) {
    Text_t buf[155];
    Text_t com[]="########################################################\n"
               "# Mdc lookup table (hardware->geometry)\n"
               "# Format:\n"
               "# sector   module   motherboard index   TDC channel \n"
               "#    layer   cell\n"
               "########################################################\n";
    pFile->write(com,strlen(com));
    sprintf(buf,"[%s]\n",pPar->GetName());
    pFile->write(buf,strlen(buf));
    HMdcLookupGeom& pp = *pPar;
    Int_t layer, cell;
    for(Int_t s=0;s<6;s++) {
      HMdcLookupGSec& sec= pp[s];
      for(Int_t m=0;m<4;m++) {
        HMdcLookupGMod& mod=sec[m];
        for(Int_t l=0;l<16;l++) {
          HMdcLookupMoth& moth=mod[l];
          for(Int_t c=0;c<moth.getSize();c++) {
            HMdcLookupChan& chan=moth[c];
            layer=chan.getNLayer();
            cell=chan.getNCell();
            if (cell!=-1) {
              sprintf(buf,"%3i%3i%4i%4i%6i%5i\n",s,m,l,c,layer,cell);
              pFile->write(buf,strlen(buf));
            }
          }
        }
      }
    }
    sprintf(buf,"########################################################\n");
    pFile->write(buf,strlen(buf));
    pPar->setChanged(kFALSE);
    return 1;
  }
  cerr<<"Output is not writable"<<endl;
  return -1;
}  


Int_t HMdcParAsciiFileIo::writeFile(HMdcLookupRaw* pPar) {
  if (pFile) {
    Text_t buf[155];
    Text_t com[]="########################################################\n"
               "# Mdc lookup table (geometry->hardware)\n"
               "# Format:\n"
               "# sector   module   layer   cell\n"
               "#     motherboard index   TDC channel \n"
               "########################################################\n";
    pFile->write(com,strlen(com));
    sprintf(buf,"[%s]\n",pPar->GetName());
    pFile->write(buf,strlen(buf));
    HMdcLookupRaw& pp = *pPar;
    Int_t mbo, chan;
    for(Int_t s=0;s<6;s++) {
      HMdcLookupRSec& sec= pp[s];
      for(Int_t m=0;m<4;m++) {
        HMdcLookupRMod& mod=sec[m];
        for(Int_t l=0;l<mod.getSize();l++) {
          HMdcLookupLayer& layer=mod[l];
          for(Int_t c=0;c<layer.getSize();c++) {
            HMdcLookupCell& cell=layer[c];
            mbo=cell.getNMoth();
            chan=cell.getNChan();
            if (chan!=-1) {
              sprintf(buf,"%3i%3i%3i%5i%6i%4i\n",s,m,l,c,mbo,chan);
              pFile->write(buf,strlen(buf));
            }
          }
        }
      }
    }
    sprintf(buf,"########################################################\n");
    pFile->write(buf,strlen(buf));
    pPar->setChanged(kFALSE);
    return 1;
  }
  cerr<<"Output is not writable"<<endl;
  return -1;
}  


Int_t  HMdcParAsciiFileIo::writeFile(HMdcGeomStruct* pPar){
  if (pFile) {
    Text_t buf[155];
    Text_t com[]="########################################################\n"
               "# Parameters describing the geometrical sizes of the MDC\n"
               "# Format:\n"
               "# sector   module   layer   max number of wires\n"
               "########################################################\n";
    pFile->write(com,strlen(com));
    sprintf(buf,"[%s]\n",pPar->GetName());
    pFile->write(buf,strlen(buf));
    HMdcGeomStruct& pp = *pPar;
    for(Int_t s=0;s<6;s++) {
      HMdcGeomSecStru& sec= pp[s];
      for(Int_t m=0;m<4;m++) {
        HMdcGeomModStru& mod=sec[m];
        for(Int_t l=0;l<6;l++) {
          Int_t ncell= mod[l];
          if (ncell) {
            sprintf(buf,"%3i%3i%3i%5i\n", s, m, l, ncell);
            pFile->write(buf,strlen(buf));
          }
        }
      }
    }
    sprintf(buf,"########################################################\n");
    pFile->write(buf,strlen(buf));
    pPar->setChanged(kFALSE);
    return 1;
  }
  cerr<<"Output is not writable"<<endl;
  return -1;
}


Int_t  HMdcParAsciiFileIo::writeFile(HMdcCalPar* pPar){
  if (pFile) {
    Text_t com[]="########################################################\n"
               "# Calibration parameters of the MDC\n"
               "# Format:\n"
               "# sector   module   layer   cell   slope   offset\n"
               "########################################################\n";
    pFile->write(com,strlen(com));
    Text_t buf[155];
    Float_t slo, off;
    sprintf(buf,"[%4s]\n",pPar->GetName());
    pFile->write(buf,strlen(buf));
    HMdcCalPar& pp = *pPar;
    for(Int_t s=0;s<6;s++) {
      HMdcCalParSec& sec=pp[s];
      for(Int_t m=0;m<4;m++) {
        HMdcCalParMod& mod=sec[m];
        for(Int_t l=0;l<6;l++) {
          HMdcCalParLayer& lay=mod[l];
          Int_t nc=lay.getSize();
          for(Int_t n=0;n<nc;n++) {
            HMdcCalParCell& cell=lay[n];
            slo=cell.getSlope();
            off=cell.getOffset();
            sprintf(buf,"%3i%3i%3i%5i%8.4f%8.4f\n", s, m, l, n, slo, off);
            pFile->write(buf,strlen(buf));
          }
        }
      }
    }
    sprintf(buf,"########################################################\n");
    pFile->write(buf,strlen(buf));
    pPar->setChanged(kFALSE);
    return 1;
  }
  cerr<<"Output is not writable"<<endl;
  return -1;
}


Int_t  HMdcParAsciiFileIo::writeFile(HMdcDigitPar* pPar){
  if (pFile) {
    Text_t buf[155];
    Text_t com[]="########################################################\n"
               "# Digitization parameters of the MDC\n"
               "# Format:\n"
               "# sector   module   layer   drift velocity  efficiency\n"
               "#       5 fit parameters\n"
               "########################################################\n";
    pFile->write(com,strlen(com));
    sprintf(buf,"[%s]\n",pPar->GetName());
    pFile->write(buf,strlen(buf));
    HMdcDigitPar& pp = *pPar;
    for(Int_t s=0;s<6;s++) {
      HMdcDigitParSec& sec= pp[s];
      for(Int_t m=0;m<4;m++) {
        HMdcDigitParMod& mod=sec[m];
        for(Int_t l=0;l<6;l++) {
          HMdcDigitParLayer& lay=mod[l];
          Float_t vel=lay.getDriftVelocity();
          if (vel>0.0F) {
            sprintf(buf,"%3i%3i%3i%6.2f%8.4f%8.3f%8.3f%8.3f%8.3f%8.3f\n",
		    s,m,l,vel,lay.getMdcEfficiency(),lay.getSpacePar1(),
		    lay.getSpacePar2(),lay.getSpacePar3(),
		    lay.getSpacePar4(),lay.getSpacePar5());
            pFile->write(buf,strlen(buf));
          }
        }
      }
    }
    sprintf(buf,"########################################################\n");
    pFile->write(buf,strlen(buf));
    pPar->setChanged(kFALSE);
    return 1;
  }
  cerr<<"Output is not writable"<<endl;
  return -1;
}


Int_t  HMdcParAsciiFileIo::writeFile(HMdcLayerGeomPar* pPar){
  if (pFile) {
    Text_t buf[155];
    Text_t com[]="########################################################\n"
               "# Geometry parameters for digitization of the MDC\n"
               "# Format:\n"
               "# sector   module   layer\n"
               "#       pitch\n"
               "#       distace of cathode planes\n"
               "#       wire orientation\n"
               "#       number of central wire\n"
               "########################################################\n";
    pFile->write(com,strlen(com));
    sprintf(buf,"[%s]\n",pPar->GetName());
    pFile->write(buf,strlen(buf));
    HMdcLayerGeomPar& pp = *pPar;
    for(Int_t s=0;s<6;s++) {
      HMdcLayerGeomParSec& sec= pp[s];
      for(Int_t m=0;m<4;m++) {
        HMdcLayerGeomParMod& mod=sec[m];
        for(Int_t l=0;l<6;l++) {
          HMdcLayerGeomParLay& lay=mod[l];
          Int_t n=lay.getNumWires();
          if (n>0) {
            sprintf(buf,"%3i%3i%3i%8.4f%8.4f%8.2f%8.4f\n",
                s,m,l,lay.getPitch(),lay.getCatDist(),lay.getWireOrient(),
                lay.getCentralWireNr());
            pFile->write(buf,strlen(buf));
          }
        }
      }
    }
    sprintf(buf,"########################################################\n");
    pFile->write(buf,strlen(buf));
    pPar->setChanged(kFALSE);
    return 1;
  }
  cerr<<"Output is not writable"<<endl;
  return -1;
}


Bool_t HMdcParAsciiFileIo::read(HMdcGeomStruct* pPar,Int_t* set) {
  if (pFile){
    const Int_t MAXBUF=155;
    Text_t buf[MAXBUF],key[20];
    Int_t sector, module, layer, nwires, pos;
    Char_t c;
    Bool_t all=kFALSE;
    while (!all && !pFile->eof()) {
      pFile->getline(buf, MAXBUF);
      if( (buf[0]=='[')) {
        sscanf(buf,"%s",key);
        if(strstr(buf,pPar->GetName())!=0){
          while (!all && !pFile->eof()) {             
            c=pFile->get();
            if (c=='#' || c=='/') all=kTRUE;   
            else {
              pFile->putback(c);
              pFile->getline(buf,MAXBUF);
              sscanf(buf,"%i%i%i%i",
                     &sector,&module,&layer,&nwires);
              pos=sector*4+module;
              if (set[pos]) {
                HMdcGeomStruct &pp = *pPar;
                pp[sector][module].fill(layer,nwires);
                set[pos]=999;
              }
            } 
	  }
	}
      }
    }
    if (all) {
      pPar->setInputVersion(1,inputNumber);
      pPar->setChanged();
      printf("MdcGeomStruct initialized from Ascii file\n");
      return checkAllFound(set);
    }
  }
  return kFALSE;
}


Bool_t HMdcParAsciiFileIo::read(HMdcRawStruct* pPar,Int_t* set) {
  if (pFile) {
    const Int_t maxbuf=155;
    Text_t buf[maxbuf], mboName[10], key[20];
    Char_t c;
    Int_t sec, mod, mboInd, num;
    Bool_t all=kFALSE;
    while (!all && !pFile->eof()) {
      pFile->getline(buf,maxbuf);
      if (buf[0]=='[') {
        sscanf(buf,"%s",key);
        if (strstr(buf,pPar->GetName())!=0) {
          while (!all && !pFile->eof()) {
            c=pFile->get();
            if (c=='#' || c=='/') all=kTRUE;
            else {
              pFile->putback(c);
              pFile->getline(buf,maxbuf);
              sscanf(buf,"%i%i%i%s%i",
                     &sec,&mod,&mboInd,mboName,&num);
              Int_t n=sec*4+mod;
              if (set[n]) {
                HMdcRawMothStru& r=(*pPar)[sec][mod][mboInd];
                r.SetName(mboName);
                r.setNTdcs(num);
                set[n]=999;
              }
            }
          }
        }
      }
    }
    if (all) {
      pPar->setInputVersion(1,inputNumber);
      pPar->setChanged();
      pPar->setStatic();
      printf("MdcRawStruct initialized from Ascii file\n");
      return checkAllFound(set);
    }
  }
  return kFALSE;
} 


Bool_t HMdcParAsciiFileIo::read(HMdcMboReadout* pPar,Int_t* set) {
  if (pFile) {
    const Int_t maxbuf=155;
    Text_t buf[maxbuf], mboName[10], key[20];
    Char_t c;
    Int_t sec, mod, mboInd, bus, busPos, roc, sam, samPos;
    Bool_t all=kFALSE;
    while (!all && !pFile->eof()) {
      pFile->getline(buf,maxbuf);
      if (buf[0]=='[') {
        sscanf(buf,"%s",key);
        if (strstr(buf,pPar->GetName())!=0) {
          while (!all && !pFile->eof()) {
            c=pFile->get();
            if (c=='#' || c=='/') all=kTRUE;
            else {
              pFile->putback(c);
              pFile->getline(buf,maxbuf);
              sscanf(buf,"%i%i%i%s%i%i%i%i%i",
                     &sec,&mod,&mboInd,mboName,&bus,&busPos,&roc,&samPos,&sam);
              Int_t n=sec*4+mod;
              if (set[n]) {
                HMdcMboReadoutMoth& r=(*pPar)[sec][mod][mboInd];
                if (strcmp(r.GetName(),mboName)!=0) {
                  cerr<<"MBO names are different in MdcRawStruct and in Ascii file"
                      <<endl;
                  return kFALSE;
                }
                r.setBus(bus);
                r.setBusPos(busPos);
                r.setRoc(roc);
                r.setSamPos(samPos);
                r.setSam(sam);
                set[n]=999;
              }
            }
          }
        }
      }
    }
    if (all) {
      pPar->setInputVersion(1,inputNumber);
      pPar->setChanged();
      pPar->setStatic();
      printf("MdcMboReadout initialized from Ascii file\n");
      return checkAllFound(set);
    }
  }
  return kFALSE;
} 


Bool_t HMdcParAsciiFileIo::read(HMdcCalPar* pPar,Int_t* set) {
  if (pFile) {
    const Int_t maxbuf=155;
    Text_t buf[maxbuf], key[20];
    Char_t c;
    Int_t sector, module, layer, cell;
    Float_t slope=1.F, offset=0.F;
    Bool_t all=kFALSE;
    while (!all && !pFile->eof()) {
      pFile->getline(buf,maxbuf);
      if (buf[0]=='[') {
        sscanf(buf,"%s",key);
        if (strstr(buf,pPar->GetName())!=0) {
          while (!all && !pFile->eof()) {
            c=pFile->get();
            if (c=='#' || c=='/') all=kTRUE;
            else {
              pFile->putback(c);
              pFile->getline(buf,maxbuf);
              sscanf(buf,"%i%i%i%i%f%f",
                             &sector,&module,&layer,&cell,&slope,&offset);
              Int_t n=sector*4+module;
              if (set[n]) {
                HMdcCalParCell& r=
                    (*pPar)[sector][module][layer][cell];
                r.setSlope(slope);
                r.setOffset(offset);
                set[n]=999;
              }
            }
          }
        }
      }
    }
    if (all) {
      pPar->setInputVersion(1,inputNumber);
      pPar->setChanged();
      printf("MdcCalPar initialized from Ascii file\n");
      return checkAllFound(set);
    }
  }
  return kFALSE;
}


Bool_t HMdcParAsciiFileIo::read(HMdcLookupGeom* pPar,Int_t* set) {
  if (pFile) {
    const Int_t maxbuf=155;
    Text_t buf[maxbuf], key[20];
    Char_t c;
    Int_t sec, mod, mboInd, chan, layer, wire;
    Bool_t all=kFALSE;
    while (!all && !pFile->eof()) {
      pFile->getline(buf,maxbuf);
      if (buf[0]=='[') {
        sscanf(buf,"%s",key);
        if (strstr(buf,pPar->GetName())!=0) {
          while (!all && !pFile->eof()) {
            c=pFile->get();
            if (c=='#' || c=='/') all=kTRUE;
            else {
              pFile->putback(c);
              pFile->getline(buf,maxbuf);
              sscanf(buf,"%i%i%i%i%i%i",
                     &sec,&mod,&mboInd,&chan,&layer,&wire);
              Int_t n=sec*4+mod;
              if (set[n]) {
                HMdcLookupChan& r=
                   (*pPar)[sec][mod][mboInd][chan];
                r.fill(layer,wire);
                set[n]=999;
              } 
            }
          }
        }
      }
    }
    if (all) {
      pPar->setInputVersion(1,inputNumber);
      pPar->setChanged();
      printf("MdcLookupGeom initialized from Ascii file\n");
      return checkAllFound(set);
    }
  }
  return kFALSE;
} 


Bool_t HMdcParAsciiFileIo::read(HMdcDigitPar* pPar,Int_t* set) {
  if (pFile) {
    const Int_t maxbuf=155;
    Text_t buf[maxbuf], key[20];
    Char_t c;
    Int_t sector, module, layer, pos;
    Float_t driftVel, mdcEf, sR1, sR2, sR3, sR4, sR5;
    Bool_t all=kFALSE;
    while (!all && !pFile->eof()) {
      pFile->getline(buf,maxbuf);
      if (buf[0]=='[') {
        sscanf(buf,"%s",key);
        if (strstr(buf,pPar->GetName())!=0) {
          while (!all && !pFile->eof()) {
            c=pFile->get();
            if (c=='#' || c=='/') all=kTRUE;
            else {
              pFile->putback(c);
              pFile->getline(buf,maxbuf);
              sscanf(buf,"%i%i%i%f%f%f%f%f%f%f",
                   &sector,&module,&layer,&driftVel,&mdcEf,&sR1,&sR2,&sR3,&sR4,&sR5);
              pos=sector*4+module;
              if (set[pos]) {
                HMdcDigitParLayer& l=
                  (*pPar)[sector][module][layer];
                l.fill(driftVel,mdcEf,sR1,sR2,sR3,sR4,sR5);
                set[pos]=999;
              }
            }
          }
        }
      }
    }
    if (all) {
      pPar->setInputVersion(1,inputNumber);
      pPar->setChanged();
      printf("MdcDigitPar initialized from Ascii file\n");
      return checkAllFound(set);
    }
  }
  return kFALSE;
} 


Bool_t HMdcParAsciiFileIo::read(HMdcLayerGeomPar* pPar,Int_t* set) {
  if (pFile) {
    const Int_t maxbuf=155;
    Text_t buf[maxbuf], key[20];
    Char_t c;
    Int_t sector, module, layer, pos;
    Float_t pit, cd, wor, cwn;
    Bool_t all=kFALSE;
    while (!all && !pFile->eof()) {
      pFile->getline(buf,maxbuf);
      if (buf[0]=='[') {
        sscanf(buf,"%s",key);
        //      cout << "Name = " <<pPar->GetName() << endl;
        if (strstr(buf,pPar->GetName())!=0) {
          while (!all && !pFile->eof()) {
            c=pFile->get();
            if (c=='#' || c=='/') all=kTRUE;
            else {
              pFile->putback(c);
              pFile->getline(buf,maxbuf);
              sscanf(buf,"%i%i%i%f%f%f%f",
                   &sector,&module,&layer,&pit,&cd,&wor,&cwn);
              pos=sector*4+module;
              if (set[pos]) {
                HMdcLayerGeomParLay& l=
                  (*pPar)[sector][module][layer];
                l.fill(pit,cd,wor,cwn);
                set[pos]=999;
              }
            }
          }
        }
      }
    }
    if (all) {
      pPar->setInputVersion(1,inputNumber);
      pPar->setChanged();
      printf("MdcLayerGeomPar initialized from Ascii file\n");
      return checkAllFound(set);
    }
  }
  return kFALSE;
} 


Bool_t HMdcParAsciiFileIo::checkAllFound(Int_t* set) {
  // checks if all modules have been initialized
  Bool_t allFound=kTRUE;
  for(Int_t i=0;i<24;i++) {
    if (set[i]==999) set[i]=0;
    if (set[i]!=0) allFound=kFALSE;
  }
  return allFound;
}







