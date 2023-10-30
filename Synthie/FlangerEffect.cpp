#include "pch.h"
#include "FlangerEffect.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

FlangerEffect::FlangerEffect(int sampleRate)
    : m_sampleRate(sampleRate), m_rate(0.5), m_depth(0.002), m_phase(0.0)
{
    m_delay = new SimpleDelay(static_cast<int>(sampleRate * 0.02)); // max delay of 20ms
}

FlangerEffect::~FlangerEffect() {
    delete m_delay;
}

double FlangerEffect::process(double input) {
    double delayTime = (1.0 + sin(m_phase) * m_depth) * m_sampleRate * 0.02; // 0.02 to get max of 20ms delay
    m_phase += 2.0 * M_PI * m_rate / m_sampleRate;
    if (m_phase > 2.0 * M_PI) m_phase -= 2.0 * M_PI;

    double delayed = m_delay->process(input, static_cast<int>(delayTime));
    return (input + delayed) * 0.5;  // Mixing input and delayed signal
}
