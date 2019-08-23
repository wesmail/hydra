#pragma implementation

// -----------------------------------------------------------------------------

#include "hsuhldfileinfo.h"
#include "hldfilevt.h"
#include "hevent.h"
#include "hrecevent.h"

// -----------------------------------------------------------------------------
//*-- Author : Marcin Jaskula
// -----------------------------------------------------------------------------

//______________________________________________________________________________
// A simple class for counting the number of events in the hld file.
// The outlook is to add here some histograms for getting information
// about the subevents structures and sizes in the file

ClassImp(HSUHldFileInfo);

// -----------------------------------------------------------------------------

HSUHldFileInfo::HSUHldFileInfo(Text_t *psName, Bool_t bAutoProceed)
                : HldFileSource()
{
// Open the hld file
// If bAutoProceed = kTRUE then automatically the file is proceeded
char *p;

    if((p = strrchr(psName, '/')) == NULL)
    {
        setDirectory(".");
        addFile(psName);
    }
    else
    {
        *p = '\0';
        setDirectory(psName);
        addFile(p + 1);
        *p = '/';
    }

    m_ppEvent = NULL;
    setEventAddress(&m_ppEvent);

    init();

    if(bAutoProceed)
        proceed();
}

// -----------------------------------------------------------------------------

void HSUHldFileInfo::print(void)
{
// Print info about the file name, its run id and number of events
    printf("File  : %s\nRunId : %d\nEvents: %d\n",
            getCurrentFileName(), getCurrentRunId(), fEventNr);
}

// -----------------------------------------------------------------------------

void HSUHldFileInfo::proceed(void)
{
// Calculate the events number in the file
    for( ; ; )
    {
        if((isDumped) && (dumpEvt() == kDsError))
            break;

        if((isScanned) && (scanEvt() == kDsError))
            break;

        if( ! ((HldFilEvt*)fReadEvent)->execute())
            break;

        fEventNr++;
        decodeHeader((*fEventAddr)->getHeader());
    }
}
