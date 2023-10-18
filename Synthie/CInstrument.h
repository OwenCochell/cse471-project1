#pragma once
#include "CAudioNode.h"
#include "CNote.h"
#include "AmplitudeEnvelope.h"

class CInstrument :
    public CAudioNode
{
public:
    virtual void SetNote(CNote* note) = 0;

    AmplitudeEnvelope* GetAmplitudeEnvelope() { return &env; }

    double GenerateEnvelopeAmp(double time) { return env.DetermineAmplitude(time); }

private:

    AmplitudeEnvelope env;
};

