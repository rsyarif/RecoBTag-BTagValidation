void makeComparisonBackground(TString var , double low =0., double max =100., TString label="", int rebin=1, TString option="330", bool isLog=0, bool Reweight = false){

gROOT->SetStyle("Plain");
gStyle->SetPadGridX(0);
gStyle->SetPadGridY(0);
gStyle->SetOptStat(0);

TCanvas * c1 = new TCanvas("c1","c1", 700, 700);
TLegend *leg = new TLegend(0.4444465625,0.7021654,0.8365625,0.8603839,NULL,"brNDC");
   leg->SetBorderSize(0);
   leg->SetTextSize(0.045);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(0);

TString massp = "M-1000";
TString reweight = "";

if (Reweight) reweight = "reweightPT_";

TFile * f ;
f = new TFile("/afs/cern.ch/user/r/rsyarif/workHere/HbbTagVal/Update_11Dec/CMSSW_7_4_14/src/RecoBTag/BTagValidation/test/rizki_signalMC_GluGLu/signalMC_"+massp+"_fjPt"+option+"_bTagValPlots.root");

f->cd();
TH1D *h = new TH1D();
h = (TH1D*) f->Get("btagval/FatJet_"+var);
// h->Draw();//ptPruned>300 & ptPruned<500");
//delete c1;
h->SetLineColor(kBlue+2);
h->SetLineWidth(3);
// h->GetXaxis()->SetTitle(label);
h->SetTitle("");
double Ymax = h->GetMaximum();
h->GetXaxis()->SetRangeUser(low, max);
h->GetYaxis()->SetRangeUser(0.01, Ymax*3.);
// h->DrawNormalized("");
int nbin =  h->GetXaxis()->GetNbins();
// std::stringstream ss;
// ss << nbin;
// TString strNb = ss.str();
// //float low =  h->GetXaxis()->GetXmin();
// //if( low<0) {low=0; }
// std::stringstream ss1;
// ss1 << low;
// TString strL=(ss1.str());
// //float max = h->GetXaxis()->GetXmax();
// std::stringstream ss2;
// ss2 << max;
// TString strM= (ss2.str());
// //Fjets->Draw(var+">>h("+strNb+","+strL+","+strM+")"," abs(int(flavour))==5 && nbHadrons<2 && massPruned>80 && massPruned<200");//ptPruned>300 & ptPruned<500");
h->SetLineColor(kBlue+2);
h->SetLineWidth(3);
// h->GetXaxis()->SetTitle(label);
h->SetTitle("");
// h->SetName("h_");
h->GetYaxis()->SetRangeUser(0.01, Ymax*3);
h->Scale(1/(h->Integral(1,nbin)));
h->Rebin(rebin);
// h->DrawNormalized("HIST");
// h->SetBins(nbin, low,max);

c1->Update();

//std::cout<<nbin<< "  "<<low<<"  "<<max<<std::endl;
leg->AddEntry(h,"Hbb (bfromg flavor)","l");
// TCanvas * d = new TCanvas("d","d");
// d->cd();


TFile * f2 ;
if(option=="330"){
// 	f2 = new TFile("/afs/cern.ch/user/r/rsyarif/workHere/HbbTagVal/Update_11Dec/CMSSW_7_4_14/src/RecoBTag/BTagValidation/test/rizki_LXBatch_BoostedTaggerValidation_BTagMu_QCDMuEnriched_usePruned_fjpt330_eta2p4_DoubleMuTag_50m200_"+reweight+"merged/Final_histograms_btagval_DoubleMuonTaggedFatJets_MuonEnrichedJets_Pruned.root");
	f2 = new TFile("/afs/cern.ch/work/r/rsyarif/work/HbbTagVal/Update_11Dec/CMSSW_7_4_14/src/RecoBTag/BTagValidation/test/rizki_LXBatch_BoostedTaggerValidation_cutFlow_BTagMu_QCDMuEnriched_usePruned_fjpt330_eta2p4_DoubleMuTag_50m200_reweightPT_merged/Final_histograms_btagval_DoubleMuonTaggedFatJets_MuonEnrichedJets_Pruned.root");
	}
else if(option=="450"){
	f2 = new TFile("/afs/cern.ch/user/r/rsyarif/workHere/HbbTagVal/Update_11Dec/CMSSW_7_4_14/src/RecoBTag/BTagValidation/test/rizki_LXBatch_BoostedTaggerValidation_DoubleMuonTagged_50m200_tau21-1p0_fj450_usePruned_debug_ptRatioCut_"+reweight+"merged/Final_histograms_btagval_DoubleMuonTaggedFatJets_MuonEnrichedJets_Pruned.root");
	}
f2->cd();
TH1D *h2 = new TH1D();
h2 = (TH1D*) f2->Get("QCD__FatJet_"+var);

//h2->Draw("same");
h2->SetLineColor(kAzure+1);
h2->SetLineWidth(3);
h2->GetXaxis()->SetTitle(var);
h2->GetXaxis()->SetRangeUser(low, max);
h2->SetTitle("");
// h2->SetName("h2_");
//h2->GetYaxis()->SetRangeUser(0., Ymax*2.);
c1->cd();
c1->Update();
h2->Scale(1/(h2->Integral(1,nbin)));
h2->Rebin(rebin);
// h2->DrawNormalized("sameHIST");
// h2->SetBins(nbin,low,max);

leg->AddEntry(h2,"QCDbb (bfromg flavor)","l");

// THStack *stack = new THStack("stack","");
// stack->Add(h,"hist");
// stack->Add(h2,"hist");
// stack->Draw("nostack");
// if(isLog==1)stack->SetMaximum(Ymax*3);
// stack->GetXaxis()->SetRangeUser(low, max);
// stack->GetXaxis()->SetTitle(label);

//delete h2;

if(var=="tau2IVF/tau1IVF") var = "subIVF";
if(var=="tau2/tau1") var = "sub";

// c1->Update();
// c1->Modified();
// if(isLog==0){
// 	c1->Print(("plots/"+var+"_"+massp+"_"+option+".png"));
// 	c1->SaveAs("plots/"+var+"_"+massp+"_"+option+".pdf");
// 	}
// if(isLog==1){	
// 	c1->SetLogy();
// 	c1->Print(("plots/"+var+"_"+massp+"_"+option+"_log.png"));
// 	c1->SaveAs("plots/"+var+"_"+massp+"_"+option+"_log.pdf");
// 	}
//delete c1;
//delete strL;


// RATIO PLOT

TH1D *hratio = (TH1D*)h->Clone("hratio");
hratio->Divide(h2);

 //*************************************************
  // Variables
  bool topPlotLogY = 1;      // 0 = no log; 1= log
  TString yTitle2 = "Hbb/QCDbb"; // bottom plot y axis title

  vector<int> histColors; 
  histColors.push_back(kBlue);  // change colors as you like
//   histColors.push_back(kRed);
//   histColors.push_back(kGreen-1);

//   int histDenominatorColor = kBlack;

  float defaultRatioYmin = -0.5;
  float defaultRatioYmax = 2.5;
  // END of Variables
  //*************************************************

 // Bottom plot
  TPad *c1_1 = new TPad("c1_1", "newpad",0.01,0.01,0.99,0.32);
  c1_1->Draw();
  c1_1->cd();
  c1_1->SetTopMargin(0.01);
  c1_1->SetBottomMargin(0.3);
  c1_1->SetRightMargin(0.1);
  c1_1->SetFillStyle(0);
  c1_1->SetGridy();
	

  hratio->SetLineWidth(1);
  hratio->SetMarkerStyle(1);
  hratio->SetLineColor(histColors[0]);
  hratio->SetMinimum(defaultRatioYmin);
  hratio->SetMaximum(defaultRatioYmax);
  hratio->GetYaxis()->SetNdivisions(5);
  hratio->SetTitle(";"+label+";"+yTitle2);
//   hratio->SetTitle(";"+xTitle+";"+yTitle2);
  hratio->GetXaxis()->SetTitleSize(0.14);
  hratio->GetXaxis()->SetLabelSize(0.14);
  hratio->GetYaxis()->SetLabelSize(0.11);
  hratio->GetYaxis()->SetTitleSize(0.14);
  hratio->GetYaxis()->SetTitleOffset(0.28);
  hratio->GetXaxis()->SetRangeUser(low, max);
  hratio->Draw("same");
  
  // End bottom plot
  //*************************************************

  //*************************************************
  // Top Plot
  c1->cd();
  c1_2 = new TPad("c1_2", "newpad",0.01,0.33,0.99,0.99);
  c1_2->Draw(); 
  c1_2->cd();
  c1_2->SetTopMargin(0.1);
  c1_2->SetBottomMargin(0.01);
  c1_2->SetRightMargin(0.1);
  c1_2->SetFillStyle(0);

  h2->SetLineWidth(2);
//   h2->Draw("hist");
  h2->SetLabelSize(0.0);
  h2->GetXaxis()->SetTitleSize(0.00);
  h2->GetYaxis()->SetLabelSize(0.07);
  h2->GetYaxis()->SetTitleSize(0.08);
  h2->GetYaxis()->SetTitleOffset(0.76);
  h2->SetMaximum(1.2*h->GetMaximum());
//   h2->GetYaxis()->SetRangeUser(1e-6, 1.2*h->GetMaximum());
  
  h->SetLineWidth(2);
//   h->Draw("samehist");

THStack *stack = new THStack("stack","");
stack->Add(h,"hist");
stack->Add(h2,"hist");
stack->Draw("nostack");
if(isLog==1)stack->SetMaximum(Ymax*2);
stack->GetXaxis()->SetRangeUser(low, max);
// stack->GetXaxis()->SetTitle(label);
stack->GetXaxis()->SetLabelSize(0.0);
stack->GetXaxis()->SetLabelOffset(999);


  c1_2->SetLogy(isLog);
  // End top plot

leg->Draw("same");
c1->Modified();
if(isLog==0){
	c1->Print(("plots/"+var+"_"+massp+"_"+reweight+option+"_linear_ratio.png"));
	c1->SaveAs("plots/"+var+"_"+massp+"_"+reweight+option+"_linear_ratio.pdf");
	}
if(isLog==1){	
	c1->SetLogy();
	c1->Print(("plots/"+var+"_"+massp+"_"+reweight+option+"_log_ratio.png"));
	c1->SaveAs("plots/"+var+"_"+massp+"_"+reweight+option+"_log_ratio.pdf");
	}


}
