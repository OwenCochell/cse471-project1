#pragma once

class BaseEnvelope {
public:

	BaseEnvelope() = default;

	void SetDuration(double duration) { this->duration = duration; }
	double GetDuration() { return this->duration; }

	void SetTimePerBeat(double tpb) { this->tpb = tpb; }
	double GetTimePerBeat() { return this->tpb; }

	void SetStartValue(double start) { this->start_value = start; }
	double GetStartValue() { return this->start_value; }

	void SetStopValue(double stop) { this->stop_value = stop; }
	double GetStopValue() { return this->stop_value; }

	void SetStartTime(double start) { this->start_time = start; }
	double GetStartTime() { return this->start_time; }

	void SetStopTime(double start) { this->stop_time = start; }
	double GetStopTime() { return this->stop_time; }

	virtual double DetermineAmplitude(double time) { return 0.0; }

private:

	// Duration of this envelope:
	double duration = 0;

	// Start value of this envelope:
	double start_value;

	// Stop value of this envelope:
	double stop_value;

	// Start time of this envelope
	double start_time;

	// Stop time of this envelope
	double stop_time;

	// Time per beat
	double tpb;
};

class LinearRamp : public BaseEnvelope {
public:

	LinearRamp()=default;

	virtual double DetermineAmplitude(double time) override;
};

class ConstantEnvelope : public BaseEnvelope {
public:

	ConstantEnvelope()=default;

	virtual double DetermineAmplitude(double time) override;
};

// TODO: Use more defined envelopes for this class
class AmplitudeEnvelope
{
public:

	AmplitudeEnvelope();

	void SetAttack(double attack) { this->attack = attack; }
	double GetAttack() { return this->attack; }

	void SetRelease(double release) { this->release = release; }
	double GetRelease() { this->release; }

	void SetSustain(double sustain) { this->sustain = sustain; }
	double GetSustain() { this->release; }

	void SetDuration(double duration) { this->duration = duration; }
	double GetDuration() { return this->duration; }

	void SetTimePerBeat(double tpb) { this->tpb = tpb; }

	double DetermineAmplitude(double time);

private:
	 
	double Ramp(double time, double startt, double stopt, double startv, double stopv);

	double duration;
	double attack;
	double release;
	double sustain = 1.;
	double tpb;
};
