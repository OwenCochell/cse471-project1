#include "pch.h"
#include "WaveContainer.h"

void WaveContainer::FromCWave(CWaveIn& wav)
{
	// Set our samplerate:

	this->sample_rate = wav.SampleRate();

	// Grab the channels:

	this->channel = wav.NumChannels();

	// Reserve the vector:

	this->samples.reserve(wav.NumSampleFrames());

	// Iterate over frames:

	for (int i = 0; i < wav.NumSampleFrames(); ++i) {

		// Define the frame:
		short frame = 0;

		// Read the current frame:

		wav.ReadFrame(&frame);

		// Add frame to vector:

		this->samples.push_back(frame);
	}
}
