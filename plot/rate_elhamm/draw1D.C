std::vector<Double_t> integral(TH1D* h, Double_t xmin, Double_t xmax){
std::vector<Double_t> sum;


TAxis *axis = h->GetXaxis();
Int_t bmin = axis->FindBin(xmin); //in your case xmin=-1.5
Int_t bmax = axis->FindBin(xmax); //in your case xmax=0.8

Double_t error= 0;
Double_t integral = h->IntegralAndError(bmin,bmax, error); // This is a slight over estimate of the error. But to our precision it shouldn't be noticeable.
integral -= h->GetBinContent(bmin)*(xmin-axis->GetBinLowEdge(bmin))/axis->GetBinWidth(bmin);
integral -= h->GetBinContent(bmax)*(axis->GetBinUpEdge(bmax)-xmax)/axis->GetBinWidth(bmax);

sum.push_back(65*7*integral);
sum.push_back(65*7*error);


return sum;
}

int draw1D(TString input, TString sector){
std::map<TString,TFile*> f;
std::map<TString,TH1D*> h;


std::map<TString, TString> gen{
	{"moller", Form("%s/analysed_moller.root", input.Data())},
	{"elastic", Form("%s/analysed_elastic.root", input.Data())},
	{"pion", Form("%s/analysed_pion.root", input.Data())}
};


std::vector<TString> g;
g.push_back("moller");
g.push_back("elastic");
g.push_back("pion");

auto legend=new TLegend(0.75,0.7, 0.9,0.9,"brNDC");
legend->SetHeader("Particle Type", "C");

auto label= new TLatex();
label->SetTextAlign(23);
label->SetTextSize(0.04);
label->SetTextAngle(90);
std::map<TString, Color_t> col{
	{"moller", kBlack},
	{"elastic", kRed},
        {"pion", kGreen},
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
        h[g[i]]->Scale(1.0);
	h[g[i]]->SetLineColor(col[g[i]]);
        h[g[i]]->SetLineWidth(2);
        legend->AddEntry(h[g[i]], g[i]);
        hs.Add(h[g[i]]);
        

}

// Example of calculating integral over the whole radial range of analyzed histogram. In this case, the analyzed histogram spread was taken to be 600-1300 mm on main detector instead of just 900-1060 mm 
std::cout<< h["moller"]->Integral()*65*7 << "GHz"<< std::endl;
//Example of calculating integral over a subrange. For example ring 5.
std::vector<Double_t> ring5_integral=integral(h["moller"], 900, 1060);
std::cout<< ring5_integral[0] << "+/-" << ring5_integral[1] << "GHz" << std::endl;

hs.Draw("HISTnostack");
hs.GetXaxis()->SetRangeUser(500,1300);
//hs.SetMaximum(0.02);
//hs.SetMinimum(0);
gPad->SetLogy();
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

