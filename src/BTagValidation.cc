// -*- C++ -*-
//
// Package:    BTagValidation
// Class:      BTagValidation
//
/**\class BTagValidation BTagValidation.cc RecoBTag/BTagValidation/src/BTagValidation.cc

Description: [one line class summary]

Implementation:
[Notes on implementation]
*/
//
// Original Author:  Devdatta Majumder,13 2-054,+41227671675,
//         Created:  Fri May 17 13:56:04 CEST 2013
// $Id: BTagValidation.cc,v 1.1 2013/05/18 11:54:42 devdatta Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "RecoBTag/PerformanceMeasurements/interface/JetInfoBranches.h"
#include "RecoBTag/PerformanceMeasurements/interface/EventInfoBranches.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include <TString.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TLorentzVector.h>

#include <string>
#include <map>
#include <vector>

//
// class declaration
//

class BTagValidation : public edm::EDAnalyzer {
  public:
    explicit BTagValidation(const edm::ParameterSet&);
    ~BTagValidation();

    static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

  private:
    virtual void beginJob();
    virtual void analyze(const edm::Event&, const edm::EventSetup&);
    virtual void endJob();

    virtual void beginRun(edm::Run const&, edm::EventSetup const&);
    virtual void endRun(edm::Run const&, edm::EventSetup const&);
    virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
    virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);

    bool passTrigger();

    //// Manage histograms
    void AddHisto(TString name, TString title, int nbins, double min, double max);
    void AddHisto2D(TString name, TString title, int nbins, double min, double max, int nbins2, double min2, double max2);

    template <class Type>
    void FillHisto(TString name, int flavour, bool isGS, Type value, double weight);

    template <class Type1, class Type2>
    void FillHisto2D(TString name, int flavour, bool isGS, Type1 value, Type2 value2, double weight);

    // ----------member data ---------------------------
    EventInfoBranches EvtInfo;
    JetInfoBranches   FatJetInfo;
    JetInfoBranches   SubJetInfo;

    TChain* JetTree;

    std::map<TString, TH1D*> HistoBtag_map;
    std::map<TString, TH2D*> HistoBtag2D_map;

    bool isData;

    edm::Service<TFileService> fs;

    TH1D *h1_CutFlow;

    //// Configurables
    const int                       maxEvents_;
    const int                       reportEvery_;
    const bool                      useJetProbaTree_;
    const std::string               inputTTree_;
    const std::vector<std::string>  inputFiles_;
    const std::vector<std::string>  triggerSelection_;
    const std::vector<std::string>  triggerPathNames_;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
BTagValidation::BTagValidation(const edm::ParameterSet& iConfig) :

  maxEvents_(iConfig.getParameter<int>("MaxEvents")),
  reportEvery_(iConfig.getParameter<int>("ReportEvery")),
  useJetProbaTree_(iConfig.getParameter<bool>("UseJetProbaTree")),
  inputTTree_(iConfig.getParameter<std::string>("InputTTree")),
  inputFiles_(iConfig.getParameter<std::vector<std::string> >("InputFiles")),
  triggerSelection_(iConfig.getParameter<std::vector<std::string> >("TriggerSelection")),
  triggerPathNames_(iConfig.getParameter<std::vector<std::string> >("TriggerPathNames"))

{
  //now do what ever initialization is needed
  isData = true;
}


