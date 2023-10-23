#pragma once
#include "CInstrument.h"
#include "CSineWave.h"

class CToneInstrument :
    public CInstrument
{
public:

    CToneInstrument();
    virtual void Start();
    virtual bool Generate();

    void SetFreq(double f) { m_sinewave.SetFreq(f); }
    void SetAmplitude(double a) { m_sinewave.SetAmplitude(a); }

    virtual void SetNote(CNote* note) override;

private:
    CSineWave   m_sinewave;

};

