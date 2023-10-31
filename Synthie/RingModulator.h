#pragma once
#include <cmath> 

class RingModulator {
public:
    void Configure(double modFrequency, int sampleRate);
    double process(double input);

    void FromXML(IXMLDOMNode* xml);

private:
    double m_modFrequency;
    double m_phase;
    double m_phaseIncrement;
};
