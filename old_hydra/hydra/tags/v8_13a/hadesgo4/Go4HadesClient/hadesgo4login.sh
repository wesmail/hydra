#!/usr/local/bin/bash
myhome=/u/hades-qa/HYDRA
myhydra=online
export ORA_USER=hades_ana/hades@db-hades
export HADES_GO4_MON_ACTIVE=1
export CVSROOT=/misc/halo/repos/anal
echo "RUNNING ON ${HOST}"

	
unset PATH
export PATH=/bin:/usr/local/bin:/usr/bin
export PATH=$PATH:/usr/X11R6/bin:/usr/bin/X11
export GSI_OS_VERSION=3.1  # work arround!
        
if [ ! -z `which oraenv` ]
then
    # . oraenv
    if [ ! -z `which go4login` ]
    then
        . go4login 210-00  
                
        
        if [ -f ${myhome}/${myhydra}/defall.sh ]
	then
	    . ${myhome}/${myhydra}/defall.sh 
	
	    alias Go4HadesClient='${HADDIR}/hadesgo4/Go4HadesClient/MainGo4HadesClient'
	    # LOCAL means the directory where the hadesgo4 is installed
	    export LOCAL=${HADDIR}/hadesgo4
            export GO4USERGUI=${HADDIR}/hadesgo4/Go4Library/libGo4UserGui.so
       	 	
	    LD_LIBRARY_PATH=$LOCAL/Go4Library:$LD_LIBRARY_PATH
	    export LD_LIBRARY_PATH
	    echo "*******************************************************"
	    echo "* GO4 Hades enviroment initialized                    *"
	    echo "* Go4Gui -> go4                                       *"
	    echo "* AnalysisClient: Go4HadesClient                      *"
	    echo "*******************************************************"

        else
	    echo "Error : \n \". ${myhome}/${myhydra}/defall.sh\" not found!"
        fi

    else
	echo "Error : \n \"go4login\" not found!"
    fi

else
    echo "Error : \n \"oraenv\" not found!"
fi
