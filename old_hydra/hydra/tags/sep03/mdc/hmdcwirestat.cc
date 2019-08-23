////////////////////////////////////////////////////////////////////////////
//*-- AUTHOR : J. Markert
////////////////////////////////////////////////////////////////////////////
// HMdcWireStat
//
//  Container class for lookup parameters of the status of the
//  wire (working or dead + noise + efficiency).
//  The parameters are stored in TH2F histograms which can be plotted.
//  This container is needed by HMdcDigitizer and HMdcTrackFinder
//
//  THe Status value is coded in the following way
//
//  status of the wire: 1=working,
//                     -1=not connected,
//                     -3=no correct offset,
//                     -7=dead,
//                    -10=init
//
//
////////////////////////////////////////////////////////////////////////////
#include "hmdcwirestat.h"
#include "hpario.h"
#include "hdetpario.h"
#include "hmessagemgr.h"
#include "hparamlist.h"
#include "hmessagemgr.h"
#include "hmdcdetector.h"
#include "hspectrometer.h"
#include "TStyle.h"
#include "TH1.h"
#include <stdlib.h>

ClassImp(HMdcWireStat)

HMdcWireStat::HMdcWireStat(const char* name,const char* title,
                           const char* context)
    : HParCond(name,title,context)
{
    //
    strcpy(detName,"Mdc");
    Char_t histname[300];
    for(Int_t s=0;s<6;s++){
        for(Int_t m=0;m<4;m++){
            sprintf(histname,"status_s%i_%i",s,m);
            statusmap[s][m]=new TH2F(histname,histname,220,0,220,6,0,6);
            sprintf(histname,"efficiency_s%i_%i",s,m);
            efficiency[s][m]=new TH2F(histname,histname,220,0,220,6,0,6);
            sprintf(histname,"mb_s%i_%i",s,m);
            mb[s][m]=new TH2F(histname,histname,220,0,220,6,0,6);
            sprintf(histname,"tdc_s%i_%i",s,m);
            tdc[s][m]=new TH2F(histname,histname,220,0,220,6,0,6);
        }
    }
    clear();
}
HMdcWireStat::~HMdcWireStat()
{
    // destructor
    for(Int_t s=0;s<6;s++){
        for(Int_t m=0;m<4;m++){
            delete statusmap[s][m];
            delete efficiency[s][m];
            delete mb[s][m];
            delete tdc[s][m];
        }
    }
}
void HMdcWireStat::clear()
{
    for(Int_t s=0;s<6;s++){
	for(Int_t m=0;m<4;m++){
	    for(Int_t binx=0;binx<220;binx++){
		for(Int_t biny=0;biny<6;biny++){

		    statusmap [s][m]->SetBinContent(binx+1,biny+1,-10);
		    efficiency[s][m]->SetBinContent(binx+1,biny+1,-10);
		    mb [s][m]       ->SetBinContent(binx+1,biny+1,-10);
		    tdc[s][m]       ->SetBinContent(binx+1,biny+1,-10);
		}
	    }
	}
    }

    status=kFALSE;
    resetInputVersions();
    changed=kFALSE;
}
void HMdcWireStat::printStats()
{
    Int_t working;      //  1
    Int_t notconnected; // -1
    Int_t nooffset;     // -3
    Int_t dead;         // -7

    Int_t workingsum=0;      //  1
    Int_t notconnectedsum=0; // -1
    Int_t nooffsetsum=0;     // -3
    Int_t deadsum=0;         // -7
    Int_t stat;
    for(Int_t s=0;s<6;s++){
	for(Int_t m=0;m<4;m++){
	    working=0;
	    notconnected=0;
	    nooffset=0;
	    dead=0;
	    for(Int_t biny=0;biny<6;biny++){
		for(Int_t binx=0;binx<220;binx++){
	 
		    stat=(Int_t)statusmap [s][m]->GetBinContent(binx+1,biny+1,-10);
                    if(stat== 1) working++;
                    if(stat==-1) notconnected++;
                    if(stat==-3) nooffset++;
                    if(stat==-7) dead++;
		}
	    }
	    workingsum     =workingsum+working;
	    notconnectedsum=notconnectedsum+notconnected;
	    nooffsetsum    =nooffsetsum+nooffset;
            deadsum        =deadsum+dead;
	    printf("s %i m %i \n" ,s,m);
	    printf("          working: %i \n",working);
	    printf("    not connected: %i \n",notconnected);
	    printf("   no calibration: %i \n",nooffset);
            printf("             dead: %i \n",dead);
	}
    }

    printf("SUM\n");
    printf("          working: %i \n",workingsum);
    printf("    not connected: %i \n",notconnectedsum);
    printf("   no calibration: %i \n",nooffsetsum);
    printf("             dead: %i \n",deadsum);

}
void HMdcWireStat::printParam(void)
{
    // prints the parameters of HMdcWireStat to the screen.
    SEPERATOR_msg("#",60);
    INFO_msg(10,DET_MDC,"HMdcWireStat:");
    INFO_msg(10,DET_MDC,"Status:");
    for(Int_t s=0;s<6;s++){
        for(Int_t m=0;m<4;m++){
            for(Int_t l=0;l<6;l++){
                for(Int_t c=0;c<220;c++){
                    gHades->getMsg()->info(10,DET_MDC,GetName(),
                                           "s %i m %i l %i c %3i mb %2i tdc %2i status %i efficiency %5.4f",
                                           s,m,l,c,
                                           (Int_t)mb[s][m]        ->GetBinContent(c+1,l+1),
                                           (Int_t)tdc[s][m]       ->GetBinContent(c+1,l+1),
                                           (Int_t)statusmap[s][m] ->GetBinContent(c+1,l+1),
                                           efficiency[s][m]       ->GetBinContent(c+1,l+1));
                }
            }
        }
    }
    SEPERATOR_msg("#",60);
}
Bool_t HMdcWireStat::init(HParIo* inp,Int_t* set)
{
    // intitializes the container from an input
    HDetParIo* input=inp->getDetParIo("HMdcParIo");
    if (input) return (input->init(this,set));
    return kFALSE;
}
Int_t HMdcWireStat::write(HParIo* output)
{
    // writes the container to an output
    HDetParIo* out=output->getDetParIo("HMdcParIo");
    if (out) return out->write(this);
    return -1;
}
void HMdcWireStat::putParams(HParamList* l)
{
    // Puts all params of HMdcWireStat to the parameter list of
    // HParamList (which ist used by the io);
    if (!l) return;

}
Bool_t HMdcWireStat::getParams(HParamList* l)
{
    if (!l) return kFALSE;

    return kTRUE;
}
TCanvas* HMdcWireStat::plotStatusMbVsTdc()
{
    gStyle->SetOptStat(0);
    Char_t name[400];
    sprintf(name,"%s","Mb_vs_Tdc");
    TCanvas* result=new TCanvas(name,name,1000,800);
    result->Divide(6,4);

    TH2F* hmb_vs_tdc[6][4];
    Int_t mbo,t,st;
    for(Int_t sec=0;sec<6;sec++){
        for(Int_t mod=0;mod<4;mod++){
            sprintf(name,"%s%i%s%i","hStatusMb_vs_Tdc_sec",sec,"_mod",mod);
            hmb_vs_tdc[sec][mod]=new TH2F(name,name,96,0,96,16,0,16);
            hmb_vs_tdc[sec][mod]->SetXTitle("tdc channel");
            hmb_vs_tdc[sec][mod]->SetYTitle("Mbo");
	    for(Int_t bx=0;bx<96;bx++){
		for(Int_t by=0;by<16;by++){
		    hmb_vs_tdc[sec][mod]->SetBinContent(bx+1,by+1,-10);
		}
	    }
            for(Int_t l=0;l<6;l++){
                Int_t ncell=statusmap[sec][mod]->GetNbinsX();
                for(Int_t c=0;c<ncell;c++){
                    mbo=(Int_t)mb[sec][mod]       ->GetBinContent(c+1,l+1);
                    t  =(Int_t)tdc[sec][mod]      ->GetBinContent(c+1,l+1);
                    st =(Int_t)statusmap[sec][mod]->GetBinContent(c+1,l+1);
                    hmb_vs_tdc[sec][mod]->SetBinContent(t+1,mbo+1,st);
                }
            }
            hmb_vs_tdc[sec][mod]->SetMinimum(-4);
            result->cd(mod*6+sec+1);
            hmb_vs_tdc[sec][mod]->DrawCopy("Colz");
        }
    }
    return result;
}
TCanvas* HMdcWireStat::plotEfficiencyMbVsTdc()
{
    gStyle->SetOptStat(0);
    Char_t name[400];
    sprintf(name,"%s","Mb_vs_Tdc");
    TCanvas* result=new TCanvas(name,name,1000,800);
    result->Divide(6,4);

    TH2F* hmb_vs_tdc[6][4];
    Int_t mbo,t;
    Float_t eff;
    for(Int_t sec=0;sec<6;sec++){
        for(Int_t mod=0;mod<4;mod++){
            sprintf(name,"%s%i%s%i","hEfficiencyMb_vs_Tdc_sec",sec,"_mod",mod);
            hmb_vs_tdc[sec][mod]=new TH2F(name,name,96,0,96,16,0,16);
            hmb_vs_tdc[sec][mod]->SetXTitle("tdc channel");
            hmb_vs_tdc[sec][mod]->SetYTitle("Mbo");

	    for(Int_t bx=0;bx<96;bx++){
		for(Int_t by=0;by<16;by++){
		    hmb_vs_tdc[sec][mod]->SetBinContent(bx+1,by+1,-10);
		}
	    }

	    for(Int_t l=0;l<6;l++){
                Int_t ncell=statusmap[sec][mod]->GetNbinsX();
                for(Int_t c=0;c<ncell;c++){
                    mbo=(Int_t)mb[sec][mod]          ->GetBinContent(c+1,l+1);
                    t  =(Int_t)tdc[sec][mod]         ->GetBinContent(c+1,l+1);
                    eff=(Float_t)efficiency[sec][mod]->GetBinContent(c+1,l+1);
                    hmb_vs_tdc[sec][mod]->SetBinContent(t+1,mbo+1,eff);
                }
            }
            result->cd(mod*6+sec+1);
            hmb_vs_tdc[sec][mod]->DrawCopy("Colz");
        }
    }
    return result;
}
TCanvas* HMdcWireStat::plotStatus(Int_t sec,Int_t mod)
{
    gStyle->SetOptStat(0);
    Char_t name[400];
    sprintf(name,"status_s%i_m%i",sec,mod);
    TCanvas* result=new TCanvas(name,name,1000,800);
    result->Divide(1,6);

    TH1F* hmdcstatus[6];
    Int_t mbo,t,st;
    for(Int_t l=0;l<6;l++){
        sprintf(name,"%s%i%s%i%s%i","hmdcstatus_sec",sec,"_mod",mod,"_layer",l);
        hmdcstatus[l]=new TH1F(name,name,220,0,220);
        hmdcstatus[l]->SetXTitle("wire number");
        hmdcstatus[l]->SetYTitle("status");
        hmdcstatus[l]->SetFillColor(8);
        Int_t ncell=statusmap[sec][mod]->GetNbinsX();
        for(Int_t c=0;c<ncell;c++){
            mbo=(Int_t)mb[sec][mod]       ->GetBinContent(c+1,l+1);
            t  =(Int_t)tdc[sec][mod]      ->GetBinContent(c+1,l+1);
            st =(Int_t)statusmap[sec][mod]->GetBinContent(c+1,l+1);
	    hmdcstatus[l]->SetBinContent(c+1,st);
        }
        result->cd(l+1);
        hmdcstatus[l]->DrawCopy();
    }
    return result;
}
TCanvas* HMdcWireStat::plotEfficiency(Int_t sec,Int_t mod)
{
    gStyle->SetOptStat(0);
    Char_t name[400];
    sprintf(name,"efficiency_s%i_m%i",sec,mod);
    TCanvas* result=new TCanvas(name,name,1000,800);
    result->Divide(1,6);

    TH1F* hmdcefficiency[6];
    Int_t mbo,t;
    Float_t eff;
    for(Int_t l=0;l<6;l++){
        sprintf(name,"%s%i%s%i%s%i","hmdcefficiency_sec",sec,"_mod",mod,"_layer",l);
        hmdcefficiency[l]=new TH1F(name,name,220,0,220);
        hmdcefficiency[l]->SetXTitle("wire number");
        hmdcefficiency[l]->SetYTitle("efficiency");
        Int_t ncell=efficiency[sec][mod]->GetNbinsX();
        for(Int_t c=0;c<ncell;c++){
            mbo=(Int_t)mb[sec][mod]          ->GetBinContent(c+1,l+1);
            t  =(Int_t)tdc[sec][mod]         ->GetBinContent(c+1,l+1);
            eff=(Float_t)efficiency[sec][mod]->GetBinContent(c+1,l+1);
            hmdcefficiency[l]->SetBinContent(c+1,eff);
        }
        result->cd(l+1);
        hmdcefficiency[l]->DrawCopy();
    }
    return result;
}