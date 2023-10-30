#pragma once
#include "CombFilter.h"
#include "AllPassFilter.h"

class Reverb {
public:
    Reverb(int sampleRate);
    ~Reverb();
    double process(double input);

private:
    CombFilter* m_combFilters[4];
    AllPassFilter* m_allPassFilters[2];
};