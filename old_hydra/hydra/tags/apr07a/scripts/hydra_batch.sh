#!/usr/local/bin/bash
DATE1="`date +%d%m%y`"
  tapehldpath="nov01/hld"
  tapeoutputpath="hadesdst/nov01/gen2"
# tapeoutputpath="test/1"
  protocolpath="/d/hades/hadesdst/nov01/gen2/prot/"
  myhydrapath="/misc/halo/anal"
  myhydraversion="v6_06"
  mymodule=""
  mymoduledir=""
  mynumberofevents=100000000
#default mymodule=none mymoduledir=""
    myscratchdir="/s/hadesdst"

echo $DATE1
if [ $# -eq 4 ]
then
	myinputhld=$1
	myoutputroot=$2
	myanalysismacro=$3
        myhydrabatch=$4

	if [ -e ${myoutputroot} ]
	then
		rm /tmp/${myinputhld}*.root
	fi

	if [ -e /tmp/${myinputhld}*.hld ]
	then
        	rm /tmp/${myinputhld}*.hld
	fi
	
	cd /tmp
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

	echo before retrieving file

	if [ $mytempreturn -eq 0 ]
	then
	      adsmcli retr ${myinputhld}.hld hades ${tapehldpath}
              mytempreturn=$?
	      trialcounter=0
	      while [ ${mytempreturn} -ne 0 ]
	      do
		let trialcounter=${trialcounter}+1
		if [ ${trialcounter} -le  6 ]
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

	. hadenv.sh ${myhydraversion} -H ${myhydrapath}

	if [ -e /tmp/${myinputhld}.hld ]
 	then
            ${myhydrabatch} ${myanalysismacro} ${myinputhld} ${mynumberofevents} 
	    mytempreturn=$?
	    if [ ${mytempreturn} -eq 0 ]
	    then
	       crashed=FALSE
	    else
		echo "COMMAND \"${myhydrabatch} ${myanalysismacro} ${myinputhld} ${mynumberofevents}\" CRASHED"
		crashed=TRUE
	    fi
        fi

        
	 if [ -e ${myoutputroot} ]
	     then
		myoutputlist=`ls -1 /tmp/${myinputhld}*.root` 
   		for item in ${myoutputlist}
                do
		   adsmcli arc ${item} hades ${tapeoutputpath} 0
		   mytempreturn=$?
		   trialcounter=0
		   while [ ${mytempreturn} -ne 0 ]
		   do
			   let trialcounter=${trialcounter}+1
			   if [ ${trialcounter} -le  10 ]
			   then
				echo adsmcli busy : wait 10 min
				sleep 600
				adsmcli arc ${item} hades ${tapeoutputpath} 0
				mytempreturn=$?
			   else
				mv  /tmp/${item} ${myscratchdir}
				mytempreturn=0
			   fi
			   
		   done
		  
		done
		
		if [ ${mytempreturn} -eq 0  ]
		then
		    ls -1 /tmp/${myinputhld}*.root > /tmp/${myinputhld}_check.txt
		    mv  /tmp/${myinputhld}_check.txt ${protocolpath}
		    rm  /tmp/${myinputhld}*.root
		fi
	 fi


       	 if [ -e /tmp/${myinputhld}.hld ]
	 then
	    rm /tmp/${myinputhld}.hld 
	 fi                      	
                   	
else
	echo 
	echo usage $0 inputfilehld outputfileroot
	echo
fi
