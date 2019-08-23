#!/usr/local/bin/bash

############################################################
# rfioenv.sh
# set rfio environment variables for remote file i/o 
# Author: P.Zumbruch (P.Zumbruch@gsi.de)
# Date: 09 Oct 2001
# Last Modification: 14 November 2001
############################################################

## --------------------------------------------------
##  checking for right method to call the script
## --------------------------------------------------
   
    if [ `basename $0` = rfioenv.sh ]
    then 
	echo
	echo use a leading " . " to call the `basename $0` script!
	echo -e "exiting ...\n"
	exit
    fi

## --------------------------------------------------
##  export variables
## --------------------------------------------------

    ADSM_BASE=/GSI/staging/adsm/v20
    if [ -d ${ADSM_BASE} ]
    then
	export RFIO_SUPPORT=on
	export ADSM_BASE
	    
    ## --------------------------------------------------
    ##  printout
    ## --------------------------------------------------
	
	echo ".-------------------------------------------------------------."
	echo "|            RFIO environment initialized                     |"
	echo ".-------------------------------------------------------------."

    else
	clear
    ## --------------------------------------------------
    ##  printout
    ## --------------------------------------------------
	
    	echo "!!! ----------------------------------------------------------."
	echo "!!!  ERROR while initializing RFIO environment                |"
	echo "!!!                                                           |"
	echo "!!!  directory: ${ADSM_BASE} is not a valid path !!!          |" 
	echo "!!! ----------------------------------------------------------."
    fi

    
