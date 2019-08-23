#include<hshowergeometry.h>
#include<hades.h>
#include<hshowerdetector.h>
#include<hspectrometer.h>
#include<hdetpario.h>
#include "hpario.h"
#include "hgeomcompositevolume.h"

ClassImp(HShowerGeometry)
HShowerGeometry::HShowerGeometry(const Char_t* name, const Char_t* title, const Char_t* context)
					:HDetGeomPar(name,title,context,"Shower") {	
					m_nModules = 3;
				  m_nSectors = 6;
  				m_nRows = 32;
  				m_nColumns = 32;
  				m_nLab = 0;
  				for(Int_t i = 0; i < 6; i++)
    				sectors[i].setSectorID(i);
}
HShowerGeometry::~HShowerGeometry()
{
  clear();
}


Bool_t HShowerGeometry::init(HParIo* inp,Int_t* set){
	HDetParIo* input=inp->getDetParIo("HShowerParIo");
	if (input) return (input->init(this,set));
	return kFALSE;

}

Int_t HShowerGeometry::write(HParIo* output) {
  HDetParIo* out=output->getDetParIo("HShowerParIo");
  if (out) return out->write(this);
  return -1;
}


Bool_t HShowerGeometry::initAscii(HParHadAsciiFileIo* pHadAsciiFile)
{
//Reading and initializating data from ASCII file (HadAsciiKey format)

 Bool_t status = kTRUE;
 if (!pHadAsciiFile) return kFALSE;

 try {

    HAsciiKey* pHAscii = pHadAsciiFile->GetKeyAscii();
    HAsciiKey &mydata = *pHAscii;

    mydata.SetActiveSection(" Shower General Geometry Data ");

    mydata.ReadInt(&m_nSectors, "Sectors");
    mydata.ReadInt(&m_nModules, "Modules");
    mydata.ReadInt(&m_nRows, "Rows");
    mydata.ReadInt(&m_nColumns, "Columns");
    mydata.ReadInt(&m_nLab, "Labolatory");

    for(Int_t i = 0; i < 6; i++)
        sectors[i].initAscii(pHadAsciiFile);

    for(Int_t i = 0; i < 3; i++) {
        frames[i].setModuleID(i);
        frames[i].initAscii(pHadAsciiFile);
    }

    for(Int_t i = 0; i < 3; i++) {
        wires[i].setModuleID(i);
        wires[i].initAscii(pHadAsciiFile);
    }

    for(Int_t i = 0; i < 3; i++) {
        pads[i].setModuleID(i);
        pads[i].setFrame(&frames[i]);
        pads[i].initAscii(pHadAsciiFile);
        pads[i].setModuleID(i);
    }

    attachTables();

		initModGeomParAscii(pHadAsciiFile);
		updateDetGeom();
	}
  catch (Bool_t ret) {
    status = ret;
  }

  return status;
}

Bool_t HShowerGeometry::writeAscii(HParHadAsciiFileIo* pHadAsciiFile)
{
//Writing to ASCII file (HadAsciiKey format)

 Bool_t status = kTRUE;
 if (!pHadAsciiFile) return kFALSE;

 try {

    HAsciiKey* pHAscii = pHadAsciiFile->GetKeyAscii();
    HAsciiKey &mydata = *pHAscii;

    if(!mydata.WriteSection(" Shower General Geometry Data "))
       return kFALSE;

    mydata.WriteInt("Sectors", m_nSectors);
    mydata.WriteInt("Modules", m_nModules);
    mydata.WriteInt("Rows", m_nRows);
    mydata.WriteInt("Columns", m_nColumns);
    mydata.WriteInt("Labolatory", m_nLab);
	
    for(Int_t i = 0; i < 6; i++)
        sectors[i].writeAscii(pHadAsciiFile);  //writing lab coord information

    for(Int_t i = 0; i < 3; i++)
        frames[i].writeAscii(pHadAsciiFile);  //writing local coord inforamtion

    for(Int_t i = 0; i < 3; i++)
        wires[i].writeAscii(pHadAsciiFile);

    for(Int_t i = 0; i < 3; i++)
        pads[i].writeAscii(pHadAsciiFile);

		writeModGeomParAscii(pHadAsciiFile);
		writeCompositeVolAscii(pHadAsciiFile);
  }

  catch (Bool_t ret) {
    status = ret;
  }

  return status;
}


