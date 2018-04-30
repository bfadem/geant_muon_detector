//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/// \file DetectorConstruction.hh
/// \brief Definition of the DetectorConstruction class
//
//
// $Id: DetectorConstruction.hh 98241 2016-07-04 16:56:59Z gcosmo $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4Box;
class G4Tubs;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class DetectorMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
  
    DetectorConstruction();
    virtual ~DetectorConstruction();
    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField(); 
     
  private:

    G4double            scintSeperation;
    G4double            scintRadius;
    G4double            scintHeight;

    G4Material*         fDefaultMaterial;
    G4Box*              fSolidWorld;    //pointer to the solid World 
    G4LogicalVolume*    fLogicWorld;    //pointer to the logical World
    G4VPhysicalVolume*  fPhysiWorld;    //pointer to the physical World

    G4Box*              solidEnv;
    G4LogicalVolume*    logicEnv;
    G4VPhysicalVolume*  pvEnv;

    G4Material*         scintMaterial;

    G4Tubs*             topScint;
    G4LogicalVolume*    log_topScint;
    G4VPhysicalVolume*  phys_topScint;

    G4Tubs*             bottomScint;
    G4LogicalVolume*    log_bottomScint;
    G4VPhysicalVolume*  phys_bottomScint;

    G4Material*         chipMaterial;
    G4Box*              chip;
    G4LogicalVolume*    log_chip;
    G4VPhysicalVolume*  phys_chip;
          
    DetectorMessenger* fDetectorMessenger;  //pointer to the Messenger
      
  private:
    
     G4VPhysicalVolume* ConstructDetector();   

  public:
     G4double getScintSeperation() { return scintSeperation; };
     G4double getScintRadius() { return scintRadius; };  
     G4double getScintHeight() { return scintHeight; };
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

