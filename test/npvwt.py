#!/usr/bin/env python

import ROOT

files_data = [ 'JetHT__Run2015B-PromptReco-v1__AODSIM.root' ]
files_mc = [
'QCD_Pt_1000to1400_TuneCUETP8M1_13TeV_pythia8__RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v2__AODSIM.root',
'QCD_Pt_1400to1800_TuneCUETP8M1_13TeV_pythia8__RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1__AODSIM.root',
'QCD_Pt_1800to2400_TuneCUETP8M1_13TeV_pythia8__RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1__AODSIM.root',
'QCD_Pt_2400to3200_TuneCUETP8M1_13TeV_pythia8__RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1__AODSIM.root',
'QCD_Pt_300to470_TuneCUETP8M1_13TeV_pythia8__RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1__AODSIM.root',
'QCD_Pt_3200toInf_TuneCUETP8M1_13TeV_pythia8__RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1__AODSIM.root',
'QCD_Pt_470to600_TuneCUETP8M1_13TeV_pythia8__RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v2__AODSIM.root',
'QCD_Pt_600to800_TuneCUETP8M1_13TeV_pythia8__RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v2__AODSIM.root',
'QCD_Pt_800to1000_TuneCUETP8M1_13TeV_pythia8__RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v2__AODSIM.root',
]

hnpv_data = ROOT.TH1F("hnpv_data", ";N(PV);;", 60,0.,60.)
hnpv_mc_unw = ROOT.TH1F("hnpv_mc_unw", ";N(PV);;", 60,0.,60.)
hnpv_mc_rew = ROOT.TH1F("hnpv_mc_rew", ";N(PV);;", 60,0.,60.)
hpvwt_data_mc = ROOT.TH1F("hpvwt_data_mc", ";PV weight;;", 60,0.,60.) 

for f in files_data:
  fin = ROOT.TFile.Open(f, "READ")
  h = fin.Get("/btagval/h1_nPV_data")
  integral = h.Integral()
  h.Scale(1./integral)
  hnpv_data.Add(h)
  fin.Close()
  
for f in files_mc: 
  fin = ROOT.TFile.Open(f, "READ")
  h = fin.Get("/btagval/h1_nPV_mc_unw")
  hnpv_mc_unw.Add(h)
  integral = hnpv_mc_unw.Integral()
  hnpv_mc_unw.Scale(1./integral)
  fin.Close()

nbins = hnpv_data.GetNbinsX()
for ibin in xrange(1, nbins+1):
  ndata = hnpv_data.GetBinContent(ibin)
  nmc = hnpv_mc_unw.GetBinContent(ibin)
  wt = ndata/nmc if nmc else ndata 
  hpvwt_data_mc.SetBinContent(ibin, wt)
  hnpv_mc_rew.SetBinContent(ibin, nmc*wt)
  print "bin", ibin , " wt", wt

fout = ROOT.TFile("hnpv_data_mc_RunIISpring15_50ns_MINIAOD.root", "RECREATE")
fout.cd()
hnpv_data.Write()
hnpv_mc_unw.Write()
hnpv_mc_rew.Write()
hpvwt_data_mc.Write()
fout.Close()

hnpv_mc_unw.SetLineStyle(2)
hnpv_mc_unw.SetLineColor(2)

c0 = ROOT.TCanvas()
c0.cd()
hnpv_data.Draw("histe0")
hnpv_mc_unw.Draw("histsame")

leg = ROOT.TLegend(0.6,0.7,0.88,0.88)
leg.AddEntry(hnpv_data, "data")
leg.AddEntry(hnpv_mc_unw, "QCD MC")
leg.Draw()

c0.SaveAs("npv_data_mc_unw_50ns__MINIAOD.pdf")
  
c1 = ROOT.TCanvas()
c1.cd()
hnpv_data.Draw("histe0")
hnpv_mc_rew.Draw("histsame")

leg = ROOT.TLegend(0.6,0.7,0.88,0.88)
leg.AddEntry(hnpv_data, "data")
leg.AddEntry(hnpv_mc_rew, "QCD MC")
leg.Draw()

c1.SaveAs("npv_data_mc_rew_50ns__MINIAOD.pdf")
  
                                                                               
