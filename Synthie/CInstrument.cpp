#include "pch.h"
#include "CInstrument.h"

bool CInstrument::Generate()
{
    // Determine time of beat:

    double tpb = GetTPB();

    GetAmplitudeEnvelope()->SetTimePerBeat(tpb);

    // Determine the amplitude:

    current_amplitude = GenerateEnvelopeAmp(m_time);

    // Update time
    m_time += GetSamplePeriod();

    return IsDone();
}

bool CInstrument::IsDone()
{
    // Determines if this voice is done:

    return m_time < GetDuration() * GetTPB();
}
