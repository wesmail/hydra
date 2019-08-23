#!/usr/local/bin/bash
myhome=/u/hades-qa/HYDRA
myhydra=online
export ORA_USER=hades_ana/hades@db-hades
export HADES_GO4_MON_ACTIVE=1
echo "RUNNING ON ${HOST}"
if [ -f /usr/local/gsiroot/gsimgr/gsi_environment.sh ]
then
	
	unset PATH
        export PATH=/bin:/usr/local/bin:/usr/bin
	# set gsi variables
	. /usr/local/gsiroot/gsimgr/gsi_environment.sh
        export PATH=$PATH:/usr/X11R6/bin:/usr/bin/X11
        
        export GSI_OS_VERSION=3.0
        
        if [ ! -z `which oraenv` ]
	then
		. oraenv
	if [ ! -z `which go4login` ]
	then
                . go4login 210-00  
                
        
        if [ -f ${myhome}/${myhydra}/admin/hadenv.sh ]
	then
                
                . ${myhome}/${myhydra}/rfioenv.sh 
                . ${myhome}/${myhydra}/onlinenv 
                
                # change this line to use your hadenv.sh and the loading path for HYDRA and the hadesgo4 directory
		. ${myhome}/${myhydra}/admin/hadenv.sh -H ${myhome} ${myhydra} -M ${myhome}/${myhydra} -l all 
	
		alias Go4HadesClient='${HADDIR_HADESGO4}/Go4HadesClient/MainGo4HadesClient'
		# LOCAL means the directory where the hadesgo4 is installed
		export LOCAL=${HADDIR_HADESGO4}
                export GO4USERGUI=${HADDIR_HADESGO4}/Go4Library/libGo4UserGui.so
       	 	
                LD_LIBRARY_PATH=$LOCAL/Go4Library:$LOCAL:$Go4UserGUI:$HADDIR_MDC_LIB:$HADDIR_MDC:$HADDIR_HADESGO4_LIB:$HADDIR_HADESGO4:$HADDIR/lib:$LD_LIBRARY_PATH
		export LD_LIBRARY_PATH
		echo "*******************************************************"
		echo "* GO4 Hades enviroment initialized                    *"
		echo "* Go4Gui -> go4                                       *"
		echo "* AnalysisClient: Go4HadesClient                      *"
		echo "*******************************************************"

else
	echo "Error in ${myhome}/${myhydra}/hadesgo4login.sh: \n \". ${myhome}/${myhydra}/admin/hadenv.sh\" not found!"
fi

else
	echo "Error in ${myhome}/${myhydra}/hadesgo4login.sh: \n \"go4login\" not found!"
fi

else
       	echo "Error in ${myhome}/${myhydra}/hadesgo4login.sh: \n \"oraenv\" not found!"
fi

else
	echo "Error in ${myhome}/${myhydra}/hadesgo4login.sh: \n \"/usr/local/gsiroot/gsimgr/gsi_environment.sh\" not found!"
fi
