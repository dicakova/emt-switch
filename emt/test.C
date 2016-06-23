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

void test(Int_t projection = 2, Int_t cut = 3, Bool_t is2D = kFALSE,
		Bool_t save = kFALSE) {
	gROOT->LoadMacro("EmtMatrix.cxx+");

	const Int_t nDim = 7;
	char *names[] = { "r0x", "r0y", "nu01", "nu10", "mxy", "myx", "ri" };
	char *titles[] = { "r_{0x}", "r_{0y}", "#nu_{01}", "#nu_{10}", "m_{xy}",
			"m_{yx}", "ri" };

//	nu01		    r0x r0y  nu01 nu10 mxy myx r/i
//	Int_t bins[nDim] = { 1, 1, 1001,  1,   1, 1, 2 };
//	Double_t min[nDim] = { 0, 0, 0, 0, 	0, 0,  1 };
//	Double_t max[nDim] = { 2, 4, 10, 0.1, 0.01, 0.01, 2 };

////	nu10		    r0x r0y  nu01 nu10 		mxy myx r/i
//	Int_t bins[nDim] = { 1, 1, 1, 1001, 1, 1, 2 };
//	Double_t min[nDim] = { 0, 0, 0, 0, 0, 0, 1 };
//	Double_t max[nDim] = { 1, 1.5, 0.1, 10, 2, 1, 2 };

//		nu10		    r0x r0y  nu01 nu10 		mxy myx r/i
	Int_t bins[nDim] = { 1, 1, 1001, 1001, 1, 1, 2 };
	Double_t min[nDim] = { 0, 0, 0, 0, 0, 0, 1 };
	Double_t max[nDim] = { 1, 1.5, 10, 10, 2, 1, 2 };

	// TODO mame problem s nulou, ked min a max je s rovnakym znamienkom
	FixBinning(nDim, bins, min, max);

	THn *hs = new THnF("hs", "hs", nDim, bins, min, max);
	for (Int_t i = 0; i < nDim; i++) {
		hs->GetAxis(i)->SetName(names[i]);
		hs->GetAxis(i)->SetTitle(titles[i]);
	}

	EmtMatrix *k = new EmtMatrix(hs);
	// 0=No Debug, 1=ear_max, 2 ear, 3 all
	k->SetDebug(0);
	k->Process();

//	TFile *f = TFile::Open("out.root","RECREATE");
//	hs->Write();

//		hs->GetAxis(3)->SetRange(1,1);
//	Int_t mybin = hs->GetAxis(4)->FindBin(0.06);
//	hs->GetAxis(3)->SetRange(mybin, mybin);
	//hs->GetAxis(3)->SetRangeUser(0.06, 0.07);
	// REAL

	TH1::AddDirectory(kFALSE);

	if (!is2D) {
		// IMAGINARY
		hs->GetAxis(6)->SetRange(2, 2);
		TCanvas *c1 = new TCanvas();
		for (Int_t i = 1; i <= hs->GetAxis(cut)->GetNbins(); i++) {
			hs->GetAxis(cut)->SetRange(i, i);
			hs->GetAxis(6)->SetRange(1, 1);
			TH1 * histNu01 = hs->Projection(projection);
			histNu01->SetStats(0);
			histNu01->SetTitle(
					TString::Format("%s for %s=%f",
							hs->GetAxis(projection)->GetTitle(),
							hs->GetAxis(cut)->GetTitle(),
							hs->GetAxis(cut)->GetBinUpEdge(i)).Data());
			;
			histNu01->GetXaxis()->SetTitle(hs->GetAxis(projection)->GetTitle());
			histNu01->GetXaxis()->CenterTitle();
			histNu01->GetYaxis()->SetTitle("eigen value");
			histNu01->GetYaxis()->CenterTitle();
			histNu01->GetYaxis()->SetRangeUser(0.0, 1.5);
			histNu01->Draw();
			if (save)
				c1->SaveAs(
						TString::Format("/tmp/%s-%s_%f.pdf",
								hs->GetAxis(projection)->GetName(),
								hs->GetAxis(cut)->GetName(),
								hs->GetAxis(cut)->GetBinUpEdge(i)).Data());
			else {
				c1->Update();
			}
		}
	} else {
		hs->GetAxis(2)->SetRange(1, 0);
		hs->GetAxis(3)->SetRange(1, 0);
		TCanvas *c3 = new TCanvas();
		TH2D * histNu01Nu10 = hs->Projection(3, 2);
		histNu01Nu10->SetStats(0);
		histNu01Nu10->GetXaxis()->SetTitle(hs->GetAxis(projection)->GetTitle());
		histNu01Nu10->GetXaxis()->CenterTitle();
		histNu01Nu10->GetYaxis()->SetTitle("eigen value");
		histNu01Nu10->GetYaxis()->CenterTitle();
		hs->GetAxis(6)->SetRange(1, 1);
		histNu01Nu10->Draw("CONT COL");
		if (save)
			c3->SaveAs(TString::Format("/tmp/%s-%s_2D.pdf",
					hs->GetAxis(projection)->GetName(),
					hs->GetAxis(cut)->GetName()).Data());
		else {
			c3->Update();
		}
	}
}
