#! /usr/local/bin/bash
mypath=/u/hadesdst/nov01/gen2/scripts
mypathout_prot=/d/hades/hadesdst/nov01/gen2
batchname=batch



    
FILES="xx01341050952 xx01341052023 xx01341053256 xx01341054331 \
       xx01341055400 xx01341060420 xx01341012253 xx01341032049 \
       xx01341033127 xx01341035239 xx01341040655 xx01341042841 \
       xx01341043355 xx01341044432 xx01341013730 xx01341014809 \
       xx01341015853 xx01341020958 xx01341022405 xx01341023453 
       xx01341025048"

#FILES="xx01341052023 xx01341053256 xx01341033127 xx01341035239 \
#	xx01341040655 xx01341013730 xx01341014809 xx01341020958 \
#	xx01341025048"

#FILES="xx01341050952"
myanamacro="analysisDSTField"
myhydrabatchfield="hydra_batch_Field"

for item in ${FILES}
do 


bsub -q $batchname -o  ${mypathout_prot}/${item}_prot.out ${mypath}/hydra_batch.sh ${item} /tmp/${item}_dst.root ${myanamacro} ${myhydrabatchfield}


done

