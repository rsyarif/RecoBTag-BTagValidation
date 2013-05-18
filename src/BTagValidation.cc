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
// $Id$
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
    virtual void beginJob() ;
    virtual void analyze(const edm::Event&, const edm::EventSetup&);
    virtual void endJob() ;

    virtual void beginRun(edm::Run const&, edm::EventSetup const&);
    virtual void endRun(edm::Run const&, edm::EventSetup const&);
    virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
    virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);

    //// Manage histograms 
    virtual void AddHisto(TString name, TString title,  int nbins, double min, double max) ;  
    virtual void AddHisto2D(TString name,TString title,int nbins,double min,double max,int nbins2,double min2,double max2) ; 

    template <class Type>
    void FillHisto(int flavour, bool isGS, int number, Type value, double weight) ; 

    template <class Type>
    void FillHistoFromMap(TString name, int flavour, bool isGS, Type value, double weight) ; 

    template <class Type1, class Type2>
    void FillHisto2DFromMap(TString name, int flavour, bool isGS, Type1 value, Type2 value2, double weight) ;  

    // ----------member data ---------------------------
    EventInfoBranches EvtInfo_ ; 
    JetInfoBranches   JetInfo_ ;
    JetInfoBranches   FatJetInfo_ ; 

    TChain* JetTree_ ; 
    TChain* FatJetTree_ ; 

    std::vector<TH1D*> HistoBtag_ ; 
    std::vector<TH2D*> HistoBtag2D_ ; 

    std::map<TString, int> HistoBtag_map_ ; 
    std::map<TString, int> HistoBtag2D_map_ ; 

    int nHistos_ ;
    int nHistos2D_ ; 

    //// Configurables 
    std::string inputfile_ ;
    std::string outfilename_ ;
    TFile* outfile_ ; 

    int maxEvents_ ; 
    bool isData_ ;
    bool runSubJets_ ;

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
  nHistos_(0),
  nHistos2D_(0),  
  inputfile_(iConfig.getUntrackedParameter<std::string>("inputfile")),
  outfilename_(iConfig.getUntrackedParameter<std::string>("outfilename")), 
  maxEvents_(iConfig.getUntrackedParameter<int>("maxEvents")), 
  isData_(iConfig.getUntrackedParameter<bool>("isData")),
  runSubJets_(iConfig.getUntrackedParameter<bool>("runSubJets")) 
{
  //now do what ever initialization is needed

}


BTagValidation::~BTagValidation() { 

  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void BTagValidation::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) { 

  using namespace edm;

  for (int iEntry = 0; iEntry < maxEvents_; iEntry++) { 

    JetTree_->GetEntry(iEntry) ; 
    if((iEntry%1000) == 0) edm::LogInfo("EventNumber") << "Processing event " << iEntry << " of " << maxEvents_ ;

    if (EvtInfo_.Run == -1) isData_ = false ; 

    if (JetInfo_.nJet < 1) continue ; 

    double jetCone(0.4) ; 

    for (int iJet = 0; iJet < JetInfo_.nJet; ++iJet) {

      int flav = JetInfo_.Jet_flavour[iJet] ;
      TLorentzVector jet_p4 ; 
      jet_p4.SetPtEtaPhiM(JetInfo_.Jet_pt[iJet], JetInfo_.Jet_eta[iJet], JetInfo_.Jet_phi[iJet], JetInfo_.Jet_mass[iJet]) ;

      bool isGluonSplit(false) ; 
      int hasBHadMatched(0) ; 
      for (int iB = 0; iB < EvtInfo_.nBHadrons; ++iB) {
        TLorentzVector bhad_p4 ; 
        bhad_p4.SetPtEtaPhiM(EvtInfo_.BHadron_pT[iB], EvtInfo_.BHadron_eta[iB], EvtInfo_.BHadron_phi[iB], EvtInfo_.BHadron_mass[iB]) ; 
        if ( jet_p4.DeltaR(bhad_p4) < jetCone ) ++hasBHadMatched ; 
      }
      if ( hasBHadMatched > 1 ) isGluonSplit = true ; 

      double ww(1) ; //// To be filled by PU weight for MC 

      FillHistoFromMap("jet_pt_all", flav, isGluonSplit , jet_p4.Pt() , ww) ; 
      FillHistoFromMap("jet_eta", flav, isGluonSplit , jet_p4.Eta() , ww) ; 
      FillHistoFromMap("jet_phi", flav, isGluonSplit , jet_p4.Phi() , ww) ; 
      if (JetInfo_.Jet_SV_multi[iJet] > 0) FillHistoFromMap("jet_pt_sv", flav, isGluonSplit , jet_p4.Pt() , ww) ; 

    } 

  }

#ifdef THIS_IS_AN_EVENT_EXAMPLE
  Handle<ExampleData> pIn;
  iEvent.getByLabel("example",pIn);
#endif

#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
  ESHandle<SetupData> pSetup;
  iSetup.get<SetupRecord>().get(pSetup);
#endif

}


