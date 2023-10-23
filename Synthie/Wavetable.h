#pragma once

#include <vector>

#include "CInstrument.h"
#include "CSample.h"

class Wavetable : public CInstrument
{
public:

	Wavetable();

	virtual bool Generate();

	virtual void Start() { samp.Start(); }

	void SetFreq(double freq) { this->freq = freq; samp.SetFrequency(freq); };
	void SetSamp(CSample samp) { this->samp = samp; }

	virtual void SetNote(CNote* note) override;
private:

	// Sample component to utilize:
	CSample samp;

	// Frequency to output
	double freq;
};
