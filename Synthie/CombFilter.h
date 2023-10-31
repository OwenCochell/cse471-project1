#pragma once

class CombFilter {
public:
    CombFilter(int delaySamples);
    ~CombFilter();
    double process(double input);

private:
    double* m_buffer;
    int m_bufferSize;
    int m_index;
};
