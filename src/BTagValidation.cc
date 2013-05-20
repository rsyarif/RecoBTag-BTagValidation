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
// $Id: BTagValidation.cc,v 1.3 2013/05/20 04:55:14 ferencek Exp $
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
    virtual void beginJob();
    virtual void analyze(const edm::Event&, const edm::EventSetup&);
    virtual void endJob();

    virtual void beginRun(edm::Run const&, edm::EventSetup const&);
    virtual void endRun(edm::Run const&, edm::EventSetup const&);
    virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
    virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);

    bool passTrigger();
    bool passMuonSelection(int muIdx, int iJet, const JetInfoBranches& JetInfo);

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
    TH1D *h1_nPUtrue_mc;
    TH1D *h1_nPV_data;
    TH1D *h1_nPV_mc;
    TH1D *h1_nPV_mc_unw;
    TH1D *h1_pt_hat;
    TH1D *h1_fatjet_pt_mc;
    TH1D *h1_nJet_data;
    TH1D *h1_nJet_mc;

    //// Lumi reweighting object 
    edm::LumiReWeighting LumiWeights_;

    //// Configurables
    const int                       maxEvents_;
    const int                       reportEvery_;
    const bool                      useJetProbaTree_;
    const std::string               inputTTree_;
    const std::vector<std::string>  inputFiles_;
    const double                    jetPtMin_;
    const double                    jetPtMax_;
    const double                    jetAbsEtaMax_;
    const std::vector<std::string>  triggerSelection_;
    const std::vector<std::string>  triggerPathNames_;
    const std::string               file_PUDistMC_ ; 
    const std::string               file_PUDistData_ ; 
    const std::string               hist_PUDistMC_ ; 
    const std::string               hist_PUDistData_ ; 
    const int                       doPUReweighting_ ; 
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
  jetPtMin_(iConfig.getParameter<double>("JetPtMin")),
  jetPtMax_(iConfig.getParameter<double>("JetPtMax")),
  jetAbsEtaMax_(iConfig.getParameter<double>("JetAbsEtaMax")),
  triggerSelection_(iConfig.getParameter<std::vector<std::string> >("TriggerSelection")),
  triggerPathNames_(iConfig.getParameter<std::vector<std::string> >("TriggerPathNames")),  
  file_PUDistMC_(iConfig.getParameter<std::string>("File_PUDistMC")), 
  file_PUDistData_(iConfig.getParameter<std::string>("File_PUDistData")), 
  hist_PUDistMC_(iConfig.getParameter<std::string>("Hist_PUDistMC")), 
  hist_PUDistData_(iConfig.getParameter<std::string>("Hist_PUDistData")), 
  doPUReweighting_(iConfig.getParameter<bool>("DoPUReweighting")) 
{
  //now do what ever initialization is needed
  isData = true;
  if (doPUReweighting_) LumiWeights_ = edm::LumiReWeighting(file_PUDistMC_, file_PUDistData_, hist_PUDistMC_, hist_PUDistData_) ; 
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

  double PtMax = 1000.;
  double pi=TMath::Pi();

  h1_CutFlow      = fs->make<TH1D>("h1_CutFlow",      "h1_CutFlow",                 2, -0.5, 1.5);
  h1_nPUtrue_mc   = fs->make<TH1D>("h1_nPUtrue_mc",   "h1_nPUtrue_mc",             60,-0.5,59.5);
  h1_nPV_data     = fs->make<TH1D>("h1_nPV_data",     "h1_nPV_data",               60,-0.5,59.5);
  h1_nPV_mc       = fs->make<TH1D>("h1_nPV_mc",       "h1_nPV_mc",                 60,-0.5,59.5);
  h1_nPV_mc_unw   = fs->make<TH1D>("h1_nPV_mc_unw",   "h1_nPV_mc_unw",             60,-0.5,59.5);
  h1_pt_hat       = fs->make<TH1D>("h1_pt_hat",       "h1_pt_hat",                400,0,2000);
  h1_fatjet_pt_mc = fs->make<TH1D>("h1_fatjet_pt_mc", "h1_fatjet_pt_mc",          PtMax/10,0,PtMax);
  h1_nJet_data    = fs->make<TH1D>("h1_nJet_data",    "h1_nJet_data",             100,0,100);
  h1_nJet_mc      = fs->make<TH1D>("h1_nJet_mc",      "h1_nJet_mc",               100,0,100);


  AddHisto("FatJet_pt_all" ,"p_{T} of all fat jets"             ,PtMax/10  ,0      ,PtMax );
  AddHisto("FatJet_pt_sv"  ,"p_{T} of fat jets containing a SV" ,PtMax/10  ,0      ,PtMax);
  AddHisto("FatJet_eta"    ,"#eta of all fat jets"              ,50        ,-2.5   ,2.5);
  AddHisto("FatJet_phi"    ,"#phi of all fat jets"              ,40        ,-1.*pi ,pi);

  AddHisto("FatJet_muon_multi"   ,   "number of muons",           7, -0.5,6.5    );
  AddHisto("FatJet_muon_multi_sel",  "number of selected muons",  7, -0.5,6.5   );
  AddHisto("FatJet_mu_ptrel"  ,      "p_{T,rel} of the muon",    50,  0,5        );
  AddHisto("FatJet_mu_chi2"   ,      "norm. chi2 of the muon",   50,  0,10       );
  AddHisto("FatJet_muon_Pt",         "Muon p_{T}",               50,  0, 100  );
  AddHisto("FatJet_muon_eta",        "Muon #eta",                50, -2.5, 2.5 );
  AddHisto("FatJet_muon_phi",        "Muon #phi",                40, -1.*pi,pi);
  AddHisto("FatJet_muon_Ip3d",       "Muon 3D IP",               50, -0.1, 0.1 );
  AddHisto("FatJet_muon_Ip2d",       "Muon 2D IP",               50, -0.1, 0.1 );
  AddHisto("FatJet_muon_Sip3d",      "Muon 3D IP significance",  50, -35, 35   );
  AddHisto("FatJet_muon_Sip2d",      "Muon 2D IP significance",  50, -35, 35   );
  AddHisto("FatJet_muon_DeltaR",     "Muon1 #DeltaR",            50,0,0.5); //90

  AddHisto("FatJet_sv_deltaR_jet",      "sv_deltaR_jet",                                       50,0.,0.5    );
  AddHisto("FatJet_sv_deltaR_sumJet",   "SVvtxSumJetDeltaR",                                   50,0.,0.5    );
  AddHisto("FatJet_sv_deltaR_sumDir",   "SVvtxSumVtxDirDeltaR",                                50,0.,0.5    );
  AddHisto("FatJet_sv_en_ratio",        "Fractional energy",                                   50,0.,1.     );
  AddHisto("FatJet_sv_aboveC",          "IP significance 2D charm",                            50,-35.,35.  );
  AddHisto("FatJet_sv_pt",              "Vtx p_{T}",                                           50,0.,100.   );
  AddHisto("FatJet_sv_eta",             "Vtx #eta",                                            50, -2.5, 2.5);
  AddHisto("FatJet_sv_phi",             "Vtx #phi",                                            40, -1.*pi,pi);
  AddHisto("FatJet_sv_flightSig2D",     "Flight distance significance 2D",                     50,0.,80.    );
  AddHisto("FatJet_sv_flight2D",        "Flight distance 2D",                                  50,0.,2.5    );
  AddHisto("FatJet_sv_flightSig2D_3trk","Flight distance 2D with #geq3 tracks",                50,0.,2.5    );
  AddHisto("FatJet_sv_flight3D",        "Flight distance 3D",                                  50,0.,15.    );
  AddHisto("FatJet_sv_flight3DSig" ,    "flight distance significance 3D",                     50,0.,80.    );
  AddHisto("FatJet_sv_multi_0"       ,  "number of secondary vertex",                          6,-0.5,5.5   );
  AddHisto("FatJet_sv_multi"         ,  "number of secondary vertex",                          6,-0.5,5.5   );
  AddHisto("FatJet_sv_mass"   ,         "invariant mass of the secondary vertex",              50,0.,8.     );
  AddHisto("FatJet_sv_chi2norm"  ,      "normalized chi2 of the secondary vertex",             50,0.,10.    );
  AddHisto("FatJet_sv_tot_charge",      "Total charge",                                        21,-10.5,10.5);
  AddHisto("FatJet_svnTrk",             "Track multiplicity : SVnVertexTracks (centered)",     13,-0.5,12.5 );
  AddHisto("FatJet_svnTrk_firstVxt",    "Track multiplicity : SVnFirstVertexTracks (centered)",11,-0.5,10.5 );
  AddHisto("FatJet_sv_flight3Derr",     "Flight distance error 3D",                            50,0.,0.5);
  AddHisto("FatJet_sv_flight2Derr",     "Flight distance error 2D",                            50,0.,0.2);
  AddHisto("FatJet_sv_mass_3trk"   ,    "invariant mass of the secondary vertex with at least 3 SV tracks",  50,0.,8.);

  AddHisto("FatJet_track_multi"  ,      "number of tracks in the jets",                80,-0.5,79.5  );
  AddHisto("FatJet_trk_multi_sel"  ,    "number of selected tracks in the jets",       40,-0.5,39.5  );
  AddHisto("FatJet_track_chi2"   ,      "normalized chi2 of the tracks",               100,0.,5.    );
  AddHisto("FatJet_track_nHit" ,        "number of hits ",                             35,-0.5, 34.5 );
  AddHisto("FatJet_track_HPix"   ,      "number of hits in the Pixel",                 10,-0.5, 9.5  );

  AddHisto("FatJet_track_IPs"    ,      "3D IP significance of all tracks",            100,-35.,35.  );
  AddHisto("FatJet_track_IPs1tr" ,      "3D IP significance of the first track",       100,-35.,35.  );
  AddHisto("FatJet_track_IPs2tr" ,      "3D IP significance of the second track",      100,-35.,35.  );
  AddHisto("FatJet_track_IP"     ,      "3D IP of all tracks",                         100,-0.1,0.1);
  AddHisto("FatJet_track_IP1tr"  ,      "3D IP of the first track",                    100,-0.1,0.1);
  AddHisto("FatJet_track_IP2tr"  ,      "3D IP of the second track",                   100,-0.1,0.1);
  AddHisto("FatJet_track_IP2Ds"    ,    "2D IP significance of all tracks",            100,-35.,35.  );
  AddHisto("FatJet_track_IP2Ds1tr" ,    "2D IP significance of the first track",       100,-35.,35.  );
  AddHisto("FatJet_track_IP2Ds2tr" ,    "2D IP significance of the second track",      100,-35.,35.  );
  AddHisto("FatJet_track_IP2D"    ,     "2D IP of all tracks",                         100,-0.1,0.1);
  AddHisto("FatJet_track_IP2D1tr" ,     "2D IP of the first track",                    100,-0.1,0.1);
  AddHisto("FatJet_track_IP2D2tr" ,     "2D IP of the second track",                   100,-0.1,0.1);
  AddHisto("FatJet_track_IP2Derr1tr" ,  "2D IP error of the first track",              100,0,0.1);
  AddHisto("FatJet_track_IPerr1tr"   ,  "3D IP error of the first track",              100,0,0.1);
  AddHisto("FatJet_track_IP2Derr2tr" ,  "2D IP error of the second track",             100,0,0.1);
  AddHisto("FatJet_track_IPerr2tr"   ,  "3D IP error of the second track",             100,0,0.1);
  AddHisto("FatJet_track_IP2Derr" ,     "2D IP error",                                 100,0,0.1);
  AddHisto("FatJet_track_IPerr"   ,     "3D IP error",                                 100,0,0.1);
  AddHisto("FatJet_track_IPs3tr" ,      "3D IP significance of the third track",       100,-35.,35.  );
  AddHisto("FatJet_track_IP3tr"  ,      "3D IP of the third track",                    100,-0.1,0.1);
  AddHisto("FatJet_track_IPerr3tr"   ,  "3D IP error of the third track",              100,0,0.1);
  AddHisto("FatJet_track_IP2Ds3tr" ,    "2D IP significance of the second track",      100,-35.,35.  );
  AddHisto("FatJet_track_IP2D3tr" ,     "2D IP of the third track",                    100,-0.1,0.1);
  AddHisto("FatJet_track_IP2Derr3tr" ,  "2D IP error of the third track",              100,0,0.1);

  AddHisto("FatJet_track_len"     ,     "decay length",                    100,0,5.     );
  AddHisto("FatJet_track_dist"    ,     "distance to the jet axis",        100,0.,0.08    );
  AddHisto("FatJet_track_dz"     ,      "transverse IP",                   100,-3,3  );
  AddHisto("FatJet_track_isfromSV",     "Track is from SV",                  2,-0.5, 1.5   );
  AddHisto("FatJet_track_pt"         ,  "pT of all the tracks",             80,0.,200.    );
  AddHisto("FatJet_track_pt15"   ,      "pT of all the tracks",            150,0.,15.    );
  AddHisto("FatJet_track_chi2_cut"     ,"normalized chi2 ",                100,0.,30.    );
  AddHisto("FatJet_track_nHit_cut"   ,  "number of hits  ",                 35,-0.5, 34.5 );
  AddHisto("FatJet_track_HPix_cut"     ,"number of hits in the Pixel ",     10,-0.5, 9.5  );
  AddHisto("FatJet_track_len_cut"      ,"decay length ",                   100,0,25.     );
  AddHisto("FatJet_track_dist_cut"     ,"distance to the jet axis ",       100,0.,0.3    );
  AddHisto("FatJet_track_dz_cut"       ,"transverse IP ",                  100,-20., 20.  );
  AddHisto("FatJet_track_pt_cut"       ,"pT ",                              80,0.,200.);
  AddHisto("FatJet_track_pt15_cut"    , "pT of all the tracks",            150,0.,15.    );
  AddHisto("FatJet_track_IP2D_cut"     ,"IP2D ",                           100,-1.,1.);

  AddHisto("FatJet_TCHE"      ,         "TCHE",                             50,0.,30. );
  AddHisto("FatJet_TCHP"      ,         "TCHP",                             50,0.,30. );
  AddHisto("FatJet_JP"        ,         "JP",                               50,0.,2.5 );
  AddHisto("FatJet_JBP"       ,         "JBP",                              50,0.,8.  );
  AddHisto("FatJet_SSV"       ,         "SSVHE",                            70,0.,7.  );
  AddHisto("FatJet_SSVHP"     ,         "SSVHP",                            70,0.,7.  );
  AddHisto("FatJet_CSV"       ,         "CSV",                              50,0.,1.  );

  AddHisto("FatJet_TCHE_extended1"   ,  "TCHE_extended1",                   70, -30.,30. );
  AddHisto("FatJet_TCHP_extended1"   ,  "TCHP_extended1",                   70, -30.,30. );
  AddHisto("FatJet_TCHE_extended2"   ,  "TCHE_extended2",                  100,-30.,30. );
  AddHisto("FatJet_TCHP_extended2"   ,  "TCHP_extended2",                  100,-30.,30. );
  AddHisto("FatJet_discri_ssche0",      "SSVHE Discriminator",              80, -1., 7.   );
  AddHisto("FatJet_discri_sschp0",      "SSVHP Discriminator",              80, -1., 7.   );

  AddHisto("FatJet_SoftMu"    ,         "SoftMu",                           50,0.,1.  );
  AddHisto("FatJet_SoftEl"    ,         "SoftEl",                           50,0.,1.  );
  AddHisto("FatJet_CombCSVJP"        ,  "CombCSVJP",                        50,0.,1.  );
  AddHisto("FatJet_CombCSVSL"        ,  "CombCSVSL",                        50,0.,1.  );
  AddHisto("FatJet_CombCSVJPSL"  ,      "CombCSVJPSL",                      50,0.,1.  );
  AddHisto("FatJet_RetCombSvx"   ,      "RetCombSvx",                       50,0.,1.  );

  AddHisto2D("FatJet_seltrack_vs_jetpt", "sel track multiplicity vs jet pt",         PtMax/20,0,PtMax, 100,-0.5,99.5);
  AddHisto2D("FatJet_sv_mass_vs_flightDist3D", " SVMass vs SV 3D flight distance ",  50,0, 10,60,0,6);
  AddHisto2D("FatJet_avg_sv_mass_vs_jetpt","Avg SVMass vs jet pt",                   PtMax/20,0,PtMax, 100,0,6);
  AddHisto2D("FatJet_sv_deltar_jet_vs_jetpt","SVJetDeltaR vs jet pt",                PtMax/20,0,PtMax, 50,0.,0.5);
  AddHisto2D("FatJet_sv_deltar_sum_jet_vs_jetpt","SVvtxSumJetDeltaR vs jet pt",      PtMax/20,0,PtMax, 50,0.,0.5);
  AddHisto2D("FatJet_sv_deltar_sum_dir_vs_jetpt","SVvtxSumVtxDirDeltaR vs jet pt",   PtMax/20,0,PtMax, 50,0.,0.5);
  AddHisto2D("FatJet_muon_ptrel_vs_jetpt","Muon_p{T}^{rel} vs jet pt",               PtMax/20,0,PtMax,50,0,5);
  AddHisto2D("FatJet_muon_DeltaR_vs_jetpt","Muon1 DeltaR vs jet pt",                 PtMax/20,0,PtMax,50,0,0.5);

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

    double wtPU(1) ; 
    if ( doPUReweighting_ && !isData ) {
      wtPU = LumiWeights_.weight(EvtInfo.nPUtrue) ;  
    }

    h1_CutFlow->Fill(0.,wtPU); //// count all events
    if( !isData ) h1_pt_hat->Fill(EvtInfo.pthat,wtPU);

    if( !passTrigger() ) continue; //// apply trigger selection

    h1_CutFlow->Fill(1.,wtPU); //// count events passing trigger selection

    if(FatJetInfo.nJet <= 0) continue; //// require at least 1 jet in the event

    if( isData )
      h1_nPV_data->Fill(EvtInfo.nPV);
    else
    {
      h1_nPUtrue_mc->Fill(EvtInfo.nPUtrue,wtPU);
      h1_nPV_mc    ->Fill(EvtInfo.nPV,wtPU);
      h1_nPV_mc_unw->Fill(EvtInfo.nPV);
    }

    int njet_mc   = 0;
    int njet_data = 0;

    double jetCone(0.8);

    //---------------------------- Start jet loop ---------------------------------------//
    for(int iJet = 0; iJet < FatJetInfo.nJet; ++iJet)
    {
      float ptjet    = FatJetInfo.Jet_pt[iJet];
      float etajet   = FatJetInfo.Jet_eta[iJet];
      float phijet   = FatJetInfo.Jet_phi[iJet];
      float ntrkjet  = FatJetInfo.Jet_ntracks[iJet];
      int   flav     = FatJetInfo.Jet_flavour[iJet];

      if ( ptjet < jetPtMin_ || ptjet > jetPtMax_   ) continue;
      if ( fabs(etajet) > jetAbsEtaMax_ )             continue;

      int idxFirstMuon = -1;
      int nselmuon = 0;
      int nmu = 0;
      if (FatJetInfo.nMuon>0)
      {
        for (int iMu=0; iMu<FatJetInfo.nMuon; ++iMu)
        {
          if (FatJetInfo.Muon_IdxJet[iMu]==iJet )
          {
            ++nmu;
            if (passMuonSelection(iMu, iJet, FatJetInfo))
            {
              if(nselmuon == 0)
                idxFirstMuon = iMu;
              ++nselmuon;
            }
          }
        }
      }

      //// jet multiplicity
      if( !isData )
      {
        ++njet_mc;
        h1_fatjet_pt_mc->Fill(ptjet,wtPU);
      }
      else ++njet_data;

      TLorentzVector jet_p4;
      jet_p4.SetPtEtaPhiM(FatJetInfo.Jet_pt[iJet], FatJetInfo.Jet_eta[iJet], FatJetInfo.Jet_phi[iJet], FatJetInfo.Jet_mass[iJet]);

      //// test if the jet is a gluon splitting b jet
      bool isGluonSplit(false);
      int nMatchedBHadrons(0);
      for (int iB = 0; iB < EvtInfo.nBHadrons; ++iB)
      {
        if (EvtInfo.BHadron_hasBdaughter[iB]==1) continue; //// skip excited B hadrons
        TLorentzVector bhad_p4;
        bhad_p4.SetPtEtaPhiM(EvtInfo.BHadron_pT[iB], EvtInfo.BHadron_eta[iB], EvtInfo.BHadron_phi[iB], EvtInfo.BHadron_mass[iB]);
        if ( jet_p4.DeltaR(bhad_p4) < jetCone ) ++nMatchedBHadrons;
      }
      if ( nMatchedBHadrons > 1 ) isGluonSplit = true ; 

      FillHisto("FatJet_pt_all", flav, isGluonSplit , ptjet , wtPU) ; 
      if (FatJetInfo.Jet_SV_multi[iJet] > 0) FillHisto("FatJet_pt_sv", flav, isGluonSplit , ptjet , wtPU) ; 

      FillHisto("FatJet_eta"         ,flav ,isGluonSplit ,etajet  ,wtPU) ; 
      FillHisto("FatJet_phi"         ,flav ,isGluonSplit ,phijet  ,wtPU) ; 
      FillHisto("FatJet_track_multi" ,flav ,isGluonSplit ,ntrkjet ,wtPU) ; 

      float mass_sv        = 0.;
      int   n_sv           = 0 ;
      float chi2norm_sv    = 0.;
      float flightSig_sv   = 0.;
      float flight2DSig_sv = 0.;
      float sv_dR_jet      = 0.;
      float sv_dR_dir_sum  = 0.;
      float sv_dR_jet_sum  = 0.;
      float sv_en_rat      = 0.;
      float sv_abovC       = 0.;
      float sv_pt          = 0.;
      float sveta          = 0.;
      float svphi          = 0.;
      float sv_flight3D    = 0.;
      float sv_flight3Derr = 0.;
      float sv_flight2D    = 0.;
      float sv_flight2Derr = 0.;
      int   sv_totchar     = 0;
      float sv_nTrk        = 0.;
      float sv_1st_nTrk    = 0.;

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
        for (int iTrk=FatJetInfo.Jet_nFirstTrack[iJet]; iTrk<FatJetInfo.Jet_nLastTrack[iJet] ; ++iTrk)
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

          if (FatJetInfo.Track_nHitAll[iTrk]>=8)      passNhit=true;
          if (FatJetInfo.Track_nHitPixel[iTrk]>=2)    passPix= true;
          if (fabs(FatJetInfo.Track_dz[iTrk])<17)     passIPz=true;
          if (FatJetInfo.Track_pt[iTrk]>1)            passPt=true;
          if (FatJetInfo.Track_chi2[iTrk]<5)          passnormchi2=true;
          if (fabs(FatJetInfo.Track_dist[iTrk])<0.07) passtrkdist=true;
          if (FatJetInfo.Track_length[iTrk]<5)        passtrklen=true;
          if (fabs(FatJetInfo.Track_IP2D[iTrk])<0.2)  passTrackIP2D=true;

          if (passNhit && passPix && passIPz && passPt && passnormchi2 && passtrkdist && passTrackIP2D)
            FillHisto("FatJet_track_len_cut",          flav, isGluonSplit ,FatJetInfo.Track_length[iTrk] , wtPU);

          if (passNhit && passPix && passIPz && passPt && passnormchi2 && passtrklen && passTrackIP2D)
            FillHisto("FatJet_track_dist_cut",         flav, isGluonSplit ,fabs(FatJetInfo.Track_dist[iTrk])   , wtPU);

          if (passNhit && passPix && passIPz && passPt && passtrkdist && passtrklen && passTrackIP2D)
            FillHisto("FatJet_track_chi2_cut",         flav, isGluonSplit ,FatJetInfo.Track_chi2[iTrk]         , wtPU);

          if (passNhit && passPix && passIPz && passnormchi2 && passtrkdist && passtrklen && passTrackIP2D)
          {
            FillHisto("FatJet_track_pt_cut",           flav, isGluonSplit ,FatJetInfo.Track_pt[iTrk]     , wtPU);
            FillHisto("FatJet_track_pt15_cut",         flav, isGluonSplit ,FatJetInfo.Track_pt[iTrk]     , wtPU);
          }

          if (passNhit && passPix && passPt && passnormchi2 && passtrkdist && passtrklen)
            FillHisto("FatJet_track_dz_cut",          flav, isGluonSplit ,FatJetInfo.Track_dz[iTrk]      ,wtPU);

          if (passNhit && passIPz && passPt && passnormchi2 && passtrkdist && passtrklen && passTrackIP2D)
            FillHisto("FatJet_track_HPix_cut",         flav, isGluonSplit ,FatJetInfo.Track_nHitPixel[iTrk],wtPU);

          if (passPix && passIPz && passPt && passnormchi2 && passtrkdist && passtrklen && passTrackIP2D)
            FillHisto("FatJet_track_nHit_cut",       flav, isGluonSplit ,FatJetInfo.Track_nHitAll[iTrk],wtPU);

          if (passNhit && passPix && passIPz && passPt && passnormchi2 && passtrkdist && passtrklen )
            FillHisto("FatJet_track_IP2D_cut",         flav, isGluonSplit ,FatJetInfo.Track_IP2D[iTrk],wtPU);

          // -------------------- start selected tracks -------------------------
          if (passNhit && passPix && passIPz && passPt && passnormchi2 && passtrkdist && passtrklen && passTrackIP2D)
          {
            ++ntracksel;

            FillHisto("FatJet_track_chi2",    flav, isGluonSplit ,FatJetInfo.Track_chi2[iTrk]      ,wtPU);
            FillHisto("FatJet_track_nHit",    flav, isGluonSplit ,FatJetInfo.Track_nHitAll[iTrk]   ,wtPU);
            FillHisto("FatJet_track_HPix",    flav, isGluonSplit ,FatJetInfo.Track_nHitPixel[iTrk] ,wtPU);
            FillHisto("FatJet_track_IPs",     flav, isGluonSplit ,FatJetInfo.Track_IPsig[iTrk]     ,wtPU);
            FillHisto("FatJet_track_IP",      flav, isGluonSplit ,FatJetInfo.Track_IP[iTrk]        ,wtPU);
            FillHisto("FatJet_track_IP2Ds",   flav, isGluonSplit ,FatJetInfo.Track_IP2Dsig[iTrk]   ,wtPU);
            FillHisto("FatJet_track_IP2D",    flav, isGluonSplit, FatJetInfo.Track_IP2D[iTrk]      ,wtPU);
            FillHisto("FatJet_track_IP2Derr", flav, isGluonSplit, FatJetInfo.Track_IP2Derr[iTrk]   ,wtPU);
            FillHisto("FatJet_track_IPerr",   flav, isGluonSplit, FatJetInfo.Track_IPerr[iTrk]     ,wtPU);
            FillHisto("FatJet_track_dz",      flav, isGluonSplit ,FatJetInfo.Track_dz[iTrk]        ,wtPU);
            FillHisto("FatJet_track_isfromSV",flav, isGluonSplit ,FatJetInfo.Track_isfromSV[iTrk]  ,wtPU);
            FillHisto("FatJet_track_len",     flav, isGluonSplit ,FatJetInfo.Track_length[iTrk]    ,wtPU);
            FillHisto("FatJet_track_dist",    flav, isGluonSplit ,fabs(FatJetInfo.Track_dist[iTrk]),wtPU);
            FillHisto("FatJet_track_pt",      flav, isGluonSplit ,FatJetInfo.Track_pt[iTrk]        ,wtPU);
            FillHisto("FatJet_track_pt15",    flav, isGluonSplit ,FatJetInfo.Track_pt[iTrk]        ,wtPU);

            // tracks sorted by IP
            float sig   = FatJetInfo.Track_IP[iTrk]/FatJetInfo.Track_IPerr[iTrk];
            float sig2D = FatJetInfo.Track_IP2D[iTrk]/FatJetInfo.Track_IP2Derr[iTrk];
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
          FillHisto("FatJet_track_IPs1tr",    flav, isGluonSplit ,sig1_ip                          , wtPU) ;
          FillHisto("FatJet_track_IP1tr",     flav, isGluonSplit ,FatJetInfo.Track_IP[n1_ip]       , wtPU) ;
          FillHisto("FatJet_track_IPerr1tr",  flav, isGluonSplit ,FatJetInfo.Track_IPerr[n1_ip]    , wtPU) ;
          FillHisto("FatJet_track_IP2Ds1tr",  flav, isGluonSplit ,sig12D_ip                        , wtPU) ;
          FillHisto("FatJet_track_IP2D1tr",   flav, isGluonSplit ,FatJetInfo.Track_IP2D[n1_ip]     , wtPU) ;
          FillHisto("FatJet_track_IP2Derr1tr",flav, isGluonSplit ,FatJetInfo.Track_IP2Derr[n1_ip]  , wtPU) ;
        }

        if (n2_ip>-1) {
          FillHisto("FatJet_track_IPs2tr",    flav, isGluonSplit ,sig2_ip                          , wtPU) ;
          FillHisto("FatJet_track_IP2tr",     flav, isGluonSplit ,FatJetInfo.Track_IP[n2_ip]       , wtPU) ;
          FillHisto("FatJet_track_IPerr2tr",  flav, isGluonSplit ,FatJetInfo.Track_IPerr[n2_ip]    , wtPU) ;
          FillHisto("FatJet_track_IP2Ds2tr",  flav, isGluonSplit ,sig22D_ip                        , wtPU) ;
          FillHisto("FatJet_track_IP2D2tr",   flav, isGluonSplit ,FatJetInfo.Track_IP2D[n2_ip]     , wtPU) ;
          FillHisto("FatJet_track_IP2Derr2tr",flav, isGluonSplit ,FatJetInfo.Track_IP2Derr[n2_ip]  , wtPU) ;
        }

        if (n3_ip>-1) {
          FillHisto("FatJet_track_IPs3tr",    flav, isGluonSplit ,sig3_ip                          , wtPU) ;
          FillHisto("FatJet_track_IP3tr",     flav, isGluonSplit ,FatJetInfo.Track_IP[n3_ip]       , wtPU) ;
          FillHisto("FatJet_track_IPerr3tr",  flav, isGluonSplit ,FatJetInfo.Track_IPerr[n3_ip]    , wtPU) ;
          FillHisto("FatJet_track_IP2Ds3tr",  flav, isGluonSplit ,sig32D_ip                        , wtPU) ;
          FillHisto("FatJet_track_IP2D3tr",   flav, isGluonSplit ,FatJetInfo.Track_IP2D[n3_ip]     , wtPU) ;
          FillHisto("FatJet_track_IP2Derr3tr",flav, isGluonSplit ,FatJetInfo.Track_IP2Derr[n3_ip]  , wtPU) ;
        }

        FillHisto("FatJet_trk_multi_sel",     flav, isGluonSplit , ntracksel           , wtPU);
        FillHisto2D("FatJet_seltrack_vs_jetpt", flav, isGluonSplit ,ptjet ,  ntracksel , wtPU);

        // ------------------------------------------------
        // -------------- SV information ------------------
        // ------------------------------------------------
        n_sv = FatJetInfo.Jet_SV_multi[iJet];
        FillHisto("FatJet_sv_multi_0",      flav, isGluonSplit ,n_sv   ,         wtPU);

        if (n_sv>0)
        {
          chi2norm_sv    = FatJetInfo.SV_chi2[FatJetInfo.Jet_nFirstSV[iJet]]/FatJetInfo.SV_ndf[FatJetInfo.Jet_nFirstSV[iJet]];
          flightSig_sv   = FatJetInfo.SV_flight[FatJetInfo.Jet_nFirstSV[iJet]]/FatJetInfo.SV_flightErr[FatJetInfo.Jet_nFirstSV[iJet]];
          flight2DSig_sv = FatJetInfo.SV_flight2D[FatJetInfo.Jet_nFirstSV[iJet]]/FatJetInfo.SV_flight2DErr[FatJetInfo.Jet_nFirstSV[iJet]];
          mass_sv        = FatJetInfo.SV_mass[FatJetInfo.Jet_nFirstSV[iJet]];
          sv_dR_jet      = FatJetInfo.SV_deltaR_jet[FatJetInfo.Jet_nFirstSV[iJet]];
          sv_dR_dir_sum  = FatJetInfo.SV_deltaR_sum_dir[FatJetInfo.Jet_nFirstSV[iJet]];
          sv_dR_jet_sum  = FatJetInfo.SV_deltaR_sum_jet[FatJetInfo.Jet_nFirstSV[iJet]];
          sv_en_rat      = FatJetInfo.SV_energy_ratio[FatJetInfo.Jet_nFirstSV[iJet]];
          sv_abovC       = FatJetInfo.SV_aboveC[FatJetInfo.Jet_nFirstSV[iJet]];
          sv_pt          = FatJetInfo.SV_vtx_pt[FatJetInfo.Jet_nFirstSV[iJet]];
          sveta          = FatJetInfo.SV_vtx_eta[FatJetInfo.Jet_nFirstSV[iJet]];
          svphi          = FatJetInfo.SV_vtx_phi[FatJetInfo.Jet_nFirstSV[iJet]];

          sv_flight3D     = FatJetInfo.SV_flight[FatJetInfo.Jet_nFirstSV[iJet]] ;
          sv_flight3Derr  = FatJetInfo.SV_flightErr[FatJetInfo.Jet_nFirstSV[iJet]];
          sv_flight2D     = FatJetInfo.SV_flight2D[FatJetInfo.Jet_nFirstSV[iJet]];
          sv_flight2Derr  = FatJetInfo.SV_flight2DErr[FatJetInfo.Jet_nFirstSV[iJet]];
          sv_totchar      = FatJetInfo.SV_totCharge[FatJetInfo.Jet_nFirstSV[iJet]] ;

          sv_nTrk         = FatJetInfo.SV_nTrk[FatJetInfo.Jet_nFirstSV[iJet]] ;
          sv_1st_nTrk     = FatJetInfo.SV_nTrk_firstVxt[FatJetInfo.Jet_nFirstSV[iJet]];

          // --------- SV histograms --------
          FillHisto("FatJet_sv_multi",        flav, isGluonSplit ,n_sv ,  wtPU);
          FillHisto("FatJet_sv_chi2norm",     flav, isGluonSplit ,chi2norm_sv        , wtPU);
          FillHisto("FatJet_sv_mass",         flav, isGluonSplit ,mass_sv,             wtPU);
          FillHisto("FatJet_sv_deltaR_jet",   flav, isGluonSplit ,sv_dR_jet,           wtPU);
          FillHisto("FatJet_sv_deltaR_sumJet",flav, isGluonSplit ,sv_dR_dir_sum,       wtPU);
          FillHisto("FatJet_sv_deltaR_sumDir",flav, isGluonSplit ,sv_dR_jet_sum,       wtPU);
          FillHisto("FatJet_sv_en_ratio",     flav, isGluonSplit ,sv_en_rat,           wtPU);
          FillHisto("FatJet_sv_aboveC",       flav, isGluonSplit ,sv_abovC,            wtPU);
          FillHisto("FatJet_sv_pt",           flav, isGluonSplit ,sv_pt,               wtPU);
          FillHisto("FatJet_sv_flight2D",     flav, isGluonSplit ,sv_flight2D,         wtPU);
          FillHisto("FatJet_sv_flight2Derr",  flav, isGluonSplit ,sv_flight2Derr,      wtPU);
          FillHisto("FatJet_sv_flightSig2D",  flav, isGluonSplit ,flight2DSig_sv,      wtPU);
          FillHisto("FatJet_sv_tot_charge",   flav, isGluonSplit ,sv_totchar,          wtPU);
          FillHisto("FatJet_svnTrk",          flav, isGluonSplit ,sv_nTrk,             wtPU);
          FillHisto("FatJet_svnTrk_firstVxt", flav, isGluonSplit ,sv_1st_nTrk,         wtPU);
          FillHisto("FatJet_sv_eta",          flav, isGluonSplit ,sveta,               wtPU);
          FillHisto("FatJet_sv_phi",          flav, isGluonSplit ,svphi,               wtPU);
          FillHisto("FatJet_sv_flight3D",     flav, isGluonSplit ,sv_flight3D,         wtPU);
          FillHisto("FatJet_sv_flight3Derr",  flav, isGluonSplit ,sv_flight3Derr,      wtPU);
          FillHisto("FatJet_sv_flight3DSig",  flav, isGluonSplit ,flightSig_sv,        wtPU);

          if (sv_nTrk >2)
          {
            FillHisto("FatJet_sv_mass_3trk", flav, isGluonSplit ,mass_sv,wtPU);
            FillHisto("FatJet_sv_flightSig2D_3trk",  flav, isGluonSplit ,flight2DSig_sv,        wtPU);
          }

          FillHisto2D("sv_mass_vs_flightDist3D"     ,flav,isGluonSplit ,sv_flight3D,mass_sv,wtPU);
          FillHisto2D("FatJet_avg_sv_mass_vs_jetpt"        ,flav,isGluonSplit ,ptjet,mass_sv,wtPU);
          FillHisto2D("sv_deltar_jet_vs_jetpt"      ,flav,isGluonSplit ,ptjet,sv_dR_jet,wtPU);
          FillHisto2D("sv_deltar_sum_jet_vs_jetpt"  ,flav,isGluonSplit ,ptjet,sv_dR_dir_sum,wtPU);
          FillHisto2D("sv_deltar_sum_dir_vs_jetpt"  ,flav,isGluonSplit ,ptjet,sv_dR_jet_sum,wtPU);
        }
      } // end useJetProbaTree

      // ------------------------------------------------
      // ------------- tagger information ---------------
      // ------------------------------------------------
      float tche      = FatJetInfo.Jet_Ip2P[iJet];
      float tchp      = FatJetInfo.Jet_Ip3P[iJet];
      float jetproba  = FatJetInfo.Jet_Proba[iJet];
      float jetbproba = FatJetInfo.Jet_Bprob[iJet];
      float ssvhe     = FatJetInfo.Jet_Svx[iJet] ;
      float ssvhp     = FatJetInfo.Jet_SvxHP[iJet];
      float csv       = FatJetInfo.Jet_CombSvx[iJet];

      FillHisto("FatJet_TCHE",  flav, isGluonSplit, tche   ,   wtPU);
      FillHisto("FatJet_TCHP",  flav, isGluonSplit, tchp   ,   wtPU);
      FillHisto("FatJet_JP",    flav, isGluonSplit, jetproba  , wtPU);
      FillHisto("FatJet_JBP",   flav, isGluonSplit, jetbproba , wtPU);
      FillHisto("FatJet_SSV",   flav, isGluonSplit, ssvhe         ,   wtPU);
      FillHisto("FatJet_SSVHP", flav, isGluonSplit, ssvhp         ,   wtPU);
      FillHisto("FatJet_CSV",   flav, isGluonSplit, csv    ,   wtPU);

      FillHisto("FatJet_TCHE_extended1",  flav, isGluonSplit, tche  , wtPU);
      FillHisto("FatJet_TCHP_extended1",  flav, isGluonSplit, tchp  , wtPU);
      FillHisto("FatJet_TCHE_extended2",  flav, isGluonSplit, tche  , wtPU);
      FillHisto("FatJet_TCHP_extended2",  flav, isGluonSplit, tchp  , wtPU);
      FillHisto("FatJet_discri_ssche0",   flav, isGluonSplit, ssvhe , wtPU);
      FillHisto("FatJet_discri_sschp0",   flav, isGluonSplit, ssvhp , wtPU);

      float softmu       = FatJetInfo.Jet_SoftMu[iJet];
      float solfel       = FatJetInfo.Jet_SoftEl[iJet];
      float combjp       = FatJetInfo.Jet_CombCSVJP[iJet];
      float combsl       = FatJetInfo.Jet_CombCSVSL[iJet];
      float combjpsl     = FatJetInfo.Jet_CombCSVJPSL[iJet];
      float retrainedcsv = FatJetInfo.Jet_RetCombSvx[iJet];

      FillHisto("FatJet_SoftMu",      flav, isGluonSplit, softmu  ,   wtPU);
      FillHisto("FatJet_SoftEl",      flav, isGluonSplit, solfel  ,   wtPU);
      FillHisto("FatJet_CombCSVJP",   flav, isGluonSplit, combjp  ,   wtPU);
      FillHisto("FatJet_CombCSVSL",   flav, isGluonSplit, combsl  ,   wtPU);
      FillHisto("FatJet_CombCSVJPSL", flav, isGluonSplit, combjpsl  ,   wtPU);
      FillHisto("FatJet_RetCombSvx",  flav, isGluonSplit, retrainedcsv  ,   wtPU);

      // ------------------------------------------------
      // ------------- muon information -----------------
      // ------------------------------------------------
      FillHisto("FatJet_muon_multi_sel",  flav, isGluonSplit , nselmuon   ,wtPU);
      FillHisto("FatJet_muon_multi",      flav, isGluonSplit , nmu        ,wtPU);

      if(idxFirstMuon > -1)
      {
        FillHisto("FatJet_mu_ptrel",    flav, isGluonSplit ,FatJetInfo.Muon_ptrel[idxFirstMuon] ,wtPU);
        FillHisto("FatJet_mu_chi2",     flav, isGluonSplit ,FatJetInfo.Muon_chi2[idxFirstMuon]  ,wtPU);
        FillHisto("FatJet_muon_Pt",     flav, isGluonSplit, FatJetInfo.Muon_pt[idxFirstMuon] ,     wtPU);
        FillHisto("FatJet_muon_eta",    flav, isGluonSplit, FatJetInfo.Muon_eta[idxFirstMuon] ,    wtPU);
        FillHisto("FatJet_muon_phi",    flav, isGluonSplit, FatJetInfo.Muon_phi[idxFirstMuon] ,    wtPU);
        FillHisto("FatJet_muon_Ip3d",   flav, isGluonSplit, FatJetInfo.Muon_IP[idxFirstMuon] ,     wtPU);
        FillHisto("FatJet_muon_Ip2d",   flav, isGluonSplit, FatJetInfo.Muon_IP2D[idxFirstMuon] ,   wtPU);
        FillHisto("FatJet_muon_Sip3d",  flav, isGluonSplit, FatJetInfo.Muon_IPsig[idxFirstMuon] ,  wtPU);
        FillHisto("FatJet_muon_Sip2d",  flav, isGluonSplit, FatJetInfo.Muon_IP2Dsig[idxFirstMuon] ,wtPU);

        TLorentzVector themuon, thejet;

        thejet.SetPtEtaPhiM(FatJetInfo.Jet_pt[iJet], FatJetInfo.Jet_eta[iJet], FatJetInfo.Jet_phi[iJet], FatJetInfo.Jet_mass[iJet]);
        themuon.SetPtEtaPhiM(FatJetInfo.Muon_pt[idxFirstMuon], FatJetInfo.Muon_eta[idxFirstMuon], FatJetInfo.Muon_phi[idxFirstMuon], 0);

        FillHisto("FatJet_muon_DeltaR",         flav, isGluonSplit, themuon.DeltaR(thejet) , wtPU);
        FillHisto2D("FatJet_muon_ptrel_vs_jetpt", flav, isGluonSplit,ptjet,FatJetInfo.Muon_ptrel[idxFirstMuon],wtPU);
        FillHisto2D("FatJet_muon_DeltaR_vs_jetpt",flav, isGluonSplit,ptjet,themuon.DeltaR(thejet),wtPU);
      }
    }
    //----------------------------- End jet loop ----------------------------------------//

    // fill jet multiplicity
    if( isData ) h1_nJet_data->Fill(njet_data);
    else         h1_nJet_mc->Fill(njet_mc, wtPU);
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
    if( isGS )
      HistoBtag_map[name+"_bfromg"]->Fill(double(value),weight);
    else
    {
      if (fabs(flavour)==5)                           HistoBtag_map[name+"_b"]     ->Fill(double(value),weight);
      else if (fabs(flavour)==4)                      HistoBtag_map[name+"_c"]     ->Fill(double(value),weight);
      else if (fabs(flavour)< 4 || fabs(flavour)==21) HistoBtag_map[name+"_l"]     ->Fill(double(value),weight);
    }
  }
  else                                              HistoBtag_map[name+"_data"]  ->Fill(double(value));
}

