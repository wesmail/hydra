#! /usr/local/bin/bash
mypath=/u/hadesdst/nov01/gen2/scripts
mypathout_prot=/d/hades/hadesdst/nov01/gen2
batchname=batch


#FILES="xx01341174201 xx01341194912 xx01341163333 \
#       xx01341175048 xx01341185408 xx01341233041 xx01341225500 \
#       xx01341233929 xx01341122335 xx01341125659 xx01341131830 \
#       xx01341133258 xx01341134608 xx01341140843 xx01341143025 \
#       xx01341145049 xx01341151117 xx01341153938 xx01341155041 \
#       xx01341160524 xx01341165709 xx01341172227 xx01341173044 \
#       xx01341180136 xx01341181155 xx01341182734 xx01341191237 \
#       xx01341191435 xx01341194205 xx01341200346 xx01341201426 \
#       xx01341202519 xx01341204908 xx01341205356 \
#       xx01341211540 xx01341215810 xx01341224625 xx01341230341 \
#       xx01341231221 xx01341232209 xx01341234756 xx01341235137\
#       xx01341184508 xx01341204714"
FILES="xx01341184508 xx01341204714"

myanamacro="analysisDSTLowField"
myhydrabatchlowfield="hydra_batch_LowField"

for item in ${FILES}
do 


bsub -q $batchname -o  ${mypathout_prot}/${item}_prot.out ${mypath}/hydra_batch_LowF.sh ${item} /tmp/${item}_dst.root ${myanamacro} ${myhydrabatchlowfield}

done


