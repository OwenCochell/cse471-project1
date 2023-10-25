#include "pch.h"
#include "FreqMange.h"

#include <utility>

double FreqMange::DetermineFreq(double time)
{
	// Run through our envelope:

	return env->DetermineAmplitude(time);
}

void FreqMange::SetNote(CNote* note)
{
	// Set the note object:

	this->note = note;

	// Determine if we are a glissando:

	if (note->HasGliss()) {

		// We have glissando, configure a LinearRamp:

		auto penv = std::make_unique<LinearRamp>();

		// Configure the envelope:

		penv->SetStartValue(note->Freq());
		penv->SetStopValue(note->GFreq());
		penv->SetStartTime(0);
		penv->SetStopTime(note->Duration());

		// Set the envelope:

		this->env = std::move(penv);
	}

	// We do not have a glissando:

	else {

		// Just use a constant envelope:

		auto penv = std::make_unique<ConstantEnvelope>();

		// Configure the envelope:

		penv->SetStartValue(note->Freq());

		// Set the envelope:

		this->env = std::move(penv);
	}
}