void HShowerGeometry::attachTables() {
//Make connections between local coordinates of frame, wires, pads
//to containers which contained inforamation about module position
//in labolatory 
   Int_t i,j;
   for(i = 0; i < 6; i++)
     for(j = 0; j < 3; j++) {
       HShowerModuleGeom* pModGeom = sectors[i].getModule(j);
       pModGeom->setFrame(frames + j);
       pModGeom->setWires(wires + j);
       pModGeom->setPads(pads + j);
     }

   for(Int_t i = 0; i < 3; i++)
       pads[i].setFrame(&frames[i]);
}
void HShowerGeometry::clear()
{
}
const HGeomTransform&  HShowerGeometry::getTransform(Int_t sect) {
//return transformation data of sector sect
  return sectors[sect].getTransform();
}

const HGeomTransform&  HShowerGeometry::getTransform(Int_t sect, Int_t mod) {
//return transformation data of module mod in sector sect
  return sectors[sect].getModule(mod)->getTransform();
}
void HShowerGeometry::getLocalCoord(HLocation& loc, HGeomVector& v) {
//retrieve local coordinates of center of pads in position loc

  Float_t fX, fY;
  Float_t scale = 10.0; //transform cm to mm

  HShowerPadTab* pPads = getPadParam(loc[1]);
  pPads->getPad(loc[2], loc[3])->getPadCenter(&fX, &fY);

  v.setX(scale*fX);
  v.setY(scale*fY);
  v.setZ(0.0);  //z==0 local coordinates are in plain
}


// transform X_loc, Y_Loc -> X,Y,Z Lab
void HShowerGeometry::transVectToLab(HLocation& loc, HGeomVector vLocal, HGeomVector2& vLab) {
 
 HGeomTransform tt = getTransform(loc[0], loc[1]);
 vLab = tt.transFrom(vLocal);
}

void HShowerGeometry::getLabCoord(HLocation& loc, HGeomVector2& v) {
//retrieve lab coordinates of center of pads in position loc
// Local coordinates are transformed to lab using HGeomTransform class
// There is data for transformation this in HShowerModuleGeom object
// addressed by loc[0], loc[1]

  HGeomVector vLoc;
  HGeomTransform tt;

  getLocalCoord(loc, vLoc);
  tt = getTransform(loc[0], loc[1]);

  v = tt.transFrom(vLoc);
}

void HShowerGeometry::getSphereCoord(HLocation& loc, HGeomVector2& v, HGeomVector *pvTarget){
//Sphere coordinate are calculated from lab coordinates
  Float_t r,theta,phi;
  HGeomVector2 vLoc;

  getLabCoord(loc,vLoc);

  if(pvTarget != NULL)
    vLoc -= *pvTarget;

  vLoc.sphereCoord(r,theta,phi); //calculation of sphere coord

  v.setRad(r);
  v.setTheta(theta);
  v.setPhi(phi);
}

// transform X_Lab,Y_Lab,Z_Lab -> Phi,Theta,R 
void HShowerGeometry::transLabToSphereCoord(HGeomVector2& labIn, HGeomVector2& sphereOut, HGeomVector *pvTarget){
//Sphere coordinate are calculated from lab coordinates
  Float_t r,theta,phi;

  if(pvTarget != NULL)
    //vLoc -= *pvTarget;
    labIn -= *pvTarget;

  labIn.sphereCoord(r,theta,phi); //calculation of sphere coord

  sphereOut.setRad(r);
  sphereOut.setTheta(theta);
  sphereOut.setPhi(phi);
}


void HShowerGeometry::translModules() {
  sectors[0].translateModules();  //Only in sector 0
          //Sector 0 is reference to others
}

