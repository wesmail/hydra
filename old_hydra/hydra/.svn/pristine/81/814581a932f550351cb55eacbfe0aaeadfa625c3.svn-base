#ifndef HPIDTASKSET_H
#define HPIDTASKSET_H

///////////////////////////////////////////////////////////////////////////////////////////
//
// This taskset collects all PID reconstructor tasks and delivers options parsed from a
// a string to them. This should make the user's life easier if he/she is not a PID expert
// Suitable "collections" of options can be defined to extend this scheme
//
///////////////////////////////////////////////////////////////////////////////////////////

#include "htaskset.h"
#include <TString.h>

//All PID tasks shall be run
#define ALLTASKS "TRACKCANDIDATES,PIDCANDIDATES,PARTICLES,PDFDATA,PERFORMANCETEST"

//All cetegories shall be executed
#define ALLCATEGORIES "TRACKCANDIDATES,CANDIDATES,PARTICLES"

//The "beamtime" tasks are the PidTrackFiller and the PdfMaker tasks. 
//Those can be executed together already during the beamtime and they allow a nice overview 
//of the data on tape.
#define BEAMTIMETASKS "TRACKCANDIDATES,PDFMAKER"

//Default arguments for all tasks
#define TRACKFILLERDEFAULTS "NOCHI2SEG1,SKIPNOLEPTON"
#define RECONSTRUCTORDEFAULTS "ALG_KICK,PDF,CL"
#define ALGORITHMDEFAULTS "STANDARDCUTS"
#define PARTICLEFILLERDEFAULTS "KICK,ALGLIKELIHOOD"
#define SKIPDEFAULTS "LVL1"

class HPidTaskSet : public HTaskSet {

public:
  HPidTaskSet(void);
  HPidTaskSet(Text_t name[],Text_t title[]);
  ~HPidTaskSet(void);
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //
  // Calling the "make" function without any non-mandatory argument supplies the defaults defined above to the 
  // PID reconstructors
  //
  // To make the user's life easier he/she can supply one argument per reconstructor and a list of algorithms
  // separately. If the user wants to change only some arguments with respect to the defaults and not change all,
  // he/she can supply "D" as option string to the others which will supply default parameters.
  //
  // e.g. make("real","D","LVL2","D","ALG_KICK,PDF","D","D") would do the following:
  // all reconstructors
  // skip level 2 events
  // Set the defaults for HPidTrackFiller
  // Set the flags ALG_KICK,PDF for HPidReconstructor
  // Set default options for its algorithms (alg-std-cuts)
  // Set default options for HPidParticleFiller
  //
  // There is still one feature missing: There is no way to set an individual outputfile for 
  // alghardcuts and algstandcuts.
  //
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
  static HTask *makeTasks(const char *dataType, 
			  Option_t *tasklist=ALLCATEGORIES, 
			  Option_t *opt_skiptrigger=SKIPDEFAULTS,
			  Option_t *opt_trackfiller=TRACKFILLERDEFAULTS,
			  Option_t *opt_reconstructor=RECONSTRUCTORDEFAULTS, 
			  Option_t *opt_algorithms=ALGORITHMDEFAULTS, 
			  Option_t *opt_particlefiller=PARTICLEFILLERDEFAULTS);


  static HTask *makeBeamtimeTasks(const Char_t *dataType, const Char_t* inputfile);
  static HTask *makePDFCreationTasks(const Char_t *dataType, const Char_t* inputfile);
  static HTask *makeBayesianIdTasks(const Char_t *dataType, const Char_t* inputfile, const Char_t* trackingmethod);
  static HTask *makeBayesianIdTasksFromScratch(const Char_t *dataType, const Char_t* inputfile, const Char_t* trackingmethod);
  static HTask *makeHardcutIdTasks(const Char_t *dataType, const Char_t* inputfile, const Char_t* trackingmethod, const Char_t* targetfile);
  static HTask *make(const Char_t *dataType, const Char_t* keyword, const Char_t* inputfile=NULL);
  
 private:
  
  ClassDef(HPidTaskSet,1) // Set of tasks
    };
    
#endif /* !HPIDTASKSET_H */
