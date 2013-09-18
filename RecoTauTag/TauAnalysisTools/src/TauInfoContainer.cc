
#include "RecoTauTag/TauAnalysisTools/interface/TauInfoContainer.h"
#include <TLorentzVector.h>
#include "Math/GenVector/LorentzVector.h"

using namespace edm;

TauInfoContainer::TauInfoContainer(const pat::Tau* recoTauCand, std::vector<const reco::Candidate*>* trigObj, const reco::Candidate* GenParticle ,unsigned int index, unsigned int nTotalObjects, const GenEventInfoProduct* GenInfo, unsigned int NVTX, const edm::Event* evt, const reco::Candidate* pfJet, const reco::Vertex* Vertex ):
  recoTauCand_(recoTauCand), trigObj_(trigObj),GenParticle_(GenParticle),index_(index), nTotalObjects_(nTotalObjects), genInfo_(GenInfo),Nvtx_(NVTX),Evt_(evt), pfJet_(pfJet), Vertex_(Vertex){
  
        // Create a dummy reco::Candidate Object with unrealistic LorentzVector values as a default output to return in case of a failed matching.  
        dummyCandidate_ = dynamic_cast<reco::Candidate* >( recoTauCand->clone());
        math::XYZTLorentzVector *v = new math::XYZTLorentzVector();
        v->SetPxPyPzE(-999.,-999.,-9999.,-999.);
        dummyCandidate_->setP4(((const math::XYZTLorentzVector)*v)); 
  }

TauInfoContainer::TauInfoContainer(){}

const reco::Vertex* TauInfoContainer::getPV() const{
   return Vertex_;
}


unsigned int TauInfoContainer::index() const {
   return index_;
}

unsigned int TauInfoContainer::nTotalObjects() const {
   return nTotalObjects_;
}

const pat::Tau* TauInfoContainer::recoTauCand() const {
   return recoTauCand_;
}

const reco::Candidate* TauInfoContainer::PfJet() const {
   if(pfJet_ != NULL) return pfJet_;
   else return dummyCandidate_; // Careful! Method return dummy object to ensure successfull termination of program. Only use GenParticle values if "bool TauInfoContainer::isPfJetMatched()" returns "true"

}

bool TauInfoContainer::isPfJetMatched() const{
   return pfJet_ != NULL;
}

const GenEventInfoProduct* TauInfoContainer::genInfo() const {
   return genInfo_;
}

double TauInfoContainer::RunNr() const {
   return Evt_->id().run();
}

double TauInfoContainer::EvtNr() const {
   return Evt_->id().event();
}

double TauInfoContainer::LumiSec() const {
   return Evt_->id().luminosityBlock();
}

const reco::Candidate* TauInfoContainer::GenParticle() const {
   if(GenParticle_ != NULL) return GenParticle_;
   else return dummyCandidate_; // Careful! Method return dummy object to ensure successfull termination of program. Only use GenParticle values if "bool TauInfoContainer::isGenParticelMatched()" returns "true"
}

const reco::Candidate* TauInfoContainer::GenTauJet() const {
   if(recoTauCand_->genJet() != NULL) return recoTauCand_->genJet();
   else return dummyCandidate_; // Careful!  Method return dummy object to ensure successfull termination of program. Only use GenTauJet values if "bool TauInfoContainer::isTauGenJetMatched()" returns "true"

}

bool TauInfoContainer::isGenParticelMatched() const {
   return GenParticle_ != NULL;
}

bool TauInfoContainer::isTauGenJetMatched() const {
   return recoTauCand_->genJet() != NULL;
}

bool TauInfoContainer::isTrigObjMatched(int a) const {
   return trigObj_->at(a) != NULL;
}

double TauInfoContainer::recoTauCandID(std::string DiscriminatorName) const{

   return recoTauCand_->tauID(DiscriminatorName);  

} 

int TauInfoContainer::genDecayMode() const{

   std::string genDecayMode = "NoGenJet";

   if(recoTauCand_->genJet() != 0) genDecayMode = JetMCTagUtils::genTauDecayMode(*(recoTauCand_->genJet()));

   int prong = -999;
   int pi0 = -999;
   int posPi0 = -1;
   int result = -999;

   posPi0 = genDecayMode.find("Prong") + 5;


   if(genDecayMode.at(0)== 'o') prong = 0;
   else if(genDecayMode.at(0)=='t') prong = 10;

   if(posPi0!=4 && genDecayMode.at(posPi0)!='O' ) pi0 = genDecayMode.at(posPi0) - '0';

   result = prong + pi0;
   
   if(genDecayMode.at(0)== 'o' && genDecayMode.at(posPi0)=='O') result = -1; 
   if(genDecayMode.at(0)== 't' && genDecayMode.at(posPi0)=='O') result = -10;
   if(genDecayMode.at(0)== 'r') result = -100;
   if(genDecayMode.at(0)== 'N') result = -999;

//   if(result!=-999)std::cout << result <<" : " << genDecayMode << std::endl;

   return result;  

}    

int TauInfoContainer::Nvtx() const{
   return Nvtx_;
}
