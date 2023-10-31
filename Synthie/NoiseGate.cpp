#include "pch.h"
#include "NoiseGate.h"

NoiseGate::~NoiseGate() {}

double NoiseGate::Process(double frameIn)
{
    return AdjustGateValue(frameIn, m_gateL);
}

double NoiseGate::AdjustGateValue(double frameValue, double& gateValue)
{
    gateValue += (frameValue < m_threshold && frameValue > -m_threshold) ? -0.005 : 0.005;
    // Ensure gateValue is between 0 and 1
    if (gateValue < 0.0) gateValue = 0.0;
    if (gateValue > 1.0) gateValue = 1.0;

    return (frameValue * gateValue);
}

void NoiseGate::FromXML(IXMLDOMNode* xml)
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
    }
}
