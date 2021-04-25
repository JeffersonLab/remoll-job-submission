int draw1DfA(TString input, TString sector){
std::map<TString,TFile*> f;
std::map<TString,TH1D*> h;
std::map<TString,TH1D*> h1;

std::map<TString, TString> gen{
	{"moller", Form("/w/moller12gev-sciwork18/rahmans/constantRadiusUpstream/out/rate/moller/%s.root", input.Data())},
	{"elastic", Form("/w/moller12gev-sciwork18/rahmans/constantRadiusUpstream/out/rate/elastic/%s.root", input.Data())},
	{"inelastic", Form("/w/moller12gev-sciwork18/rahmans/constantRadiusUpstream/out/rate/inelastic/%s.root", input.Data())}
};

std::map<TString, TString> gen1{
        {"moller", Form("/w/moller12gev-sciwork18/rahmans/constantRadiusUpstream/out/rate/moller/%s.root", input.Data())},
        {"elastic", Form("/w/moller12gev-sciwork18/rahmans/constantRadiusUpstream/out/rate/elastic/%s.root", input.Data())},
        {"inelastic", Form("/w/moller12gev-sciwork18/rahmans/constantRadiusUpstream/out/rate/inelastic/%s.root", input.Data())}

};
 
std::vector<TString> g{"moller", "elastic", "inelastic"};

auto legend=new TLegend(0.75,0.7, 0.9,0.9,"brNDC");
legend->SetHeader("Particle Type", "C");

auto label= new TLatex();
label->SetTextAlign(23);
label->SetTextSize(0.04);
label->SetTextAngle(90);
std::map<TString, Color_t> col{
	{"moller", kBlack},
	{"elastic", kRed},
        {"inelastic", kGreen},
};

TCanvas c("c","c", 800, 600);
c.SetMargin(0.13,0.13,0.13,0.13);
THStack hs("hs", Form("Sector: %s",sector.Data()));
gStyle->SetPalette(56);

for (Int_t i=0;i<3;i++){
        f[g[i]]=new TFile((gen[g[i]]).Data());
        h[g[i]]=(TH1D*) f[g[i]]->Get(Form("pr_all"));
}

h["moller"]->Add(h["elastic"]);
h["moller"]->Add(h["inelastic"]);


for (Int_t i=0;i<3;i++){
	f[g[i]]=new TFile((gen[g[i]]).Data());
        h1[g[i]]=(TH1D*) f[g[i]]->Get(Form("pr_%s_rA",sector.Data()));
        h1[g[i]]->Scale(-1.0);
        h1[g[i]]->Divide(h["moller"]);
	h1[g[i]]->SetLineColor(col[g[i]]);
        h1[g[i]]->SetLineWidth(2);
        legend->AddEntry(h1[g[i]], g[i]);
        hs.Add(h1[g[i]]);

}

Double_t error=0;
Double_t integral=0;
std::cout<<"\n" ;
for(int i=0;i<3;i++){
integral=h1[g[i]]->IntegralAndError(0,-1,error);
std::cout<<"Integral "<<integral<< "+/-"<< error << std::endl;
}

hs.Draw("HISTnostack");
hs.GetXaxis()->SetRangeUser(500,1300);
hs.SetMaximum(190);
hs.SetMinimum(0);
//.SetTitle("Radial distribution of ee, ep, and ine [GHz/uA/(5mm){}^{2}]");
hs.GetXaxis()->SetTitle("r(mm)");
hs.GetYaxis()->SetTitle("f_{i}A_{i} (ppb/5mm)");
legend->Draw("NDC");
//h["moller"]->GetXaxis()->SetRangeUser(-130,-50);
//.Draw("colz");
Float_t detinnerr[7]= {65.0, 69.0, 73.5, 79.0,  90.0, 106,116};
Float_t pos[7]={0.27,0.31,0.37,0.42, 0.6, 0.7, 0.8};
TLine *l;
for(int i=0;i<7;i++){
l=new TLine(detinnerr[i]*10,0, detinnerr[i]*10,190);
l->SetLineColor(11);
l->Draw();
if(i!=6){
label->DrawLatexNDC(pos[i],0.6, Form("#color[11]{Ring %d}",i+1));
}
}
c.Print(Form("1DProfile_fA_%s.png",sector.Data()));
return 0;

}




















