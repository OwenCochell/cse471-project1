#pragma once

class NoiseGate {
public:
    NoiseGate();
    ~NoiseGate();

    void Process(double* frameIn, double* frameOut);
    void XmlLoad(IXMLDOMNode* xml);  
    

private:
    double AdjustGateValue(double frameValue, double& gateValue);

    double m_threshold;
    double m_dry;
    double m_wet;
    double m_gateL;
    double m_gateR;
};
