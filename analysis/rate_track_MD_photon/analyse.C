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

Double_t fRate=0;
remollEvent_t *fEvent=0;
std::vector<remollGenericDetectorHit_t>  *fHit=0;
std::vector<remollEventParticle_t> *fPart=0;

T.SetBranchAddress("ev", &fEvent);
T.SetBranchAddress("hit", &fHit);
T.SetBranchAddress("rate", &fRate);
T.SetBranchAddress("part", &fPart);

for (size_t j=0; j< nEvents; j++){
    T.GetEntry(j);
    std::map<TStrin,std::set<Int_t>> selectedEvent = isValid(fHit);
    if (selectedEvent.empty()) { continue; }

    for (size_t i=0;i<fHit->size();i++){
        remollGenericDetectorHit_t hit=fHit->at(i);
        Int_t sec=findSector(hit.x, hit.y);
        Double_t rate=0;

    }
}


return 0;
}

std::map<TString, std::set<Int_t>> isValid(std::vector<remollGenericDetectorHit_t> *fHit){

 std::map<TString, std::set<Int_t>> found;

 // return true if there is atleast one hit from a photon in ring 5 from an event
 for (size_t i=0;i<fHit->size();i++){
  remollGenericDetectorHit_t hit=fHit->at(i);
  if (!(hit.det==28 && hit.pid==22 && hit.r>900 && hit.r<=1060)) continue;

  if(hit.p<=1){
      found["ypless1MeV"].insert(1);
  }else if (hit.p>1 && hit.p<=10){
      found["yp1to10MeV"].insert(2);
  }else{
      found["ypgreater10MeV"].insert(3);
  }
 }

return found;

}

Int_t findSector(Float_t x, Float_t y){
  Double_t sepdiv=2*TMath::Pi()/7.0;
  Int_t sec=0;
  Double_t phi=atan2(y,x);
  if (phi<0) {phi+=2*TMath::Pi();}

  Double_t secphi = fmod(phi, 2*TMath::Pi()/7);
  if (secphi<TMath::Pi()/28.0){sec=1;}           // closed
  else if (secphi<3*TMath::Pi()/28.0){sec=2; }    // transition
  else if (secphi<5*TMath::Pi()/28.0) {sec=3;}   // open
  else if (secphi<7*TMath::Pi()/28.0) {sec=2;}   // transition
  else {sec=1;}  //closed
  
  return sec;
}
  






