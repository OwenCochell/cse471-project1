#include "pch.h"
#include "WaveContainer.h"

void WaveContainer::FromCWave(CWaveIn& wav)
{
	// Set our samplerate:

	this->sample_rate = wav.SampleRate();

	// Grab the channels:

	this->channel = wav.NumChannels();

	// Get sample size:
	// TODO: If you find a weird wave file, utilize this value:
	// mult = (2^(sample_size)) / 2

	this->sample_size = wav.SampleSize();

	// Reserve the vector:

	this->samples.reserve(wav.NumSampleFrames());

	// Iterate over frames:

	for (int i = 0; i < wav.NumSampleFrames(); ++i) {

		// Define the frame:
		short frame = 0;

		// Read the current frame:

		wav.ReadFrame(&frame);

		// Add frame to vector:

		this->samples.push_back(double(frame) / 32767.0);
	}
}

void WaveContainer::ToCWave(CWaveOut& wav)
{
	// Set our samplerate:

	wav.SampleRate(this->sample_rate);

	// Set the channels:

	wav.NumChannels(this->channel);

	// Set sample size:

	wav.SampleSize(this->sample_size);

	// Write the frames:

	for (int i = 0; i < this->samples.size(); ++i) {

		// Write the frame:

		short frame = short(this->samples[i] * 32767);

		wav.WriteFrame(&(frame));
	}
}
