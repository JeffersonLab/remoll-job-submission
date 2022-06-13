Int_t calculateDoseNose(TString input, Int_t subcoil, Int_t scale, bool
    fixed_range=true, Int_t detector=1){

TFile *f =new TFile(Form("%s",input.Data()));
/* Float_t weight = 65*344*24*60*60/(40e-9*1.3*1e3*1e6); */
Float_t weight = 65*344*24*60*60/(1e-9*1.3*1e3*1e6);


TCanvas *c=new TCanvas("c","c", 1200, 900);
/* TCanvas *c=new TCanvas("c","c", 800, 600); */
/* c->SetMargin(0.15,0.15, 0.15, 0.15); */
/* std::map<Int_t,  std::vector<TH2D*>> h_clone; */
std::map<Int_t,  TH2D*> h_clone;

TString type="de_phph_nose";

TString out="";
for(Int_t i=1;i<=7; i++){
	
        TH2D* h=(TH2D*) f->Get(Form("pr_%d_E%d_%s", i, subcoil, type.Data()));
	TH2D* h_c=(TH2D*) h->Clone(Form("pr_%d_E%d_%s_clone", i,subcoil, type.Data()));
			
	h_c->SetDirectory(0);
	h_clone[i] = h_c;

	gStyle->SetOptStat(0);
	h_clone[i]->Scale(1.0/scale*weight);
	
}

Int_t nlevels=4;
Double_t levels[nlevels];
levels[0]=0.0;
levels[1]=50.0;
levels[2]=80.0;
levels[3]=160.0;

vector<Double_t> coil_dose;

cout << input.Data() << endl;
cout << subcoil << endl;

Float_t x_bound, y_bound;

switch(subcoil) {
  case 0:
    x_bound = 105.7;
    y_bound = 11.35;
    break;
  case 1:
    x_bound = 138.128;
    y_bound = 12.95;
    break;
  case 2:
    x_bound = 173.212;
    y_bound = 13.45;
    break;
  case 3:
    x_bound = 207.948;
    y_bound = 24.7;
    break;
}

TBox* b_coil = new TBox(-x_bound, -y_bound, x_bound, y_bound);
b_coil->SetFillStyle(0);
b_coil->SetLineWidth(2);

c->Divide(3,3); 
for(Int_t i=1;i<=7;i++){
  c->cd(i);
  gPad->SetMargin(0.15,0.15, 0.15, 0.15);
  h_clone[i]->SetTitle("Dose(MGy)");
  h_clone[i]->SetTitleSize(0.06);
  /* if ( fixed_range == kTrue ) { */
  /*   h_clone[i]->SetContour(nlevels, levels); */
  /* } */
  h_clone[i]->Draw("colz");
  if ( fixed_range == true ) {
    h_clone[i]->SetContour(nlevels, levels);
    h_clone[i]->GetZaxis()->SetRangeUser(0,200);
  }
  h_clone[i]->GetZaxis()->SetLabelSize(0.06);
  h_clone[i]->GetYaxis()->SetLabelSize(0.06);
  h_clone[i]->GetXaxis()->SetLabelSize(0.06);
  h_clone[i]->GetXaxis()->SetRangeUser(5000,12000);
  b_coil->Draw();
  /* cout << "Coil " << i << " Total Dose = " << h_clone[i]->Integral(); */
  /* cout << " MGy\n"; */
  coil_dose.push_back(h_clone[i]->Integral());
  /* cout << i << " : " << type.Data() << " : " << h_clone[i]->Integral() << "\n"; */
} 
/* Double_t coil_mean = TMath::Mean(coil_dose.begin(), coil_dose.end()); */
/* Double_t coil_sd = TMath::StdDev(coil_dose.begin(), coil_dose.end()); */
/* Double_t coil_err = coil_sd/TMath::Sqrt(7); */
/* cout << "Total : " << coil_mean << " : " << coil_err << endl; */

TString str_range = "";
if (fixed_range==true) {
  str_range = "_4level";
}

TString input_trunc(input(0, input.First(".")));

c->Print(Form("subcoil%i-%s%s.png", subcoil, input_trunc.Data(), str_range.Data()));

map<Int_t, vector<Double_t>> subcoil_dose;

for (Int_t j=2; j<=7; j++) {
  h_clone[1]->Add(h_clone[j]);
}
h_clone[1]->Scale(1.0/7.0);

/* cout << h_clone[1]->GetNbinsX() << " : " << h_clone[1]->GetNbinsY() << */
/* endl; */
/* cout << h_clone[1]->GetXaxis()->GetBinLowEdge(28) << endl; */
/* cout << h_clone[1]->GetXaxis()->GetBinLowEdge(29) << endl; */
/* cout << h_clone[1]->GetXaxis()->GetBinLowEdge(54) << endl; */
/* cout << h_clone[1]->GetXaxis()->GetBinLowEdge(55) << endl; */
/* cout << h_clone[1]->GetXaxis()->GetBinLowEdge(79) << endl; */
/* cout << h_clone[1]->GetXaxis()->GetBinLowEdge(80) << endl; */
/* cout << h_clone[1]->GetXaxis()->GetBinLowEdge(187) << endl; */
/* cout << h_clone[1]->GetXaxis()->GetBinLowEdge(180) << endl; */

Double_t integral_value, integral_error;
integral_value = h_clone[1]->IntegralAndError(0, 850, 0, 50, integral_error);
cout << "T : " << integral_value << " +/- " << integral_error << endl;
/* integral_value = h_clone[1]->IntegralAndError(1, 29, 0, 450, integral_error); */
/* cout << "1 : " << integral_value << " +/- " << integral_error << endl; */
/* integral_value = h_clone[1]->IntegralAndError(30, 54, 0, 450, integral_error); */
/* cout << "2 : " << integral_value << " +/- " << integral_error << endl; */
/* integral_value = h_clone[1]->IntegralAndError(55, 80, 0, 450, integral_error); */
/* cout << "3 : " << integral_value << " +/- " << integral_error << endl; */
/* integral_value = h_clone[1]->IntegralAndError(81, 187, 0, 450, integral_error); */
/* cout << "4 : " << integral_value << " +/- " << integral_error << endl; */

/* TCanvas *c1=new TCanvas(); */
/* h_clone[detector]->Draw("colz"); */
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

Int_t batchDoseNose(TString input, Int_t scale, bool fixed_range=false){
  
  calculateDoseNose(input, 0, scale, fixed_range);
  calculateDoseNose(input, 1, scale, fixed_range);
  calculateDoseNose(input, 2, scale, fixed_range);
  calculateDoseNose(input, 3, scale, fixed_range);

  return 0;
}
