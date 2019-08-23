#!/bin/bash
#-------------------------------------------------------
# Do everything automatically for apr06 beamtime
# Starting from DST file, rich, tof, mu, muEff
# write logfile, write histogramms to postscript files
# convert postscripts to pdf and show it.
#                            4 May 2006, Bjoern Spruck
#--------------------------------------------------------

# Set the Hydra env. Just in case...

. camilla/startEnv/hades.sh

# Where you copied official online-DSTs by hand...

DSTPATH=/scratch.local2/dst

# here we write logfiles
LOGPATH=/scratch.local/log
RESULTPATH=/scratch.local/corr
POSTSCRIPTPATH=/scratch.local/corr_ps

# in this path the analysis macros are

ANAPATH=/u/htrigger/camilla

# only used for analysis, not for dst
EVTCOUNT=100000

rm "$LOGPATH/tof_logfile.txt"
rm "$LOGPATH/rich_logfile.txt"
rm "$LOGPATH/mu_logfile.txt"
rm "$LOGPATH/mueff_logfile.txt"

OLDPATH=$(pwd)

cd $DSTPATH
file=$(ls -rt| tail -n 1)
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

number=${file:0:13}

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

cd $DSTPATH
mv *.ps $POSTSCRIPTPATH
mv *CORR*root $RESULTPATH

cd $POSTSCRIPTPATH
cat $number*.ps > out_$number.ps
ps2pdf out_$number.ps
rm out_$number.ps
acroread out_$number.pdf &

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
echo "To look at the ps files, use 'gv filename.ps'"
echo "To print the ps files (counting house, upstairs),"
echo "use 'lpr -h -P p120gps filename.ps'"
echo "------------------"

