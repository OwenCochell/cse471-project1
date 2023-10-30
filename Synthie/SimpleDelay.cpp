#include "pch.h"
#include "SimpleDelay.h"
SimpleDelay::SimpleDelay(int maxDelaySamples)
    : m_bufferSize(maxDelaySamples), m_index(0)
{
    m_buffer = new double[m_bufferSize]();
}

SimpleDelay::~SimpleDelay() {
    delete[] m_buffer;
}

double SimpleDelay::process(double input, int delaySamples) {
    int readIndex = m_index - delaySamples;
    if (readIndex < 0) readIndex += m_bufferSize;

    double output = m_buffer[readIndex];
    m_buffer[m_index] = input;

    m_index = (m_index + 1) % m_bufferSize;

    return output;
}