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
// $Id: BTagValidation.cc,v 1.28 2013/08/09 00:34:56 ferencek Exp $
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

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "PhysicsTools/Utilities/interface/LumiReWeighting.h"
#include "DataFormats/Math/interface/deltaR.h"

#include "RecoBTag/PerformanceMeasurements/interface/JetInfoBranches.h"
#include "RecoBTag/PerformanceMeasurements/interface/EventInfoBranches.h"

#include <TString.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TProfile.h>
#include <TLorentzVector.h>
#include <TF1.h>

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

    bool passTrigger() ;
    bool passMuonSelection(const int muIdx, const JetInfoBranches& JetInfo, const int iJet, const double deltaR=0.4);

    //// Manage histograms
    void createJetHistos(const TString& histoTag);
    void AddHisto(const TString& name, const TString& title, const int nbins, const double min, const double max);
    void AddHisto2D(const TString& name, const TString& title, const int nbins, const double min, const double max, const int nbins2, const double min2, const double max2);

    template <class Type>
    void FillHisto(const TString& name, const int flavour, const bool isGS, const Type value, const double weight);

    template <class Type1, class Type2>
    void FillHisto2D(const TString& name, const int flavour, const bool isGS, const Type1 value, const Type2 value2, const double weight);

    //// Fill jet histograms
    void fillJetHistos(const JetInfoBranches& JetInfo, const int iJet, const bool isGluonSplit, const TString& histoTag, const int nmu, const int nselmuon, const int idxFirstMuon, const double wt);

    // b-tagging scale factors
    double scaleFactor(const int partonFlavor, const double jetPt, const double jetEta, const bool isCSVM);

    double scaleFactorB_CSVL(const double jetPt, const double jetEta);
    double scaleFactorC_CSVL(const double jetPt, const double jetEta);
    double scaleFactorUDSG_CSVL(const double jetPt, const double jetEta);

    double scaleFactorB_CSVM(const double jetPt, const double jetEta);
    double scaleFactorC_CSVM(const double jetPt, const double jetEta);
    double scaleFactorUDSG_CSVM(const double jetPt, const double jetEta);

    // ----------member data ---------------------------
    EventInfoBranches EvtInfo;
    JetInfoBranches FatJetInfo;
    SubJetInfoBranches SubJetInfo;

    JetInfoBranches SubJets ; 

    TChain* JetTreeEvtInfo;
    TChain* JetTree;

    std::map<TString, TH1D*> HistoBtag_map;
    std::map<TString, TH2D*> HistoBtag2D_map;

    edm::Service<TFileService> fs;

    TH1D *h1_CutFlow;
    TH1D *h1_CutFlow_unw;
    TH1D *h1_nPUtrue_mc;
    TH1D *h1_nPUtrue_mc_unw;
    TH1D *h1_nPV_data;
    TH1D *h1_nPV_mc;
    TH1D *h1_nPV_mc_unw;
    TH1D *h1_pt_hat;

    TH1D *h1_nFatJet;
    TH1D *h1_fatjet_pt;

    TH1D *h1_nSubJet;
    TH1D *h1_subjet_pt;

    TProfile *p1_SubJetPt_TotalTracks;
    TProfile *p1_SubJetPt_SharedTracks;
    TProfile *p1_SubJetPt_SharedTracksRatio;

    TProfile *p1_FatJetPt_TotalTracks;
    TProfile *p1_FatJetPt_SharedTracks;
    TProfile *p1_FatJetPt_SharedTracksRatio;

    // CSVL scale factors
    TF1  *CSVL_SFb_0to2p4;
    TH1D *CSVL_SFb_errors;

    TF1 *CSVL_SFl_0to0p5;
    TF1 *CSVL_SFl_0p5to1p0;
    TF1 *CSVL_SFl_1p0to1p5;
    TF1 *CSVL_SFl_1p5to2p4;

    TF1 *CSVL_SFl_0to0p5_min;
    TF1 *CSVL_SFl_0p5to1p0_min;
    TF1 *CSVL_SFl_1p0to1p5_min;
    TF1 *CSVL_SFl_1p5to2p4_min;

    TF1 *CSVL_SFl_0to0p5_max;
    TF1 *CSVL_SFl_0p5to1p0_max;
    TF1 *CSVL_SFl_1p0to1p5_max;
    TF1 *CSVL_SFl_1p5to2p4_max;

    // CSVM scale factors
    TF1  *CSVM_SFb_0to2p4;
    TH1D *CSVM_SFb_errors;

    TF1 *CSVM_SFl_0to0p8;
    TF1 *CSVM_SFl_0p8to1p6;
    TF1 *CSVM_SFl_1p6to2p4;

    TF1 *CSVM_SFl_0to0p8_min;
    TF1 *CSVM_SFl_0p8to1p6_min;
    TF1 *CSVM_SFl_1p6to2p4_min;

    TF1 *CSVM_SFl_0to0p8_max;
    TF1 *CSVM_SFl_0p8to1p6_max;
    TF1 *CSVM_SFl_1p6to2p4_max;

    //// Lumi reweighting object
    edm::LumiReWeighting LumiWeights_;

    //// Configurables
    const int                       maxEvents_;
    const int                       reportEvery_;
    const bool                      useJetProbaTree_;
    const std::string               inputTTreeEvtInfo_;
    const std::string               inputTTree_;
    const std::vector<std::string>  inputFiles_;
    const bool                      useFlavorCategories_;
    const bool                      useRelaxedMuonID_;
    const bool                      applyFatJetMuonTagging_;
    const bool                      applyFatJetBTagging_;
    const bool                      fatJetDoubleTagging_;
    const bool                      processSubJets_;
    const bool                      applySubJetMuonTagging_;
    const bool                      applySubJetBTagging_;
    const bool                      dynamicMuonSubJetDR_;
    const bool                      applySFs_;
    const double                    fatJetBDiscrCut_;
    const double                    subJetBDiscrCut_;
    const double                    fatJetPtMin_;
    const double                    fatJetPtMax_;
    const double                    fatJetAbsEtaMax_;
    const double                    fatJetSoftDropMassMin_;
    const double                    fatJetSoftDropMassMax_;
    const double                    SFbShift_;
    const double                    SFlShift_;
    const std::vector<std::string>  triggerSelection_;
    const std::vector<std::string>  triggerPathNames_;
    const std::string               file_PUDistMC_ ;
    const std::string               file_PUDistData_ ;
    const std::string               hist_PUDistMC_ ;
    const std::string               hist_PUDistData_ ;
    const int                       doPUReweighting_ ;

    //// Event variables
    bool isData;
    int nEventsAll;
    int nEventsStored;
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
  inputTTreeEvtInfo_(iConfig.getParameter<std::string>("InputTTreeEvtInfo")),
  inputTTree_(iConfig.getParameter<std::string>("InputTTree")),
  inputFiles_(iConfig.getParameter<std::vector<std::string> >("InputFiles")),
  useFlavorCategories_(iConfig.getParameter<bool>("UseFlavorCategories")),
  useRelaxedMuonID_(iConfig.getParameter<bool>("UseRelaxedMuonID")),
  applyFatJetMuonTagging_(iConfig.getParameter<bool>("ApplyFatJetMuonTagging")),
  applyFatJetBTagging_(iConfig.getParameter<bool>("ApplyFatJetBTagging")),
  fatJetDoubleTagging_(iConfig.getParameter<bool>("FatJetDoubleTagging")),
  processSubJets_(iConfig.getParameter<bool>("ProcessSubJets")),
  applySubJetMuonTagging_(iConfig.getParameter<bool>("ApplySubJetMuonTagging")),
  applySubJetBTagging_(iConfig.getParameter<bool>("ApplySubJetBTagging")),
  dynamicMuonSubJetDR_(iConfig.getParameter<bool>("DynamicMuonSubJetDR")),
  applySFs_(iConfig.getParameter<bool>("ApplySFs")),
  fatJetBDiscrCut_(iConfig.getParameter<double>("FatJetBDiscrCut")),
  subJetBDiscrCut_(iConfig.getParameter<double>("SubJetBDiscrCut")),
  fatJetPtMin_(iConfig.getParameter<double>("FatJetPtMin")),
  fatJetPtMax_(iConfig.getParameter<double>("FatJetPtMax")),
  fatJetAbsEtaMax_(iConfig.getParameter<double>("FatJetAbsEtaMax")),
  fatJetSoftDropMassMin_(iConfig.getParameter<double>("FatJetSoftDropMassMin")),
  fatJetSoftDropMassMax_(iConfig.getParameter<double>("FatJetSoftDropMassMax")),
  SFbShift_(iConfig.getParameter<double>("SFbShift")),
  SFlShift_(iConfig.getParameter<double>("SFlShift")),
  triggerSelection_(iConfig.getParameter<std::vector<std::string> >("TriggerSelection")),
  triggerPathNames_(iConfig.getParameter<std::vector<std::string> >("TriggerPathNames")),
  file_PUDistMC_(iConfig.getParameter<std::string>("File_PUDistMC")),
  file_PUDistData_(iConfig.getParameter<std::string>("File_PUDistData")),
  hist_PUDistMC_(iConfig.getParameter<std::string>("Hist_PUDistMC")),
  hist_PUDistData_(iConfig.getParameter<std::string>("Hist_PUDistData")),
  doPUReweighting_(iConfig.getParameter<bool>("DoPUReweighting"))
{
  //now do what ever initialization is needed
  isData = false ; 
  nEventsAll = 0;
  nEventsStored = 0;

  if (doPUReweighting_) LumiWeights_ = edm::LumiReWeighting(file_PUDistMC_, file_PUDistData_, hist_PUDistMC_, hist_PUDistData_) ;

  // Pt bins for SFb
  double PtBins[] = {20, 30, 40, 50, 60, 70, 80, 100, 120, 160, 210, 260, 320, 400, 500, 600, 800};

  // CSVL scale factors
  CSVL_SFb_0to2p4 = new TF1("CSVL_SFb_0to2p4","0.997942*((1.+(0.00923753*x))/(1.+(0.0096119*x)))", 20.,800.);

  CSVL_SFb_errors = new TH1D("CSVL_SFb_errors", "CSVL_SFb_errors", 16, PtBins);
  CSVL_SFb_errors->SetBinContent( 0,(2*0.033299));
  CSVL_SFb_errors->SetBinContent( 1,0.033299);
  CSVL_SFb_errors->SetBinContent( 2,0.0146768);
  CSVL_SFb_errors->SetBinContent( 3,0.013803);
  CSVL_SFb_errors->SetBinContent( 4,0.0170145);
  CSVL_SFb_errors->SetBinContent( 5,0.0166976);
  CSVL_SFb_errors->SetBinContent( 6,0.0137879);
  CSVL_SFb_errors->SetBinContent( 7,0.0149072);
  CSVL_SFb_errors->SetBinContent( 8,0.0153068);
  CSVL_SFb_errors->SetBinContent( 9,0.0133077);
  CSVL_SFb_errors->SetBinContent(10,0.0123737);
  CSVL_SFb_errors->SetBinContent(11,0.0157152);
  CSVL_SFb_errors->SetBinContent(12,0.0175161);
  CSVL_SFb_errors->SetBinContent(13,0.0209241);
  CSVL_SFb_errors->SetBinContent(14,0.0278605);
  CSVL_SFb_errors->SetBinContent(15,0.0346928);
  CSVL_SFb_errors->SetBinContent(16,0.0350099);
  CSVL_SFb_errors->SetBinContent(17,(2*0.0350099));

  CSVL_SFl_0to0p5 =   new TF1("CSVL_SFl_0to0p5","((1.01177+(0.0023066*x))+(-4.56052e-06*(x*x)))+(2.57917e-09*(x*(x*x)))", 20.,1000.);
  CSVL_SFl_0p5to1p0 = new TF1("CSVL_SFl_0p5to1p0","((0.975966+(0.00196354*x))+(-3.83768e-06*(x*x)))+(2.17466e-09*(x*(x*x)))", 20.,1000.);
  CSVL_SFl_1p0to1p5 = new TF1("CSVL_SFl_1p0to1p5","((0.93821+(0.00180935*x))+(-3.86937e-06*(x*x)))+(2.43222e-09*(x*(x*x)))", 20.,1000.);
  CSVL_SFl_1p5to2p4 = new TF1("CSVL_SFl_1p5to2p4","((1.00022+(0.0010998*x))+(-3.10672e-06*(x*x)))+(2.35006e-09*(x*(x*x)))", 20.,850.);

  CSVL_SFl_0to0p5_min =   new TF1("CSVL_SFl_0to0p5_min","((0.977761+(0.00170704*x))+(-3.2197e-06*(x*x)))+(1.78139e-09*(x*(x*x)))", 20.,1000.);
  CSVL_SFl_0p5to1p0_min = new TF1("CSVL_SFl_0p5to1p0_min","((0.945135+(0.00146006*x))+(-2.70048e-06*(x*x)))+(1.4883e-09*(x*(x*x)))", 20.,1000.);
  CSVL_SFl_1p0to1p5_min = new TF1("CSVL_SFl_1p0to1p5_min","((0.911657+(0.00142008*x))+(-2.87569e-06*(x*x)))+(1.76619e-09*(x*(x*x)))", 20.,1000.);
  CSVL_SFl_1p5to2p4_min = new TF1("CSVL_SFl_1p5to2p4_min","((0.970045+(0.000862284*x))+(-2.31714e-06*(x*x)))+(1.68866e-09*(x*(x*x)))", 20.,850.);

  CSVL_SFl_0to0p5_max =   new TF1("CSVL_SFl_0to0p5_max","((1.04582+(0.00290226*x))+(-5.89124e-06*(x*x)))+(3.37128e-09*(x*(x*x)))", 20.,1000.);
  CSVL_SFl_0p5to1p0_max = new TF1("CSVL_SFl_0p5to1p0_max","((1.00683+(0.00246404*x))+(-4.96729e-06*(x*x)))+(2.85697e-09*(x*(x*x)))", 20.,1000.);
  CSVL_SFl_1p0to1p5_max = new TF1("CSVL_SFl_1p0to1p5_max","((0.964787+(0.00219574*x))+(-4.85552e-06*(x*x)))+(3.09457e-09*(x*(x*x)))", 20.,1000.);
  CSVL_SFl_1p5to2p4_max = new TF1("CSVL_SFl_1p5to2p4_max","((1.03039+(0.0013358*x))+(-3.89284e-06*(x*x)))+(3.01155e-09*(x*(x*x)))", 20.,850.);

  // CSVM scale factors
  CSVM_SFb_0to2p4 = new TF1("CSVM_SFb_0to2p4","(0.938887+(0.00017124*x))+(-2.76366e-07*(x*x))", 20.,800.);

  CSVM_SFb_errors = new TH1D("CSVM_SFb_errors", "CSVM_SFb_errors", 16, PtBins);
  CSVM_SFb_errors->SetBinContent( 0,(2*0.0415707));
  CSVM_SFb_errors->SetBinContent( 1,0.0415707);
  CSVM_SFb_errors->SetBinContent( 2,0.0204209);
  CSVM_SFb_errors->SetBinContent( 3,0.0223227);
  CSVM_SFb_errors->SetBinContent( 4,0.0206655);
  CSVM_SFb_errors->SetBinContent( 5,0.0199325);
  CSVM_SFb_errors->SetBinContent( 6,0.0174121);
  CSVM_SFb_errors->SetBinContent( 7,0.0202332);
  CSVM_SFb_errors->SetBinContent( 8,0.0182446);
  CSVM_SFb_errors->SetBinContent( 9,0.0159777);
  CSVM_SFb_errors->SetBinContent(10,0.0218531);
  CSVM_SFb_errors->SetBinContent(11,0.0204688);
  CSVM_SFb_errors->SetBinContent(12,0.0265191);
  CSVM_SFb_errors->SetBinContent(13,0.0313175);
  CSVM_SFb_errors->SetBinContent(14,0.0415417);
  CSVM_SFb_errors->SetBinContent(15,0.0740446);
  CSVM_SFb_errors->SetBinContent(16,0.0596716);
  CSVM_SFb_errors->SetBinContent(17,(2*0.0596716));

  CSVM_SFl_0to0p8 =   new TF1("CSVM_SFl_0to0p8","((1.07541+(0.00231827*x))+(-4.74249e-06*(x*x)))+(2.70862e-09*(x*(x*x)))", 20.,1000.);
  CSVM_SFl_0p8to1p6 = new TF1("CSVM_SFl_0p8to1p6","((1.05613+(0.00114031*x))+(-2.56066e-06*(x*x)))+(1.67792e-09*(x*(x*x)))", 20.,1000.);
  CSVM_SFl_1p6to2p4 = new TF1("CSVM_SFl_1p6to2p4","((1.05625+(0.000487231*x))+(-2.22792e-06*(x*x)))+(1.70262e-09*(x*(x*x)))", 20.,850.);

  CSVM_SFl_0to0p8_min =   new TF1("CSVM_SFl_0to0p8_min","((0.964527+(0.00149055*x))+(-2.78338e-06*(x*x)))+(1.51771e-09*(x*(x*x)))", 20.,1000.);
  CSVM_SFl_0p8to1p6_min = new TF1("CSVM_SFl_0p8to1p6_min","((0.946051+(0.000759584*x))+(-1.52491e-06*(x*x)))+(9.65822e-10*(x*(x*x)))", 20.,1000.);
  CSVM_SFl_1p6to2p4_min = new TF1("CSVM_SFl_1p6to2p4_min","((0.956736+(0.000280197*x))+(-1.42739e-06*(x*x)))+(1.0085e-09*(x*(x*x)))", 20.,850.);

  CSVM_SFl_0to0p8_max =   new TF1("CSVM_SFl_0to0p8_max","((1.18638+(0.00314148*x))+(-6.68993e-06*(x*x)))+(3.89288e-09*(x*(x*x)))", 20.,1000.);
  CSVM_SFl_0p8to1p6_max = new TF1("CSVM_SFl_0p8to1p6_max","((1.16624+(0.00151884*x))+(-3.59041e-06*(x*x)))+(2.38681e-09*(x*(x*x)))", 20.,1000.);
  CSVM_SFl_1p6to2p4_max = new TF1("CSVM_SFl_1p6to2p4_max","((1.15575+(0.000693344*x))+(-3.02661e-06*(x*x)))+(2.39752e-09*(x*(x*x)))", 20.,850.);
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

  JetTreeEvtInfo = new TChain(inputTTreeEvtInfo_.c_str());
  JetTree = new TChain(inputTTree_.c_str());

  for(unsigned i=0; i<inputFiles_.size(); ++i)
  {
    JetTreeEvtInfo->Add(inputFiles_.at(i).c_str());
    JetTree->Add(inputFiles_.at(i).c_str());

    TFile *f = TFile::Open(inputFiles_.at(i).c_str(),"READ");

    TH1D *hEventCountAll    = (TH1D*)f->Get("allEvents/hEventCount");
    TH1D *hEventCountStored = (TH1D*)f->Get("selectedEvents/hEventCount");

    nEventsAll+=hEventCountAll->GetBinContent(1);
    nEventsStored+=hEventCountStored->GetBinContent(1);

    f->Close();
  }

  EvtInfo.ReadTree(JetTreeEvtInfo);
  EvtInfo.ReadPatMuonTree(JetTree);

  FatJetInfo.ReadTree(JetTree,"FatJetInfo");
  FatJetInfo.ReadFatJetSpecificTree(JetTree,"FatJetInfo");
  SubJetInfo.ReadTree(JetTree,"FatJetInfo","SoftDrop");

  SubJets.ReadTree(JetTree,"SoftDropSubJetInfo") ; 
  SubJets.ReadSubJetSpecificTree(JetTree,"SoftDropSubJetInfo") ; 

  if (useJetProbaTree_) {
    EvtInfo.ReadJetTrackTree(JetTree);
    FatJetInfo.ReadJetTrackTree(JetTree,"FatJetInfo");
    SubJets.ReadJetTrackTree(JetTree,"FatJetInfo");
  }

  double PtMax = 3000.;

  h1_CutFlow        = fs->make<TH1D>("h1_CutFlow",       "h1_CutFlow",     4, -0.5, 3.5);
  h1_CutFlow->Sumw2();
  h1_CutFlow->SetDefaultSumw2(kTRUE); // enables automatic calling of TH1::Sumw2 for all subsequent histograms
  h1_CutFlow_unw    = fs->make<TH1D>("h1_CutFlow_unw",   "h1_CutFlow_unw", 4, -0.5, 3.5);
  h1_nPUtrue_mc     = fs->make<TH1D>("h1_nPUtrue_mc",    "h1_nPUtrue_mc",  60,0.,60.);
  h1_nPUtrue_mc_unw = fs->make<TH1D>("h1_nPUtrue_mc_unw","h1_nPUtrue_mc",  60,0.,60.);
  h1_nPV_data       = fs->make<TH1D>("h1_nPV_data",      "h1_nPV_data",    60,0.,60.);
  h1_nPV_mc         = fs->make<TH1D>("h1_nPV_mc",        "h1_nPV_mc",      60,0.,60.);
  h1_nPV_mc_unw     = fs->make<TH1D>("h1_nPV_mc_unw",    "h1_nPV_mc_unw",  60,0.,60.);
  h1_pt_hat         = fs->make<TH1D>("h1_pt_hat",        "h1_pt_hat",      600,0,3000);

  h1_nFatJet        = fs->make<TH1D>("h1_nFatJet",       "h1_nFatJet",     100,0,100);
  h1_fatjet_pt      = fs->make<TH1D>("h1_fatjet_pt",     "h1_fatjet_pt",   PtMax/10,0,PtMax);

  if( processSubJets_ )
  {
    h1_nSubJet        = fs->make<TH1D>("h1_nSubJet",       "h1_nSubJet",     100,0,100);
    h1_subjet_pt      = fs->make<TH1D>("h1_subjet_pt",     "h1_subjet_pt",   PtMax/10,0,PtMax);
  }

  p1_SubJetPt_TotalTracks = fs->make<TProfile>("p1_SubJetPt_TotalTracks",";p_{T} [GeV];",20,0,1000);
  p1_SubJetPt_SharedTracks = fs->make<TProfile>("p1_SubJetPt_SharedTracks",";p_{T} [GeV];",20,0,1000);
  p1_SubJetPt_SharedTracksRatio = fs->make<TProfile>("p1_SubJetPt_SharedTracksRatio",";p_{T} [GeV];",20,0,1000);

  p1_FatJetPt_TotalTracks = fs->make<TProfile>("p1_FatJetPt_TotalTracks",";p_{T} [GeV];",20,0,1000);
  p1_FatJetPt_SharedTracks = fs->make<TProfile>("p1_FatJetPt_SharedTracks",";p_{T} [GeV];",20,0,1000);
  p1_FatJetPt_SharedTracksRatio = fs->make<TProfile>("p1_FatJetPt_SharedTracksRatio",";p_{T} [GeV];",20,0,1000);

  //// Create jet histograms
  AddHisto("FatJet_softdropMass"       ,"softdrop mass of all fat jets"                          ,200       ,0      ,400);
  AddHisto("FatJet_massDrop"         ,"mass drop"                                            ,100       ,0      ,1);
  AddHisto("FatJet_subjet_dR"        ,"#DeltaR(subjet_{1},subjet_{2}) in #eta-#phi plane"    ,100       ,0      ,1);
  AddHisto("FatJet_subjet_dyphi"     ,"#DeltaR(subjet_{1},subjet_{2}) in y-#phi plane"       ,100       ,0      ,1);
  AddHisto("FatJet_nsubjettiness"    ,"#tau_{2}/#tau_{1}"                                    ,50        ,0      ,1);
  AddHisto2D("FatJet_softdropMass_nsubjettiness", "Nsubjettiness vs. softdrop mass"              ,200       ,0      ,400      ,50        ,0      ,1);
  AddHisto2D("FatJet_pt_softdropMass",  "softdrop mass vs. p_{T}"                                ,PtMax/10  ,0      ,PtMax    ,200       ,0      ,400);
  //// Common histograms for both fat jets and subjets
  createJetHistos("FatJet");
  if( processSubJets_ ) createJetHistos("SubJet");

}

