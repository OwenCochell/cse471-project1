#include "pch.h"
#include "CombFilter.h"

CombFilter::CombFilter(int delaySamples)
    : m_bufferSize(delaySamples), m_index(0)
{
    m_buffer = new double[m_bufferSize]();
}

CombFilter::~CombFilter() {
    delete[] m_buffer;
}

double CombFilter::process(double input) {
    double output = m_buffer[m_index];
    m_buffer[m_index] = input + 0.5 * output;  // 0.5 is a damping factor
    m_index = (m_index + 1) % m_bufferSize;
    return output;
}