void HShowerGeometry::rotateSectors() {
  Int_t i, j;

  HGeomTransform modt[3];
  HGeomTransform st;

  st = sectors[0].getTransform();
  for (j = 0; j < sectors[0].getModules(); j++)
    modt[j] = sectors[0].getModule(j)->getTransform();

  for(i = 1; i < m_nSectors; i++) {
    sectors[i].setTransform(st);
    for(j = 0; j < sectors[i].getModules(); j++)
        sectors[i].getModule(j)->setTransform(modt[j]);

    sectors[i].rotateSector();
  }
}
Bool_t HShowerGeometry::transToLab() {
  if (IsLab()) {
     printf("Geometry has been transformed to labolatory coordinates already\n");
     return kFALSE;
  }

  printf("Geometry is being transformed to labolatory coordinates now\n");

  translModules();
  rotateSectors();
  setLab();
  setChanged();

  return kTRUE;
}
Bool_t HShowerGeometry::transPadToLab() {
   HGeomTransform tt;
   HGeomVector2 vLoc,vLab;
   Float_t fX,fY,r,theta,phi;
   Float_t scale = 10.0; //transform cm to mm
   FILE *fp;
   fp = fopen("test.txt","w");

   for(Int_t i=0;i<m_nSectors;i++) {
   for(Int_t j=0;j<sectors[i].getModules();j++) {
    for (Int_t k=0;k < m_nRows; k++) {
    for (Int_t l=0;l < m_nColumns; l++) {

    HShowerPadTab* pPads = getPadParam(j);
    pPads->getPad(k,l)->getPadCenter(&fX, &fY);
    vLoc.setX(scale*fX);
    vLoc.setY(scale*fY);
    vLoc.setZ(0.0);

    tt = getTransform(i,j);
    vLab = tt.transFrom(vLoc);

    vLab.sphereCoord(r,theta,phi);
    if(!j) fprintf(fp,"%3d%3d%3d %10.3f%10.3f\n",i,k,l,phi,theta);

   }
  }
 }
}
 fclose(fp);
 return kTRUE;
}

void HShowerGeometry::Streamer(TBuffer &R__b)
{
	UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
    if (R__v<2){
			Warning("Streamer","\n!!! Old version of the ShowerGeometry\n!!! Not full functionality from HDetGeomPar will be available\n");
			HShowerParSet showerSet;
			showerSet.Streamer(R__b);
      R__b >> m_nSectors;
      R__b >> m_nModules;
      R__b >> m_nRows;
      R__b >> m_nColumns;
      R__b >> m_nLab;
      Int_t R__i;
      for (R__i = 0; R__i < 6; R__i++)
         sectors[R__i].Streamer(R__b);
      for (R__i = 0; R__i < 3; R__i++)
         frames[R__i].Streamer(R__b);
      for (R__i = 0; R__i < 3; R__i++)
         wires[R__i].Streamer(R__b);
      for (R__i = 0; R__i < 3; R__i++)
         pads[R__i].Streamer(R__b);
			if ( !initDetGeom() || !updateDetGeom()){
				Warning("Streamer","ShowerDetGeom not initialized correctly");
			}
  	}
		else {
					HDetGeomPar::Streamer(R__b);
					R__b >> m_nSectors;
					R__b >> m_nModules;
					R__b >> m_nRows;
					R__b >> m_nColumns;
					R__b >> m_nLab;
				 Int_t R__i;
					for (R__i = 0; R__i < 6; R__i++)
						 sectors[R__i].Streamer(R__b);
					for (R__i = 0; R__i < 3; R__i++)
						 frames[R__i].Streamer(R__b);
					for (R__i = 0; R__i < 3; R__i++)
						 wires[R__i].Streamer(R__b);
					for (R__i = 0; R__i < 3; R__i++)
						 pads[R__i].Streamer(R__b);
					R__b.CheckByteCount(R__s, R__c, HShowerGeometry::IsA());
					attachTables();
			}
   } else {
      R__c = R__b.WriteVersion(HShowerGeometry::IsA(), kTRUE);
      HDetGeomPar::Streamer(R__b);
      R__b << m_nSectors;
      R__b << m_nModules;
      R__b << m_nRows;
      R__b << m_nColumns;
      R__b << m_nLab;
      Int_t R__i;
      for (R__i = 0; R__i < 6; R__i++)
         sectors[R__i].Streamer(R__b);
      for (R__i = 0; R__i < 3; R__i++)
         frames[R__i].Streamer(R__b);
      for (R__i = 0; R__i < 3; R__i++)
         wires[R__i].Streamer(R__b);
      for (R__i = 0; R__i < 3; R__i++)
         pads[R__i].Streamer(R__b);
      R__b.SetByteCount(R__c, kTRUE);
   }


}

