#!/usr/local/bin/bash
echo on

##
## This file will manage preparations around 
## execution just one single job in another directory,
## possibly on another machine or even on another machine
## so that it will be usable for a batch sending procedure
## It copies necessary files from hldFileDir to 
## tmpBatchDir which must be available from TAPE robot
## and recommended place is "/tmp" directory
## runs the root.exe -l -q -b with a macro name behind
##
echo "      *----------------------------------------*"
echo "      *    singleJobManager (c) A.Sadovski     *"
echo "      *----------------------------------------*"
echo "      " echo $DATE1
echo " "

###############################################################
##################### User Parameters #########################
###############################################################
#
# Parameters from command line while calling
#
origMacrosDir=$1
rootMacroName=$2   # "asdAnalysisLowLevelQA.C"
hldInputFile=$3    # needed by tape robot #instead of "myinputhld"origMacrosDir=$1  #Dir with macro itself and all local parameter files#"/net/home2/hades/sadovski/dev/llqaMacros"
output_Dir=$4      # "/d/hades/sadovski/llqaNov02"
numberEvts=$5      # number of events
firstEvent=$6      # first event to analyse
diskORtape=$7      #"TAPE"            #diskORtape="DISK";
hldFileDir=$8      # Destination of files in case of DISK usage, in case of TAPE it is ignored

#
# Hard coded parameters
#
#hldFileDir="/d/hades/sadovski/llqaNov02"
tapehldpath="nov02/hld"      #inpDataDir="/d/hades/sadovski/llqaNov02/data"
rootLogonFile="rootlogon.C"
tmpBatchDir="/tmp"

###############################################################
###############################################################
###############################################################

echo "------------------------------------------------------------------------------"
echo "- Before we start we suppose you are sure what:"
echo "- macroses:          rootMacroName = ${rootMacroName}"
echo "-            and     rootLogonFile = ${rootLogonFile}"
echo "- are executed from  origMacrosDir = ${origMacrosDir}  directory"
echo "- using              hldInputFile  = ${hldInputFile}" to be taken from ${diskORtape}
echo "- will be taken from: hldFileDir   = ${hldFileDir}"
echo "- Results will go to: output_Dir   = ${output_Dir}"
echo "- numberEvts = ${numberEvts}, starting from  ${firstEvent}"
echo "------------------------------------------------------------------------------"
echo " warning: in case of TAPE ROBOT you better use hldInputFile=/tmp as a storage!"
echo "------------------------------------------------------------------------------"
echo " "
echo "Entering macros dir to run scripts from"


echo cd ${origMacrosDir}
     cd ${origMacrosDir}

echo "Can you see yours macroses here?"
echo ls -l *.C
     ls -l *.C


if [ $diskORtape == "TAPE" ]
then
   echo $DATE1
   echo "-------------------------------------------------------------------------"
   echo "--- Reading from the TAPE robot, Temporary storage will be ${tmpBatchDir}"
   echo "-------------------------------------------------------------------------"
   echo cd ${tmpBatchDir}
        cd ${tmpBatchDir}

   echo  adsmcli que  ${hldInputFile}.hld hades ${tapehldpath}
         adsmcli que  ${hldInputFile}.hld hades ${tapehldpath} > tmp.txt
   Querystat=`grep 'no matching objects found' tmp.txt`

   if [ ${?} -eq 0 ]
   then
     echo file not found on TAPE
     mytempreturn=1
   else
     mytempreturn=0
   fi

   rm tmp.txt

   echo before retrieving file

   if [ $mytempreturn -eq 0 ]
   then
         echo adsmcli retr ${hldInputFile}.hld hades ${tapehldpath}
         adsmcli retr ${hldInputFile}.hld hades ${tapehldpath}
         mytempreturn=$?
         trialcounter=0
         while [ ${mytempreturn} -ne 0 ]
         do
   	echo ${trialcounter}. attempt to retrieve file from tape
   	let trialcounter=${trialcounter}+1
   	if [ ${trialcounter} -le  6 ]
   	then
   	    echo adsmcli busy : wait 10 min
   	    sleep 600
   	    adsmcli retr ${hldInputFile}.hld hades ${tapehldpath}
   	    mytempreturn=$?
   	else
   	    adsmcli retr ${hldInputFile}.hld hades ${tapehldpath} stage=no
   	    mytempreturn=$?
   	fi
         done
   fi
   ##No we work on /tmp##echo changing back to old directory
   ##No we work on /tmp##cd -

   echo "got the file from robot, go back to macroses directory"
   echo cd -
        cd -

else
   echo "|------------------------------------------------------------------------"
   echo "| Reading from disk: HLD-files directory is: ${tmpBatchDir} "
   echo "| Warning: TAPE ROBOT can not see /misc/haddisk! Use /d/hades/... instead"
   echo "|------------------------------------------------------------------------"
   echo "| we will read files directly from ${tmpBatchDir}"
   echo "|------------------------------------------------------------------------"

   tmpBatchDir=${hldFileDir}

   #echo cp ${hldFileDir}/${hldInputFile}.hld ${tmpBatchDir}
   #     cp ${hldFileDir}/${hldInputFile}.hld ${tmpBatchDir}

fi


#
# Installing and Loading ROOT version, HYDRA libraries and shered libraries
# just before the execution of the ROOT macros
#
. rootlogin dev
. hadenv.sh dev -l "mdc" -M /u/sadovski/dev
. oraenv

echo "--------------------------------------------------------------"

k="\""
q="'"
echo  root.exe -l -b "${rootMacroName}( ${k}${tmpBatchDir}${k} , ${k}${hldInputFile}${k} , ${k}${output_Dir}${k} , ${numberEvts} ,${firstEvent} )"  #executing the command
      root.exe -l -b "${rootMacroName}( ${k}${tmpBatchDir}${k} , ${k}${hldInputFile}${k} , ${k}${output_Dir}${k} , ${numberEvts} ,${firstEvent} )"  #executing the command

echo "--------------------------------------------------------------"


if [ $diskORtape == "TAPE" ]
then
  if [ -e ${tmpBatchDir}/${hldInputFile}.hld ]
    then
       echo rm ${tmpBatchDir}/${hldInputFile}.hld from disk
            rm ${tmpBatchDir}/${hldInputFile}.hld 
       echo adsmcli unstage  ${hldInputFile}.hld hades ${tapehldpath}
            adsmcli unstage  ${hldInputFile}.hld hades ${tapehldpath}
  fi        

  echo ls ${tmpBatchDir}
       ls ${tmpBatchDir}
fi

echo "      *----------------------------------------*"
echo "      *        END of singleJobManager         *"
echo "      *----------------------------------------*"
