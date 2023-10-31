#pragma once

class AllPassFilter {
public:
    AllPassFilter(int delaySamples);
    ~AllPassFilter();
    double process(double input);

private:
    double* m_buffer;
    int m_bufferSize;
    int m_index;
};
