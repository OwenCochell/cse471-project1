#include "pch.h"
#include "CSample.h"

bool CSample::Generate()
{

    // Determine if we are past the end:

    if (sample >= wavetable->size()) {

        // We are done, just return

        return false;
    }

    // Determine the rate to increment:

    //double increment = (wavetable->size() / GetSampleRate()) * (freq / base_freq);
    double increment = (freq / base_freq);

    // Increment the sample:

    sample += increment;

    if (sample >= sustain_point) {

        // Otherwise, return to loop point:

        sample = start_point;
    }

    // Determine the actual index
 
    // Rounding Method:

    //int index = int(sample + 0.5);
    //double samp = this->wavetable->GetSamples()->at(index);

    // Linear Interpolation:

    double inbetween = fmod(sample, 1);
    double samp = 0;

    samp += (1. - inbetween) * this->wavetable->GetSamples()->at(int(sample));
    samp += inbetween * this->wavetable->GetSamples()->at(int(sample) + 1);

    // Set the sample:

    m_frame[0] = samp;
    m_frame[1] = samp;

    // Determine if we are done:

    if (done) {

        // Determine if we need to jump past the sustain point:

        if (sample <= sustain_point) {

            // Jump past sustain point + 1:

            sample = sustain_point + 1;
        }
    }

    return true;
}

void CSample::Start()
{
    // Set done value:

    done = false;

    // Set sample:

    sample = 0;

    // Determine if we should make the sustain point the stop:

    if (sustain_point == -1) {

        // Just set the sustain point to the end of the sample:

        sustain_point = wavetable->GetSamples()->size()-1;
    }
}
