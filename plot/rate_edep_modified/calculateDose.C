Int_t calculateDose(TString input, TString type){

	
//TFile *f = new TFile("~/projects/rrg-jmammei/rahmans/spectrometerStudy/analysis/develop-tracking/rate_edep/beam/segmentedJLAB_v2.root");
		
//commented out by Naz	
//TFile *f =new TFile("~/projects/rrg-jmammei/rahmans/magnetHotspot/analysis/develop-magnetHotspotV1/rate_edep/"+src+"/beam.root");	
	
//added by Naz
TFile *f =new TFile(Form("%s",input.Data()));
Float_t weight = 65*344*24*60*60/(40e-9*1.3*1e3*1e6);

TCanvas *c=new TCanvas("c","c", 800, 600);

//c->SetLogz();
c->SetMargin(0.15,0.15, 0.15, 0.15);
std::map<Int_t,  std::vector<TH2D*>> h_clone;

TString out="";
for(Int_t i=1;i<=7; i++){
	
	for (Int_t j=0;j<4;j++){
						
        TH2D* h=(TH2D*) f->Get(Form("pr_%d_E%d_%s", i, j, type.Data()));
	TH2D* h_c=(TH2D*) h->Clone(Form("pr_%d_E%d_%s_clone", i,j, type.Data()));
			
	h_c->SetDirectory(0);
	h_clone[i].push_back(h_c);
	if(j!=0){
		
		
		h_clone[i][0]->Add(h_clone[i][j]);
	}
	}

	//cchanging 1000 to 100 by Naz
	gStyle->SetOptStat(0);
	h_clone[i][0]->Scale(1.0/1000*weight);
        h_clone[i][0]->RebinX(2);
//	h_clone[i][0]->Draw("COL4Z");
	
	
	TLine *l=new TLine(5937.67,0,5937.67, 300);
	l->Draw();
	
	TLine *l1=new TLine(6972.42,0,6972.42, 300);
	l1->Draw();
	
	TLine *l2=new TLine(7964.635,0,7964.35, 300);
	l2->Draw();
	
	
	/*
	TSpectrum2 *s=new TSpectrum2(100, 2);
	
	
	
	Double_t sigma=1;
	Double_t threshhold=1e-8;
	Int_t nfound = s->Search(h_clone[i][0],sigma,"col", threshhold);
	
   
	
	Double_t *xpeaks = s->GetPositionX();
	Double_t *ypeaks = s->GetPositionY();
	
	
	
	s->Print();
	
	
	out+=Form("Septant %d\n", i);
	
	
	
	for (Int_t p=0;p<nfound;p++){ 
	
  	 out+=Form("%3.3f %3.3f \n", xpeaks[p], ypeaks[p]);
	
	}
	
	*/

	
	out+=Form("Maximum= %g\n", h_clone[i][0]->GetMaximum());
	out+= Form("Maxmimum Bin= %d, Content= %g, Error= %g\n", h_clone[i][0]->GetMaximumBin(), h_clone[i][0]->GetBinContent( h_clone[i][0]->GetMaximumBin()), h_clone[i][0]->GetBinError( h_clone[i][0]->GetMaximumBin()));
		
//        c->Print(Form("coil%d.png",i));
 }	
/*
c->Divide(2,2);
c->cd(1);
gPad->SetMargin(0.15,0.15, 0.15, 0.15);
//h_clone[1][0]->RebinX(4);
h_clone[1][0]->SetTitle("Energy Deposited In Tungsten Plated Coil (W/uA/(5x20x2)mm^3)");
h_clone[1][0]->SetTitleSize(0.06);
h_clone[1][0]->Draw("colz");
h_clone[1][0]->GetZaxis()->SetLabelSize(0.06);
h_clone[1][0]->GetYaxis()->SetLabelSize(0.06);
h_clone[1][0]->GetXaxis()->SetLabelSize(0.06);
std::cout<< "Integral of tungsten plated coil " << h_clone[1][0]->Integral() <<std::endl;
*/
//Naz:
c->Divide(3,3); 
 for(Int_t i=1;i<=7;i++){
c->cd(i);
gPad->SetMargin(0.15,0.15, 0.15, 0.15);

h_clone[i][0]->SetTitle("Dose(MGy)");
h_clone[i][0]->SetTitleSize(0.06);
h_clone[i][0]->Draw("colz");
h_clone[i][0]->GetZaxis()->SetLabelSize(0.06);
h_clone[i][0]->GetYaxis()->SetLabelSize(0.06);
h_clone[i][0]->GetXaxis()->SetLabelSize(0.06);
h_clone[i][0]->GetXaxis()->SetRangeUser(5000,8000);
//h_clone[i][0]->SetMaximum(10e-6);
std::cout<< "Integral of tungsten plated coil " << h_clone[i][0]->Integral() <<std::endl;
 } 
/*

c->cd(2);
gPad->SetMargin(0.15,0.15, 0.15, 0.15);
h_clone[2][0]->RebinX(4);
h_clone[2][0]->SetTitle("Energy Deposited In Unshielded Coil (W/uA/(5x20x2)mm^3)");
h_clone[2][0]->SetTitleSize(0.06);
h_clone[2][0]->Draw("colz");
h_clone[2][0]->GetZaxis()->SetLabelSize(0.06);
h_clone[2][0]->GetYaxis()->SetLabelSize(0.06);
h_clone[2][0]->GetXaxis()->SetLabelSize(0.06);
std::cout<< "Integral of Unshielded coil:" << h_clone[2][0]->Integral()<< std::endl;

c->cd(3);
gPad->SetMargin(0.15,0.15, 0.15, 0.15);
TH2F* hc1=(TH2F*) h_clone[1][0]->Clone("hc1");
TH2F* hc2= (TH2F*) h_clone[2][0]->Clone("hc2");
hc2->SetTitle("Difference");
hc2->SetTitleSize(0.06);
hc1->Scale(-1.0);
hc2->Add(hc1);
hc2->Draw("colz");
hc2->GetZaxis()->SetLabelSize(0.06);
hc2->GetYaxis()->SetLabelSize(0.06);
hc2->GetXaxis()->SetLabelSize(0.06);


*/
c->Print(Form("%s.png", type.Data()));

TCanvas *c1=new TCanvas();
h_clone[1][0]->Draw("colz");
TLine *l=new TLine(5937.67,0,5937.67, 300);
TLine *l1=new TLine(6972.42,0,6972.42, 300);
TLine *l2=new TLine(7964.635,0,7964.35, 300);
l->Draw();
l1->Draw();
l2->Draw();

c1->Print(Form("%s_1.png", type.Data()));

std::cout<<out<<std::endl;




return 0;





}
