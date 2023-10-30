#pragma once

class DRC {
public:
    DRC();
    ~DRC();

    void Process(double* frameIn, double* frameOut);
    void XmlLoad(IXMLDOMNode* xml);

private:
    double Compress(double frameValue);

    double m_threshold;
    double m_ratio;
    double m_attack;
    double m_release;
    double m_knee;

    double m_compressionAmount;
    double m_envelope;
};
