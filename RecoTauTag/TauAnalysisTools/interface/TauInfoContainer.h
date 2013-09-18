#ifndef TauInfoContainer_h
#define TauInfoContainer_h

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Provenance/interface/EventID.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "PhysicsTools/JetMCUtils/interface/JetMCTag.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

class TauInfoContainer {
  public:
    // Default needed for persistency
    TauInfoContainer(); 

    TauInfoContainer( const pat::Tau* recoTauCand, std::vector<const reco::Candidate*>* trigObj, const reco::Candidate* GenParticle, unsigned int index, unsigned int nTotalObjects, const GenEventInfoProduct* GenInfo, unsigned int NVTX, const edm::Event* evt, const reco::Candidate* pfJet, const reco::Vertex* Vertex );
    
    // Get tag tau object
    const pat::Tau* recoTauCand() const;

    const reco::Candidate* PfJet() const;

    const reco::Candidate* GenParticle() const; 

    bool isGenParticelMatched() const; 

    const reco::Vertex* getPV() const;

    // return true if pat::tau is matched to a hadronically decaying Gen Tau
    bool isTauGenJetMatched() const;

    bool isPfJetMatched() const;

    // Get match status of trigger filter object
    bool isTrigObjMatched(int a) const;

    // Get status of Discriminator 
    double recoTauCandID(std::string DiscriminatorName) const;

    // Get the index of this match in the event.
    unsigned int index() const;
    // Get the total number of reco objects in this event.
    unsigned int nTotalObjects() const;

    const reco::Candidate* GenTauJet() const; 

    const GenEventInfoProduct* genInfo() const;

    int genDecayMode() const;

    int Nvtx() const;

    // const edm::Event* Evt() const;

    double RunNr() const;

    double EvtNr() const;

    double LumiSec() const;

  private:

    const pat::Tau* recoTauCand_;
    std::vector<const reco::Candidate*>* trigObj_;
    const reco::Candidate* GenParticle_;
    reco::Candidate* dummyCandidate_;
    unsigned int index_;
    unsigned int nTotalObjects_;
    const GenEventInfoProduct* genInfo_; 
    unsigned int Nvtx_;
    const edm::Event* Evt_;
    const reco::Candidate* pfJet_;
    const reco::Vertex* Vertex_;
};

#endif /* end of include guard: TauInfoContainer _h */
