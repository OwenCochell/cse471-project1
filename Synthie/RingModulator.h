#pragma once
#include <cmath> 

class RingModulator {
public:
    RingModulator(double modFrequency, int sampleRate);
    double process(double input);

private:
    double m_modFrequency;
    double m_phase;
    double m_phaseIncrement;
};