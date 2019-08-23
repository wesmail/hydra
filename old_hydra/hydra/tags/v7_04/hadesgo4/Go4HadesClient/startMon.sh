#------------------------CONFIGURATION AREA-----------------------
ConfigFile=/misc/mdc/MonGo4/analysisParams.txt
monlogin=/misc/mdc/MonGo4/hadesgo4login.sh
myhost=lxi008.gsi.de
#------------------------END OF CONFIGURATION---------------------

echo "##################### CONFIGURATION ############################################"
echo "Config-File    : $ConfigFile"  
echo "Login script   : $monlogin"  
echo "Client runs on : $myhost"  
echo "################################################################################"


if [ -z $HADES_GO4_MON_ACTIVE ]
then
. $monlogin
fi

netstat -a --inet --program | grep LISTEN | grep MainGo4| awk '{ print $4" "$7 }'>/tmp/list1.txt

go4& >/dev/null 2>&1
sleep 10

netstat -a --inet --program | grep LISTEN | grep MainGo4| awk '{ print $4" "$7 }'>/tmp/list2.txt


perl -pi -e 's/ /:/g' /tmp/list1.txt /tmp/list2.txt
myline=""
for item in `cat /tmp/list2.txt | sed -e 's/\*//g'`
do 
   hhh=`grep $item /tmp/list1.txt`
   if [ "$hhh" = "" ] 
   then myline=${item}
fi
done

myline=$(echo $myline | sed -e 's/:/ /g') 
pid=`echo $myline | awk '{ print $2}'`
pid=${pid%/MainGo4GUI}
port=`echo $myline | awk '{ print $1}'`

rm -f /tmp/list1.txt
rm -f /tmp/list2.txt

echo "##################### MainGo4GUI detected : pid $pid on port $port" 
echo -e "\n\n==> Executing: "
echo -n "    "
echo ${HADDIR_HADESGO4}/Go4HadesClient/MainGo4HadesClient MEGAMON `hostname` $port $ConfigFile
echo -e "\n\n\n"

localhost=`hostname`
{ 
   xterm -rightbar -sb -sl 10000 -T "HADES MONITOR CLIENT on $myhost" \
         -e ssh $myhost ". $monlogin; \${HADDIR_HADESGO4}/Go4HadesClient/MainGo4HadesClient MEGAMON $localhost $port $ConfigFile; killall MainGo4HadesClient; exit 0";
   killall MainGo4GUI
} &   

sleep 10
echo "##################### PRESS START TO RUN THE MONITOR ###########################" 


