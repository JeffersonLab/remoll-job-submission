using namespace ROOT;

int analyse(TString source, TString out, TString gen){
  
  /*Add source root file to TChain and get total number of events*/

  TChain T("T");
  T.Add(Form("%s", source.Data())); 
  Int_t nEvents = T.GetEntries();  
  std::cout << "Analyzing "<< nEvents << "events" << std::endl;
  
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

  /*Initiate output root file and histograms*/
  TFile f(Form("%s", out.Data()), "RECREATE");

  std::map<TString, TH2D*> h_xy;
  std::map<TString, TH2D*> h_xy_e;
  std::map<TString, TH2D*> h_xy_R_30_300mm;
  std::map<TString, TH2D*> h_xy_e_R_30_300mm;
  std::map<TString, TH1D*> h_angle_R_30_300mm;
  std::map<TString, TH1D*> h_e_R_30_300mm;
  std::map<TString, TH2D*> h_xy_R_300_1000mm;
  std::map<TString, TH2D*> h_xy_e_R_300_1000mm;
  std::map<TString, TH1D*> h_angle_R_300_1000mm;
  std::map<TString, TH1D*> h_e_R_300_1000mm;
  std::map<TString, TH1D*> h_r;
  std::map<TString, TH1D*> h_r_e;
 
  std::vector<TString> detector_name;
  detector_name.push_back("Col2Ent");
  detector_name.push_back("Col2Exit");
  detector_name.push_back("z=1651mm");
  detector_name.push_back("z=1879.6mm");
  detector_name.push_back("z=2781.3mm");
  detector_name.push_back("z=3098.8mm");
  detector_name.push_back("Col4Ent");
  detector_name.push_back("Col4Exit");
  detector_name.push_back("MD");

  std::vector<TString> particle_name;
  particle_name.push_back("electron");
  particle_name.push_back("positron");
  particle_name.push_back("photon");
  particle_name.push_back("all");

  TString id;

  for(Int_t i=0;i<detector_name.size();i++){
    for(Int_t j=0;j<particle_name.size();j++){
      id = detector_name[i]+"_"+particle_name[j];
      h_xy[id] = new TH2D(id+"_xy",\
                          id+"_xy, Generator="+gen,\
                          2000, -2000, 2000, 2000, -2000, 2000);
      h_xy_e[id] = new TH2D(id+"_xy_e",\
                            id+"_xy_e, Generator="+gen,\
                            2000, -2000, 2000, 2000, -2000, 2000);
      h_xy_R_30_300mm[id] = new TH2D(id+"_xy_R_30_300mm",\
                                     id+"_xy_R_30_300mm, Generator="+gen,\
                                     2000, -2000, 2000, 2000, -2000, 2000);
      h_xy_e_R_30_300mm[id] = new TH2D(id+"_xy_e_R_30_300mm",\
                                       id+"_xy_e_R_30_300mm, Generator="+gen,\
                                       2000, -2000, 2000, 2000, -2000, 2000);
      h_angle_R_30_300mm[id] = new TH1D(id+"_angle_R_30_300mm",\
                                        id+"_angle_R_30_300mm, Generator="+gen,\
                                        1000, 0, 0.1);
      h_e_R_30_300mm[id] = new TH1D(id+"_e_R_30_300mm",\
                                    id+"_e_R_30_300mm, Generator="+gen,\
                                    1100, 0, 11000);
      h_xy_R_300_1000mm[id] = new TH2D(id+"_xy_R_300_1000mm",\
                                       id+"_xy_R_300_1000mm, Generator="+gen,\
                                       2000, -2000, 2000, 2000, -2000, 2000);
      h_xy_e_R_300_1000mm[id] = new TH2D(id+"_xy_e_R_300_1000mm",\
                                         id+"_xy_e_R_300_1000mm, Generator="+gen,\
			                        	         2000, -2000, 2000, 2000, -2000, 2000);
      h_angle_R_300_1000mm[id] = new TH1D(id+"_angle_R_300_1000mm",\
                                          id+"_angle_R_300_1000mm, Generator="+gen,\
                                          1000, 0, 0.1);
      h_e_R_300_1000mm[id] = new TH1D(id+"_e_R_300_1000mm",\
                                      id+"_e_R_300_1000mm, Generator="+gen,\
                                      1100, 0, 11000);
      h_r[id]  = new TH1D(id+"_r",\
                          id+"_r, Generator="+gen,\
			                    500, 0, 1000);
      h_r_e[id] = new TH1D(id+"_r_e",\
                           id+"_r_e, Generator="+gen,\
			                     500, 0, 1000);
    }
  }

  /*Set up branches to be read from input root file*/
  Double_t fRate=0; 
  remollEvent_t *fEvent=0;
  std::vector<remollGenericDetectorHit_t>  *fHit=0;
  std::vector<remollEventParticle_t> *fPart=0;

  T.SetBranchAddress("ev", &fEvent);
  T.SetBranchAddress("hit", &fHit);
  T.SetBranchAddress("rate", &fRate);
  T.SetBranchAddress("part", &fPart);


  /*Loop over events and hits per event and fill histograms*/
  for (size_t j=0;j< nEvents;j++){
    T.GetEntry(j);
    for (size_t k=0;k<fHit->size();k++){
      remollGenericDetectorHit_t hit=fHit->at(k);

      std::map<TString,Bool_t> detector_id{{"Col2Ent",hit.det==38},   \
                                           {"Col2Exit",hit.det==39},  \
                                           {"z=1651mm",hit.det==40},  \
                                           {"z=1879.6mm",hit.det==41},\
                                           {"z=2781.3mm",hit.det==42},\
                                           {"z=3098.8mm",hit.det==43},\
                                           {"Col4Ent",hit.det==44},   \
                                           {"Col4Exit",hit.det==45},  \
                                           {"MD",hit.det==28}};       \
                
      std::map<TString,Bool_t> particle_id{{"electron",hit.pid==11},   \
                                           {"positron",hit.det==-11},  \
                                           {"photon",hit.det==22},  \
                                           {"all",1}};



      if (gen=="beam"){
        fRate=1;
      }
    
      for (Int_t i=0; i<detector_name.size();i++){
        for(Int_t j=0; j<particle_name.size();j++){
          id = detector_name[i]+"_"+particle_name[j];
          if (detector_id[detector_name[i]] && particle_id[particle_name[j]]){
	          h_xy[id]->Fill(hit.x,hit.y, fRate*weight);
	          h_xy_e[id]->Fill(hit.x,hit.y, hit.e*fRate*weight);
            h_r[id]->Fill(hit.r, fRate*weight);
	          h_r_e[id]->Fill(hit.r, hit.e*fRate*weight);
	          if(hit.r>=30 && hit.r<300){
              h_xy_R_30_300mm[id]->Fill(hit.x,hit.y, fRate*weight);
              h_xy_e_R_30_300mm[id]->Fill(hit.x,hit.y, hit.e*fRate*weight);
              h_angle_R_30_300mm[id]->Fill(sqrt(hit.px*hit.px+hit.py*hit.py)/abs(hit.pz), fRate*weight);
              h_e_R_30_300mm[id]->Fill(hit.e, fRate*weight);
	          }
	          if(hit.r>=300 && hit.r<1000){
              h_xy_R_300_1000mm[id]->Fill(hit.x,hit.y, fRate*weight);
              h_xy_e_R_300_1000mm[id]->Fill(hit.x,hit.y, hit.e*fRate*weight);
              h_angle_R_300_1000mm[id]->Fill(sqrt(hit.px*hit.px+hit.py*hit.py)/abs(hit.pz), fRate*weight);
              h_e_R_300_1000mm[id]->Fill(hit.e, fRate*weight);
            }
          }
        } 
      }		      
				
    }
  }
  
  /*Write histograms to output root file*/
  for(Int_t i=0; i<detector_name.size();i++){
    for(Int_t j=0; j<particle_name.size(); j++){
      id = detector_name[i]+"_"+particle_name[j];
      h_xy[id]->Write("", TObject::kOverwrite);
      h_xy_e[id]->Write("", TObject::kOverwrite);
      h_xy_R_30_300mm[id]->Write("", TObject::kOverwrite);
      h_xy_e_R_30_300mm[id]->Write("", TObject::kOverwrite);
      h_angle_R_30_300mm[id]->Write("", TObject::kOverwrite);
      h_e_R_30_300mm[id]->Write("", TObject::kOverwrite);
      h_xy_R_300_1000mm[id]->Write("", TObject::kOverwrite);
      h_xy_e_R_300_1000mm[id]->Write("", TObject::kOverwrite);
      h_angle_R_300_1000mm[id]->Write("", TObject::kOverwrite);
      h_e_R_300_1000mm[id]->Write("", TObject::kOverwrite);
      h_r[id]->Write("", TObject::kOverwrite);
      h_r_e[id]->Write("", TObject::kOverwrite);
    }
  }

return 0;
}
