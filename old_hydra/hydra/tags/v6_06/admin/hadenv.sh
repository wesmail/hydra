#!/usr/local/bin/bash

############################################################
# hadenv.sh
# set basic environment variables for hydra
# Author: P.Zumbruch (P.Zumbruch@gsi.de)
# Date: 09 July 2001
# Last Modification: 15 November 2001
############################################################

## --------------------------------------------------
## List of Modules 
## --------------------------------------------------

MODULES="basedir \
	 base alignment evtserver kickplane mdc mdctrackD mdctrackS \
	 online ora orautil phyana qa revt rich richutil shower showertofino \
	 showerutil start tof tofino trigger"

## --------------------------------------------------
## List of Submodules 
## --------------------------------------------------

BASESUBMODULES="correlation datasource datastruct \
		eventhandling evtserver geometry runtimedb util"

DUMP=FALSE

################################################
################################################
# modify in this place here
################################################
################################################

#This is the debug level the libraries will be compiled with. A higher
#debug level means that more info is printed during execution.

debuglevel=0

#This is the halo base directory

HYDRADIR=/misc/halo/anal

#This is the base directory for your local developments

MYDEVHADDIR=~/anal/dev

#load the resources of this modules from your local development path MYDEVHADDIR
#     example: LOCALS="kickplane mdc mdctrackD mdctrackS" 
LOCALS=""

################################################
################################################
# nothing more to edit !
################################################
################################################

## --------------------------------------------------
## check for help option
## --------------------------------------------------

helper=`echo $1 | sed "s/\-h.*/\-h/g"`  
if [ "$helper" = "-h" ] 
then
    shift 
fi

## --------------------------------------------------
## Exit and give help if no arguments are given or 
## help is requested
## --------------------------------------------------

if [ -z $1 ]
then
    echo "       "
    echo "Usage: . `basename $0` (-ldDHM) version"
    echo
    echo "       version         : subdir of general HYDRA resource" 
    echo "                         (default: $HYDRADIR)"
    echo "                         e.g. pro new may2001 test"
    echo "                         MUST be the last argument"
    echo "       -d value        : set hades debug level to value"
    echo "       -D              : dump the exported environment variables"
    echo "                         to stdout"
    echo "       -H path         : set the general source path from"
    echo "                         $HYDRADIR to path"
    echo "       -l \"mdc rich\"   : modules to be loaded from" 
    echo "                         the local resource path"
    echo "                         (default: $MYDEVHADDIR)"
    echo "       -l all          : set all modules to be loaded from"
    echo "                         local resource,"
    echo "                         (default: $MYDEVHADDIR)"
    echo "                         in this case \$HADDIR is set to the local resource"
    echo "                         argument \'version\' is not needed."
    echo "       -l none         : set all modules to be loaded from"
    echo "                         general resource," 
    echo "                         (default: $HADDIR)"
    echo "       -M path         : set the local source path from"
    echo "                         $MYDEVHADDIR to path"
    echo "       -a              : add additional modules, not contained in "
    echo "                         \$MODULES"
    echo "       "
