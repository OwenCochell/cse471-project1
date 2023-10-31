#pragma once
#include "CombFilter.h"
#include "AllPassFilter.h"

class Reverb {
public:
    void Configure(int sampleRate);
    ~Reverb();
    double process(double input);

    void FromXML(IXMLDOMNode* xml);
private:
    CombFilter* m_combFilters[4];
    AllPassFilter* m_allPassFilters[2];
};
