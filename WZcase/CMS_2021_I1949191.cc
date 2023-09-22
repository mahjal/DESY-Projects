// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/FastJets.hh"
#include "Rivet/Projections/DressedLeptons.hh"
#include "Rivet/Projections/MissingMomentum.hh"
#include "Rivet/Projections/DirectFinalState.hh"
#include "Rivet/Projections/IdentifiedFinalState.hh"
#include "Rivet/Projections/InvisibleFinalState.hh"
#include "Rivet/Projections/VetoedFinalState.hh"
#include <iostream>


namespace Rivet {


  /// @brief Add a short analysis description here
  class CMS_2021_I1949191 : public Analysis {
  public:

    /// Constructor
    RIVET_DEFAULT_ANALYSIS_CTOR(CMS_2021_I1949191);


    /// @name Analysis methods
    /// @{

    /// Book histograms and initialise projections before the run
    void init() {

      // Initialise and register projections

      // The basic final-state projection:
      // all final-state particles within
      // the given eta acceptance
      const FinalState fs(Cuts::abseta < 5.0);
      
      VisibleFinalState visfs(fs);
      PromptFinalState pfs(fs);

      // Leptons
      PromptFinalState bareMuons(Cuts::abspid == PID::MUON);
      DressedLeptons dressedMuons(pfs, bareMuons, 0.1, Cuts::abseta < 2.5 && Cuts::pT > 10*GeV, true);
      declare(dressedMuons, "muons");

      PromptFinalState bareElectrons(Cuts::abspid == PID::ELECTRON);
      DressedLeptons dressedElectrons(pfs, bareElectrons, 0.1, Cuts::abseta < 2.5 && Cuts::pT > 10*GeV, true);
      declare(dressedElectrons, "electrons");
      
      // Neutrinos
      InvisibleFinalState neutrinos(true, true, true);
      declare(neutrinos, "Neutrinos");
      
      // Jets
      VetoedFinalState fs_jets(fs);
      fs_jets.addVetoOnThisFinalState(dressedMuons);
      fs_jets.addVetoOnThisFinalState(dressedElectrons);
      fs_jets.vetoNeutrinos();
      declare(FastJets(fs_jets, FastJets::ANTIKT, 0.4), "jets");
      
      // Missing momentum
      declare(MissingMomentum(fs), "MET");

      // Book histograms
      //Figure 14                                                                                                                                                                                       
      book(_h["Z_pt"], 28, 1, 1);
      book(_h["LeadingJet_pt"], 31, 1, 1);
      book(_h["nJets"], 34, 1, 1);
	  book(_h["WZ_mass"], 37, 1, 1);

	  //Figure 16
	  book(_h["Wlepton_inclusive_pt"], 40, 1, 1);
	  book(_h["Wlepton_positive_pt"], 43, 1, 1);
	  book(_h["Wlepton_negative_pt"], 45, 1, 1);

	  book(_h["C_Wlepton_theta_Q_inclusive"], 47, 1, 1);
	  book(_h["C_Wlepton_theta_Q_positive"], 50, 1, 1);
	  book(_h["C_Wlepton_theta_Q_negative"], 52, 1, 1);

	  book(_h["C_Zlepton_theta_inclusive"], 54, 1, 1);
	  book(_h["C_Zlepton_theta_positive"], 57, 1, 1);
	  book(_h["C_Zlepton_theta_negative"], 59, 1, 1);

      
    }
    
    /// @param leptons pt-ordered list of electrons or muons from which to build the Z boson
    std::pair<int,int> Zleptons_finder(const Particles& leptons) {
		
		std::pair<int,int> zll_ids = std::make_pair(-1,-1);
		
		double best_mass_diff = 100000.;
		
		for (long unsigned int iLepton = 0; iLepton < leptons.size(); ++iLepton) {
		
			for (long unsigned int jLepton = iLepton+1; jLepton < leptons.size(); ++jLepton) {
		
				if ( leptons[iLepton].charge()*leptons[jLepton].charge()<0 && leptons[iLepton].pT() > 25.*GeV && leptons[jLepton].pT() > 10.*GeV) {
		
				        Particle cand(PID::ZBOSON, leptons[iLepton].mom() + leptons[jLepton].mom());
					
					if (cand.mass() > 60.*GeV && cand.mass() < 120.*GeV && fabs(cand.mass() - 91.18*GeV) < best_mass_diff) {
						
						zll_ids = std::make_pair(iLepton,jLepton);
						best_mass_diff = fabs(cand.mass() - 91.18*GeV);
						
					}
		
				}
		
			}
		}
		
	    return zll_ids;
    }
    
