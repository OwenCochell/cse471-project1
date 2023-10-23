#include "pch.h"
#include "CToneInstrument.h"
#include "Notes.h"

CToneInstrument::CToneInstrument(void)
{
    SetDuration(0.1);
}

void CToneInstrument::Start()
{
    m_sinewave.SetSampleRate(GetSampleRate());
    m_sinewave.Start();
}


bool CToneInstrument::Generate()
{

    // Run the top level generate method:
    
    bool done = CInstrument::Generate();

    // Tell the component to generate an audio sample
    m_sinewave.Generate();

    // Get the amplitude

    double amp = this->GetAmplitude();

    // Read the component's sample and make it our resulting frame.
    m_frame[0] = amp * m_sinewave.Frame(0);
    m_frame[1] = amp * m_sinewave.Frame(1);

    // We return if we are done.
    return done;
}

void CToneInstrument::SetNote(CNote* note)
{
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

        else if (name == "note")
        {
            SetFreq(NoteToFrequency(value.bstrVal));
        }

    }

}
