#pragma once

#include <vector>
#include <memory>

#include "CAudioNode.h"
#include "WaveContainer.h"

/**
* Manages samples and preforms looping, pitching, etc/.
*/
class CSample : public CAudioNode
{
public:

	CSample() = default;

	CSample(std::shared_ptr<WaveContainer> wav) : wavetable(wav) {}

	/**
	* Generates a sample.
	* 
	* We do all the necessary steps for sample eneration here.
	*/
	virtual bool Generate() override;

	/**
	* Starts this sampler,
	* we simply load all samples from the CWave object.
	* 
	* This method should be called before this class is utilized.
	*/
	void Start();

	/**
	* Tells this sampler to stop,
	* we play out the rest of the wave after the loop points.
	*/
	void Stop() { this->done = true; }

	int GetStartPoint() const { return start_point; }
	void SetStartPoint(int start) { this->start_point = start; }

	int GetSustainPoint() const { return this->sustain_point; }
	void SetSustainPoint(int sustain) { this->sustain_point = sustain; }

	double GetBaseFrequency() const { return this->base_freq; }
	void SetBaseFrequency(double freq) { this->base_freq = freq; }

	double GetFrequency() const { return this->freq; }
	void SetFrequency(double freq) { this->freq = freq; }

	void SetWaveContainer(std::shared_ptr<WaveContainer> wav) { wavetable = wav; }
private:

	// Table of samples
	std::shared_ptr<WaveContainer> wavetable;

	// Loop Points

	// Sample the start section ends
	int start_point = 0;

	// Sample the sustain section ends
	int sustain_point = 0;

	// Frequency of sampled wave
	double base_freq = 0;

	// Desiered pitch
	double freq = 0;

	// Current sample we are on
	double sample = 0;

	// Determines if we are done playing and need to exit
	bool done = false;
};
