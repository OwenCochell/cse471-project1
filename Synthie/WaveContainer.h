#pragma once

#include <vector>

#include "audio/Wave.h"

/**
* A class that holds sample data.
* 
* This should be shared between multiple containers
*/
class WaveContainer
{
public:

	double GetSampleRate() const { return this->sample_rate; }
	void SetSampleRate(double samp) { this->sample_rate = samp; }

	int GetNumChannels() const { return this->channel; }

	const std::vector<double>* GetSamples() const { return &(this->samples); }

	int size() { return this->samples.size(); }

	/**
	* Configures ourselves using the given Wave object
	*/
	void FromCWave(CWaveIn& wav);

	void ToCWave(CWaveOut& wav);

private:

	// Sample rate of the audio
	double sample_rate;

	// Samples of audio data
	std::vector<double> samples;

	// Number of channels in this container
	int channel;

	// Size of each sample
	int sample_size;
};
