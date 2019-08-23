. oraenv
. go4login 
# change this line to use your hadenv.sh and the loading path for HYDRA and the hadesgo4 directory
. /misc/kempter/bin/hadenv.sh -H /misc/halo/anal dev -M /misc/kempter/HYDRA/local -l "hadesgo4" -D 
alias Go4HadesClient='${HADDIR_HADESGO4}/Go4HadesClient/MainGo4HadesClient'
# LOCAL means the directory where the hadesgo4 is installed
export LOCAL=${HADDIR_HADESGO4}

LD_LIBRARY_PATH=$LOCAL/Go4Library:$LOCAL:$Go4UserGUI:$HADDIR_MDC_LIB:$HADDIR_MDC:$HADDIR_HADESGO4_LIB:$HADDIR_HADESGO4:$HADDIR/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH
echo "*******************************************************"
echo "* GO4 Hades enviroment initialized                   *"
echo "* Go4Gui -> go4                                       *"
echo "* AnalysisClient: Go4HadesClient                      *"
echo "*******************************************************"