void BTagValidation::createJetHistos(const TString& histoTag) {

  double PtMax = 3000.;
  double pi=TMath::Pi();

  AddHisto(histoTag+"_pt_all"           ,"p_{T} of all jets"             ,PtMax/10  ,0      ,PtMax );
  AddHisto(histoTag+"_pt_sv"            ,"p_{T} of jets containing a SV" ,PtMax/10  ,0      ,PtMax);
  AddHisto(histoTag+"_eta"              ,"#eta of all jets"              ,50        ,-2.5   ,2.5);
  AddHisto(histoTag+"_phi"              ,"#phi of all jets"              ,40        ,-1.*pi ,pi);
  AddHisto(histoTag+"_mass"             ,"mass of all jets"              ,200       ,0      ,400);

  AddHisto(histoTag+"_muon_multi",      "number of muons",           7, -0.5,6.5  );
  AddHisto(histoTag+"_muon_multi_sel",  "number of selected muons",  7, -0.5,6.5  );
  AddHisto(histoTag+"_mu_ptrel",        "p_{T,rel} of the muon",    50,  0,5      );
  AddHisto(histoTag+"_mu_chi2",         "norm. chi2 of the muon",   50,  0,10     );
  AddHisto(histoTag+"_muon_Pt",         "Muon p_{T}",               250, 0, 500   );
  AddHisto(histoTag+"_muon_eta",        "Muon #eta",                50, -2.5, 2.5 );
  AddHisto(histoTag+"_muon_phi",        "Muon #phi",                40, -1.*pi,pi );
  AddHisto(histoTag+"_muon_Ip3d",       "Muon 3D IP",              100, -0.2, 0.2 );
  AddHisto(histoTag+"_muon_Ip2d",       "Muon 2D IP",              100, -0.2, 0.2 );
  AddHisto(histoTag+"_muon_Sip3d",      "Muon 3D IP significance", 100, -50, 50   );
  AddHisto(histoTag+"_muon_Sip2d",      "Muon 2D IP significance", 100, -50, 50   );
  AddHisto(histoTag+"_muon_DeltaR",     "Muon1 #DeltaR",            50, 0,   0.5  ); //90

  AddHisto(histoTag+"_sv_deltaR_jet",      "sv_deltaR_jet",                                       50,0.,0.5    );
  AddHisto(histoTag+"_sv_deltaR_sumJet",   "SVvtxSumJetDeltaR",                                   50,0.,0.5    );
  AddHisto(histoTag+"_sv_deltaR_sumDir",   "SVvtxSumVtxDirDeltaR",                                50,0.,0.5    );
  AddHisto(histoTag+"_sv_en_ratio",        "Fractional energy",                                   60,0.,1.2    );
  //DMAddHisto(histoTag+"_sv_aboveC",          "IP significance 2D charm",                           100,-50.,50.  );
  AddHisto(histoTag+"_sv_pt",              "Vtx p_{T}",                                          250,0.,500.   );
  AddHisto(histoTag+"_sv_eta",             "Vtx #eta",                                            50, -2.5, 2.5);
  AddHisto(histoTag+"_sv_phi",             "Vtx #phi",                                            40, -1.*pi,pi);
  AddHisto(histoTag+"_sv_flightSig2D",     "Flight distance significance 2D",                    150,0.,150.   );
  AddHisto(histoTag+"_sv_flight2D",        "Flight distance 2D",                                 100,0.,5.     );
  AddHisto(histoTag+"_sv_flightSig2D_3trk","Flight distance 2D with #geq3 tracks",               100,0.,5.     );
  AddHisto(histoTag+"_sv_flight3D",        "Flight distance 3D",                                  50,0.,15.    );
  AddHisto(histoTag+"_sv_flight3DSig",     "flight distance significance 3D",                    150,0.,150.   );
  AddHisto(histoTag+"_sv_multi_0",         "number of secondary vertex",                          6,-0.5,5.5   );
  AddHisto(histoTag+"_sv_multi",           "number of secondary vertex",                          6,-0.5,5.5   );
  AddHisto(histoTag+"_sv_mass",            "invariant mass of the secondary vertex",             750,0.,15.    );
  AddHisto(histoTag+"_sv_chi2norm",        "normalized chi2 of the secondary vertex",            100,0.,20.    );
  AddHisto(histoTag+"_sv_tot_charge",      "Total charge",                                        21,-10.5,10.5);
  AddHisto(histoTag+"_svnTrk",             "Track multiplicity : SVnVertexTracks (centered)",     15,-0.5,14.5 );
  AddHisto(histoTag+"_svnTrk_firstVxt",    "Track multiplicity : SVnFirstVertexTracks (centered)",15,-0.5,14.5 );
  AddHisto(histoTag+"_sv_flight3Derr",     "Flight distance error 3D",                           100,0.,1.0);
  AddHisto(histoTag+"_sv_flight2Derr",     "Flight distance error 2D",                           100,0.,1.0);
  AddHisto(histoTag+"_sv_mass_3trk",       "invariant mass of the secondary vertex with at least 3 SV tracks", 750,0.,15.);

  AddHisto(histoTag+"_track_multi"  ,      "number of tracks in the jets",                80,-0.5,79.5  );
  AddHisto(histoTag+"_trk_multi_sel"  ,    "number of selected tracks in the jets",       80,-0.5,79.5  );
  AddHisto(histoTag+"_track_chi2"   ,      "normalized chi2 of the tracks",               200,0.,10.   );
  AddHisto(histoTag+"_track_nHit" ,        "number of hits ",                             35,-0.5, 34.5 );
  AddHisto(histoTag+"_track_HPix"   ,      "number of hits in the Pixel",                 10,-0.5, 9.5  );

  AddHisto(histoTag+"_track_IPs"    ,      "3D IP significance of all tracks",            200,-50.,50.  );
  AddHisto(histoTag+"_track_IPs1tr" ,      "3D IP significance of the first track",       200,-50.,50.  );
  AddHisto(histoTag+"_track_IPs2tr" ,      "3D IP significance of the second track",      200,-50.,50.  );
  AddHisto(histoTag+"_track_IP"     ,      "3D IP of all tracks",                         200,-0.2,0.2);
  AddHisto(histoTag+"_track_IP1tr"  ,      "3D IP of the first track",                    200,-0.2,0.2);
  AddHisto(histoTag+"_track_IP2tr"  ,      "3D IP of the second track",                   200,-0.2,0.2);
  AddHisto(histoTag+"_track_IP2Ds"    ,    "2D IP significance of all tracks",            200,-50.,50.  );
  AddHisto(histoTag+"_track_IP2Ds1tr" ,    "2D IP significance of the first track",       200,-50.,50.  );
  AddHisto(histoTag+"_track_IP2Ds2tr" ,    "2D IP significance of the second track",      200,-50.,50.  );
  AddHisto(histoTag+"_track_IP2D"    ,     "2D IP of all tracks",                         200,-0.2,0.2);
  AddHisto(histoTag+"_track_IP2D1tr" ,     "2D IP of the first track",                    200,-0.2,0.2);
  AddHisto(histoTag+"_track_IP2D2tr" ,     "2D IP of the second track",                   200,-0.2,0.2);
  AddHisto(histoTag+"_track_IP2Derr1tr" ,  "2D IP error of the first track",              100,0,0.1);
  AddHisto(histoTag+"_track_IPerr1tr"   ,  "3D IP error of the first track",              100,0,0.1);
  AddHisto(histoTag+"_track_IP2Derr2tr" ,  "2D IP error of the second track",             100,0,0.1);
  AddHisto(histoTag+"_track_IPerr2tr"   ,  "3D IP error of the second track",             100,0,0.1);
  AddHisto(histoTag+"_track_IP2Derr" ,     "2D IP error",                                 100,0,0.1);
  AddHisto(histoTag+"_track_IPerr"   ,     "3D IP error",                                 100,0,0.1);
  AddHisto(histoTag+"_track_IPs3tr" ,      "3D IP significance of the third track",       200,-50.,50.  );
  AddHisto(histoTag+"_track_IP3tr"  ,      "3D IP of the third track",                    200,-0.2,0.2);
  AddHisto(histoTag+"_track_IPerr3tr"   ,  "3D IP error of the third track",              100,0,0.1);
  AddHisto(histoTag+"_track_IP2Ds3tr" ,    "2D IP significance of the second track",      100,-35.,35.  );
  AddHisto(histoTag+"_track_IP2D3tr" ,     "2D IP of the third track",                    200,-0.2,0.2);
  AddHisto(histoTag+"_track_IP2Derr3tr" ,  "2D IP error of the third track",              100,0,0.1);

  AddHisto(histoTag+"_track_len"     ,     "decay length",                    100,0,5.     );
  AddHisto(histoTag+"_track_dist"    ,     "distance to the jet axis",        100,0.,0.08    );
  AddHisto(histoTag+"_track_dz"     ,      "transverse IP",                   100,-3,3  );
  AddHisto(histoTag+"_track_isfromSV",     "Track is from SV",                  2,-0.5, 1.5   );
  AddHisto(histoTag+"_track_pt"         ,  "pT of all the tracks",            200,0.,1000.   );
  AddHisto(histoTag+"_track_pt15"   ,      "pT of all the tracks",            500,0.,50.    );
  AddHisto(histoTag+"_track_chi2_cut"     ,"normalized chi2 ",                100,0.,30.    );
  AddHisto(histoTag+"_track_nHit_cut"   ,  "number of hits  ",                 35,-0.5, 34.5 );
  AddHisto(histoTag+"_track_HPix_cut"     ,"number of hits in the Pixel ",     10,-0.5, 9.5  );
  AddHisto(histoTag+"_track_len_cut"      ,"decay length ",                   100,0,25.     );
  AddHisto(histoTag+"_track_dist_cut"     ,"distance to the jet axis ",       100,0.,0.3    );
  AddHisto(histoTag+"_track_dz_cut"       ,"transverse IP ",                  100,-20., 20.  );
  AddHisto(histoTag+"_track_pt_cut"       ,"pT ",                             200,0.,1000.);
  AddHisto(histoTag+"_track_pt15_cut"    , "pT of all the tracks",            300,0.,30.);
  AddHisto(histoTag+"_track_IP2D_cut"     ,"IP2D ",                           100,-1.,1.);

  AddHisto(histoTag+"_TCHE"      ,         "TCHE",                             50,0.,30. );
  AddHisto(histoTag+"_TCHP"      ,         "TCHP",                             50,0.,30. );
  AddHisto(histoTag+"_JP"        ,         "JP",                               50,0.,2.5 );
  AddHisto(histoTag+"_JBP"       ,         "JBP",                              50,0.,8.  );
  AddHisto(histoTag+"_SSV"       ,         "SSVHE",                            70,0.,7.  );
  AddHisto(histoTag+"_SSVHP"     ,         "SSVHP",                            70,0.,7.  );
  AddHisto(histoTag+"_CSV"       ,         "CSV",                              50,0.,1.  );

  AddHisto(histoTag+"_TCHE_extended1"   ,  "TCHE_extended1",                   70, -30.,30. );
  AddHisto(histoTag+"_TCHP_extended1"   ,  "TCHP_extended1",                   70, -30.,30. );
  AddHisto(histoTag+"_TCHE_extended2"   ,  "TCHE_extended2",                  100,-30.,30. );
  AddHisto(histoTag+"_TCHP_extended2"   ,  "TCHP_extended2",                  100,-30.,30. );
  AddHisto(histoTag+"_discri_ssche0",      "SSVHE Discriminator",              80, -1., 7.   );
  AddHisto(histoTag+"_discri_sschp0",      "SSVHP Discriminator",              80, -1., 7.   );

  AddHisto(histoTag+"_SoftMu"    ,         "SoftMu",                           50,0.,1.  );
  AddHisto(histoTag+"_SoftEl"    ,         "SoftEl",                           50,0.,1.  );
  AddHisto(histoTag+"_CombCSVSL"        ,  "CombCSVSL",                        50,0.,1.  );
  AddHisto(histoTag+"_RetCombSvx"   ,      "RetCombSvx",                       50,0.,1.  );

  AddHisto2D(histoTag+"_seltrack_vs_jetpt", "sel track multiplicity vs jet pt",         PtMax/20,0,PtMax, 100,-0.5,99.5);
  AddHisto2D(histoTag+"_sv_mass_vs_flightDist3D", " SVMass vs SV 3D flight distance ",  50,0, 10,60,0,6);
  AddHisto2D(histoTag+"_avg_sv_mass_vs_jetpt","Avg SVMass vs jet pt",                   PtMax/20,0,PtMax, 100,0,6);
  AddHisto2D(histoTag+"_sv_deltar_jet_vs_jetpt","SVJetDeltaR vs jet pt",                PtMax/20,0,PtMax, 50,0.,0.5);
  AddHisto2D(histoTag+"_sv_deltar_sum_jet_vs_jetpt","SVvtxSumJetDeltaR vs jet pt",      PtMax/20,0,PtMax, 50,0.,0.5);
  AddHisto2D(histoTag+"_sv_deltar_sum_dir_vs_jetpt","SVvtxSumVtxDirDeltaR vs jet pt",   PtMax/20,0,PtMax, 50,0.,0.5);
  AddHisto2D(histoTag+"_muon_ptrel_vs_jetpt","PFMuon_p{T}^{rel} vs jet pt",               PtMax/20,0,PtMax,50,0,5);
  AddHisto2D(histoTag+"_muon_DeltaR_vs_jetpt","Muon1 DeltaR vs jet pt",                 PtMax/20,0,PtMax,50,0,0.5);
}

