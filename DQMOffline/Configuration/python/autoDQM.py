autoDQM = { 'common' : ['DQMOfflineCommon',
                        'DQMHarvestCommon+DQMCertCommon'],
            'commonSiStripZeroBias' : ['DQMOfflineCommonSiStripZeroBias',
                                       'DQMHarvestCommonSiStripZeroBias+DQMCertCommon'],
            'muon': ['DQMOfflineMuon',
                     'DQMHarvestMuon+DQMCertMuon'],
            'hcal':     ['DQMOfflineHcal',
                         'DQMHarvestHcal+DQMCertHcal'],
            'jetmet':  ['DQMOfflineJetMET',
                        'DQMHarvestJetMET+DQMCertJetMET'],
            'ecal':       ['DQMOfflineEcal',
                           'DQMHarvestEcal+DQMCertEcal'],
            'egamma':       ['DQMOfflineEGamma',
                           'DQMHarvestEGamma'],
            'express':       ['@commonSiStripZeroBias+@muon+@hcal+@jetmet+@ecal',
                              '@commonSiStripZeroBias+@muon+@hcal+@jetmet+@ecal'],
            'allForPrompt':  ['@common+@muon+@hcal+@jetmet+@ecal',
                              '@common+@muon+@hcal+@jetmet+@ecal'],
            'MinimumBias': ['@commonSiStripZeroBias+bTagPlots+@muon+@hcal+HcalDQMOfflineSequence+@jetmet+@ecal+egammaDQMOffline',
                            '@commonSiStripZeroBias+bTagCollectorSequence+@muon+@hcal+HcalDQMOfflinePostProcessor+@jetmet+@ecal+egammaPostProcessing']
            }

