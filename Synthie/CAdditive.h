#pragma once
#include "CInstrument.h"
#include "CSineWave.h"
#include "FreqMange.h"
#include <vector>

class CAdditive :
    public CInstrument
{
public:
    CAdditive();

    virtual bool Generate();

    virtual void Start();

    virtual void SetNote(CNote* note) override;

private:
    std::vector<CSineWave> m_sinewaves1;

    std::vector<double> m_harmonics1;

    std::vector<CSineWave> m_sinewaves2;

    std::vector<double> m_harmonics2;

    FreqMange m_freq;
};

