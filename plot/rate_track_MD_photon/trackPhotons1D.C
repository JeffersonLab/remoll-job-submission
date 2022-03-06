Int_t trackPhotons1D(TString loc, TString sector, Int_t nFiles, TString det, TString suff){

Float_t scale = 1.0/nFiles;

TString pid = "photon";
  
std::vector<TString>  det;
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
  


  
return 0;
}
