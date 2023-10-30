#include "DRC.h"
#include <stdlib.h>
#include <corecrt_math.h>
#include <CSynthesizer.cpp>

DRC::DRC() :
    m_threshold(-20.0), m_ratio(4.0), m_attack(0.001), m_release(0.05),
    m_knee(5.0), m_compressionAmount(0.0), m_envelope(0.0) {}

DRC::~DRC() {}

double DRC::Compress(double frameValue) {
    double inputLevel = 20.0 * log10(abs(frameValue) + 0.00001);
    double difference = inputLevel - m_threshold;

    if (difference > m_knee) {
        m_compressionAmount = difference / m_ratio;
    }
    else {
        m_compressionAmount = difference * difference / (2.0 * m_knee);
    }

    double targetGain = inputLevel - m_compressionAmount;
    double change = targetGain - m_envelope;

    if (change > 0.0) {
        m_envelope += change * m_attack;
    }
    else {
        m_envelope += change * m_release;
    }

    return frameValue * pow(10.0, (m_envelope - inputLevel) / 20.0);
}

void DRC::Process(double* frameIn, double* frameOut) {
    frameOut[0] = Compress(frameIn[0]);
    frameOut[1] = Compress(frameIn[1]);
}

void DRC::XmlLoad(IXMLDOMNode* xml) {
    CComPtr<IXMLDOMNamedNodeMap> attributes;
    xml->get_attributes(&attributes);
    long len;
    attributes->get_length(&len);

    for (int i = 0; i < len; i++) {
        CComPtr<IXMLDOMNode> attrib;
        attributes->get_item(i, &attrib);

        CComBSTR name;
        attrib->get_nodeName(&name);
        CComVariant value;
        attrib->get_nodeValue(&value);

        if (name == "threshold") {
            value.ChangeType(VT_R8);  
            m_threshold = value.dblVal;
        }
        else if (name == "ratio") {
            value.ChangeType(VT_R8);  
            m_ratio = value.dblVal;
        }
        else if (name == "attack") {
            value.ChangeType(VT_R8);  
            m_attack = value.dblVal;
        }
        else if (name == "release") {
            value.ChangeType(VT_R8);  
            m_release = value.dblVal;
        }
        else if (name == "knee") {
            value.ChangeType(VT_R8);  
            m_knee = value.dblVal;
        }
    }
}