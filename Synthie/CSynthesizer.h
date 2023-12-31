#pragma once

#include "msxml2.h"

#include <list>
#include <vector>
#include <map>
#include <memory>
#include "CNote.h"
#include "WaveContainer.h"
#include "Effects.h"

class CInstrument;

using namespace std;

class CSynthesizer
{
public:

    CSynthesizer() {
        m_channels = 2;
        m_sampleRate = 44100.;
        m_samplePeriod = 1 / m_sampleRate;

        m_bpm = 120;
        m_secperbeat = 0.5;
        m_beatspermeasure = 4;

    }

    //! Number of audio channels
    int GetNumChannels() { return m_channels; }

    //! Sample rate in samples per second
    double GetSampleRate() { return m_sampleRate; }

    //! Sample period in seconds (1/samplerate)
    double GetSamplePeriod() { return m_samplePeriod; }

    //! Set the number of channels
    void SetNumChannels(int n) { m_channels = n; }

    //! Set the sample rate
    void SetSampleRate(double s) { m_sampleRate = s;  m_samplePeriod = 1.0 / s; }

    //! Get the time since we started generating audio
    double GetTime() { return m_time; }

    void Start(void);
    void Clear(void);

    std::map<wstring, std::shared_ptr<WaveContainer>>* GetSampleMap() { return &(this->sample_map); }

    void OpenScore(CString& filename);

    bool CSynthesizer::Generate(double* frame);

private:
    void XmlLoadScore(IXMLDOMNode* xml);
    void XmlLoadInstrument(IXMLDOMNode* xml);
    void XmlLoadNote(IXMLDOMNode* xml, std::wstring& instrument);
    void XmlLoadSamples(IXMLDOMNode* xml);
    void XmlLoadSample(IXMLDOMNode* xml);
    void XmlLoadEffects(IXMLDOMNode* xml);

    double  m_bpm;                  //!< Beats per minute
    int     m_beatspermeasure;  //!< Beats per measure
    double  m_secperbeat;        //!< Seconds per beat

    int m_currentNote;          //!< The current note we are playing
    int m_measure;              //!< The current measure
    double m_beat;              //!< The current beat within the measure

    int		m_channels;
    double	m_sampleRate;
    double	m_samplePeriod;
    double m_time;
    std::list<CInstrument*>  m_instruments;
    std::vector<CNote> m_notes;

    bool nomeasure = false;

    // A map of samples loaded for use by other components
    std::map<wstring, std::shared_ptr<WaveContainer>> sample_map = {};

    // Effects to utilize
    Effects effect;
};

#pragma comment(lib, "msxml2.lib")
