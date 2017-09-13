//Draw PMT Traces

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include "TTimeStamp.h" 

#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <memory>

#include "TTimeStamp.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TH1F.h"
#include "TFrame.h"
#include "TGaxis.h"
#include "TPad.h"
#include "TLegend.h"
#include "TString.h"
#include "TH1D.h"
#include "TH1.h"
#include "TLatex.h"

#include "tools.h"
#include "color_text.h"

void drawmanytraces_PMT_4chs() {

  //read list of files ------------------------------------------------------//
  //string str_listname="./file_list.txt";
  //string str_listname="./BIG_PMT_Traces_file_list.txt";
  //string str_listname="./file_list_CR_demo.txt";
  //string str_listname="./file_list_up2date_20170419.txt";
  //string str_listname="./file_list_only_20170419.txt";
  //string str_listname="./file_list_all.txt";
  //string str_listname="./file_list_20170420.txt";
  //string str_listname="./file_list_20170421.txt";
  //string str_listname="./files_test.txt";
  //string str_listname="./files_flashlamp_20170425.txt";
  //string str_listname="./files_20170425_sparklike_candidates.txt";
  //string str_listname="./psd_timestamp_strangeqa2_for_display.txt";
  //string str_listname="./file_05022017.txt";
  //string str_listname="./files_spark_candidates_2017_0503_5pm.txt";
  //string str_listname="./files_2017_0503-0504.txt";
  //string str_listname="./PMT_Traces_GAr.txt";
  //string str_listname="./35ton_HV_test_PMT_Traces_GasTest_20170727.txt";
  //string str_listname="./check_strange_for_display.txt";
  //string str_listname="./active_ch3_for_display.txt";
  string str_listname="./gas_test_20170728.txt";
  //string str_listname="./gas_test_20170727.txt";

  string buffer_string;
  ifstream f_string(str_listname.c_str());
  vector<string> wf_fielname;
  while (f_string>>buffer_string) { wf_fielname.push_back(buffer_string); }
  f_string.close();

  //Output file name
  //TString outfig="./PMT_traces.pdf";
  //TString outfig="./PMT_BIG_traces.pdf";
  //TString outfig="./PMT_CR_traces.pdf";
  //TString outfig="./PMT_traces_up2date_20170419.pdf";
  //TString outfig="./PMT_traces_only_20170419.pdf";
  //TString outfig="./PMT_traces_all.pdf";
  //TString outfig="./PMT_traces_20170420.pdf";
  //TString outfig="./PMT_traces_20170421.pdf";
  //TString outfig="./PMT_traces_flashlamp_20170425.pdf";
  //TString outfig="./PMT_traces_20170425_sparklike_candidates.pdf";
  //TString outfig="./psd_timestamp_strangeqa2_for_display.pdf";
  //TString outfig="./file_05022017.pdf";
  //TString outfig="./pmt_2017_0503_5pm.pdf";
  //TString outfig="./files_2017_0503-0504.pdf";
  //TString outfig="./files_GAr.pdf";
  //TString outfig="./files_GasTest_20170727.pdf";
  //TString outfig="./check_strange_for_display.pdf";
  TString outfig="./gas_test_20170728.pdf";
  //TString outfig="./gas_test_20170727_largescale.pdf";
  //TString outfig="./PMT_traces_20170425_sparklike_candidates_zoom_in.pdf";
  //TString outfig="./PMT_traces_20170425_sparklike_candidates_zoom_in_rebin.pdf";
  //TString outfig="./PMT_traces_20170425_sparklike_candidates_rebin.pdf";

  //Setting of the traces ----------------------------------//
  //const int nbin=2500;
  //Double_t pulse_length=2500; //num of bins
  const int n_waves=4;
  int n_ch=*const_cast<int*>(&n_waves);
  int time_unit_convert=1000000; //10^6: in us ; 1: in s
  int n_rebin=20; //rebin number

  //settting for figures ----------------------------------------------//
  //gStyle->SetOptTitle(0); //no title box
  gStyle->SetOptStat(0); //no statistics box
  //gROOT->SetStyle("Plain"); //Set the overall design to "Plain" mode
  gStyle->SetFillColor(0);
  gStyle->SetCanvasColor(0);
  gStyle->SetPadColor(0);
  gStyle->SetFrameFillColor(0); //Set the frame color to white
  gStyle->SetTitleFillColor(0);
  gStyle->SetStatColor(0);
  gStyle->SetCanvasBorderMode(0); // -1=down, 0=no border, 1=up
  gStyle->SetFrameBorderSize(3);
  gStyle->SetTextSize(1.1);
  //gStyle->SetLabelSize(0.05,"xy");
  //gStyle->SetTitleSize(0.05,"xy");
  gStyle->SetLabelSize(0.06,"xy");
  gStyle->SetTitleSize(0.07,"xy");
  gStyle->SetTitleSize(0.1,"t"); 

  //gStyle->SetTitleSize(0.15);
  //Width
  gStyle->SetLineWidth(2.);
  gStyle->SetFrameLineWidth(2);
  //Offset
  gStyle->SetTitleOffset(.9,"x");
  gStyle->SetTitleOffset(0.6,"y");
  gStyle->SetTitleOffset(1.2,"t");
  //position of title
  //gStyle->SetTitleAlign(23);
  //gStyle->SetTitleX(0.5);

  //Margin
  float global_btmargin=0.15;
  //gStyle->SetPadTopMargin(0.2);
  //gStyle->SetPadTopMargin(0.15);
 //gStyle->SetPadTopMargin(0.12);
  gStyle->SetPadTopMargin(0.1);
  gStyle->SetPadBottomMargin(0.15);
  gStyle->SetPadLeftMargin(0.1);
  gStyle->SetPadRightMargin(0.12);
  //format
  gStyle->SetPaintTextFormat("5.2f");
  //gROOT->ForceStyle();

  //gStyle->SetPalette(51,0); //
  //gStyle->SetNumberContours(100);
  Float_t lt_size=0.09;
 
  //file loop --------------------------------------------------------------------------------------------//
  //int index[n_waves]={0,2502,5004,7506}; //indices for cropping data

  TString str_title;
  string str_temp;
  size_t k;
  size_t n_broken=0;
  size_t n_good=0;
  for (size_t ii=0; ii<wf_fielname.size();ii++) { //file loop
	//get file name
        TString inputfilename(wf_fielname[ii]);
        //cout<<"open PMT WF file ["<<ii<<"]:"<<wf_fielname[ii]<<endl;

        //get title of each file
	string str_name0=wf_fielname[ii].substr(wf_fielname[ii].find("_2017_")+1);
	string str_name1=str_name0.substr(0,str_name0.find("csv")-1);
	//string str_date=str_name1.substr(0,str_name1.find("-")-1);
	string str_date=str_name1.substr(0,str_name1.find("-"));
	string str_time=str_name1.substr(str_name1.find("-")+1);
	TString str_title_date(str_date);
	TString str_title_time(str_time);
	str_title_date.ReplaceAll("_","/");
	str_title_time.ReplaceAll("_",":");
        str_title=str_title_date+"-"+str_title_time;
        //str_title=Form("Event %d  ",ii+1)+str_title_date+"-"+str_title_time;
	//cout<<str_title_date<<"-"<<str_title_time<<endl;

        //read each data in the given list
    	std::ifstream File(inputfilename.Data());
    	Double_t buffer;
    	vector<Double_t> data;
    	vector<Double_t> zero;
    	while (File>>buffer) { data.push_back(buffer); }
    	File.close();

	//get number of bins per data
	//const_cast<const int&>(data[0]);

	//get indices to extract traces
        int index[n_waves];
	index[0]=1;
	for (size_t nn=1; nn<n_waves; nn++) { index[nn]=index[nn-1]+data[0]+2; }
	//cout<<index[0]<<","<<index[1]<<","<<index[2]<<","<<index[3]<<endl;
        //cout<<"pulse_length:"<<data[0]<<endl;
	//cout<<""<<endl;

	//check if the file is broken or not
	bool IsGoodData=true;
        if ((data[0]*n_ch)>data.size()||data[0]<=0) { //if the file is broken or not recorded in 4 channels
		IsGoodData=false;
		n_broken++;
	} //if the file is broken or not recorded in 4 channels
        if (IsGoodData==true) { cout<<"open PMT file ["<<ii<<"]:"<<wf_fielname[ii]<<"     go go go!!"<<endl; }
        else { cout<<red<<"Borken Toroid file: " <<  wf_fielname[ii]<<" :-( "<< reset << endl; }

        if (IsGoodData==true) { //is good data
		n_good++;
        	//get dt, t0 info from each data
        	Double_t dt[n_waves];
        	Double_t t0[n_waves]; 
        	Double_t tmax[n_waves];
		Double_t pulse_length=(Double_t)data[0]; //1st column is the number of bins of traces

        	//Traces loop
        	TH1D **hps=new TH1D*[n_waves];
    		TH2D **frame=new TH2D*[n_waves];

    		TLine **line_rms=new TLine*[n_waves];
    		TLine **line_rms2=new TLine*[n_waves];
    		TLine **line_rms3=new TLine*[n_waves];
    		TLine **line_rms4=new TLine*[n_waves];
    		TLine **line_tmax=new TLine*[n_waves];
    		TLine **line_tabthr_min=new TLine*[n_waves];
    		TLine **line_tabthr_max=new TLine*[n_waves];
		//Double_t frame_ymin[n_waves]={-.05,-.05,-.05,-.2};
		//Double_t frame_ymax[n_waves]={.05, .05, .05, .2};
		//Double_t frame_ymin[n_waves]={-.2,-.2,-.2,-.2};
		//Double_t frame_ymax[n_waves]={.2, .2, .2, .2};
		//Double_t frame_ymin[n_waves]={-.01,-.03,-.01,-.2};
		//Double_t frame_ymax[n_waves]={.04, .02, .03, .2};
		//Double_t frame_ymin[n_waves]={-.9,-.9,-.9,-.9};
		//Double_t frame_ymax[n_waves]={.3, .3, .3, .3};
		Double_t frame_ymin[n_waves]={-1,-1,-1,-1};
		Double_t frame_ymax[n_waves]={1, 1, 1, 1};
		//Double_t frame_ymin[n_waves]={-.2,-.2,-.2,-.2};
		//Double_t frame_ymax[n_waves]={.4, .4, .4, .4};
		//Double_t frame_ymin[n_waves]={-.2,-.2,-.2,-.2};
		//Double_t frame_ymax[n_waves]={.2, .2, .2, .2};
		Double_t frame_xmin[n_waves];
		Double_t frame_xmax[n_waves];

		Double_t amax[n_waves];
		Double_t tamax[n_waves];
		Double_t tabthr_min[n_waves];
		Double_t tabthr_max[n_waves];
		Double_t thr[n_waves];
		Double_t q[n_waves];

		Int_t Trigger_Scalar[n_waves]={100,10,1,0};
		Int_t Trigger=0;
        	for (size_t i=0; i<n_waves; i++) { //waveform loop
    			for (k=0; k<data.size() ; k++) { //1st data loop to get t0, dt
				if (k==index[i]) t0[i]=data[k]; 
                		if (k==(index[i]+1)) dt[i]=data[k];
			} //1st data loop to get t0, dt
	        	//cout<<"t0["<<i<<"]:"<<t0[i]<<" ; dt["<<i<<"]:"<<dt[i]<<endl; //print what u got
	        	cout<<"length:"<<data[0]<<" ; t0["<<i<<"]:"<<t0[i]<<" ; dt["<<i<<"]:"<<dt[i]<<endl; //print what u got


			//2nd loop to extract waveforms from data
			tmax[i]=t0[i]+pulse_length*dt[i]; //cout<<"tmax:"<<tmax[i]<<endl;
       			hps[i]=new TH1D(Form("hps_%d",i),Form("ch%d",i+1),data[0],t0[i]*time_unit_convert,tmax[i]*time_unit_convert);
        		int st,ed;
			st=index[i]+2;
        		ed=st+data[0];
        		//cout<<""<<st<<" - "<<ed<<endl;
        		for (k=st; k<ed; k++) { hps[i]->SetBinContent(k-st+1,data[k]); } //invert the traces

			//set waveform style
			if (i==0) { hps[i]->SetLineColor(1); }
			if (i==1) { hps[i]->SetLineColor(2); }
			if (i==2) { hps[i]->SetLineColor(4); }
			if (i==3) { hps[i]->SetLineColor(95); }
        		//hps[i]->SetLineWidth(2);
        		hps[i]->SetLineWidth(1);

			//calculate RMS
			Int_t st_bin_rms=10;
			Int_t nbin_rms=50;
			Double_t rms_line=RMS(st_bin_rms,st_bin_rms+nbin_rms,hps[i]);
			Double_t mean_line=AVBASE(st_bin_rms,st_bin_rms+nbin_rms,hps[i]);

			Double_t xmin_rms=t0[0]*time_unit_convert+(Double_t)(st_bin_rms-1)*dt[i]*time_unit_convert;
			Double_t xmax_rms=xmin_rms+time_unit_convert*dt[i]*(Double_t)nbin_rms;
			//cout<<"mean["<<i<<"]:"<<mean_line<<":"<<"rms["<<i<<"]:"<<rms_line<<": plot range of x["<<i<<"]:"<<xmin_rms<<" - "<<xmax_rms<<endl;

			//Get maximaum amplitude
			amax[i]=AMAX(1,hps[i]->GetNbinsX(),hps[i]); //get max. amp. from 1st to the last bin 
			tamax[i]=TMAX(1,hps[i]->GetNbinsX(),hps[i]); //get max. amp. from 1st to the last bin


			//Get time above threshold
			thr[i]=mean_line+5.*rms_line;
			tabthr_min[i]=TMIN_ABTH(1,hps[i]->GetNbinsX(),hps[i],thr[i]);
			tabthr_max[i]=TMAX_ABTH(1,hps[i]->GetNbinsX(),hps[i],thr[i]);
			//calculate charge
			Int_t tbin_max=TMAXBIN(1,hps[i]->GetNbinsX(),hps[i]);
			cout<<"tbin_max:"<<tbin_max<<endl;
			//q[i]=Q(1,hps[i]->GetNbinsX(),hps[i]);
			//q[i]=Q(tbin_max-63*2,tbin_max+63*2,hps[i]); //before-after ~0.2us

			q[i]=Q(tbin_max-63,tbin_max+63,hps[i]); //before-after ~0.1us
			//q[i]=Q(TMIN_BIN_ABTH(1,hps[i]->GetNbinsX(),hps[i],thr[i]),TMAX_BIN_ABTH(1,hps[i]->GetNbinsX(),hps[i],thr[i]),hps[i]);
			//q[i]=TB(200,500,hps[i]);
		//	Int_t ttt_bin_abth=TMAX_BIN_ABTH(1,hps[i]->GetNbinsX(),hps[i],thr[i]);
                        //cout<<"tbin_ab_th:"<<ttt_bin_abth<<endl;
			//q[i]=Q(-10+TMAX_BIN_ABTH(1,hps[i]->GetNbinsX(),hps[i],thr[i]),10+TMAX_BIN_ABTH(1,hps[i]->GetNbinsX(),hps[i],thr[i]),hps[i]);

			printf("q[%d]:%.20f\n",i,q[i]);
			//Determine it is trigger or not
			//trigger: 100*[PMT1 trigger]+10*[PMT 2 trigger]+1*[PMT 3 trigger]
			if (amax[i]>thr[i]) Trigger+=Trigger_Scalar[i]; 

			line_rms[i]=new TLine(xmin_rms,mean_line+rms_line,xmax_rms,mean_line+rms_line);
			line_rms2[i]=new TLine(xmax_rms,mean_line+rms_line,tmax[0]*time_unit_convert,mean_line+rms_line);
			line_rms3[i]=new TLine(xmin_rms,mean_line+3.*rms_line,tmax[0]*time_unit_convert,mean_line+3.*rms_line);
			line_rms4[i]=new TLine(xmin_rms,mean_line+5.*rms_line,tmax[0]*time_unit_convert,mean_line+5.*rms_line);

			line_tmax[i]=new TLine(tamax[i],frame_ymin[i],tamax[i],amax[i]);
			line_tabthr_min[i]=new TLine(tabthr_min[i],frame_ymin[i],tabthr_min[i],frame_ymax[i]);
			line_tabthr_max[i]=new TLine(tabthr_max[i],frame_ymin[i],tabthr_max[i],frame_ymax[i]);

			line_rms[i]->SetLineStyle(1);
			line_rms2[i]->SetLineStyle(7);
			line_rms3[i]->SetLineStyle(2);
			line_rms4[i]->SetLineStyle(2);
			line_tabthr_min[i]->SetLineStyle(2);
			line_tabthr_max[i]->SetLineStyle(2);

			line_tmax[i]->SetLineStyle(1);

			line_rms[i]->SetLineColor(40);
			line_rms2[i]->SetLineColor(40);
			line_rms3[i]->SetLineColor(38);
			line_rms4[i]->SetLineColor(6);
			line_tabthr_min[i]->SetLineColor(16);
			line_tabthr_max[i]->SetLineColor(7);

			line_tmax[i]->SetLineColor(3);

			line_rms[i]->SetLineWidth(1);
			line_rms2[i]->SetLineWidth(1);
			line_rms3[i]->SetLineWidth(1);
			line_rms4[i]->SetLineWidth(1);

			line_tmax[i]->SetLineWidth(1);
			line_tabthr_min[i]->SetLineWidth(1);
			line_tabthr_max[i]->SetLineWidth(1);

			//set frame for each trace
			frame_xmin[i]=t0[0]*time_unit_convert;
			frame_xmax[i]=tmax[0]*time_unit_convert;
    			if (i==0) { frame[i]=new TH2D(Form("frame%d",i),str_title.Data(),data[0],frame_xmin[i],frame_xmax[i],100,frame_ymin[i],frame_ymax[i]); }
    			else { frame[i]=new TH2D(Form("frame%d",i),Form(""),data[0],frame_xmin[i],frame_xmax[i],100,frame_ymin[i],frame_ymax[i]); }

    			frame[i]->GetXaxis()->SetTitle("Time [#mus]");
    			frame[i]->GetYaxis()->SetTitle("Amplitude [V]");
		} //waveform loop

        	//draw waveforms
    		TCanvas *c_ps=new TCanvas("c_ps"," ",1000, 1200); //draw individually
    		gStyle->SetOptStat(0);

        	//draw all traces separately    
    		c_ps->Divide(1,4);
    		TLegend **leg=new TLegend*[n_waves];
    		for (size_t j=0; j<n_waves; j++) {
    			c_ps->cd(j+1);
			if (j>0) { frame[j]->SetTitle(""); }
    			frame[j]->Draw();
			//hps[j]->Rebin(n_rebin);
			//hps[j]->Scale(1./(Double_t)n_rebin);      
        		hps[j]->Draw("same");

			//draw rms line
			//line_rms[j]->Draw("same");
			//line_rms2[j]->Draw("same");
			//line_rms3[j]->Draw("same");
			//line_rms4[j]->Draw("same");

			//draw diagonstics parameters
			//line_tmax[j]->Draw("same");
			//line_tabthr_min[j]->Draw("same");
			//line_tabthr_max[j]->Draw("same");

			//draw tmax, amax
			if (j==0) {
					Double_t xmin_trig_text=frame_xmin[j]+0.05*(-frame_xmin[j]+frame_xmax[j]);	
					Double_t ymin_trig_text=frame_ymin[j]+0.9*(-frame_ymin[j]+frame_ymax[j]);	
   					TLatex trig_text;
   					trig_text.SetTextAlign(12);  //centered
   					trig_text.SetTextFont(72);   				
					trig_text.SetTextSize(.05);
   					//trig_text.DrawLatex(xmin_trig_text,ymin_trig_text,Form("Trigger condition: %d",Trigger));
			  	  }

			//draw diagonstics parameters
			Double_t xmin_at_text=frame_xmin[j]+0.6*(-frame_xmin[j]+frame_xmax[j]);
			Double_t ymin_at_text=frame_ymin[j]+0.8*(-frame_ymin[j]+frame_ymax[j]);
			TLatex at_text; 
			at_text.SetTextAlign(12);
			at_text.SetTextFont(72);
			at_text.SetTextSize(.05);
 			//at_text.DrawLatex(xmin_at_text,ymin_at_text,Form("Amax:%.2f [V]; Tmax:%.2f [#mus] q:%.2f",amax[j],tamax[j],q[j]));

			Double_t xmin_tot_text=frame_xmin[j]+0.6*(-frame_xmin[j]+frame_xmax[j]);		
	   		Double_t ymin_tot_text=frame_ymin[j]+0.7*(-frame_ymin[j]+frame_ymax[j]);
			TLatex tot_text; 
			tot_text.SetTextAlign(12);
			tot_text.SetTextFont(72);
			tot_text.SetTextSize(.05);
 			//tot_text.DrawLatex(xmin_tot_text,ymin_tot_text,Form("ToT:%.2f [#mus]",tabthr_max[j]-tabthr_min[j]));

			//draw ch label	
    			leg[j]=new TLegend(0.75,0.8,0.88,0.9);
			//leg[j]->SetName(Form("leg_%d",j));
    			if (j<3) leg[j]->AddEntry((TObject*)hps[j],Form("PMT Ch %d",j+1),"l");
    			if (j==3) leg[j]->AddEntry((TObject*)hps[j],Form("Bottom Ground Plane"),"l");
    			//if (j==3) leg[j]->AddEntry((TObject*)hps[j],Form("Top West G.P."),"l");
			leg[j]->Draw();
    		}
     		c_ps->Update();

     		if (n_good==1) { c_ps->Print(Form("%s(",outfig.Data())); }
     		if (n_good>1&&n_good<(wf_fielname.size()-n_broken)) { c_ps->Print(Form("%s", outfig.Data())); }
     		if (n_good==(wf_fielname.size()-n_broken)) {  c_ps->Print(Form("%s)",outfig.Data())); }

		//release memory in the end
		for (size_t i=0; i<n_waves; i++) { //release memory
			delete hps[i];
			delete frame[i];

			delete line_rms[i];
			delete line_rms2[i];
			delete line_rms3[i];
			delete line_rms4[i];

			delete line_tmax[i];
			delete line_tabthr_min[i];		
			delete line_tabthr_max[i];		

			delete leg[i];
		} //release memory
		delete c_ps;
  	} //is good data
  } //file loop

  cout<<"\n"<<endl;
  cout<<"=========================================="<<endl;
  cout<<green<<"There are "<<wf_fielname.size()<<" files."<<reset<<endl;
  cout<<red<<"But "<<n_broken<<" files are broken."<<reset<<endl;
  cout<<"=========================================="<<endl;


}
