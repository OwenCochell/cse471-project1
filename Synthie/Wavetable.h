#pragma once

#include <vector>

#include "CInstrument.h"

class Wavetable : public CInstrument
{
public:

	virtual bool Generate();

	void SetFreq(double freq) { this->freq = freq; };

	virtual void SetNote(CNote* note) override;
private:

	// Table of samples
	std::vector<short> wavetable;

	// Loop Points

	// Sample the start section ends
	int start_point;

	// Sample the sustain sectio  ends
	int sustain_point;

	// Frequency to output
	double freq;

	// Current sample we are on
	int curr;
};
