#!/bin/bash
. hyddev dev+
cd -

BEAMTIME=$1       #a shortcut like 'nov02' describing the beamtime for which we are making parameters
EXPERIMENTTYPE=$2 #'sim' or 'exp'
GENERATION=$3     #the generation of the DSTs

TARGETDIRECTORY=$4/${BEAMTIME}/${EXPERIMENTTYPE}/${GENERATION}/
TRACKINGCONTEXT=$5

mkdir -p ${TARGETDIRECTORY}

EXPERIMENT_ID=${BEAMTIME}${EXPERIMENTTYPE}${GENERATION}

FILELIST=pdfNtupleList${EXPERIMENT_ID}.txt

LOCAL=$PWD

shift 5

TF=( 0 0 0 0 0 0 )
if [ "${TRACKINGCONTEXT}" == "BestRes" ]; then 
    if [ -n "$6" ]; then

	TF=( $1 $2 $3 $4 $5 $6 )
    else 
	echo "specify 6 tracking flags when running in best-resolution-mode!"
	exit
    fi
fi

root -l <<EOF
.L ../macros/computeNormalizationFromSmallNTuple.C
computeNormalizationBatch("${TARGETDIRECTORY}","${TARGETDIRECTORY}/${FILELIST}","${EXPERIMENT_ID}","${TRACKINGCONTEXT}",${TF[0]},${TF[1]},${TF[2]},${TF[3]},${TF[4]},${TF[5]});
EOF
