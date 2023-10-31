#include "pch.h"
#include "RingModulator.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


void RingModulator::Configure(double modFrequency, int sampleRate)
{

    m_modFrequency = modFrequency;
    m_phase = 0.;
    m_phaseIncrement = 2.0 * M_PI * m_modFrequency / sampleRate;
}

double RingModulator::process(double input) {
    double modulator = sin(m_phase);
    m_phase += m_phaseIncrement;
    if (m_phase > 2.0 * M_PI) m_phase -= 2.0 * M_PI;

    return input * modulator;
}

void RingModulator::FromXML(IXMLDOMNode* xml)
{
    // Get a list of all attribute nodes and the
    // length of that list
    CComPtr<IXMLDOMNamedNodeMap> attributes;
    xml->get_attributes(&attributes);
    long len;
    attributes->get_length(&len);

    // Attributes to set:

    double freq;
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

        if (name == L"freq")
        {
            value.ChangeType(VT_R8);
            freq = value.dblVal;
        }

        else if (name == L"samp") {

            value.ChangeType(VT_I4);
            sr = value.intVal;
        }
    }

    // Configure ourselves:

    Configure(freq, sr);

}
