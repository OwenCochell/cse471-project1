#pragma once

class SimpleDelay {
public:
    void Configure(int maxDelaySamples);
    ~SimpleDelay();
    double process(double input, int delaySamples);

    void FromXML(IXMLDOMNode* xml);
private:
    double* m_buffer;
    int m_bufferSize;
    int m_index;
};
