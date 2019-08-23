Bool_t fileValidationOra(const TString inFile) {
//////////////////////////////////////////////////////////////////////////////
//
// USE ONLY FOR FILES WITH RICH TRIP!!!
//
// This macro should be used for putting validation information
// to Oracle for hld files, where a trip occured (usage of the 
// file set to "restricted use" ==  1). To put validation information
// to Oracle, comments has to be already known to Oracle. The rest of 
// files is putted to a file identified by the date located in 
// the directory ./ManualValidation.
//
// Quality used for validation
//
// quailty  0 => good
//         -1 => unusable
//          1 => restricted
//
// Input for this macro is sorted output (sort by filename) from macro 
// RichHVSummarySep05.C.
//
// USAGE:
// ======
//
// > root -n (no need to load hydra libs)
// > fileValidationOra("validation_DATE.txt")
//
//////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//                              INITAILISATION
//
////////////////////////////////////////////////////////////////////////////////

#define MAX_FILES 400
#define EXIT_SUCCESS  0
#define EXIT_FAILURE  1

  gSystem->Load("libOraUtil.so");

  ifstream *iFile = new ifstream();
  ofstream *oFile = new ofstream();

  TString fileName[MAX_FILES];
  TString comment[MAX_FILES];
  TString day = "";
  TString cTmp;

  Int_t commentID[2][MAX_FILES] = { 0 };
  Int_t counter = 0;
  Int_t numTrips = 0;
  Int_t oldSec = 0;
  Int_t currentSec = 0;

//////////////////////////////////////////////////////////////////////////////
//
// If you create a new comment, please insert its number here!!!
//
//////////////////////////////////////////////////////////////////////////////

  const Int_t quality = 1;
  const Int_t tripSimple[6] = {2800, 2792, 2802, 2783, 2795, 2794};
  const Int_t tripCombined[6][6] = {    0,    0,    0, 3124,    0, 2799,
                                        0,    0,    0, 2798, 2796, 2803,
                                        0,    0,    0, 3121,    0,    0,
                                     3124, 2798, 3121,    0, 2797, 2804.
                                        0, 2796,    0, 2797,    0,    0,
				     2799, 2803,    0, 2804,    0,    0};

////////////////////////////////////////////////////////////////////////////////
//
//                         ORACLE INITIALISATION
//                           FILE MANIPULATION
//
////////////////////////////////////////////////////////////////////////////////

  day = inFile;
  day.Remove(0, 11);
  day.Remove(day.Length() - 4, 4);

  HOraDetRunInfoIo *io = new HOraDetRunInfoIo();
  if (!io->open("rich_ana")) {
    cerr << "ERROR: can not open detector run info IO" << endl;
    delete io;
    exit(EXIT_FAILURE);
  }

  TString peopleID = io->getPeopleId("Martin", "Jurkovic");
  if (peopleID.IsNull()) {
    cerr << "ERROR: can not get peopleID" << endl;
    exit(EXIT_FAILURE);
  }

  Bool_t rc = io->setDetectorSetupVersion("RICH", 35);
  if (!rc) {
    cerr << "ERROR: can not set detector setup version" << endl;
    exit(EXIT_FAILURE);
  }

  io->print();

  cTmp = "ManualValidation/manual_";
  cTmp.Append(day.Data());
  cTmp.Append(".txt");
  cout << cTmp << endl;

  oFile->open(cTmp.Data(), ios::out);
  if(!oFile) {
    cerr << "ERROR: can not open output file" << oFile->data() << endl;
    exit(EXIT_FAILURE);
  }

  iFile->open(inFile.Data(), ios::in);
  if (!iFile) {
    cerr << "ERROR: can not open file" << iFile->data() << endl;
    exit(EXIT_FAILURE);
  }

////////////////////////////////////////////////////////////////////////////////
//
//                         PREPARE DATA FOR ORACLE
//
////////////////////////////////////////////////////////////////////////////////

  while(!iFile->eof()) {
    *iFile >> fileName[counter] >> comment[counter];

    if(iFile->eof()) {
      cout << "End of file reached----------------------" << endl;
      break;
    }

    strcat(fileName[counter].Data(),"");
    cout << counter << " Filename: " << "\t" << fileName[counter].Data() 
	 << "\t" << comment[counter].Data() << endl;

    if(counter == 0) {
      cTmp = comment[counter];
      cTmp.Remove(0,3);
      commentID[0][counter]++;
      commentID[1][counter] = tripSimple[atoi(cTmp.Data()) - 1];
      cout << "==== TripNr: " << commentID[0][counter] << " commentID: "
	   << commentID[1][counter] << "===============" << endl;
    } else {
// CHECK, IF THIS IS THE FIRST TRIP IN THE CURRENT FILE
      if(!fileName[counter].CompareTo(fileName[counter - 1].Data())) {
	cout << " More than one trip per file" << endl;

// CHECK, IF ALREADY MORE THAN ONE TRIP FOR CURRENT FILE EXIST
// IF YES, NEEDS MANUAL INVESTIGATION
	if(commentID[0][counter - 1] >= 2) {
	  cTmp = comment[counter].Data();
	  cTmp.Remove(0,3);
	  currentSec = atoi(cTmp.Data());

// FOR CURRENT FILE SET PREVIOUS COMMENT_ID'S TO ZERO
	  for(Int_t i = 0; i <= commentID[0][counter - 1]; i++) {
	    commentID[1][counter - i] = -1;
	  }

	  commentID[0][counter] = commentID[0][counter - 1] + 1;
	  *oFile << fileName[counter].Data() 
		 << "\tMore than 2 trip in this file (" 
		 << commentID[0][counter] << ")" << endl;
	  cout << "\t\tToo many trips, needs manual investigation!!!" << endl;
	} else {
// ONLY ONE TRIP FOR THE CURRENT FILE ALREADY EXISTS,
// IF COMMENT_ID FOR YOUR SECTOR COMBINATION EXISTS,
// TAKE IT FOR FURTHER PROCESSING AND ERASE 
// PREVIOUS COMMENT_ID FOR THE CURRENT FILE, 
// OTHERVICE MARK CURRENT FILE FOR MANUAL INVESTIGATION
// AND ERASE PREVOUS COMMENT-ID'S
	  cTmp = comment[counter - 1].Data();
	  cTmp.Remove(0,3);
	  oldSec = atoi(cTmp.Data()) - 1;
	  cTmp = comment[counter].Data();
	  cTmp.Remove(0,3);
	  currentSec = atoi(cTmp.Data()) - 1;

	  if(tripCombined[oldSec][currentSec] == 0) {
	    *oFile << fileName[counter].Data() 
		   << "\tComment does not exist for this sector combination ("
		   << oldSec + 1 << " && " << currentSec + 1 << ")" << endl;
	    cout << "\t\tComment does not exist!!!" << endl;
	    commentID[1][counter - 1] = -1;
	    commentID[0][counter] = commentID[0][counter - 1] + 1;
	    commentID[1][counter] = -1;
	  } else {
	    commentID[1][counter - 1] = 0;
	    commentID[1][counter] = tripCombined[oldSec][currentSec];
	    commentID[0][counter] = commentID[0][counter - 1] + 1;
	    cout << "==== TripNr: " << commentID[0][counter] << " commentID: "
		 << commentID[1][counter] << "===============" << endl;
	  }
	}
      } else {
// FIRST TRIP FOR THE CURRENT FILE, GET THE CORRECT COMMENT_ID
	cTmp = comment[counter];
	cTmp.Remove(0,3);
	commentID[0][counter]++;
	commentID[1][counter] = tripSimple[atoi(cTmp.Data()) - 1];
	cout << "==== TripNr: " << commentID[0][counter] << " commentID: "
	     << commentID[1][counter] << "===============" << endl;
      }
    }

    counter++;

    if(counter >= MAX_FILES) {
      cerr << "ERROR: Too many trips, allocate more memory!!!" << endl;
      exit(EXIT_FAILURE);
    }
  }

  numTrips = counter;
  cout << "Number of trips for day " << day << ": " << numTrips << endl;

////////////////////////////////////////////////////////////////////////////////
//
//                         CLOSE ALL OPENED FILES
//
////////////////////////////////////////////////////////////////////////////////

  cTmp = "results_";
  cTmp.Append(day.Data());
  cTmp.Append(".txt");
  ofstream f(cTmp.Data(), ios::out);
  if(!f) {
    cerr << "ERROR: can not open output file" << f.data() << endl;
    exit(EXIT_FAILURE);
  }
  for(Int_t i = 0; i < numTrips; i++) {
    f << fileName[i].Data() << "\t" << commentID[0][i] << "\t" << commentID[1][i] << endl;
  }
  f.close();

  if(iFile) {
    iFile->close();
    delete iFile;
    iFile = NULL;
  }
 
  if(oFile) {
    oFile->close();
    delete oFile;
    oFile = NULL;
  }

////////////////////////////////////////////////////////////////////////////////
//
//                         WRITE TO ORACLE
//
////////////////////////////////////////////////////////////////////////////////

  for(counter = 0; counter < numTrips; counter++) {
    if(commentID[1][counter] > 0) {
      cout << "make comment to oracle" << endl;
      cout << "======================" << endl;
      cout << fileName[counter].Data() << "\t" << quality << "\t" << commentID[1][counter] << endl;
      rc = io->setRunQuality(fileName[counter].Data(), quality, commentID[1][counter]);
    }
  }

  io->finish(); 
  delete io;

  return kTRUE;
}
