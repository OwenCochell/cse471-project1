#include "pch.h"
#include "CSynthesizer.h"
#include "CToneInstrument.h"
#include "xmlhelp.h"
#include "audio/Wave.h"
#include "Wavetable.h"
#include "CAdditive.h"

#include <algorithm>
#include <utility>

//! Start the synthesizer
void CSynthesizer::Start(void)
{
    m_instruments.clear();
    m_currentNote = 0;
    m_measure = 0;
    m_beat = 0;
    m_time = 0;

}

void CSynthesizer::Clear(void)
{
    m_instruments.clear();
    m_notes.clear();
}

void CSynthesizer::XmlLoadInstrument(IXMLDOMNode* xml)
{
    wstring instrument = L"";

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

        if (name == "instrument")
        {
            instrument = value.bstrVal;
        }
    }


    CComPtr<IXMLDOMNode> node;
    xml->get_firstChild(&node);
    for (; node != NULL; NextNode(node))
    {
        // Get the name of the node
        CComBSTR name;
        node->get_nodeName(&name);

        if (name == L"note")
        {
            XmlLoadNote(node, instrument);
        }
    }

}

void CSynthesizer::XmlLoadNote(IXMLDOMNode* xml, std::wstring& instrument)
{
    m_notes.push_back(CNote());
    m_notes.back().XmlLoad(xml, instrument, nomeasure);

}

void CSynthesizer::XmlLoadSamples(IXMLDOMNode* xml)
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

        if (name == L"sample")
        {
            XmlLoadSample(node);
        }
    }
}

void CSynthesizer::XmlLoadSample(IXMLDOMNode* xml)
{
    // Get a list of all attribute nodes and the
// length of that list
    CComPtr<IXMLDOMNamedNodeMap> attributes;
    xml->get_attributes(&attributes);
    long len;
    attributes->get_length(&len);

    // Attributes to set:

    wstring sname;
    wstring path;
    wstring dump;

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

        if (name == L"name")
        {
            sname = value.bstrVal;
        }
        else if (name == L"path")
        {
            path = value.bstrVal;
        }

        else if (name == L"dump") {

            dump = value.bstrVal;
        }
    }

    // Create a wave in component:

    CWaveIn wave(path.c_str());

    // Create WaveContainer:

    auto container = std::make_shared<WaveContainer>();

    // Populate wave container:

    container->FromCWave(wave);

    // Add wave container to sample map:

    auto pair = std::pair<wstring, std::shared_ptr<WaveContainer>>(sname, container);

    sample_map.insert(pair);

    // Determine if we need to dump:

    if (!dump.empty()) {

        // Create the WaveOut:

        CWaveOut owave(dump.c_str());

        // Dump the contents:

        container->ToCWave(owave);

        // Close the wave:

        owave.close();
    }
}

void CSynthesizer::XmlLoadEffects(IXMLDOMNode* xml)
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
            effect.FromXML(node);
        }
    }
}

void CSynthesizer::XmlLoadScore(IXMLDOMNode* xml)
{
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

        // Get the value of the attribute.  A CComVariant is a variable
        // that can have any type. It loads the attribute value as a
        // string (UNICODE), but we can then change it to an integer 
        // (VT_I4) or double (VT_R8) using the ChangeType function 
        // and then read its integer or double value from a member variable.
        CComVariant value;
        attrib->get_nodeValue(&value);

        if (name == L"bpm")
        {
            value.ChangeType(VT_R8);
            m_bpm = value.dblVal;
            m_secperbeat = 1 / (m_bpm / 60);
        }
        else if (name == L"beatspermeasure")
        {
            value.ChangeType(VT_I4);
            m_beatspermeasure = value.intVal;
        }
        else if (name == L"nomeasure") {
            // Configure ourselves to ignore measures:
            nomeasure = true;
        }
    }


    CComPtr<IXMLDOMNode> node;
    xml->get_firstChild(&node);
    for (; node != NULL; NextNode(node))
    {
        // Get the name of the node
        CComBSTR name;
        node->get_nodeName(&name);

        if (name == L"instrument")
        {
            XmlLoadInstrument(node);
        }

        if (name == "samples") {
            XmlLoadSamples(node);
        }

        if (name == "effects") {
            XmlLoadEffects(node);
        }
    }
}

