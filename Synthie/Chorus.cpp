#include "pch.h"
#include "Chorus.h"
#define M_PI 3.14159265358979323846
Chorus::Chorus(int sampleRate) :
    m_sampleRate(sampleRate), m_rate(1.5), m_depth(0.002), m_phase(0.0)
{
    m_bufferSize = static_cast<int>(sampleRate * 0.05); // assuming max delay of 50ms
    m_delayBuffer = new double[m_bufferSize]();
    m_writeIndex = 0;
}

Chorus::~Chorus() {
    delete[] m_delayBuffer;
}

double Chorus::DelayedSignal(double frameValue) {
    // LFO modulation
    double delayTime = (1.0 + sin(m_phase) * m_depth) * m_sampleRate;
    m_phase += 2.0 * M_PI * m_rate / m_sampleRate;
    if (m_phase > 2.0 * M_PI) m_phase -= 2.0 * M_PI;

    int delaySamples = static_cast<int>(delayTime);
    int readIndex = m_writeIndex - delaySamples;
    if (readIndex < 0) readIndex += m_bufferSize;

    double delayed = m_delayBuffer[readIndex];
    m_delayBuffer[m_writeIndex] = frameValue;

    m_writeIndex++;
    if (m_writeIndex >= m_bufferSize) m_writeIndex = 0;

    return delayed;
}

void Chorus::Process(double* frameIn, double* frameOut) {
    double wetSignalL = DelayedSignal(frameIn[0]);
    double wetSignalR = DelayedSignal(frameIn[1]);

    frameOut[0] = 0.5 * (frameIn[0] + wetSignalL);  // Mixing dry and wet signals
    frameOut[1] = 0.5 * (frameIn[1] + wetSignalR);
}