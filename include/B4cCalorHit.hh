
#ifndef B4cCalorHit_h
#define B4cCalorHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4Threading.hh"

/// Calorimeter hit class
///
/// It defines data members to store the the energy deposit
/// of charged particles in a selected volume:
/// - fEdep

class B4cCalorHit : public G4VHit
{
  public:
    B4cCalorHit();
    B4cCalorHit(const B4cCalorHit&);
    virtual ~B4cCalorHit();

    // operators
    const B4cCalorHit& operator=(const B4cCalorHit&);
    G4int operator==(const B4cCalorHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    // methods from base class
    virtual void Draw() {}
    virtual void Print();

    //G4Double GetEdep() { return "HELLO WORLD";};

    // get methods
    void SetEdep(G4double edep) { fEdep = edep; };
    G4double GetEdep() { return fEdep; };
    void SetPosition(G4ThreeVector pos) {position = pos; };
    G4ThreeVector GetPosition() { return position; }
    void SetPDGID(G4int id) { PDGID = id; };
    G4double GetPDGID() { return PDGID; };
    G4String        GetParticleName()           { return particleName; };
    void SetParticleName(G4String name)         { particleName = name; };

    void SetTrackID(G4int tckID)                { trackID = tckID; };
    void SetParentID(G4int parID)               { parentID = parID; };
    G4int           GetTrackID()                { return trackID; };
    G4int           GetParentID()               { return parentID; };

  private:
    G4double fEdep;        ///< Energy deposit in the sensitive volume
    G4int    PDGID;
    G4ThreeVector position;
    G4int   trackID;
    G4int   parentID;
    std::string particleName;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

using B4cCalorHitsCollection = G4THitsCollection<B4cCalorHit>;

extern G4ThreadLocal G4Allocator<B4cCalorHit>* B4cCalorHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* B4cCalorHit::operator new(size_t)
{
  if (!B4cCalorHitAllocator) {
    B4cCalorHitAllocator = new G4Allocator<B4cCalorHit>;
  }
  void *hit;
  hit = (void *) B4cCalorHitAllocator->MallocSingle();
  return hit;
}

inline void B4cCalorHit::operator delete(void *hit)
{
  if (!B4cCalorHitAllocator) {
    B4cCalorHitAllocator = new G4Allocator<B4cCalorHit>;
  }
  B4cCalorHitAllocator->FreeSingle((B4cCalorHit*) hit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
