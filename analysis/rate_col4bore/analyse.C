// Track particles from target region coming through collimator 4 bore and hitting main detector 
#include <iostream>
#include <map>
#include <tuple>
#include <vector>

using std::map;
using std::vector;
using std::string;
using std::tuple;
using std::make_tuple;
using std::cout;
using std::endl;
using TMath::Pi;


void isValid(vector<remollGenericDetectorHit_t> *fHit, vector<int> &MD_trid, vector<int> &R5_trid, vector<int> &col4bore_trid, vector<int> &col4all_trid);

int analyse(TString source, TString out, TString gen, TString detector, TString writeState)
{
  TChain T("T");
  T.Add(Form("%s", source.Data())); // Adding source file
  Int_t nEvents= T.GetEntries();    // Number of primary events
  /*Initiate weight variable that allows us to express rate as count per incident electron.
  For physics generators, first we need to normalize fRate in Hz by beam current (this should be set to whatever the
  sims were run with. Assuming 85 uA here).
  Then we convert Hz/uA to count/incident electron
    Hz/uA = 1/(1e-6 Coulombs) = e/(1e-6*e*Couloumbs) = 1.602e-19Coulombs/(1e-6*e*Coulombs)
                                     = 1.602e-13*1/e
  For beam generator, fRate is set to 1. So, to get count per incident electron, it's ok to just normalize by the number
  of events.
  This results in the unit of power weighted plots being
    MeV/e = W/uA
  
  */
  Double_t weight=1;
  if (gen!="beam"){
    weight= (1.0/85)*(1.602*1e-13); // 1/e. 
  }else{
    weight= 1.0/nEvents; // 1/e
  }
  
  TFile* f = new TFile(Form("%s", out.Data()), writeState);
  TDirectory* subdir = f->mkdir(detector);
  
  vector<TString> ptype;
  ptype.push_back("all");
  ptype.push_back("electron");
  ptype.push_back("positron");
  ptype.push_back("photon");
  ptype.push_back("neutron");
  
  vector<TString> ctype;
  ctype.push_back("nocut");
  ctype.push_back("col4bore_MD");
  ctype.push_back("col4all_MD");
  ctype.push_back("col4bore_R5");
  ctype.push_back("col4all_R5");
  
  vector<TString> plottype;
  plottype.push_back("xy");
  plottype.push_back("r");
  plottype.push_back("e");
  plottype.push_back("th");
  
  map<TString, tuple<Int_t,Double_t,Double_t>> bins{ \
                                 {"MD_nocut_xy", make_tuple(1500, -1500, 1500)}, \
                                 {"MD_col4bore_MD_xy", make_tuple(1500, -1500, 1500)}, \
                                 {"MD_col4all_MD_xy", make_tuple(1500, -1500, 1500)}, \
                                 {"MD_col4bore_R5_xy", make_tuple(1500, -1500, 1500)}, \
                                 {"MD_col4all_R5_xy", make_tuple(1500, -1500, 1500)}, \
                                 {"MD_nocut_r", make_tuple(800, 500, 1300)}, \
                                 {"MD_col4bore_MD_r", make_tuple(800, 500, 1300)}, \
                                 {"MD_col4all_MD_r", make_tuple(800, 500, 1300)}, \
                                 {"MD_col4bore_R5_r", make_tuple(800, 500, 1300)}, \
                                 {"MD_col4all_R5_r", make_tuple(800, 500, 1300)}, \
                                 {"MD_nocut_e", make_tuple(1200, 0, 12000)}, \
                                 {"MD_col4bore_MD_e", make_tuple(1200, 0, 12000)}, \
                                 {"MD_col4all_MD_e", make_tuple(1200, 0, 12000)}, \
                                 {"MD_col4bore_R5_e", make_tuple(1200, 0, 12000)}, \
                                 {"MD_col4all_R5_e", make_tuple(1200, 0, 12000)}, \
                                 {"MD_nocut_th", make_tuple(140, 0, 7)}, \
                                 {"MD_col4bore_MD_th", make_tuple(140, 0, 7)}, \
                                 {"MD_col4all_MD_th", make_tuple(140, 0, 7)}, \
                                 {"MD_col4bore_R5_th", make_tuple(140, 0, 7)}, \
                                 {"MD_col4all_R5_th", make_tuple(140, 0, 7)}, \
                                 {"Col4Ent_nocut_xy", make_tuple(100, -500, 500)}, \
                                 {"Col4Ent_col4bore_MD_xy", make_tuple(100, -100, 100)}, \
                                 {"Col4Ent_col4all_MD_xy", make_tuple(100, -500, 500)}, \
                                 {"Col4Ent_col4bore_R5_xy", make_tuple(100, -100, 100)}, \
                                 {"Col4Ent_col4all_R5_xy", make_tuple(100, -500, 500)}, \
                                 {"Col4Ent_nocut_r", make_tuple(100, 0, 500)}, \
                                 {"Col4Ent_col4bore_MD_r", make_tuple(100, 0, 100)}, \
                                 {"Col4Ent_col4all_MD_r", make_tuple(100, 0, 500)}, \
                                 {"Col4Ent_col4bore_R5_r", make_tuple(100, 0, 100)}, \
                                 {"Col4Ent_col4all_R5_r", make_tuple(100, 0, 500)}, \
                                 {"Col4Ent_nocut_e", make_tuple(1200, 0, 12000)}, \
                                 {"Col4Ent_col4bore_MD_e", make_tuple(1200, 0, 12000)}, \
                                 {"Col4Ent_col4all_MD_e", make_tuple(1200, 0, 12000)}, \
                                 {"Col4Ent_col4bore_R5_e", make_tuple(1200, 0, 12000)}, \
                                 {"Col4Ent_col4all_R5_e", make_tuple(1200, 0, 12000)}, \
                                 {"Col4Ent_nocut_th", make_tuple(140, 0, 7)}, \
                                 {"Col4Ent_col4bore_MD_th", make_tuple(140, 0, 7)}, \
                                 {"Col4Ent_col4all_MD_th", make_tuple(140, 0, 7)}, \
                                 {"Col4Ent_col4bore_R5_th", make_tuple(140, 0, 7)}, \
                                 {"Col4Ent_col4all_R5_th", make_tuple(140, 0, 7)}, \
                                 {"Col4Exit_nocut_xy", make_tuple(100, -500, 500)}, \
                                 {"Col4Exit_col4bore_MD_xy", make_tuple(100, -100, 100)}, \
                                 {"Col4Exit_col4all_MD_xy", make_tuple(100, -500, 500)}, \
                                 {"Col4Exit_col4bore_R5_xy", make_tuple(100, -100, 100)}, \
                                 {"Col4Exit_col4all_R5_xy", make_tuple(100, -500, 500)}, \
                                 {"Col4Exit_nocut_r", make_tuple(100, 0, 500)}, \
                                 {"Col4Exit_col4bore_MD_r", make_tuple(100, 0, 100)}, \
                                 {"Col4Exit_col4all_MD_r", make_tuple(100, 0, 500)}, \
                                 {"Col4Exit_col4bore_R5_r", make_tuple(100, 0, 100)}, \
                                 {"Col4Exit_col4all_R5_r", make_tuple(100, 0, 500)}, \
                                 {"Col4Exit_nocut_e", make_tuple(1200, 0, 12000)}, \
                                 {"Col4Exit_col4bore_MD_e", make_tuple(1200, 0, 12000)}, \
                                 {"Col4Exit_col4all_MD_e", make_tuple(1200, 0, 12000)}, \
                                 {"Col4Exit_col4bore_R5_e", make_tuple(1200, 0, 12000)}, \
                                 {"Col4Exit_col4all_R5_e", make_tuple(1200, 0, 12000)}, \
                                 {"Col4Exit_nocut_th", make_tuple(140, 0, 7)}, \
                                 {"Col4Exit_col4bore_MD_th", make_tuple(140, 0, 7)}, \
                                 {"Col4Exit_col4all_MD_th", make_tuple(140, 0, 7)}, \
                                 {"Col4Exit_col4bore_R5_th", make_tuple(140, 0, 7)}, \
                                 {"Col4Exit_col4all_R5_th", make_tuple(140, 0, 7)}, \
                                 {"Col6AEnt_nocut_xy", make_tuple(200, -1000, 1000)}, \
                                 {"Col6AEnt_col4bore_MD_xy", make_tuple(200, -200, 200)}, \
                                 {"Col6AEnt_col4all_MD_xy", make_tuple(120, -600, 600)}, \
                                 {"Col6AEnt_col4bore_R5_xy", make_tuple(200, -200, 200)}, \
                                 {"Col6AEnt_col4all_R5_xy", make_tuple(120, -600, 600)}, \
                                 {"Col6AEnt_nocut_r", make_tuple(1500, 0, 1500)}, \
                                 {"Col6AEnt_col4bore_MD_r", make_tuple(200, 0, 200)}, \
                                 {"Col6AEnt_col4all_MD_r", make_tuple(600, 0, 600)}, \
                                 {"Col6AEnt_col4bore_R5_r", make_tuple(200, 0, 200)}, \
                                 {"Col6AEnt_col4all_R5_r", make_tuple(600, 0, 600)}, \
                                 {"Col6AEnt_nocut_e", make_tuple(1200, 0, 12000)}, \
                                 {"Col6AEnt_col4bore_MD_e", make_tuple(1200, 0, 12000)}, \
                                 {"Col6AEnt_col4all_MD_e", make_tuple(1200, 0, 12000)}, \
                                 {"Col6AEnt_col4bore_R5_e", make_tuple(1200, 0, 12000)}, \
                                 {"Col6AEnt_col4all_R5_e", make_tuple(1200, 0, 12000)}, \
                                 {"Col6AEnt_nocut_th", make_tuple(140, 0, 7)}, \
                                 {"Col6AEnt_col4bore_MD_th", make_tuple(140, 0, 7)}, \
                                 {"Col6AEnt_col4all_MD_th", make_tuple(140, 0, 7)}, \
                                 {"Col6AEnt_col4bore_R5_th", make_tuple(140, 0, 7)}, \
                                 {"Col6AEnt_col4all_R5_th", make_tuple(140, 0, 7)}, \
                                 {"Col6AMid_nocut_xy", make_tuple(200, -1000, 1000)}, \
                                 {"Col6AMid_col4bore_MD_xy", make_tuple(200, -200, 200)}, \
                                 {"Col6AMid_col4all_MD_xy", make_tuple(120, -600, 600)}, \
                                 {"Col6AMid_col4bore_R5_xy", make_tuple(200, -200, 200)}, \
                                 {"Col6AMid_col4all_R5_xy", make_tuple(120, -600, 600)}, \
                                 {"Col6AMid_nocut_r", make_tuple(1500, 0, 1500)}, \
                                 {"Col6AMid_col4bore_MD_r", make_tuple(200, 0, 200)}, \
                                 {"Col6AMid_col4all_MD_r", make_tuple(600, 0, 600)}, \
                                 {"Col6AMid_col4bore_R5_r", make_tuple(200, 0, 200)}, \
                                 {"Col6AMid_col4all_R5_r", make_tuple(600, 0, 600)}, \
                                 {"Col6AMid_nocut_e", make_tuple(1200, 0, 12000)}, \
                                 {"Col6AMid_col4bore_MD_e", make_tuple(1200, 0, 12000)}, \
                                 {"Col6AMid_col4all_MD_e", make_tuple(1200, 0, 12000)}, \
                                 {"Col6AMid_col4bore_R5_e", make_tuple(1200, 0, 12000)}, \
                                 {"Col6AMid_col4all_R5_e", make_tuple(1200, 0, 12000)}, \
                                 {"Col6AMid_nocut_th", make_tuple(140, 0, 7)}, \
                                 {"Col6AMid_col4bore_MD_th", make_tuple(140, 0, 7)}, \
                                 {"Col6AMid_col4all_MD_th", make_tuple(140, 0, 7)}, \
                                 {"Col6AMid_col4bore_R5_th", make_tuple(140, 0, 7)}, \
                                 {"Col6AMid_col4all_R5_th", make_tuple(140, 0, 7)}, \
                                 {"Col6AExit_nocut_xy", make_tuple(200, -1000, 1000)}, \
                                 {"Col6AExit_col4bore_MD_xy", make_tuple(200, -200, 200)}, \
                                 {"Col6AExit_col4all_MD_xy", make_tuple(120, -600, 600)}, \
                                 {"Col6AExit_col4bore_R5_xy", make_tuple(200, -200, 200)}, \
                                 {"Col6AExit_col4all_R5_xy", make_tuple(120, -600, 600)}, \
                                 {"Col6AExit_nocut_r", make_tuple(1500, 0, 1500)}, \
                                 {"Col6AExit_col4bore_MD_r", make_tuple(200, 0, 200)}, \
                                 {"Col6AExit_col4all_MD_r", make_tuple(600, 0, 600)}, \
                                 {"Col6AExit_col4bore_R5_r", make_tuple(200, 0, 200)}, \
                                 {"Col6AExit_col4all_R5_r", make_tuple(600, 0, 600)}, \
                                 {"Col6AExit_nocut_e", make_tuple(1200, 0, 12000)}, \
                                 {"Col6AExit_col4bore_MD_e", make_tuple(1200, 0, 12000)}, \
                                 {"Col6AExit_col4all_MD_e", make_tuple(1200, 0, 12000)}, \
                                 {"Col6AExit_col4bore_R5_e", make_tuple(1200, 0, 12000)}, \
                                 {"Col6AExit_col4all_R5_e", make_tuple(1200, 0, 12000)}, \
                                 {"Col6AExit_nocut_th", make_tuple(140, 0, 7)}, \
                                 {"Col6AExit_col4bore_MD_th", make_tuple(140, 0, 7)}, \
                                 {"Col6AExit_col4all_MD_th", make_tuple(140, 0, 7)}, \
                                 {"Col6AExit_col4bore_R5_th", make_tuple(140, 0, 7)}, \
                                 {"Col6AExit_col4all_R5_th", make_tuple(140, 0, 7)}, \
                                 {"Col6BEnt_nocut_xy", make_tuple(200, -1000, 1000)}, \
                                 {"Col6BEnt_col4bore_MD_xy", make_tuple(200, -200, 200)}, \
                                 {"Col6BEnt_col4all_MD_xy", make_tuple(120, -600, 600)}, \
                                 {"Col6BEnt_col4bore_R5_xy", make_tuple(200, -200, 200)}, \
                                 {"Col6BEnt_col4all_R5_xy", make_tuple(120, -600, 600)}, \
                                 {"Col6BEnt_nocut_r", make_tuple(1500, 0, 1500)}, \
                                 {"Col6BEnt_col4bore_MD_r", make_tuple(200, 0, 200)}, \
                                 {"Col6BEnt_col4all_MD_r", make_tuple(600, 0, 600)}, \
                                 {"Col6BEnt_col4bore_R5_r", make_tuple(200, 0, 200)}, \
                                 {"Col6BEnt_col4all_R5_r", make_tuple(600, 0, 600)}, \
                                 {"Col6BEnt_nocut_e", make_tuple(1200, 0, 12000)}, \
                                 {"Col6BEnt_col4bore_MD_e", make_tuple(1200, 0, 12000)}, \
                                 {"Col6BEnt_col4all_MD_e", make_tuple(1200, 0, 12000)}, \
                                 {"Col6BEnt_col4bore_R5_e", make_tuple(1200, 0, 12000)}, \
                                 {"Col6BEnt_col4all_R5_e", make_tuple(1200, 0, 12000)}, \
                                 {"Col6BEnt_nocut_th", make_tuple(140, 0, 7)}, \
                                 {"Col6BEnt_col4bore_MD_th", make_tuple(140, 0, 7)}, \
                                 {"Col6BEnt_col4all_MD_th", make_tuple(140, 0, 7)}, \
                                 {"Col6BEnt_col4bore_R5_th", make_tuple(140, 0, 7)}, \
                                 {"Col6BEnt_col4all_R5_th", make_tuple(140, 0, 7)}, \
                                 {"Col6BMid_nocut_xy", make_tuple(200, -1000, 1000)}, \
                                 {"Col6BMid_col4bore_MD_xy", make_tuple(200, -200, 200)}, \
                                 {"Col6BMid_col4all_MD_xy", make_tuple(120, -600, 600)}, \
                                 {"Col6BMid_col4bore_R5_xy", make_tuple(200, -200, 200)}, \
                                 {"Col6BMid_col4all_R5_xy", make_tuple(120, -600, 600)}, \
                                 {"Col6BMid_nocut_r", make_tuple(1500, 0, 1500)}, \
                                 {"Col6BMid_col4bore_MD_r", make_tuple(200, 0, 200)}, \
                                 {"Col6BMid_col4all_MD_r", make_tuple(600, 0, 600)}, \
                                 {"Col6BMid_col4bore_R5_r", make_tuple(200, 0, 200)}, \
                                 {"Col6BMid_col4all_R5_r", make_tuple(600, 0, 600)}, \
                                 {"Col6BMid_nocut_e", make_tuple(1200, 0, 12000)}, \
                                 {"Col6BMid_col4bore_MD_e", make_tuple(1200, 0, 12000)}, \
                                 {"Col6BMid_col4all_MD_e", make_tuple(1200, 0, 12000)}, \
                                 {"Col6BMid_col4bore_R5_e", make_tuple(1200, 0, 12000)}, \
                                 {"Col6BMid_col4all_R5_e", make_tuple(1200, 0, 12000)}, \
                                 {"Col6BMid_nocut_th", make_tuple(140, 0, 7)}, \
                                 {"Col6BMid_col4bore_MD_th", make_tuple(140, 0, 7)}, \
                                 {"Col6BMid_col4all_MD_th", make_tuple(140, 0, 7)}, \
                                 {"Col6BMid_col4bore_R5_th", make_tuple(140, 0, 7)}, \
                                 {"Col6BMid_col4all_R5_th", make_tuple(140, 0, 7)}, \
                                 {"Col6BExit_nocut_xy", make_tuple(200, -1000, 1000)}, \
                                 {"Col6BExit_col4bore_MD_xy", make_tuple(200, -200, 200)}, \
                                 {"Col6BExit_col4all_MD_xy", make_tuple(120, -600, 600)}, \
                                 {"Col6BExit_col4bore_R5_xy", make_tuple(200, -200, 200)}, \
                                 {"Col6BExit_col4all_R5_xy", make_tuple(120, -600, 600)}, \
                                 {"Col6BExit_nocut_r", make_tuple(1500, 0, 1500)}, \
                                 {"Col6BExit_col4bore_MD_r", make_tuple(200, 0, 200)}, \
                                 {"Col6BExit_col4all_MD_r", make_tuple(600, 0, 600)}, \
                                 {"Col6BExit_col4bore_R5_r", make_tuple(200, 0, 200)}, \
                                 {"Col6BExit_col4all_R5_r", make_tuple(600, 0, 600)}, \
                                 {"Col6BExit_nocut_e", make_tuple(1200, 0, 12000)}, \
                                 {"Col6BExit_col4bore_MD_e", make_tuple(1200, 0, 12000)}, \
                                 {"Col6BExit_col4all_MD_e", make_tuple(1200, 0, 12000)}, \
                                 {"Col6BExit_col4bore_R5_e", make_tuple(1200, 0, 12000)}, \
                                 {"Col6BExit_col4all_R5_e", make_tuple(1200, 0, 12000)}, \
                                 {"Col6BExit_nocut_th", make_tuple(140, 0, 7)}, \
                                 {"Col6BExit_col4bore_MD_th", make_tuple(140, 0, 7)}, \
                                 {"Col6BExit_col4all_MD_th", make_tuple(140, 0, 7)}, \
                                 {"Col6BExit_col4bore_R5_th", make_tuple(140, 0, 7)}, \
                                 {"Col6BExit_col4all_R5_th", make_tuple(140, 0, 7)} };
                                  
  map<TString, TH1*> h;
  TString cut;
  TString part;
    
  for(Int_t i=0; i<ptype.size(); i++){
    for(Int_t j=0; j<ctype.size(); j++){
      for(Int_t k=0; k<plottype.size(); k++){ 
        cut= Form("%s_%s_%s", detector.Data(), ctype[j].Data(), plottype[k].Data());
        part = Form("%s_%s", cut.Data(), ptype[i].Data());
        if(plottype[k]=="xy"){
          h[part]=new TH2D(part, Form("%s rate-weighted distro, Generator=%s", part.Data(), gen.Data()), \
                           get<0>(bins[cut]),get<1>(bins[cut]),get<2>(bins[cut]),get<0>(bins[cut]),get<1>(bins[cut]),get<2>(bins[cut]));
        }else{
          h[part]=new TH1D(part, Form("%s rate-weighted distro, Generator=%s", part.Data(), gen.Data()), \
                           get<0>(bins[cut]),get<1>(bins[cut]),get<2>(bins[cut]));
        }
      }
    }  
  } 
    
  Double_t fRate=0;
  remollEvent_t *fEvent=0;
  vector<remollGenericDetectorHit_t> *fHit=0;
  vector<remollEventParticle_t> *fPart=0;

  T.SetBranchAddress("ev", &fEvent);
  T.SetBranchAddress("hit", &fHit);
  T.SetBranchAddress("rate", &fRate);
  T.SetBranchAddress("part", &fPart);
  
  for (size_t j=0; j< nEvents; j++){
    T.GetEntry(j);
    if(gen=="beam") fRate=1;
    
    vector<int> MD_trid;
    vector<int>::iterator MD_it;
    vector<int> R5_trid;
    vector<int>::iterator R5_it;
    vector<int> col4bore_trid;
    vector<int>::iterator col4bore_it;
    vector<int> col4all_trid;
    vector<int>::iterator col4all_it;
    
    isValid(fHit,MD_trid,R5_trid,col4bore_trid,col4all_trid);
        
    for(size_t i=0;i<fHit->size();i++){
      remollGenericDetectorHit_t hit=fHit->at(i);
      Int_t trid = hit.trid;
      MD_it = find(MD_trid.begin(),MD_trid.end(),trid);
      R5_it = find(R5_trid.begin(),R5_trid.end(),trid);
      col4bore_it = find(col4bore_trid.begin(),col4bore_trid.end(),trid);
      col4all_it = find(col4all_trid.begin(),col4all_trid.end(),trid);

      map<TString, Bool_t> detector_cut{ {"MD", hit.det==28}, \
                                         {"Col4Ent", hit.det==44}, \
                                         {"Col4Exit", hit.det==45}, \
                                         {"Col6AEnt", hit.det==66}, \
                                         {"Col6AMid", hit.det==68}, \
                                         {"Col6AExit", hit.det==69}, \
                                         {"Col6BEnt", hit.det==72}, \
                                         {"Col6BMid", hit.det==73}, \
                                         {"Col6BExit", hit.det==74} };
     
      map<TString, Bool_t> ptype_cut{ {"all", 1}, \
                                      {"electron", hit.pid==11}, \
                                      {"positron", hit.pid==-11}, \
                                      {"photon", hit.pid==22}, \
                                      {"neutron", hit.pid==2112} };
      
      map<TString, Bool_t> ctype_cut{ {"nocut", 1}, \
                                      {"col4bore_MD", MD_it!=MD_trid.end() && col4bore_it!=col4bore_trid.end() && trid==*MD_it && trid==*col4bore_it}, \
                                      {"col4all_MD", MD_it!=MD_trid.end() && col4all_it!=col4all_trid.end() && trid==*MD_it && trid==*col4all_it}, \
                                      {"col4bore_R5", R5_it!=R5_trid.end() && col4bore_it!=col4bore_trid.end() && trid==*R5_it && trid==*col4bore_it}, \
                                      {"col4all_R5", R5_it!=R5_trid.end() && col4all_it!=col4all_trid.end() && trid==*R5_it && trid==*col4all_it} };                                 
      
      
      if(MD_it==MD_trid.end() || col4all_it==col4all_trid.end()) {continue;}
      
      for(Int_t i=0; i<ptype.size(); i++){
        for(Int_t j=0; j<ctype.size(); j++){
          for(Int_t k=0; k<plottype.size(); k++){ 
            cut= Form("%s_%s_%s", detector.Data(), ctype[j].Data(), plottype[k].Data());
            part = Form("%s_%s", cut.Data(), ptype[i].Data());
            if(plottype[k]=="xy" && detector_cut[detector] && ptype_cut[ptype[i]] && ctype_cut[ctype[j]] && hit.vz<=-3875){
              ((TH2D*) h[part])->Fill(hit.x, hit.y, fRate*weight);
            }else if(plottype[k]=="r"  && detector_cut[detector] && ptype_cut[ptype[i]] && ctype_cut[ctype[j]] && hit.vz<=-3875){
              h[part]->Fill(hit.r, fRate*weight);
            }else if(plottype[k]=="e"  && detector_cut[detector] && ptype_cut[ptype[i]] && ctype_cut[ctype[j]] && hit.vz<=-3875){
              h[part]->Fill(hit.e, fRate*weight);  
            }else if(plottype[k]=="th"  && detector_cut[detector] && ptype_cut[ptype[i]] && ctype_cut[ctype[j]] && hit.vz<=-3875 && hit.pz>=0){
              h[part]->Fill((180./Pi())*atan2(sqrt(hit.px*hit.px+hit.py*hit.py),hit.pz), fRate*weight);  
            }
          }
        }  
      }     
 
    }
  }
                                                                                                     
  for(Int_t i=0; i<ptype.size(); i++){
    for(Int_t j=0; j<ctype.size(); j++){
      for(Int_t k=0; k<plottype.size(); k++){ 
        cut= Form("%s_%s_%s", detector.Data(), ctype[j].Data(), plottype[k].Data());
        part = Form("%s_%s", cut.Data(), ptype[i].Data());
        h[part]->SetDirectory(subdir);
      }
    }  
  }
  f->Write();                                                                                                   
  return 0; 
}

void isValid(vector<remollGenericDetectorHit_t> *fHit, vector<int> &MD_trid, vector<int> &R5_trid, vector<int> &col4bore_trid, vector<int> &col4all_trid)
{
  for(size_t i=0;i<fHit->size();i++){
    remollGenericDetectorHit_t hit=fHit->at(i);
    int det = hit.det;
    int pid = hit.pid;
    if(det==28 && hit.r>=660 && hit.r<=1170 && hit.vz<=-3875){
        MD_trid.push_back(hit.trid);
    }
    if(det==28 && hit.r>=930 && hit.r<=1070 && hit.vz<=-3875){
        R5_trid.push_back(hit.trid);
    }
    if(det==44 && hit.r<=40 && hit.vz<=-3875){
        col4bore_trid.push_back(hit.trid);
    }
    if(det==44 && hit.r<=210 && hit.vz<=-3875){
        col4all_trid.push_back(hit.trid);
    }
  }
}
