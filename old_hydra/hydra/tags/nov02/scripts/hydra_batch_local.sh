#!/usr/local/bin/bash
DATE1="`date +%d%m%y`"
  tapehldpath="nov01/hld"
#  tapeoutputpath="hadesdst/nov01/gen1"
  tapeoutputpath="test/1"
  protocolpath="/d/hades/hadesdst/nov01/gen1/prot/"
  myhydrapath="/u/halo/anal"
  myhydraversion="v5_45"
  mynumberofevents=10
  diagexecutable="hydra_diagnostic_341"
  diagmacro="dstDiagnostic"
  diaginputdir="/s/hadesdst/"

echo $DATE1
if [ $# -eq 4 ]
then
	    myinputhld=$1
	    myoutputroot=$2
	    myanalysismacro=$3
	    myhydrabatch=$4

	    if [ -e ${myoutputroot} ]
	    then
		rm /s/hadesdst/${myinputhld}*.root
	    fi
		
	    if [ -e /s/hadesdst/${myinputhld}*.hld ]
	    then
		    rm /s/hadesdst/${myinputhld}*.hld
	    fi
	    
	    cd /s/hadesdst
	    adsmcli que  ${myinputhld}.hld hades ${tapehldpath} > tmp.txt
	    Querystat=`grep 'no matching objects found' tmp.txt`
	
	    if [ ${?} -eq 0 ]
	    then
		echo file not found on TAPE
		mytempreturn=1
	    else
		mytempreturn=0
	    fi
        
	    rm tmp.txt
	
	    if [ $mytempreturn -eq 0 ]
	    then
		adsmcli retr ${myinputhld}.hld hades ${tapehldpath}
		mytempreturn=$?
		trialcounter=0
		while [ ${mytempreturn} -ne 0 ]
		do
		    let trialcounter=${trialcounter}+1
		    if [ trialcounter -le  6 ]
		    then
			echo adsmcli busy : wait 10 min
			sleep 600
			adsmcli retr ${myinputhld}.hld hades ${tapehldpath}
			mytempreturn=$?
		    else
			adsmcli retr ${myinputhld}.hld hades ${tapehldpath} stage=no
			mytempreturn=$?
		    fi
		done
	    fi
	    cd -

	    echo got the file from robot	

	    . hadenv.sh -l none -H ${myhydrapath} ${myhydraversion}

	    if [ -e /s/hadesdst/${myinputhld}.hld ]
	    then
		${myhydrabatch} ${myanalysismacro} ${myinputhld} ${mynumberofevents} 
		mytempreturn=$?
		if [ mytempreturn -eq 0 ]
		then
		    cout analysis completed succefully 
		    crashed=FALSE
		else
		    echo "COMMAND \"${myhydrabatch} ${myanalysismacro} ${myinputhld} ${mynumberofevents}\" CRASHED"
		    crashed=TRUE
		fi
	    fi

        
	    if [ -e ${myoutputroot} ]
	    then
		myoutputlist=`ls -1 /s/hadesdst/${myinputhld}*.root` 
   		for item in ${myoutputlist}
                do
		    ${diagexecutable} ${diagmacro} ${diaginputdir} ${item}
		    mytempreturn=$?
		    if [ mytempreturn -eq 0 ]
		    then
			echo diagnosis completed successfully
			adsmcli arc ${item} hades ${tapeoutputpath} 0
			mytempreturn=$?
			trialcounter=0
			while [ ${mytempreturn} -ne 0 ]
			do
			    let trialcounter=${trialcounter}+1
			    if [ trialcounter -le  6 ]
			    then
				echo adsmcli busy : wait 10 min
				sleep 600
				adsmcli arc ${item} hades ${tapeoutputpath} 0
				mytempreturn=$?
			    else
				adsmcli arc ${item} hades ${tapeoutputpath} 0 stage=no
				mytempreturn=$?
			    fi
			done
		    else
			echo "FILE ${item} crashed during diagnosis"
		    fi
		done
		
	    fi

else
	echo 
	echo usage $0 inputfilehld outputfileroot
	echo
fi
