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
/// \file electromagnetic/TestEm11/src/HistoManager.cc
/// \brief Implementation of the HistoManager class
//
//
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "B3aHistoManager.hh"
//#include "B3DetectorConstruction.hh"
#include "GDMLDetectorConstruction.hh"
#include "DetectorConstruction.hh"

#include "G4UnitsTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::HistoManager(G4VUserDetectorConstruction* patient)
  : fFileName("B3Atest"),fpatient(patient)
{
  Book();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::~HistoManager()
{
  delete G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Book()
{
  // Create or get analysis manager
  // The choice of analysis technology is done via selection of a namespace
  // in HistoManager.hh
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetFileName(fFileName);
  analysisManager->SetVerboseLevel(0);
  analysisManager->SetActivation(true);

  // Define histograms start values
  const G4int kMaxHisto = 2;
  const G4String id[] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
                         "10","11","12","13","14","15","16","17","18","19",
                         "20","21","22"};

  /*const G4String title[] = 
                { "dummy",                                        //0
                  "Edep (MeV/mm) along absorbers",                //1
                  "total Energy deposited in absorbers",          //2
                  "true track length of the primary particle",    //3
                  "true step size of the primary particle",       //4
                  "projected range of the primary particle",      //5
                  "true track length of charged secondaries",     //6
                  "true step size of charged secondaries",        //7
                  "Edep (MeV.cm2/g) along x/r0",                  //8
                  "dummy",                                        //9
                  "dummy"                                         //10
                 };
*/
   const G4String title[] = 
                { "E-Deposition (keV/mm)",
		  "E-Deposition 2D (keV/mm)",
		  "E-Deposition 3D (keV/mm)",
		  "Secondary List",
"E-Deposition Total (keV)",
"E-Deposition y (keV)",
"E-Deposition opticalPhotons (keV)",
"E-Deposition e-  (keV)",
"Dose (Gy)",
"Energy Change (keV/step)",
"Photon-Deposition 2D (keV/mm)"};

   const std::string second[] = { "positron","electron","opticalphoton","gammas","proton","alpha","Li6","Be7","C11","C12","N15","O15","O16"};
   const int secondSize = sizeof(second)/sizeof(second[0]);
  // Default values (to be reset via /analysis/h1/set command)
  G4double* worldsizeP;
  G4double worldsize = (((DetectorConstruction*) fpatient)->GetWorldSizeXY());
  worldsizeP = new G4double(worldsize);

  G4int nbinsx = 100;
  G4double xmin = 0.;
  G4double xmax = 300;
  G4int nbinsy = 100;
  G4double ymin = 0.;
  G4double ymax = 300;
  G4int nbinsz = 100;
  G4double zmin = 0.;
  G4double zmax = 300;
  G4double maxEn = 511;//keV

  G4double sbins = secondSize;

    G4int ih = analysisManager->CreateH1(title[0], title[0], nbinsx, ymin, ymax);
    analysisManager->SetH1Activation(ih, true);
G4cout << "### ####### " << worldsize << " worldsize." << G4endl;
    G4int ih2 = analysisManager->CreateH2(title[1], title[1], nbinsx, xmin, xmax, nbinsy, ymin, ymax);
    analysisManager->SetH2Activation(ih2, true);
    G4int ih3 = analysisManager->CreateH3(title[2], title[2], nbinsx, xmin, xmax, nbinsy, ymin, ymax, nbinsz, zmin, zmax);
    analysisManager->SetH2Activation(ih3, true);





    /*analysisManager->CreateNtuple(id[3], title[3]);
	for(int i = 0; i < secondSize; i++)
	{
		analysisManager->CreateNtupleDColumn(second[i]);
	}
	analysisManager->FinishNtuple();*/
    

    G4int ih4 = analysisManager->CreateH1(title[3], title[3], sbins, 0, sbins);
    analysisManager->SetH1Activation(ih4, true);

//could have used a loop w endpointlist --
G4int nBinE = 100;
G4int nBinG = 100;

	G4int ih5 = analysisManager->CreateH1(title[4], title[4], nBinE,0,maxEn);
    	analysisManager->SetH1Activation(ih5, true);
ih5 = analysisManager->CreateH1(title[5], title[5], nBinE,0,maxEn);
    	analysisManager->SetH1Activation(ih5, true);
ih5 = analysisManager->CreateH1(title[6], title[6], nBinE,0,maxEn);
    	analysisManager->SetH1Activation(ih5, true);
ih5 = analysisManager->CreateH1(title[7], title[7], nBinE,0,maxEn);
    	analysisManager->SetH1Activation(ih5, true);

ih5 = analysisManager->CreateH1(title[8], title[8], nBinG,0,5.e-12);
    	analysisManager->SetH1Activation(ih5, true);

  G4int nbinst = 40000;
  G4double tmin = 0.;
  G4double tmax = 40000; //in steps


ih5 = analysisManager->CreateH1(title[9], title[9], nbinst, tmin, tmax);
    	analysisManager->SetH1Activation(ih5, true);

G4double Ox = -2.58*cm;
G4double Oy = 4.83*cm;
G4double Dx = 7.74*cm;
G4double Dy = 10.32*cm;
G4int nx = 3;
G4int ny = 16; //4x4

ih5 = analysisManager->CreateH2(title[10], title[10], nx, (-Dx/2), (Dx/2), ny,(-Dy/2),(Dy/2));
    analysisManager->SetH2Activation(ih2, true);


/*
  // Create all histograms as inactivated 
  // as we have not yet set nbins, vmin, vmax
  for (G4int k=0; k<=kMaxHisto; k++) {

  }
  
 G4String title2;
  for (G4int k=1; k<kMaxAbsor; k++) {
    title2 = "Edep in absorber " + id[k];
    G4int ih 
      = analysisManager->CreateH1(id[kMaxHisto+k], title2, nbins, vmin, vmax);
    analysisManager->SetH1Activation(ih, false);
  }*/
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
