using namespace ROOT;

Double_t in_coil_nose_epoxy(Double_t r, Double_t z, Double_t r_inner, Double_t r_outer) {

  Double_t phi = TMath::ATan(r/z);
  Double_t radius = TMath::Sqrt(r*r + z*z);

  if (r_outer >= radius && radius > r_inner) {
    Double_t circ_pos;
    circ_pos = r_outer * phi;
    return circ_pos;
  }

  return -100000;
}

int analyse(TString source, TString out, TString gen){
  
  /*Add source root file to TChain and get total number of events*/
  
  Double_t z_start = 1108.985;                                       // Location where straight part starts with respect to hall center
  Double_t z_end   = 2886.942;                                       // Location where straight part ends with respect to hall center
  Double_t r_bottom_start = 33;                                      // Lower radial limit of lower straight section of coil excluding insulation
  Double_t r_top_start = 250.844;                                    // Upper radial limit of upper straight section of coil excluding insulation
  Double_t front_arc_radius =  (r_top_start-r_bottom_start)/2.0;
  Double_t front_arc_center =  r_bottom_start + front_arc_radius;
  Double_t insulation_thickness  = 1.0;                              // Thickness of insulation
  Double_t azimuthal_width    = 9.0;                              // Azimuthal width of the coil in mm
  
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
  Int_t energy_bins=4;

  TString part;
  
  for(Int_t k=0;k<energy_bins;k++){
    for(Int_t i=0; i<n_septant; i++){
      part= Form("pr_%d_E%d",i+1,  k);
      std::cout<< part<< std::endl;
      h_ue_rz_left[part]=new TH2D(part+"_ue_rz_left", Form("%s upstream edep, Generator=%s", part.Data(), gen.Data()), 2400, 800, 3200, 300, 0, 300);
      h_ue_rz_right[part]=new TH2D(part+"_ue_rz_right", Form("%s upstream epoxy edep, Generator=%s", part.Data(), gen.Data()), 2400, 800, 3200, 300, 0, 300);
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

      Double_t global_ph = hit.zl;
      Double_t global_z  = z_start-hit.yl;
      Double_t global_r  = front_arc_center+hit.xl;
        
      Bool_t ene_cut;
      
      for (Int_t k=0;k<energy_bins;k++){
        if(k==0){ ene_cut= hit.p<1; }
        else if (k==1) { ene_cut=hit.p>=1 && hit.p<10;}
        else if (k==2) { ene_cut=hit.p>=10 && hit.p<100;}
        else{ ene_cut = hit.p>= 100;}

        for (Int_t i=0; i<n_septant; i++){
          part= Form("pr_%d_E%d",i+1,k);
          
          
          if(ene_cut){
            if(hit.det==(4008+i)){
              if(global_ph>azimuthal_width/2.0){
                h_ue_rz_left[part]->Fill(global_z, global_r, hit.edep*(fRate)*weight);
                if(global_r>=32 && global_r<=52){
                  h_ue_rz_left_1D[part]->Fill(global_z, hit.edep*(fRate)*weight);
                }
              }
              if(global_ph<-azimuthal_width/2.0){
                h_ue_rz_right[part]->Fill(global_z, global_r, hit.edep*(fRate)*weight);
                if(global_r>=32 && global_r<=52){
                  h_ue_rz_right_1D[part]->Fill(global_z, hit.edep*(fRate)*weight);
                }
              }
              if(global_r<r_bottom_start && global_z >= z_start && global_z <= z_end){
                h_ue_phz_bottom[part]->Fill(global_z, global_ph, hit.edep*(fRate)*weight);
                h_ue_phz_bottom_1D[part]->Fill(global_z, hit.edep*(fRate)*weight);
              }
              if (global_z < z_start) {
                Double_t circ_pos = in_coil_nose_epoxy(hit.xl, hit.yl, front_arc_radius, front_arc_radius+insulation_thickness);
                if (circ_pos > -100000) {
                  h_ue_phph_nose[part]->Fill(circ_pos, global_ph, hit.edep*(fRate)*weight);
                  h_ue_phph_nose_1D[part]->Fill(circ_pos, hit.edep*(fRate)*weight);
                }
              }
            }
          }
        }
      }
    }
  }

  /*Write histograms to file*/
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
