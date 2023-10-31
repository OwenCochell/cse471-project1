#include "pch.h"
#include "SimpleDelay.h"
void SimpleDelay::Configure(int maxDelaySamples)
{
    m_bufferSize = maxDelaySamples;
    m_index = 0;
    m_buffer = new double[m_bufferSize]();
}

SimpleDelay::~SimpleDelay() {
    delete[] m_buffer;
}

double SimpleDelay::process(double input, int delaySamples) {
    int readIndex = m_index - delaySamples;
    if (readIndex < 0) readIndex += m_bufferSize;

    double output = m_buffer[readIndex];
    m_buffer[m_index] = input;

    m_index = (m_index + 1) % m_bufferSize;

    return output;
}

void SimpleDelay::FromXML(IXMLDOMNode* xml)
{
    // Get a list of all attribute nodes and the
// length of that list
    CComPtr<IXMLDOMNamedNodeMap> attributes;
    xml->get_attributes(&attributes);
    long len;
    attributes->get_length(&len);

    // Attributes to set:

    double delay;

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

        if (name == L"maxdelay") {

            value.ChangeType(VT_I4);
            delay = value.intVal;
        }
    }

    // Configure ourselves:

    Configure(delay);
}