else

    ## --------------------------------------------------
    ##  checking for right method to call the script
    ## --------------------------------------------------
   
    if [ `basename $0` = hadenv.sh ]
    then 
	echo
	echo use a leading " . " to call the `basename $0` script!
	echo -e "exiting ...\n"
	exit
    fi

    ## --------------------------------------------------
    ##  Argument and Options parsing
    ## --------------------------------------------------
   
    argument=FALSE
    alllocal=FALSE
    nonlocal=FALSE

    while [ $# -gt 0 ]
    do
     Option=$1

	case $Option in
	   -v ) version=$2
		shift 
		;;
	   -d ) debuglevel=$2
		shift 
		shift 
		;;
	   -D ) DUMP=TRUE
		shift
		;;
	   -H ) HYDRADIR=$2
		shift 
		shift 
		;;
	   -l ) if [ "${2}" = "all" ]
		    then 
			alllocal=TRUE
			LOCALS=${MODULES}
	 	elif [ "${2}" = "none" ]
		    then
			nonelocal=TRUE
			LOCALS=""
		else
		    LOCALS=${2}
		fi
		shift 
		shift 
		;;
	   -a ) LOCALS="${LOCALS} $2"
		shift 
		shift 
		;;
    	   -M ) MYDEVHADDIR=$2
		shift 
		shift 
		;;
	    * ) argument=TRUE
		temporaer=$Option
		shift 
		;;
         esac
    done

    ## --------------------------------------------------
    ##  Check if argument for hydra release is supplied
    ## --------------------------------------------------

    if [ "$alllocal" = "FALSE" ]
	then
	    if [ "$argument" = "FALSE" ]
		then
		    echo -e "\nmissing argument!\nuse hadenv.sh -h for help\n"
		    return
	fi
    fi

    ## --------------------------------------------------
    ##  Check if directories exist
    ## --------------------------------------------------

    if [ "$alllocal" = "FALSE" ]
	then
	    if [ ! -d "$HYDRADIR/$temporaer" ]
		then 
		    echo
		    echo directory $HYDRADIR/$temporaer does not exist! 
		    echo ... exiting.
		    echo
		    return
		else
		export HADDIR=$HYDRADIR/$temporaer
	fi
    else
	export HADDIR=$MYDEVHADDIR
    fi

    if [ "$nonelocal" = "FALSE" ]
	then
	    if [ ! -d "$MYDEVHADDIR" ]
		then 
		    echo
		    echo local directory $MYDEVHADDIR does not exist! 
		    echo ... exiting.
		    echo
		return
	    else
	    export MYDEVHADDIR
	fi
    fi

    export HDL=$debuglevel


    ## --------------------------------------------------
    ##  Generate environment variables (1) - MODULES_LIB_LIST
    ## --------------------------------------------------
    unset HYDRA_MODULES

    for item in ${MODULES}
	do 
	    HYDRA_MODULES="${HYDRA_MODULES}${item}lib "
	done  
    export HYDRA_MODULES

    ## --------------------------------------------------
    ##  Generate environment variables (2)
    ## --------------------------------------------------

    for item in ${MODULES}
	do
	    myitem=`echo $item | tr '[a-z]' '[A-Z]' `
	    export HADDIR_${myitem}=$HADDIR/$item
	    export HADDIR_${myitem}_LIB=$HADDIR/lib
	    if [ $item = basedir ]
		then
		    export HADDIR_${myitem}=$HADDIR
		    export HADDIR_${myitem}_LIB=$HADDIR/lib
	    fi
	done
	
    for item in ${LOCALS}
	do
	    myitem=`echo $item | tr '[a-z]' '[A-Z]' `
	    export HADDIR_${myitem}=$MYDEVHADDIR/$item
	    export HADDIR_${myitem}_LIB=$MYDEVHADDIR/lib
	    if [ $item = basedir ]
		then
		    export HADDIR_${myitem}=$MYDEVHADDIR
		    export HADDIR_${myitem}_LIB=$MYDEVHADDIR/lib
	    fi
	done

    ## --------------------------------------------------
    ##  Generate environment variables (3) - INCLUDES
    ## --------------------------------------------------

    unset HYDRA_VAR_INCLUDES
    unset HYDRA_INCLUDES
    for item in ${MODULES}
	do 
	if [ $item != "basedir" ]
	then
	    myitem=`echo $item | tr '[a-z]' '[A-Z]' `
	    if [ $item = "base" ]
	    then
		for element in ${BASESUBMODULES}
		do
		    HYDRA_VAR_INCLUDES="${HYDRA_VAR_INCLUDES}-I\${HADDIR_${myitem}}/${element} "
		done
	    else
		HYDRA_VAR_INCLUDES="${HYDRA_VAR_INCLUDES}-I\${HADDIR_${myitem}} "
	    fi
	fi
	done  
    export HYDRA_VAR_INCLUDES
    export HYDRA_INCLUDES=`eval echo $HYDRA_VAR_INCLUDES`

    ## --------------------------------------------------
    ##  PATHS ...
    ## --------------------------------------------------

    export PATH=$PATH:$HADDIR/exec

    ## --------------------------------------------------
    ##  Screen output
    ## --------------------------------------------------
    
    echo " "
    echo ".-------------------------------------------------------------."
    echo "|            HADES analysis environment initialized           |"
    echo ".-------------------------------------------------------------."
    echo "|  Directories set:                 "
    echo "|   Base dir                : $HADDIR        "
    echo "|   Libraries dir           : $HADDIR/lib    "
    echo "|   Macros dir              : $HADDIR/macros "
    echo "|"  
    echo "|   User development dir    : $MYDEVHADDIR "
    
    ## --------------------------------------------------
    ##  Screen output - locally linked modules
    ## --------------------------------------------------

    if [ -n "${LOCALS}" ]
	then
	echo -n "|   locally linked module(s): "
	counter=0
	for item in ${LOCALS}
	do
	    if [ ${counter} -eq 4 ]
	    then 
		echo
		echo -n "|                             "
		counter=0
	    fi
	    echo -n "$item "
	    let counter=$counter+1
	done
    echo
    fi
    echo ".-------------------------------------------------------------."
    
    ## --------------------------------------------------
    ##  DUMP values of generated values to stdout
    ## --------------------------------------------------
    
    if [ $DUMP = TRUE ]
	then
	    set | grep HADDIR_ 
    fi
fi
