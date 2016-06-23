/*
 * EmtMatrix.cxx
 *
 *  Created on: May 13, 2016
 *      Author: zdicakova
 */
#include <TString.h>
#include <THn.h>
#include <TAxis.h>
#include <TMatrixDEigen.h>
#include "EmtMatrix.h"
ClassImp(EmtMatrix)
EmtMatrix::EmtMatrix(THn *h) :
		TObject(), fResults(h), fDebug(0) {
	Printf("Creating EmtMatrix ...");
}

EmtMatrix::~EmtMatrix() {
	Printf("Deleting EmtMatrix ...");
}

void EmtMatrix::Process() {
	if (!fResults)
		return;
	Printf("Doing Process 2 ...");

	Printf("Number of all bins : %lld", fResults->GetNbins());

	Int_t coord[fResults->GetNdimensions()];
	for (Int_t i = 0; i < fResults->GetNdimensions(); ++i) {
		coord[i] = 0;
	}
	Loop(fResults->GetNdimensions(), coord);

	fResults->Print();

//	Long64_t nBins = 1;

//	// Getting number of all bins
//	for (Int_t iDim = 0; iDim < fResults->GetNdimensions(); iDim++) {
//		nBins *= fResults->GetAxis(iDim)->GetNbins();
//	}
//	Printf("Number of all bins : %lld", nBins);
//	Int_t coord[fResults->GetNdimensions()];
//	for (Long64_t iBin = 0; iBin < fResults->GetNdimensions(); iBin++) {
//		fResults->SetBinContent(iBin,0);
//		fResults->GetBinContent(iBin, coord);
//		Printf("%d %d %d", coord[0],coord[1],coord[2]);
//	}

}

void EmtMatrix::Loop(Int_t nDim, Int_t *coord) {

	if (nDim > 0) {
		for (Int_t iBin = 0; iBin < fResults->GetAxis(nDim - 1)->GetNbins();
				iBin++) {
			coord[nDim - 1] = iBin + 1;
			if (nDim == 1) {
				Double_t val[fResults->GetNdimensions()];
				Double_t valCenter[fResults->GetNdimensions()];
				for (Int_t iDim = 0; iDim < fResults->GetNdimensions();
						iDim++) {
					val[iDim] = fResults->GetAxis(iDim)->GetBinUpEdge(
							coord[iDim]);
					valCenter[iDim] = fResults->GetAxis(iDim)->GetBinCenter(
							coord[iDim]);
					if (fDebug >= 3)
						printf("c=%d val=%f ", coord[iDim], val[iDim]);

				}
				if (fDebug >= 3)
					printf("\n");
				/// A TU ROBIME MATRIX
				FillEigenValues(fResults->GetNdimensions(), val, coord,
						valCenter);

			}
			Loop(nDim - 1, coord);

		}
	} else {
		return;
	}

//	Printf("%d %d %d %d", nDim, coord[0], coord[1], coord[2]);

}

void EmtMatrix::FillEigenValues(Int_t nDim, Double_t* input, Int_t *coord,
		Double_t *inputCenter) {

//	Double_t dr=input[0];
//	Double_t dnu=input[1];
//	Double_t rho=input[2];
//	Double_t gama=input[3];
//
//	Double_t r0x=1+dr;
//	Double_t r0y=2-dr;
//	Double_t nu01=0.1+dnu;
//	Double_t nu10=0.1-dnu;
//	Double_t mxy=0.01+dnu;
//	Double_t myx=0.01-dnu;

	Double_t r0x = input[0];
	Double_t r0y = input[1];
	Double_t nu01 = input[2];
	Double_t nu10 = input[3];
	Double_t mxy = input[4];
	Double_t myx = input[5];

	TMatrixD a(4, 4);
	a(0, 0) = r0x - nu01;
	a(0, 1) = nu10;
	a(0, 2) = 0;
	a(0, 3) = 0;
	a(1, 0) = nu01;
	a(1, 1) = -nu10 - mxy;
	a(1, 2) = 0;
	a(1, 3) = myx;
	a(2, 0) = 0;
	a(2, 1) = 0;
	a(2, 2) = r0y - nu01;
	a(2, 3) = nu10;
	a(3, 0) = 0;
	a(3, 1) = mxy;
	a(3, 2) = nu01;
	a(3, 3) = -nu10 - myx;

	TMatrixDEigen ea(a);

	const TVectorD ear = ea.GetEigenValuesRe();
//	ear.Print();
//	Printf("max je %f",ear.Max());
	const TVectorD eai = ea.GetEigenValuesIm();

	// TODO optimalize real and imaginary value calculation
	if (coord[nDim - 1] == 1) {
//		fResults->SetBinContent(fResults->GetBin(coord),ear.Max());
		fResults->Fill(inputCenter, ear.Max());
		if (fDebug >= 1)
			Printf("%d %f %f %f %f %f %f %f %f %f", nDim, input[0], input[1],
					input[2], input[3], input[4], input[5], input[6], ear.Max(),
					eai.Max());
		if (fDebug >= 2) {
			for (Int_t i = 0; i < 4; i++) {
				Printf("%f", ear[i]);
			}
		}
	}
//	else if (coord[nDim-1]==2) fResults->SetBinContent(coord,eai.Max());

}
