int draw1Dazi_sectorwise(TString input, TString generator){
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



auto legend=new TLegend(0.75,0.7, 0.9,0.9,"brNDC");
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
   for(Int_t j=1;j<sector.size();j++){
        h[g[i]]=(TH1D*) f[g[i]]->Get("pr_"+sector[j]+"_ph");
        h[g[i]]->Scale(1.0/100);
        h[g[i]]->RebinX(5);
        h[g[i]]->SetLineColor(col[sector[j]]);
        h[g[i]]->SetLineWidth(2);
        h[g[i]]->SetLineStyle(i+1);
        legend->AddEntry(h[g[i]], g[i]+"_"+sector[j]);
        hs.Add(h[g[i]]);
        std::cout<< h[generator]->Integral()*65*7 << std::endl;

   }
}


std::map<TString, Float_t> maximum;
maximum["moller"]=0.006;
maximum["elastic"]=0.0008;
maximum["inelastic"]=0.00004;



hs.Draw("HISTnostack");
hs.SetMaximum(maximum[generator]);
hs.SetMinimum(0);
hs.GetXaxis()->SetTitle("Azimuthal Angle (deg)");
hs.GetYaxis()->SetTitle("Rate(GHz/uA/sep/0.5deg)");
legend->Draw("NDC");
Float_t detinnerr[6]= {-6.4286, 6.4286, -19.2857, 19.2857,  -25.7143, 25.7143};
Float_t pos[6]={0.5, 0.3, 0.7,0.19, 0.81,0};
std::vector<TString> sec{"open", "transition", "transition","closed", "closed",""};
TLine *l;
for(int i=0;i<6;i++){
l=new TLine(detinnerr[i],0, detinnerr[i],maximum[generator]);
l->SetLineWidth(3);
l->Draw();
if (i!=5){
label->DrawLatexNDC(pos[i],0.6, Form("%s",sec[i].Data()));
}
}
legend->Draw();
c.Print(Form("1DAzi_ring5_sectorwise_%s.png", generator.Data()));
return 0;

}




















