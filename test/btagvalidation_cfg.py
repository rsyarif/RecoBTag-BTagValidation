import FWCore.ParameterSet.Config as cms

from FWCore.ParameterSet.VarParsing import VarParsing

options = VarParsing('python')

options.register('outFilename', 'bTagValPlots.root',
    VarParsing.multiplicity.singleton,
    VarParsing.varType.string,
    "Output file name"
)
options.register('reportEvery', 1000,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.int,
    "Report every N events (default is N=1000)"
)
options.register('triggerSelection', '',
    VarParsing.multiplicity.list,
    VarParsing.varType.string,
    "Trigger selection"
)
options.register('useJetProbaTree', False,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "Use jet probability tree"
)
options.register('applyFatJetMuonTagging', False,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "Apply muon tagging to fat jets"
)
options.register('applyFatJetBTagging', False,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "Apply b tagging to fat jets"
)
options.register('fatJetDoubleTagging', False,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "Require fat jets to be double-tagged"
)
options.register('fatJetDoubleBTagging', False,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "Require fat jets to be double-b-tagged"
)
options.register('usePrunedSubjets', False,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "Process pruned subjets"
)
options.register('useSoftDropSubjets', False,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "Process soft drop subjets"
)
options.register('applySubJetMuonTagging', False,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "Apply muon tagging to subjets"
)
options.register('applySubJetBTagging', False,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "Apply b tagging to subjets"
)
options.register('dynamicMuonSubJetDR', False,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "Use dynamic muon-subjet dR requirement"
)
options.register('applySFs', False,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "Apply b-tagging SFs"
)
options.register('useFlavorCategories', True,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "Use flavor categories for MC distributions"
)
options.register('useRelaxedMuonID', True,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "Use relaxed muon ID"
)
options.register('fatJetPtMin', 300.,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.float,
    "Minimum fat jet Pt"
)
options.register('fatJetPtMax', 1.E6,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.float,
    "Maximum fat jet Pt"
)
options.register('fatJetSoftDropMassMin', 0.,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.float,
    "Minimum fat jet softdrop mass"
)
options.register('fatJetSoftDropMassMax', 1.E6,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.float,
    "Maximum fat jet softdrop mass"
)
options.register('fatJetTau21Min', 0.0, #added by rizki
    VarParsing.multiplicity.singleton,
    VarParsing.varType.float,
    "tau2/tau1 jet substructure min cut for fat jets"
)
options.register('fatJetTau21Max', 0.5, #added by rizki
    VarParsing.multiplicity.singleton,
    VarParsing.varType.float,
    "tau2/tau1 jet substructure max cut for fat jets"
)
options.register('fatJetBDiscrCut', 0.244,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.float,
    "B discriminator cut for fat jets"
)
options.register('subJetBDiscrCut', 0.244,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.float,
    "B discriminator cut for fat jets"
)
options.register('SFbShift', 0.,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.float,
    "Shift in SFb in units of sigmas"
)
options.register('SFlShift', 0.,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.float,
    "Shift in SFl in units of sigmas"
)
options.register('doPUReweightingOfficial', False,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "Do pileup reweighting"
)
options.register('doPUReweightingNPV', False,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "Do pileup reweighting"
)

## 'maxEvents' is already registered by the Framework, changing default value
options.setDefault('maxEvents', -1)

options.parseArguments()

if options.usePrunedSubjets and options.useSoftDropSubjets:
  print "Warning: both pruned and soft drop subjets selected. Only pruned subjets will be processed."
  print "!!!Select either pruned subjets with 'usePrunedSubjets' or soft drop subjets with 'useSoftDropSubjets'."
elif not options.usePrunedSubjets and not options.useSoftDropSubjets:
  print "!!!Warning: no subjets will be processed.!!!"
  print "!!!Select either pruned subjets with 'usePrunedSubjets' or soft drop subjets with 'useSoftDropSubjets'."

#print options

process = cms.Process("BTagVal")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cout = cms.untracked.PSet(
    threshold = cms.untracked.string('INFO'),
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) ) # Keep as such

process.source = cms.Source("EmptySource")

## Output file
#ext=""
#if options.usePrunedSubjets: 
#  ext="_WithPrunedSubjets"
#elif options.useSoftDropSubjets: 
#  ext="_withSoftDropSubjets"
#outFilename = options.outFilename+ext+".root"

