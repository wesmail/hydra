#include <TFile.h>
#include <TH3F.h>
#include <string.h>
#include <fstream.h>
#include <iostream.h>
//
// Convert a 3d HADES acceptance matrix (i.e. TH3F object) into
// FORTRAN-readable format used by the HADES Theory Acceptance Filter (HAFT)
//
//
// Usage:
//
// root[0] .L writeHAFT.C+
// root[1] convertMatrix("inputFilename","matrixName","outputFilename","commentString");
//
//
// Created :   06/04/05 by R. Holzmann
// Modified :  07/04/05 by R. Holzmann
//

Int_t writeFortranStyle(Int_t bytecount, void* array, ofstream* out);

void convertMatrix(Text_t *inName, Text_t *matName, Text_t *outName, Text_t *comment) {

  Text_t commentString[] =
    "                                                                                ";
  // 1        10        20        30        40        50        60        70        80

  TFile *input = new TFile(inName,"read");
  if (input->IsOpen()==kFALSE) {
    cout << "Input file " << inName << " could not be opened!" << endl;
    return;
  } 

  TH3F *matrix = (TH3F*)(input->Get(matName));
  if (matrix==NULL) {
    cout << "Acceptance matrix " << matName << " not found in file!" << endl;
    return;
  } 

  strncpy(commentString, comment, strlen(comment));
  cout << endl << "|" << commentString << "|" << endl << endl;

  Int_t dims[3];
  Int_t xchan,ychan,zchan;
  xchan=dims[0]=matrix->GetNbinsX();          // get matrix dimensions
  ychan=dims[1]=matrix->GetNbinsY();
  zchan=dims[2]=matrix->GetNbinsZ();

  Float_t lims[6];
  Float_t xmin,xmax,ymin,ymax,zmin,zmax;
  xmin=lims[0]=matrix->GetXaxis()->GetXmin();  // get matrix limits
  xmax=lims[1]=matrix->GetXaxis()->GetXmax();
  ymin=lims[2]=matrix->GetYaxis()->GetXmin();
  ymax=lims[3]=matrix->GetYaxis()->GetXmax();
  zmin=lims[4]=matrix->GetZaxis()->GetXmin();
  zmax=lims[5]=matrix->GetZaxis()->GetXmax();

  cout << "Convert matrix with dimensions ("
       << xchan << "," << ychan << "," << zchan << ")" << endl;
  cout << "and ranges [" << xmin << "-" << xmax << "]["
       << ymin << "-" << ymax << "][" << zmin << "-" << zmax << "]" << endl;

  Float_t array[zchan][ychan][xchan];  // allocate array
  Float_t val=0.;

  for(Int_t ix=1;ix<=xchan;ix++) {     // copy to array in FORTRAN-style
    for(Int_t iy=1;iy<=ychan;iy++) {   // column-by-column mode
      for(Int_t iz=1;iz<=zchan;iz++) { // (1st index varies fastest!)
         val=matrix->GetBinContent(ix,iy,iz);
         array[iz-1][iy-1][ix-1]=val;
      }
    }
  }

  ofstream out(outName);           // open file for unformatted output
                                   // (can be checked with od -f)
  Int_t nbytes=0;
//
// FORTRAN unformatted sequential I/O needs a bytecount, i.e. each I/O
// operation works on a new record, prefixed and appended with the bytecount.
//
// The following output **MUST** be read via the following FORTRAN statements:
//
//       read(unit) comment                        ! comment string into character*80
//       read(unit) ix,iy,iz                       ! array dimensions into 3 x integer*4
//       read(unit) xmin,xmax,ymin,ymax,zmin,zmax  ! axis limits into 6 x real*4
//       read(unit) array                          ! data into real*4 array declared with proper size
//

  nbytes += writeFortranStyle(sizeof(commentString)-1,&commentString,&out);
  nbytes += writeFortranStyle(sizeof(dims),&dims,&out);
  nbytes += writeFortranStyle(sizeof(lims),&lims,&out);
  nbytes += writeFortranStyle(sizeof(array),&array,&out);

  out.close();
  cout << endl << "Conversion done: " << nbytes
       << " bytes written to file " << outName << endl << endl;
  return;
}


//
// Function to write data FORTRAN-style to output file
//
Int_t writeFortranStyle(Int_t bytecount, void* array, ofstream* out) {
  out->write((char*)&bytecount,4);
  out->write((char*)array,bytecount);
  out->write((char*)&bytecount,4);
  return bytecount+8;
}


//
// Print out (ix,iy,iz)-indexed channels for testing purposes
//
void testChannels(Text_t *inName, Text_t *matName) {

  TFile *input = new TFile(inName,"read");
  if (input->IsOpen()==kFALSE) {
    cout << "Input file " << inName << " could not be opened!" << endl;
    return;
  } 

  TH3F *matrix = (TH3F*)(input->Get(matName));
  if (matrix==NULL) {
    cout << "Acceptance matrix " << matName << " not found in file!" << endl;
    return;
  } 

  Int_t xchan=matrix->GetNbinsX();  // get matrix dimensions
  Int_t ychan=matrix->GetNbinsY();
  Int_t zchan=matrix->GetNbinsZ();

  cout << "Test matrix with dimensions ("
       << xchan << "," << ychan << "," << zchan << ")" << endl;

  Float_t val=0.;
  Int_t ix,iy,iz;
  while (1) {
    cout << "Enter ix,iy,iz : ";
    cin >> ix >> iy >> iz;
    if (ix<=0) return;
    val=matrix->GetBinContent(ix,iy,iz);
    cout << "has value : " << val << endl;
  }
}

//
// Print out (p,theta,phi)-bins for testing purposes
//
void testBins(Text_t *inName, Text_t *matName) {

  TFile *input = new TFile(inName,"read");
  if (input->IsOpen()==kFALSE) {
    cout << "Input file " << inName << " could not be opened!" << endl;
    return;
  } 

  TH3F *matrix = (TH3F*)(input->Get(matName));
  if (matrix==NULL) {
    cout << "Acceptance matrix " << matName << " not found in file!" << endl;
    return;
  } 

  Float_t xmin=matrix->GetXaxis()->GetXmin();  // get matrix limits
  Float_t xmax=matrix->GetXaxis()->GetXmax();
  Float_t ymin=matrix->GetYaxis()->GetXmin();
  Float_t ymax=matrix->GetYaxis()->GetXmax();
  Float_t zmin=matrix->GetZaxis()->GetXmin();
  Float_t zmax=matrix->GetZaxis()->GetXmax();

  cout << "Test matrix with ranges [" << xmin << "-" << xmax << "]["
       << ymin << "-" << ymax << "][" << zmin << "-" << zmax << "]" << endl;

  Float_t val=0.;
  Float_t p,th,ph;
  Int_t ix,iy,iz;
  while (1) {
    cout << "Enter p,theta,phi : ";
    cin >> p >> th >> ph;
    if (p<xmin) return;
    ix=matrix->GetXaxis()->FindBin(p);
    iy=matrix->GetYaxis()->FindBin(th);
    iz=matrix->GetZaxis()->FindBin(ph);
    val=matrix->GetBinContent(ix,iy,iz);
    cout << "has value : " << val << endl;
    Int_t binnr = matrix->GetBin(ix,iy,iz);
    cout << ix << " " << iy << " " << iz << " : " << binnr << endl;
    val=matrix->GetBinContent(binnr);
    cout << "has value : " << val << " " << endl;
  }
}
