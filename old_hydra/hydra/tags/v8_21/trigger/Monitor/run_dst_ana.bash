#!/bin/bash
#-------------------------------------------------------
# Do everything automatically for apr06 beamtime
# get_hld, showerCORR, DST, rich, tof, mu, muEff
# write logfile, write histogramms to postscript files
# convert postscripts to pdf and show it.
#                            4 May 2006, Bjoern Spruck
#--------------------------------------------------------

# Set the Hydra env. Just in case...

. camilla/startEnv/hades.sh

# HLD and DST is hardcoded in dst macro :-(
# -> dont chnage it here
# would be useful to read one and write other disk (speedup)

HLDPATH=/scratch.local2/hld
DSTPATH=/scratch.local2/dst

# here we write logfiles
LOGPATH=/scratch.local/log
RESULTPATH=/scratch.local/corr
POSTSCRIPTPATH=/scratch.local/corr_ps

# in this path the analysis macros are

ANAPATH=/u/htrigger/camilla

# only used for analysis, not for dst
EVTCOUNT=100000

rm "$LOGPATH/dst_logfile.txt"
rm "$LOGPATH/shower_logfile.txt"
rm "$LOGPATH/tof_logfile.txt"
rm "$LOGPATH/rich_logfile.txt"
rm "$LOGPATH/mu_logfile.txt"
rm "$LOGPATH/mueff_logfile.txt"

OLDPATH=$(pwd)

cd $HLDPATH
echo "Getting hld file"
get_hld -v

file=$(ls -tr be*.hld| tail -1)

echo "using Filename: $file"
echo "$HLDPATH/$file"

number=${file:0:13}

echo
echo "============"
echo "    SHOWER"
echo "============"
$ANAPATH/shower/sho_shoIPU_corr2 "$HLDPATH/$file" $EVTCOUNT | tee "$LOGPATH/shower_logfile.txt" | grep 0000
tail -n 10 "$LOGPATH/shower_logfile.txt"
corr=$(tail -n 1 "$LOGPATH/shower_logfile.txt")
echo "Create postscript from $corr"
$ANAPATH/shower/show_shower $corr

echo
echo "============"
echo "running DST"
echo "============"

$ANAPATH/dst/onlineDST_apr06_2_old "$file" $EVTCOUNT | tee "$LOGPATH/dst_logfile.txt" | grep 000

if [ $? -eq 0 ]
then
	echo "successfully analysed"
else
	echo "error in analysis"
#	rm "$HLDPATH/$file"
	exit
fi
#rm "$HLDPATH/$file"

tail -n 10 "$LOGPATH/dst_logfile.txt"

cd $DSTPATH
file=$(ls -rt be*.root| tail -n 1)
echo "using Filename: $file"
if [ -e $file ]
then
	echo "existing"
else
	echo "dst file is missing"
#	rm "$HLDPATH/$file"
	exit
fi

echo "running analysis steps"
echo "$DSTPATH/$file $EVTCOUNT"

echo
echo "============"
echo "    TOF"
echo "============"
$ANAPATH/tof/tof-tofIPU_corr2 "$DSTPATH/$file" $EVTCOUNT | tee "$LOGPATH/tof_logfile.txt" | grep 0000
tail -n 10 "$LOGPATH/tof_logfile.txt"
corr=$(tail -n 1 "$LOGPATH/tof_logfile.txt")
echo "Create postscript from $corr"
$ANAPATH/tof/show_tof $corr

echo
echo "============"
echo "   RICH"
echo "============"
$ANAPATH/rich/rich-richIPU_corr2 "$DSTPATH/$file" $EVTCOUNT | tee "$LOGPATH/rich_logfile.txt" | grep 0000
tail -n 10 "$LOGPATH/rich_logfile.txt"
corr=$(tail -n 1 "$LOGPATH/rich_logfile.txt")
echo "Create postscript from $corr"
$ANAPATH/rich/show_rich $corr

echo
echo "============"
echo "  MU new"
echo "============"
$ANAPATH/mu/mu-emu_corr-newcut2 "$DSTPATH/$file" $EVTCOUNT | tee "$LOGPATH/mu_logfile.txt" | grep 0000
tail -n 10 "$LOGPATH/mu_logfile.txt"
corr=$(tail -n 1 "$LOGPATH/mu_logfile.txt")
echo "Create postscript from $corr"
$ANAPATH/mu/show_mu $corr

echo
echo "============"
echo "  MU Eff"
echo "============"
$ANAPATH/muEff/numberNonFoundLeptons2 "$DSTPATH/$file" $EVTCOUNT | tee "$LOGPATH/mueff_logfile.txt" | grep 0000
tail -n 10 "$LOGPATH/mueff_logfile.txt"

cd $HLDPATH
mv *.ps $POSTSCRIPTPATH
mv *CORR*root $RESULTPATH

cd $DSTPATH
mv *.ps $POSTSCRIPTPATH
mv *CORR*root $RESULTPATH

cd $POSTSCRIPTPATH
cat $number*.ps > out_$number.ps
ps2pdf out_$number.ps
rm out_$number.ps
mv out_$number.pdf $number.pdf
echo "starting acroread $number"
acroread $number.pdf &

cd $OLDPATH
echo "finished"

echo "------------------"
echo "If you want to make an entry in the logbook:"
echo "cp $POSTSCRIPTPATH/$number ~/web-docs/apr06/"
echo "chmod 644 ~/web-docs/apr06/$number"
echo "Then make an logbook entry with the following html code:"
echo "<br>"
echo "<a href=\"http://www-hades.gsi.de/~htrigger/apr06/$1\">$1</a>"
echo "<br>"
echo "------------------"
echo "To look at the ps files, use \"gv filename.ps\""
echo "To print the ps files (counting house, upstairs),"
echo "use \"lpr -h -P p120gps filename.ps\""
echo "------------------"
