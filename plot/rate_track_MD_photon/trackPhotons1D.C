Int_t trackPhotons1D(TString loc, TString sector, Int_t nFiles, TString det, TString suff){

Float_t scale = 1.0/nFiles;

TString pid = "photon";
  
std::vector<TString>  detector;
det.push_back("Col2Ent");
det.push_back("Col2Exit");
det.push_back("USCoil1");
det.push_back("USCoil2");
det.push_back("USCoil3");
det.push_back("USCoil4");
det.push_back("Col4Ent");
det.push_back("Col4Exit");
det.push_back("PbWallEnt");
det.push_back("PbWallExit");
det.push_back("PhotonBlockerEnt");
det.push_back("PhotonBlockerExit");
det.push_back("DSCoil2");
det.push_back("DSCoil3");
det.push_back("DSCoil6");
det.push_back("DSCoil10");
det.push_back("MD");

THStack hs;
TLegend *l=new TLegend(0.6,0.6,0.9,0.9);
TFile *f =new TFile(Form("%s/beam.root", loc.Data()));
  
for(Int_t k=0; k<detector.size(); k++){
 part= Form("pr_%s_%s_%s_%s", detector[k].Data(), sector.Data(), pid.Data(), suff.Data());
 TH1F* h= (TH1F*) ((TDirectory*) f->Get(det))->Get(part);
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
