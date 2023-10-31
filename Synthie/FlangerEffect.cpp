#include "pch.h"
#include "FlangerEffect.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void FlangerEffect::Configure(int sampleRate)
{
    m_sampleRate = sampleRate;
    m_rate = 0.5;
    m_depth = 0.002;
    m_phase = 0.0;
    m_delay = new SimpleDelay(); // max delay of 20ms
    m_delay->Configure(static_cast<int>(sampleRate * 0.02));
}

FlangerEffect::~FlangerEffect() {
    delete m_delay;
}

double FlangerEffect::process(double input) {
    double delayTime = (1.0 + sin(m_phase) * m_depth) * m_sampleRate * 0.02; // 0.02 to get max of 20ms delay
    m_phase += 2.0 * M_PI * m_rate / m_sampleRate;
    if (m_phase > 2.0 * M_PI) m_phase -= 2.0 * M_PI;

    double delayed = m_delay->process(input, static_cast<int>(delayTime));
    return (input + delayed) * 0.5;  // Mixing input and delayed signal
}

void FlangerEffect::FromXML(IXMLDOMNode* xml)
{
    // Get a list of all attribute nodes and the
// length of that list
    CComPtr<IXMLDOMNamedNodeMap> attributes;
    xml->get_attributes(&attributes);
    long len;
    attributes->get_length(&len);

    // Attributes to set:

    int sr;

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
