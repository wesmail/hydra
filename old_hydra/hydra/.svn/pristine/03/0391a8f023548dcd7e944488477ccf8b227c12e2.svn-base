#!/usr/local/bin/bash

############################################################
# defall.sh
# set ALL basic environment variables for hydra
# Date: 18 Feb 2004 J.Markert
############################################################
myvers=v6_15
mybase=/misc/halo
mypath=${mybase}/anal

export PATH=${mybase}/valgrind/bin:$PATH  # valgrind: memory debugging tool
export CVSROOT=${mybase}/repos/anal
. rootlogin 303-09-hades-debug
. oraenv
. ${mypath}/${myvers}/hadenv.sh -H ${mypath} -M ${mypath}/${myvers} -l "none" ${myvers}
. ${mypath}/${myvers}/onlinenv
. ${mypath}/${myvers}/rfioenv.sh
#. ${mypath}/${myvers}/distenv.sh     # distributed compiling