process.TFileService = cms.Service("TFileService",
    fileName = cms.string(options.outFilename)
)

from inputFiles_cfi import *
process.btagval = cms.EDAnalyzer('BTagValidation',
    MaxEvents              = cms.int32(options.maxEvents),
    ReportEvery            = cms.int32(options.reportEvery),
    UseJetProbaTree        = cms.bool(options.useJetProbaTree),
    InputTTreeEvtInfo      = cms.string('btaganaFatJets/ttree'),
    InputTTree             = cms.string('btaganaFatJets/ttree'),
    InputFiles             = cms.vstring(FileNames),
    #InputFiles             = cms.vstring(FileNames_QCD_Pt_800to1000),
    UseFlavorCategories    = cms.bool(options.useFlavorCategories),
    UseRelaxedMuonID       = cms.bool(options.useRelaxedMuonID),
    ApplyFatJetMuonTagging = cms.bool(options.applyFatJetMuonTagging),
    ApplyFatJetBTagging    = cms.bool(options.applyFatJetBTagging),
    FatJetDoubleTagging    = cms.bool(options.fatJetDoubleTagging),
    FatJetDoubleBTagging   = cms.bool(options.fatJetDoubleBTagging),
    UsePrunedSubjets       = cms.bool(options.usePrunedSubjets),
    UseSoftDropSubjets     = cms.bool(options.useSoftDropSubjets),
    ApplySubJetMuonTagging = cms.bool(options.applySubJetMuonTagging),
    ApplySubJetBTagging    = cms.bool(options.applySubJetBTagging),
    DynamicMuonSubJetDR    = cms.bool(options.dynamicMuonSubJetDR),
    ApplySFs               = cms.bool(options.applySFs),
    FatJetBDiscrCut        = cms.double(options.fatJetBDiscrCut),
    SubJetBDiscrCut        = cms.double(options.subJetBDiscrCut),
    FatJetPtMin            = cms.double(options.fatJetPtMin),
    FatJetPtMax            = cms.double(options.fatJetPtMax),
    FatJetSoftDropMassMin    = cms.double(options.fatJetSoftDropMassMin),
    File_PVWt              = cms.string('hnpv_data_Run2015D_mc_RunIISpring15DR74-Asympt25ns_pvwt.root'),
    File_PUDistMC          = cms.string('PUDistMC_2015_25ns_Startup_PoissonOOTPU.root'),
    File_PUDistData        = cms.string('PUDistData_Run2015ABCD.root'),
    Hist_PVWt              = cms.string('hpvwt_data_mc'),
    Hist_PUDistMC          = cms.string('pileup'),
    Hist_PUDistData        = cms.string('pileup'),
    FatJetSoftDropMassMax    = cms.double(options.fatJetSoftDropMassMax),
    FatJetTau21Min         = cms.double(options.fatJetTau21Min), #added by rizki
    FatJetTau21Max         = cms.double(options.fatJetTau21Max), #added by rizki
    FatJetAbsEtaMax        = cms.double(2.4),
    SFbShift               = cms.double(options.SFbShift),
    SFlShift               = cms.double(options.SFlShift),
    DoPUReweightingOfficial= cms.bool(options.doPUReweightingOfficial),
    DoPUReweightingNPV     = cms.bool(options.doPUReweightingNPV),
    TriggerSelection       = cms.vstring( # OR of all listed triggers applied, empty list --> no trigger selection applied
        options.triggerSelection
    ),
    TriggerPathNames       = cms.vstring(
        "HLT_PFJet200_v",
        "HLT_PFJet260_v",
        "HLT_PFJet320_v",
        "HLT_PFJet400_v",
        "HLT_PFJet450_v",
        "HLT_PFJet500_v",
    )
)

#process.btagvalsubjetmu = process.btagval.clone(
    #ApplySubJetMuonTagging = cms.bool(not options.applySubJetMuonTagging),
#)

#process.btagvalsubjetbtag = process.btagval.clone(
    #ApplySubJetMuonTagging = cms.bool(not options.applySubJetMuonTagging),
    #ApplySubJetBTagging = cms.bool(True),
#)

process.p = cms.Path(process.btagval)
#process.p = cms.Path(process.btagval + process.btagvalsubjetmu + process.btagvalsubjetbtag)
