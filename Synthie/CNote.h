#pragma once

#include <string>

#include "Notes.h"

class CNote
{
public:
    CNote(void) {}
    virtual ~CNote(void) {}

    int Measure() const { return m_measure; }
    double Beat() const { return m_beat; }
    const std::wstring& Instrument() const { return m_instrument; }
    IXMLDOMNode* Node() { return m_node; }
    IXMLDOMNode* PNode() { return m_parent; }

    std::wstring Note() { return note; }
    std::wstring GNote() { return gnote; }

    double Freq() { return NoteToFrequency(note.c_str()); }
    double GFreq() { return NoteToFrequency(gnote.c_str()); }

    double Duration() { return duration; }

    bool HasGliss() { return !gnote.empty(); }

    void XmlLoad(IXMLDOMNode* xml, std::wstring& instrument);

    bool operator<(const CNote& b);

private:
    std::wstring m_instrument;
    int m_measure;
    double m_beat;
    CComPtr<IXMLDOMNode> m_node = nullptr;

    // Base note
    std::wstring note;

    // Gliss note:
    std::wstring gnote;

    // Frequency of base note
    double freq;

    // Frequency of gliss note
    double gfreq;

    // Duration of this note
    double duration;

    // Parent node, containg instrument info
    CComPtr<IXMLDOMNode> m_parent = nullptr;
};
