Int_t trackPhotons1D(TString loc, TString sector, Int_t nFiles, TString pid, TString detector){

Float_t scale = 1.0/nFiles;
  
std::vector<TString>  suff;
suff.push_back("ypless1MeV_forward_r");
suff.push_back("yp1to10MeV_forward_r");
suff.push_back("ypgreater10MeV_forward_r");
suff.push_back("ypless1MeV_backward_r");
suff.push_back("yp1to10MeV_backward_r");
suff.push_back("ypgreater10MeV_backward_r");

THStack hs;
TLegend *l=new TLegend(0.6,0.6,0.9,0.9);
TFile *f =new TFile(Form("%s/beam.root", loc.Data()));
TCanvas *c = new TCanvas("c", "c", 800, 600);
c->SetMargin(0.13,0.13, 0.13, 0.13);
  
for(Int_t k=0; k<suff.size(); k++){
 TString part= Form("pr_%s_%s_%s_%s", detector.Data(), sector.Data(), pid.Data(), suff[k].Data());
 TH1F* h= (TH1F*) ((TDirectory*) f->Get(detector))->Get(part);
 h->SetDirectory(0);
 l->AddEntry(h, suff[k]);
 h->Scale(scale);
 h->SetLineColor(k+1);
 h->SetLineWidth(2);
 hs.Add(h);
}

gPad->SetLogy();
hs.Draw("HISTnostack");

l->Draw();
hs.SetTitle(detector+" Radial Distribution (1/e/5mm)");
c->Print(loc+"/"+detector+".png");
  
return 0;
}
