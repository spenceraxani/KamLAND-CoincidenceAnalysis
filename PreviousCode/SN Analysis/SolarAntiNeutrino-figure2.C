{
gROOT->Reset();
gROOT->SetStyle("Plain");
gStyle->SetOptStat(0);
  
TCanvas c1("c1", "", 0, 0, 700, 700);
c1.Divide(2,2);
  
TFile *f_MC = TFile::Open("/data/work/shimizu/work/neutron-MC/SpaceCorrelation/VertexFluctuation/ReactorEfficiency-Hist/SelectionEfficiency-R600-dT1000-dR160-Rxy0-Ed0-Hist.root");

TH1F* h_Ed = gROOT->FindObject("h_Ed");
TH1F* h_dR = gROOT->FindObject("h_dR");
TH1F* h_dT = gROOT->FindObject("h_dT");

TFile *foo = TFile::Open("DelayedCoincidence-SolarAntiNeutrino.root");

//TCut RunCut = "run>=1313 && run<=5221";
 TCut RunCut = "run!=5380";//all run // temporary decal electron cut
//TCut RunCut = "run<=4051";//geo-nu result
//TCut RunCut = "run<=3206";//second result
//TCut RunCut = "run<=1466";//first result
//TCut RunCut = "run<1896";//before e-change
//TCut RunCut = "run>=1896";//after e-change
//TCut RunCut = "run<1313";//before OD-fix
//TCut RunCut = "run>=1313";//after OD-fix

////analysis 1////
//  TCut FiducialCut = "rp<500 && rd<500";
//  TCut CylinderCut = "sqrt(xd*xd+yd*yd)>=120";
//  TCut EpCut = "Ep>=0.9 && Ep<20";
//  TCut EdCut = "Ed>=1.8 && Ed<2.6";
//  TCut dTCut = "dT>=0.5 && dT<660";
//  TCut dRCut = "dR<=160";
//  TCut MuonCut = "NhitOD<5";
//  TCut MuonVeto = "veto==0";
//  TCut MultiplicityCut = "";

////analysis 2////
//TCut FiducialCut = "";
//TCut FiducialCut = "zp<650 && zd<650";
//TCut FiducialCut = "rp<650 && rd<650";
//TCut FiducialCut = "rp<550 && rd<550";
TCut FiducialCut = "rp<600 && rd<600";
TCut CylinderCut = "";//No cylinder Cut
//TCut EpCut = "Ep>=7.5 && Ep<14.0";
TCut EpCut = "Ep>=7.5 && Ep<30.0";
TCut EdCut = "Ed>=1.8 && Ed<2.6";
TCut dTCut = "dT>=0.5 && dT<1000";
TCut dTpCut = "dTp>=15";
//TCut dTpCut = "dTp>=15.0";
//TCut dTCut = "dT>=10.0 && dT<1000";
//TCut dRCut = "dR<200";
TCut dRCut = "dR<160";
TCut MuonCut = "N200OD<5";
TCut MuonVeto = "veto==0";
TCut MultiplicityCut = "";
//TCut MultiplicityCut = "multiplicity<=1";
//TCut MultiplicityCut = "multiplicity>1";


TTree* tree1 = nt.CopyTree(RunCut&&FiducialCut&&CylinderCut&&EdCut&&dTCut&&dTpCut&&dRCut&&MuonCut&&MuonVeto&&EpCut);


c1.cd(1);
c1_1.SetTopMargin(0.05);
c1_1.SetRightMargin(0.05);
c1_1.SetBottomMargin(0.15);
c1_1.SetLeftMargin(0.15);

float Rfid2 = 6.0*6.0;

 TH2F waku1("waku1","",2,0,38,2,-6.2,6.2);
//TH2F waku1("waku1","",2,0,50,2,-8,8);
waku1.SetNdivisions(505);
waku1.GetXaxis()->SetTitleOffset(1.0);
waku1.GetYaxis()->SetTitleOffset(1.1);
waku1.GetXaxis()->SetLabelSize(0.07);
waku1.GetXaxis()->SetTitleSize(0.07);
waku1.GetYaxis()->SetLabelSize(0.07);
waku1.GetYaxis()->SetTitleSize(0.07);
waku1.GetXaxis()->SetLabelFont(132);
waku1.GetXaxis()->SetTitleFont(132);
waku1.GetYaxis()->SetLabelFont(132);
waku1.GetYaxis()->SetTitleFont(132);
waku1.GetXaxis()->CenterTitle();
waku1.GetYaxis()->CenterTitle();
waku1.SetXTitle("X^{2}+Y^{2} (m^{2})");
waku1.SetYTitle("Z (m)");

// TH1F waku1("waku1","",2,0,50);
// waku1.SetMinimum(-8);
// waku1.SetMaximum(8);
// waku1.SetXTitle("x^{2}+y^{2} [m^{2}]");
// waku1.SetYTitle("z [m]");
// waku1.GetXaxis()->SetLabelSize(0.05);
// waku1.GetYaxis()->SetLabelSize(0.05);
// waku1.GetXaxis()->SetLabelFont(22);
// waku1.GetYaxis()->SetLabelFont(22);

float x[1000], y[1000];
int index=0;
for(float r2=0; r2<=Rfid2; r2+=0.1){
  x[index] = r2;
  y[index] = sqrt(Rfid2-x[index]);
  index++;
}
for(float r2=Rfid2; r2>=0; r2-=0.1){
  x[index] = r2;
  y[index] = -sqrt(Rfid2-x[index]);
  index++;
}

TGraph g1(index,x,y);
g1.SetLineWidth(2);
g1.SetLineColor(4);
g1.SetLineStyle(2);
// g1.SetLineWidth(3);
// g1.SetLineColor(9);
// g1.SetLineStyle(1);

index=0;
for(float r2=0; r2<=42.25; r2+=0.1){
  x[index] = r2;
  y[index] = sqrt(42.25-x[index]);
  index++;
}
for(float r2=42.25; r2>=0; r2-=0.1){
  x[index] = r2;
  y[index] = -sqrt(42.25-x[index]);
  index++;
}

TGraph g2(index,x,y);
g2.SetLineWidth(3);
g2.SetLineColor(9);
g2.SetLineStyle(2);


waku1.Draw();
g1.Draw("L");
//g2.Draw("L");

nt.SetMarkerStyle(8);
nt.SetMarkerColor(1);
nt.SetMarkerSize(0.25);

c1.Update();
nt.Draw("zp*0.01:(xp*xp+yp*yp)*0.0001",RunCut&&FiducialCut&&CylinderCut&&EdCut&&dTCut&&dTpCut&&dRCut&&MuonCut&&MuonVeto&&EpCut,"same");
c1.Update();

waku1.Draw("AXIS,same");


TLatex* text = new TLatex(0 - (38 - 0) * 0.19, 6.2, "(a)");
text->SetTextFont(132);
text->SetTextSize(0.07);
text->Draw();


// //double BinWidth = (2.6 - 0.9) / 4.0;
// //double BinWidth = 0.5;
// double BinWidth = 1.0;
// //double Min = 0.9;
// //double Min = 0;
// double Min = 5.5;
// //double Max = 0.9 + 24 * BinWidth;
// //double Max = 0 + 40 * BinWidth;
// //double Max = 0 + 70 * BinWidth;
// double Max = Min + 25 * BinWidth;

// // TH1F h1("h1", "", 24, Min, Max);
// // TH1F h1_UpperECut("h1_UpperECut", "", 24, Min, Max);
// // TH1F h1("h1", "", 40, Min, Max);
// // TH1F h1_UpperECut("h1_UpperECut", "", 40, Min, Max);
// // TH1F h1("h1", "", 70, Min, Max);
// // TH1F h1_UpperECut("h1_UpperECut", "", 70, Min, Max);
// TH1F h1("h1", "", 25, Min, Max);
// TH1F h1_UpperECut("h1_UpperECut", "", 25, Min, Max);

// //TH1F h1_m("h1_m", "", 40, Min, Max);
// //TH1F h1_m("h1_m", "", 70, Min, Max);
// TH1F h1_m("h1_m", "", 25, Min, Max);

// c1.Update();
// nt.Draw("Ep>>h1", RunCut&&FiducialCut&&CylinderCut&&EdCut&&dTCut&&dTpCut&&dRCut&&MuonCut&&MuonVeto&&MultiplicityCut);
// nt.Draw("Ep>>h1_UpperECut", RunCut&&FiducialCut&&CylinderCut&&EdCut&&dTCut&&dTpCut&&dRCut&&MuonCut&&MuonVeto&&MultiplicityCut&&"Ep<30.0");
// //nt.Draw("Ep>>h1_UpperECut", RunCut&&FiducialCut&&CylinderCut&&EdCut&&dTCut&&dTpCut&&dRCut&&MuonCut&&MuonVeto&&MultiplicityCut&&"Ep<14.0");
// c1.Update();

// c1.Update();
// nt.Draw("Ep>>h1_m", RunCut&&FiducialCut&&CylinderCut&&EdCut&&dTCut&&dTpCut&&dRCut&&MuonCut&&MuonVeto&&MultiplicityCut&&"multiplicity>1");
// c1.Update();

// // multiplicity 2 case
// // for(int i=1;i<=70;i++){
//  for(int i=1;i<=25;i++){
//    double content_m = h1_m.GetBinContent(i);
//    h1_m.SetBinContent(i, content_m * 0.5);

//    double content = h1.GetBinContent(i);
//    h1.SetBinContent(i, content - content_m * 0.5);

//    double content_u = h1_UpperECut.GetBinContent(i);
//    h1_UpperECut.SetBinContent(i, content_u - content_m * 0.5);
//  }


// //TH1F waku("waku", "", 2, 0.0, 10.0);
// //TH1F waku("waku", "", 2, 6.5, 20.0);
// // TH1F waku("waku", "", 2, 6.5, 35.0);
// // waku.SetMinimum(0);
// // waku.SetMaximum(50);
// //TH1F waku("waku", "", 2, 5.0, 35.0);
// TH1F waku("waku", "", 2, 5.5, 30.5);
// waku.SetMinimum(0.1);
// waku.SetMaximum(200);
// waku.SetNdivisions(505);
// waku.GetXaxis()->SetTitleOffset(1.5);
// waku.GetYaxis()->SetTitleOffset(1.5);
// waku.SetXTitle("prompt energy [MeV]");
// waku.SetYTitle("events/0.5MeV");
// //waku.SetYTitle("events/0.425MeV");
// waku.Draw();

// h1.SetLineStyle(1);
// h1.SetLineWidth(2);
// h1.SetFillColor(5);
// h1.Draw("same");

// h1_m.SetLineStyle(1);
// h1_m.SetLineWidth(2);
// h1_m.SetFillColor(9);
// //h1_m.Draw("same");

// waku.Draw("same, AXIS");

// int total_LAT = 0;
// int total_HAT = 0;
// //for(int i=1;i<=40;i++){
// //for(int i=1;i<=70;i++){
// for(int i=1;i<=25;i++){

//   double mean_energy  = BinWidth * (double(i-1) + 0.5) + Min;
//   double lower_energy = BinWidth * (double(i-1) + 0.0) + Min;
//   double upper_energy = BinWidth * (double(i-1) + 1.0) + Min;

//   if(mean_energy > 30) break;

//   cerr << mean_energy << " " << lower_energy << " " << upper_energy << " " << h1.GetBinContent(i) << endl;

//   if(mean_energy > 7.5) total_LAT = total_LAT + h1_UpperECut.GetBinContent(i);
//   if(mean_energy > 14.0) total_HAT = total_HAT + h1_UpperECut.GetBinContent(i);
// }
// cerr << endl;

// cerr << "Total LAT (7.5 MeV energy threshold) = " << total_LAT << endl;
// cerr << "Total HAT (14.0 MeV energy threshold) = " << total_HAT << endl;
// cerr << endl;

// //TLine *line = new TLine(2.6, 0.0, 2.6, waku.GetMaximum());
// TLine *line = new TLine(7.5, 0.0, 7.5, waku.GetMaximum());
// line->SetLineColor(2);
// line->SetLineStyle(3);
// line->SetLineWidth(3);
// line->Draw();
// //TLine *line = new TLine(14.0, 0.0, 14.0, waku.GetMaximum());
// TLine *line = new TLine(30.0, 0.0, 30.0, waku.GetMaximum());
// line->SetLineColor(2);
// line->SetLineStyle(3);
// line->SetLineWidth(3);
// line->Draw();


c1.cd(2);
c1_2.SetTopMargin(0.05);
c1_2.SetRightMargin(0.05);
c1_2.SetBottomMargin(0.15);
c1_2.SetLeftMargin(0.15);

TH1F h2("h2", "", 16, 1.4, 3.0);
TH1F h2_m("h2_m", "", 16, 1.4, 3.0);

// TH1F h2("h2", "", 46, 1.4, 6);
// TH1F h2_m("h2_m", "", 46, 1.4, 6);

// TH1F h2("h2", "", 50, 1, 6);
// TH1F h2_m("h2_m", "", 50, 1, 6);

c1.Update();
nt.Draw("Ed>>h2", RunCut&&FiducialCut&&CylinderCut&&EpCut&&dTCut&&dTpCut&&dRCut&&MuonCut&&MuonVeto&&MultiplicityCut);
c1.Update();

c1.Update();
nt.Draw("Ed>>h2_m", RunCut&&FiducialCut&&CylinderCut&&EpCut&&dTCut&&dTpCut&&dRCut&&MuonCut&&MuonVeto&&MultiplicityCut&&"multiplicity>1");
c1.Update();

h2.SetMinimum(0);
h2.SetMaximum(18);
h2.GetXaxis()->SetNdivisions(505);
h2.GetYaxis()->SetNdivisions(505);
h2.GetXaxis()->SetTitleOffset(1.0);
h2.GetYaxis()->SetTitleOffset(1.1);
h2.GetXaxis()->SetLabelSize(0.07);
h2.GetXaxis()->SetTitleSize(0.07);
h2.GetYaxis()->SetLabelSize(0.07);
h2.GetYaxis()->SetTitleSize(0.07);
h2.GetXaxis()->SetLabelFont(132);
h2.GetXaxis()->SetTitleFont(132);
h2.GetYaxis()->SetLabelFont(132);
h2.GetYaxis()->SetTitleFont(132);
h2.GetXaxis()->CenterTitle();
h2.GetYaxis()->CenterTitle();
h2.SetLineWidth(1);
h2.SetMarkerStyle(20);
h2.SetMarkerSize(0.5);
h2.SetMarkerColor(1);
h2.SetXTitle("E_{d} (MeV)");
h2.SetYTitle("Events / 0.1 MeV");
h2.Draw("E");

h2_m.SetLineStyle(1);
h2_m.SetLineWidth(2);
h2_m.SetFillColor(9);
//h2_m.Draw("same");

h_Ed->Scale(tree1->GetEntries() / h_Ed->GetEntries() * 10.0);

h_Ed->Draw("same");

TLine *line = new TLine(1.8, 0.0, 1.8, h2.GetMaximum());
//TLine *line = new TLine(1.8, 0.0, 1.8, h2.GetMaximum() * 1.05);
line->SetLineColor(4);
line->SetLineStyle(2);
line->SetLineWidth(2);
line->Draw();
TLine *line = new TLine(2.6, 0.0, 2.6, h2.GetMaximum());
//TLine *line = new TLine(2.6, 0.0, 2.6, h2.GetMaximum() * 1.05);
line->SetLineColor(4);
line->SetLineStyle(2);
line->SetLineWidth(2);
line->Draw();


TLatex* text = new TLatex(1.4 - (3.0 - 1.4) * 0.19, h2.GetMaximum(), "(b)");
text->SetTextFont(132);
text->SetTextSize(0.07);
text->Draw();


c1.cd(3);
c1_3.SetTopMargin(0.05);
c1_3.SetRightMargin(0.05);
c1_3.SetBottomMargin(0.15);
c1_3.SetLeftMargin(0.15);

TH1F h3("h3", "", 20, 0, 2.0);
TH1F h3_m("h3_m", "", 20, 0, 2.0);

// TH1F h3("h3", "", 15, 0, 300);
// TH1F h3_m("h3_m", "", 15, 0, 300);


c1.Update();
nt.Draw("dR*0.01>>h3", RunCut&&FiducialCut&&CylinderCut&&EpCut&&EdCut&&dTCut&&dTpCut&&MuonCut&&MuonVeto&&MultiplicityCut);
c1.Update();

c1.Update();
nt.Draw("dR*0.01>>h3_m", RunCut&&FiducialCut&&CylinderCut&&EpCut&&EdCut&&dTCut&&dTpCut&&MuonCut&&MuonVeto&&MultiplicityCut&&"multiplicity>1");
c1.Update();

h3.SetMinimum(0);
h3.SetMaximum(12);
h3.GetXaxis()->SetNdivisions(505);
h3.GetYaxis()->SetNdivisions(505);
h3.GetXaxis()->SetTitleOffset(1.0);
h3.GetYaxis()->SetTitleOffset(1.1);
h3.GetXaxis()->SetLabelSize(0.07);
h3.GetXaxis()->SetTitleSize(0.07);
h3.GetYaxis()->SetLabelSize(0.07);
h3.GetYaxis()->SetTitleSize(0.07);
h3.GetXaxis()->SetLabelFont(132);
h3.GetXaxis()->SetTitleFont(132);
h3.GetYaxis()->SetLabelFont(132);
h3.GetYaxis()->SetTitleFont(132);
h3.GetXaxis()->CenterTitle();
h3.GetYaxis()->CenterTitle();
h3.SetLineWidth(1);
h3.SetMarkerStyle(20);
h3.SetMarkerSize(0.5);
h3.SetMarkerColor(1);
h3.SetXTitle("#DeltaR (m)");
h3.SetYTitle("Events / 0.1 m");
h3.Draw("E");

// h3.SetLineStyle(1);
// h3.SetLineWidth(2);
// h3.SetFillColor(5);
// h3.SetNdivisions(505);
// h3.GetXaxis()->SetTitleOffset(1.5);
// h3.GetYaxis()->SetTitleOffset(1.5);
// h3.SetXTitle("#Delta R [cm]");
// h3.SetYTitle("events/20cm");
// h3.Draw("");

h3_m.SetLineStyle(1);
h3_m.SetLineWidth(2);
h3_m.SetFillColor(9);
//h3_m.Draw("same");

h_dR->Scale(tree1->GetEntries() / h_dR->GetEntries() * 10.0);

h_dR->Draw("same");

//TLine *line = new TLine(200, 0.0, 200, h3.GetMaximum() * 1.05);
//TLine *line = new TLine(160, 0.0, 160, h3.GetMaximum() * 1.05);
TLine *line = new TLine(1.60, 0.0, 1.60, h3.GetMaximum());
line->SetLineColor(4);
line->SetLineStyle(2);
line->SetLineWidth(2);
line->Draw();


TLatex* text = new TLatex(0 - (2.0 - 0) * 0.19, h3.GetMaximum(), "(c)");
text->SetTextFont(132);
text->SetTextSize(0.07);
text->Draw();


c1.cd(4);
c1_4.SetTopMargin(0.05);
c1_4.SetRightMargin(0.05);
c1_4.SetBottomMargin(0.15);
c1_4.SetLeftMargin(0.15);

TH1F h4("h4", "", 25, 0, 2.5);
TH1F h4_m("h4_m", "", 25, 0, 2.5);

c1.Update();
//nt.Draw("dT*0.001>>h4", RunCut&&FiducialCut&&CylinderCut&&EpCut&&EdCut&&dRCut&&MuonCut&&MuonVeto&&MultiplicityCut);
nt.Draw("dT*0.001>>h4", RunCut&&FiducialCut&&CylinderCut&&EpCut&&EdCut&&dRCut&&MuonCut&&MuonVeto&&MultiplicityCut&&"dT>=10.0");
c1.Update();

c1.Update();
nt.Draw("dT*0.001>>h4_m", RunCut&&FiducialCut&&CylinderCut&&EpCut&&EdCut&&dRCut&&MuonCut&&MuonVeto&&MultiplicityCut&&"dT>=10.0"&&"multiplicity>1");
c1.Update();

h4.SetMinimum(0);
h4.SetMaximum(15);
h4.GetXaxis()->SetNdivisions(505);
h4.GetYaxis()->SetNdivisions(505);
h4.GetXaxis()->SetTitleOffset(1.0);
h4.GetYaxis()->SetTitleOffset(1.1);
h4.GetXaxis()->SetLabelSize(0.07);
h4.GetXaxis()->SetTitleSize(0.07);
h4.GetYaxis()->SetLabelSize(0.07);
h4.GetYaxis()->SetTitleSize(0.07);
h4.GetXaxis()->SetLabelFont(132);
h4.GetXaxis()->SetTitleFont(132);
h4.GetYaxis()->SetLabelFont(132);
h4.GetYaxis()->SetTitleFont(132);
h4.GetXaxis()->CenterTitle();
h4.GetYaxis()->CenterTitle();
h4.SetLineWidth(1);
h4.SetMarkerStyle(20);
h4.SetMarkerSize(0.5);
h4.SetMarkerColor(1);
h4.SetXTitle("#DeltaT (ms)");
h4.SetYTitle("Events / 0.1 ms");
h4.Draw("E");

// h4.SetLineStyle(1);
// h4.SetLineWidth(2);
// h4.SetFillColor(5);
// h4.SetNdivisions(505);
// h4.GetXaxis()->SetTitleOffset(1.5);
// h4.GetYaxis()->SetTitleOffset(1.5);
// h4.SetXTitle("#Delta T [msec]");
// h4.SetYTitle("events/0.1msec");
// h4.Draw("");

h4_m.SetLineStyle(1);
h4_m.SetLineWidth(2);
h4_m.SetFillColor(9);
//h4_m.Draw("same");

h_dT->Scale(tree1->GetEntries() / h_dT->GetEntries() * 10.0);

h_dT->Draw("same");

TLine *line = new TLine(1.0, 0.0, 1.0, h4.GetMaximum());
//TLine *line = new TLine(1.0, 0.0, 1.0, h4.GetMaximum() * 1.05);
//TLine *line = new TLine(0.66, 0.0, 0.66, h4.GetMaximum() * 1.05);
line->SetLineColor(4);
line->SetLineStyle(2);
line->SetLineWidth(2);
line->Draw();


TLatex* text = new TLatex(0 - (2.5 - 0) * 0.19, h4.GetMaximum(), "(d)");
text->SetTextFont(132);
text->SetTextSize(0.07);
text->Draw();}
}

