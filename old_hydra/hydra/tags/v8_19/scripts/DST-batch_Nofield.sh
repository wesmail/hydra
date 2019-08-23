#! /usr/local/bin/bash
mypath=/u/hadesdst/nov01/gen2/scripts
mypathout_prot=/d/hades/hadesdst/nov01/gen2
batchname=batch


FILES="xx01340163337 xx01340164412 xx01340171714 xx01340174438 \
       xx01340180815 xx01340192322 xx01340194939"





myanamacro="analysisDSTNoField"
myhydrabatchnofield="hydra_batch_NoField"

for item in ${FILES}
do 


bsub -q $batchname -o  ${mypathout_prot}/${item}_prot.out ${mypath}/hydra_batch.sh ${item} /tmp/${item}_dst.root ${myanamacro} ${myhydrabatchnofield}


done

