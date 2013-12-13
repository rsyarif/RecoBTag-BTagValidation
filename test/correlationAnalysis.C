#include <iostream>
#include "TROOT.h"
#include "TStyle.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TProfile.h"
#include "TGraph.h"
#include "TGraphAsymmErrors.h"
#include "TF1.h"
#include "TLine.h"
#include "TLatex.h"
#include "TLegend.h"
#include "exoStyle.C"


using namespace std;


void plot_eff_NoErrors(const string& fInputFile, const string& fPlot, const double fOP, const string& fTitle, const string& fXAxisTitle, const string& fYAxisTitle, const string& fOutputFile,
                       const double fYmin=0., const double fYmax=1.0, const Int_t fLogy=0, const Double_t fTitleOffsetX=1.0, const Double_t fTitleOffsetY=1.0,
                       const Double_t fLeftMargin=0.12, const Double_t fTopMargin=0.07, const Double_t fPlotWidth=0.8)
{
  gROOT->SetBatch(kTRUE);
  setEXOStyle();
  gStyle->SetGridColor(kGray);
  gStyle->SetOptStat(kFALSE);
  gStyle->SetPadTopMargin(fTopMargin);
  gStyle->SetPadBottomMargin(1.-fTopMargin-fPlotWidth);
  gStyle->SetPadLeftMargin(fLeftMargin);
  gStyle->SetPadRightMargin(1.-fLeftMargin-fPlotWidth);
  gROOT->ForceStyle();

  TFile *file = new TFile(fInputFile.c_str());

  TH1D *h1_total = new TH1D("h1_total","h1_total",4,0.,0.8);
  TH1D *h1_subjet1 = new TH1D("h1_subjet1","h1_subjet1",4,0.,0.8);
  TH1D *h1_subjet2 = new TH1D("h1_subjet2","h1_subjet2",4,0.,0.8);
  TH1D *h1_subjet12 = new TH1D("h1_subjet12","h1_subjet12",4,0.,0.8);

  string bin_strings[4] = {"0to0p2", "0p2to0p4", "0p4to0p6", "0p6to0p8"};

  for(int i=0; i<4; ++i)
  {
     TH2D *h2 = (TH2D*)file->Get((fPlot + bin_strings[i]).c_str());

     int bin = h2->GetXaxis()->FindBin(fOP);

     h1_total->SetBinContent(   i+1,h2->Integral(0,101,0,101));
     h1_subjet1->SetBinContent( i+1,h2->Integral(bin,101,0,101));
     h1_subjet2->SetBinContent( i+1,h2->Integral(0,101,bin,101));
     h1_subjet12->SetBinContent(i+1,h2->Integral(bin,101,bin,101));
  }

  TCanvas *c = new TCanvas("c", "",1000,800);
  c->cd();

  TH2D *bkg = new TH2D("bkg","",100,0.,0.8,100,fYmin,fYmax);
  bkg->GetXaxis()->SetTitle(fXAxisTitle.c_str());
  bkg->GetYaxis()->SetTitle(fYAxisTitle.c_str());
  bkg->SetTitleOffset(fTitleOffsetX,"X");
  bkg->SetTitleOffset(fTitleOffsetY,"Y");
  bkg->Draw();
  c->SetGridx();
  c->SetGridy();

  TGraph *g_eff_subjet1 = new TGraph(4);
  g_eff_subjet1->SetMarkerStyle(20);
  g_eff_subjet1->SetMarkerColor(kBlue+1);
  g_eff_subjet1->SetLineColor(kBlue+1);
  g_eff_subjet1->SetLineStyle(2);
  for(int i=0; i<4; ++i)
    g_eff_subjet1->SetPoint(i,h1_total->GetBinCenter(i+1),h1_subjet1->GetBinContent(i+1)/h1_total->GetBinContent(i+1));

  TGraph *g_eff_subjet2 = new TGraph(4);
  g_eff_subjet2->SetMarkerStyle(21);
  g_eff_subjet2->SetMarkerColor(kGreen+1);
  g_eff_subjet2->SetLineColor(kGreen+1);
  g_eff_subjet2->SetLineStyle(2);
  for(int i=0; i<4; ++i)
    g_eff_subjet2->SetPoint(i,h1_total->GetBinCenter(i+1),h1_subjet2->GetBinContent(i+1)/h1_total->GetBinContent(i+1));

  TGraph *g_eff_subjet12 = new TGraphAsymmErrors(4);
  g_eff_subjet12->SetMarkerStyle(22);
  g_eff_subjet12->SetMarkerColor(kBlack);
  g_eff_subjet12->SetLineColor(kBlack);
  g_eff_subjet12->SetLineStyle(2);
  for(int i=0; i<4; ++i)
    g_eff_subjet12->SetPoint(i,h1_total->GetBinCenter(i+1),h1_subjet12->GetBinContent(i+1)/h1_total->GetBinContent(i+1));

  TGraph *g_eff_subjet12_prod = new TGraph(4);
  g_eff_subjet12_prod->SetMarkerStyle(26);
  g_eff_subjet12_prod->SetMarkerColor(kRed);
  g_eff_subjet12_prod->SetLineColor(kRed);
  g_eff_subjet12_prod->SetLineStyle(2);
  for(int i=0; i<4; ++i)
    g_eff_subjet12_prod->SetPoint(i,h1_total->GetBinCenter(i+1),(h1_subjet1->GetBinContent(i+1)*h1_subjet2->GetBinContent(i+1))/(h1_total->GetBinContent(i+1)*h1_total->GetBinContent(i+1)));

  g_eff_subjet1->Draw("PLsame");
  g_eff_subjet2->Draw("PLsame");
  g_eff_subjet12->Draw("PLsame");
  g_eff_subjet12_prod->Draw("PLsame");

  TLegend *legend = new TLegend(.6,.3,.85,.55);
  legend->SetBorderSize(0);
  legend->SetFillColor(0);
  legend->SetFillStyle(0);
  legend->SetTextFont(42);
  legend->SetTextSize(0.05);
  legend->AddEntry(g_eff_subjet1, "#varepsilon_{1}","lp");
  legend->AddEntry(g_eff_subjet2, "#varepsilon_{2}","lp");
  legend->AddEntry(g_eff_subjet12, "#varepsilon_{1 & 2}","lp");
  legend->AddEntry(g_eff_subjet12_prod, "#varepsilon_{1} #times #varepsilon_{2}","lp");
  legend->Draw();

  TLatex l1;
  l1.SetTextAlign(13);
  l1.SetTextFont(42);
  l1.SetTextSize(0.045);
  l1.SetNDC();
  l1.DrawLatex(fLeftMargin+0.03,0.91, fTitle.c_str());

  l1.SetTextAlign(12);
  l1.SetTextSize(0.05);
  l1.SetTextFont(62);
  l1.DrawLatex(fLeftMargin,0.97, "CMS Simulation Preliminary, #sqrt{s} = 8 TeV");

  c->SetLogy(fLogy);
  c->SaveAs(fOutputFile.c_str());
}


