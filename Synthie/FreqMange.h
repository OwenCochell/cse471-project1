#pragma once

#include <memory>

#include "AmplitudeEnvelope.h"
#include "CNote.h"

/**
* This class manages frequency info for an instrument.
* 
* We will automatically determine if we have a constant frequency,
* or if we have a glissando.
* 
* If we have a glissando, then we will ramp to the value.
*/
class FreqMange
{
public:

	double DetermineFreq(double time);

	void SetNote(CNote* note);

private:

	// CNote we will extract info from
	CNote* note;

	// Envelope we will utilize:
	std::unique_ptr<BaseEnvelope> env;
};