    Particle Wneutrino_finder(const Particles& neutrinos, const Particle& W_lepton) {
		
		Particle W_neutrino = neutrinos[0];
		
		double best_mass_diff = 100000.;
		
		for (long unsigned int iNeutrino = 0; iNeutrino < neutrinos.size(); ++iNeutrino) {
		
			Particle cand(PID::WPLUS, W_lepton.mom() + neutrinos[iNeutrino].mom());
					
			if (cand.mass() > 50.*GeV && cand.mass() < 110.*GeV && fabs(cand.mass() - 80.34*GeV) < best_mass_diff) {
				
				W_neutrino = neutrinos[iNeutrino];
				best_mass_diff = fabs(cand.mass() - 80.34*GeV);
				
			}
		
		}
		
	    return W_neutrino;
    }


    /// Perform the per-event analysis
    void analyze(const Event& event) {

      // Get leptons
      const Particles& muons = apply<DressedLeptons>(event, "muons").particlesByPt();
      const Particles& electrons = apply<DressedLeptons>(event, "electrons").particlesByPt();
      
      // Get neutrinos
      const Particles neutrinos = apply<InvisibleFinalState>(event, "Neutrinos").particlesByPt();
      
      // Only three leptons 
      if ( (muons.size()+electrons.size()) != 3 ) vetoEvent;
      
      // Z adn W leptons
      Particle Z_leading_lepton;
      Particle Z_trailing_lepton;
      Particle W_lepton;
      if (electrons.size() >= 2) { // Look for Z->ee

		std::pair<int,int> Z_leptons_index = Zleptons_finder(electrons);
		
		if ( Z_leptons_index.first < 0 && Z_leptons_index.second < 0 ) vetoEvent;
		else {
			
			Z_leading_lepton = electrons[Z_leptons_index.first];
			Z_trailing_lepton = electrons[Z_leptons_index.second];
			
			if (electrons.size() == 2) W_lepton = muons[0];
			else W_lepton = electrons[3-Z_leptons_index.first-Z_leptons_index.second];
			
			}
	  }

      else { // look for Z->mumu
        
		std::pair<int,int> Z_leptons_index = Zleptons_finder(muons);
		
		if ( Z_leptons_index.first < 0 && Z_leptons_index.second < 0 ) vetoEvent;
		else {
			
			Z_leading_lepton = muons[Z_leptons_index.first];
			Z_trailing_lepton = muons[Z_leptons_index.second];
			
			if (muons.size() == 2) W_lepton = electrons[0];
			else W_lepton = muons[3-Z_leptons_index.first-Z_leptons_index.second];
			
		    }
	  }

	  // Making the Z
	  Particle Z(PID::ZBOSON, Z_leading_lepton.mom() + Z_trailing_lepton.mom());
	  
	  // Making the W
	  if ( neutrinos.size() < 1 || W_lepton.pT() < 25*GeV) vetoEvent;
	  Particle W_neutrino = Wneutrino_finder(neutrinos, W_lepton);
          W_neutrino.setMomentum(W_neutrino.pT(),W_neutrino.px(),W_neutrino.py(),0.);
	  Particle W(PID::WPLUS, W_lepton.mom() + W_neutrino.mom());
	  //	  if (W_lepton.charge() < 0) Particle W(PID::WMINUS, W_lepton.mom() + W_neutrino.mom());
	  if (W_lepton.charge() < 0) {
	    W = Particle(PID::WMINUS, W_lepton.mom() + W_neutrino.mom());
	  } else {
	    W = Particle(PID::WPLUS, W_lepton.mom() + W_neutrino.mom());
	  }
	 
	  // Minimum invariant mass
	  if ( (Z_leading_lepton.momentum()+W_lepton.momentum()).mass() < 4 || (Z_trailing_lepton.momentum()+W_lepton.momentum()).mass() < 4) vetoEvent;
	  
	  // Minimum invariant of the tri-lepton system
	  if ( (Z.momentum()+W_lepton.momentum()).mass() < 100 ) vetoEvent;
	  
      // Cluster jets
      const FastJets& fj = apply<FastJets>(event, "jets");
      const Jets jets = fj.jetsByPt(Cuts::abseta < 2.5 && Cuts::pT > 25*GeV);

      // Remove jets overlapping with any of the selected leptons
      discardIfAnyDeltaRLess(jets, electrons, 0.4);
      discardIfAnyDeltaRLess(jets, muons, 0.4);

      // Select jets ghost-associated to B-hadrons with a certain fiducial selection
      const Jets bJets = filter_select(jets, [](const Jet& j) { return j.bTagged(); } );
      
      // Select events without any b-jet
      if ( bJets.size() > 0 ) vetoEvent;

      //calculation of cosine of polarization angle of W
      FourMomentum W_lepton_4p(W_lepton.mom());
      FourMomentum W_4p(W.mom());
      const LorentzTransform LTW = LorentzTransform::mkFrameTransformFromBeta(W_4p.betaVec());
      Matrix3 zrot(W_4p.p3(), Vector3(0.0, 0.0, 1.0));
      W_lepton_4p = LTW.transform(W_lepton_4p);
      Vector3 W_lepton_p3 = W_lepton_4p.p3();
      W_lepton_p3 = zrot*W_lepton_p3;
      double thetas = W_lepton_p3.theta();
      double cos_theta_W(cos(thetas));
      int charge = W_lepton.charge();


      //calculation of cosine of polarization angle of Z                                                                                                                                                  
      Particle negatively_charged_lepton;
      if (Z_leading_lepton.charge() < 0) {
	negatively_charged_lepton = Z_leading_lepton;
      } else {
	negatively_charged_lepton = Z_trailing_lepton;
      }

      FourMomentum negatively_charged_lepton_4p(negatively_charged_lepton.momentum());
      FourMomentum Z_4p(Z.momentum());
      const LorentzTransform LTZ = LorentzTransform::mkFrameTransformFromBeta(Z_4p.betaVec());
      Matrix3 zrot2(Z_4p.p3(), Vector3(0.0, 0.0, 1.0));
      negatively_charged_lepton_4p = LTZ.transform(negatively_charged_lepton_4p);
      Vector3 negatively_charged_lepton_p3 = negatively_charged_lepton_4p.p3();
      negatively_charged_lepton_p3 = zrot2*negatively_charged_lepton_p3;
      double thetas_Z = negatively_charged_lepton_p3.theta();
      double cos_theta_Z(cos(thetas_Z));

      // Fill histograms
	  _h["Z_pt"]->fill(Z.pt()/GeV);
	  if ( jets.size() > 0 ) _h["LeadingJet_pt"]->fill(jets[0].pT()/GeV);
	  _h["nJets"]->fill(jets.size());
	  _h["WZ_mass"]->fill((W.momentum()+Z.momentum()).mass()/GeV);

	   if (charge != 0) {
	    
	    _h["Wlepton_inclusive_pt"]->fill(W_lepton.pt() / GeV);
	    _h["C_Wlepton_theta_Q_inclusive"]->fill(charge*cos_theta_W / GeV);
	    _h["C_Zlepton_theta_inclusive"]->fill( cos_theta_Z / GeV);
	   
	    if (charge > 0) {
	      _h["Wlepton_positive_pt"]->fill(W_lepton.pt() / GeV);
	      _h["C_Wlepton_theta_Q_positive"]->fill( charge*cos_theta_W / GeV);
	      _h["C_Zlepton_theta_positive"]->fill( cos_theta_Z / GeV);

	    } if (charge < 0){
	      _h["Wlepton_negative_pt"]->fill(W_lepton.pt() / GeV);
	      _h["C_Wlepton_theta_Q_negative"]->fill( charge*cos_theta_W / GeV);
	      _h["C_Zlepton_theta_negative"]->fill( cos_theta_Z / GeV);

	    }

	    }
    }

