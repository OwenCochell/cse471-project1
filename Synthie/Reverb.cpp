#include "pch.h"
#include "Reverb.h"

void Reverb::Configure(int sampleRate) {
    m_combFilters[0] = new CombFilter(sampleRate * 0.0297);
    m_combFilters[1] = new CombFilter(sampleRate * 0.0371);
    m_combFilters[2] = new CombFilter(sampleRate * 0.0411);
    m_combFilters[3] = new CombFilter(sampleRate * 0.045);
    m_allPassFilters[0] = new AllPassFilter(sampleRate * 0.005);
    m_allPassFilters[1] = new AllPassFilter(sampleRate * 0.0017);
}

Reverb::~Reverb() {
    for (int i = 0; i < 4; i++) delete m_combFilters[i];
    for (int i = 0; i < 2; i++) delete m_allPassFilters[i];
}

double Reverb::process(double input) {
    double combSum = 0.0;
    for (int i = 0; i < 4; i++) {
        combSum += m_combFilters[i]->process(input);
    }
    double allPassOutput = m_allPassFilters[0]->process(combSum);
    allPassOutput = m_allPassFilters[1]->process(allPassOutput);
    return allPassOutput;
}

void Reverb::FromXML(IXMLDOMNode* xml)
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

        if (name == L"samp") {

            value.ChangeType(VT_I4);
            sr = value.intVal;
        }
    }

    // Configure ourselves:

    Configure(sr);
}
