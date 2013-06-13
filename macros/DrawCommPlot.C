#include "CMSstyle.C"
#include "help.C"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "TH1D.h"
#include "TMultiGraph.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "THStack.h"
#include "TFile.h"
#include "TROOT.h"
#include "TF1.h"
#include "TMath.h"
#include <TLegend.h>
#include <TProfile.h>
#include <TLatex.h>
#include <TAxis.h>
#include <TProfile.h>
#include <TString.h>

#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdlib.h>

using namespace std;

//TString filename="/afs/cern.ch/work/f/ferencek/CMSSW_5_3_9/src/MyAnalysis/BTagValidation/test/LXBatch_Jobs_InclusiveJets/Final_histograms.root";
//TString filename="/afs/cern.ch/work/f/ferencek/CMSSW_5_3_9/src/MyAnalysis/BTagValidation/test/LXBatch_Jobs_MuonTaggedFatJets/Final_histograms.root";
//TString filename="/afs/cern.ch/work/f/ferencek/CMSSW_5_3_9/src/MyAnalysis/BTagValidation/test/LXBatch_Jobs_MuonTaggedSubJets/Final_histograms.root";
//TString filename="/afs/cern.ch/work/f/ferencek/CMSSW_5_3_9/src/MyAnalysis/BTagValidation/test/LXBatch_Jobs_DoubleMuonTaggedFatJets/Final_histograms.root";
//TString filename="/afs/cern.ch/work/f/ferencek/CMSSW_5_3_9/src/MyAnalysis/BTagValidation/test/LXBatch_Jobs_DoubleMuonTaggedFatJets_FatJetPt250/Final_histograms.root";

TString filename="/afs/cern.ch/work/f/ferencek/CMSSW_5_3_9/src/MyAnalysis/BTagValidation/test/LXBatch_Jobs_DoubleMuonTaggedFatJets_RelaxedMuonID/Final_histograms_btagval.root";
//TString filename="/afs/cern.ch/work/f/ferencek/CMSSW_5_3_9/src/MyAnalysis/BTagValidation/test/LXBatch_Jobs_DoubleMuonTaggedFatJets_RelaxedMuonID_BTaggedSubJets/Final_histograms_btagval.root";
//TString filename="/afs/cern.ch/work/f/ferencek/CMSSW_5_3_9/src/MyAnalysis/BTagValidation/test/LXBatch_Jobs_DoubleMuonAndBTaggedFatJets_RelaxedMuonID/Final_histograms_btagval.root";

//TString filename="/afs/cern.ch/work/f/ferencek/CMSSW_5_3_9/src/MyAnalysis/BTagValidation/test/LXBatch_Jobs_DoubleMuonTaggedFatJets_RelaxedMuonID_BTaggedSubJets_AppliedSFs/Final_histograms_btagval.root";
//TString filename="/afs/cern.ch/work/f/ferencek/CMSSW_5_3_9/src/MyAnalysis/BTagValidation/test/LXBatch_Jobs_DoubleMuonAndBTaggedFatJets_RelaxedMuonID_AppliedSFs/Final_histograms_btagval.root";

TString filename_ext="/afs/cern.ch/work/f/ferencek/CMSSW_5_3_9/src/MyAnalysis/BTagValidation/test/LXBatch_Jobs_DoubleMuonTaggedFatJets_RelaxedMuonID/Final_histograms_btagval.root";

TString dir4plots="Commissioning_plots";
TString title= "CMS Preliminary, #sqrt{s} = 8 TeV,  L = 19.8 fb^{-1}";
TString datacaption = "Data";//"HLT_PFJet320, jet p_{T}>400 GeV";
TString format=".png";
bool bOverflow=true;
bool web = false;

void DrawAll(bool Draw_track_plots, bool Draw_Nminus1_plots, bool Draw_sv_plots, bool Draw_muons_plots, bool Draw_discriminator_plots, bool Draw_tagRate_plots, bool Draw_2D_plots, TString histoTag);
void Draw(TString name, TString histotitle, bool log);
void DrawStacked(TString name, TString histotitle, bool log, bool doData, bool extNorm=false, int nRebin=1);
void DrawTagRate(TString name, TString histotitle, bool log, bool doData);
void Draw2DPlot(TString name, TString histotitle, TString titleX, TString titleY, bool log, bool doData);


//--------------------------
void DrawCommPlot(bool Draw_track_plots=false, bool Draw_Nminus1_plots=false, bool Draw_sv_plots=false, bool Draw_muons_plots=false, bool Draw_discriminator_plots=false, bool Draw_tagRate_plots=false, bool Draw_2D_plots=false){

  gROOT->SetBatch(kTRUE);
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  gStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
  gStyle->SetPadTickY(1);  // To get tick marks on the opposite side of the frame

  TString action = "mkdir -p " + dir4plots;
  system(action);

  Draw("h1_nPV"      ,"# of PV",0);
//   Draw("h1_nFatJet"  ,"# of fat jets",0);
//   Draw("h1_nSubJet"  ,"# of subjets",0);

  TString histoTag = "FatJet" ;
  DrawAll(Draw_track_plots, Draw_Nminus1_plots, Draw_sv_plots, Draw_muons_plots, Draw_discriminator_plots, Draw_tagRate_plots, Draw_2D_plots, histoTag) ;
  histoTag = "SubJet" ;
  DrawAll(Draw_track_plots, Draw_Nminus1_plots, Draw_sv_plots, Draw_muons_plots, Draw_discriminator_plots, Draw_tagRate_plots, Draw_2D_plots, histoTag) ;

  return ;

}

