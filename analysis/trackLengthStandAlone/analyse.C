using namespace ROOT;

std::map<TString, std::set<Int_t>> isValid(std::vector<remollGenericDetectorHit_t> *);
Int_t findSector(Float_t, Float_t);

int analyse(TString source, TString out, TString gen, TString detid, TString writeState){

TChain T("T");
T.Add(Form("%s", source.Data())); // Adding source file
Int_t nEvents= T.GetEntries();  // Number of primary events
Double_t weight= 1.0/nEvents; 

TFile* f = new TFile(Form("%s", out.Data()), writeState);
TDirectory* subdir = f->mkdir(detid);

std::map<TString, TH1D*> h_tracklength;
std::map<TString, TH2D*> h_edep;

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
    std::set<Int_t> selectedEvent = isValid(fHit);
    if (selectedEvent.empty()) { continue; }

    for (size_t i=0;i<fHit->size();i++){
        remollGenericDetectorHit_t hit=fHit->at(i);
        
        if (selectedEvent.find(hit.trid) != selectedEvent.end())
              
           
    }
}


for (Int_t j=0; j<sector.size(); j++){
   h_r[part]->SetDirectory(subdir);
   h_ph[part]->SetDirectory(subdir);
}
f->Write();

return 0;
}