Bool_t HShowerGeometry::updateDetGeom() {
	//Function for calculating transformations
	//in  HDetGeom interface.
	//Used for internal synchronization, especially after modyfication 
	//of the transformation in macros.
	HGeomVector vm[3] = { HGeomVector(0.000 , 0.00 , -3.997),
                        HGeomVector(0.000 , 0.00 , 67.601),
                        HGeomVector(0.000 , 0.00 , 134.897)};

	for (Int_t s=0;s<getSectors();s++){
	for(Int_t m=0;m<getModules();m++){
		HGeomVector tr = sectors[s].getModule(m)->transform.getTransVector();
		HGeomRotation rot = sectors[s].getModule(m)->getTransform().getRotMatrix();

		getModule(s,m)->getLabTransform().setRotMatrix(rot);
		getModule(s,m)->getLabTransform().setTransVector(tr -rot*vm[m]);
		}
	}

return kTRUE;
}
Bool_t HShowerGeometry::initDetGeom(){

if(gHades){
	HSpectrometer* spec=gHades->getSetup();
	HDetector* shower=0;
		if (  !(shower = spec->getDetector("Shower")) ) {
			shower = new HShowerDetector;
  		 Int_t showerMods[3]={1,1,1};
   		for(Int_t nSector =0 ; nSector<6 ;nSector++) {
     		shower->setModules(nSector,showerMods);
   		}
   		spec->addDetector(shower);
		}
		else{
			Error("initDetGeom","Detector not found");
			return kFALSE;
		}
		shapes=gHades->getSetup()->getShapes();
    maxSectors=shower->getMaxSectors();
    maxModules=shower->getMaxModules();
    numComponents=shower->getMaxComponents();
    if (maxModules>0) {
      refVolumes=new TObjArray(maxModules);
      Int_t* set=shower->getModules();
      Int_t n=0;
      if (maxSectors>0) n=maxSectors*maxModules;
      else n=maxModules;
      modules=new TObjArray(n);
      for (Int_t i=0;i<n;++i) {
        if (set[i]!=0) modules->AddAt(new HModGeomPar(),i);
        else modules->RemoveAt(i);
      }
    }
}
else return kFALSE;



Char_t modNames[18][6] = {"SH1M1",
"SH2M1",
"SH3M1",
"SH1M2",
"SH2M2",
"SH3M2",
"SH1M3",
"SH2M3",
"SH3M3",
"SH1M4",
"SH2M4",
"SH3M4",
"SH1M5",
"SH2M5",
"SH3M5",
"SH1M6",
"SH2M6",
"SH3M6",
};
Int_t i=0;
Char_t ref[6];
for(Int_t s=0;s<6;s++)
	for(Int_t m=0; m<3;m++){
		HModGeomPar* pMod=getModule(s,m);
		if( pMod ){
			pMod->SetName(modNames[i]);
			strcpy(ref,modNames[i]);
			ref[4]='1';
			pMod->setRefName(ref);
			Int_t mr=getModNumInMod(ref);
			HGeomCompositeVolume* refMod=getRefVolume(mr);
			if (refMod==0) {
                refMod=new HGeomCompositeVolume(getNumComponents());
                refMod->SetName(ref);
                addRefVolume(refMod,mr);
          //refMod->print();
			}
			pMod->setVolume(refMod);
		}
		i++;
	}

return kTRUE;
}

