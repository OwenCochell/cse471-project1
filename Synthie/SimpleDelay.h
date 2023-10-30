#pragma once

class SimpleDelay {
public:
    SimpleDelay(int maxDelaySamples);
    ~SimpleDelay();
    double process(double input, int delaySamples);

private:
    double* m_buffer;
    int m_bufferSize;
    int m_index;
};