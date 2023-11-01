#include "pch.h"
#include "CNote.h"
#include "xmlhelp.h"

void CNote::XmlLoad(IXMLDOMNode* xml,
    std::wstring& instrument, bool nomeasure)
{
    // Remember the xml node and the instrument.
    m_node = xml;
    m_instrument = instrument;

    // Get the parent node:

    xml->get_parentNode(&m_parent);

    // Get a list of all attribute nodes and the
    // length of that list
    CComPtr<IXMLDOMNamedNodeMap> attributes;
    xml->get_attributes(&attributes);
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

        // Get the value of the attribute.  
        CComVariant value;
        attrib->get_nodeValue(&value);

        if (name == "measure")
        {
            // The file has measures that start at 1.  
            // We'll make them start at zero instead.
            value.ChangeType(VT_I4);
            m_measure = value.intVal - 1;
        }
        else if (name == "beat")
        {
            // Same thing for the beats.
            value.ChangeType(VT_R8);
            m_beat = value.dblVal - 1;
        }

        else if (name == "gliss") {

            // We have a glissando,
            // so we pitch to the given note over our duration

            gnote = value.bstrVal;
        }

        else if (name == "note") {

            // Set our note:

            note = value.bstrVal;
        }

        else if (name == "duration") {

            // Set our duration:

            value.ChangeType(VT_R8);
            duration = value.dblVal;
        }
    }

    if (nomeasure) {
        m_measure = 0;
    }
}

bool CNote::operator<(const CNote& b)
{
    if (m_measure < b.m_measure)
        return true;
    if (m_measure > b.m_measure)
        return false;
    if (m_beat < b.m_beat)
        return true;

    return false;
}
