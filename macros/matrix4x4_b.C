//set B - nu10 is variable
void matrix4x4_b(Int_t idDataSet = 0,Double_t min=0.1, Double_t max=10.0, Double_t step=0.1) {

  Double_t width = 3.0;

  TList *list = new TList();
  TGraph *g1 = new TGraph();
  g1->SetLineColor(kMagenta);
  g1->SetLineWidth(width);
  list->Add(g1);
  TGraph *g2 = new TGraph(); 
  g2->SetLineColor(kGreen); 
  g2->SetLineWidth(width);
  list->Add(g2);
  TGraph *g3 = new TGraph(); 
  g3->SetLineColor(7);
  g3->SetLineWidth(width);
  list->Add(g3);
  TGraph *g4 = new TGraph(); 
  g4->SetLineColor(kOrange); 
  g4->SetLineWidth(width);
  list->Add(g4);

  for (Double_t val=min;val<=max;val+=step) {
    MatrixZuzka(idDataSet,val,list);
  }

  g1->GetXaxis()->SetTitle("#nu_{10}");
  g1->GetXaxis()->CenterTitle();
  g1->GetYaxis()->SetTitle("four eigenvalues");
  g1->GetYaxis()->CenterTitle();
  g1->GetYaxis()->SetRangeUser(-14,2);

  TCanvas *c = new TCanvas();
//  if (idDataSet == 1) g1->GetYaxis()->SetRangeUser(-14,2);

  g1->Draw("AL");
  g2->Draw("L SAME");
  g3->Draw("L SAME");
  g4->Draw("L SAME");

  TLegend *leg = new TLegend(0.747318,0.673958,0.83969,0.878125);
  leg->AddEntry(g1,"#lambda_{1}","l");
  leg->AddEntry(g2,"#lambda_{2}","l");
  leg->AddEntry(g3,"#lambda_{3}","l");
  leg->AddEntry(g4,"#lambda_{4}","l");
  leg->Draw();

  c->SaveAs(Form("pic_b%d.pdf",idDataSet));
}

void MatrixZuzka(Int_t idDataSet = 0,Double_t nu10=0.2, TList *lg=0) {
// fixne 
Double_t r0x=1.0;
Double_t r0y=1.5;
Double_t nu01=0.1;
Double_t mxy=2.0;
Double_t myx=1.0;

// old
// if (idDataSet == 1) {
//   r0x=1.0;
//   r0y=1.5;
//   nu10=0.1;
//   mxy=2.0;
//   myx=1.0;
// }


TMatrixD a(4,4);
a(0,0)=r0x-nu01;
a(0,1)=nu10;
a(0,2)=0;
a(0,3)=0;
a(1,0)=nu01;
a(1,1)=-nu10-mxy;
a(1,2)=0;
a(1,3)=myx;
a(2,0)=0;
a(2,1)=0;
a(2,2)=r0y-nu01;
a(2,3)=nu10;
a(3,0)=0;
a(3,1)=mxy;
a(3,2)=nu01;
a(3,3)=-nu10-myx;

TMatrixDEigen ea(a);


const TVectorD ear = ea.GetEigenValuesRe();
const TVectorD eai = ea.GetEigenValuesIm();

TGraph *g1 = (TGraph*) lg->At(0);
g1->SetTitle(Form("4x4 matrix blok, constant parameters: r_{0x}=%.1f, r_{0y}=%.1f, #nu_{01}=%.2f, #mu_{xy}=%.2f, #mu_{yx}=%.2f",r0x,r0y,nu01,mxy,myx));

TGraph *g2 = (TGraph*) lg->At(1);
TGraph *g3 = (TGraph*) lg->At(2);
TGraph *g4 = (TGraph*) lg->At(3);

g1->SetPoint(g1->GetN(),nu10,ear(0));
g2->SetPoint(g2->GetN(),nu10,ear(1));
g3->SetPoint(g3->GetN(),nu10,ear(2));
g4->SetPoint(g4->GetN(),nu10,ear(3));

Printf("%f %f %f %f", ear(0), ear(1), ear(2), ear(3));

}
