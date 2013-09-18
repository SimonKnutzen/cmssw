#import FWCore.ParameterSet.Config as cms
from PhysicsTools.PatAlgos.tools.coreTools import *

process = cms.Process("tauNTuple")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.categories.extend(['TauValidationNTupleProd'])
process.MessageLogger.cerr.default.limit = -1
process.MessageLogger.cerr.FwkReport.reportEvery = 10
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

#-- Calibration tag -----------------------------------------------------------
process.load("Configuration.Geometry.GeometryIdeal_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = cms.string('START53_V7F::All')
process.load("Configuration.StandardSequences.MagneticField_cff")

#-- PAT standard config -------------------------------------------------------
process.load("PhysicsTools.PatAlgos.patSequences_cff")
process.load("RecoVertex.Configuration.RecoVertex_cff")


process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )

## Dummy output for PAT. Not used in the analysis ##
process.out = cms.OutputModule(
    "PoolOutputModule",
    fileName       = cms.untracked.string('dummy.root'),
    SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
    dropMetaData   = cms.untracked.string('DROPPED'),
    outputCommands = cms.untracked.vstring('keep *')
    )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
#        'file:///disk1/knutzen/CMSSW/CMSSW_5_3_3_patch3/src/aachen3a/TEST/TEST/MyOutputFile.root'
         '/store/mc/Summer12/TTJets_TuneZ2star_8TeV-madgraph-tauola/AODSIM/PU_S8_START52_V9-v1/0000/B27ECBD4-06C2-E111-BEA5-001A9281171E.root' 

    ),

    dropDescendantsOfDroppedBranches = cms.untracked.bool(False),
    inputCommands = cms.untracked.vstring(
        'keep *',
        'drop recoPFTaus_*_*_*'                      
    )
)


from PhysicsTools.SelectorUtils.pvSelector_cfi import pvSelector
process.goodOfflinePrimaryVertices = cms.EDFilter(
    "PrimaryVertexObjectFilter",
    filterParams = pvSelector.clone( minNdof = cms.double(4.0), maxZ = cms.double(24.0) ),
    src=cms.InputTag('offlinePrimaryVertices')
    )

process.load("PhysicsTools/PatAlgos/patSequences_cff")

from PhysicsTools.PatAlgos.tools.tauTools import *
switchToPFTauHPS(process) #create HPS Taus from the pat default sequence


# switch on PAT trigger
from PhysicsTools.PatAlgos.tools.trigTools import switchOnTrigger
switchOnTrigger(process) #create pat trigger objects

process.load("RecoTauTag.Configuration.RecoPFTauTag_cff")



process.TFileService = cms.Service("TFileService",
                                               fileName = cms.string('TauValidationNTuple.root') #output file
                                                                                  )
###############################################
#############    User input  ##################
###############################################

# Enter a list with all trigger filter names you want to investigate.
# A bool with the same name will be created for each filter which denotes if a filter object is matched to the tag tau
filterName = [
               "hltPFTau35",
               "hltPFTau35Track",
               "hltPFTau35TrackPt20",
               ]

#Enter a list of HPS discriminators you want to store in the output tree for the tag tau
IDName = [  
          "decayModeFindingNewDMs", 
          "decayModeFindingOldDMs", 
          "decayModeFinding", 
          "byLooseIsolation", 
          "byVLooseCombinedIsolationDeltaBetaCorr", 
          "byLooseCombinedIsolationDeltaBetaCorr", 
          "byMediumCombinedIsolationDeltaBetaCorr", 
          "byTightCombinedIsolationDeltaBetaCorr", 
          "byCombinedIsolationDeltaBetaCorrRaw", 
          "byLooseCombinedIsolationDeltaBetaCorr3Hits", 
          "byMediumCombinedIsolationDeltaBetaCorr3Hits", 
          "byTightCombinedIsolationDeltaBetaCorr3Hits", 
          "byCombinedIsolationDeltaBetaCorrRaw3Hits", 
          "chargedIsoPtSum", 
          "neutralIsoPtSum", 
          "puCorrPtSum", 
          "byIsolationMVA3oldDMwoLTraw", 
          "byVLooseIsolationMVA3oldDMwoLT", 
          "byLooseIsolationMVA3oldDMwoLT", 
          "byMediumIsolationMVA3oldDMwoLT", 
          "byTightIsolationMVA3oldDMwoLT", 
          "byVTightIsolationMVA3oldDMwoLT", 
          "byVVTightIsolationMVA3oldDMwoLT", 
          "byIsolationMVA3oldDMwLTraw", 
          "byVLooseIsolationMVA3oldDMwLT", 
          "byLooseIsolationMVA3oldDMwLT", 
          "byMediumIsolationMVA3oldDMwLT", 
          "byTightIsolationMVA3oldDMwLT", 
          "byVTightIsolationMVA3oldDMwLT", 
          "byVVTightIsolationMVA3oldDMwLT", 
          "byIsolationMVA3newDMwoLTraw", 
          "byVLooseIsolationMVA3newDMwoLT", 
          "byLooseIsolationMVA3newDMwoLT", 
          "byMediumIsolationMVA3newDMwoLT", 
          "byTightIsolationMVA3newDMwoLT", 
          "byVTightIsolationMVA3newDMwoLT", 
          "byVVTightIsolationMVA3newDMwoLT", 
          "byIsolationMVA3newDMwLTraw", 
          "byVLooseIsolationMVA3newDMwLT", 
          "byLooseIsolationMVA3newDMwLT", 
          "byMediumIsolationMVA3newDMwLT", 
          "byTightIsolationMVA3newDMwLT", 
          "byVTightIsolationMVA3newDMwLT", 
          "byVVTightIsolationMVA3newDMwLT", 
          "againstElectronLoose", 
          "againstElectronMedium", 
          "againstElectronTight", 
          "againstElectronMVA5raw", 
          "againstElectronMVA5category", 
          "againstElectronVLooseMVA5", 
          "againstElectronLooseMVA5", 
          "againstElectronMediumMVA5", 
          "againstElectronTightMVA5", 
          "againstElectronVTightMVA5", 
          "againstElectronDeadECAL", 
          "againstMuonLoose", 
          "againstMuonMedium", 
          "againstMuonTight", 
          "againstMuonLoose2", 
          "againstMuonMedium2", 
          "againstMuonTight2", 
          "againstMuonLoose3", 
          "againstMuonTight3", 
          "againstMuonMVAraw", 
          "againstMuonLooseMVA", 
          "againstMuonMediumMVA", 
          "againstMuonTightMVA"
               ]