Bool_t  HShowerGeometry::writeModGeomParAscii(HParHadAsciiFileIo* pHadAsciiFile){
	Bool_t status = kTRUE;
	Float_t matrixRot[9];
  Float_t transVect[3];

	if (!pHadAsciiFile) return kFALSE;

	try {

		HAsciiKey* pHAscii = pHadAsciiFile->GetKeyAscii();
		HAsciiKey &mydata = *pHAscii;
		mydata.WriteString("\n");
		mydata.WriteRemark("");
		mydata.WriteRemark("Geometry Data according to the HDetGeomPar scheme");
		mydata.WriteRemark("DO NOT change Rotation Matrix or Translation Vector");
		mydata.WriteRemark("for \"Shower ModGeom\"!!! Your changes will be lost!!!");
		mydata.WriteRemark("Transformation for ModGeom is always recalculated from:");
		mydata.WriteRemark("\"SectorX Geometry Data\"");
		mydata.WriteRemark("");
		mydata.WriteString("\n");
		if(!mydata.WriteSection("Shower ModGeom"))
    return kFALSE;
		for(Int_t i=0;i< getNumModules();i++) {
			HModGeomPar* p = getModule(i);
		 Char_t buf[10];
    	if (p){
			if(i!=0)	
			mydata.WriteLine("\n");
			sprintf(buf,"\"%s\"",p->GetName());
    	mydata.WriteString("Name", buf);
			sprintf(buf,"\"%s\"",p->getRefName());
    	mydata.WriteString("RefName", buf);
			for(Int_t i=0; i < 9; i++)
			matrixRot[i]=p->getLabTransform().getRotMatrix()(i);
			
			transVect[0]=p->getLabTransform().getTransVector().getX();
			transVect[1]=p->getLabTransform().getTransVector().getY();
			transVect[2]=p->getLabTransform().getTransVector().getZ();
			
			mydata.SetNumInLine(3);
			mydata.WriteFloat("Rotation Matrix*", 9, (Float_t*)matrixRot);
			mydata.WriteFloat("Translation Vector", 3, (Float_t*)transVect);

			}

			}
	
  	}

	catch (Bool_t ret) {
    status = ret;
	}

	return status;
}

Bool_t  HShowerGeometry::writeCompositeVolAscii(HParHadAsciiFileIo* pHadAsciiFile){
	Bool_t status = kTRUE;
	if (!pHadAsciiFile) return kFALSE;

	try {

    HAsciiKey* pHAscii = pHadAsciiFile->GetKeyAscii();
    HAsciiKey &mydata = *pHAscii;
		if(!mydata.WriteSection("Shower Geom Volume "))
    return kFALSE;
		
		for(Int_t i=0;i<refVolumes->GetSize();i++) {
	    HGeomCompositeVolume* p=(HGeomCompositeVolume*)refVolumes->At(i);
			if(p){
				writeGeomVolAscii(pHadAsciiFile,p);
				mydata.WriteLine("\n");
				writeGeomVolAscii(pHadAsciiFile,p->getComponent(0));
				mydata.WriteLine("\n");
			}
  	}

	}


	catch (Bool_t ret) {
    status = ret;
	}

	return status;
}

Bool_t  HShowerGeometry::writeGeomVolAscii(HParHadAsciiFileIo* pHadAsciiFile,HGeomVolume *p){
  Bool_t status = kTRUE;
 	Float_t geomVect[3];
	Float_t matrixRot[9];
  Float_t transVect[3];
 Char_t buf[10];
	if (!pHadAsciiFile) return kFALSE;

  try {

    HAsciiKey* pHAscii = pHadAsciiFile->GetKeyAscii();
    HAsciiKey &mydata = *pHAscii;
    if (p) {
		Int_t nPoints = p->getNumPoints();
		sprintf(buf,"\"%s\"",p->getName().Data());
		mydata.WriteString("Name",buf);
		sprintf(buf,"\"%s\"",p->getShape().Data());
		mydata.WriteString("Shape", buf);
		sprintf(buf,"\"%s\"", p->getMother().Data());
		mydata.WriteString("Mother", buf);
		if (nPoints)
			for (Int_t i=0;i<nPoints;i++){
				HGeomVector *pVector = p->getPoint(i);
				geomVect[0]=pVector->getX();
      	geomVect[1]=pVector->getY();
      	geomVect[2]=pVector->getZ();
				mydata.WriteFloat("Point", 3, (Float_t*)geomVect);				
  		}
		for(Int_t n=0; n < 9; n++)
			matrixRot[n]=p->getTransform().getRotMatrix()(n);
			
			transVect[0]=p->getTransform().getTransVector().getX();
			transVect[1]=p->getTransform().getTransVector().getY();
			transVect[2]=p->getTransform().getTransVector().getZ();
		  //if(n==0)	
			mydata.SetNumInLine(3);
			mydata.WriteFloat("Rotation Matrix*", 9, (Float_t*)matrixRot);
			mydata.WriteFloat("Translation Vector", 3, (Float_t*)transVect);
      }
    }

  

  catch (Bool_t ret) {
    status = ret;
  }

  return status;
}

