#!/usr/bin/env python

import string, re
from ROOT import *
from array import array


gROOT.SetBatch(kTRUE)
gStyle.SetOptStat(0)
gStyle.SetOptTitle(0)
gStyle.SetTitleFont(42, "XYZ")
gStyle.SetTitleSize(0.06, "XYZ")
gStyle.SetLabelFont(42, "XYZ")
gStyle.SetLabelSize(0.05, "XYZ")
gStyle.SetCanvasBorderMode(0)
gStyle.SetFrameBorderMode(0)
gStyle.SetCanvasColor(kWhite)
gStyle.SetPadTickX(1)
gStyle.SetPadTickY(1)
gStyle.SetPadLeftMargin(0.15)
gStyle.SetPadRightMargin(0.05)
gStyle.SetPadTopMargin(0.05)
gStyle.SetPadBottomMargin(0.15)
gROOT.ForceStyle()


PtBins_b = array('d',[20, 30, 40, 50, 60, 70, 80, 100, 120, 160, 210, 260, 320, 400, 500, 600, 800])

# CSVL
CSVL_SFb_0to2p4 = TF1("CSVL_SFb_0to2p4","0.997942*((1.+(0.00923753*x))/(1.+(0.0096119*x)))", 20.,800.)

CSVL_SFb_errors = TH1D("CSVL_SFb_errors", "CSVL_SFb_errors", 16, PtBins_b)
CSVL_SFb_errors.SetBinContent( 0,(2*0.033299))
CSVL_SFb_errors.SetBinContent( 1,0.033299)
CSVL_SFb_errors.SetBinContent( 2,0.0146768)
CSVL_SFb_errors.SetBinContent( 3,0.013803)
CSVL_SFb_errors.SetBinContent( 4,0.0170145)
CSVL_SFb_errors.SetBinContent( 5,0.0166976)
CSVL_SFb_errors.SetBinContent( 6,0.0137879)
CSVL_SFb_errors.SetBinContent( 7,0.0149072)
CSVL_SFb_errors.SetBinContent( 8,0.0153068)
CSVL_SFb_errors.SetBinContent( 9,0.0133077)
CSVL_SFb_errors.SetBinContent(10,0.0123737)
CSVL_SFb_errors.SetBinContent(11,0.0157152)
CSVL_SFb_errors.SetBinContent(12,0.0175161)
CSVL_SFb_errors.SetBinContent(13,0.0209241)
CSVL_SFb_errors.SetBinContent(14,0.0278605)
CSVL_SFb_errors.SetBinContent(15,0.0346928)
CSVL_SFb_errors.SetBinContent(16,0.0350099)
CSVL_SFb_errors.SetBinContent(17,(2*0.0350099))

# CSVM
CSVM_SFb_0to2p4 = TF1("CSVM_SFb_0to2p4","(0.938887+(0.00017124*x))+(-2.76366e-07*(x*x))", 20.,800.)

CSVM_SFb_errors = TH1D("CSVM_SFb_errors", "CSVM_SFb_errors", 16, PtBins_b)
CSVM_SFb_errors.SetBinContent( 0,(2*0.0415707))
CSVM_SFb_errors.SetBinContent( 1,0.0415707)
CSVM_SFb_errors.SetBinContent( 2,0.0204209)
CSVM_SFb_errors.SetBinContent( 3,0.0223227)
CSVM_SFb_errors.SetBinContent( 4,0.0206655)
CSVM_SFb_errors.SetBinContent( 5,0.0199325)
CSVM_SFb_errors.SetBinContent( 6,0.0174121)
CSVM_SFb_errors.SetBinContent( 7,0.0202332)
CSVM_SFb_errors.SetBinContent( 8,0.0182446)
CSVM_SFb_errors.SetBinContent( 9,0.0159777)
CSVM_SFb_errors.SetBinContent(10,0.0218531)
CSVM_SFb_errors.SetBinContent(11,0.0204688)
CSVM_SFb_errors.SetBinContent(12,0.0265191)
CSVM_SFb_errors.SetBinContent(13,0.0313175)
CSVM_SFb_errors.SetBinContent(14,0.0415417)
CSVM_SFb_errors.SetBinContent(15,0.0740446)
CSVM_SFb_errors.SetBinContent(16,0.0596716)
CSVM_SFb_errors.SetBinContent(17,(2*0.0596716))


