#!/bin/sh

#This script takes a set of variables describing what data you want to process and a path to
#yout inputfiles. It then builds a text-file holding a list of thes inputfiles. The list is required
#to run the subsequent scripts. The only convention is that the files match the regexp 'sum*.root'

BEAMTIME=$1       #a shortcut like 'nov02' describing the beamtime for which we are making parameters
EXPERIMENTTYPE=$2 #'sim' or 'exp'
GENERATION=$3     #the generation of the DSTs
NTUPLEPATH=$4     #where are the ntuples to merge
TARGETDIRECTORY=$5/$1/$2/$3/

if [ -d ${TARGETDIRECTORY} ]; then
    echo "Target exists!" 
#    exit 
fi

mkdir -p ${TARGETDIRECTORY}


#make an ascii file containing the full path names of all merged pdf-ntuples to be processed
#this is required by some of the ROOT macros running in batch mode

FILELIST=pdfNtupleList${BEAMTIME}${EXPERIMENTTYPE}${GENERATION}.txt
ls ${NTUPLEPATH}/sum*.root > ${TARGETDIRECTORY}/${FILELIST}