// ------------ method called once each job just before starting event loop  ------------
void BTagValidation::beginJob() { 

  JetTree_ = new TChain("btagana/ttree") ;
  FatJetTree_ = new TChain("btaganaSubJets/ttree") ; 

  JetTree_->Add(inputfile_.c_str()) ; 
  FatJetTree_->Add(inputfile_.c_str()) ; 

  if (JetTree_->GetEntries() != FatJetTree_->GetEntries()) {
    edm::LogError("EventNumberMismatch") << "Error: events in JetTree and FatJetTree do not match\n" ; 
    return ; 
  }

  if( maxEvents_ < 0 || maxEvents_ > JetTree_->GetEntries() ) maxEvents_ = JetTree_->GetEntries() ;

  EvtInfo_.ReadTree(JetTree_) ; 
  JetInfo_.ReadTree(JetTree_,"JetInfo") ;
  FatJetInfo_.ReadTree(FatJetTree_,"FatJetInfo") ; 

  outfile_ = new TFile(outfilename_.c_str(),"RECREATE") ; 

  double PtMax = 1000. ; 
  double pi=acos(-1) ; 
  AddHisto("jet_pt_all" ,"pT of all jets"             ,PtMax/10  ,0      ,PtMax ) ; 
  AddHisto("jet_pt_sv"  ,"pT of jets containing a SV" ,PtMax/10  ,0      ,PtMax) ; 
  AddHisto("jet_eta"    ,"eta of all jets"            ,50        ,-2.5   ,2.5) ; 
  AddHisto("jet_phi"    ,"phi of all jets"            ,40        ,-1.*pi ,pi) ; 

}

