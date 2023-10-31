#include "pch.h"
#include "AllPassFilter.h"

AllPassFilter::AllPassFilter(int delaySamples)
    : m_bufferSize(delaySamples), m_index(0)
{
    m_buffer = new double[m_bufferSize]();
}

AllPassFilter::~AllPassFilter() {
    delete[] m_buffer;
}

double AllPassFilter::process(double input) {
    double bufferedSample = m_buffer[m_index];
    double output = -input + bufferedSample;
    m_buffer[m_index] = input + (bufferedSample * 0.5); // 0.5 is feedback coefficient
    m_index = (m_index + 1) % m_bufferSize;
    return output;
}