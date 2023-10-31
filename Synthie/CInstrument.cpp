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

void CInstrument::LoadEffects(IXMLDOMNode* xml)
{
    // Get a list of all attribute nodes and the
    // length of that list
    CComPtr<IXMLDOMNamedNodeMap> attributes;
    xml->get_attributes(&attributes);
    long len;
    attributes->get_length(&len);

    CComPtr<IXMLDOMNode> node;
    xml->get_firstChild(&node);
    for (; node != NULL; NextNode(node))
    {
        // Get the name of the node
        CComBSTR name;
        node->get_nodeName(&name);

        if (name == L"effect")
        {
            // Send effect to effect component
            LoadEffect(node);
        }
    }

    // Determine if we have no effects

    if (einfo.empty()) {

        // Add none effect:
        EffectInfo tinfo;
        tinfo.dry = 1;
        tinfo.wet = 1;
        tinfo.num = -1;

        einfo.push_back(tinfo);
    }
}

void CInstrument::LoadEffect(IXMLDOMNode* xml)
{
    // Get a list of all attribute nodes and the
    // length of that list
    CComPtr<IXMLDOMNamedNodeMap> attributes;
    xml->get_attributes(&attributes);
    long len;
    attributes->get_length(&len);

    // Attributes to set:

    int num;
    double dry;
    double wet;

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

        if (name == L"which")
        {
            std::wstring sname = value.bstrVal;

            // Determine num:

            num = Effects::NameToNum(sname);
        }
        else if (name == L"wet")
        {
            value.ChangeType(VT_R8);
            wet = value.dblVal;
        }

        else if (name == L"dry") {
            value.ChangeType(VT_R8);
            dry = value.dblVal;
        }
    }

    // Create effects info

    EffectInfo tinfo;
    tinfo.num = num;
    tinfo.dry = dry;
    tinfo.wet = wet;

    einfo.push_back(tinfo);
}
