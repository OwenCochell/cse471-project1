#include "pch.h"
#include "Chorus.h"
#define M_PI 3.14159265358979323846

#include <algorithm>

void Chorus::Configure(int sampleRate)
{
    m_sampleRate = sampleRate;
    m_rate = 1.5;
    m_depth = 0.002;
    m_phase = 0.0;
    m_bufferSize = static_cast<int>(sampleRate * 0.05); // assuming max delay of 50ms
    m_delayBuffer.reserve(m_bufferSize);

    // Fill buffer with zeros:

    for (int i = 0; i < m_bufferSize; ++i) {
        m_delayBuffer.push_back(0);
    }
    m_writeIndex = 0;
}

double Chorus::DelayedSignal(double frameValue) {
    // LFO modulation
    double delayTime = (1.0 + sin(m_phase) * m_depth) * m_sampleRate;
    m_phase += 2.0 * M_PI * m_rate / m_sampleRate;
    if (m_phase > 2.0 * M_PI) m_phase -= 2.0 * M_PI;

    int delaySamples = static_cast<int>(delayTime);
    int readIndex = m_writeIndex - delaySamples;
    if (readIndex < 0) readIndex += m_bufferSize;

    double delayed = m_delayBuffer[readIndex];
    m_delayBuffer[m_writeIndex] = frameValue;

    m_writeIndex++;
    if (m_writeIndex >= m_bufferSize) m_writeIndex = 0;

    return delayed;
}

double Chorus::Process(double frameIn) {
    double wetSignalL = DelayedSignal(frameIn);

    return 0.5 * (frameIn + wetSignalL);  // Mixing dry and wet signals
}

void Chorus::FromXML(IXMLDOMNode* xml)
{
    // Get a list of all attribute nodes and the
// length of that list
    CComPtr<IXMLDOMNamedNodeMap> attributes;
    xml->get_attributes(&attributes);
    long len;
    attributes->get_length(&len);

    // Attributes to set:

    double sr;

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

        if (name == L"samp") {

            value.ChangeType(VT_I4);
            sr = value.intVal;
        }
    }

    // Configure ourselves:

    Configure(sr);
}