//--------------------------
void DrawAll(bool Draw_track_plots, bool Draw_Nminus1_plots, bool Draw_sv_plots, bool Draw_muons_plots, bool Draw_discriminator_plots, bool Draw_tagRate_plots, bool Draw_2D_plots, TString histoTag) {

  DrawStacked(histoTag+"_pt_all"      ,"p_{T} of all jets"          ,1 ,1, 0, 2);
  DrawStacked(histoTag+"_eta"         ,"#eta of all jets"           ,0 ,1, 0, 2);
  DrawStacked(histoTag+"_phi"         ,"#phi of all jets"           ,0 ,1, 0, 2);
  DrawStacked(histoTag+"_mass"        ,"mass of all jets"           ,0 ,1, 0, 2);
  if( histoTag=="FatJet" )
  {
    DrawStacked(histoTag+"_pruned_mass"  ,"pruned mass all jets"     ,0 ,1, 0, 2);
    DrawStacked(histoTag+"_subjet_dR"    ,"dR(subjet_{1},subjet_{2}) in #eta-#phi plane"   ,0 ,1, 0, 2);
    DrawStacked(histoTag+"_subjet_dyphi" ,"dR(subjet_{1},subjet_{2}) in y-#phi plane"      ,0, 1, 0, 2);
    DrawStacked(histoTag+"_nsubjettiness","#tau_{2}/#tau_{1}"      ,0 ,1);
  }

  if (Draw_track_plots){
//     DrawStacked(histoTag+"_track_multi"   ,    "number of tracks in the jets",0,1);
    DrawStacked(histoTag+"_trk_multi_sel" ,    "number of selected tracks in the jets",0,1);
//     DrawStacked(histoTag+"_track_chi2"    ,    "normalized #chi^{2} of the tracks",1,1);
    DrawStacked(histoTag+"_track_nHit"    ,    "number of hits",1,1);
    DrawStacked(histoTag+"_track_HPix"    ,    "number of hits in the Pixel",1,1);
    DrawStacked(histoTag+"_track_len"     ,    "Track decay length",1,1);
    DrawStacked(histoTag+"_track_dist"    ,    "Track distance to the jet axis",1,1);
//     DrawStacked(histoTag+"_track_dz"      ,    "Track transverse IP",1,1);
    DrawStacked(histoTag+"_track_pt"      ,    "p_{T} of all the tracks",1,1);
//     DrawStacked(histoTag+"_track_isfromSV",    "Track is from SV",1,1);

    DrawStacked(histoTag+"_track_IPs"    ,      "3D IP significance of all tracks",1,1);
//     DrawStacked(histoTag+"_track_IPs1tr" ,      "3D IP significance of the first track",1,1);
//     DrawStacked(histoTag+"_track_IPs2tr" ,      "3D IP significance of the second track",1,1);
//     DrawStacked(histoTag+"_track_IPs3tr" ,      "3D IP significance of the third track",1,1);
    DrawStacked(histoTag+"_track_IP"     ,      "3D IP of all tracks",1,1);
//     DrawStacked(histoTag+"_track_IP1tr"  ,      "3D IP of the first track",1,1);
//     DrawStacked(histoTag+"_track_IP2tr"  ,      "3D IP of the second track",1,1);
//     DrawStacked(histoTag+"_track_IP3tr"  ,      "3D IP of the third track",1,1);
//     DrawStacked(histoTag+"_track_IP2Ds"	,     "2D IP significance of all tracks",1,1);
//     DrawStacked(histoTag+"_track_IP2Ds1tr" ,    "2D IP significance of the first track",1,1);
//     DrawStacked(histoTag+"_track_IP2Ds2tr" ,    "2D IP significance of the second track" ,1,1);
//     DrawStacked(histoTag+"_track_IP2Ds3tr" ,    "2D IP significance of the second track" ,1,1);
//     DrawStacked(histoTag+"_track_IP2D"    ,     "2D IP of all tracks",1,1);
//     DrawStacked(histoTag+"_track_IP2D1tr" ,     "2D IP of the first track",1,1);
//     DrawStacked(histoTag+"_track_IP2D2tr" ,     "2D IP of the second track",1,1);
//     DrawStacked(histoTag+"_track_IP2D3tr" ,     "2D IP of the third track",1,1);
//     DrawStacked(histoTag+"_track_IP2Derr" ,     "2D IP error of all tracks",1,1);
//     DrawStacked(histoTag+"_track_IP2Derr1tr" ,  "2D IP error of the first track",1,1);
//     DrawStacked(histoTag+"_track_IP2Derr2tr" ,  "2D IP error of the second track" ,1,1);
//     DrawStacked(histoTag+"_track_IP2Derr3tr" ,  "2D IP error of the third track",1,1);
//     DrawStacked(histoTag+"_track_IPerr"   ,     "3D IP error of all tracks",1,1);
//     DrawStacked(histoTag+"_track_IPerr1tr"   ,  "3D IP error of the first track" ,1,1);
//     DrawStacked(histoTag+"_track_IPerr2tr"   ,  "3D IP error of the second track" ,1,1);
//     DrawStacked(histoTag+"_track_IPerr3tr"   ,  "3D IP error of the third track" ,1,1);
  }
  if (Draw_Nminus1_plots){
//     DrawStacked(histoTag+"_track_chi2_cut"    ,"Normalized #chi^{2} @N-1 step",1,1);
    DrawStacked(histoTag+"_track_nHit_cut"    ,"Number of hits @N-1 step",1,1);
    DrawStacked(histoTag+"_track_HPix_cut"    ,"Number of hits in the Pixel @N-1 step",1,1);
//     DrawStacked(histoTag+"_track_len_cut"     ,"Decay length @N-1 step",1,1);
//     DrawStacked(histoTag+"_track_dist_cut"    ,"Distance to the jet axis @N-1 step" ,1,1);
//     DrawStacked(histoTag+"_track_dz_cut"      ,"Transverse IP @N-1 step",1,1);
//     DrawStacked(histoTag+"_track_pt_cut"	    ,"Track p_{T} @N-1 step",1,1);
  }
  if (Draw_sv_plots){
    DrawStacked(histoTag+"_pt_sv"     ,"p_{T} of jets containing a SV",1 ,1);
    DrawStacked(histoTag+"_sv_multi_0","nr. of SV including bin 0",1,1);
//     DrawStacked(histoTag+"_sv_multi","nr. of SV",1,1);
    DrawStacked(histoTag+"_sv_mass","SV mass",0,1);
//     DrawStacked(histoTag+"_sv_mass_3trk","SV mass if #tracks@SV >=3",0,1);
//     DrawStacked(histoTag+"_sv_chi2norm","SV norm. #chi^{2}",1,1);
    DrawStacked(histoTag+"_sv_deltaR_jet","#DeltaR between the jet and the SV direction",0,1);
//     DrawStacked(histoTag+"_sv_deltaR_sumJet","#DeltaR between the jet and the SV",0,1);
//     DrawStacked(histoTag+"_sv_deltaR_sumDir","#DeltaR between the jet direction and the SV",0,1);
    DrawStacked(histoTag+"_sv_en_ratio","SV energy ratio",0,1);
//     DrawStacked(histoTag+"_sv_aboveC","IP2D of the first track above the charm threshold",1,1);
    DrawStacked(histoTag+"_sv_pt","SV p_{T}",1,1);
//     DrawStacked(histoTag+"_sv_eta","SV #eta",0,1);
//     DrawStacked(histoTag+"_sv_phi","SV #phi",0,1);
//     DrawStacked(histoTag+"_sv_flight3D","SV 3D flight distance",1,1);
//     DrawStacked(histoTag+"_sv_flight2D","SV 2D flight distance",1,1);
    DrawStacked(histoTag+"_sv_flight3DSig","SV 3D flight distance significance",1,1);
//     DrawStacked(histoTag+"_sv_flightSig2D","SV 2D flight distance significance",1,1);
//     DrawStacked(histoTag+"_sv_flight3Derr","SV 3D flight distance error",1,1);
//     DrawStacked(histoTag+"_sv_flight2Derr","SV 2D flight distance error",1,1);
//     DrawStacked(histoTag+"_svnTrk","nr. of tracks from a SV",1,1);
    DrawStacked(histoTag+"_svnTrk_firstVxt","nr. of tracks from the first SV",1,1);
  }
  if (Draw_muons_plots){
    DrawStacked(histoTag+"_muon_multi"   ,      "number of muons", 1,1);
    DrawStacked(histoTag+"_muon_multi_sel"   ,  "number of selected muons",1,1);
    DrawStacked(histoTag+"_mu_ptrel"     ,      "p_{T} rel. of the muon",0,1);
    DrawStacked(histoTag+"_mu_chi2"      ,      "norm. #chi^{2} of the muon", 1,1);
    DrawStacked(histoTag+"_muon_Pt",	     "Muon p_{T}",1,1);
    DrawStacked(histoTag+"_muon_eta",	     "Muon #eta",0,1);
    DrawStacked(histoTag+"_muon_phi",	     "Muon #phi",0,1);
    DrawStacked(histoTag+"_muon_Ip3d",	     "Muon 3D IP",1,1);
    DrawStacked(histoTag+"_muon_Ip2d",	     "Muon 2D IP",1,1);
    DrawStacked(histoTag+"_muon_Sip3d",	     "Muon 3D IP significance",1,1);
    DrawStacked(histoTag+"_muon_Sip2d",	     "Muon 2D IP significance",1,1);
    DrawStacked(histoTag+"_muon_DeltaR",	     "Muon1 #Delta R",0,1);

  }
  if (Draw_discriminator_plots){
//     DrawStacked(histoTag+"_TCHE_extended1"       ,"TCHE (extended)",1,1);
//     DrawStacked(histoTag+"_TCHP_extended1"       ,"TCHP (extended)",1,1);
//     DrawStacked(histoTag+"_discri_ssche0",      "SSVHE Discriminator", 1,1);
//     DrawStacked(histoTag+"_discri_sschp0",      "SSVHP Discriminator", 1,1);
//
//     DrawStacked(histoTag+"_TCHE"	      ,"TCHE Discriminator", 1,1);
//     DrawStacked(histoTag+"_TCHP"	      ,"TCHP Discriminator",1,1);
    DrawStacked(histoTag+"_JP"	      ,"JP Discriminator",1,1);
//     DrawStacked(histoTag+"_JBP"	      ,"JBP Discriminator",1,1);
//     DrawStacked(histoTag+"_SSV"	      ,"SSVHE Discriminator",1,1);
//     DrawStacked(histoTag+"_SSVHP"        ,"SSVHP Discriminator",1,1);
    DrawStacked(histoTag+"_CSV"	      ,"CSV Discriminator",1,1);
  }

  if (Draw_tagRate_plots){
    DrawTagRate(histoTag+"_TCHE_extended1","TCHE (extended)", 1, 1);
    DrawTagRate(histoTag+"_TCHP_extended1"," TCHP (extended)",1, 1);
    DrawTagRate(histoTag+"_discri_ssche0","SSVHE (extended)", 1, 1);
    DrawTagRate(histoTag+"_discri_sschp0","SSVHP (extended)", 1, 1);

    DrawTagRate(histoTag+"_TCHE"	      ,"TCHE Discriminator"     ,1 ,1);
    DrawTagRate(histoTag+"_TCHP"	      ,"TCHP Discriminator"     ,1 ,1);
    DrawTagRate(histoTag+"_JP"	      ,"JP Discriminator"         ,1 ,1);
    DrawTagRate(histoTag+"_JBP"	      ,"JBP Discriminator"      ,1 ,1);
    DrawTagRate(histoTag+"_SSV"	      ,"SSVHE Discriminator"    ,1 ,1);
    DrawTagRate(histoTag+"_SSVHP"         ,"SSVHP Discriminator" ,1 ,1);
    DrawTagRate(histoTag+"_CSV"	      ,"CSV Discriminator"      ,1 ,1);

  }

  if (Draw_2D_plots){
    Draw2DPlot("seltrack_vs_jetpt", "nr. of selected tracks as a function of the jet p_{T}", "jet p_{T}","nr. of selected tracks",0,1);
    Draw2DPlot("sv_mass_vs_flightDist3D", " SV mass as a function of the SV 3D flight distance ","SV 3D flight distance","SV mass",0,1);
    Draw2DPlot("avg_sv_mass_vs_jetpt","Avg SV mass as a function of the jet p_{T}","jet p_{T}","Avg SV mass",0,1);
    Draw2DPlot("sv_deltar_jet_vs_jetpt","#Delta R between the SV and the jet as a function of the jet p_{T}","jet p_{T}","#Delta R between the SV and the jet",0,1);
    Draw2DPlot("sv_deltar_sum_jet_vs_jetpt","#Delta R between the SV and the jet sum as a function of the jet p_{T}","jet p_{T}","#Delta R between the SV and the jet sum",0,1);
    Draw2DPlot("sv_deltar_sum_dir_vs_jetpt","#Delta R between the SV and the jet direction as a function of the jet p_{T}", "jet p_{T}","#Delta R between the SV and the jet direction",0,1);
    Draw2DPlot("muon_ptrel_vs_jetpt","Muon_p{T}^{rel} as a function of the jet p_{T}","jet p_{T}","Muon_p{T}^{rel}",0,1);
    Draw2DPlot("muon_DeltaR_vs_jetpt","Muon #Delta R as a function of the jet p_{T}","jet p_{T}","Muon #Delta R",0,1);
  }
}