void CSynthesizer::OpenScore(CString& filename)
{
    Clear();

    //
    // Create an XML document
    //

    CComPtr<IXMLDOMDocument>  pXMLDoc;
    bool succeeded = SUCCEEDED(CoCreateInstance(CLSID_DOMDocument, NULL, CLSCTX_INPROC_SERVER,
        IID_IXMLDOMDocument, (void**)&pXMLDoc));
    if (!succeeded)
    {
        AfxMessageBox(L"Failed to create an XML document to use");
        return;
    }

    // Open the XML document
    VARIANT_BOOL ok;
    succeeded = SUCCEEDED(pXMLDoc->load(CComVariant(filename), &ok));
    if (!succeeded || ok == VARIANT_FALSE)
    {
        AfxMessageBox(L"Failed to open XML score file");
        return;
    }

    //
    // Traverse the XML document in memory!!!!
    // Top level tag is <score>
    //

    CComPtr<IXMLDOMNode> node;
    pXMLDoc->get_firstChild(&node);
    for (; node != NULL; NextNode(node))
    {
        // Get the name of the node
        CComBSTR nodeName;
        node->get_nodeName(&nodeName);

        if (nodeName == "score")
        {
            XmlLoadScore(node);
        }

    }

    sort(m_notes.begin(), m_notes.end());

}

//! Generate one audio frame
bool CSynthesizer::Generate(double* frame)
{
    //
    // Phase 1: Determine if any notes need to be played.
    //

    while (m_currentNote < (int)m_notes.size())
    {
        // Get a pointer to the current note
        CNote* note = &m_notes[m_currentNote];

        // If the measure is in the future we can't play
        // this note just yet.
        if (note->Measure() > m_measure)
            break;

        // If this is the current measure, but the
        // beat has not been reached, we can't play
        // this note.
        if (note->Measure() == m_measure && note->Beat() > m_beat)
            break;

        //
        // Play the note!
        //

        // Create the instrument object
        CInstrument* instrument = NULL;
        if (note->Instrument() == L"ToneInstrument")
        {
            instrument = new CToneInstrument();
        }

        //
        // Wavetable Voice
        //

        if (note->Instrument() == L"WaveTable") {
            instrument = new Wavetable();
        }

        //
        // Additive Voice
        //

        if (note->Instrument() == L"Additive") {
            instrument = new CAdditive();
        }

        // Configure the instrument object
        if (instrument != NULL)
        {
            instrument->SetSynth(this);
            instrument->SetSampleRate(GetSampleRate());
            instrument->SetNote(note);
            instrument->SetBPM(m_bpm);
            instrument->Start();

            m_instruments.push_back(instrument);
        }

        m_currentNote++;
    }

    //
    // Phase 2: Clear all channels to silence 
    //

    for (int c = 0; c < GetNumChannels(); c++)
    {
        frame[c] = 0;
    }

    //
    // Phase 3: Play an active instruments
    //

    //
    // We have a list of active (playing) instruments.  We iterate over 
    // that list.  For each instrument we call generate, then add the
    // output to our output frame.  If an instrument is done (Generate()
    // returns false), we remove it from the list.
    //

    for (list<CInstrument*>::iterator node = m_instruments.begin(); node != m_instruments.end(); )
    {
        // Since we may be removing an item from the list, we need to know in 
        // advance, what is after it in the list.  We keep that node as "next"
        list<CInstrument*>::iterator next = node;
        next++;

        // Get a pointer to the allocated instrument
        CInstrument* instrument = *node;

        // Call the generate function
        if (instrument->Generate())
        {
            double tframe[2];

            // If we returned true, we have a valid sample.  Add it 
            // to the frame.
            for (int c = 0; c < GetNumChannels(); c++)
            {

                // Create temporary frame container

                tframe[c] = instrument->Frame(c);
            }

            // Run through effects:

            double eoutput = effect.Process(tframe[0], instrument->GetEffectInfo());

            // Set frame:

            frame[0] += eoutput;
            frame[1] += eoutput;
        }
        else
        {
            // If we returned false, the instrument is done.  Remove it
            // from the list and delete it from memory.
            m_instruments.erase(node);
            delete instrument;
        }

        // Move to the next instrument in the list
        node = next;
    }

    //
    // Phase 4: Advance the time and beats
    //

    // Time advances by the sample period
    m_time += GetSamplePeriod();

    // Beat advances by the sample period divided by the 
    // number of seconds per beat.  The inverse of seconds
    // per beat is beats per second.
    m_beat += GetSamplePeriod() / m_secperbeat;

    // When the measure is complete, we move to
    // a new measure.  We might be a fraction into
    // the new measure, so we subtract out rather 
    // than just setting to zero.
    if (!nomeasure && m_beat > m_beatspermeasure)
    {
        m_beat -= m_beatspermeasure;
        m_measure++;
    }

    //
    // Phase 5: Determine when we are done
    //

    // We are done when there is nothing to play.  We'll put something more 
    // complex here later.
    return !m_instruments.empty() || m_currentNote < (int)m_notes.size();
}
