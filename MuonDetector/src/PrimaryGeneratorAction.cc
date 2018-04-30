#include "PrimaryGeneratorAction.hh"

#include "DetectorConstruction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"

#include <random>
#include <math.h>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction(DetectorConstruction* DC)
: G4VUserPrimaryGeneratorAction(),
  fParticleGun(0),
  fDetector(DC)
{
  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);

  // Set default particle to muon
  fParticleGun->SetParticleDefinition(G4ParticleTable::GetParticleTable()
    ->FindParticle("mu-"));

  // default particle kinematics
  fParticleGun->SetParticleEnergy(3.*GeV);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  //This function is called at the begining of event
  //
  double height = 1.5*(fDetector->getScintSeperation());

  // double x = this->genUniformRealNum(0,fDetector->getScintRadius()) * pow(-1,rand());
  fParticleGun->SetParticlePosition(G4ThreeVector(0.*cm, 0.*cm, height/2));
  fParticleGun->SetParticleMomentumDirection({0, 0, -1});

  // fParticleGun->SetParticlePosition({0,0,height/2});
  // fParticleGun->SetParticlePosition(genPointOnCircle(fDetector->getScintRadius()));
  // double randPhi = 2. * M_PI * genUniformRealNum(0,1);
  // double thetaCut = atan(fDetector->getScintRadius() / fDetector->getScintHeight());
  // double u = genUniformRealNum(-1,-2 + thetaCut);
  // fParticleGun->SetParticleMomentumDirection(vectorOnSphere(1.,u,randPhi));

  fParticleGun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreeVector PrimaryGeneratorAction::vectorOnSphere(double radius, double u,
  double phi)
{
  double x = radius * sqrt(1-(u*u)) * cos(phi);
  double y = radius * sqrt(1-(u*u)) * sin(phi);
  double z = radius * u;
  return G4ThreeVector(x,y,z);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreeVector PrimaryGeneratorAction::genPointOnCircle(double radius)
{
  //Generate a point on a disk from a uniform distribution
  double randPhi = 2. * M_PI * genUniformRealNum(0,1);
  double randRadius = std::sqrt(genUniformRealNum(0,1) * std::pow(radius,2));

  G4ThreeVector position = G4ThreeVector(randRadius * cos(randPhi),
                                         randRadius * sin(randPhi),
                                                                0);
  return position;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

double PrimaryGeneratorAction::genUniformRealNum(double min,double max)
{
  //Generate a number from a uniformly distributed set of doubles
  //This is poorly implemented. std::random_device and std::mt19937
  //Are huge objects and it is not necessary to create them for each call.
  std::random_device                        rand_dev;
  std::mt19937                              generator(rand_dev());
  std::uniform_real_distribution<double>    distr(min, max);
  return distr(generator);
}
