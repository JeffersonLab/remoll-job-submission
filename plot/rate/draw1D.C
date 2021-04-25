int draw1D(TString input, TString sector){
std::map<TString,TFile*> f;
std::map<TString,TH1D*> h;


std::map<TString, TString> gen{
	{"moller", Form("/w/moller12gev-sciwork18/rahmans/constantRadiusUpstream/out/rate/moller/%s.root", input.Data())},
	{"elastic", Form("/w/moller12gev-sciwork18/rahmans/constantRadiusUpstream/out/rate/elastic/%s.root", input.Data())},
	{"inelastic", Form("/w/moller12gev-sciwork18/rahmans/constantRadiusUpstream/out/rate/inelastic/%s.root", input.Data())}
};


std::vector<TString> g;
g.push_back("moller");
g.push_back("elastic");
g.push_back("inelastic");

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
        {"beam", kBlue}
};

TCanvas c("c","c", 800, 600);
c.SetMargin(0.13,0.13,0.13,0.13);
THStack hs("hs", Form("Sector: %s", sector.Data()));
gStyle->SetPalette(56);
//ad->SetLogy();

for (Int_t i=0;i<g.size();i++){
	f[g[i]]=new TFile((gen[g[i]]).Data());
        h[g[i]]=(TH1D*) f[g[i]]->Get(Form("pr_%s",sector.Data()));
        h[g[i]]->Scale(1.0/100);
	h[g[i]]->SetLineColor(col[g[i]]);
        h[g[i]]->SetLineWidth(2);
        legend->AddEntry(h[g[i]], g[i]);
        hs.Add(h[g[i]]);

}

std::cout<< h["moller"]->Integral()*65*7 << std::endl;


hs.Draw("HISTnostack");
hs.GetXaxis()->SetRangeUser(500,1300);
hs.SetMaximum(0.02);
hs.SetMinimum(0);
hs.GetXaxis()->SetTitle("r(mm)");
hs.GetYaxis()->SetTitle("Rate(GHz/uA/sep/5mm)");
legend->Draw("NDC");
Float_t detinnerr[7]= {65.0, 69.0, 73.5, 79.0,  90.0, 106,116};
Float_t pos[7]={0.27,0.31,0.37,0.42, 0.6, 0.7, 0.8};
TLine *l;
for(int i=0;i<7;i++){
l=new TLine(detinnerr[i]*10,0, detinnerr[i]*10,0.02);
l->SetLineColor(11);
l->Draw();
if(i!=6){
label->DrawLatexNDC(pos[i],0.6, Form("#color[11]{Ring %d}",i+1));
}
}
c.Print(Form("1DProfile_%s.png", sector.Data()));
return 0;

}




