// ------------ method called once each job just after ending the event loop  ------------
void BTagValidation::endJob() { 

  outfile_->cd() ; 
  for (unsigned int ii = 0; ii < HistoBtag_.size(); ii++) {
    HistoBtag_[ii]->Write();
  }
  for (unsigned int ii = 0; ii < HistoBtag2D_.size(); ii++) {
    HistoBtag2D_[ii]->Write(); 
  }  
  outfile_->Close() ; 

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

void BTagValidation::AddHisto(TString name, TString title, int nbins, double min, double max)  {

  TH1D* h_b      = new TH1D(name+"_b"     ,title+"_b"     ,nbins,min,max); 
  TH1D* h_bfromg = new TH1D(name+"_bfromg",title+"_bfromg",nbins,min,max);  
  TH1D* h_c      = new TH1D(name+"_c"     ,title+"_c"     ,nbins,min,max);  
  TH1D* h_l      = new TH1D(name+"_l"     ,title+"_l"     ,nbins,min,max);
  TH1D* h_data   = new TH1D(name+"_data"  ,title+"_data"  ,nbins,min,max);

  h_b      ->Sumw2();
  h_bfromg ->Sumw2();  
  h_c      ->Sumw2();  
  h_l      ->Sumw2(); 
  h_data   ->Sumw2();

  HistoBtag_.push_back(h_b);
  HistoBtag_.push_back(h_bfromg);  
  HistoBtag_.push_back(h_c);  
  HistoBtag_.push_back(h_l);  
  HistoBtag_.push_back(h_data);  
  HistoBtag_map_[name] = nHistos_;

  nHistos_++;

}

template <class Type>
void BTagValidation::FillHisto(int flavour, bool isGS, int number, Type value, double weight)  {

  if (!isData_){
    if (fabs(flavour)==5 && !isGS)                  HistoBtag_[number*5 +0]->Fill(value,weight);
    else if (fabs(flavour)==5 && isGS)              HistoBtag_[number*5 +1]->Fill(value,weight);
    else if (fabs(flavour)==4)                      HistoBtag_[number*5 +2]->Fill(value,weight); 
    else if (fabs(flavour)< 4 || fabs(flavour)==21) HistoBtag_[number*5 +3]->Fill(value,weight);
  }  
  else                                              HistoBtag_[number*5 +4]->Fill(value);

}

template <class Type>
void BTagValidation::FillHistoFromMap(TString name, int flavour, bool isGS, Type value, double weight)  {

  int number = HistoBtag_map_[name] ;
  if (!isData_){
    if (fabs(flavour)==5 && !isGS)                  HistoBtag_[number*5 +0]->Fill(value,weight);
    else if (fabs(flavour)==5 && isGS)              HistoBtag_[number*5 +1]->Fill(value,weight);
    else if (fabs(flavour)==4)                      HistoBtag_[number*5 +2]->Fill(value,weight); 
    else if (fabs(flavour)< 4 || fabs(flavour)==21) HistoBtag_[number*5 +3]->Fill(value,weight);
  }  
  else                                              HistoBtag_[number*5 +4]->Fill(value);


}

void BTagValidation::AddHisto2D(TString name, TString title, int nbins, double min, double max, int nbins2, double min2, double max2) { 

  TH2D* h_b      = new TH2D(name+"_b"     ,title+"_b"     ,nbins,min,max,nbins2,min2,max2);
  TH2D* h_bfromg = new TH2D(name+"_bfromg",title+"_bfromg",nbins,min,max,nbins2,min2,max2);  
  TH2D* h_c      = new TH2D(name+"_c"     ,title+"_c"     ,nbins,min,max,nbins2,min2,max2);  
  TH2D* h_l      = new TH2D(name+"_l"     ,title+"_l"     ,nbins,min,max,nbins2,min2,max2);
  TH2D* h_data   = new TH2D(name+"_data"  ,title+"_data"  ,nbins,min,max,nbins2,min2,max2);

  h_b        ->Sumw2();
  h_bfromg   ->Sumw2();  
  h_c        ->Sumw2();  
  h_l        ->Sumw2(); 
  h_data     ->Sumw2();

  HistoBtag2D_.push_back(h_b);
  HistoBtag2D_.push_back(h_bfromg);  
  HistoBtag2D_.push_back(h_c);  
  HistoBtag2D_.push_back(h_l);  
  HistoBtag2D_.push_back(h_data);  
  HistoBtag2D_map_[name] = nHistos2D_;
  nHistos2D_++;

}

template <class Type1, class Type2>
void BTagValidation::FillHisto2DFromMap(TString name, int flavour, bool isGS, Type1 value, Type2 value2, double weight) { 

  int number = HistoBtag2D_map_[name] ;
  if (!isData_){
    if (fabs(flavour)==5 && !isGS)                  HistoBtag2D_[number*5 +0]->Fill(value,value2,weight);
    else if (fabs(flavour)==5 && isGS)              HistoBtag2D_[number*5 +1]->Fill(value,value2,weight);
    else if (fabs(flavour)==4)                      HistoBtag2D_[number*5 +2]->Fill(value,value2,weight); 
    else if (fabs(flavour)< 4 || fabs(flavour)==21) HistoBtag2D_[number*5 +3]->Fill(value,value2,weight);
  }  
  else                                              HistoBtag2D_[number*5 +4]->Fill(value,value2);

}

//define this as a plug-in
DEFINE_FWK_MODULE(BTagValidation);
