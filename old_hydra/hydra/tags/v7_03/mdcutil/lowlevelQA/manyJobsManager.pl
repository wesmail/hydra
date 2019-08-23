#!/usr/bin/perl -w
print"\n";
print"******************************************************************\n";
print"*  Perl many batch jobs manager    (c)A.Sadovski                 *\n";
print"******************************************************************\n";
print"\n";

$batchName   =        "batch";  #"research"; #"batch";

$origMacrosDir      = "/misc/sudol/lowlevelQA";
$rootMacroName      = "analysisLowLevelQA.C";
$diskORtape         = "DISK"; #or# "TAPE";
$hldFileDir         = "/d/hades02/data/lowlevelQA/340";
$outputDir          = "/misc/sudol";
$eventsNum          = "0";
$firstEvent         = "0";

$protocolOutDir     = "/misc/sudol/lowlevelQA/prot";


#
# Already finished files
#
@fready = qw{
be02340000721 
be02340043844  
be02340235521  
};

#
# All files to perform
#
@fname = qw {
be02341043230
be02340002532  
be02340044042  
be02341000407  
be02341044446
be02340003732  
be02340045912  
be02341001722  
be02341045124
be02340004945  
be02340051408  
be02341002527  
be02341050440
be02340011106  
be02340052752  
be02341003354  
be02341051659
be02340011705 
be02340054037 
be02341004225 
be02341053740
be02340012349 
be02340054627 
be02341005521  
be02341054408
be02340012907  
be02340212501
be02341010330  
be02341055402
be02340013944  
be02340213455  
be02341011335  
be02341060111
be02340015230  
be02340214507  
be02341013743  
be02341123254
be02340020453  
be02340214812  
be02341014558 
be02342013903
be02340021708  
be02340215338  
be02341015103  
be02342022339
be02340022944 
be02340220806 
be02341020310 
be02342025340
be02340024215  
be02340222220  
be02341021510 
be02342033915
be02340025253  
be02340222803  
be02341022444  
be02342051117
be02340030529  
be02340223403  
be02341024225  
be02342195246
be02340031210  
be02340224207  
be02341024849  
be02342205827
be02340032713  
be02340225444  
be02341030149  
be02342225713
be02340033431  
be02340230649  
be02341031421  
be02340040001  
be02340232232  
be02341033025
be02340041316  
be02340233128  
be02341034643
be02340042553  
be02340234234  
be02341041941 
};

#
# Launching not finished files
#
foreach $filename (@fname){
   print"\nProcessing file: $filename\n";

   #loop over ready files
   $processedFlag = 0;
   foreach $r_filename (@fready){
     #print"?? $r_filename\n";
     if($filename =~ $r_filename){
       print"this file already processed, we skip it now...\n";
       $processedFlag = 1;
     }
   }

   if($processedFlag =~ 0){  # if_not_processed_yet=>process_it_now
      print"Sending file $filename to the batch farm\n";
      $protocol_out="$protocolOutDir"."/".$filename."_prot.out";
      system("echo bsub -q $batchName -o $protocol_out singleJob2Batch.sh $origMacrosDir $rootMacroName $filename $outputDir $eventsNum $firstEvent $diskORtape $hldFileDir");
      system("     bsub -q $batchName -o $protocol_out singleJob2Batch.sh $origMacrosDir $rootMacroName $filename $outputDir $eventsNum $firstEvent $diskORtape $hldFileDir");
   }
}

print("******************************************************************\n");
print("***             DONE:  manyJobsManager.pl                      ***\n");
print("******************************************************************\n");
