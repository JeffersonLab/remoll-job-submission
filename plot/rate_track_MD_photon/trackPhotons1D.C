Int_t trackPhotons1D(TString loc, TString sector, Int_t nFiles, TString pid, TString suff){

Float_t scale = 1.0/nFiles;
  
std::vector<TString>  detector;
detector.push_back("Col2Ent");
detector.push_back("Col2Exit");
detector.push_back("USCoil1");
detector.push_back("USCoil2");
detector.push_back("USCoil3");
detector.push_back("USCoil4");
detector.push_back("Col4Ent");
detector.push_back("Col4Exit");
detector.push_back("PbWallEnt");
detector.push_back("PbWallExit");
detector.push_back("PhotonBlockerEnt");
detector.push_back("PhotonBlockerExit");
detector.push_back("DSCoil2");
detector.push_back("DSCoil3");
detector.push_back("DSCoil6");
detector.push_back("DSCoil10");
detector.push_back("MD");

THStack hs;
TLegend *l=new TLegend(0.6,0.6,0.9,0.9);
TFile *f =new TFile(Form("%s/beam.root", loc.Data()));
TCanvas *c = new TCanvas("c", "c", 800, 600);
c->SetMargin(0.13,0.13, 0.13, 0.13);
  
for(Int_t k=0; k<detector.size(); k++){
 TString part= Form("pr_%s_%s_%s_%s", detector[k].Data(), sector.Data(), pid.Data(), suff.Data());
 TH1F* h= (TH1F*) ((TDirectory*) f->Get(detector[k]))->Get(part);
 h->SetDirectory(0);
 l->AddEntry(h, detector[k]);
 h->Scale(scale);
 h->SetLineColor(k+1);
 h->SetLineWidth(2);
 hs.Add(h);
}

hs.Draw("HIST");
gPad->SetLogy();
hs.SetTitle("Radial Distribution (1/e/5mm)");
c->Print(loc+"/summary.png");
  
return 0;
}