//--------------------------
void Draw(TString name, TString histotitle, bool log) {

  TFile *myFile     = TFile::Open(filename,"READ");
  TH1D* hist_mc;
  TH1D* hist_data;

  if (name=="h1_nPV") {
    hist_mc       = (TH1D*)myFile->Get("QCD__"+name+"_mc");
    hist_data     = (TH1D*)myFile->Get("DATA__"+name+"_data");
  } else {
    hist_mc       = (TH1D*)myFile->Get("QCD__"+name);
    hist_data     = (TH1D*)myFile->Get("DATA__"+name);
  }

  float scale_f = (hist_data->Integral())/(hist_mc->Integral());
  hist_mc->Scale(scale_f);

  beautify(hist_data  , 1     , 0, 1) ;

  TH1D* histo_ratio;
  histo_ratio = (TH1D*) hist_data->Clone();
  //histo_ratio->Sumw2();
  histo_ratio->SetName("histo_ratio");
  histo_ratio->SetTitle("");
  histo_ratio->Divide(hist_mc);

  hist_data->SetLineWidth(2);
  hist_data->SetMarkerStyle(20);
  hist_data->SetMarkerSize(0.75);

  hist_mc->SetLineColor(2);
  hist_mc->SetFillColor(2);

  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);

  TCanvas *c1 = new TCanvas("c1", "c1",1200,800);
  c1->SetFillColor(10);
  c1->cd();

  TPad* pad0 = new TPad("pad0", "pad0",0,0.25,1.0,0.98);
  pad0 ->Draw();
  pad0 ->cd();

  pad0->SetLogy(log);

  hist_mc->GetXaxis()->SetTitle(name);
  hist_mc->GetYaxis()->SetTitle("Entries");
  hist_mc->SetTitleOffset(0.75,"Y");
  hist_mc->GetYaxis()->SetLabelSize( 0.05 );
  hist_mc->GetYaxis()->SetTitleSize( 0.06 );

  hist_mc->Draw("hist");

  hist_data->Draw("SAMEE");

  TLegend* qw =  new TLegend(0.7,0.73,0.95,1.);
  //Legend
  qw->AddEntry(hist_data,     datacaption,   "pl");
  qw->AddEntry(hist_mc,        "MC "  ,      "f");

  qw->SetFillColor(0);
  qw->Draw();

  TLatex *latex = new TLatex();
  latex->SetNDC();
  latex->SetTextSize(0.055);
  latex->SetTextFont(42);
  latex->SetTextAlign(13);
  latex->DrawLatex(0.10, 0.96, title);

  c1->cd();

  TPad* pad1 = new TPad("pad1", "pad1",0,0.,1.0,0.32);
  pad1->Draw();
  pad1->cd();
  gPad->SetBottomMargin(0.375);
  gPad->SetGridy();

  histo_ratio->SetMarkerStyle(20);
  histo_ratio->SetMarkerSize(0.75);
  histo_ratio->SetLineWidth(2);

  histo_ratio->GetYaxis()->SetTitle("Data/MC");
  histo_ratio->SetTitleOffset(0.3,"Y");
  histo_ratio->GetXaxis()->SetTitle(histotitle);
  histo_ratio->GetYaxis()->SetNdivisions( 505 );

  double labelsizex=0.12;
  double labelsizey=0.12;
  double titlesizex=0.15;
  double titlesizey=0.14;

  histo_ratio->GetXaxis()->SetLabelSize( labelsizex );
  histo_ratio->GetXaxis()->SetTitleSize( titlesizex );
  histo_ratio->GetYaxis()->SetLabelSize( labelsizey );
  histo_ratio->GetYaxis()->SetTitleSize( titlesizey );

  histo_ratio->SetMinimum(0.4);
  histo_ratio->SetMaximum(1.6);
  histo_ratio->Draw("E1X0");

  c1->cd();

  TString name_plot=name+"_Linear"+format;
  if(log) name_plot=name+"_Log"+format;
  c1->SaveAs(dir4plots+"/"+name_plot);

}

