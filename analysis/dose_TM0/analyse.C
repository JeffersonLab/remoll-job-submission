using namespace ROOT;

Double_t in_coil_nose_epoxy(Double_t r, Double_t z, Double_t r_outer, Double_t r_0) {

  const Double_t r_inner = r_outer - 1.0; // Inner radius of epoxy
  const Double_t z_0 = 0.0;
  Double_t delta_r = r - r_0;
  Double_t delta_z = z - z_0;
  Double_t phi = TMath::ATan(delta_r/delta_z);
  Double_t radius = TMath::Sqrt(delta_r*delta_r + delta_z*delta_z);

  if (r_outer > radius && radius > r_inner) {
    Double_t circ_pos;
    circ_pos = r_outer * phi;
    return circ_pos;
  }

  return -100000;
}

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

  std::map<TString, TH2D*> h_ue_rz_left;
  std::map<TString, TH2D*> h_ue_rz_right;
  std::map<TString, TH2D*> h_ue_phz_bottom;
  std::map<TString, TH2D*> h_ue_phph_nose;
  std::map<TString, TH1D*> h_ue_rz_left_1D;
  std::map<TString, TH1D*> h_ue_rz_right_1D;
  std::map<TString, TH1D*> h_ue_phz_bottom_1D;
  std::map<TString, TH1D*> h_ue_phph_nose_1D;

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
      h_ue_rz_left[part]=new TH2D(part+"_ue_rz_left", Form("%s upstream edep, Generator=%s", part.Data(), gen.Data()), 2400, 800, 3200, 60, 0, 60);
      h_ue_rz_right[part]=new TH2D(part+"_ue_rz_right", Form("%s upstream epoxy edep, Generator=%s", part.Data(), gen.Data()), 2400, 800, 3200, 60, 0, 60);
      h_ue_phz_bottom[part]=new TH2D(part+"_ue_phz_bottom", Form("%s upstream edep, Generator=%s", part.Data(), gen.Data()), 2400, 800, 3200, 50 , -25.0, 25.0);
      h_ue_phph_nose[part]=new TH2D(part+"_ue_phph_nose", Form("%s upstream edep, Generator=%s", part.Data(), gen.Data()), 450, -225, 225, 50, -25.0, 25.0);
      h_ue_rz_left_1D[part]=new TH1D(part+"_ue_rz_left_1D", Form("%s upstream epoxy edep, Generator=%s", part.Data(), gen.Data()), 2400, 800, 3200);
      h_ue_rz_right_1D[part]=new TH1D(part+"_ue_rz_right_1D", Form("%s upstream edep, Generator=%s", part.Data(), gen.Data()), 2400, 800, 3200);
      h_ue_phz_bottom_1D[part]=new TH1D(part+"_ue_phz_bottom_1D", Form("%s upstream epoxy edep, Generator=%s", part.Data(), gen.Data()), 2400, 800, 3200);
      h_ue_phph_nose_1D[part]=new TH1D(part+"_ue_phph_nose_1D", Form("%s upstream edep, Generator=%s", part.Data(), gen.Data()), 450, -225, 225);
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
  
  /*Loop over events and hits per event and fill histograms*/
  for (size_t j=0;j< nEvents;j++){
    T.GetEntry(j);
    for (size_t i=0;i<fHit->size();i++){
      if(gen=="beam"){
        fRate=1;
      }
      remollGenericDetectorHit_t hit=fHit->at(i);

      TVector2 xy(hit.x, hit.y);
      TVector2 XY(hit.x, hit.y);
      
      Bool_t ene_cut;
      std::map<TString, Bool_t> hit_pid;
      
      for (Int_t k=0;k<energy_bins;k++){
        if(k==0){ ene_cut= hit.p<1; }
        else if (k==1) { ene_cut=hit.p>=1 && hit.p<10;}
        else if (k==2) { ene_cut=hit.p>=10 && hit.p<100;}
        else{ ene_cut = hit.p>= 100;}

        for (Int_t i=0; i<n_septant; i++){
          part= Form("pr_%d_E%d",i+1,k);
          XY= xy.Rotate(-i*size_septant);
          hit_pid[part]= ene_cut ;
          
          if(hit.det==(4008+i)){
            if(XY.Y()>4.5){
              h_ue_rz_left[part]->Fill(hit.z, XY.X(), hit.edep*(fRate)*weight);
              if(XY.X()>=41 && XY.X()<=61){
                h_ue_rz_left_1D[part]->Fill(hit.z, hit.edep*(fRate)*weight);
              }
            }
            if(XY.Y()<-4.5){
              h_ue_rz_right[part]->Fill(hit.z, XY.X(), hit.edep*(fRate)*weight);
              if(XY.X()>=41 && XY.X()<=61){
                h_ue_rz_right_1D[part]->Fill(hit.z, hit.edep*(fRate)*weight);
              }
            }
            if(XY.X()<33 && hit.yl<0.0 && hit.yl> -1777.957){
              h_ue_phz_bottom[part]->Fill(hit.z, XY.Y(), hit.edep*(fRate)*weight);
              h_ue_phz_bottom_1D[part]->Fill(hit.z, hit.edep*(fRate)*weight);
            }
            if (hit.yl>0.0) {
              Double_t circ_pos = in_coil_nose_epoxy(XY.X(), hit.yl, 109.922, 141.922);
              if (circ_pos > -100000) {
                h_ue_phph_nose[part]->Fill(circ_pos, XY.Y(), hit.edep*(fRate)*weight);
                h_ue_phph_nose_1D[part]->Fill(circ_pos, hit.edep*(fRate)*weight);
              }
            }
          }
        }
      }
    }
  }


  for(Int_t k=0; k<energy_bins;k++){
    for (Int_t i=0; i<n_septant; i++){
      part= Form("pr_%d_E%d", i+1,k);
      h_ue_rz_left[part]->Write("", TObject::kOverwrite);
      h_ue_rz_right[part]->Write("", TObject::kOverwrite);
      h_ue_phz_bottom[part]->Write("", TObject::kOverwrite);
      h_ue_phph_nose[part]->Write("", TObject::kOverwrite);
      h_ue_rz_left_1D[part]->Write("", TObject::kOverwrite);
      h_ue_rz_right_1D[part]->Write("", TObject::kOverwrite);
      h_ue_phz_bottom_1D[part]->Write("", TObject::kOverwrite);
      h_ue_phph_nose_1D[part]->Write("", TObject::kOverwrite);
    }
  }

return 0;
}
