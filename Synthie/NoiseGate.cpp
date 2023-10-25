#include "NoiseGate.h"
#include <CSynthesizer.cpp>



NoiseGate::NoiseGate() :
    m_threshold(0.0), m_dry(0.0), m_wet(0.0), m_gateL(1.0), m_gateR(1.0) {}

NoiseGate::~NoiseGate() {}

void NoiseGate::Process(double* frameIn, double* frameOut)
{
    frameOut[0] = AdjustGateValue(frameIn[0], m_gateL);
    frameOut[1] = AdjustGateValue(frameIn[1], m_gateR);
}

double NoiseGate::AdjustGateValue(double frameValue, double& gateValue)
{
    gateValue += (frameValue < m_threshold && frameValue > -m_threshold) ? -0.005 : 0.005;
    // Ensure gateValue is between 0 and 1
    if (gateValue < 0.0) gateValue = 0.0;
    if (gateValue > 1.0) gateValue = 1.0;

    return m_dry * frameValue + (m_wet * frameValue * gateValue);
}

void NoiseGate::XmlLoad(IXMLDOMNode* xml)
{
    CComPtr<IXMLDOMNamedNodeMap> attributes;
    xml->get_attributes(&attributes);
    long len;
    attributes->get_length(&len);

    for (int i = 0; i < len; i++)
    {
        CComPtr<IXMLDOMNode> attrib;
        attributes->get_item(i, &attrib);

        CComBSTR name;
        attrib->get_nodeName(&name);
        CComVariant value;
        attrib->get_nodeValue(&value);

        if (name == "threshold")
        {
            value.ChangeType(VT_I4);
            m_threshold = value.intVal;
        }
        else if (name == "wet")
        {
            value.ChangeType(VT_R8);
            m_wet = value.dblVal;
        }
        else if (name == "dry")
        {
            value.ChangeType(VT_R8);
            m_dry = value.dblVal;
        }
    }
}