common_ntuple_branches = cms.PSet(
    index = cms.string("index"), # Index of reco object in the event
    nRecoObjects = cms.string("nTotalObjects"), # Number of reco objects in the event

    nvtx = cms.string("Nvtx"),

    PV_z = cms.string("getPV.z"),

    genWeight = cms.string("genInfo.weight"),

    RunNr = cms.string("RunNr"),
    EvtNr = cms.string("EvtNr"),
    LumiSec = cms.string("LumiSec"),

    recoTauCand_Pt = cms.string("recoTauCand.pt"),
    recoTauCand_Eta = cms.string("recoTauCand.eta"),
    recoTauCand_Phi = cms.string("recoTauCand.phi"),
    recoTauCand_DecayMode = cms.string("recoTauCand.decayMode"),
    recoTauCand_Vz = cms.string("recoTauCand.vz"),

    isTauGenJetMatched = cms.string("isTauGenJetMatched"),
    isGenParticleMatched = cms.string("isGenParticelMatched"),

    tauSeedPfJet_Pt = cms.string("recoTauCand.pfJetRef().pt"),
    tauSeedPfJet_Eta = cms.string("recoTauCand.pfJetRef().eta"),
    tauSeedPfJet_Phi = cms.string("recoTauCand.pfJetRef().phi"),
    tauSeedPfJet_Vz = cms.string("recoTauCand.pfJetRef().vz"),

    pfJet_Pt = cms.string("PfJet.pt"),
    pfJet_Eta = cms.string("PfJet.eta"),
    pfJet_Phi = cms.string("PfJet.phi"),
    pfJetVz = cms.string("PfJet.vz"),
    isPfJetMatched = cms.string("isPfJetMatched"),

    # Careful! Only use GenTauJet (returns the values of the generated tau Jet) values if "bool TauTrigMatch::GenHadTauMatch()" returns "true". Otherwise it contains (unrealsitic) default values
    GenTauJet_Pt = cms.string("GenTauJet.pt"),
    GenTauJet_Eta = cms.string("GenTauJet.eta"),
    GenTauJet_Phi = cms.string("GenTauJet.phi"),
    GenTauJet_DecayMode = cms.string("genDecayMode"), # Decay Modes encoded as: 5 * (Prong - 1) + numberOfPi0 (example: oneProng2Pi0 = 2, threeProng1Pi0 = 11) cf. recoDecayModes. Additional decays: oneProngOther = -1, threeProngOther = -10, rare = -100 and NoGenJet = -999 

    # Careful! Only use GenTauMatch (returns the values of the generator particle matched to the tagTau) values if "bool TauTrigMatch::GenTauMatchTest()" returns "true". Otherwise it contains (unrealsitic) default values
    GenParticle_Pt = cms.string("GenParticle.pt"),
    GenParticle_Eta = cms.string("GenParticle.eta"),
    GenParticle_Phi = cms.string("GenParticle.phi"),
    GenParticel_pdgId = cms.string("GenParticle.pdgId"),
)


process.tauNTuple = cms.EDAnalyzer('TauValidationNTupleProd',
      tauTag         = cms.InputTag("patTaus"),
      trigTag        = cms.InputTag("patTriggerEvent"),
      ntuple         = common_ntuple_branches,
      maxDR          = cms.double(0.5), #The DeltaR parameter used for the trigger matching
      filterNames    = cms.vstring(),
)

###############################################

for j in range(len(filterName)):
    setattr(common_ntuple_branches, filterName[j], cms.string( "isTrigObjMatched(%i)"%j) )

for j in range(len(IDName)):
    setattr(common_ntuple_branches, IDName[j], cms.string( "recoTauCandID(\"%s\")"%IDName[j]) )

process.tauNTuple.filterNames = cms.vstring( filterName )


process.p = cms.Path(
        process.goodOfflinePrimaryVertices*
        process.recoTauClassicHPSSequence*
        process.patDefaultSequence*
        process.tauNTuple
        )
