#pragma once

class Chorus {
public:
    Chorus(int sampleRate);
    ~Chorus();

    void Process(double* frameIn, double* frameOut);

private:
    double DelayedSignal(double frameValue);

    double* m_delayBuffer;
    int m_bufferSize;
    int m_writeIndex;

    double m_rate;    // LFO rate
    double m_depth;   // LFO depth
    double m_phase;   // LFO current phase

    int m_sampleRate;
};