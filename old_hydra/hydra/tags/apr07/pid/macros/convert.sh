myoutputlist=`ls -1 *.ps`
for item1 in ${myoutputlist}
do
file=$(echo $item1 | sed -e 's/\.ps/\.png/g')
echo $file
convert -resize 1280x960 -rotate 90 ${item1} $file
rm  ${item1}
done