// ------------ method called for each event  ------------
void BTagValidation::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

  using namespace edm;

  if(JetTree == 0 && JetTreeEvtInfo == 0) return;

  Long64_t nEntries = JetTree->GetEntries();
  if(maxEvents_>=0) nEntries = maxEvents_;

  //---------------------------- Start event loop ---------------------------------------//
  for(Long64_t iEntry = 0; iEntry < nEntries; ++iEntry)
  {
    JetTreeEvtInfo->GetEntry(iEntry);
    JetTree->GetEntry(iEntry);
    if((iEntry%reportEvery_) == 0) edm::LogInfo("EventNumber") << "Processing event " << iEntry << " of " << nEntries;

    if(EvtInfo.Run < 0) isData = false;

    double wtPU = 1.;
    if ( doPUReweighting_ && !isData )
      wtPU *= LumiWeights_.weight(EvtInfo.nPUtrue);

    h1_CutFlow->Fill(2.,wtPU); //// count all events
    h1_CutFlow_unw->Fill(2.);

    //edm::LogInfo("PUWeight") << " EvtInfo.nPUtrue: " << EvtInfo.nPUtrue << " wtPU: " << wtPU ;

    if( !isData ) h1_pt_hat->Fill(EvtInfo.pthat,wtPU);

    if( !passTrigger() ) continue; //// apply trigger selection

    h1_CutFlow->Fill(3.,wtPU); //// count events passing trigger selection
    h1_CutFlow_unw->Fill(3.);

    //// pileup distributions
    if( isData )
      h1_nPV_data->Fill(EvtInfo.nPV);
    else
    {
      h1_nPUtrue_mc    ->Fill(EvtInfo.nPUtrue,wtPU);
      h1_nPUtrue_mc_unw->Fill(EvtInfo.nPUtrue);
      h1_nPV_mc        ->Fill(EvtInfo.nPV,wtPU);
      h1_nPV_mc_unw    ->Fill(EvtInfo.nPV);
    }

    if(FatJetInfo.nJet <= 0) continue; //// require at least 1 fat jet in the event

    int nFatJet = 0;
    int nSubJet = 0;

    double fatJetCone(0.8);

    //---------------------------- Start fat jet loop ---------------------------------------//
    for(int iJet = 0; iJet < FatJetInfo.nJet; ++iJet)
    {
      if ( FatJetInfo.Jet_pt[iJet] < fatJetPtMin_ ||
           FatJetInfo.Jet_pt[iJet] > fatJetPtMax_ )                  continue; //// apply jet pT cut
      if ( fabs(FatJetInfo.Jet_eta[iJet]) > fatJetAbsEtaMax_ )       continue; //// apply jet eta cut
      if ( FatJetInfo.Jet_looseID[iJet]==0 )                         continue; //// apply loose jet ID
      if ( FatJetInfo.Jet_massSoftDrop[iJet] < fatJetSoftDropMassMin_ ||
           FatJetInfo.Jet_massSoftDrop[iJet] > fatJetSoftDropMassMax_ )  continue; //// apply softdrop jet mass cut

      int idxFirstMuon = -1;
      int nselmuon = 0;
      int nmu = 0;

      if (FatJetInfo.nPFMuon>0)
      {
        for (int iMu=0; iMu<FatJetInfo.nPFMuon; ++iMu)
        {
          if (FatJetInfo.PFMuon_IdxJet[iMu]==iJet )
          {
            ++nmu;
            if (passMuonSelection(iMu, FatJetInfo, iJet))
            {
              if(nselmuon == 0) idxFirstMuon = iMu;
              ++nselmuon;
            }
          }
        }
      }

      TLorentzVector jet_p4;
      jet_p4.SetPtEtaPhiM(FatJetInfo.Jet_pt[iJet], FatJetInfo.Jet_eta[iJet], FatJetInfo.Jet_phi[iJet], FatJetInfo.Jet_mass[iJet]);

      if ( SubJetInfo.Jet_nSubJets[iJet] != 2 ) continue ; 

      int iSubJet1 = SubJetInfo.SubJetIdx[0] ; 
      int iSubJet2 = SubJetInfo.SubJetIdx[1] ; 

      if( processSubJets_ && (SubJets.Jet_pt[iSubJet1]==0. || SubJets.Jet_pt[iSubJet2]==0.) ) continue; // if processing subjets, skip fat jets for which one of the subjets has pT=0

      TLorentzVector subjet1_p4, subjet2_p4;
      subjet1_p4.SetPtEtaPhiM(SubJets.Jet_pt[iSubJet1], SubJets.Jet_eta[iSubJet1], SubJets.Jet_phi[iSubJet1], SubJets.Jet_mass[iSubJet1]);
      subjet2_p4.SetPtEtaPhiM(SubJets.Jet_pt[iSubJet2], SubJets.Jet_eta[iSubJet2], SubJets.Jet_phi[iSubJet2], SubJets.Jet_mass[iSubJet2]);

      //double fatjet_subjet1_dR = jet_p4.DeltaR(subjet1_p4);
      //double fatjet_subjet2_dR = jet_p4.DeltaR(subjet2_p4);
      double subjet_dR = subjet1_p4.DeltaR(subjet2_p4);
      //double subjet_deta = subjet1_p4.Eta() - subjet2_p4.Eta() ;
      double subjet_dy = subjet1_p4.Rapidity() - subjet2_p4.Rapidity() ;
      double subjet_dphi = subjet1_p4.DeltaPhi(subjet2_p4); ;
      double subjet_dyphi = sqrt( subjet_dy*subjet_dy + subjet_dphi*subjet_dphi ) ;

      //if( processSubJets_ && subjet_dR>0.8 ) continue; // if processing subjets, skip fat jets for which the subjets are separated by dR>0.8

      if( processSubJets_ && subjet_dR<(FatJetInfo.Jet_mass[iJet]/FatJetInfo.Jet_pt[iJet]) ) continue; // if processing subjets, skip infrared unsafe configurations

//       if( subjet_dR > 0.8 )
//       {
//         std::string category = "LargeDRSubjet1Subjet2";
//
//         std::cout << category << ": ----------- START ------------" << std::endl;
//
//         std::cout << category << ": Run, lumi, event: " << EvtInfo.Run << ", "
//                                                         << EvtInfo.LumiBlock << ", "
//                                                         << EvtInfo.Evt << std::endl;
//         std::cout << category << ": N fat jets, N subjets: " << FatJetInfo.nJet << ", "
//                                                            << SubJetInfo.nJet << std::endl;
//         std::cout << category << ": Fat jet idx, pt, eta, phi, mass, jes: " << iJet << ", "
//                                                                             << FatJetInfo.Jet_pt[iJet] << ", "
//                                                                             << FatJetInfo.Jet_eta[iJet] << ", "
//                                                                             << FatJetInfo.Jet_phi[iJet] << ", "
//                                                                             << FatJetInfo.Jet_mass[iJet] << ", "
//                                                                             << FatJetInfo.Jet_jes[iJet] << std::endl;
//         std::cout << category << ": SubJet1 idx, pt, eta, phi, mass, jes: " << iSubJet1 << ", "
//                                                                             << SubJetInfo.Jet_pt[iSubJet1] << ", "
//                                                                             << SubJetInfo.Jet_eta[iSubJet1] << ", "
//                                                                             << SubJetInfo.Jet_phi[iSubJet1] << ", "
//                                                                             << SubJetInfo.Jet_mass[iSubJet1] << ", "
//                                                                             << SubJetInfo.Jet_jes[iSubJet1] << std::endl;
//         std::cout << category << ": SubJet2 idx, pt, eta, phi, mass, jes: " << iSubJet2 << ", "
//                                                                             << SubJetInfo.Jet_pt[iSubJet2] << ", "
//                                                                             << SubJetInfo.Jet_eta[iSubJet2] << ", "
//                                                                             << SubJetInfo.Jet_phi[iSubJet2] << ", "
//                                                                             << SubJetInfo.Jet_mass[iSubJet2] << ", "
//                                                                             << SubJetInfo.Jet_jes[iSubJet2] << std::endl;
//         std::cout << category << ": dR(fat jet, subjet1): "<< fatjet_subjet1_dR << std::endl;
//         std::cout << category << ": dR(fat jet, subjet2): "<< fatjet_subjet2_dR << std::endl;
//         std::cout << category << ": dR(subjet1, subjet2): "<< subjet_dR << std::endl;
//         std::cout << category << ": dyphi(subjet1, subjet2): "<< subjet_dyphi << std::endl;
//         std::cout << category << ": |deta(subjet1, subjet2)|: "<< fabs(subjet_deta) << std::endl;
//         std::cout << category << ": |dy(subjet1, subjet2)|: "<< fabs(subjet_dy) << std::endl;
//         std::cout << category << ": |dphi(subjet1, subjet2)|: "<< fabs(subjet_dphi) << std::endl;
//
//         std::cout << category << ": ------------ END -------------" << std::endl;
//       }

      bool isDoubleMuonTagged = false;

      if ( fatJetDoubleTagging_ && SubJets.nPFMuon>0) {
        // collect all muons matched to the two subjets
        std::vector<int> selectedMuonIdx1, selectedMuonIdx2;

        for (int iMu=0; iMu<SubJets.nPFMuon; ++iMu) {
          if ( SubJets.PFMuon_IdxJet[iMu]==iSubJet1 ) {
            if (passMuonSelection(iMu, SubJets, iSubJet1, (dynamicMuonSubJetDR_ ? subjet_dR/2 : 0.4 )))
              selectedMuonIdx1.push_back(iMu);
          }
          if ( SubJets.PFMuon_IdxJet[iMu]==iSubJet2 )
          {
            if (passMuonSelection(iMu, SubJets, iSubJet2, (dynamicMuonSubJetDR_ ? subjet_dR/2 : 0.4 )))
              selectedMuonIdx2.push_back(iMu);
          }
        }

        // check that there are at least two distinct muons matched to the two subjets
        for(unsigned int iMu1=0; iMu1<selectedMuonIdx1.size(); ++iMu1)
        {
          for(unsigned int iMu2=0; iMu2<selectedMuonIdx2.size(); ++iMu2)
          {
            if( fabs( SubJets.PFMuon_eta[selectedMuonIdx1.at(iMu1)] - SubJets.PFMuon_eta[selectedMuonIdx2.at(iMu2)] ) > 1.E-03 &&
                fabs( SubJets.PFMuon_phi[selectedMuonIdx1.at(iMu1)] - SubJets.PFMuon_phi[selectedMuonIdx2.at(iMu2)] ) > 1.E-03 &&
                fabs( SubJets.PFMuon_pt[selectedMuonIdx1.at(iMu1)] - SubJets.PFMuon_pt[selectedMuonIdx2.at(iMu2)] ) > 1.E-03 )
            {
              isDoubleMuonTagged = true;
              break;
            }
          }
          if( isDoubleMuonTagged ) break;
        }
      }

      if( applyFatJetMuonTagging_ ) //// if enabled, select muon-tagged fat jets
      {
        if( fatJetDoubleTagging_ && !isDoubleMuonTagged ) continue;
        else if( !fatJetDoubleTagging_ && nselmuon==0)    continue;
      }

      if( applyFatJetBTagging_ ) //// if enabled, select b-tagged fat jets
      {
        if( fatJetDoubleTagging_ && !(SubJets.Jet_CombSvx[iSubJet1]>subJetBDiscrCut_ && SubJets.Jet_CombSvx[iSubJet2]>subJetBDiscrCut_) ) continue;
        else if( !fatJetDoubleTagging_ && FatJetInfo.Jet_CombSvx[iJet]<=fatJetBDiscrCut_ ) continue;
      }

      //// apply b-tagging scale factors
      double wtFatJet = 1.;
      if( applySFs_ && !isData && applyFatJetBTagging_ && fatJetDoubleTagging_ ) {
        wtFatJet *= ( scaleFactor(SubJets.Jet_flavour[iSubJet1], SubJets.Jet_pt[iSubJet1], SubJets.Jet_eta[iSubJet1], (subJetBDiscrCut_>0.25)) *
                      scaleFactor(SubJets.Jet_flavour[iSubJet2], SubJets.Jet_pt[iSubJet2], SubJets.Jet_eta[iSubJet2], (subJetBDiscrCut_>0.25)) );
      }

      //// fat jet multiplicity
      ++nFatJet;

      //// test if the jet is a gluon splitting b jet
      bool isGluonSplit(false);
      int nMatchedBHadrons(0);
      for (int iB = 0; iB < EvtInfo.nBHadrons; ++iB)
      {
        if (EvtInfo.BHadron_hasBdaughter[iB]==1) continue; //// skip excited B hadrons
        TLorentzVector bhad_p4;
        bhad_p4.SetPtEtaPhiM(EvtInfo.BHadron_pT[iB], EvtInfo.BHadron_eta[iB], EvtInfo.BHadron_phi[iB], EvtInfo.BHadron_mass[iB]);
        if ( jet_p4.DeltaR(bhad_p4) < fatJetCone ) ++nMatchedBHadrons;
      }
      if ( nMatchedBHadrons > 1 ) isGluonSplit = true ;

      //// fill fat jet histograms
      h1_fatjet_pt->Fill(FatJetInfo.Jet_pt[iJet],wtPU*wtFatJet);
      FillHisto("FatJet_softdropMass"                 ,FatJetInfo.Jet_flavour[iJet] ,isGluonSplit ,FatJetInfo.Jet_massSoftDrop[iJet]                     ,wtPU*wtFatJet);
      FillHisto("FatJet_massDrop"                   ,FatJetInfo.Jet_flavour[iJet] ,isGluonSplit ,std::max( SubJets.Jet_mass[iSubJet1]/SubJets.Jet_jes[iSubJet1], SubJets.Jet_mass[iSubJet2]/SubJets.Jet_jes[iSubJet2] )/(FatJetInfo.Jet_massSoftDrop[iJet]/FatJetInfo.Jet_jes[iJet]) ,wtPU*wtFatJet);
      FillHisto("FatJet_subjet_dR"                  ,FatJetInfo.Jet_flavour[iJet] ,isGluonSplit ,subjet_dR                                           ,wtPU*wtFatJet);
      FillHisto("FatJet_subjet_dyphi"               ,FatJetInfo.Jet_flavour[iJet] ,isGluonSplit ,subjet_dyphi                                        ,wtPU*wtFatJet);
      FillHisto("FatJet_nsubjettiness"              ,FatJetInfo.Jet_flavour[iJet] ,isGluonSplit ,FatJetInfo.Jet_tau2[iJet]/FatJetInfo.Jet_tau1[iJet] ,wtPU*wtFatJet);
      FillHisto2D("FatJet_softdropMass_nsubjettiness" ,FatJetInfo.Jet_flavour[iJet] ,isGluonSplit ,FatJetInfo.Jet_massSoftDrop[iJet] ,FatJetInfo.Jet_tau2[iJet]/FatJetInfo.Jet_tau1[iJet] ,wtPU*wtFatJet);

      fillJetHistos(FatJetInfo, iJet, isGluonSplit, "FatJet", nmu, nselmuon, idxFirstMuon, wtPU*wtFatJet);

      //// ------- start process subjets --------------
      if( processSubJets_ )
      {
        int nTotalFat = 0, nSharedFat = 0; // for track sharing

        for(int sj=0; sj<2; ++sj)
        {
          int iSubJet = SubJetInfo.Jet_nFirstSJ[iJet];
          if( sj==1 ) iSubJet = SubJetInfo.Jet_nLastSJ[iJet];

          int idxFirstMuonSubJet = -1;
          int nselmuonSubJet = 0;
          int nmuSubJet = 0;

          if (SubJets.nPFMuon>0)
          {
            for (int iMu=0; iMu<SubJets.nPFMuon; ++iMu)
            {
              if (SubJets.PFMuon_IdxJet[iMu]==iSubJet )
              {
                ++nmuSubJet;
                if (passMuonSelection(iMu, SubJets, iSubJet, (dynamicMuonSubJetDR_ ? subjet_dR/2 : 0.4 )))
                {
                  if(nselmuonSubJet == 0) idxFirstMuonSubJet = iMu;
                  ++nselmuonSubJet;
                }
              }
            }
          }

          if(applySubJetMuonTagging_ && nselmuonSubJet==0)                              continue;  //// if enabled, select muon-tagged subjets
          if(applySubJetBTagging_ && SubJets.Jet_CombSvx[iSubJet]<=subJetBDiscrCut_) continue;  //// if enabled, select b-tagged subjets

          //// apply b-tagging scale factors
          double wtSubJet = 1.;
          if( applySFs_ && !isData ) {
            if( applyFatJetBTagging_ && fatJetDoubleTagging_ ) wtSubJet *= wtFatJet;
            else                                               wtSubJet *= scaleFactor(SubJets.Jet_flavour[iSubJet], SubJets.Jet_pt[iSubJet], SubJets.Jet_eta[iSubJet], (subJetBDiscrCut_>0.25));
          }

          //// subjet multiplicity
          ++nSubJet;

          //// fill subjet histograms
          h1_subjet_pt->Fill(SubJets.Jet_pt[iSubJet],wtPU*wtSubJet);

          fillJetHistos(SubJets, iSubJet, false, "SubJet", nmuSubJet, nselmuonSubJet, idxFirstMuonSubJet, wtPU*wtSubJet);

          //// track sharing
          int iSubJetComp = (sj==0 ? SubJetInfo.Jet_nLastSJ[iJet] : SubJetInfo.Jet_nLastSJ[iJet]); // companion subjet index
          int nTotal = 0, nShared = 0;

          for (int iTrk=SubJets.Jet_nFirstTrack[iSubJet]; iTrk<SubJets.Jet_nLastTrack[iSubJet]; ++iTrk)
          {
            if( reco::deltaR( SubJets.Track_eta[iTrk], SubJets.Track_phi[iTrk], SubJets.Jet_eta[iSubJet], SubJets.Jet_phi[iSubJet] ) < 0.3 )
            {
              ++nTotal;
              ++nTotalFat;
              if( reco::deltaR( SubJets.Track_eta[iTrk], SubJets.Track_phi[iTrk], SubJets.Jet_eta[iSubJetComp], SubJets.Jet_phi[iSubJetComp] ) < 0.3 )
              {
                ++nShared;
                if(sj==0) ++nSharedFat;
              }
            }
          }

          p1_SubJetPt_TotalTracks->Fill(SubJets.Jet_pt[iSubJet], nTotal, wtPU*wtSubJet);
          p1_SubJetPt_SharedTracks->Fill(SubJets.Jet_pt[iSubJet], nShared, wtPU*wtSubJet);
          if( nTotal>0 ) p1_SubJetPt_SharedTracksRatio->Fill(SubJets.Jet_pt[iSubJet], double(nShared)/double(nTotal), wtPU*wtSubJet);
        }

        p1_FatJetPt_TotalTracks->Fill(FatJetInfo.Jet_pt[iJet], nTotalFat-nSharedFat, wtPU*wtFatJet);
        p1_FatJetPt_SharedTracks->Fill(FatJetInfo.Jet_pt[iJet], nSharedFat, wtPU*wtFatJet);
        if( nTotalFat>0 ) p1_FatJetPt_SharedTracksRatio->Fill(FatJetInfo.Jet_pt[iJet], double(nSharedFat)/double(nTotalFat-nSharedFat), wtPU*wtFatJet);

      } //// ------- end process subjets --------------
    }
    //----------------------------- End fat jet loop ----------------------------------------//

    // fill jet multiplicity
    h1_nFatJet->Fill(nFatJet, wtPU);
    if( processSubJets_ ) h1_nSubJet->Fill(nSubJet, wtPU);
  }
  //----------------------------- End event loop ----------------------------------------//
}

