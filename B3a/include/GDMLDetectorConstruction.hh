#ifndef _GDMLDETECTORCONSTRUCTION_H_
#define _GDMLDETECTORCONSTRUCTION_H_

#include "DetectorConstruction.hh"
#include "globals.hh"


#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4PSEnergyDeposit.hh"
#include "G4PSDoseDeposit.hh"
#include "G4VisAttributes.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4VSDFilter.hh"
#include "G4SDParticleFilter.hh"
#include "G4Scintillation.hh"
#include "G4PhysicalConstants.hh"


class G4VPhysicalVolume;
class G4LogicalVolume;

//#include "B3DetectorConstruction.hh"
class GDMLDetectorConstruction : public DetectorConstruction
{
  public:
   GDMLDetectorConstruction(G4VPhysicalVolume *setWorld = 0) : DetectorConstruction()
   {
      World = setWorld;
   }

   G4VPhysicalVolume *Construct()
   {
     return World;
   }

   virtual ~GDMLDetectorConstruction() {};

  public:
    void ConstructSDandField()
    {
        /*G4MultiFunctionalDetector* patient = new G4MultiFunctionalDetector("patient");
        G4SDManager::GetSDMpointer()->AddNewDetector(patient);
        G4VPrimitiveScorer* sec1 = new G4PSDoseDeposit("dose");
        patient->RegisterPrimitive(sec1);*/

        /*for (G4int i = 0; i < 109; i++) {
          char a[100];
          sprintf(a,"%d", i);
          strcat(a,"_EmphaticLV");
          SetSensitiveDetector(a,patient);
        }*/
        /*SetSensitiveDetector("_EmphaticLV",patient);*/
    }


  private:
    G4VPhysicalVolume *World;
    void DefineMaterials();
    G4bool  fCheckOverlaps;

};

#endif
