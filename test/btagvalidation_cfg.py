import FWCore.ParameterSet.Config as cms

from FWCore.ParameterSet.VarParsing import VarParsing
import copy

options = VarParsing('python')

options.register('infile',
    'root://eoscms//eos/cms/store/group/phys_btag/performance/CMSSW_5_3_9/MC/QCD_Pt-1800_TuneZ2star_8TeV_pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/JetTree_mc_subjets_10_1_mDl.root', 
    VarParsing.multiplicity.singleton,
    VarParsing.varType.string,
    "Input file name")

options.register('maxEventsToProcess',
    100,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.int,
    "Maximum events to process") 

options.register('isData',
    False,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "Is Data") 

options.register('runSubJets',
    True,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "Run SubJets") 

options.register('outfilename',
    'commonPlots.root',
    VarParsing.multiplicity.singleton,
    VarParsing.varType.string,
    "Output file name")

options.parseArguments()

print options 

process = cms.Process("BTagVal")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger = cms.Service("MessageLogger",
   destinations = cms.untracked.vstring( 
     'detailedInfo',
     ),
   detailedInfo = cms.untracked.PSet(
     threshold = cms.untracked.string('INFO'), 
     ), 
   )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) ) # Keep as such 

process.source = cms.Source("EmptySource")

process.btagval = cms.EDAnalyzer('BTagValidation',
    inputfile = cms.untracked.string(options.infile),
    outfilename = cms.untracked.string(options.outfilename),
    maxEvents = cms.untracked.int32(options.maxEventsToProcess), 
    isData = cms.untracked.bool(options.isData),
    runSubJets = cms.untracked.bool(options.runSubJets), 
    )

process.p = cms.Path(process.btagval)
