#!/usr/local/bin/bash

############################################################
# set ALL basic environment variables for hydra
# Date: 27 Apr 2006
############################################################
export CVSROOT=/misc/halo/repos/anal
. rootlogin  404-02-g 
. oraenv
. /misc/halo/anal/apr06/rfioenv.sh
. /misc/halo/anal/apr06/onlinenv
. /misc/halo/anal/apr06/hadenv.sh -M /u/hades-qa/online-dsts/apr06 -l "qa" apr06
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$HADDIR/lib:/u/hades-qa/online-dsts/apr06/lib
