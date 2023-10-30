#pragma once
#include "SimpleDelay.h"
#include <cmath> 

class FlangerEffect {
public:
    FlangerEffect(int sampleRate);
    ~FlangerEffect();
    double process(double input);

private:
    SimpleDelay* m_delay;
    double m_rate;
    double m_depth;
    double m_phase;
    int m_sampleRate;
};