/*
 * EmtMatrix.h
 *
 *  Created on: May 13, 2016
 *      Author: zdicakova
 */

#ifndef EMT_EMTMATRIX_H_
#define EMT_EMTMATRIX_H_
#include <TObject.h>
//class THn;
class EmtMatrix : public TObject {
public:
	EmtMatrix(THn *h = 0);
	virtual ~EmtMatrix();

	void Process();
	void Loop(Int_t nDim,Int_t *coord);
	void FillEigenValues(Int_t nDim, Double_t *input, Int_t *coord,Double_t *inputCenter);

	void SetDebug(Int_t debugLevel=1) { fDebug = debugLevel; }
private:
	THn *fResults;
	Int_t fDebug;

	ClassDef(EmtMatrix,1)
};

#endif /* EMT_EMTMATRIX_H_ */
