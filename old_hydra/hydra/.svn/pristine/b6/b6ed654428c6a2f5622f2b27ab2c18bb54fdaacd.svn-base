#!/usr/bin/ksh
myhome=/misc/halo
myhydra=anal/dev

echo "RUNNING ON ${HOST}"
if [ -f /usr/local/gsiroot/gsimgr/gsi_environment.sh ]
then
	
	unset PATH
	# set gsi variables
	. /usr/local/gsiroot/gsimgr/gsi_environment.sh
        PATH=$PATH:/misc/halo/bin:/cern/98/bin:/usr/local/bin:/usr/local/teTeX/bin:/usr/bin:/bin:/usr/X11R6/bin:/usr/bin/X11:/LSF/lsf-4.0.1/bin
	export PATH
        
        export GSI_OS_VERSION=3.0
        
                if [ ! -z `which oraenv` ]
	then
		. oraenv
	if [ ! -z `which go4login` ]
	then
      		. go4login 207-00
        
        if [ -f ${myhome}/${myhydra}/admin/hadenv.sh ]
	then
                
                #. ${myhome}/${myhydra}/rfioenv.sh 
                . ${myhome}/${myhydra}/onlinenv 
                
                # change this line to use your hadenv.sh and the loading path for HYDRA and the hadesgo4 directory
		. ${myhome}/${myhydra}/admin/hadenv.sh -H ${myhome} ${myhydra} -M ${myhome}/${myhydra} -l all 
	
		alias Go4HadesClient='${HADDIR_HADESGO4}/Go4HadesClient/MainGo4HadesClient'
		# LOCAL means the directory where the hadesgo4 is installed
		export LOCAL=${HADDIR_HADESGO4}

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
