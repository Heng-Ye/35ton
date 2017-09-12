//**** Make Tree for PMT Traces ******//
//** H. Y. Liao, created on 04/24/2017 
//************************************//
//Bugs fix for rms and baseline, 05/03/2017
//add extra charge calculation for the compensation of short traces, 05/06/2017
//Add time zero(t0) and reference time w.r.t. the time of the 1st evt(reftime), 2017/07/27 H.Y.
//Add fraction of sec to the time format, 2017/08/24 H.Y.


#include <algorithm>
#include <cstdio>
#include <string.h>

#include "tools.h"
#include "color_text.h"

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
#include "TString.h"
#include "TH1D.h"
#include "TH1.h"

#include "TTree.h"
#include "TFile.h"
#include "TNtuple.h"
#include "TH1.h"
#include "TTimeStamp.h"
#include <TParameter.h>


void makeTree_PMT() {
  //Output ROOT file name
  //TString str_out=Form("PmtFile.root");
  //TString str_out=Form("PmtFile_GAr.root");
  //TString str_out=Form("PmtFile_20170728_Gas.root");
  //TString str_out=Form("PmtFile_GasTest_20170802.root");
  TString str_out=Form("PmtFile_GasTest_20170824.root");
	
  //Input txt file list
  //string str_listname="./file_list_20170421.txt";
  //string str_listname="./files_all.txt";
  //string str_listname="./files_GAr.txt";
  //string str_listname="./35ton_HV_test_PMT_Traces_GasTest_20170728.txt";
  //string str_listname="./PMTfile_GasTest_20170802.txt";
  string str_listname="./PMTfile_GasTest_20170824.txt";

  //read input txt files
  string buffer_string;
  ifstream f_string(str_listname.c_str());
  vector<string> wf_fielname;
  while (f_string>>buffer_string) { wf_fielname.push_back(buffer_string); }
  f_string.close();

  //Constant values of the PMT traces --------------------------------------------------//
  const int n_waves=4;
  int n_ch=*const_cast<int*>(&n_waves);
  int time_unit_convert=1000000; //10^6 means in a unit of us ; 1 means in a unit of s

  //Output Tree Parameters 
  TTimeStamp time;
  string evt_title;
  Double_t reftime;

  Int_t coinc;
  Int_t id;

  vector<Double_t> *amaxPtr=new vector<Double_t>(n_waves); //maximum amplitude
  vector<Double_t> &amax=*amaxPtr;
  vector<Double_t> *tmaxPtr=new vector<Double_t>(n_waves); //timing of maximum amplitude
  vector<Double_t> &tmax=*tmaxPtr;

  vector<Double_t> *aminPtr=new vector<Double_t>(n_waves); //minimum amplitude
  vector<Double_t> &amin=*aminPtr;
  vector<Double_t> *tminPtr=new vector<Double_t>(n_waves); //timing of min. amplitude
  vector<Double_t> &tmin=*tminPtr;

  vector<Double_t> *qPtr=new vector<Double_t>(n_waves); //charge
  vector<Double_t> &q=*qPtr;

  vector<Double_t> *q0Ptr=new vector<Double_t>(n_waves); //charge of in the beginning binnings
  vector<Double_t> &q0=*q0Ptr;

  vector<Double_t> *pqPtr=new vector<Double_t>(n_waves); //partial charge
  vector<Double_t> &pq=*pqPtr;

  vector<Double_t> *lengthPtr=new vector<Double_t>(n_waves); //pulse length
  vector<Double_t> &length=*lengthPtr;

  vector<Double_t> *bPtr=new vector<Double_t>(n_waves); //av. of the baseline
  vector<Double_t> &b=*bPtr;
  vector<Double_t> *thPtr=new vector<Double_t>(n_waves); //threshold
  vector<Double_t> &th=*thPtr;
  vector<Double_t> *rPtr=new vector<Double_t>(n_waves); //RMS of the baseline
  vector<Double_t> &r=*rPtr;

 vector<Double_t> *tmin_abthPtr=new vector<Double_t>(n_waves); //1st time bin above threshold
  vector<Double_t> &tmin_abth=*tmin_abthPtr;
  vector<Double_t> *tmax_abthPtr=new vector<Double_t>(n_waves); //last time bin above threshold
  vector<Double_t> &tmax_abth=*tmax_abthPtr;

  //create output root file 
  TFile *fout=new TFile(str_out.Data(),"RECREATE");
  TTree* tree_pmt=new TTree("tPmt","35t PMT Data");

  tree_pmt->Branch("id", &id);
  tree_pmt->Branch("time", &time);
  tree_pmt->Branch("timetag", &evt_title);

  tree_pmt->Branch("coinc", &coinc);
  tree_pmt->Branch("amax", &amaxPtr);
  tree_pmt->Branch("tmax", &tmaxPtr);
  tree_pmt->Branch("amin", &aminPtr);
  tree_pmt->Branch("tmin", &tminPtr);
  tree_pmt->Branch("q", &qPtr);
  tree_pmt->Branch("q0", &q0Ptr);
  tree_pmt->Branch("pq", &pqPtr);

  tree_pmt->Branch("length", &lengthPtr);
  tree_pmt->Branch("b", &bPtr);
  tree_pmt->Branch("th", &thPtr);
  tree_pmt->Branch("r", &rPtr);

  tree_pmt->Branch("tmin_abth", &tmin_abthPtr);
  tree_pmt->Branch("tmax_abth", &tmax_abthPtr);

  fout->cd();

  //file loop --------------------------------------------------------------------------------------------//
  TString str_title;
  string str_temp;
  size_t k;
  TTimeStamp time_min;
  TTimeStamp time_max;
  Double_t time_duration=0.;
  size_t n_broken=0;
  size_t n_good=0;
  Int_t id_cnt=0;
  for (size_t ii=0; ii<wf_fielname.size();ii++) { //file loop
	//get file name
        TString inputfilename(wf_fielname[ii]);
        //cout<<"open PMT WF file ["<<ii<<"]:"<<wf_fielname[ii]<<endl;

        //get title of each file
	string str_name0=wf_fielname[ii].substr(wf_fielname[ii].find("_2017_")+1);
	string str_name1=str_name0.substr(0,str_name0.find("csv")-1);
	string str_date=str_name1.substr(0,str_name1.find("-"));
	string str_time=str_name1.substr(str_name1.find("-")+1);

	//get time info
	UInt_t hour, min, sec, msec, month, day, year;
        //UInt_t jhour, jmin, jsec, jmsec, jmonth, jday, jyear;
        year=(UInt_t)atoi(str_date.substr(0,4).c_str());
	month=(UInt_t)atoi(str_date.substr(6,8).c_str());
	day=(UInt_t)atoi(str_date.substr(8,10).c_str());
        hour=(UInt_t)atoi(str_time.substr(0,2).c_str());
        min=(UInt_t)atoi(str_time.substr(3,5).c_str());
        sec=(UInt_t)atoi(str_time.substr(6,8).c_str());
	//msec=0;
	msec=(UInt_t)atoi(str_time.substr(9,11).c_str());

        //cout<<"yr:"<<atoi(tmp_yr.c_str())<<endl;
        //cout<<"yr:"<<year<<endl;
        //cout<<"month:"<<month<<endl;
        //cout<<"day:"<<day<<endl;
        //cout<<"hr:"<<hour<<endl;
        //cout<<"min:"<<min<<endl;
        //cout<<"sec:"<<sec<<endl;
        //cout<<"msec:"<<msec<<endl;
	time=TTimeStamp(year, month, day, hour, min, sec, msec*1e6);
	evt_title=str_name1;


	//if (ii==0) { time_min=time; }
	//if (ii==(wf_fielname.size()-1)) { time_max=time; }
	
	//cout<<""<<endl;

        //read each data in the given list
    	std::ifstream File(inputfilename.Data());
    	Double_t buffer;
    	vector<Double_t> data;
    	vector<Double_t> zero;
    	while (File>>buffer) { data.push_back(buffer); }
    	File.close();

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

	//cout<<"I need:"<<data[0]*n_ch<<endl;
	//cout<<"I have data size:"<<data.size()<<endl;

        if (IsGoodData==true) { //is good data
		n_good++;
		if (n_good==1) { time_min=time; }
		if (n_good==(wf_fielname.size()-1-n_broken)) { time_max=time; }
		id_cnt++;
		id=id_cnt;
	  	reftime=time.AsDouble()-time_min;

        	//get dt, t0 info from each data
        	Double_t dt[n_waves];
        	Double_t t0[n_waves]; 
        	Double_t ttmax[n_waves];
		Int_t pulse_length=(Int_t)data[0]; //1st column is the number of bins of traces
		//Double_t pulse_length=(Double_t)data[0]; //1st column is the number of bins of traces

        	//Traces loop
        	TH1D **hps=new TH1D*[n_waves];
		Double_t Amax[n_waves];
		Double_t tAmax[n_waves];
		Double_t Amin[n_waves];
		Double_t tAmin[n_waves];

		Double_t Charge[n_waves];
		Double_t pCharge[n_waves];
		Double_t Charge0[n_waves];
		Double_t Charge0x[n_waves];

		Double_t tabthr_min[n_waves];
		Double_t tabthr_max[n_waves];
		Double_t thr[n_waves];

		Double_t ped[n_waves];
		Double_t rms[n_waves];

		Int_t Trigger_Scalar[n_waves]={100,10,1,0};
		Int_t Trigger=0;
        	for (size_t i=0; i<n_waves; i++) { //waveform loop
    			for (k=0; k<data.size() ; k++) { //1st data loop to get t0, dt
				if (k==index[i]) t0[i]=data[k]; 
                		if (k==(index[i]+1)) dt[i]=data[k];
			} //1st data loop to get t0, dt
	        	//cout<<"t0["<<i<<"]:"<<t0[i]<<" ; dt["<<i<<"]:"<<dt[i]<<endl; //print what u got

			//2nd loop to extract waveforms from data
			ttmax[i]=t0[i]+(Double_t)pulse_length*dt[i]; //cout<<"ttmax:"<<ttmax[i]<<endl;
       			hps[i]=new TH1D(Form("hps_%d",i),Form("ch%d",i+1),data[0],t0[i]*time_unit_convert,ttmax[i]*time_unit_convert);
        		int st,ed;
			st=index[i]+2;
        		ed=st+data[0];
        		//cout<<""<<st<<" - "<<ed<<endl;
        		for (k=st; k<ed; k++) { hps[i]->SetBinContent(k-st+1,-data[k]); } //invert the traces

			//calculate RMS
			//Int_t st_bin_rms=10;
			//Int_t nbin_rms=50;
			Int_t st_bin_rms=1;
			Int_t nbin_rms=500;
			rms[i]=RMS(st_bin_rms,st_bin_rms+nbin_rms,hps[i]);
			ped[i]=AVBASE(st_bin_rms,st_bin_rms+nbin_rms,hps[i]);

			Double_t xmin_rms=t0[0]*time_unit_convert+(Double_t)(st_bin_rms-1)*dt[i]*time_unit_convert;
			Double_t xmax_rms=xmin_rms+time_unit_convert*dt[i]*(Double_t)nbin_rms;
			//cout<<"mean["<<i<<"]:"<<mean_line<<":"<<"rms["<<i<<"]:"<<rms_line<<": plot range of x["<<i<<"]:"<<xmin_rms<<" - "<<xmax_rms<<endl;

			//Get maximaum amplitude
			Amax[i]=AMAX(1,hps[i]->GetNbinsX(),hps[i]); //get max. amp. from 1st to the last bin
			tAmax[i]=TMAX(1,hps[i]->GetNbinsX(),hps[i]); //get max. amp. from 1st to the last bin

			//Get min. amp.
			Amin[i]=AMIN(1,hps[i]->GetNbinsX(),hps[i]); //get min. amp. from 1st to the last bin 
			tAmin[i]=TMIN(1,hps[i]->GetNbinsX(),hps[i]); //get min. amp. from 1st to the last bin

			//Get time above threshold
			thr[i]=ped[i]+5.*rms[i];
			tabthr_min[i]=TMIN_ABTH(1,hps[i]->GetNbinsX(),hps[i],thr[i]);
			tabthr_max[i]=TMAX_ABTH(1,hps[i]->GetNbinsX(),hps[i],thr[i]);

			//Calculate charge
			Int_t tbin_max=TMAXBIN(1,hps[i]->GetNbinsX(),hps[i]);
			pCharge[i]=Q(tbin_max-63,tbin_max+63,hps[i]); //before-after ~0.1us
			Charge[i]=Q(1,hps[i]->GetNbinsX(),hps[i]); //full integration charge
			Charge0[i]=Q(1,500,hps[i]); //integration of the first 500 bins

			if (pulse_length==2500){ Charge0x[i]=20.*Charge0[i]; }
			if (pulse_length==5000){ Charge0x[i]=15.*Charge0[i]; }
			if (pulse_length==10000){ Charge0x[i]=5.*Charge0[i]; }
			if (pulse_length==12500){ Charge0x[i]=0.; }


			//Determine it is trigger or not
			//trigger: 100*[PMT1 trigger]+10*[PMT 2 trigger]+1*[PMT 3 trigger]
			if (amax[i]>thr[i]) Trigger+=Trigger_Scalar[i]; 

			//tree stuff
			amax[i]=Amax[i];
			tmax[i]=tAmax[i];
			amin[i]=Amin[i];
			tmin[i]=tAmin[i];

  			length[i]=(Double_t)pulse_length;
			q[i]=Charge[i];
			pq[i]=pCharge[i];
			q0[i]=Charge0x[i];

			b[i]=ped[i];
			r[i]=rms[i];
			th[i]=thr[i];

			tmin_abth[i]=tabthr_min[i];
			tmax_abth[i]=tabthr_max[i];
		} //waveform loop
		coinc=Trigger;
		//write to the tree
		tree_pmt->Fill();

		//release memory in the end
		for (size_t i=0; i<n_waves; i++) { //release memory
			delete hps[i];
		} //release memory
  	} //is good data
  } //file loop

  //file loop --------------------------------------------------------------------------------------------//
  cout<<"=========================================="<<endl;
  cout<<green<<"There are "<<wf_fielname.size()<<" files."<<reset<<endl;
  cout<<red<<"But "<<n_broken<<" files are broken."<<reset<<endl;
  cout<<"=========================================="<<endl;
   
  fout->Write();

  //Write exposure into file	
  TParameter<Double_t>* exposure_sec=new TParameter<Double_t>("exposure_sec",0.);
  exposure_sec->SetVal(time_max.AsDouble()-time_min.AsDouble()); //total exposure (in unit of sec)
  exposure_sec->Write();

  cout<<cyan<<"Exposure:"<<(time_max.AsDouble()-time_min.AsDouble())/(24.*60.*60.)<<" days"<<reset<<endl;

  TParameter<Double_t>* t0=new TParameter<Double_t>("t0",0.);
  t0->SetVal(time_min); //time zero (in unit of sec)
  t0->Write();
  std::cout<<cyan<<"time zero:"<<t0<<std::endl;

  //close root file
  fout->Close();
  cout<<yellow<<"---->>> root file:"<<str_out.Data()<<" is done :-)) !!"<<reset<<endl;


}