//--------------------------
void DrawStacked(TString name, TString histotitle, bool log, bool doData, bool extNorm, int nRebin) {

  TH1D* hist_b;
  TH1D* hist_c;
  TH1D* hist_gsplit;
  TH1D* hist_l;
  TH1D* hist_data;

  TFile *myFile  = TFile::Open(filename,"READ") ;
  myFile->cd();

  hist_b                = (TH1D*)myFile->Get("QCD__"+name+"_b");
  hist_c                = (TH1D*)myFile->Get("QCD__"+name+"_c");
  hist_gsplit           = (TH1D*)myFile->Get("QCD__"+name+"_bfromg");
  hist_l                = (TH1D*)myFile->Get("QCD__"+name+"_l");
  if (doData) hist_data = (TH1D*)myFile->Get("DATA__"+name+"_data");

  if (nRebin>1) {
    hist_b     ->Rebin(nRebin);
    hist_c     ->Rebin(nRebin);
    hist_gsplit->Rebin(nRebin);
    hist_l     ->Rebin(nRebin);
    if (doData) hist_data->Rebin(nRebin);
  }

  //if (bOverflow && name!="SSV" && name!="SSVHP") {
  fix(hist_b);
  fix(hist_c);
  fix(hist_gsplit);
  fix(hist_l);
  if (doData) fix(hist_data);
  //}

  TH1D* histo_tot = (TH1D*) hist_b->Clone();
  //histo_tot->Sumw2();
  histo_tot ->Add(hist_c);
  histo_tot ->Add(hist_gsplit);
  histo_tot ->Add(hist_l);

  TH1D *hist_b_ext, *hist_c_ext, *hist_gsplit_ext, *hist_l_ext, *hist_data_ext;
  TFile *myFile_ext;

  if (extNorm) {
    myFile_ext = TFile::Open(filename_ext,"READ") ;
    myFile_ext->cd();
    hist_b_ext                = (TH1D*)myFile_ext->Get("QCD__"+name+"_b");
    hist_c_ext                = (TH1D*)myFile_ext->Get("QCD__"+name+"_c");
    hist_gsplit_ext           = (TH1D*)myFile_ext->Get("QCD__"+name+"_bfromg");
    hist_l_ext                = (TH1D*)myFile_ext->Get("QCD__"+name+"_l");
    if (doData) hist_data_ext = (TH1D*)myFile_ext->Get("DATA__"+name+"_data");

    fix(hist_b_ext);
    fix(hist_c_ext);
    fix(hist_gsplit_ext);
    fix(hist_l_ext);
    if (doData) fix(hist_data_ext);
  }

  if (doData) {
    float scale_f = ( hist_data->Integral() )/( histo_tot->Integral() ) ;
    if (extNorm) scale_f = ( hist_data_ext->Integral() )/( hist_b_ext->Integral() + hist_c_ext->Integral() + hist_gsplit_ext->Integral() + hist_l_ext->Integral() ) ;
    cout << "scale_f = " << scale_f << endl;
    hist_b       ->Scale(scale_f);
    hist_c       ->Scale(scale_f);
    hist_gsplit  ->Scale(scale_f);
    hist_l       ->Scale(scale_f);
    histo_tot    ->Scale(scale_f);
  }

  beautify(hist_c     , 8     , 1, 1) ;
  beautify(hist_b     , 2     , 1, 1) ;
  beautify(hist_gsplit, 7     , 1, 1) ;
  beautify(hist_l     , 4     , 1, 1) ;
  beautify(histo_tot  , 0     , 0, 0) ;
  if (doData) beautify(hist_data  , 1     , 0, 1) ;

  THStack *stack = new THStack("stack","");
  stack->Add(hist_b);
  stack->Add(hist_gsplit);
  stack->Add(hist_c);
  stack->Add(hist_l);

  TH1D* histo_ratio;
  if (doData) {
    histo_ratio = (TH1D*) hist_data->Clone();
    histo_ratio->SetName("histo_ratio");
    histo_ratio->SetTitle("");

    histo_ratio->Divide(histo_tot);
  }

  TCanvas *c1 = new TCanvas("c1", "c1",1200,800);
  c1->SetFillColor(10);
  c1->cd();

  TPad* pad0 = new TPad("pad0", "pad0",0,0.25,1.0,0.98);
  pad0 ->Draw();
  pad0 ->cd();

  pad0->SetLogy(log);

  if (doData) {
    if (hist_data->GetMaximum() > histo_tot->GetMaximum() ) histo_tot->SetMaximum( hist_data->GetMaximum()*1.1);

    hist_data->SetMarkerStyle(20);
    hist_data->SetMarkerSize(0.75);
    hist_data->SetLineWidth(2);
  }

  //if (name=="jet_phi" || name=="sv_phi" || name=="muon_phi") {
  if (log) histo_tot->SetMinimum(0.1);
  else     histo_tot->SetMinimum(0.);
  //}

  histo_tot->GetXaxis()->SetTitle(name);
  histo_tot->GetYaxis()->SetTitle("Entries");
  histo_tot->SetTitleOffset(0.75,"Y");
  histo_tot->GetYaxis()->SetLabelSize( 0.05 );
  histo_tot->GetYaxis()->SetTitleSize( 0.06 );

  histo_tot->Draw("hist");

  stack->Draw("histSAME");

  if (doData) hist_data->Draw("SAMEE");

  gPad->RedrawAxis();

  int move_legend=0;
  if (name=="jet_phi" || name=="sv_phi" || name=="muon_phi" ) move_legend=1;
  if (log && name=="sv_en_ratio" ) move_legend=1;
  TLegend* qw;
  if (move_legend==1) {
    qw =  new TLegend(0.45,0.25,0.70,0.52);
  }
  else qw =  new TLegend(0.7,0.73,0.95,1.);
  qw->SetTextFont(42);
  //qw->SetTextSize(0.04);

  //Legend
  if (doData) qw->AddEntry(hist_data,     datacaption,     "pl");
  qw->AddEntry(hist_b,        "b quark"           ,         "f");
  qw->AddEntry(hist_gsplit,   "b from gluon splitting"     ,"f");
  qw->AddEntry(hist_c,        "c quark"           ,         "f");
  qw->AddEntry(hist_l,        "uds quark or gluon"     ,    "f");

  qw->SetFillColor(0);
  qw->Draw();

  TLatex *latex = new TLatex();
  latex->SetNDC();
  latex->SetTextSize(0.055);
  latex->SetTextFont(42);
  latex->SetTextAlign(13);
  latex->DrawLatex(0.10, 0.96, title);

  c1->cd();

  if (doData) {
    TPad* pad1 = new TPad("pad1", "pad1",0,0.,1.0,0.32);
    pad1->Draw();
    pad1->cd();
    gPad->SetBottomMargin(0.375);
    gPad->SetGridy();

    histo_ratio->SetMarkerStyle(20);
    histo_ratio->SetMarkerSize(0.75);
    histo_ratio->SetLineWidth(2);

    histo_ratio->GetYaxis()->SetTitle("Data/MC");
    histo_ratio->SetTitleOffset(0.3,"Y");
    histo_ratio->GetXaxis()->SetTitle(histotitle);
    histo_ratio->GetYaxis()->SetNdivisions( 505 );

    double labelsizex=0.12;
    double labelsizey=0.12;
    double titlesizex=0.15;
    double titlesizey=0.14;

    histo_ratio->GetXaxis()->SetLabelSize( labelsizex );
    histo_ratio->GetXaxis()->SetTitleSize( titlesizex );
    histo_ratio->GetYaxis()->SetLabelSize( labelsizey );
    histo_ratio->GetYaxis()->SetTitleSize( titlesizey );

    histo_ratio->SetMinimum(0.4);
    histo_ratio->SetMaximum(1.6);
    histo_ratio->Draw("E1X0");
  }

  c1->cd();

  TString name_plot=name+"_Linear"+format;
  if(log) name_plot=name+"_Log"+format;
  c1->SaveAs(dir4plots+"/"+name_plot);

  if (log && web) {  // save also _Linear for web
    pad0 ->cd();
    pad0->SetLogy(false);
    c1->cd();
    c1->SaveAs(dir4plots+"/"+name+"_Linear"+format);
  }

}