void makePlots()
{
  // CSVL
  // QCD
  plot_eff_NoErrors("LXBatch_Jobs_InclusiveJets_SubJetTagCorrelations_QCD_Pt-300to470/Final_histograms_btagval.root", "QCD__h2_FatJet_Subjet1CSV_Subjet2CSV_dRsubjets",
                    0.244, "#splitline{QCD, CA R=0.8}{Subjet CSVL}",
                    "#DeltaR(subjets)", "Tagging efficiency","Subjet_tag_correlation_SubjetCSVL_QCD.eps", 0, 0.15);

  plot_eff_NoErrors("LXBatch_Jobs_InclusiveJets_SubJetTagCorrelations_QCD_Pt-300to470/Final_histograms_btagval.root", "QCD__h2_FatJet_Subjet1CSV_Subjet2CSV_dRsubjets",
                    0.244, "#splitline{QCD, CA R=0.8}{Subjet CSVL}",
                    "#DeltaR(subjets)", "Tagging efficiency","Subjet_tag_correlation_SubjetCSVL_QCD_Logy.eps", 1e-03, 0.15, 1);

  // CSVM
  // QCD
  plot_eff_NoErrors("LXBatch_Jobs_InclusiveJets_SubJetTagCorrelations_QCD_Pt-300to470/Final_histograms_btagval.root", "QCD__h2_FatJet_Subjet1CSV_Subjet2CSV_dRsubjets",
                    0.679, "#splitline{QCD, CA R=0.8}{Subjet CSVM}",
                    "#DeltaR(subjets)", "Tagging efficiency","Subjet_tag_correlation_SubjetCSVM_QCD.eps", 0, 0.06);

  plot_eff_NoErrors("LXBatch_Jobs_InclusiveJets_SubJetTagCorrelations_QCD_Pt-300to470/Final_histograms_btagval.root", "QCD__h2_FatJet_Subjet1CSV_Subjet2CSV_dRsubjets",
                    0.679, "#splitline{QCD, CA R=0.8}{Subjet CSVM}",
                    "#DeltaR(subjets)", "Tagging efficiency","Subjet_tag_correlation_SubjetCSVM_QCD_Logy.eps", 1e-04, 0.06, 1);

}