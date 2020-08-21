#ifndef _GDMLDETECTORCONSTRUCTION_H_
#define _GDMLDETECTORCONSTRUCTION_H_
#include "Version.hh"
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
#include "G4PSNofCollision.hh"
#include "G4VisAttributes.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4VSDFilter.hh"
#include "G4SDParticleFilter.hh"
#include "G4Scintillation.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4PhysicalConstants.hh"

#include "G4GDMLParser.hh"

using namespace std ;

class G4VPhysicalVolume;
class G4LogicalVolume;

//#include "B3DetectorConstruction.hh"
class GDMLDetectorConstruction : public DetectorConstruction
{
  public:
   GDMLDetectorConstruction(G4GDMLParser& parser, G4VPhysicalVolume *setWorld = 0) : DetectorConstruction()
   {
	G4cout <<""<< G4endl;
	G4cout << "--//\\\\Scintillator Volume ---- GDML - : " << parser.GetVolume("S_EJ208(1)")->GetSolid()->GetCubicVolume()/(cm3) << G4endl;
	G4cout <<""<< G4endl;
     	
		World = setWorld;

	//--------------

	G4NistManager* man = G4NistManager::Instance();

	G4Material* Air = man->FindOrBuildMaterial("G4_AIR");

  	G4bool isotopes = false;
  
  	G4Element*  H = man->FindOrBuildElement("H" , isotopes);
	G4Element*  C = man->FindOrBuildElement("C" , isotopes);

	G4Material* Vikuiti = new G4Material("Vikuiti", 1.07*g/cm3, 2);
	Vikuiti->AddElement(C,8);
	Vikuiti->AddElement(H,8);
	Vikuiti->GetIonisation()->SetBirksConstant(0.126*mm/MeV);

	G4Material* EJ208 = new G4Material("EJ208", 1.023*g/cm3, 2);
	EJ208->AddElement(C,9);
	EJ208->AddElement(H,10);
	EJ208->GetIonisation()->SetBirksConstant(0.1955*mm/MeV);
	
	const G4int n = 6;

	G4double PhotonEnergy[n] = {3.105*eV,2.95714*eV,2.855*eV,2.7*eV,2.5875*eV,2.388*eV}; //visible spectrum (400,420,435,460,480,520)nm
	
	G4double refractive_index_vk[n] = {1.6,1.6,1.6,1.6,1.6,1.6};
	G4double att_length_vk[n] = {400*cm,400*cm,400*cm,400*cm,400*cm,400*cm};

	G4double refractive_index_ej[n] = {1.58,1.58,1.58,1.58,1.58,1.58};
	G4double att_length_ej[n] = {400*cm,400*cm,400*cm,400*cm,400*cm,400*cm};

	G4double fast[n] = {0.032258,0.258064,0.322581,0.225806,0.129032,0.032258};//{0.1,0.8,1,0.7,0.4,0.1}

	#ifndef SSRefractionTest
		G4double reflectivity_vk[n] = {.9150,.9334,.9452,.9566,.9652,.9698}; //using ESR_Clear. DRP:{0.9643,0.9680,0.9698,0.9743,0.9761,0.9798};
	#else
		G4double reflectivity_vk[n] = {1,1,1,1,1,1};
		//G4double reflectivity_vk[n] = {0,0,0,0,0,0}; //using ESR_Clear. DRP:{0.9643,0.9680,0.9698,0.9743,0.9761,0.9798};
	#endif
	

	G4double efficiency[n] = {1.0,1.0,1.0,1.0,1.0,1.0};

	G4MaterialPropertiesTable* vkMPT = new G4MaterialPropertiesTable();
	G4MaterialPropertiesTable* scintMPT = new G4MaterialPropertiesTable();
	G4MaterialPropertiesTable* surfVKMPT = new G4MaterialPropertiesTable();
	G4MaterialPropertiesTable* surfEJMPT = new G4MaterialPropertiesTable();

	vkMPT->AddProperty("RINDEX", PhotonEnergy,refractive_index_vk,n);
	vkMPT->AddProperty("ABSLENGTH", PhotonEnergy,att_length_vk,n);

	scintMPT->AddProperty("RINDEX", PhotonEnergy,refractive_index_ej,n);
	scintMPT->AddProperty("ABSLENGTH", PhotonEnergy,att_length_ej,n);
	#ifndef ScintillationDisable
	scintMPT->AddProperty("FASTCOMPONENT", PhotonEnergy, fast, n);
  	scintMPT->AddConstProperty("SCINTILLATIONYIELD", 9200 / MeV);
  	scintMPT->AddConstProperty("RESOLUTIONSCALE", 1.0);
	scintMPT->AddConstProperty("FASTSCINTILLATIONRISETIME", 1.0 * ns);
 	scintMPT->AddConstProperty("FASTTIMECONSTANT", 3.3 * ns);
	scintMPT->AddConstProperty("YIELDRATIO", 1.0);
	vkMPT->AddProperty("REFLECTIVITY", PhotonEnergy, reflectivity_vk, n);
	scintMPT->AddProperty("REFLECTIVITY", PhotonEnergy, reflectivity_vk, n); //WHAT is the EJ reflectivity?
	#endif

	EJ208->SetMaterialPropertiesTable(scintMPT);
	Vikuiti->SetMaterialPropertiesTable(vkMPT);
	
	surfVKMPT->AddProperty("REFLECTIVITY", PhotonEnergy, reflectivity_vk, n);
	surfEJMPT->AddProperty("REFLECTIVITY", PhotonEnergy, reflectivity_vk, n); // WHAT is the EJ reflectivity?
	surfVKMPT->AddProperty("EFFICIENCY", PhotonEnergy, efficiency, n);
	surfEJMPT->AddProperty("EFFICIENCY", PhotonEnergy, efficiency, n);
	surfVKMPT->AddProperty("RINDEX", PhotonEnergy,refractive_index_vk,n);
	surfEJMPT->AddProperty("RINDEX", PhotonEnergy,refractive_index_ej,n);



	G4OpticalSurface* surfVK = new G4OpticalSurface("surfVK");
	G4OpticalSurface* surfEJ = new G4OpticalSurface("surfEJ");

	#ifndef ReflectionDisable
	G4double sigma_alpha_ground = 0.209439; //12deg., ground
	G4double sigma_alpha_polish = 0.0226893; //1.3 deg., polished 
	G4double specularlobe[n] = {1.0,1.0,1.0,1.0,1.0,1.0}; //all specular lobe (microfacet reflections) // test with specular spike (perfectly smooth surface!)
	G4double specularspike[n] = {0.0,0.0,0.0,0.0,0.0,0.0};
	G4double backscatter[n] = {0.0,0.0,0.0,0.0,0.0,0.0};
	surfVKMPT->AddProperty("SPECULARLOBECONSTANT",PhotonEnergy,specularlobe,n);
	surfVKMPT->AddProperty("SPECULARSPIKECONSTANT",PhotonEnergy,specularspike,n);
	surfVKMPT->AddProperty("BACKSCATTERCONSTANT",PhotonEnergy,backscatter,n);
	surfEJMPT->AddProperty("SPECULARLOBECONSTANT",PhotonEnergy,specularlobe,n);
	surfEJMPT->AddProperty("SPECULARSPIKECONSTANT",PhotonEnergy,specularspike,n);
	surfEJMPT->AddProperty("BACKSCATTERCONSTANT",PhotonEnergy,backscatter,n);
	surfVK->SetType(dielectric_metal);
	surfVK->SetModel(unified);
	surfVK->SetFinish(ground);
	surfVK->SetSigmaAlpha(sigma_alpha_ground);
	surfVK->SetMaterialPropertiesTable(surfVKMPT);
	surfEJ->SetType(dielectric_dielectric);
	surfEJ->SetModel(unified);
	#ifndef SSRefractionTest
		surfEJ->SetFinish(polishedbackpainted);
	#else
		surfEJ->SetFinish(polishedbackpainted);
	#endif
	surfEJ->SetSigmaAlpha(sigma_alpha_polish);
	surfEJ->SetMaterialPropertiesTable(surfEJMPT);
	#endif


	G4VisAttributes* red_col = new G4VisAttributes(G4Color(0.6,0.4,0.4,1));
	G4VisAttributes* blue_col = new G4VisAttributes(G4Color(0.4,0.4,0.6,1));
	G4VisAttributes* det_col = new G4VisAttributes(G4Color(0.8,0.8,1.0,1));
	G4VisAttributes* invis_col = new G4VisAttributes(G4Color(0.0,0.0,0.0,0));

	#ifdef MultipleStripCell

	G4LogicalVolume* EJvol;
	G4VPhysicalVolume* EJvol_P;
	G4LogicalSkinSurface* EJsurf;
	G4String a;
	for (G4int i = 1; i <= 48; i++) 
	{
        //char a[100];
		a = "S_EJ208(";
	 	a = a + to_string(i);
	  	a = a + ")";
        //sprintf(a,"%d", i);
        //strcat(a,")");
        EJvol = parser.GetVolume(a);
	  	EJvol->SetMaterial(EJ208);
	  	EJvol->SetVisAttributes (red_col);
	  	EJsurf = new G4LogicalSkinSurface("surfEJ_L",EJvol, surfEJ); 	
    }

	G4LogicalVolume* VKvol;
	G4VPhysicalVolume* VKvol_P;
	G4LogicalSkinSurface* VKsurf;
	for (G4int i = 1; i <= 48; i++) 
	{
        //char a[100];
		a = "S_Vikuiti(";
	  	a = a + to_string(i);
	  	a = a + ")";
        //sprintf(a,"%d", i);
        //strcat(a,")");
        VKvol = parser.GetVolume(a);
	  	VKvol->SetMaterial(Vikuiti);
	  	VKvol->SetVisAttributes (blue_col);
	  	VKsurf = new G4LogicalSkinSurface("surfVK_L",VKvol, surfVK);	
    }

	//--------- Detecting (SiPM like) Endcap Geometry -------------//\\

	G4double dX = 7.74*cm; G4double dY = 10.32*cm; G4double dZ = 1*cm;
	G4Box* det = new G4Box("det", dX/2, dY/2, dZ/2);
	G4LogicalVolume* logicDetR = new G4LogicalVolume(det,EJ208,"detVOLR");
	G4LogicalVolume* logicDetL = new G4LogicalVolume(det,EJ208,"detVOLL");
	logicDetR->SetVisAttributes (det_col);
	logicDetL->SetVisAttributes (det_col);
    G4ThreeVector pos = G4ThreeVector(-2.58*cm,4.83*cm,-0.5*cm);
	G4ThreeVector pos2 = G4ThreeVector(-2.58*cm,4.83*cm,100.5*cm);
	new G4PVPlacement(0,pos,"det",logicDetR,World, false,0,fCheckOverlaps);       // checking overlaps 
	new G4PVPlacement(0,pos2,"det",logicDetL,World, false,0,fCheckOverlaps); 

	#endif
	#ifdef SingleStrip


	G4LogicalVolume* EJvol;
	G4VPhysicalVolume* EJvol_P;
	G4LogicalSkinSurface* EJsurf;
	G4String a;
	for (G4int i = 1; i <= 1; i++) {
        //char a[100];
		a = "S_EJ208(";
	  	a = a + to_string(i);
	  	a = a + ")";
        //sprintf(a,"%d", i);
        //strcat(a,")");
        EJvol = parser.GetVolume(a);
	  	EJvol->SetMaterial(EJ208);
	  	EJvol->SetVisAttributes (red_col);
	  	EJsurf = new G4LogicalSkinSurface("surfEJ_L",EJvol, surfEJ); 	
    }

	for (G4int i = 2; i <= 48; i++) {
        //char a[100];
		a = "S_EJ208(";
	  	a = a + to_string(i);
	  	a = a + ")";
        //sprintf(a,"%d", i);
        //strcat(a,")");
        EJvol = parser.GetVolume(a);
	  	EJvol->SetMaterial(Air);
	  	EJvol->SetVisAttributes (invis_col);
	  	//EJsurf = new G4LogicalSkinSurface("surfEJ_L",EJvol, surfEJ); 	
    }
	G4LogicalVolume* VKvol;
	G4VPhysicalVolume* VKvol_P;
	G4LogicalSkinSurface* VKsurf;
	for (G4int i = 1; i <= 48; i++) 
	{
        //char a[100];
	  	a = "S_Vikuiti(";
	  	a = a + to_string(i);
	  	a = a + ")";
        //sprintf(a,"%d", i);
        //strcat(a,")");
        VKvol = parser.GetVolume(a);
	  	VKvol->SetMaterial(Air);
	  	VKvol->SetVisAttributes (invis_col);
	  	//VKsurf = new G4LogicalSkinSurface("surfVK_L",VKvol, surfVK);	
	}

	//--------- Detecting (SiPM like) Endcap Geometry -------------//\\

	G4double dX = 2.54*cm; G4double dY = .62*cm; G4double dZ = 1*cm;
	G4Box* det = new G4Box("det", dX/2, dY/2, dZ/2);
	G4LogicalVolume* logicDetR = new G4LogicalVolume(det,EJ208,"detVOLR");
	G4LogicalVolume* logicDetL = new G4LogicalVolume(det,EJ208,"detVOLL");
	logicDetR->SetVisAttributes (det_col);
	logicDetL->SetVisAttributes (det_col);
	G4ThreeVector pos = G4ThreeVector(-5.14*cm,9.66*cm,-0.5*cm);
	G4ThreeVector pos2 = G4ThreeVector(-5.14*cm,9.66*cm,100.5*cm);
	new G4PVPlacement(0,pos,"det",logicDetR,World, false,0,fCheckOverlaps);       // checking overlaps 
	new G4PVPlacement(0,pos2,"det",logicDetL,World, false,0,fCheckOverlaps); 

	#endif




	/*G4Tubs* testP = new G4Tubs("Crystal",0*cm,10*cm, 5*cm,0,2*pi);
	G4LogicalVolume* lc = new G4LogicalVolume(testP,EJ208,"Crystal");
	G4ThreeVector pos = G4ThreeVector(0*cm,0*cm,0*cm);
	G4VPhysicalVolume* pC = new G4PVPlacement(0,pos,"Crystal", lc, World, false,0); //test crystal

	G4Tubs* testP2 = new G4Tubs("Crystal2",4*cm,10*cm, 5*cm,0,2*pi);
	G4LogicalVolume* lc2 = new G4LogicalVolume(testP2,Vikuiti,"Crystal2");
	G4ThreeVector pos2 = G4ThreeVector(0*cm,8*cm,8*cm);
	G4VPhysicalVolume* pC2 = new G4PVPlacement(0,pos2,"Crystal2", lc2, World, false,0);*/
   }

   G4VPhysicalVolume *Construct()
   {
     return World;
   }

   virtual ~GDMLDetectorConstruction() {};

  public:
    void ConstructSDandField()
    {
		G4SDParticleFilter* photonFilter = new G4SDParticleFilter("photonFilter");
  		photonFilter->add("opticalphoton");
        G4MultiFunctionalDetector* detL = new G4MultiFunctionalDetector("detL");
		G4MultiFunctionalDetector* detR = new G4MultiFunctionalDetector("detR");
        G4SDManager::GetSDMpointer()->AddNewDetector(detL);
		G4SDManager::GetSDMpointer()->AddNewDetector(detR);
        G4VPrimitiveScorer* npho = new G4PSNofCollision("nPho",0);
		npho->SetFilter(photonFilter);
        detL->RegisterPrimitive(npho);
		detR->RegisterPrimitive(npho);

        SetSensitiveDetector("detVOLL",detL);
		SetSensitiveDetector("detVOLR",detR);

    }


  private:
    G4VPhysicalVolume *World;
    void DefineMaterials();
    G4bool  fCheckOverlaps;

};

#endif
