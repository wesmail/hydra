#!/bin/sh

for i in {0,1,2,3,4,5};
#for i in 0
  do
# root -l<<EOF
#L analyzeWins.C
#omputeWindowsPerMomentum("$1","$2",$i,"$3")
#q
#OF

  root -l<<EOF
.L computeWindowsPerMomentum.C
computeWindowsPerMomentum("$1","$2",$i,"$3","$4",$5)
.q
EOF
done