BTagValidation::~BTagValidation() {

  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called once each job just before starting event loop  ------------
void BTagValidation::beginJob() {

  JetTree = new TChain(inputTTree_.c_str());

  for(unsigned i=0; i<inputFiles_.size(); ++i)
    JetTree->Add(inputFiles_.at(i).c_str());

  //--------------------------------------------------------
  EvtInfo.ReadTree(JetTree);
  EvtInfo.ReadPatMuonTree(JetTree);
  //--------------------------------------------------------
  FatJetInfo.ReadTree(JetTree,"FatJetInfo");
  FatJetInfo.ReadFatJetSpecificTree(JetTree,"FatJetInfo");
  //--------------------------------------------------------
  SubJetInfo.ReadTree(JetTree,"JetInfo");
  SubJetInfo.ReadSubJetSpecificTree(JetTree,"JetInfo");
  //--------------------------------------------------------
  if(useJetProbaTree_)
  {
    EvtInfo.ReadJPTree(JetTree);
    FatJetInfo.ReadJPTree(JetTree,"FatJetInfo");
    SubJetInfo.ReadJPTree(JetTree,"JetInfo");
  }

  h1_CutFlow = fs->make<TH1D>("h1_CutFlow", "h1_CutFlow", 2, -0.5, 1.5);

  double PtMax = 1000.;
  double pi=TMath::Pi();
  AddHisto("FatJet_pt_all" ,"p_{T} of all fat jets"             ,PtMax/10  ,0      ,PtMax );
  AddHisto("FatJet_pt_sv"  ,"p_{T} of fat jets containing a SV" ,PtMax/10  ,0      ,PtMax);
  AddHisto("FatJet_eta"    ,"#eta of all fat jets"              ,50        ,-2.5   ,2.5);
  AddHisto("FatJet_phi"    ,"#phi of all fat jets"              ,40        ,-1.*pi ,pi);

}

// ------------ method called for each event  ------------
void BTagValidation::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

  using namespace edm;

  if(JetTree == 0) return;

  Long64_t nEntries = JetTree->GetEntries();
  if(maxEvents_>=0) nEntries = maxEvents_;

  //---------------------------- Start event loop ---------------------------------------//
  for(Long64_t iEntry = 0; iEntry < nEntries; ++iEntry)
  {
    JetTree->GetEntry(iEntry);
    if((iEntry%reportEvery_) == 0) edm::LogInfo("EventNumber") << "Processing event " << iEntry << " of " << nEntries;

    if(EvtInfo.Run < 0) isData = false;

    double ww(1); //// To be filled by PU weight for MC

    h1_CutFlow->Fill(0.,ww); // count all events

    if( !passTrigger() ) continue; // apply trigger selection

    h1_CutFlow->Fill(1.,ww); // count events passing trigger selection

    if(FatJetInfo.nJet <= 0) continue; // require at least 1 jet in the event

    double jetCone(0.8);

    for(int iJet = 0; iJet < FatJetInfo.nJet; ++iJet)
    {
      int flav = FatJetInfo.Jet_flavour[iJet];
      TLorentzVector jet_p4;
      jet_p4.SetPtEtaPhiM(FatJetInfo.Jet_pt[iJet], FatJetInfo.Jet_eta[iJet], FatJetInfo.Jet_phi[iJet], FatJetInfo.Jet_mass[iJet]);

      bool isGluonSplit(false);
      int nMatchedBHadrons(0);
      for (int iB = 0; iB < EvtInfo.nBHadrons; ++iB)
      {
        if(EvtInfo.BHadron_hasBdaughter[iB]==1) continue; // skip excited B hadrons
        TLorentzVector bhad_p4;
        bhad_p4.SetPtEtaPhiM(EvtInfo.BHadron_pT[iB], EvtInfo.BHadron_eta[iB], EvtInfo.BHadron_phi[iB], EvtInfo.BHadron_mass[iB]);
        if( jet_p4.DeltaR(bhad_p4) < jetCone ) ++nMatchedBHadrons;
      }
      if ( nMatchedBHadrons > 1 ) isGluonSplit = true;

      FillHisto("FatJet_pt_all", flav, isGluonSplit , jet_p4.Pt() , ww);
      FillHisto("FatJet_eta", flav, isGluonSplit , jet_p4.Eta() , ww);
      FillHisto("FatJet_phi", flav, isGluonSplit , jet_p4.Phi() , ww);
      if (FatJetInfo.Jet_SV_multi[iJet] > 0) FillHisto("FatJet_pt_sv", flav, isGluonSplit , jet_p4.Pt() , ww);

    }
  }
  //----------------------------- End event loop ----------------------------------------//
}

