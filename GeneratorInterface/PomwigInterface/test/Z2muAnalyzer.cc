//
// Original Author:  Fabian Stoeckli
//         Created:  Tue Nov 14 13:43:02 CET 2006
// $Id: H4muAnalyzer.cc,v 1.2 2007/02/14 15:51:35 fabstoec Exp $
//
// Modified for PomwigInterface test for Z/gamma* -> 2mu
// 02/2007
//


// system include files
#include <memory>
#include <iostream>

// user include files
#include "Z2muAnalyzer.h"


#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "CLHEP/HepMC/GenEvent.h"
#include "CLHEP/HepMC/GenParticle.h"

#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"

#include "TH1D.h"
#include "TFile.h"

Z2muAnalyzer::Z2muAnalyzer(const edm::ParameterSet& iConfig)
{
  outputFilename=iConfig.getUntrackedParameter<std::string>("OutputFilename","dummy.root");
  invmass_histo = new TH1D("invmass_histo","invmass_histo",100,0.,100.);
}


Z2muAnalyzer::~Z2muAnalyzer()
{
 
}

// ------------ method called to for each event  ------------
void
Z2muAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
  
   // get HepMC::GenEvent ...
   Handle<HepMCProduct> evt_h;
   iEvent.getByType(evt_h);
   HepMC::GenEvent * evt = new  HepMC::GenEvent(*(evt_h->GetEvent()));


   // look for stable muons
   std::vector<HepMC::GenParticle*> muons;   
   for(HepMC::GenEvent::particle_iterator it = evt->particles_begin(); it != evt->particles_end(); ++it) {
     if(abs((*it)->pdg_id())==13 && (*it)->status()==1)
       muons.push_back(*it);
   }
   
   // if there are at least two muons
   // calculate invarant mass of first two and fill it into histogram
   HepLorentzVector tot_momentum;
   double inv_mass = 0.0;
   std::cout<<muons.size()<<std::endl;
   if(muons.size()>=2) {
     tot_momentum = muons[0]->momentum();
     tot_momentum += muons[1]->momentum();
     inv_mass = sqrt(tot_momentum.m2());
   }
   
   invmass_histo->Fill(inv_mass);
   std::cout<<inv_mass<<std::endl;

}


// ------------ method called once each job just before starting event loop  ------------
void 
Z2muAnalyzer::beginJob(const edm::EventSetup&)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
Z2muAnalyzer::endJob() {
  // save histograms into file
  TFile file(outputFilename.c_str(),"RECREATE");
  invmass_histo->Write();
  file.Close();

}

//define this as a plug-in
DEFINE_FWK_MODULE(Z2muAnalyzer);
