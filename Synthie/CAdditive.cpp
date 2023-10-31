#include "pch.h"
#include "CAdditive.h"

#include <sstream>
#include <iterator>

std::string ConvertWCSToMBS(const wchar_t* pstr, long wslen)
{
    int len = ::WideCharToMultiByte(CP_ACP, 0, pstr, wslen, NULL, 0, NULL, NULL);

    std::string dblstr(len, '\0');
    len = ::WideCharToMultiByte(CP_ACP, 0 /* no flags */,
        pstr, wslen /* not necessary NULL-terminated */,
        &dblstr[0], len,
        NULL, NULL /* no default char */);

    return dblstr;
}

std::string ConvertBSTRToMBS(BSTR bstr)
{
    int wslen = ::SysStringLen(bstr);
    return ConvertWCSToMBS((wchar_t*)bstr, wslen);
}

CAdditive::CAdditive()
{
    SetDuration(0.1);
}

void CAdditive::Start()
{
    for (int i = 0; i < m_harmonics1.size(); i++) {

        m_sinewaves1.push_back(CSineWave());
        m_sinewaves1[i].SetSampleRate(GetSampleRate());
        m_sinewaves1[i].SetFreq(m_freq.GetBaseFreq() * (i + 1));
        m_sinewaves1[i].SetAmplitude(m_harmonics1[i]);
        m_sinewaves1[i].Start();
    }

    for (int i = 0; i < m_harmonics2.size(); i++) {

        m_sinewaves2.push_back(CSineWave());
        m_sinewaves2[i].SetSampleRate(GetSampleRate());
        m_sinewaves2[i].SetFreq(m_freq.GetBaseFreq() * (i + 1));
        m_sinewaves2[i].SetAmplitude(m_harmonics2[i]);
        m_sinewaves2[i].Start();
    }
}

void CAdditive::SetNote(CNote* note)
{
    CInstrument::LoadEffects(note->PNode());

    // Send the note to the frequency component:

    m_freq.SetNote(note);

    // Get a list of all attribute nodes and the
    // length of that list
    CComPtr<IXMLDOMNamedNodeMap> attributes;
    note->Node()->get_attributes(&attributes);
    long len;
    attributes->get_length(&len);

    // Loop over the list of attributes
    for (int i = 0; i < len; i++)
    {
        // Get attribute i
        CComPtr<IXMLDOMNode> attrib;
        attributes->get_item(i, &attrib);

        // Get the name of the attribute
        CComBSTR name;
        attrib->get_nodeName(&name);

        // Get the value of the attribute.  A CComVariant is a variable
        // that can have any type. It loads the attribute value as a
        // string (UNICODE), but we can then change it to an integer 
        // (VT_I4) or double (VT_R8) using the ChangeType function 
        // and then read its integer or double value from a member variable.
        CComVariant value;
        attrib->get_nodeValue(&value);

        if (name == "duration")
        {
            value.ChangeType(VT_R8);
            SetDuration(value.dblVal);
        }
    }

    // Get list of attributes for the parent:
    CComPtr<IXMLDOMNamedNodeMap> pattributes;
    note->PNode()->get_attributes(&pattributes);
    long plen;
    pattributes->get_length(&plen);

    for (int i = 0; i < plen; i++) {

        // Get attribute i
        CComPtr<IXMLDOMNode> attrib;
        pattributes->get_item(i, &attrib);

        // Get the name of the attribute
        CComBSTR name;
        attrib->get_nodeName(&name);

        // Get the value of the attribute.  A CComVariant is a variable
        // that can have any type. It loads the attribute value as a
        // string (UNICODE), but we can then change it to an integer 
        // (VT_I4) or double (VT_R8) using the ChangeType function 
        // and then read its integer or double value from a member variable.
        CComVariant value;
        attrib->get_nodeValue(&value);

        if (name == "harmonic") {

            // We have the harmonics, iterate over the space-seperated list:

            std::string harm = ConvertBSTRToMBS(value.bstrVal);

            std::istringstream iss(harm);
            std::string item;

            while (std::getline(iss, item, ' ')) {

                // Convert string to number:

                double num = stod(item);

                m_harmonics1.push_back(num);
            }
        }

        if (name == "harmonic2") {

            // We have the harmonics, iterate over the space-seperated list:

            std::string harm = ConvertBSTRToMBS(value.bstrVal);

            std::istringstream iss(harm);
            std::string item;

            while (std::getline(iss, item, ' ')) {

                // Convert string to number:

                double num = stod(item);

                m_harmonics2.push_back(num);
            }
        }
    }
}

bool CAdditive::Generate()
{
    m_frame[0] = 0;
    m_frame[1] = 0;
    
    // Run the top level generate method:

    bool done = CInstrument::Generate();

    // Generate current frequency:

    double freq = m_freq.DetermineFreq(GetTime());

    double frame1[2] = { 0., 0. };
    double frame2[2] = { 0., 0. };

    // Tell the component to generate an audio sample
    // when implementing glissando, change frequency before generating sine wave
    for (int i = 0; i < m_sinewaves1.size(); i++) {

        // Determine frequency for this sine wave:

        double sfreq = freq * (i + 1);

        // Set the sine wave frequency:

        m_sinewaves1[i].SetFreq(sfreq);

        m_sinewaves1[i].Generate();
        frame1[0] += m_sinewaves1[i].Frame(0);
        frame1[1] += m_sinewaves1[i].Frame(1);
    }

    // Tell the component to generate an audio sample
    // when implementing glissando, change frequency before generating sine wave
    for (int i = 0; i < m_sinewaves2.size(); i++) {

        // Determine frequency for this sine wave:

        double sfreq = freq * (i + 1);

        // Set the sine wave frequency:

        m_sinewaves2[i].SetFreq(sfreq);

        m_sinewaves2[i].Generate();
        frame2[0] += m_sinewaves2[i].Frame(0);
        frame2[1] += m_sinewaves2[i].Frame(1);
    }

    // Interpolate between the two:

    double inbetween = GetTime() / GetDuration();
    double samp = 0;

    m_frame[0] += (1. - inbetween) * frame1[0];
    m_frame[1] += (1. - inbetween) * frame1[1];

    m_frame[0] += inbetween * frame2[0];
    m_frame[1] += inbetween * frame2[1];

    // Get the amplitude
    double amp = this->GetAmplitude();

    // Read the component's sample and make it our resulting frame.
    m_frame[0] = amp * m_frame[0];
    m_frame[1] = amp * m_frame[1];

    // We return if we are done.
    return done;
}
