#pragma once
#include "Box.h"
#define _USE_MATH_DEFINES
#include <math.h>

class Oscillator : public Generator {
public:
	enum class OscType { Sine, Saw };

	Oscillator() : mOscType(OscType::Sine), mAmp(0.5f), mTheta(0.0f), mFreq(200.0f), Generator("Oscillator") {}

	virtual void process() override {
		if (mOut.space() > 64) {
			for (size_t i = 0; i < 64; ++i) {
				mOut.push(getNext());
			}
		}
	}

	void setFreq(float f) { mFreq = f; }
	void setAmp(float f) { mAmp = f; }
	void setType(OscType type) { mOscType = type; mTheta = 0.0f; }

	float getNext() {
		float ret = 0.0f;
		switch (mOscType) {
		case OscType::Sine:
			ret = std::sinf(2 * M_PI * mTheta);
			break;
		case OscType::Saw:
			ret = mTheta;
			break;
		default:
			ret = 0;
		}
		ret *= mAmp;
		step(mTheta);
		return ret;
	}

	void getFullBuffer(TSBuffer& buff, size_t nSecs) {
		size_t samples = nSecs * TS_SAMPLERATE;
		buff.resize(samples);
		for (size_t i = 0; i < buff.size(); ++i) {
			buff.push(getNext());
		}
	}

private:
	void step(float& theta) {
		mTheta += TS_TIMESTEP * mFreq;
		if (mTheta > 1.0f) {
			mTheta = fmodf(mTheta, 1.0f);
		}
	}

	float mTheta;
	float mAmp;
	float mFreq;
	OscType mOscType;
};