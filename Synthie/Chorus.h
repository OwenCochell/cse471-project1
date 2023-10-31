#pragma once

#include <vector>

class Chorus {
public:
    void Configure(int sampleRate);

    double Process(double frameIn);

    void FromXML(IXMLDOMNode* xml);

private:
    double DelayedSignal(double frameValue);

    std::vector<double> m_delayBuffer;
    int m_bufferSize;
    int m_writeIndex;

    double m_rate;    // LFO rate
    double m_depth;   // LFO depth
    double m_phase;   // LFO current phase

    int m_sampleRate;
};
