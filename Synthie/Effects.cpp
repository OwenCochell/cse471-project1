#include "pch.h"
#include "Effects.h"

#include <string>

void Effects::FromXML(IXMLDOMNode* xml)
{
    // Get a list of all attribute nodes and the
// length of that list
    CComPtr<IXMLDOMNamedNodeMap> attributes;
    xml->get_attributes(&attributes);
    long len;
    attributes->get_length(&len);

    std::wstring name;

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
            name = value.bstrVal;

            // Determine which effect to load:

            if (name == L"delay") {
                delay.FromXML(xml);
            }
            else if (name == L"ringmod") {
                ring.FromXML(xml);
            }
            else if (name == L"reverb") {
                reverb.FromXML(xml);
            }
            else if (name == L"flanger") {
                flang.FromXML(xml);
            }
            else if (name == L"chorus") {
                chor.FromXML(xml);
            }
            else if (name == L"noise") {
                noise.FromXML(xml);
            }
            else if (name == L"drc") {
                compress.FromXML(xml);
            }
        }
    }
}

double Effects::Process(double frame, std::vector<EffectInfo>* info)
{
    // Iterate over effect info:

    double final = 0;

    for (int i = 0; i < info->size(); ++i) {

        // Determe dry frame:

        double dframe = info->at(i).dry * frame;
        double dwet = info->at(i).wet;
        double etmp = 0.;

        // Determine the number:

        int num = info->at(i).num;

        // Determine the effect to use:

        if (num == 0) {
            final += dwet * delay.process(dframe, 44100);
        }
        else if (num == 1) {
            final += dwet * ring.process(dframe);
        }
        else if (num == 2) {
            final += dwet * reverb.process(dframe);
        }
        else if (num == 3) {
            final += dwet * flang.process(dframe);
        }
        else if (num == 4) {
            final += dwet * chor.Process(dframe);
        }
        else if (num == 5) {
            final += dwet * noise.Process(dframe);
        }
        else if (num == 6) {
            final += dwet * compress.Process(dframe);
        }
        else if (num == -1) {
            final += dwet * dframe;
        }
    }
    return final;
}
