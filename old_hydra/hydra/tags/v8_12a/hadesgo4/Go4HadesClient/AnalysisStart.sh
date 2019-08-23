#!/usr/bin/ksh
# startup script for Go4HadesClient example J.A. 30-Jul-2002
echo "Starting AnalysisStart.sh....."
echo "$1 $2 $3 $4"
myparameter=$1
myexec=$2
myhost=$3
myport=$4

unset PATH
PATH=$PATH:${HOME}/bin:/misc/halo/bin:/cern/98/bin:/usr/local/bin:/usr/bin:/bin:/usr/X11R6/bin:/usr/bin/X11:/LSF/lsf-4.0.1/bin
export PATH

. /misc/halo/anal/dev/hadesgo4/Go4HadesClient/hadesgo4login.sh

echo $PATH
echo useranalysis startup script for hades client
echo "*******************************************************"
echo "* GO4 Hades enviroment initialized                    *"
echo "* Go4Gui -> go4                                       *"
echo "* AnalysisClient: Go4HadesClient                      *"
echo "*******************************************************"
echo $GO4SYS


echo $LD_LIBRARY_PATH

echo $PATH

echo $ROOTSYS

echo $GO4SYS




echo starting client..with  $myexec $myhost $myport ${myparameter%% -gui}
${HADDIR_HADESGO4}/Go4HadesClient/MainGo4HadesClient $myexec $myhost $myport ${myparameter%% -gui}

