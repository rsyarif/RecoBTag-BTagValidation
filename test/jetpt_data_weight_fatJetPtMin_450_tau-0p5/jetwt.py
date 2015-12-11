#!/usr/bin/env python

import ROOT

files_data = [
#'JetHT__Run2015D-PromptReco-v3__MINIAOD_fatJetPtMin_400.root'
'Final_histograms_btagval_InclusiveJets_noWeight.root'
]
#files_mc = [
#'QCD_Pt_120to170_TuneCUETP8M1_13TeV_pythia8__RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1__MINIAODSIM_fatJetPtMin_425.root',
#'QCD_Pt_170to300_TuneCUETP8M1_13TeV_pythia8__RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2__MINIAODSIM_fatJetPtMin_425.root',
#'QCD_Pt_300to470_TuneCUETP8M1_13TeV_pythia8__RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1__MINIAODSIM_fatJetPtMin_425.root',
#'QCD_Pt_470to600_TuneCUETP8M1_13TeV_pythia8__RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2__MINIAODSIM_fatJetPtMin_425.root',
#'QCD_Pt_600to800_TuneCUETP8M1_13TeV_pythia8__RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2__MINIAODSIM_fatJetPtMin_425.root',
#'QCD_Pt_800to1000_TuneCUETP8M1_13TeV_pythia8__RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2__MINIAODSIM_fatJetPtMin_425.root',
#'QCD_Pt_1000to1400_TuneCUETP8M1_13TeV_pythia8__RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1__MINIAODSIM_fatJetPtMin_425.root',
#'QCD_Pt_1400to1800_TuneCUETP8M1_13TeV_pythia8__RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1__MINIAODSIM_fatJetPtMin_425.root',
#'QCD_Pt_1800to2400_TuneCUETP8M1_13TeV_pythia8__RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1__MINIAODSIM_fatJetPtMin_425.root',
#'QCD_Pt_2400to3200_TuneCUETP8M1_13TeV_pythia8__RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1__MINIAODSIM_fatJetPtMin_425.root',
#'QCD_Pt_3200toInf_TuneCUETP8M1_13TeV_pythia8__RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1__MINIAODSIM_fatJetPtMin_425.root'
#]

jetpt_data = ROOT.TH1D("jetpt_data", ";pT(all jets);;",500,0.,5000.)
jetpt_mc_noweight = ROOT.TH1D("jetpt_mc_noweight",";pT(all jets);;",500,0.,5000.)
jetpt_mc_reweight = ROOT.TH1D("jetpt_mc_reweight",";pT(all jets);;",500,0.,5000.)
jetptweight_mc_data = ROOT.TH1D("jetptweight_mc_data",";pT(all jets);;",500,0.,5000.)

for f in files_data:
  fin = ROOT.TFile.Open(f, "READ")
  h = fin.Get("DATA__h1_fatjet_pt")
  h2 = fin.Get("QCD__h1_fatjet_pt")
  integral = h.Integral()
  integral2 = h2.Integral()
  h.Scale(1./integral)
  h2.Scale(1./integral2)
  jetpt_data.Add(h)
  jetpt_mc_noweight.Add(h2)
  fin.Close()
  
#for f in files_mc: 
#  fin = ROOT.TFile.Open(f, "READ")
#  h = fin.Get("/btagval/h1_fatjet_pt")
#  jetpt_mc_noweight.Add(h)
#  fin.Close()

#integral = jetpt_mc_noweight.Integral()
#jetpt_mc_noweight.Scale(1./integral)

nbins = jetpt_data.GetNbinsX()
for ibin in xrange(1, nbins+1):
  ndata = jetpt_data.GetBinContent(ibin)
  nmc = jetpt_mc_noweight.GetBinContent(ibin)
  wt = ndata/nmc if nmc else ndata 
  jetptweight_mc_data.SetBinContent(ibin, wt)
  jetpt_mc_reweight.SetBinContent(ibin, nmc*wt)
  print "bin", ibin , " wt", wt

fout = ROOT.TFile("jetpt_data_mc_RunIISpring15_25ns_MINIAOD.root", "RECREATE")
fout.cd()
jetpt_data.Write()
jetpt_mc_noweight.Write()
jetpt_mc_reweight.Write()
jetptweight_mc_data.Write()
fout.Close()

jetpt_mc_noweight.SetLineStyle(2)
jetpt_mc_noweight.SetLineColor(2)

c0 = ROOT.TCanvas()
c0.cd()
jetpt_data.Draw("histe0")
jetpt_mc_noweight.Draw("histsame")

leg = ROOT.TLegend(0.6,0.7,0.88,0.88)
leg.AddEntry(jetpt_data, "data")
leg.AddEntry(jetpt_mc_noweight, "QCD MC")
leg.Draw()

c0.SaveAs("jetpt_mc_data_noweight_25ns__MINIAOD.pdf")
  
c1 = ROOT.TCanvas()
c1.cd()
jetpt_data.Draw("histe0")
jetpt_mc_reweight.Draw("histsame")

leg = ROOT.TLegend(0.6,0.7,0.88,0.88)
leg.AddEntry(jetpt_data, "data")
leg.AddEntry(jetpt_mc_reweight, "QCD MC")
leg.Draw()

c1.SaveAs("jetpt_mc_data_reweight_25ns__MINIAOD.pdf")
