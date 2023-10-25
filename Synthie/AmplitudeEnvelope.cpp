#include "pch.h"
#include "AmplitudeEnvelope.h"

#include <cmath>

AmplitudeEnvelope::AmplitudeEnvelope() {

	duration = 0;
	attack = 0.05;
	release = 0.05;
}

double AmplitudeEnvelope::Ramp(double time, double startt, double stopt, double startv, double stopv) {

	return startv + (stopv - startv) * (time - startt) / (stopt - startt);
}

double AmplitudeEnvelope::DetermineAmplitude(double time) {

	double tduration = duration * tpb;

	// Determine if our time is before our attack time:

	if (std::abs(tduration - time) <= release) {

		// We must ramp to zero:

		return Ramp(time, tduration - release, tduration, 1, 0);

	}

	if (time <= attack) {

		// We must ramp up to 1:

		return Ramp(time, 0, attack, 0, 1);

	}

	// Otherwise, return sustain:
	return this->sustain;
}

double LinearRamp::DetermineAmplitude(double time)
{

	// Determine if our time is done:

	if (time >= GetStopTime()) {
		// Just return stop value:

		return GetStopValue();
	}
	return GetStartValue() + (GetStopValue() - GetStartValue()) * (time - GetStartTime()) / (GetStopTime() - GetStartTime());
}

double ConstantEnvelope::DetermineAmplitude(double time)
{
	// Just return the start value always:

	return GetStartValue();
}
