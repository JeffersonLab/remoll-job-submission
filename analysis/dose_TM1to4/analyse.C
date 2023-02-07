int analyse(TString source, TString out, TString gen){
  
  /*Add source root file to TChain and get total number of events*/
  
  TChain T("T");
  T.Add(Form("%s", source.Data())); 
  Int_t nEvents = T.GetEntries();  
  std::cout << "Analyzing "<< nEvents << "events" << std::endl;
  
  /*Initiate weight variable that allows us to express rate as count per incident electron.
  For physics generators, first we need to normalize fRate in Hz by beam current (this should be set to whatever the
  sims were run with. Assuming 85 uA here).
  Then we convert Hz/uA to count/incident electron
    Hz/uA = 1/(1e-6 Coulombs) = e/(1e-6*e*Couloumbs) = 1.602e-19Coulombs/(1e-6*e*Coulombs)
                                     = 1.602e-13*1/e
  For beam generator, fRate is set to 1. So, to get count per incident electron, it's ok to just normalize by the number
  of events.
  This results in the unit of power weighted plots being
    MeV/e = W/uA
  */
  
  Double_t weight=1;
  if (gen!="beam"){
    weight= (1.0/85)*(1.602*1e-13); // 1/e. 
  }else{
    weight= 1.0/nEvents; // 1/e
  }
  
  /*Initiate output root file and histograms*/
  
  TFile f(Form("%s", out.Data()), "RECREATE");

  std::map<TString, TH2D*> h_de_rz_left;
  std::map<TString, TH2D*> h_de_rz_right;
  std::map<TString, TH2D*> h_de_phz_bottom;
  std::map<TString, TH2D*> h_de_phph_nose;
  std::map<TString, TH1D*> h_de_rz_left_1D;
  std::map<TString, TH1D*> h_de_rz_right_1D;
  std::map<TString, TH1D*> h_de_phz_bottom_1D;
  std::map<TString, TH1D*> h_de_phph_nose_1D;

  /*There are 7 subcoils in each torus magnet and the hits are divided into 4 energy ranges*/
  Int_t n_septant=7;
  Double_t size_septant=2.0*TMath::Pi()/n_septant;
  std::vector<Double_t> off_septant;
  
  Int_t energy_bins=4;

  TString part;
  
  for(Int_t k=0;k<energy_bins;k++){
    for(Int_t i=0; i<n_septant; i++){
      off_septant.push_back((3.0-1.0*i)*size_septant);
      part= Form("pr_%d_E%d",i+1,  k);
      std::cout<< part<< std::endl;
      h_de_rz_left[part]=new TH2D(part+"_de_rz_left", Form("%s downstream edep, Generator=%s", part.Data(), gen.Data()), 7500, 4800, 12300, 450, 0, 450);
      h_de_rz_right[part]=new TH2D(part+"_de_rz_right", Form("%s downstream epoxy edep, Generator=%s", part.Data(), gen.Data()), 7500, 4800, 12300, 450, 0, 450);
      h_de_phz_bottom[part]=new TH2D(part+"_de_phz_bottom", Form("%s downstream edep, Generator=%s", part.Data(), gen.Data()), 7500, 4800, 12300, 50, -25.0, 25.0);
      h_de_phph_nose[part]=new TH2D(part+"_de_phph_nose", Form("%s downstream edep, Generator=%s", part.Data(), gen.Data()), 450, -225, 225, 210, -25.0, 185.0);
      h_de_rz_left_1D[part]=new TH1D(part+"_de_rz_left_1D", Form("%s downstream epoxy edep, Generator=%s", part.Data(), gen.Data()), 7500, 4800, 12300);
      h_de_rz_right_1D[part]=new TH1D(part+"_de_rz_right_1D", Form("%s downstream edep, Generator=%s", part.Data(), gen.Data()), 7500, 4800, 12300);
      h_de_phz_bottom_1D[part]=new TH1D(part+"_de_phz_bottom_1D", Form("%s downstream epoxy edep, Generator=%s", part.Data(), gen.Data()), 7500, 4800, 12300);
      h_de_phph_nose_1D[part]=new TH1D(part+"_de_phph_nose_1D", Form("%s downstream edep, Generator=%s", part.Data(), gen.Data()), 450, -225, 225);
    }
  }
  
  
  /*Set up branches to be read from input root file*/
  
  Double_t fRate=0;
  remollEvent_t *fEvent=0;
  std::vector<remollGenericDetectorHit_t>  *fHit=0;
  std::vector<remollEventParticle_t> *fPart=0;

  T.SetBranchAddress("ev", &fEvent);
  T.SetBranchAddress("hit", &fHit);
  T.SetBranchAddress("rate", &fRate);
  T.SetBranchAddress("part", &fPart);
  
  
  

  
  
}
