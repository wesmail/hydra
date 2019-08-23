#ifndef HORASLOWCHANNEL_H
#define HORASLOWCHANNEL_H

using namespace std;
#include "TNamed.h"
#include "TObjArray.h"
#include <iostream>
#include <iomanip>
#include <fstream>

class HOraSlowPartition;
class TGraph;
class TGraphErrors;
class TGraphAsymmErrors;

class HOraSlowChannel : public TNamed {
private:
  Int_t              channelId;      // Id of the channel in Oracle
  TString            channelType;    // Type of the channel
  Int_t              maxPrecision;   // Maximum precision of the values in the actual timerange
  TObjArray*         pRunSumData;    // Array of summary data (object type HOraSlowChanRunSum)
  TObjArray*         pMetaData;      // Array of meta data (object type HOraSlowChanMata)
  TIterator*         runSumDataIter; // Iterator on array of summary data 
  TGraph*            pGraph;         // Graph of summary data 
  TGraph*            pRawGraph;      // Graph raw data  
  HOraSlowPartition* partition;      // Pointer to actual partition
  TObjArray*         pRawData;       // Array of raw data
  TString            rawStart;       // Start time of raw data
  TString            rawEnd;         // End time of raw data
  TIterator*         rawIter;        // Iterator on array of raw data
public:
  HOraSlowChannel(const char* name="");
  ~HOraSlowChannel();
  void setChannelId(Int_t n) {channelId=n;}
  void setChannelType(const char* s) {channelType=s;}
  void setMaxPrecision(Int_t n) {maxPrecision=n;}
  void setPartition(HOraSlowPartition*);
  void setRunSumData(TObjArray*);
  void setRawData(TObjArray*,const char*,const char*);
  void setMetaData(TObjArray*);
  Int_t getChannelId() {return channelId;}
  const char* getChannelType() {return channelType.Data();}
  Int_t getMaxPrecision() {return maxPrecision;}
  TObjArray* getRunSumData() {return pRunSumData;}
  TObjArray* getMetaData() {return pMetaData;}
  TObjArray* getRawData() {return pRawData;}
  Bool_t readRawData(Int_t);
  Bool_t readRawData(const char*,const char*);
  void printRunSumData(Int_t opt=0);  
  void printMetaData();  
  void printRawData();  
  void writeRunSumData(fstream&,Int_t opt=0);
  void writeMetaData(fstream&);  
  void writeRawData(fstream&);  
  TGraphErrors* getRunSumMeanSigmaGraph(Int_t opt=0,
					Int_t mStyle=7,Int_t mColor=4);
  TGraphAsymmErrors* getRunSumMeanMinMaxGraph(Int_t opt=0,
					      Int_t mStyle=7,Int_t mColor=2);
  TGraph* getRawDataGraph(Int_t mStyle=7,Int_t mColor=4);
private:
  void clearRunSumData();
  void clearRawData();
  void deleteGraph();
  void deleteRawGraph();
  ClassDef(HOraSlowChannel,0) // Class for a slowcontrol channel
};

#endif  /* !HORASLOWCHANNEL */
