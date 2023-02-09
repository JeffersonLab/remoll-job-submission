Int_t calculateDose(TString input, TString type, Int_t scale, \
                    bool fixed_range=true, Int_t detector=0){

  TFile *f =new TFile(Form("%s",input.Data()));
  TCanvas *c=new TCanvas("c","c", 1200, 900);

  /* Float_t weight = 70*344*24*60*60/(1e-9*1.3*1e3*1e6); */
  Float_t weight = 70*344*24*60*60/(40e-9*1.3*1e3*1e6);
  std::map<Int_t,  std::vector<TH2D*>> h_clone;

  TString out="";
  for(Int_t i=1;i<=7; i++){
    for (Int_t j=0;j<4;j++){		
      TH2D* h=(TH2D*) f->Get(Form("pr_%d_E%d_%s", i, j, type.Data()));
      TH2D* h_c=(TH2D*) h->Clone(Form("pr_%d_E%d_%s_clone", i,j, type.Data()));
      h_c->SetDirectory(0);
      h_clone[i].push_back(h_c);
      if(j!=0){
        h_clone[i][0]->Add(h_clone[i][j]);
      }
    }	
    gStyle->SetOptStat(0);
    h_clone[i][0]->Scale(1.0/scale*weight);
    h_clone[i][0]->RebinX(40);
  }

  Int_t nlevels=4;
  Double_t levels[nlevels];
  levels[0]=0.0;
  levels[1]=50.0;
  levels[2]=80.0;
  levels[3]=160.0;

  vector<Double_t> coil_dose;

  cout << input.Data() << endl;
  cout << type.Data() << endl;

  Int_t i_min, i_max;

  if (detector == 0) {
    c->Divide(3,3); 
    i_min = 1;
    i_max = 7;
  } else {
    i_min = detector;
    i_max = detector;
  }

  for(Int_t i=i_min;i<=i_max;i++){
    if (detector == 0) {
      c->cd(i);
    }
    gPad->SetMargin(0.15,0.15, 0.15, 0.15);
    h_clone[i][0]->SetTitle(Form("Septant %i Dose (MGy)", i));
    h_clone[i][0]->SetTitleSize(0.06);
    h_clone[i][0]->Draw("colz");
    if ( fixed_range == true ) {
      h_clone[i][0]->SetContour(nlevels, levels);
      h_clone[i][0]->GetZaxis()->SetRangeUser(0,200);
    }
    h_clone[i][0]->GetZaxis()->SetLabelSize(0.06);
    h_clone[i][0]->GetYaxis()->SetLabelSize(0.06);
    h_clone[i][0]->GetXaxis()->SetLabelSize(0.06);
    h_clone[i][0]->GetYaxis()->SetTitleSize(0.06);
    h_clone[i][0]->GetXaxis()->SetTitleSize(0.06);
    h_clone[i][0]->GetXaxis()->SetTitle("Longitudinal Position (mm)");
    if (type == "de_phz_bottom") {
      h_clone[i][0]->GetYaxis()->SetTitle("Transverse Position (mm)");
    } else {
      h_clone[i][0]->GetYaxis()->SetTitle("Radial Position (mm)");
    }
    h_clone[i][0]->GetXaxis()->SetRangeUser(800,3200);
  }

TString str_range = "";
if (fixed_range==true) {
  str_range = "_4level";
}

TString str_detector = "";
if (detector != 0) {
  str_detector = Form("_coil%i", detector);
}

TString input_trunc(input(0, input.First(".")));

c->Print(Form("%s-%s%s%s.png", type.Data(), input_trunc.Data(), str_detector.Data(), str_range.Data()));

return 0;
}

Int_t batchDose(TString input, Int_t scale, Int_t detector=0) {
  
  int fixed_range = false;
  calculateDose(input, "ue_phz_bottom", scale, fixed_range, detector);
  calculateDose(input, "ue_rz_left", scale, fixed_range, detector);
  calculateDose(input, "ue_rz_right", scale, fixed_range, detector);

  fixed_range = true;
  calculateDose(input, "ue_phz_bottom", scale, fixed_range, detector);
  calculateDose(input, "ue_rz_left", scale, fixed_range, detector);
  calculateDose(input, "ue_rz_right", scale, fixed_range, detector);

  return 0;
}