// ------------ method called once each job just after ending the event loop  ------------
void BTagValidation::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void BTagValidation::beginRun(edm::Run const&, edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a run  ------------
void BTagValidation::endRun(edm::Run const&, edm::EventSetup const&) {
}

// ------------ method called when starting to processes a luminosity block  ------------
void BTagValidation::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a luminosity block  ------------
void BTagValidation::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) {
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void BTagValidation::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

// ------------------------------------------------------------------------------
void BTagValidation::AddHisto(TString name, TString title, int nbins, double min, double max)  {

  TH1D* h_b      = fs->make<TH1D>(name+"_b"     ,title+"_b"     ,nbins,min,max);
  TH1D* h_bfromg = fs->make<TH1D>(name+"_bfromg",title+"_bfromg",nbins,min,max);
  TH1D* h_c      = fs->make<TH1D>(name+"_c"     ,title+"_c"     ,nbins,min,max);
  TH1D* h_l      = fs->make<TH1D>(name+"_l"     ,title+"_l"     ,nbins,min,max);
  TH1D* h_data   = fs->make<TH1D>(name+"_data"  ,title+"_data"  ,nbins,min,max);

  h_b      ->Sumw2();
  h_bfromg ->Sumw2();
  h_c      ->Sumw2();
  h_l      ->Sumw2();
  h_data   ->Sumw2();

  HistoBtag_map[name+"_b"]      = h_b;
  HistoBtag_map[name+"_bfromg"] = h_bfromg;
  HistoBtag_map[name+"_c"]      = h_c;
  HistoBtag_map[name+"_l"]      = h_l;
  HistoBtag_map[name+"_data"]   = h_data;
}

// ------------------------------------------------------------------------------
void BTagValidation::AddHisto2D(TString name, TString title, int nbins, double min, double max, int nbins2, double min2, double max2)
{
  TH2D* h_b      = fs->make<TH2D>(name+"_b"     ,title+"_b"     ,nbins,min,max,nbins2,min2,max2);
  TH2D* h_bfromg = fs->make<TH2D>(name+"_bfromg",title+"_bfromg",nbins,min,max,nbins2,min2,max2);
  TH2D* h_c      = fs->make<TH2D>(name+"_c"     ,title+"_c"     ,nbins,min,max,nbins2,min2,max2);
  TH2D* h_l      = fs->make<TH2D>(name+"_l"     ,title+"_l"     ,nbins,min,max,nbins2,min2,max2);
  TH2D* h_data   = fs->make<TH2D>(name+"_data"  ,title+"_data"  ,nbins,min,max,nbins2,min2,max2);

  h_b        ->Sumw2();
  h_bfromg   ->Sumw2();
  h_c        ->Sumw2();
  h_l        ->Sumw2();
  h_data     ->Sumw2();

  HistoBtag2D_map[name+"_b"]      = h_b;
  HistoBtag2D_map[name+"_bfromg"] = h_bfromg;
  HistoBtag2D_map[name+"_c"]      = h_c;
  HistoBtag2D_map[name+"_l"]      = h_l;
  HistoBtag2D_map[name+"_data"]   = h_data;
}

// ------------------------------------------------------------------------------
template <class Type>
void BTagValidation::FillHisto(TString name, int flavour, bool isGS, Type value, double weight)
{
  if (!isData){
    if (fabs(flavour)==5 && !isGS)                  HistoBtag_map[name+"_b"]     ->Fill(double(value),weight);
    else if (fabs(flavour)==5 && isGS)              HistoBtag_map[name+"_bfromg"]->Fill(double(value),weight);
    else if (fabs(flavour)==4)                      HistoBtag_map[name+"_c"]     ->Fill(double(value),weight);
    else if (fabs(flavour)< 4 || fabs(flavour)==21) HistoBtag_map[name+"_l"]     ->Fill(double(value),weight);
  }
  else                                              HistoBtag_map[name+"_data"]  ->Fill(double(value));
}

// ------------------------------------------------------------------------------
template <class Type1, class Type2>
void BTagValidation::FillHisto2D(TString name, int flavour, bool isGS, Type1 value, Type2 value2, double weight)
{
  if (!isData){
    if (fabs(flavour)==5 && !isGS)                  HistoBtag2D_map[name+"_b"]     ->Fill(double(value),double(value2),weight);
    else if (fabs(flavour)==5 && isGS)              HistoBtag2D_map[name+"_bfromg"]->Fill(double(value),double(value2),weight);
    else if (fabs(flavour)==4)                      HistoBtag2D_map[name+"_c"]     ->Fill(double(value),double(value2),weight);
    else if (fabs(flavour)< 4 || fabs(flavour)==21) HistoBtag2D_map[name+"_l"]     ->Fill(double(value),double(value2),weight);
  }
  else                                              HistoBtag2D_map[name+"_data"]  ->Fill(double(value),double(value2));
}

// ------------------------------------------------------------------------------
bool BTagValidation::passTrigger()
{
  bool ret = false;

  if(triggerSelection_.size()==0)
    ret = true;
  else
  {
    for(unsigned i=0; i<triggerSelection_.size(); ++i)
    {
      std::vector<std::string>::const_iterator it = std::find(triggerPathNames_.begin(), triggerPathNames_.end(), triggerSelection_.at(i));
      if( it != triggerPathNames_.end() )
      {
        int triggerIdx = ( it - triggerPathNames_.begin() );
        int bitIdx = int(triggerIdx/32);
        if ( EvtInfo.BitTrigger[bitIdx] & ( 1 << (triggerIdx - bitIdx*32) ) )
        {
          ret = true;
          break;
        }
      }
    }
  }

  return ret;
}

//define this as a plug-in
DEFINE_FWK_MODULE(BTagValidation);