//--------------------------
void DrawTagRate(TString name, TString histotitle, bool log, bool doData){

  TH1D* hist_b;
  TH1D* hist_c;
  TH1D* hist_gsplit;
  TH1D* hist_l;
  TH1D* hist_data;

  TFile *myFile  = TFile::Open(filename,"READ");

  myFile->cd();
  hist_b         = (TH1D*)myFile->Get("QCD__"+name+"_b");
  hist_c         = (TH1D*)myFile->Get("QCD__"+name+"_c");
  hist_gsplit    = (TH1D*)myFile->Get("QCD__"+name+"_bfromg");
  hist_l         = (TH1D*)myFile->Get("QCD__"+name+"_l");
  if (doData) hist_data      = (TH1D*)myFile->Get("DATA__"+name+"_data");

  TH1D* histo_tot = (TH1D*) hist_b->Clone();
  //histo_tot->Sumw2();
  histo_tot ->Add(hist_c);
  histo_tot ->Add(hist_gsplit);
  histo_tot ->Add(hist_l);

  if (doData) {
    float scale_f = (hist_data->Integral())/(hist_b->Integral() + hist_c ->Integral()+ hist_gsplit->Integral() + hist_l->Integral());
    hist_b       ->Scale(scale_f);
    hist_c       ->Scale(scale_f);
    hist_gsplit  ->Scale(scale_f);
    hist_l       ->Scale(scale_f);
    histo_tot    ->Scale(scale_f);
  }

  int   nbinx = hist_data->GetNbinsX();
  float minx  = hist_data->GetXaxis()->GetXmin();
  float maxx  = hist_data->GetXaxis()->GetXmax();

  TH1D * TagRate_Data    = new TH1D ("TagRate_Data",histotitle,nbinx,minx,maxx);
  TH1D * TagRate_MC      = new TH1D ("TagRate_MC",histotitle,nbinx,minx,maxx);
  TH1D * TagRate_MC_b    = new TH1D ("TagRate_MC_b",histotitle,nbinx,minx,maxx);
  TH1D * TagRate_MC_c    = new TH1D ("TagRate_MC_c",histotitle,nbinx,minx,maxx);
  TH1D * TagRate_MC_udsg = new TH1D ("TagRate_MC_udsg",histotitle,nbinx,minx,maxx);
  TH1D * TagRate_MC_gspl = new TH1D ("TagRate_MC_gspl",histotitle,nbinx,minx,maxx);

  int nbin_max ;
  if (doData) nbin_max = hist_data->GetNbinsX() ;

  for (int ii=0; ii<nbin_max; ii++) {

    float totdata ;
    float val ;
    float err ;
    if (doData) {
      totdata = hist_data->Integral(0,nbin_max+1);
      val = hist_data->Integral(ii+1,nbin_max+1) / totdata;
      err = sqrt(totdata *val*(1-val))/ totdata;
      TagRate_Data->SetBinContent(ii+1, val);
      TagRate_Data->SetBinError(ii+1, err);
    }

    float totmc   = histo_tot->Integral(0,nbin_max+1);
    float valMC = histo_tot->Integral(ii+1,nbin_max+1)/ totmc;
    float errMC = sqrt(totmc *valMC*(1-valMC))/ totmc;

    TagRate_MC->SetBinContent(ii+1,   histo_tot->Integral(ii+1,nbin_max+1) / totmc);
    TagRate_MC->SetBinError(ii+1, errMC  );
    TagRate_MC_b->SetBinContent(ii+1, hist_b->Integral(ii+1,nbin_max+1) / totmc);
    TagRate_MC_c->SetBinContent(ii+1, hist_c->Integral(ii+1,nbin_max+1) / totmc);
    TagRate_MC_udsg->SetBinContent(ii+1, hist_l->Integral(ii+1,nbin_max+1) / totmc);
    TagRate_MC_gspl->SetBinContent(ii+1, hist_gsplit->Integral(ii+1,nbin_max+1) / totmc);

  }

  double titleoffsety=0.2;
  double titlesizex=0.17;
  double titlesizey=0.2;
  double labelsizex=0.14;
  double labelsizey=0.12;

  if (doData) {
    TagRate_Data  ->GetYaxis()->SetLabelSize(labelsizey);
    TagRate_Data  ->GetYaxis()->SetTitleSize(titlesizey);
    TagRate_Data  ->GetYaxis()->SetTitleOffset(titleoffsety);
  }

  TagRate_MC     ->GetYaxis()->SetLabelSize(labelsizey);
  TagRate_MC     ->GetYaxis()->SetTitleSize(titlesizey);
  TagRate_MC     ->GetYaxis()->SetTitleOffset(titleoffsety);
  TagRate_MC_b   ->GetYaxis()->SetLabelSize(labelsizey);
  TagRate_MC_b   ->GetYaxis()->SetTitleSize(titlesizey);
  TagRate_MC_b   ->GetYaxis()->SetTitleOffset(titleoffsety);

  // MAKE DATA/MC RATIO

  TH1D* histo_ratio;
  if (doData) {
    histo_ratio = (TH1D*) TagRate_Data->Clone();
    histo_ratio->SetName("histo0_ratio");
    histo_ratio->SetTitle("");
    histo_ratio->Divide(TagRate_MC);

    TagRate_Data->SetMarkerStyle(20);
    TagRate_Data->SetMarkerSize(0.75);
    TagRate_Data->GetXaxis()->SetTitle();
  }

  // SET COLORS
  TagRate_MC->SetLineColor(2);
  TagRate_MC_b->SetFillColor(2);
  TagRate_MC_c->SetFillColor(8);
  TagRate_MC_gspl->SetFillColor(7);
  TagRate_MC_udsg->SetFillColor(4);

  // DO STACK
  THStack* hs= new THStack();

  hs->Add(TagRate_MC_b);
  hs->Add(TagRate_MC_gspl);
  hs->Add(TagRate_MC_c);
  hs->Add(TagRate_MC_udsg);

  // SET COSMETICS
  gStyle->SetOptTitle(0);

  // CREATE CANVAS
  TCanvas *c1 = new TCanvas("c1", "c1",10,32,782,552);
  c1->cd();

  // FIRST MC & DATA
  TPad *c1_1 = new TPad("pad0", "pad0",0,0.25,1.0,0.98);
  c1_1->Draw();
  c1_1->cd();
  c1_1->SetLogy(log);

  if (doData)
    if (TagRate_Data->GetMaximum() > hs->GetMaximum() ) {
      hs->SetMaximum(TagRate_Data->GetMaximum()*1.1 );
    }
  hs->Draw("hist");

  hs->GetHistogram()->SetTitleSize(0.08,"Y");
  hs->GetHistogram()->SetTitleOffset(0.55,"Y");
  hs->GetHistogram()->GetYaxis()->SetTitle("Tag Rate");
  hs->GetHistogram()->GetXaxis()->SetTitle(histotitle);

  if (doData) TagRate_Data->Draw("e same");
  // ADD LEGEND
  TLegend* qw = 0;
  qw = new TLegend(0.6,0.73,0.95,1.);
  if (doData) qw->AddEntry(TagRate_Data,        datacaption                     ,"p");
  qw->AddEntry(TagRate_MC_b,        "b quark"                  ,"f");
  qw->AddEntry(TagRate_MC_gspl,     "b from gluon splitting"   ,"f");
  qw->AddEntry(TagRate_MC_c,        "c quark"                  ,"f");
  qw->AddEntry(TagRate_MC_udsg,     "uds quark or gluon"     ,"f");

  qw->SetFillColor(0);
  qw->Draw();

  TLatex *latex = new TLatex();
  latex->SetNDC();
  latex->SetTextSize(0.055);
  latex->SetTextFont(42); //22

  latex->SetTextAlign(13);
  latex->DrawLatex(0.08, 0.96, title);

  c1->cd();

  if (doData) {
    TPad* pad1 = new TPad("pad1", "pad1",0,0.,1.0,0.32);
    pad1->Draw();
    pad1->cd();

    gPad->SetBottomMargin(0.375);
    gPad->SetGridy();

    histo_ratio->SetMarkerStyle(20);
    histo_ratio->SetMarkerSize(0.75);
    histo_ratio->SetLineWidth(2);

    histo_ratio->GetYaxis()->SetTitle("Data/MC");
    histo_ratio->GetXaxis()->SetTitle(histotitle);
    histo_ratio->GetYaxis()->SetNdivisions( 505 );

    histo_ratio->GetXaxis()->SetLabelSize( labelsizex);
    histo_ratio->GetXaxis()->SetTitleSize( titlesizex );

    histo_ratio->SetMinimum(0.4);
    histo_ratio->SetMaximum(1.6);
    histo_ratio->Draw("E1X0");
  }

  c1->cd();

  TString name_plot="tag_"+name+"_Linear"+format;
  if(log) name_plot="tag_"+name+"_Log"+format;
  c1->SaveAs(dir4plots+"/"+name_plot);

  if (log && web) {  // save also _Linear for web
    c1_1 ->cd();
    c1_1->SetLogy(false);
    c1->cd();
    c1->SaveAs(dir4plots+"/tag_"+name+"_Linear"+format);
  }
}

