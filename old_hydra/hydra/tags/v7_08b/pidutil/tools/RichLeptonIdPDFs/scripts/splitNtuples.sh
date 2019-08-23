#!/bin/sh
#This script calls a macro to separate single, double, fake and hadron-rings (fakes pointing to hadrons)
#It builds an output file with three ntuples used for the further processing of the data
#
BEAMTIME=$1       #a shortcut like 'nov02' describing the beamtime for which we are making parameters
EXPERIMENTTYPE=$2 #'sim' or 'exp'
GENERATION=$3     #the generation of the DSTs
TARGETDIRECTORY=$4/$1/$2/$3/

FILELIST=pdfNtupleList${BEAMTIME}${EXPERIMENTTYPE}${GENERATION}.txt
SPLITFILENAME=SplitPIDNtuples${BEAMTIME}${EXPERIMENTTYPE}${GENERATION}.root

echo "Splitting the PDF-ntuples to three smaller ntuples for easier handling now"
rm ${TARGETDIRECTORY}/${SPLITFILENAME}
root -l <<EOF
.L ../macros/splitPIDNtuple.C
splitPIDNtuple("${TARGETDIRECTORY}","${SPLITFILENAME}","${TARGETDIRECTORY}/${FILELIST}")
EOF