     void normalizeToSumHeight(Histo1DPtr hist) {
		
		for (size_t iBin = 0; iBin < hist->numBins(); ++iBin) hist->bin(iBin).scaleW( fabs(hist->bin(iBin).xMax()-hist->bin(iBin).xMin()) );
		
		double sum = 0;
		for (size_t iBin = 0; iBin < hist->numBins(); ++iBin) sum += hist->bin(iBin).height();
		
		scale(hist, 1./sum);
	}
	
    /// Normalise histograms etc., after the run
    void finalize() {
      
      normalizeToSumHeight(_h["Z_pt"]);
      normalizeToSumHeight(_h["LeadingJet_pt"]);
      normalizeToSumHeight(_h["nJets"]);
	  normalizeToSumHeight(_h["WZ_mass"]);

	  normalizeToSumHeight(_h["Wlepton_inclusive_pt"]);
	  normalizeToSumHeight(_h["Wlepton_positive_pt"]);
	  normalizeToSumHeight(_h["Wlepton_negative_pt"]);
        

	  normalizeToSumHeight(_h["C_Wlepton_theta_Q_inclusive"]);
	  normalizeToSumHeight(_h["C_Wlepton_theta_Q_positive"]);
	  normalizeToSumHeight(_h["C_Wlepton_theta_Q_negative"]);

	  normalizeToSumHeight(_h["C_Zlepton_theta_inclusive"]);
	  normalizeToSumHeight(_h["C_Zlepton_theta_positive"]);
	  normalizeToSumHeight(_h["C_Zlepton_theta_negative"]);
    }

    /// @}


    /// @name Histograms
    /// @{
    map<string, Histo1DPtr> _h;
    map<string, Profile1DPtr> _p;
    map<string, CounterPtr> _c;
    /// @}


  };


  RIVET_DECLARE_PLUGIN(CMS_2021_I1949191);

}
