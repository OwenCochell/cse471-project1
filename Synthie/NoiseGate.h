#pragma once

class NoiseGate {
public:
    ~NoiseGate();

    double Process(double frameIn);
    void FromXML(IXMLDOMNode* xml);  
    
private:
    double AdjustGateValue(double frameValue, double& gateValue);

    double m_threshold = 0.0;
    double m_gateL = 1.0;
    double m_gateR = 1.0;
};