Bool_t HShowerGeometry::initModGeomParAscii(HParHadAsciiFileIo* pHadAsciiFile)
{
	//Reading and initializating data from ASCII file (HadAsciiKey format)
	
	Bool_t status = kTRUE;
	if (!pHadAsciiFile) return kFALSE;
	
	try {
		HAsciiKey* pHAscii = pHadAsciiFile->GetKeyAscii();
		HAsciiKey &mydata = *pHAscii;

		mydata.SetActiveSection(" Shower ModGeom ");
		if(gHades){
		HSpectrometer* spec=gHades->getSetup();
		HDetector* shower=0;
		spec->print();
			if (  !(shower = spec->getDetector("Shower")) ) {
				shower = new HShowerDetector;
				 Int_t showerMods[3]={1,1,1};
				for(Int_t nSector =0 ; nSector<6 ;nSector++) {
					shower->setModules(nSector,showerMods);
				}
				spec->addDetector(shower);
			}
			shapes=gHades->getSetup()->getShapes();
			maxSectors=shower->getMaxSectors();
			maxModules=shower->getMaxModules();
			numComponents=shower->getMaxComponents();
			if (maxModules>0) {
				refVolumes=new TObjArray(maxModules);
				Int_t* set=shower->getModules();
				Int_t n=0;
				if (maxSectors>0) n=maxSectors*maxModules;
				else n=maxModules;
				modules=new TObjArray(n);
				for (Int_t i=0;i<n;++i) {
					if (set[i]!=0) modules->AddAt(new HModGeomPar(),i);
					else modules->RemoveAt(i);
				}
			}
		}
	 Char_t name[10],refName[10],shape[10],mother[10];
		Float_t fRot[9];
		Float_t fTrans[3];
		Float_t fPoint[3];
		while(mydata.IsNextKeyword()){
			strcpy(name,mydata.ReadString("*"));
			strcpy(refName,mydata.ReadString("*"));
			HModGeomPar* pMod=getModule(getSecNum(name),getModNumInMod(name));
			pMod->SetName(name);
			pMod->setRefName(refName);

			mydata.ReadFloat(fRot,"*");
			mydata.ReadFloat(fTrans,"*");
			 
			pMod->getLabTransform().setTransVector(fTrans);
			pMod->getLabTransform().setRotMatrix(fRot);
			refName[4]='1';
			Int_t mr=getModNumInMod(refName);
			HGeomCompositeVolume* refMod=getRefVolume(mr);
				if (refMod==0) {
					refMod=new HGeomCompositeVolume(getNumComponents());
					refMod->SetName(refName);
					addRefVolume(refMod,mr);
			 }
				pMod->setVolume(refMod);
		}
		
		mydata.SetActiveSection("Shower Geom Volume");
		while(mydata.IsNextKeyword()){	
			strcpy(name,mydata.ReadString("*"));
			strcpy(shape,mydata.ReadString("*"));
			strcpy(mother,mydata.ReadString("*"));
			HGeomCompositeVolume* p=
					(HGeomCompositeVolume*)refVolumes->At(getModNumInMod(name));
			p->createPoints(8);
			for (Int_t i=0;i<8;i++){
				mydata.ReadFloat(fPoint,"*");
				p->setPoint(i,fPoint[0],fPoint[1],fPoint[2]);
			}	
			mydata.ReadFloat(fRot,"*");
			mydata.ReadFloat(fTrans,"*");
			p->getTransform().setTransVector(fTrans);
			p->getTransform().setRotMatrix(fRot);
			p->SetName(name);
			p->setShape(shape);
			p->setMother(mother);
			HGeomVolume *vol = p->getComponent(0);
			vol->createPoints(8);
			strcpy(name,mydata.ReadString("*"));
			strcpy(shape,mydata.ReadString("*"));
			strcpy(mother,mydata.ReadString("*"));
			vol->SetName(name);
			vol->setShape(shape);
			vol->setMother(mother);
			for (Int_t i=0;i<8;i++){
				mydata.ReadFloat(fPoint,"*");
				vol->setPoint(i,fPoint[0],fPoint[1],fPoint[2]);
			}
			mydata.ReadFloat(fRot,"*");
			mydata.ReadFloat(fTrans,"*");
			vol->getTransform().setTransVector(fTrans);
			vol->getTransform().setRotMatrix(fRot);
		}
	}
	catch (Bool_t ret) {
		status = ret;
	}

	return status;
}

