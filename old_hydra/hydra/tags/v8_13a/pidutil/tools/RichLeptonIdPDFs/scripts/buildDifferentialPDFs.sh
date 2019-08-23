#!/bin/sh
#This script controls the execution of the ROOT macro that actually fills the PDF-histograms
#Depending on the conf0g-variables histograms for different types of tracking are filled
#The variation of ring-properties is also taken into account when the appropriate config-flags
#are set.

BEAMTIME=$1       #a shortcut like 'nov02' describing the beamtime for which we are making parameters
EXPERIMENTTYPE=$2 #'sim' or 'exp'
GENERATION=$3     #the generation of the DSTs
TARGETDIRECTORY=$4/$1/$2/$3/
EXPERIMENT_ID=${BEAMTIME}${EXPERIMENTTYPE}${GENERATION}
CENTROID=$5
TRACKCONTEXT=$6

shift 6

TRACKINGFLAGS=( 0 0 0 0 0 0 )
if [ "${TRACKCONTEXT}" == "BestRes" ]; then 
    if [ -n "$6" ]; then

	TRACKINGFLAGS=( $1 $2 $3 $4 $5 $6 )
    else 
	echo "specify 6 tracking flags when running in best-resolution-mode!"
	exit
    fi
fi

for sector in {0,1,2,3,4,5}
do
  if [ ${TRACKINGFLAGS[${sector}]} -eq 1 ]; then
      TRACKTYPE=spline
  elif [ ${TRACKINGFLAGS[${sector}]} -eq 0 ]; then
      TRACKTYPE=kick
  else
      echo "No tracking ${trackingFlags[${sector}]} is known"
  fi
      
  if [ "$CENTROID" = "w" ] 
      then
      
      for i in {1,3,5}
	do
	let lower=$i
	let upper=$lower+2
	if [ $lower -eq 5 ]
	    then upper=9
	fi 
	root -l <<EOF
.L ../macros/buildRICHPDFs.C
buildRICHPDFs("n_rich_patmatrix","f_rich_avcharge",1,0,1,1,0,"${TARGETDIRECTORY}/SplitPIDNtuples${EXPERIMENT_ID}.root",${lower}*10.0,${upper}*10.0,"${TARGETDIRECTORY}/","${EXPERIMENT_ID}",$sector,1,1,0,"${TRACKTYPE}","${TRACKCONTEXT}");
EOF
	
      done
  fi
  
if [ "$CENTROID" = "n" ] 
      then
      
      for i in {1,3,5}
	do
	let lower=$i
	let upper=$lower+2
	if [ $lower -eq 5 ]
	    then upper=9
	fi 
	root -l <<EOF
.L ../macros/buildRICHPDFs.C
buildRICHPDFs("n_rich_patmatrix","f_rich_avcharge",1,0,1,1,0,"${TARGETDIRECTORY}/SplitPIDNtuples${EXPERIMENT_ID}.root",${lower}*10.0,${upper}*10.0,"${TARGETDIRECTORY}/","${EXPERIMENT_ID}",$sector,-1,1,0,"${TRACKTYPE}","${TRACKCONTEXT}");
EOF
	
      done
  fi


done

