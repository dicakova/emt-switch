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

void Calculate(const char *output="histogram.root") {
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
	Int_t bins[nDim] = { 5, 5, 101, 101, 1, 1, 2 };
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

	TFile *f = TFile::Open(output,"RECREATE");
	hs->Write();
}
