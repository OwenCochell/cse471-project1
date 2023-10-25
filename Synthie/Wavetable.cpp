#include "pch.h"
#include "Wavetable.h"
#include "Notes.h"

Wavetable::Wavetable()
{
    SetDuration(0.1);
}

bool Wavetable::Generate()
{
    // Run the top level generate method:

    bool done = CInstrument::Generate();

    // Get the frequency:

    double nfreq = freq.DetermineFreq(GetTime());

    // Set the frequency:

    samp.SetFrequency(nfreq);

    // Generate the sample:

    samp.Generate();

    // Get the amplitude

    double amp = this->GetAmplitude();

    // Read the component's sample and make it our resulting frame.
    //m_frame[0] = amp * samp.Frame(0);
    //m_frame[1] = amp * samp.Frame(1);

    double val = amp * samp.Frame(0);

    m_frame[0] = val;
    m_frame[1] = val;

    return done;
}

void Wavetable::SetNote(CNote* note)
{

    // Send the note to the frequency component:
    
    freq.SetNote(note);

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

        if (name == "sample") {

            // We have the sample name, set the sample:

            auto sampc = GetSynth()->GetSampleMap()->at(value.bstrVal);

            samp.SetWaveContainer(sampc);
        }

        if (name == "base") {

            // We have the base frequency:

            samp.SetBaseFrequency(NoteToFrequency(value.bstrVal));
        }

        if (name == "start") {

            // Set the start value:

            value.ChangeType(VT_I4);
            samp.SetStartPoint(value.intVal);
        }

        if (name == "stop") {

            // Set stop value:

            value.ChangeType(VT_I4);
            samp.SetSustainPoint(value.intVal);
        }
    }
}
