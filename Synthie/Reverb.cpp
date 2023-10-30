#include "Reverb.h"

Reverb::Reverb(int sampleRate) {
    m_combFilters[0] = new CombFilter(sampleRate * 0.0297);
    m_combFilters[1] = new CombFilter(sampleRate * 0.0371);
    m_combFilters[2] = new CombFilter(sampleRate * 0.0411);
    m_combFilters[3] = new CombFilter(sampleRate * 0.045);
    m_allPassFilters[0] = new AllPassFilter(sampleRate * 0.005);
    m_allPassFilters[1] = new AllPassFilter(sampleRate * 0.0017);
}

Reverb::~Reverb() {
    for (int i = 0; i < 4; i++) delete m_combFilters[i];
    for (int i = 0; i < 2; i++) delete m_allPassFilters[i];
}

double Reverb::process(double input) {
    double combSum = 0.0;
    for (int i = 0; i < 4; i++) {
        combSum += m_combFilters[i]->process(input);
    }
    double allPassOutput = m_allPassFilters[0]->process(combSum);
    allPassOutput = m_allPassFilters[1]->process(allPassOutput);
    return allPassOutput;
}