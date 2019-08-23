#!/usr/bin/ksh
# startup script for Go4HadesClient example J.A. 30-Jul-2002

unset PATH
PATH=$PATH:${HOME}/bin:/misc/halo/bin:/cern/98/bin:/usr/local/bin:/usr/bin:/bin:/usr/X11R6/bin:/usr/bin/X11:/LSF/lsf-4.0.1/bin
export PATH

. /misc/kempter/HYDRA/local/hadesgo4/Go4HadesClient/hadesgo4login.sh
echo $PATH
echo useranalysis startup script for hades client
echo "*******************************************************"
echo "* GO4 Shower enviroment initialized                   *"
echo "* Go4Gui -> go4                                       *"
echo "* AnalysisClient: Go4HadesClient                      *"
echo "*******************************************************"
echo $GO4SYS
echo starting client..with  $2 $3 $4 
${1%% -gui}
${HADDIR_HADESGO4}/Go4HadesClient/MainGo4HadesClient $2 $3 $4 ${1%% -gui}