def plot_SFb(mass_min, mass_max, n_steps, SFb_mean, SFb_error, xAxisTitle, yAxisTitle, label, outputFilename):

  masses = array('d')
  masses_unc = array('d')

  step = (mass_max-mass_min)/float(n_steps)

  for i in range(0,n_steps+1):
    mass = mass_min+float(i)*step
    masses.append(mass)
    masses_unc.append(mass)

  for i in range(0,n_steps+1):
    masses_unc.append(masses[n_steps-i])

  a_SFb = array('d')
  a_SFb_unc = array('d')

  for i in range(0,len(masses)):
    a_SFb.append(SFb_mean.Eval(masses[i]))

  for i in range(0,len(masses_unc)):
    if(i <= n_steps):
      a_SFb_unc.append(SFb_mean.Eval(masses_unc[i]) - SFb_error.GetBinContent(SFb_error.GetXaxis().FindBin(masses_unc[i])))
    else:
      a_SFb_unc.append(SFb_mean.Eval(masses_unc[i]) + SFb_error.GetBinContent(SFb_error.GetXaxis().FindBin(masses_unc[i])))

  g_SFb = TGraph(len(masses),masses,a_SFb)
  g_SFb.SetLineWidth(2)
  g_SFb.SetLineColor(kRed)
  g_SFb.GetXaxis().SetTitle(xAxisTitle)
  g_SFb.GetYaxis().SetTitle(yAxisTitle)
  g_SFb.GetYaxis().SetRangeUser(0.,1.3)

  g_SFb_unc = TGraph(len(masses_unc),masses_unc,a_SFb_unc)
  g_SFb_unc.SetLineColor(0)
  g_SFb_unc.SetFillColor(kRed)
  g_SFb_unc.SetFillStyle(3013)

  c = TCanvas("c", "",800,600)
  c.cd()

  g_SFb.Draw("AL")
  g_SFb_unc.Draw("F")

  legend = TLegend(.55,.55,.85,.60)
  legend.SetBorderSize(0)
  legend.SetFillColor(0)
  legend.SetFillStyle(0)
  legend.SetTextFont(42)
  legend.SetTextSize(0.04)
  legend.AddEntry(g_SFb_unc, "Uncertainty (stat \oplus syst)","f")
  legend.Draw()

  l1 = TLatex()
  l1.SetTextAlign(12)
  l1.SetTextFont(42)
  l1.SetNDC()
  l1.SetTextSize(0.04)
  l1.DrawLatex(0.18,0.34, "CMS Preliminary")
  l1.DrawLatex(0.18,0.26, "#intLdt = 19.8 fb^{-1}")
  l1.DrawLatex(0.19,0.21, "#sqrt{s} = 8 TeV")
  l1.SetTextSize(0.05)
  l1.DrawLatex(0.57,0.40, label)

  c.SaveAs(outputFilename)


if __name__ == "__main__":
  # CSVL
  plot_SFb(20., 799.99, 100, CSVL_SFb_0to2p4, CSVL_SFb_errors, "p_{T} [GeV]", "SF_{b}", "#splitline{CSVL}{|#eta| < 2.4}", "SFb_CSVL_eta0to2p4.eps")

  # CSVM
  plot_SFb(20., 799.99, 100, CSVM_SFb_0to2p4, CSVM_SFb_errors, "p_{T} [GeV]", "SF_{b}", "#splitline{CSVM}{|#eta| < 2.4}", "SFb_CSVM_eta0to2p4.eps")