// ------------------------------------------------------------------------------
template <class Type1, class Type2>
void BTagValidation::FillHisto2D(TString name, int flavour, bool isGS, Type1 value, Type2 value2, double weight)
{
  if (!isData){
    if( isGS )
      HistoBtag2D_map[name+"_bfromg"]->Fill(double(value),double(value2),weight);
    else
    {
      if (fabs(flavour)==5)                           HistoBtag2D_map[name+"_b"]     ->Fill(double(value),double(value2),weight);
      else if (fabs(flavour)==4)                      HistoBtag2D_map[name+"_c"]     ->Fill(double(value),double(value2),weight);
      else if (fabs(flavour)< 4 || fabs(flavour)==21) HistoBtag2D_map[name+"_l"]     ->Fill(double(value),double(value2),weight);
    }
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

// ------------------------------------------------------------------------------
bool BTagValidation::passMuonSelection(int muIdx, int iJet, const JetInfoBranches& JetInfo)
{
  TLorentzVector muon, jet;

  jet.SetPtEtaPhiM(JetInfo.Jet_pt[iJet], JetInfo.Jet_eta[iJet], JetInfo.Jet_phi[iJet], JetInfo.Jet_mass[iJet]);
  muon.SetPtEtaPhiM(JetInfo.Muon_pt[muIdx], JetInfo.Muon_eta[muIdx], JetInfo.Muon_phi[muIdx], 0);

  bool cut_mu_pass = false;
  if (JetInfo.Muon_pt[muIdx] > 5 && fabs(JetInfo.Muon_eta[muIdx]) < 2.4 && JetInfo.Muon_isGlobal[muIdx] == 1 &&
      JetInfo.Muon_nMuHit[muIdx] > 0 && JetInfo.Muon_nMatched[muIdx] > 1 && JetInfo.Muon_nTkHit[muIdx] > 10 &&
      JetInfo.Muon_nPixHit[muIdx] > 1 && JetInfo.Muon_nOutHit[muIdx] < 3 && JetInfo.Muon_chi2Tk[muIdx] < 10 &&
      JetInfo.Muon_chi2[muIdx] < 10  && // JetInfo.Muon_vz[muIdx]< 2 &&
      jet.DeltaR(muon) < 0.4 &&
      fabs(JetInfo.Muon_vz[muIdx]-EvtInfo.PVz) < 2.)
    cut_mu_pass = true ;

  return cut_mu_pass;
}

//define this as a plug-in
DEFINE_FWK_MODULE(BTagValidation);
