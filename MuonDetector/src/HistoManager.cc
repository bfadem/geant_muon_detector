#include "HistoManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "B4cCalorHit.hh"
#include "EventAction.hh"
#include <TH1D.h>
#include <TH2D.h>
#include <TFile.h>
#include <TTree.h>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::HistoManager()
:rootFile(0),
histo(0)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::~HistoManager()
{ if (rootFile) delete rootFile; }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Book()
{
  G4String fileName = "AnaEx02.root";
  rootFile = new TFile(fileName,"RECREATE");
  if (! rootFile) {
    G4cout << " HistoManager::Book :"
           << " problem creating the ROOT TFile "
           << G4endl;
    return;
  }

  hitsTree = new TTree("hitsTree", "hitsTree");
  hitsTree->Branch("Edep", &edep);
  hitsTree->Branch("Angle",&angle);
  hitsTree->Branch("PDGID", &PDGID);

  histo = new TH1D("edep","Edep (MeV)",100,0.,1*CLHEP::MeV);
  edepVsXPos = new TH2D("edepVsXPos","Edep Vs. X",100,10.,10.,100,10.,10.);
  edepVsAngle = new TH2D("edepVsAngle","Edep Vs. Angle",100,10.,10.,100,10.,10.);

  G4cout << "\n----> Output file is open in " << fileName << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Save()
{
  if (! rootFile) return;

  rootFile->Write();       // Writing the histograms to the file
  rootFile->Close();       // and closing the tree (and the file)

  G4cout << "\n----> Histograms and ntuples are saved\n" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::fill(B4cCalorHitsCollection* hits,const G4Event* event)
{
  G4int nhits = hits->entries();
  for(G4int i_hitcounter = 0; i_hitcounter < nhits; i_hitcounter++)
  {
    G4ThreeVector momDir = event->GetPrimaryVertex()->GetPrimary()->GetMomentumDirection();
    double x = momDir.x();
    double y = momDir.y();
    double z = momDir.z();
  	edep = (*hits)[i_hitcounter]->GetEdep();
    angle = atan2(sqrt(x*x + y*y),-z)*180/3.14;
    PDGID = (*hits)[i_hitcounter]->GetPDGID();

    hitsTree->Fill();
    histo->Fill((*hits)[i_hitcounter]->GetEdep());
    edepVsXPos->Fill(event->GetPrimaryVertex()->GetX0(), edep);
    edepVsAngle->Fill(angle, edep);
    std::cout<<(*hits)[i_hitcounter]->GetParticleName()<<" Hit!! Edep: ";
    std::cout<<edep<<" MeV"<<std::endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/*
void HistoManager::fill(B4cCalorHitsCollection* hits,const G4Event* event)
{
  if (!hits) return;

  G4int nhits = hits->entries();
  if (nhits > 0) {

    edep = 0;
    PDGID = (*hits)[0]->GetPDGID();
    for(G4int i = 0; i < nhits; i++)
    {

      if (i + 1 < nhits) {
        if ( sameID((*crystalHits)[i], (*crystalHits)[i+1]) )
        {
          edep += (*crystalHits)[i]->GetEdep();
        } else {
          edep += (*crystalHits)[i]->GetEdep();
          hitsTree->Fill();
          histo->Fill(edep);
          edep = 0;
          PDGID = (*hits)[i+1]->GetPDGID();
        }
      } else { //Handels the final entry in hits
        edep += (*crystalHits)[i]->GetEdep();
        hitsTree->Fill();
        histo->Fill(edep);
      }

      G4ThreeVector momDir = event->GetPrimaryVertex()->GetPrimary()->GetMomentumDirection();
      double x = momDir.x();
      double y = momDir.y();
      double z = momDir.z();
      angle = atan2(sqrt(x*x + y*y),-z)*180/3.14;
      PDGID = (*hits)[i_hitcounter]->GetPDGID();

      edepVsXPos->Fill(event->GetPrimaryVertex()->GetX0(), edep);
      edepVsAngle->Fill(angle, edep);
    }
  }
}
*/
