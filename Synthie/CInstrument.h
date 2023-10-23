#pragma once
#include "CAudioNode.h"
#include "CNote.h"
#include "AmplitudeEnvelope.h"

class CInstrument :
    public CAudioNode
{
public:

    virtual void SetNote(CNote* note) = 0;

    void SetDuration(double d) { m_duration = d; GetAmplitudeEnvelope()->SetDuration(d); }
    double GetDuration() const { return this->m_duration; }

    double GetAmplitude() const { return this->current_amplitude; }

    AmplitudeEnvelope* GetAmplitudeEnvelope() { return &env; }

    double GenerateEnvelopeAmp(double time) { return env.DetermineAmplitude(time); }

    virtual bool Generate() override;

    bool IsDone();
    double GetTPB() { return 1 / (this->GetBPM() / 60); }
private:

    AmplitudeEnvelope env;

    // Duration of this instrument
    double m_duration;

    // Current time of this instrument
    double m_time = 0;

    // Amplitude of the last envelope operation
    double current_amplitude = 0;
};
