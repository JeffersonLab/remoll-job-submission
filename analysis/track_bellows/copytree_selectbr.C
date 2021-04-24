using namespace ROOT;

Int_t isValid(std::vector<remollGenericDetectorHit_t> *);

int copytree_selectbr(TString source, TString out){

TChain T("T");
T.Add(Form("%s", source.Data())); // Adding source file
Int_t nEvents= T.GetEntries();  // Number of primary events
std::cout << "Analyzing "<< nEvents << "events" << std::endl;

remollEvent_t *oevent=0;
std::vector<remollGenericDetectorHit_t>  *ohit=0;

TFile f(Form("%s", out.Data()), "RECREATE");
auto *tree     = new TTree("T", "outputtree");
auto *br_event = tree->Branch("ev", &oevent);
auto *br_hit   = tree->Branch("hit", &ohit);

remollEvent_t *fEvent=0;
std::vector<remollGenericDetectorHit_t>  *fHit=0;

T.SetBranchAddress("ev", &fEvent);
T.SetBranchAddress("hit", &fHit);


for (size_t j=0;j< nEvents;j++){
	T.GetEntry(j);
        
        if(isValid(fHit)){   // identify events for which track id 1 hits bellows
              oevent = fEvent;
                   

              for(size_t i=0;i<fHit->size();i++){
                   remollGenericDetectorHit_t hit=fHit->at(i);
                   if(hit.trid==1){  
                         ohit->push_back(hit);
                   }
              }

              tree->Fill();              
              ohit->clear();
             
        }
}

tree->Print();
tree->Write();


return 0;
}



Int_t isValid(std::vector<remollGenericDetectorHit_t> *fHit){

Int_t found=0;
// return true if there is atleast one hit on one of the bellows with track id 1 for an individual event
for (size_t i=0;i<fHit->size();i++){
  remollGenericDetectorHit_t hit=fHit->at(i);
  if ((hit.det>=70 && hit.det <=76) && hit.trid==1){
       found=1;      
       break;
  }
}

return found;

}