// ------------------------------------------------------------------------------
void BTagValidation::fillJetHistos(const JetInfoBranches& JetInfo, const int iJet, const bool isGluonSplit, const TString& histoTag,
                                   const int nmu, const int nselmuon, const int idxFirstMuon, const double wt) {

  float ptjet      = JetInfo.Jet_pt[iJet];
  float etajet     = JetInfo.Jet_eta[iJet];
  float phijet     = JetInfo.Jet_phi[iJet];
  float mass       = JetInfo.Jet_mass[iJet];
  float ntrkjet    = JetInfo.Jet_ntracks[iJet];
  int   flav       = JetInfo.Jet_flavour[iJet];

  FillHisto(histoTag+"_pt_all", flav, isGluonSplit , ptjet , wt) ;
  if (JetInfo.Jet_SV_multi[iJet] > 0) FillHisto(histoTag+"_pt_sv", flav, isGluonSplit , ptjet , wt) ;

  FillHisto(histoTag+"_eta"         ,flav ,isGluonSplit ,etajet     ,wt) ;
  FillHisto(histoTag+"_phi"         ,flav ,isGluonSplit ,phijet     ,wt) ;
  FillHisto(histoTag+"_mass"        ,flav ,isGluonSplit ,mass       ,wt) ;
  FillHisto(histoTag+"_track_multi" ,flav ,isGluonSplit ,ntrkjet    ,wt) ;

  float mass_sv        = 0.;
  int   n_sv           = 0 ;
  float chi2norm_sv    = 0.;
  float flightSig_sv   = 0.;
  float flight2DSig_sv = 0.;
  float sv_dR_jet      = 0.;
  float sv_dR_dir_sum  = 0.;
  float sv_dR_jet_sum  = 0.;
  float sv_en_rat      = 0.;
  //DMfloat sv_aboveC      = 0.;
  float sv_pt          = 0.;
  float sveta          = 0.;
  float svphi          = 0.;
  float sv_flight3D    = 0.;
  float sv_flight3Derr = 0.;
  float sv_flight2D    = 0.;
  float sv_flight2Derr = 0.;
  int   sv_totchar     = 0;
  float sv_nTrk        = 0.;
  //DMfloat sv_1st_nTrk    = 0.;

  int   n1_ip     = -1;
  int   n2_ip     = -1;
  int   n3_ip     = -1;
  float sig1_ip   = -9999;
  float sig2_ip   = -9999;
  float sig3_ip   = -9999;
  float sig12D_ip = -9999;
  float sig22D_ip = -9999;
  float sig32D_ip = -9999;

  int ntracksel   = 0;

  if ( useJetProbaTree_ )
  {
    //---------------------------- Start tracks loop ---------------------------------------//
    for (int iTrk=JetInfo.Jet_nFirstTrack[iJet]; iTrk<JetInfo.Jet_nLastTrack[iJet] ; ++iTrk)
    {
      // -----------------------------------------
      // -------- track information --------------
      // -----------------------------------------
      bool passNhit=false;
      bool passPix= false;
      bool passIPz=false;
      bool passPt=false;
      bool passnormchi2=false;
      bool passtrkdist=false;
      bool passtrklen=false;
      bool passTrackIP2D=false;

      if (JetInfo.Track_nHitAll[iTrk]>=8)      passNhit=true;
      if (JetInfo.Track_nHitPixel[iTrk]>=2)    passPix= true;
      if (fabs(JetInfo.Track_dz[iTrk])<17)     passIPz=true;
      if (JetInfo.Track_pt[iTrk]>1)            passPt=true;
      if (JetInfo.Track_chi2[iTrk]<5)          passnormchi2=true;
      if (fabs(JetInfo.Track_dist[iTrk])<0.07) passtrkdist=true;
      if (JetInfo.Track_length[iTrk]<5)        passtrklen=true;
      if (fabs(JetInfo.Track_IP2D[iTrk])<0.2)  passTrackIP2D=true;

      if (passNhit && passPix && passIPz && passPt && passnormchi2 && passtrkdist && passTrackIP2D)
        FillHisto(histoTag+"_track_len_cut",          flav, isGluonSplit ,JetInfo.Track_length[iTrk] , wt);

      if (passNhit && passPix && passIPz && passPt && passnormchi2 && passtrklen && passTrackIP2D)
        FillHisto(histoTag+"_track_dist_cut",         flav, isGluonSplit ,fabs(JetInfo.Track_dist[iTrk])   , wt);

      if (passNhit && passPix && passIPz && passPt && passtrkdist && passtrklen && passTrackIP2D)
        FillHisto(histoTag+"_track_chi2_cut",         flav, isGluonSplit ,JetInfo.Track_chi2[iTrk]         , wt);

      if (passNhit && passPix && passIPz && passnormchi2 && passtrkdist && passtrklen && passTrackIP2D)
      {
        FillHisto(histoTag+"_track_pt_cut",           flav, isGluonSplit ,JetInfo.Track_pt[iTrk]     , wt);
        FillHisto(histoTag+"_track_pt15_cut",         flav, isGluonSplit ,JetInfo.Track_pt[iTrk]     , wt);
      }

      if (passNhit && passPix && passPt && passnormchi2 && passtrkdist && passtrklen)
        FillHisto(histoTag+"_track_dz_cut",          flav, isGluonSplit ,JetInfo.Track_dz[iTrk]      ,wt);

      if (passNhit && passIPz && passPt && passnormchi2 && passtrkdist && passtrklen && passTrackIP2D)
        FillHisto(histoTag+"_track_HPix_cut",         flav, isGluonSplit ,JetInfo.Track_nHitPixel[iTrk],wt);

      if (passPix && passIPz && passPt && passnormchi2 && passtrkdist && passtrklen && passTrackIP2D)
        FillHisto(histoTag+"_track_nHit_cut",       flav, isGluonSplit ,JetInfo.Track_nHitAll[iTrk],wt);

      if (passNhit && passPix && passIPz && passPt && passnormchi2 && passtrkdist && passtrklen )
        FillHisto(histoTag+"_track_IP2D_cut",         flav, isGluonSplit ,JetInfo.Track_IP2D[iTrk],wt);

      // -------------------- start selected tracks -------------------------
      if (passNhit && passPix && passIPz && passPt && passnormchi2 && passtrkdist && passtrklen && passTrackIP2D)
      {
        ++ntracksel;

        FillHisto(histoTag+"_track_chi2",    flav, isGluonSplit ,JetInfo.Track_chi2[iTrk]      ,wt);
        FillHisto(histoTag+"_track_nHit",    flav, isGluonSplit ,JetInfo.Track_nHitAll[iTrk]   ,wt);
        FillHisto(histoTag+"_track_HPix",    flav, isGluonSplit ,JetInfo.Track_nHitPixel[iTrk] ,wt);
        FillHisto(histoTag+"_track_IPs",     flav, isGluonSplit ,JetInfo.Track_IPsig[iTrk]     ,wt);
        FillHisto(histoTag+"_track_IP",      flav, isGluonSplit ,JetInfo.Track_IP[iTrk]        ,wt);
        FillHisto(histoTag+"_track_IP2Ds",   flav, isGluonSplit ,JetInfo.Track_IP2Dsig[iTrk]   ,wt);
        FillHisto(histoTag+"_track_IP2D",    flav, isGluonSplit, JetInfo.Track_IP2D[iTrk]      ,wt);
        FillHisto(histoTag+"_track_IP2Derr", flav, isGluonSplit, JetInfo.Track_IP2Derr[iTrk]   ,wt);
        FillHisto(histoTag+"_track_IPerr",   flav, isGluonSplit, JetInfo.Track_IPerr[iTrk]     ,wt);
        FillHisto(histoTag+"_track_dz",      flav, isGluonSplit ,JetInfo.Track_dz[iTrk]        ,wt);
        FillHisto(histoTag+"_track_isfromSV",flav, isGluonSplit ,JetInfo.Track_isfromSV[iTrk]  ,wt);
        FillHisto(histoTag+"_track_len",     flav, isGluonSplit ,JetInfo.Track_length[iTrk]    ,wt);
        FillHisto(histoTag+"_track_dist",    flav, isGluonSplit ,fabs(JetInfo.Track_dist[iTrk]),wt);
        FillHisto(histoTag+"_track_pt",      flav, isGluonSplit ,JetInfo.Track_pt[iTrk]        ,wt);
        FillHisto(histoTag+"_track_pt15",    flav, isGluonSplit ,JetInfo.Track_pt[iTrk]        ,wt);

        // tracks sorted by IP
        float sig   = JetInfo.Track_IP[iTrk]/JetInfo.Track_IPerr[iTrk];
        float sig2D = JetInfo.Track_IP2D[iTrk]/JetInfo.Track_IP2Derr[iTrk];
        if (sig>sig1_ip)
        {
          sig3_ip   = sig2_ip;
          sig2_ip   = sig1_ip;
          sig1_ip   = sig;
          sig32D_ip = sig22D_ip;
          sig22D_ip = sig12D_ip;
          sig12D_ip = sig2D;
          n3_ip     = n2_ip;
          n2_ip     = n1_ip;
          n1_ip     = iTrk;
        }
        else if (sig>sig2_ip)
        {
          sig3_ip   = sig2_ip;
          sig2_ip   = sig;
          sig32D_ip = sig22D_ip;
          sig22D_ip = sig2D;
          n3_ip     = n2_ip;
          n2_ip     = iTrk;
        }
        else if (sig>sig3_ip)
        {
          sig3_ip   = sig;
          sig32D_ip = sig2D;
          n3_ip     = iTrk;
        }
      }
      // -------------------- end selected tracks -------------------------
    }
    //---------------------------- End tracks loop ---------------------------------------//

    if (n1_ip>-1)  {
      FillHisto(histoTag+"_track_IPs1tr",    flav, isGluonSplit ,sig1_ip                          , wt) ;
      FillHisto(histoTag+"_track_IP1tr",     flav, isGluonSplit ,JetInfo.Track_IP[n1_ip]       , wt) ;
      FillHisto(histoTag+"_track_IPerr1tr",  flav, isGluonSplit ,JetInfo.Track_IPerr[n1_ip]    , wt) ;
      FillHisto(histoTag+"_track_IP2Ds1tr",  flav, isGluonSplit ,sig12D_ip                        , wt) ;
      FillHisto(histoTag+"_track_IP2D1tr",   flav, isGluonSplit ,JetInfo.Track_IP2D[n1_ip]     , wt) ;
      FillHisto(histoTag+"_track_IP2Derr1tr",flav, isGluonSplit ,JetInfo.Track_IP2Derr[n1_ip]  , wt) ;
    }

    if (n2_ip>-1) {
      FillHisto(histoTag+"_track_IPs2tr",    flav, isGluonSplit ,sig2_ip                          , wt) ;
      FillHisto(histoTag+"_track_IP2tr",     flav, isGluonSplit ,JetInfo.Track_IP[n2_ip]       , wt) ;
      FillHisto(histoTag+"_track_IPerr2tr",  flav, isGluonSplit ,JetInfo.Track_IPerr[n2_ip]    , wt) ;
      FillHisto(histoTag+"_track_IP2Ds2tr",  flav, isGluonSplit ,sig22D_ip                        , wt) ;
      FillHisto(histoTag+"_track_IP2D2tr",   flav, isGluonSplit ,JetInfo.Track_IP2D[n2_ip]     , wt) ;
      FillHisto(histoTag+"_track_IP2Derr2tr",flav, isGluonSplit ,JetInfo.Track_IP2Derr[n2_ip]  , wt) ;
    }

    if (n3_ip>-1) {
      FillHisto(histoTag+"_track_IPs3tr",    flav, isGluonSplit ,sig3_ip                          , wt) ;
      FillHisto(histoTag+"_track_IP3tr",     flav, isGluonSplit ,JetInfo.Track_IP[n3_ip]       , wt) ;
      FillHisto(histoTag+"_track_IPerr3tr",  flav, isGluonSplit ,JetInfo.Track_IPerr[n3_ip]    , wt) ;
      FillHisto(histoTag+"_track_IP2Ds3tr",  flav, isGluonSplit ,sig32D_ip                        , wt) ;
      FillHisto(histoTag+"_track_IP2D3tr",   flav, isGluonSplit ,JetInfo.Track_IP2D[n3_ip]     , wt) ;
      FillHisto(histoTag+"_track_IP2Derr3tr",flav, isGluonSplit ,JetInfo.Track_IP2Derr[n3_ip]  , wt) ;
    }

    FillHisto(histoTag+"_trk_multi_sel",     flav, isGluonSplit , ntracksel           , wt);
    FillHisto2D(histoTag+"_seltrack_vs_jetpt", flav, isGluonSplit ,ptjet ,  ntracksel , wt);

    // ------------------------------------------------
    // -------------- SV information ------------------
    // ------------------------------------------------
    n_sv = JetInfo.Jet_SV_multi[iJet];
    FillHisto(histoTag+"_sv_multi_0",      flav, isGluonSplit ,n_sv   ,         wt);

    if (n_sv>0)
    {
      chi2norm_sv    = JetInfo.SV_chi2[JetInfo.Jet_nFirstSV[iJet]]/JetInfo.SV_ndf[JetInfo.Jet_nFirstSV[iJet]];
      flightSig_sv   = JetInfo.SV_flight[JetInfo.Jet_nFirstSV[iJet]]/JetInfo.SV_flightErr[JetInfo.Jet_nFirstSV[iJet]];
      flight2DSig_sv = JetInfo.SV_flight2D[JetInfo.Jet_nFirstSV[iJet]]/JetInfo.SV_flight2DErr[JetInfo.Jet_nFirstSV[iJet]];
      mass_sv        = JetInfo.SV_mass[JetInfo.Jet_nFirstSV[iJet]];
      sv_dR_jet      = JetInfo.SV_deltaR_jet[JetInfo.Jet_nFirstSV[iJet]];
      sv_dR_dir_sum  = JetInfo.SV_deltaR_sum_dir[JetInfo.Jet_nFirstSV[iJet]];
      sv_dR_jet_sum  = JetInfo.SV_deltaR_sum_jet[JetInfo.Jet_nFirstSV[iJet]];
      sv_en_rat      = JetInfo.SV_EnergyRatio[JetInfo.Jet_nFirstSV[iJet]];
      //DMsv_aboveC       = JetInfo.SV_aboveC[JetInfo.Jet_nFirstSV[iJet]];
      sv_pt          = JetInfo.SV_vtx_pt[JetInfo.Jet_nFirstSV[iJet]];
      sveta          = JetInfo.SV_vtx_eta[JetInfo.Jet_nFirstSV[iJet]];
      svphi          = JetInfo.SV_vtx_phi[JetInfo.Jet_nFirstSV[iJet]];

      sv_flight3D     = JetInfo.SV_flight[JetInfo.Jet_nFirstSV[iJet]] ;
      sv_flight3Derr  = JetInfo.SV_flightErr[JetInfo.Jet_nFirstSV[iJet]];
      sv_flight2D     = JetInfo.SV_flight2D[JetInfo.Jet_nFirstSV[iJet]];
      sv_flight2Derr  = JetInfo.SV_flight2DErr[JetInfo.Jet_nFirstSV[iJet]];
      sv_totchar      = JetInfo.SV_totCharge[JetInfo.Jet_nFirstSV[iJet]] ;

      sv_nTrk         = JetInfo.SV_nTrk[JetInfo.Jet_nFirstSV[iJet]] ;
      //DMsv_1st_nTrk     = JetInfo.SV_nTrk_firstVxt[JetInfo.Jet_nFirstSV[iJet]];

      // --------- SV histograms --------
      FillHisto(histoTag+"_sv_multi",        flav, isGluonSplit ,n_sv ,  wt);
      FillHisto(histoTag+"_sv_chi2norm",     flav, isGluonSplit ,chi2norm_sv        , wt);
      FillHisto(histoTag+"_sv_mass",         flav, isGluonSplit ,mass_sv,             wt);
      FillHisto(histoTag+"_sv_deltaR_jet",   flav, isGluonSplit ,sv_dR_jet,           wt);
      FillHisto(histoTag+"_sv_deltaR_sumJet",flav, isGluonSplit ,sv_dR_dir_sum,       wt);
      FillHisto(histoTag+"_sv_deltaR_sumDir",flav, isGluonSplit ,sv_dR_jet_sum,       wt);
      FillHisto(histoTag+"_sv_en_ratio",     flav, isGluonSplit ,sv_en_rat,           wt);
      //DMFillHisto(histoTag+"_sv_aboveC",       flav, isGluonSplit ,sv_aboveC,            wt);
      FillHisto(histoTag+"_sv_pt",           flav, isGluonSplit ,sv_pt,               wt);
      FillHisto(histoTag+"_sv_flight2D",     flav, isGluonSplit ,sv_flight2D,         wt);
      FillHisto(histoTag+"_sv_flight2Derr",  flav, isGluonSplit ,sv_flight2Derr,      wt);
      FillHisto(histoTag+"_sv_flightSig2D",  flav, isGluonSplit ,flight2DSig_sv,      wt);
      FillHisto(histoTag+"_sv_tot_charge",   flav, isGluonSplit ,sv_totchar,          wt);
      FillHisto(histoTag+"_svnTrk",          flav, isGluonSplit ,sv_nTrk,             wt);
      //DMFillHisto(histoTag+"_svnTrk_firstVxt", flav, isGluonSplit ,sv_1st_nTrk,         wt);
      FillHisto(histoTag+"_sv_eta",          flav, isGluonSplit ,sveta,               wt);
      FillHisto(histoTag+"_sv_phi",          flav, isGluonSplit ,svphi,               wt);
      FillHisto(histoTag+"_sv_flight3D",     flav, isGluonSplit ,sv_flight3D,         wt);
      FillHisto(histoTag+"_sv_flight3Derr",  flav, isGluonSplit ,sv_flight3Derr,      wt);
      FillHisto(histoTag+"_sv_flight3DSig",  flav, isGluonSplit ,flightSig_sv,        wt);

      if (sv_nTrk >2)
      {
        FillHisto(histoTag+"_sv_mass_3trk", flav, isGluonSplit ,mass_sv,wt);
        FillHisto(histoTag+"_sv_flightSig2D_3trk",  flav, isGluonSplit ,flight2DSig_sv,        wt);
      }

      FillHisto2D(histoTag+"_sv_mass_vs_flightDist3D"     ,flav,isGluonSplit ,sv_flight3D,mass_sv,wt);
      FillHisto2D(histoTag+"_avg_sv_mass_vs_jetpt"        ,flav,isGluonSplit ,ptjet,mass_sv,wt);
      FillHisto2D(histoTag+"_sv_deltar_jet_vs_jetpt"      ,flav,isGluonSplit ,ptjet,sv_dR_jet,wt);
      FillHisto2D(histoTag+"_sv_deltar_sum_jet_vs_jetpt"  ,flav,isGluonSplit ,ptjet,sv_dR_dir_sum,wt);
      FillHisto2D(histoTag+"_sv_deltar_sum_dir_vs_jetpt"  ,flav,isGluonSplit ,ptjet,sv_dR_jet_sum,wt);
    } // if n_sv > 0
  } // end useJetProbaTree

  // ------------------------------------------------
  // ------------- tagger information ---------------
  // ------------------------------------------------
  float tche      = JetInfo.Jet_Ip2P[iJet];
  float tchp      = JetInfo.Jet_Ip3P[iJet];
  float jetproba  = JetInfo.Jet_Proba[iJet];
  float jetbproba = JetInfo.Jet_Bprob[iJet];
  float ssvhe     = JetInfo.Jet_Svx[iJet] ;
  float ssvhp     = JetInfo.Jet_SvxHP[iJet];
  float csv       = JetInfo.Jet_CombSvx[iJet];

  FillHisto(histoTag+"_TCHE",  flav, isGluonSplit, tche   ,   wt);
  FillHisto(histoTag+"_TCHP",  flav, isGluonSplit, tchp   ,   wt);
  FillHisto(histoTag+"_JP",    flav, isGluonSplit, jetproba  , wt);
  FillHisto(histoTag+"_JBP",   flav, isGluonSplit, jetbproba , wt);
  FillHisto(histoTag+"_SSV",   flav, isGluonSplit, ssvhe         ,   wt);
  FillHisto(histoTag+"_SSVHP", flav, isGluonSplit, ssvhp         ,   wt);
  FillHisto(histoTag+"_CSV",   flav, isGluonSplit, csv    ,   wt);

  FillHisto(histoTag+"_TCHE_extended1",  flav, isGluonSplit, tche  , wt);
  FillHisto(histoTag+"_TCHP_extended1",  flav, isGluonSplit, tchp  , wt);
  FillHisto(histoTag+"_TCHE_extended2",  flav, isGluonSplit, tche  , wt);
  FillHisto(histoTag+"_TCHP_extended2",  flav, isGluonSplit, tchp  , wt);
  FillHisto(histoTag+"_discri_ssche0",   flav, isGluonSplit, ssvhe , wt);
  FillHisto(histoTag+"_discri_sschp0",   flav, isGluonSplit, ssvhp , wt);

  float softmu       = JetInfo.Jet_SoftMu[iJet];
  float solfel       = JetInfo.Jet_SoftEl[iJet];
  //DMfloat combsl       = JetInfo.Jet_CombCSVSL[iJet];

  FillHisto(histoTag+"_SoftMu",      flav, isGluonSplit, softmu  ,   wt);
  FillHisto(histoTag+"_SoftEl",      flav, isGluonSplit, solfel  ,   wt);
  //DMFillHisto(histoTag+"_CombCSVSL",   flav, isGluonSplit, combsl  ,   wt);

  // ------------------------------------------------
  // ------------- muon information -----------------
  // ------------------------------------------------
  FillHisto(histoTag+"_muon_multi_sel",  flav, isGluonSplit , nselmuon   ,wt);
  FillHisto(histoTag+"_muon_multi",      flav, isGluonSplit , nmu        ,wt);

  if(idxFirstMuon > -1)
  {
    FillHisto(histoTag+"_mu_ptrel",    flav, isGluonSplit ,JetInfo.PFMuon_ptrel[idxFirstMuon] ,wt);
    FillHisto(histoTag+"_mu_chi2",     flav, isGluonSplit ,JetInfo.PFMuon_chi2[idxFirstMuon]  ,wt);
    FillHisto(histoTag+"_muon_Pt",     flav, isGluonSplit, JetInfo.PFMuon_pt[idxFirstMuon] ,     wt);
    FillHisto(histoTag+"_muon_eta",    flav, isGluonSplit, JetInfo.PFMuon_eta[idxFirstMuon] ,    wt);
    FillHisto(histoTag+"_muon_phi",    flav, isGluonSplit, JetInfo.PFMuon_phi[idxFirstMuon] ,    wt);
    FillHisto(histoTag+"_muon_Ip3d",   flav, isGluonSplit, JetInfo.PFMuon_IP[idxFirstMuon] ,     wt);
    FillHisto(histoTag+"_muon_Ip2d",   flav, isGluonSplit, JetInfo.PFMuon_IP2D[idxFirstMuon] ,   wt);

    TLorentzVector themuon, thejet;

    thejet.SetPtEtaPhiM(JetInfo.Jet_pt[iJet], JetInfo.Jet_eta[iJet], JetInfo.Jet_phi[iJet], JetInfo.Jet_mass[iJet]);
    themuon.SetPtEtaPhiM(JetInfo.PFMuon_pt[idxFirstMuon], JetInfo.PFMuon_eta[idxFirstMuon], JetInfo.PFMuon_phi[idxFirstMuon], 0);

    FillHisto(histoTag+"_muon_DeltaR",         flav, isGluonSplit, themuon.DeltaR(thejet) , wt);
    FillHisto2D(histoTag+"_muon_ptrel_vs_jetpt", flav, isGluonSplit,ptjet,JetInfo.PFMuon_ptrel[idxFirstMuon],wt);
    FillHisto2D(histoTag+"_muon_DeltaR_vs_jetpt",flav, isGluonSplit,ptjet,themuon.DeltaR(thejet),wt);
  }
}

