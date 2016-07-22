#include <TH1.h>
#include <TH2.h>
void Draw(Int_t projection = 2, Int_t cut = 3,Double_t *cuts=0, Bool_t is2D = kFALSE,
		Bool_t save = kFALSE,const char *inputFileName="histogram.root") {
	gROOT->LoadMacro("EmtMatrix.cxx+");

	TFile *f = TFile::Open(inputFileName,"READ");
	if (!f) return;

	THn *hs = (THn*) f->Get("hs");
	if (!hs) return;

	if (!cuts) {
		cuts = new Double_t[hs->GetNdimensions()-1];
		cuts[0] = 0.0; // r0x
		cuts[1] = 0.0; // r0y
		cuts[2] = 0.0; // nu01
		cuts[3] = 0.0; // nu10
		cuts[4] = 0.0; // mxy
		cuts[5] = 0.0; // myx
	}

	TH1::AddDirectory(kFALSE);

	// Setting up fixed parameters
	for (Int_t iCut =0; iCut < hs->GetNdimensions()-1; iCut++) {
		if (iCut == projection) continue;
		if (iCut == cut) continue;
		if (hs->GetAxis(iCut)->GetNbins()<2) continue;
		Printf("Cutting at cuts[%d]=%f",iCut,cuts[iCut]);
		Int_t mybin = hs->GetAxis(iCut)->FindBin(cuts[iCut]);
		hs->GetAxis(iCut)->SetRange(mybin, mybin);
	}


	if (!is2D) {
		// REAL
		hs->GetAxis(6)->SetRange(1, 1);
		// IMAGINARY
        // hs->GetAxis(6)->SetRange(2, 2);

		TCanvas *c1 = new TCanvas();
		for (Int_t i = 1; i <= hs->GetAxis(cut)->GetNbins(); i++) {
			hs->GetAxis(cut)->SetRange(i, i);

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
