using namespace ROOT;

std::map<TString, std::set<Int_t>> isValid(std::vector<remollGenericDetectorHit_t> *);
Int_t findSector(Float_t, Float_t);

int analyse(TString source, TString out, TString gen, TString detid, TString writeState){

TChain T("T");
T.Add(Form("%s", source.Data())); // Adding source file
Int_t nEvents= T.GetEntries();  // Number of primary events
Double_t weight= 1.0/85; // Divide by current. So, the Y-axis gets units of Hz/uA.

TFile* f = new TFile(Form("%s", out.Data()), writeState);
TDirectory* subdir = f->mkdir(detid);

std::map<TString, TH1D*> h_vz;
std::map<TString, TH2D*> h_xy;
std::map<TString, TH1D*> h_r;
std::map<TString, TH1D*> h_ph;

std::vector<TString> sector;
sector.push_back("all");
sector.push_back("closed");
sector.push_back("trans");
sector.push_back("open");

std::vector<TString> photon_ene;
photon_ene.push_back("ypless1MeV");
photon_ene.push_back("yp1to10MeV");
photon_ene.push_back("ypgreater10MeV");

TString detector = detid;
TString pid = "photon";


TString part;

for(Int_t j=0; j<sector.size(); j++){
 for(Int_t k=0; k<photon_ene.size();k++){
   part= Form("pr_%s_%s_%s_%s", detector.Data(), sector[j].Data(), pid.Data(), photon_ene[k].Data());
   h_vz[part]=new TH1D(part+"_vz", Form("%s_vz rate-weighted vertex, Generator=%s", part.Data(), gen.Data()), 360, -6000, 30000);
   h_xy[part]=new TH2D(part+"_xy", Form("%s_xy rate-weighted distribution MD, Generator=%s", part.Data(), gen.Data() ), 520, -1300, 1300, 520, -1300, 1300);
   h_r[part]=new TH1D(part+"_r", Form("%s_r rate-weighted distribution MD, Generator=%s", part.Data(), gen.Data()), 400, 0, 2000);
   h_ph[part]=new TH1D(part+"_ph", Form("%s_ph rate-weighted distribution MD, Generator=%s", part.Data(), gen.Data()), 400,-4,4);
  }
}

 





