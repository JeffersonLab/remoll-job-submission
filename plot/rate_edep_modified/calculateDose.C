Int_t calculateDose(TString input, TString type, Int_t scale, bool
    fixed_range=true){

TFile *f =new TFile(Form("%s",input.Data()));
Float_t weight = 65*344*24*60*60/(40e-9*1.3*1e3*1e6);

TCanvas *c=new TCanvas("c","c", 800, 600);
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

	gStyle->SetOptStat(0);
	h_clone[i][0]->Scale(1.0/scale*weight);
        h_clone[i][0]->RebinX(2);
	
	
	TLine *l=new TLine(5937.67,0,5937.67, 300);
	l->Draw();
	
	TLine *l1=new TLine(6972.42,0,6972.42, 300);
	l1->Draw();
	
	TLine *l2=new TLine(7964.635,0,7964.35, 300);
	l2->Draw();
	

	
}

Int_t nlevels=4;
Double_t levels[nlevels];
levels[0]=0.0;
levels[1]=50.0;
levels[2]=80.0;
levels[3]=160.0;

c->Divide(3,3); 
 for(Int_t i=1;i<=7;i++){
c->cd(i);
gPad->SetMargin(0.15,0.15, 0.15, 0.15);
h_clone[i][0]->SetTitle("Dose(MGy)");
h_clone[i][0]->SetTitleSize(0.06);
/* if ( fixed_range == kTrue ) { */
/*   h_clone[i][0]->SetContour(nlevels, levels); */
/* } */
h_clone[i][0]->Draw("colz");
if ( fixed_range == true ) {
  h_clone[i][0]->SetContour(nlevels, levels);
  h_clone[i][0]->GetZaxis()->SetRangeUser(0,200);
}
h_clone[i][0]->GetZaxis()->SetLabelSize(0.06);
h_clone[i][0]->GetYaxis()->SetLabelSize(0.06);
h_clone[i][0]->GetXaxis()->SetLabelSize(0.06);
h_clone[i][0]->GetXaxis()->SetRangeUser(5000,8000);
} 
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
