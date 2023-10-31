#pragma once

#include <vector>
#include <string>

#include "SimpleDelay.h"
#include "RingModulator.h"
#include "Reverb.h"
#include "FlangerEffect.h"
#include "Chorus.h"
#include "NoiseGate.h"
#include "DRC.h"

struct EffectInfo {

	// Number of effect to utilize
	int num;

	// Amplitude of output
	double dry;

	// Amplitude of output:
	double wet;
};

/**
* A class for managing effects.
* 
* Each sample provided must provide a SampleDefinition,
* which describes the sample number, wet and dry values.
* 
* -1 - No effect
* 0 - Delay
* 1 - RingModulator
* 2 - Reverb
* 3 - Flanger
* 4 - Chorus
* 5 - NoiseGate
* 6 - Dynamic Range Compressor
*/
class Effects
{
public:

	Effects() = default;

	/**
	* Configures an effect from a XML Code
	*/
	void FromXML(IXMLDOMNode* xml);

	double Process(double frame, std::vector<EffectInfo>* info);

	static int NameToNum(std::wstring name) {
		if (name == L"delay") {
			return 0;
		}
		else if (name == L"ringmod") {
			return 1;
		}
		else if (name == L"reverb") {
			return 2;
		}
		else if (name == L"flanger") {
			return 3;
		}
		else if (name == L"chorus") {
			return 4;
		}
		else if (name == L"noise") {
			return 5;
		}
		else if (name == L"drc") {
			return 6;
		}
		else if (name == L"none") {
			return -1;
		}
	}

private:

	//
	// Effect Instances
	//

	SimpleDelay delay;

	RingModulator ring;

	Reverb reverb;

	FlangerEffect flang;

	Chorus chor;

	NoiseGate noise;

	DRC compress;
};
