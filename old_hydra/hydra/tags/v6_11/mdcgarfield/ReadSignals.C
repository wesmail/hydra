void ReadSignals()
{
    TString input="";
    Char_t path[300];
    Char_t inputfile[300];
    sprintf(path,"%s","/misc/kempter/scratchlocal1/jochen/garfield/signals/");
    TStopwatch timer;
    timer.Reset();
    timer.Start();
    HMdcGarSignalReader* reader=new HMdcGarSignalReader();
    reader->setNSignals(100);
    reader->setNBinSignal(1000);
    reader->setRangeSignal(1000);
    reader->setWriteHists(0,0,1,1,1,1); // sig,sigint,sum,dist,timedistance,charge
    //reader->setFileNameOut("/misc/kempter/scratchlocal1/jochen/garfield/signals/mdc4/2200/test.root");
    reader->setFileNameOut("/misc/kempter/batch/test.root");
    //reader->setFileNameIn(input);
    for(Int_t mdc=0;mdc<4;mdc++){
	for(Int_t angle=0;angle<18;angle++){
	    if(mdc==3)
	    {
		sprintf(inputfile,"%s%s%i%s%i%s%i%s",path,"mdc",mdc+1,"/2200/s",angle*5,"-mdc",mdc+1,"_all.txt");
                input=inputfile;
	    }
	    else
	    {
		sprintf(inputfile,"%s%s%i%s%i%s%i%s",path,"mdc",mdc+1,"/s",angle*5,"-mdc",mdc+1,"_all.txt");
	        input=inputfile;
	    }
            cout<<input.Data()<<endl;
	    reader->make(mdc,angle,input);
	}
    }

    timer.Stop();
    timer.Print();
    reader->closeOutput();
}
