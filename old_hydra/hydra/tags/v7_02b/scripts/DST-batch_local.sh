#! /usr/local/bin/bash
mypath=/u/hadesdst/nov01/gen1/scripts
mypathout_prot=/d/hades/hadesdst/nov01/gen1
batchname=batch




FILES="xx01341033127"


myanamacro="analysisDSTlocal"
myhydrabatchfield="hydra_batch_local"

for item in ${FILES}
do 



${mypath}/hydra_batch_local.sh ${item} /s/hadesdst/${item}.root ${myanamacro} ${myhydrabatchfield}

done

