//*-- AUTHOR : Jochen Markert

//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////////////////////////////////
//
// HMdcVertexWriter
//
// This class writes the reconstructed Event vertex to an root output file
// which will be later used for the preparation of the simulations files for
// the event embeddding. The class has to connect last to the task list to not
// write out skipped events. The outputfile will be equal to the
// current inputfile_vertex.txt if no output file is set in HADES. If the
// outputfile is set in HADES it will be equal outputfile_vertex.txt. The
// file will be splitted in the same way as the the root output file.
//
//  static void setSkipNoVertex (Bool_t skip = kTRUE){doSkipNoVertex = skip; }
//  With doSkipNoVertex == kTRUE (default : kTRUE) events without calculated vertex will be skipped.
//
//////////////////////////////////////////////////////////////////////////////
#include "hmdcvertexwriter.h"
#include "hades.h"
#include "hevent.h"
#include "heventheader.h"
#include "hdatasource.h"
#include "hcategory.h"
#include "hgeantkine.h"
#include "hgeantdef.h"


#include "TFile.h"
#include "TSystem.h"

#include <iostream> 
#include <iomanip>
#include <stdlib.h>



ClassImp(HMdcVertexWriter)

Bool_t HMdcVertexWriter::doSkipNoVertex = kTRUE;

HMdcVertexWriter::HMdcVertexWriter(void)
{
    // Default constructor .
    outfile       = NULL;
    vertex        = NULL;
    filename      = "";
    fileNameFromInput = kTRUE;
    isEmbedding       = kFALSE;


}
HMdcVertexWriter::HMdcVertexWriter(const Text_t* name,const Text_t* title)
:  HReconstructor(name,title)
{
    // Constructor calls the constructor of class HReconstructor with the name
    // and the title as arguments.
    outfile       = NULL;
    vertex        = NULL;
    filename      = "";
    fileNameFromInput = kTRUE;
    isEmbedding       = kFALSE;
}

HMdcVertexWriter::~HMdcVertexWriter(void) {
  // destructor close output if still open

    if (outfile){
        vertex ->Write();
	outfile->Close();
	outfile = NULL;
	vertex  = NULL;
    }
}
Bool_t HMdcVertexWriter::init(void)
{
    // init the ascii io ofstream with filename
    // taken from the current file of the data source
    // The file name will be checked inside the eventloop
    // against the HADES outputfile.


    //---------------------------------------------
    // now we have to finde the outputfile name
    // in the init function we have no access to
    // the outputfile name of hades (it will be created
    // after init). We take the file name from the datasource
    // end check in the eventloop if the file name has changed

    HDataSource* datasource = gHades->getDataSource();

    if(datasource){

	filename = datasource->getCurrentFileName();
	if(filename.Last('.') >= 0 )
	{
	    filename.Replace(filename.Last('.'),filename.Length() - filename.Last('.'),"_vertex.root");
	} else {
	    Error("HMdcVertexWriter::init()","Could not retrieve current input file name = %s ! Something seems to be wrong!",filename.Data());
	    exit(1);
	}
    } else {
	Error("HMdcVertexWriter::init()","Retrieve NULL pointer for datasource!");
	exit(1);
    }

    TString dir = filename;

    if(dir.Last('/') >= 0){
        // if filename contains a path.
	dir.Replace(dir.Last('/') + 1,dir.Length() - filename.Last('/') + 1,"");
    } else {
	dir = gSystem->WorkingDirectory();
    }
    Bool_t check = gSystem->AccessPathName(dir.Data(), kWritePermission);
    if(check == kFALSE)
    {   // inverse logic !!!
	outfile = new TFile(filename.Data(),"RECREATE");
	if(!outfile){
	    Error("HMdcVertexWriter::init()","Retrieve NULL for root output file = %s!",filename.Data());
	    exit(1);
	}
        outfile->cd();
	vertex = new TNtuple("vertex","vertex","vX:vY:vZ:seqNumber");

    } else {
	Warning("HMdcVertexWriter::init()","No Permission to write outputfile file = %s! \n Will try with outputfile name!",filename.Data());
    }
    //---------------------------------------------

    // find out if we are running embedding
    HCategory* catKine = (HCategory*)gHades->getCurrentEvent()->getCategory(catGeantKine);
    if(catKine && gHades ->getEmbeddingMode() > 0) {
	isEmbedding   = kTRUE;
    }

    return kTRUE;
}
Int_t HMdcVertexWriter::execute(void)
{
    // check if the root output file should be changed (taking
    // the file name from the HADES out put file). The coordinates
    // of the Event Vertex will be written to the root file output.


    //---------------------------------------------
    // Now we have to check if the outputfile name
    // has changed (due to file splitting)
    // Only needed if output is enabled otherwise
    // we have taken the input file name from the data source
    if(gHades->getOutputFile())
    {
	if(fileNameFromInput && outfile){
	    // we have to close and remove the old file
	    // if we now want to use the output file name
	    // convention
	    outfile->cd();
	    vertex ->Write();
	    delete vertex;
	    outfile->Close();
	    gSystem->Exec(Form("rm %s",filename.Data()));
	    vertex  = NULL;
            outfile = NULL;
	}

	TString currentfile = gHades->getOutputFile()->GetName();

	if(currentfile.Last('.') >= 0 )
	{
	    currentfile.Replace(currentfile.Last('.'),currentfile.Length() - currentfile.Last('.'),"_vertex.root");
	}
	if(currentfile.CompareTo(filename.Data()) != 0)
	{
	    // filename changed

	    filename = currentfile;
	    if(outfile){
		// close old file
		outfile->cd();
		vertex  ->Write();
		delete vertex;
		outfile ->Close();
		vertex  = NULL;
		outfile = NULL;
	    }

	    outfile = new TFile(filename.Data(),"RECREATE");
	    if(!outfile){
		Error("HMdcVertexWriter::init()","Retrieve NULL for root output file = %s!",filename.Data());
		exit(1);
	    }
	    outfile->cd();
	    vertex = new TNtuple("vertex","vertex","vX:vY:vZ:seqNumber");

	}
	fileNameFromInput = kFALSE;
    } else {

	if(!outfile){
	    Error("HMdcVertexWriter::execute()","Could not create filename from  output and input file !");
	    exit(1);
	}


    }
    //---------------------------------------------

    if(outfile){

	HEvent* event = gHades->getCurrentEvent();
	if(event){
	    HEventHeader* header = event ->getHeader();
	    if(header){
		HVertex& event_vertex = header->getVertex();
		if(doSkipNoVertex)
		{
		    // write only if a vertex has been
		    // calculated
		    if(event_vertex.getX() != -1000 &&
		       event_vertex.getY() != -1000 &&
		       event_vertex.getZ() != -1000
		      )
		    {
			vertex->Fill(event_vertex.getX(),
				     event_vertex.getY(),
				     event_vertex.getZ(),
				     header->getEventSeqNumber()
				    );
		    }

		} else {
		    // fill allways
		    vertex->Fill(event_vertex.getX(),
				 event_vertex.getY(),
				 event_vertex.getZ(),
				 header->getEventSeqNumber()
				);
		}
	    }
	}
    }

    return 0;
}
Bool_t HMdcVertexWriter::finalize(void)
{
    // close the root file ouput

    if(outfile) {
	outfile->cd();
        vertex ->Write();
        delete vertex;
	outfile->Close();
	outfile = NULL;
        vertex  = NULL;
    }
    return kTRUE;
}