// ------------ method called once each job just after ending the event loop  ------------
void BTagValidation::endJob() {

  h1_CutFlow->SetBinContent(1, nEventsAll); //// strictly speaking not correct since event weights not applied
  h1_CutFlow->SetBinContent(2, nEventsStored); //// strictly speaking not correct since event weights not applied
  h1_CutFlow_unw->SetBinContent(1, nEventsAll);
  h1_CutFlow_unw->SetBinContent(2, nEventsStored);
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
void BTagValidation::AddHisto(const TString& name, const TString& title, const int nbins, const double min, const double max)  {

  TH1D* h_b;
  TH1D* h_bfromg;
  TH1D* h_c;
  TH1D* h_l;
  TH1D* h_mc;

  if( useFlavorCategories_ )
  {
    h_b      = fs->make<TH1D>(name+"_b"     ,title+"_b"     ,nbins,min,max);
    h_bfromg = fs->make<TH1D>(name+"_bfromg",title+"_bfromg",nbins,min,max);
    h_c      = fs->make<TH1D>(name+"_c"     ,title+"_c"     ,nbins,min,max);
    h_l      = fs->make<TH1D>(name+"_l"     ,title+"_l"     ,nbins,min,max);

    h_b      ->Sumw2();
    h_bfromg ->Sumw2();
    h_c      ->Sumw2();
    h_l      ->Sumw2();

    HistoBtag_map[name+"_b"]      = h_b;
    HistoBtag_map[name+"_bfromg"] = h_bfromg;
    HistoBtag_map[name+"_c"]      = h_c;
    HistoBtag_map[name+"_l"]      = h_l;
  }
  else
  {
    h_mc = fs->make<TH1D>(name+"_mc",title+"_mc",nbins,min,max);
    h_mc->Sumw2();
    HistoBtag_map[name+"_mc"] = h_mc;
  }

  TH1D* h_data= fs->make<TH1D>(name+"_data",title+"_data",nbins,min,max);
  h_data->Sumw2();
  HistoBtag_map[name+"_data"] = h_data;
}

// ------------------------------------------------------------------------------
void BTagValidation::AddHisto2D(const TString& name, const TString& title, const int nbins, const double min, const double max, const int nbins2, const double min2, const double max2)
{
  TH2D* h_b;
  TH2D* h_bfromg;
  TH2D* h_c;
  TH2D* h_l;
  TH2D* h_mc;

  if( useFlavorCategories_ )
  {
    h_b      = fs->make<TH2D>(name+"_b"     ,title+"_b"     ,nbins,min,max,nbins2,min2,max2);
    h_bfromg = fs->make<TH2D>(name+"_bfromg",title+"_bfromg",nbins,min,max,nbins2,min2,max2);
    h_c      = fs->make<TH2D>(name+"_c"     ,title+"_c"     ,nbins,min,max,nbins2,min2,max2);
    h_l      = fs->make<TH2D>(name+"_l"     ,title+"_l"     ,nbins,min,max,nbins2,min2,max2);

    h_b      ->Sumw2();
    h_bfromg ->Sumw2();
    h_c      ->Sumw2();
    h_l      ->Sumw2();

    HistoBtag2D_map[name+"_b"]      = h_b;
    HistoBtag2D_map[name+"_bfromg"] = h_bfromg;
    HistoBtag2D_map[name+"_c"]      = h_c;
    HistoBtag2D_map[name+"_l"]      = h_l;
  }
  else
  {
    h_mc = fs->make<TH2D>(name+"_mc",title+"_mc",nbins,min,max,nbins2,min2,max2);
    h_mc->Sumw2();
    HistoBtag2D_map[name+"_mc"] = h_mc;
  }

  TH2D* h_data= fs->make<TH2D>(name+"_data",title+"_data",nbins,min,max,nbins2,min2,max2);
  h_data->Sumw2();
  HistoBtag2D_map[name+"_data"] = h_data;
}

// ------------------------------------------------------------------------------
template <class Type>
void BTagValidation::FillHisto(const TString& name, const int flavour, const bool isGS, const Type value, const double weight)
{
  if (!isData)
  {
    if( useFlavorCategories_ )
    {
      if( isGS )
        HistoBtag_map[name+"_bfromg"]->Fill(double(value),weight);
      else
      {
        if (abs(flavour)==5)                          HistoBtag_map[name+"_b"]->Fill(double(value),weight);
        else if (abs(flavour)==4)                     HistoBtag_map[name+"_c"]->Fill(double(value),weight);
        else if (abs(flavour)< 4 || abs(flavour)==21) HistoBtag_map[name+"_l"]->Fill(double(value),weight);
      }
    }
    else
      HistoBtag_map[name+"_mc"]->Fill(double(value),weight);
  }
  else
    HistoBtag_map[name+"_data"]->Fill(double(value));
}

// ------------------------------------------------------------------------------
template <class Type1, class Type2>
void BTagValidation::FillHisto2D(const TString& name, const int flavour, const bool isGS, const Type1 value, const Type2 value2, const double weight)
{
  if (!isData)
  {
    if( useFlavorCategories_ )
    {
      if( isGS )
        HistoBtag2D_map[name+"_bfromg"]->Fill(double(value),double(value2),weight);
      else
      {
        if (abs(flavour)==5)                          HistoBtag2D_map[name+"_b"]->Fill(double(value),double(value2),weight);
        else if (abs(flavour)==4)                     HistoBtag2D_map[name+"_c"]->Fill(double(value),double(value2),weight);
        else if (abs(flavour)< 4 || abs(flavour)==21) HistoBtag2D_map[name+"_l"]->Fill(double(value),double(value2),weight);
      }
    }
    else
      HistoBtag2D_map[name+"_mc"]->Fill(double(value),double(value2),weight);
  }
  else
    HistoBtag2D_map[name+"_data"]->Fill(double(value),double(value2));
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

// ------------------------------------------------------------------------------
bool BTagValidation::passMuonSelection(const int muIdx, const JetInfoBranches& JetInfo, const int iJet, const double deltaR)
{
  TLorentzVector muon, jet;

  jet.SetPtEtaPhiM(JetInfo.Jet_pt[iJet], JetInfo.Jet_eta[iJet], JetInfo.Jet_phi[iJet], JetInfo.Jet_mass[iJet]);
  muon.SetPtEtaPhiM(JetInfo.PFMuon_pt[muIdx], JetInfo.PFMuon_eta[muIdx], JetInfo.PFMuon_phi[muIdx], 0);

  bool cut_mu_pass = false;
  if (JetInfo.PFMuon_pt[muIdx] > 5 && fabs(JetInfo.PFMuon_eta[muIdx]) < 2.4 && JetInfo.PFMuon_isGlobal[muIdx] == 1 &&
      JetInfo.PFMuon_nMuHit[muIdx] > 0 && JetInfo.PFMuon_nMatched[muIdx] > 1 && JetInfo.PFMuon_nTkHit[muIdx] > (useRelaxedMuonID_ ? 7 : 10) &&
      JetInfo.PFMuon_nPixHit[muIdx] > (useRelaxedMuonID_ ? 0 : 1) && JetInfo.PFMuon_nOutHit[muIdx] < (useRelaxedMuonID_ ? 99 : 3) && JetInfo.PFMuon_chi2Tk[muIdx] < 10 &&
      JetInfo.PFMuon_chi2[muIdx] < 10  && // JetInfo.PFMuon_vz[muIdx]< 2 &&
      jet.DeltaR(muon) < deltaR 
      //DM&& fabs(JetInfo.PFMuon_vz[muIdx]-EvtInfo.PVz) < 2.
      )
    cut_mu_pass = true ;

  return cut_mu_pass;
}

// ------------ method that returns pT- and eta-dependent b-tag efficiency scale factor  ------------
double BTagValidation::scaleFactor(const int partonFlavor, const double jetPt, const double jetEta, const bool isCSVM)
{
  if( isCSVM )
  {
    if( abs(partonFlavor)==5 )
      return scaleFactorB_CSVM(jetPt,jetEta);
    else if( abs(partonFlavor)==4 )
      return scaleFactorC_CSVM(jetPt,jetEta);
    else
      return scaleFactorUDSG_CSVM(jetPt,jetEta);
  }
  else
  {
    if( abs(partonFlavor)==5 )
      return scaleFactorB_CSVL(jetPt,jetEta);
    else if( abs(partonFlavor)==4 )
      return scaleFactorC_CSVL(jetPt,jetEta);
    else
      return scaleFactorUDSG_CSVL(jetPt,jetEta);
  }
}

// ------------ method that returns pT- and eta-dependent b-tag efficiency scale factor for b-jets and CSVL tagger  ------------
double BTagValidation::scaleFactorB_CSVL(const double jetPt, const double jetEta)
{
  double Pt = jetPt;
  // for scale factor extrapolation
  if(Pt<20.) Pt = 20.;
  if(Pt>800.) Pt = 800.;

  return CSVL_SFb_0to2p4->Eval(Pt) + SFbShift_*CSVL_SFb_errors->GetBinContent(CSVL_SFb_errors->GetXaxis()->FindBin(jetPt));
}

// ------------ method that returns pT- and eta-dependent b-tag efficiency scale factor for c-jets and CSVL tagger  ------------
double BTagValidation::scaleFactorC_CSVL(const double jetPt, const double jetEta)
{
  double Pt = jetPt;
  // for scale factor extrapolation
  if(Pt<20.) Pt = 20.;
  if(Pt>800.) Pt = 800.;

  return CSVL_SFb_0to2p4->Eval(Pt) + 2*SFbShift_*CSVL_SFb_errors->GetBinContent(CSVL_SFb_errors->GetXaxis()->FindBin(jetPt));
}

// ------------ method that returns pT- and eta-dependent b-tag efficiency scale factor for light flavor jets and CSVL tagger ------------
double BTagValidation::scaleFactorUDSG_CSVL(const double jetPt, const double jetEta)
{
  double SF = 1.;
  double Pt = jetPt;
  double absEta = fabs(jetEta);
  // for scale factor extrapolation
  if(Pt<20.) Pt = 20.;

  if(absEta<0.5)
  {
    if(Pt>1000.) Pt = 1000.;

    SF = CSVL_SFl_0to0p5->Eval(Pt) + ( (jetPt<20. || jetPt>1000.) ? 2. : 1. )*fabs(SFlShift_)*( SFlShift_ >= 0. ? (CSVL_SFl_0to0p5_max->Eval(Pt) - CSVL_SFl_0to0p5->Eval(Pt)) : (CSVL_SFl_0to0p5_min->Eval(Pt) - CSVL_SFl_0to0p5->Eval(Pt)) );
  }
  else if(absEta>=0.5 && absEta<1.)
  {
    if(Pt>1000.) Pt = 1000.;

    SF = CSVL_SFl_0p5to1p0->Eval(Pt) + ( (jetPt<20. || jetPt>1000.) ? 2. : 1. )*fabs(SFlShift_)*( SFlShift_ >= 0. ? (CSVL_SFl_0p5to1p0_max->Eval(Pt) - CSVL_SFl_0p5to1p0->Eval(Pt)) : (CSVL_SFl_0p5to1p0_min->Eval(Pt) - CSVL_SFl_0p5to1p0->Eval(Pt)) );
  }
  else if(absEta>=1. && absEta<1.5)
  {
    if(Pt>1000.) Pt = 1000.;

    SF = CSVL_SFl_1p0to1p5->Eval(Pt) + ( (jetPt<20. || jetPt>1000.) ? 2. : 1. )*fabs(SFlShift_)*( SFlShift_ >= 0. ? (CSVL_SFl_1p0to1p5_max->Eval(Pt) - CSVL_SFl_1p0to1p5->Eval(Pt)) : (CSVL_SFl_1p0to1p5_min->Eval(Pt) - CSVL_SFl_1p0to1p5->Eval(Pt)) );
  }
  else
  {
    if(Pt>850.) Pt = 850.;

    SF = CSVL_SFl_1p5to2p4->Eval(Pt) + ( (jetPt<20. || jetPt>850.) ? 2. : 1. )*fabs(SFlShift_)*( SFlShift_ >= 0. ? (CSVL_SFl_1p5to2p4_max->Eval(Pt) - CSVL_SFl_1p5to2p4->Eval(Pt)) : (CSVL_SFl_1p5to2p4_min->Eval(Pt) - CSVL_SFl_1p5to2p4->Eval(Pt)) );
  }

  return SF;
}

// ------------ method that returns pT- and eta-dependent b-tag efficiency scale factor for b-jets and CSVM tagger  ------------
double BTagValidation::scaleFactorB_CSVM(const double jetPt, const double jetEta)
{
  double Pt = jetPt;
  // for scale factor extrapolation
  if(Pt<20.) Pt = 20.;
  if(Pt>800.) Pt = 800.;

  return CSVM_SFb_0to2p4->Eval(Pt) + SFbShift_*CSVM_SFb_errors->GetBinContent(CSVM_SFb_errors->GetXaxis()->FindBin(jetPt));
}

// ------------ method that returns pT- and eta-dependent b-tag efficiency scale factor for c-jets and CSVM tagger  ------------
double BTagValidation::scaleFactorC_CSVM(const double jetPt, const double jetEta)
{
  double Pt = jetPt;
  // for scale factor extrapolation
  if(Pt<20.) Pt = 20.;
  if(Pt>800.) Pt = 800.;

  return CSVM_SFb_0to2p4->Eval(Pt) + 2*SFbShift_*CSVM_SFb_errors->GetBinContent(CSVM_SFb_errors->GetXaxis()->FindBin(jetPt));
}

// ------------ method that returns pT- and eta-dependent b-tag efficiency scale factor for light flavor jets and CSVM tagger ------------
double BTagValidation::scaleFactorUDSG_CSVM(const double jetPt, const double jetEta)
{
  double SF = 1.;
  double Pt = jetPt;
  double absEta = fabs(jetEta);
  // for scale factor extrapolation
  if(Pt<20.) Pt = 20.;

  if(absEta<0.8)
  {
    if(Pt>1000.) Pt = 1000.;

    SF = CSVM_SFl_0to0p8->Eval(Pt) + ( (jetPt<20. || jetPt>1000.) ? 2. : 1. )*fabs(SFlShift_)*( SFlShift_ >= 0. ? (CSVM_SFl_0to0p8_max->Eval(Pt) - CSVM_SFl_0to0p8->Eval(Pt)) : (CSVM_SFl_0to0p8_min->Eval(Pt) - CSVM_SFl_0to0p8->Eval(Pt)) );
  }
  else if(absEta>=0.8 && absEta<1.6)
  {
    if(Pt>1000.) Pt = 1000.;

    SF = CSVM_SFl_0p8to1p6->Eval(Pt) + ( (jetPt<20. || jetPt>1000.) ? 2. : 1. )*fabs(SFlShift_)*( SFlShift_ >= 0. ? (CSVM_SFl_0p8to1p6_max->Eval(Pt) - CSVM_SFl_0p8to1p6->Eval(Pt)) : (CSVM_SFl_0p8to1p6_min->Eval(Pt) - CSVM_SFl_0p8to1p6->Eval(Pt)) );
  }
  else
  {
    if(Pt>850.) Pt = 850.;

    SF = CSVM_SFl_1p6to2p4->Eval(Pt) + ( (jetPt<20. || jetPt>850.) ? 2. : 1. )*fabs(SFlShift_)*( SFlShift_ >= 0. ? (CSVM_SFl_1p6to2p4_max->Eval(Pt) - CSVM_SFl_1p6to2p4->Eval(Pt)) : (CSVM_SFl_1p6to2p4_min->Eval(Pt) - CSVM_SFl_1p6to2p4->Eval(Pt)) );
  }

  return SF;
}

//define this as a plug-in
DEFINE_FWK_MODULE(BTagValidation);