//--------------------------
void Draw2DPlot(TString name, TString histotitle, TString titleX, TString titleY, bool log, bool doData) {

  TH2F* hist_b;
  TH2F* hist_c;
  TH2F* hist_gsplit;
  TH2F* hist_l;
  TH2F* hist_data;

  TFile *myFile     = TFile::Open(filename,"READ");

  myFile->cd();
  hist_b         = (TH2F*)myFile->Get(name+"_b");
  hist_c         = (TH2F*)myFile->Get(name+"_c");
  hist_gsplit    = (TH2F*)myFile->Get(name+"_bfromg");
  hist_l         = (TH2F*)myFile->Get(name+"_l");
  hist_data      = (TH2F*)myFile->Get(name+"_data");

  TH2F* histo_tot = (TH2F*) hist_b->Clone();
  histo_tot ->Add(hist_c);
  histo_tot ->Add(hist_gsplit);
  histo_tot ->Add(hist_l);

  float scale_f = (hist_data->Integral())/(hist_b->Integral() + hist_c ->Integral()+ hist_gsplit->Integral() + hist_l->Integral());

  if (doData) {
    hist_b       ->Scale(scale_f);
    hist_c       ->Scale(scale_f);
    hist_gsplit  ->Scale(scale_f);
    hist_l       ->Scale(scale_f);
    histo_tot    ->Scale(scale_f);
  }

  TProfile * pro_mc = histo_tot->ProfileX(name+"_tot");
  TProfile * pro_mc_b = hist_b->ProfileX();
  TProfile * pro_mc_c = hist_c->ProfileX();
  TProfile * pro_mc_udsg = hist_l->ProfileX();
  TProfile * pro_mc_gspl = hist_gsplit->ProfileX();
  TProfile * pro_data = hist_data->ProfileX();

  // SET COLORS
  pro_mc->SetLineColor(1);
  pro_mc_b->SetLineColor(2);
  pro_mc_c->SetLineColor(8);
  pro_mc_udsg->SetLineColor(4);
  pro_mc_gspl->SetLineColor(7);

  pro_data->SetMarkerStyle(20);
  pro_data->SetMarkerSize(0.75);

  pro_mc_gspl->GetXaxis()->SetTitle(titleX);
  pro_mc_gspl->GetYaxis()->SetTitle(titleY);

  // SET COSMETICS
  pro_data->SetMarkerStyle(20);
  pro_data->SetMarkerSize(0.75);
  //pro_mc_gspl->GetXaxis()->SetTitle();
  //pro_mc_gspl->GetYaxis()->SetTitle();

  Double_t titleoffsetx=0.8;
  Double_t titleoffsety=0.8;
  Double_t titlesizex=0.05;
  Double_t titlesizey=0.05;
  Double_t labelsizex=0.035;
  Double_t labelsizey=0.035;

  pro_data->GetYaxis()->SetLabelSize(labelsizey);
  pro_data->GetYaxis()->SetTitleSize(titlesizey);
  pro_data->GetYaxis()->SetTitleOffset(titleoffsety);
  pro_mc->GetYaxis()->SetLabelSize(labelsizey);
  pro_mc->GetYaxis()->SetTitleSize(titlesizey);
  pro_mc->GetYaxis()->SetTitleOffset(titleoffsety);
  pro_mc_b->GetYaxis()->SetLabelSize(labelsizey);
  pro_mc_b->GetYaxis()->SetTitleSize(titlesizey);
  pro_mc_b->GetYaxis()->SetTitleOffset(titleoffsety);
  pro_mc_c->GetYaxis()->SetLabelSize(labelsizey);
  pro_mc_c->GetYaxis()->SetTitleSize(titlesizey);
  pro_mc_c->GetYaxis()->SetTitleOffset(titleoffsety);

  pro_mc_gspl->GetYaxis()->SetLabelSize(labelsizey);
  pro_mc_gspl->GetYaxis()->SetTitleSize(titlesizey);
  pro_mc_gspl->GetYaxis()->SetTitleOffset(titleoffsety);

  pro_mc_udsg->GetYaxis()->SetLabelSize(labelsizey);
  pro_mc_udsg->GetYaxis()->SetTitleSize(titlesizey);
  pro_mc_udsg->GetYaxis()->SetTitleOffset(titleoffsety);



  pro_data->GetXaxis()->SetLabelSize(labelsizex);
  pro_data->GetXaxis()->SetTitleSize(titlesizex);
  pro_data->GetXaxis()->SetTitleOffset(titleoffsetx);
  pro_mc->GetXaxis()->SetLabelSize(labelsizex);
  pro_mc->GetXaxis()->SetTitleSize(titlesizex);
  pro_mc->GetXaxis()->SetTitleOffset(titleoffsetx);
  pro_mc_b->GetXaxis()->SetLabelSize(labelsizex);
  pro_mc_b->GetXaxis()->SetTitleSize(titlesizex);
  pro_mc_b->GetXaxis()->SetTitleOffset(titleoffsetx);
  pro_mc_c->GetXaxis()->SetLabelSize(labelsizex);
  pro_mc_c->GetXaxis()->SetTitleSize(titlesizex);
  pro_mc_c->GetXaxis()->SetTitleOffset(titleoffsetx);

  pro_mc_gspl->GetXaxis()->SetLabelSize(labelsizex);
  pro_mc_gspl->GetXaxis()->SetTitleSize(titlesizex);
  pro_mc_gspl->GetXaxis()->SetTitleOffset(titleoffsetx);

  pro_mc_udsg->GetXaxis()->SetLabelSize(labelsizex);
  pro_mc_udsg->GetXaxis()->SetTitleSize(titlesizex);
  pro_mc_udsg->GetXaxis()->SetTitleOffset(titleoffsetx);

  TCanvas *canvas = new TCanvas("c1", "c1",10,32,782,552);
  canvas->cd();

  float maxhist= pro_mc_gspl->GetMaximum();
  if (pro_mc_b->GetMaximum() > maxhist) maxhist = pro_mc_b->GetMaximum()*1.1;
  if (pro_mc_c->GetMaximum() > maxhist) maxhist = pro_mc_c->GetMaximum()*1.1;
  if (pro_mc_udsg->GetMaximum() > maxhist) maxhist = pro_mc_udsg->GetMaximum()*1.1;
  if (pro_mc->GetMaximum() > maxhist) maxhist = pro_mc->GetMaximum()*1.1;
  if (pro_data->GetMaximum() > maxhist) maxhist = pro_data->GetMaximum()*1.1;

  float minhist= pro_mc_gspl->GetMinimum();
  if (pro_mc_b->GetMinimum() < minhist) minhist = pro_mc_b->GetMinimum()*0.9;
  if (pro_mc_c->GetMinimum() < minhist) minhist = pro_mc_c->GetMinimum()*0.9;
  if (pro_mc_udsg->GetMinimum() < minhist) minhist = pro_mc_udsg->GetMinimum()*0.9;
  if (pro_mc->GetMinimum() < minhist) minhist = pro_mc->GetMinimum()*0.9;
  if (pro_data->GetMinimum() < minhist) minhist = pro_data->GetMinimum()*0.9;

  if (maxhist> pro_mc_gspl->GetMaximum()) pro_mc_gspl->SetMaximum(maxhist);
  if (pro_mc_gspl->GetMinimum() >minhist) pro_mc_gspl->SetMinimum(minhist);

  pro_mc_gspl->Draw("hist");
  pro_mc_b->Draw("hist,same");
  pro_mc_c->Draw("hist,same");
  pro_mc_udsg->Draw("hist,same");
  pro_mc->Draw("hist,same");
  pro_data->Draw("e,same");

  TLegend* qw = 0;
  qw =  new TLegend(0.6,0.73,0.95,1.);
  qw->SetHeader(histotitle) ;
  qw->AddEntry(pro_data,        datacaption              ,"p") ;
  qw->AddEntry(pro_mc,          "total "                 ,"l") ;
  qw->AddEntry(pro_mc_b,        "b quark"                ,"l") ;
  qw->AddEntry(pro_mc_gspl,     "b from gluon splitting" ,"l") ;
  qw->AddEntry(pro_mc_c,        "c quark"                ,"l") ;
  qw->AddEntry(pro_mc_udsg,     "uds quark or gluon"     ,"l") ;

  qw->SetFillColor(0);
  qw->Draw();


  TString name_plot=name+"_Linear"+format;
  if(log) name_plot=name+"_Log"+format;
  canvas->SaveAs(dir4plots+"/"+name_plot);

}
