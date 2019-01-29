
#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"
#include "B4cCalorimeterSD.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4SystemOfUnits.hh"

#include "G4SDManager.hh"

//....oooOO0OOoo o........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
:G4VUserDetectorConstruction(),
 fDefaultMaterial(0),fSolidWorld(0),fLogicWorld(0),fPhysiWorld(0),
 solidEnv(0),logicEnv(0),pvEnv(0),
 scintMaterial(0),topScint(0),log_topScint(0),phys_topScint(0),
 bottomScint(0),log_bottomScint(0),phys_bottomScint(0),
 fDetectorMessenger(0)
{

  //scintSeperation = 70*cm; //All little over 2ft
  scintSeperation = 20*cm;
  scintRadius = 3.81*cm;   //1 and a half inch radius
  scintHeight = 1.905*cm;  //1 and a half inch height

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{ delete fDetectorMessenger;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  return ConstructDetector();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::ConstructDetector()
{
  auto nistManager = G4NistManager::Instance();

  // Clean old geometry, if any
  //
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  //
  // World
  //
  double worldSize = 2.2*scintSeperation;

  fSolidWorld = new G4Box("World",                                //its name
                   worldSize/2,worldSize/2,worldSize/2);  //its size

  fLogicWorld = new G4LogicalVolume(fSolidWorld,            //its solid
                                   nistManager->FindOrBuildMaterial("G4_Galactic"), //material
                                   "World");                //its name

  fPhysiWorld = new G4PVPlacement(0,                        //no rotation
                                 G4ThreeVector(),           //at (0,0,0)
                                 fLogicWorld,             //its logical volume
                                 "World",                   //its name
                                 0,                         //its mother  volume
                                 false,                  //no boolean operation
                                 0);                        //copy number
  //
  // Atmosphere Envelope
  //
  double env_size = 2*scintSeperation;

  solidEnv =
    new G4Box("Envelope",                    //its name
        0.5*env_size, 0.5*env_size, 0.5*env_size); //its size

  logicEnv =
    new G4LogicalVolume(solidEnv,            //its solid
                        nistManager->FindOrBuildMaterial("G4_AIR"),
                        "EnvelopeLV");         //its name

  pvEnv =
    new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(),         //at (0,0,0)
                    logicEnv,                //its logical volume
                    "Envelope",              //its name
                    fLogicWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0);                       //copy number

  //
  //Top Scintillator
  //
  scintMaterial = nistManager->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");

  topScint =
    new G4Tubs("topScintillator",
                 0, //inner radius
                 scintRadius, //Outer radius
                 scintHeight,
                 0, //startAngleOfTheTube
                 360); //spanningAngleOfTheTube

  log_topScint =
    new G4LogicalVolume(topScint,          //its solid
                        scintMaterial,           //its material
                        "topLV");            //its name

  phys_topScint =
    new G4PVPlacement(0,                    //no rotation
                  G4ThreeVector(0,0,scintSeperation*0.5),
                  log_topScint,           //its logical volume
                  "topScintillator",         //its name
                  logicEnv,           //its mother  volume
                  false,              //no boolean operation
                  0);                    //copy number

  //auto topScintSD = new B4cCalorimeterSD("TopScintSD", "TopScintCollection");
  //G4SDManager::GetSDMpointer()->AddNewDetector(topScintSD);
  //SetSensitiveDetector("topScintillator",topScintSD);

  //
  //Bottom Scintillator
  //
    /*
  bottomScint =
    new G4Tubs("bottomScintillator",
                 0, //inner radius
                 scintRadius, //Outer radius
                 scintHeight,
                 0, //startAngleOfTheTube
                 360); //spanningAngleOfTheTube

  log_bottomScint =
    new G4LogicalVolume(bottomScint,          //its solid
                        scintMaterial,           //its material
                          "bottomLV");            //its name

  phys_bottomScint =
    new G4PVPlacement(0,                    //no rotation
                  G4ThreeVector(0,0,-scintSeperation*0.5),
                  log_bottomScint,           //its logical volume
                  "bottomScintillator",         //its name
                  logicEnv,           //its mother  volume
                  false,              //no boolean operation
                  0);                    //copy number
*/
  //
  //Chip for Top Scintillator
  //
  double chipSize = 6*mm;

  chip = new G4Box("chip",                                //its name
                   chipSize/2,chipSize/2,0.25*mm);  //its size

  log_chip = new G4LogicalVolume(chip,            //its solid
                                  nistManager->FindOrBuildMaterial("G4_Si"),//its material
                                   "chipLV");                //its name

  phys_chip = new G4PVPlacement(0,                        //no rotation
                                 G4ThreeVector(0,0,scintSeperation*0.5-scintHeight-chipSize/2),
                                 log_chip,             //its logical volume
                                 "chip",                   //its name
                                 logicEnv,                         //its mother  volume
                                 false,                  //no boolean operation
                                 0);                        //copy number

  //
  // Visualization attributes
  //

  fLogicWorld->SetVisAttributes (G4VisAttributes::GetInvisible());

  G4VisAttributes* simpleBoxVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  simpleBoxVisAtt->SetVisibility(true);
  logicEnv->SetVisAttributes(simpleBoxVisAtt);

  return fPhysiWorld; //always return the physical World
}


void DetectorConstruction::ConstructSDandField()
{
  // G4SDManager::GetSDMpointer()->SetVerboseLevel(1);

  //
  // Sensitive detectors
  //
  auto chipSD = new B4cCalorimeterSD("ChipSD", "ChipCollection");
  G4SDManager::GetSDMpointer()->AddNewDetector(chipSD);
  SetSensitiveDetector("chipLV",chipSD);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
