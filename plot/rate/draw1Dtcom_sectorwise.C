int draw1Dtcom_sectorwise(TString input, TString generator){
std::map<TString,TFile*> f;
std::map<TString,TH1D*> h;


std::map<TString, TString> gen{
        {"moller", Form("/w/moller12gev-sciwork18/rahmans/constantRadiusUpstream/out/rate_ring5/moller/%s.root", input.Data())},
        {"elastic", Form("/w/moller12gev-sciwork18/rahmans/constantRadiusUpstream/out/rate_ring5/elastic/%s.root", input.Data())},
        {"inelastic", Form("/w/moller12gev-sciwork18/rahmans/constantRadiusUpstream/out/rate_ring5/inelastic/%s.root", input.Data())}
};

std::vector<TString> g;
g.push_back(generator);
std::vector<TString> sector;
sector.push_back("all");
sector.push_back("open");
sector.push_back("trans");
sector.push_back("closed");


auto legend=new TLegend(0.75,0.75, 0.9,0.9,"brNDC");
legend->SetHeader("Particle Type", "C");

auto label= new TLatex();
label->SetTextAlign(23);
label->SetTextSize(0.04);
label->SetTextAngle(90);
std::map<TString, Color_t> col{
	{"all", kBlack},
	{"open", kRed},
        {"trans", kGreen},
        {"closed", kBlue}
};

TCanvas c("c","c", 800, 600);
c.SetMargin(0.13,0.13,0.13,0.13);
THStack hs("hs", Form("Ring 5, Process: %s", generator.Data()));
gStyle->SetPalette(56);

for (Int_t i=0;i<g.size();i++){
	f[g[i]]=new TFile((gen[g[i]]).Data());
   for(Int_t j=0;j<sector.size();j++){
        h[g[i]]=(TH1D*) f[g[i]]->Get("pr_"+sector[j]+"_tcom");
        h[g[i]]->Scale(1.0/100);
	h[g[i]]->SetLineColor(col[sector[j]]);
        h[g[i]]->SetLineWidth(2);
        h[g[i]]->SetLineStyle(i+1);
        legend->AddEntry(h[g[i]], g[i]+"_"+sector[j]);
        hs.Add(h[g[i]]);
        std::cout<< h["moller"]->Integral()*65*7 << std::endl;

   }
}



hs.Draw("HISTnostack");
hs.SetMinimum(0);
hs.GetXaxis()->SetTitle("Theta COM(deg)");
hs.GetYaxis()->SetTitle("Rate(GHz/uA/sep/deg)");
legend->Draw("NDC");
c.Print(Form("1DTcom_ring5_sectorwise_%s.png", generator.Data()));
return 0;

}




















