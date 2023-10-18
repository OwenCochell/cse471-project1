#pragma once
class AmplitudeEnvelope
{
public:

	AmplitudeEnvelope();

	void SetAttack(double attack) { this->attack = attack; }
	double GetAttack() { return this->attack; }

	void SetRelease(double release) { this->release = release; }
	double GetRelease() { this->release; }

	void SetDuration(double duration) { this->duration = duration; }
	double GetDuration() { return this->duration; }

	void SetTimePerBeat(double tpb) { this->tpb = tpb; }

	double DetermineAmplitude(double time);

private:

	double Ramp(double time, double startt, double stopt, double startv, double stopv);

	double duration;
	double attack;
	double release;
	double tpb;
};

