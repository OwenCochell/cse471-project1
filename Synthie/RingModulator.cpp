#include "pch.h"
#include "RingModulator.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


RingModulator::RingModulator(double modFrequency, int sampleRate)
    : m_modFrequency(modFrequency), m_phase(0.0)
{
    m_phaseIncrement = 2.0 * M_PI * m_modFrequency / sampleRate;
}

double RingModulator::process(double input) {
    double modulator = sin(m_phase);
    m_phase += m_phaseIncrement;
    if (m_phase > 2.0 * M_PI) m_phase -= 2.0 * M_PI;

    return input * modulator;
}