Int_t calculateDose(TString input, TString type, Int_t scale, bool
    fixed_range=true, Int_t detector=0, bool first_three=false){

TFile *f =new TFile(Form("%s",input.Data()));
Float_t weight = 65*344*24*60*60/(40e-9*1.3*1e3*1e6);

TCanvas *c=new TCanvas("c","c", 1200, 900);
/* TCanvas *c=new TCanvas("c","c", 800, 600); */
/* c->SetMargin(0.15,0.15, 0.15, 0.15); */
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
        h_clone[i][0]->RebinX(2);
	
	
	/* TLine *l=new TLine(5937.67,0,5937.67, 300); */
	/* l->Draw(); */
	
	/* TLine *l1=new TLine(6972.42,0,6972.42, 300); */
	/* l1->Draw(); */
	
	/* TLine *l2=new TLine(7964.635,0,7964.35, 300); */
	/* l2->Draw(); */
	

	
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
  /* if ( fixed_range == kTrue ) { */
  /*   h_clone[i][0]->SetContour(nlevels, levels); */
  /* } */
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
  if ( first_three == true ) {
    h_clone[i][0]->GetXaxis()->SetRangeUser(4900, 8000);
    if (type == "de_phz_bottom") {
      h_clone[i][0]->GetYaxis()->SetRangeUser(-15,15);
    } else {
      h_clone[i][0]->GetYaxis()->SetRangeUser(0, 300);
    }
  } else {
    h_clone[i][0]->GetXaxis()->SetRangeUser(5000,12000);
  }
  /* cout << "Coil " << i << " Total Dose = " << h_clone[i][0]->Integral(); */
  /* cout << " MGy\n"; */
  coil_dose.push_back(h_clone[i][0]->Integral());
  /* cout << i << " : " << type.Data() << " : " << h_clone[i][0]->Integral() << "\n"; */
} 

TString str_range = "";
if (fixed_range==true) {
  str_range = "_4level";
}

TString str_three = "";
if (first_three==true) {
  str_three = "_first3";
}

TString str_detector = "";
if (detector != 0) {
  str_detector = Form("_coil%i", detector);
}

TString input_trunc(input(0, input.First(".")));

c->Print(Form("%s-%s%s%s%s.png", type.Data(), input_trunc.Data(), str_detector.Data(), str_range.Data(), str_three.Data()));

map<Int_t, vector<Double_t>> subcoil_dose;

for (Int_t j=2; j<=7; j++) {
  h_clone[1][0]->Add(h_clone[j][0]);
}
h_clone[1][0]->Scale(1.0/7.0);

/* cout << h_clone[1][0]->GetNbinsX() << " : " << h_clone[1][0]->GetNbinsY() << */
/* endl; */
/* cout << h_clone[1][0]->GetXaxis()->GetBinLowEdge(28) << endl; */
/* cout << h_clone[1][0]->GetXaxis()->GetBinLowEdge(29) << endl; */
/* cout << h_clone[1][0]->GetXaxis()->GetBinLowEdge(54) << endl; */
/* cout << h_clone[1][0]->GetXaxis()->GetBinLowEdge(55) << endl; */
/* cout << h_clone[1][0]->GetXaxis()->GetBinLowEdge(79) << endl; */
/* cout << h_clone[1][0]->GetXaxis()->GetBinLowEdge(80) << endl; */
/* cout << h_clone[1][0]->GetXaxis()->GetBinLowEdge(187) << endl; */
/* cout << h_clone[1][0]->GetXaxis()->GetBinLowEdge(180) << endl; */

Double_t integral_value, integral_error;
integral_value = h_clone[1][0]->IntegralAndError(1, 187, 0, 450, integral_error);
cout << "I : " << integral_value << " : " << integral_error << endl;
integral_value = h_clone[1][0]->IntegralAndError(1, 29, 0, 450, integral_error);
cout << "1 : " << integral_value << " : " << integral_error << endl;
integral_value = h_clone[1][0]->IntegralAndError(30, 54, 0, 450, integral_error);
cout << "2 : " << integral_value << " : " << integral_error << endl;
integral_value = h_clone[1][0]->IntegralAndError(55, 80, 0, 450, integral_error);
cout << "3 : " << integral_value << " : " << integral_error << endl;
integral_value = h_clone[1][0]->IntegralAndError(81, 187, 0, 450, integral_error);
cout << "4 : " << integral_value << " : " << integral_error << endl;

/* TCanvas *c1=new TCanvas(); */
/* h_clone[detector][0]->Draw("colz"); */
/* TLine *l=new TLine(5937.67,0,5937.67, 300); */
/* TLine *l1=new TLine(6972.42,0,6972.42, 300); */
/* TLine *l2=new TLine(7964.635,0,7964.35, 300); */
/* l->Draw(); */
/* l1->Draw(); */
/* l2->Draw(); */
/* c1->Print(Form("%s_1.png", type.Data())); */

/* std::cout<<out<<std::endl; */


return 0;

}

/* Int_t batchDose(TString input, Int_t scale, bool fixed_range=true, */
/*     Int_t detector=0, bool first_three=false){ */
Int_t batchDose(TString input, Int_t scale, Int_t detector=0) {
  
  int fixed_range = false;
  calculateDose(input, "de_phz_bottom", scale, fixed_range, detector, true);
  calculateDose(input, "de_rz_left", scale, fixed_range, detector, true);
  calculateDose(input, "de_rz_right", scale, fixed_range, detector, true);

  calculateDose(input, "de_phz_bottom", scale, fixed_range, detector, false);
  calculateDose(input, "de_rz_left", scale, fixed_range, detector, false);
  calculateDose(input, "de_rz_right", scale, fixed_range, detector, false);

  fixed_range = true;
  calculateDose(input, "de_phz_bottom", scale, fixed_range, detector, true);
  calculateDose(input, "de_rz_left", scale, fixed_range, detector, true);
  calculateDose(input, "de_rz_right", scale, fixed_range, detector, true);

  calculateDose(input, "de_phz_bottom", scale, fixed_range, detector, false);
  calculateDose(input, "de_rz_left", scale, fixed_range, detector, false);
  calculateDose(input, "de_rz_right", scale, fixed_range, detector, false);

  return 0;
}
