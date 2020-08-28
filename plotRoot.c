// ROOT macro file for plotting example B4 histograms 
// 
// Can be run from ROOT session:
// root[0] .x plotHisto.C
{
  gROOT->Reset();
  gROOT->SetStyle("Modern");
  
  // Draw histos filled by Geant4 simulation 
  //   

  // Open file filled by Geant4 simulation 
 // for(int i = 20;i<=60;i+=10)
//{
  char a[100];
  //char b[100];
  //sprintf(a,"%d", i);
  //sprintf(b,"%d", i);
  strcat(a,"individual.jpg");
  //strcat(b,"_B3A.root");
  //TFile f(b);
  TFile f("B3Atest.root");
  // Create a canvas and divide it into 2x2 pads
  TCanvas* c1 = new TCanvas("c1", "", 20, 20, 1000, 1000);
  const int nx = 3;
  const int ny = 4;

  c1->Divide(nx,ny);
  TH1D* hist1;
  for(int j = 1; j<=ny; j++)
  {
    for(int i = 1; i<=nx; i++)
    {
      int num = ((j-1)*nx+i);
      c1->cd(num);

      //char name[100];
      //sprintf(name,"%d", i);
      //sprintf(name,"%d", j);
      //strcat(name,"Photon-Deposition Per gamma (nOfPhotons)");
      //std::cout<<name<<std::endl;

      std::string name = to_string(i)+to_string(ny-j+1)+"_Photon-Deposition per gamma (nOfPhotons)";
      char *cptr = new char[name.size()+1]; // +1 to account for \0 byte
      std::strncpy(cptr, name.c_str(), name.size());

      hist1 = (TH1D*)f.Get(cptr);
      if(hist1)
      {
        hist1->Draw("HIST");
        hist1->GetXaxis()->SetTitle("Number of Photons");
        hist1->GetYaxis()->SetTitle("Number of Gamma Events");
      }
      else
      {
        std::cout << "ERROR !" <<std::endl;
      }
      
      
    }  
  }


  /*
  c1->Divide(2,3);

  // Draw Eabs histogram in the pad 1

  c1->cd(1);
  TH1D* hist1 = (TH1D*)f.Get("EDep");
  hist1->Draw("HIST");
  
  // Draw Labs histogram in the pad 2
  c1->cd(2);
  gPad->SetLogy(1);
  //TH1D* hist2 = (TH1D*)f.Get("Labs");
  hist1->Draw("HIST");
  

  c1->cd(3);
  TH1D* hist4 = (TH1D*)f.Get("EDep2D");
  hist4->Draw("COLZ");
  // Draw Egap histogram in the pad 3
  // with logaritmic scale for y
  TH1D* hist3 = (TH1D*)f.Get("Energy-netfraction");
  c1->cd(4);
  gStyle->SetEndErrorSize(0.001);
  hist3->SetMarkerStyle(20);
  hist3->SetMarkerColor(kRed);
  hist3->Draw("E1");
  

  c1->cd(5);
  TH1D* hist5= (TH1D*)f.Get("Dose");
  hist5->Draw("HIST");

  c1->cd(6);
  TH1D* hist6 = (TH1D*)f.Get("E-Deposition Total");
  hist6->Draw("HIST");
  // Draw Lgap histogram in the pad 4
  // with logaritmic scale for y
*/

  c1->Print(a);
//}
}  
