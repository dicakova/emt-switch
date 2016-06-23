#include <TH1.h>
#include <TH2.h>
//#include <TAxis.h>
void FixBinning(Int_t nDim, Int_t *bins, Double_t *min, Double_t *max) {
	Double_t step;
	for (Int_t i = 0; i < nDim; i++) {
		if (bins[i] == 1)
			continue;
//		if (min[i] < 0) {
		step = (max[i] - min[i]) / (bins[i] - 1);
		min[i] -= step;
//		}
	}
}

void test() {
	gROOT->LoadMacro("EmtMatrix.cxx+");

//	//				    dr     dnu		rho	  gama r/i
//	Int_t bins[5] = { 11, 1, 11, 1, 2 };
////	Int_t bins[5] = { 1001, 101, 1001, 10, 2 };
//	Double_t min[5] = { -0.5, -0.05, -0.5, 0.00, 0 };
//	Double_t max[5] = { 0.5, 0.05, 0.5, 0.01, 2 };

//	nu01		    r0x r0y  nu01 nu10 mxy myx r/i
//	Int_t bins[7] = { 1, 1, 1001,  1,   1, 1, 2 };
//	Double_t min[7] = { 0, 0, 0, 0, 	0, 0,  1 };
//	Double_t max[7] = { 2, 4, 10, 0.1, 0.01, 0.01, 2 };

////	nu10		    r0x r0y  nu01 nu10 		mxy myx r/i
//	Int_t bins[7] = { 1, 1, 1, 1001, 1, 1, 2 };
//	Double_t min[7] = { 0, 0, 0, 0, 0, 0, 1 };
//	Double_t max[7] = { 1, 1.5, 0.1, 10, 2, 1, 2 };

//		nu10		    r0x r0y  nu01 nu10 		mxy myx r/i
	Int_t bins[7] = { 1, 1, 1001, 1001, 1, 1, 2 };
	Double_t min[7] = { 0, 0, 0, 0, 0, 0, 1 };
	Double_t max[7] = { 1, 1.5, 10, 10, 2, 1, 2 };

	// TODO mame problem s nulou, ked min a max je s rovnakym znamienkom
	FixBinning(7, bins, min, max);

	THn *hs = new THnF("hs", "hs", 7, bins, min, max);

	EmtMatrix *k = new EmtMatrix(hs);
	// 0=No Debug, 1=ear_max, 2 ear, 3 all
	k->SetDebug(0);
	k->Process();

//		hs->GetAxis(3)->SetRange(1,1);
//	Int_t mybin = hs->GetAxis(4)->FindBin(0.06);
//	hs->GetAxis(3)->SetRange(mybin, mybin);
	//hs->GetAxis(3)->SetRangeUser(0.06, 0.07);
	// REAL

	Int_t mybin;

	TH1::AddDirectory(kFALSE);
	// IMAGINARY
//	hs->GetAxis(6)->SetRange(2,2);
//	TCanvas *c1 = new TCanvas();
//	for (Int_t i = 1; i <= hs->GetAxis(3)->GetNbins(); i++) {
////		// mybin = hs->GetAxis(3)->FindBin(0.1);
////		// hs->GetAxis(3)->SetRange(mybin, mybin);
//		hs->GetAxis(3)->SetRange(i, i);
//		hs->GetAxis(6)->SetRange(1, 1);
//		TH1 * histNu01 = hs->Projection(2);
//		histNu01->SetStats(0);
//		histNu01->SetTitle(TString::Format("#nu_{01} for #nu_{10}=%f",hs->GetAxis(3)->GetBinUpEdge(i)).Data());;
//		histNu01->GetXaxis()->SetTitle("#nu_{01}");
//		histNu01->GetXaxis()->CenterTitle();
//		histNu01->GetYaxis()->SetTitle("eigen value");
//		histNu01->GetYaxis()->CenterTitle();
//		histNu01->GetYaxis()->SetRangeUser(0.0,1.5);
//		histNu01->Draw();
//		c1->SaveAs(TString::Format("/tmp/nu01-nu10_%f.pdf",hs->GetAxis(3)->GetBinUpEdge(i)).Data());
//	}

	// reset 3rd axis
	hs->GetAxis(3)->SetRange(1, 0);

//	TCanvas *c2 = new TCanvas();
//	for (Int_t i = 1; i <= hs->GetAxis(2)->GetNbins(); i++) {
////		mybin = hs->GetAxis(3)->FindBin(0.1);
////		hs->GetAxis(3)->SetRange(mybin, mybin);
//		hs->GetAxis(2)->SetRange(i, i);
//		hs->GetAxis(6)->SetRange(1, 1);
//		TH1 * histNu10 = hs->Projection(3);
//		histNu10->SetStats(0);
//		histNu10->SetTitle(TString::Format("#nu_{10} for #nu_{01}=%f",hs->GetAxis(2)->GetBinUpEdge(i)).Data());;
//		histNu10->GetXaxis()->SetTitle("#nu_{10}");
//		histNu10->GetXaxis()->CenterTitle();
//		histNu10->GetYaxis()->SetTitle("eigen value");
//		histNu10->GetYaxis()->CenterTitle();
//		histNu10->GetYaxis()->SetRangeUser(0.0,1.5);
//		histNu10->Draw();
//		c2->SaveAs(TString::Format("/tmp/nu01-nu10_%f.pdf",hs->GetAxis(2)->GetBinUpEdge(i)).Data());
//	}

	hs->GetAxis(2)->SetRange(1, 0);
	hs->GetAxis(3)->SetRange(1, 0);

	TCanvas *c3 = new TCanvas();
	TH2D * histNu01Nu10 = hs->Projection(3,2);
	histNu01Nu10->SetStats(0);
	histNu01Nu10->GetXaxis()->SetTitle("#nu_{01}");
	histNu01Nu10->GetYaxis()->SetTitle("#nu_{10}");
	hs->GetAxis(6)->SetRange(1, 1);
	histNu01Nu10->Draw("CONT COL");
	c3->SaveAs("/tmp/nu01-nu10_2d.pdf");
//
//	hs->GetAxis(2)->SetRange(2,2);
//    hist = hs->Projection(1);
//    hist->Draw();

}
