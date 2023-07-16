#pragma once
#include "Box.h"
#define _USE_MATH_DEFINES
#include <math.h>

enum class OscType { Sine, Saw, RevSaw };

class Oscillator : public Box {
public:

	Oscillator() : mOscType(OscType::Sine), mAmp(0.5f), mTheta(0.0f), mFreq(200.0f), 
		Box("Oscillator"), mOut(TS_BUFFERSIZE) {
		
		mpIns.resize(2, nullptr);
		
	}

	virtual void process() override {
		if (mOut.space() > TS_FRAMESIZE && allReady()) {
			for (size_t i = 0; i < TS_FRAMESIZE; ++i) {
				mOut.push(getNext());
			}
		}
	}
	
	void setFreq(float f) { mFreq = f; }
	void setAmp(float f) { mAmp = f; }
	void setType(OscType type) { mOscType = type; mTheta = 0.0f; }

	TSBuffer* getOutput() { return &mOut; }

	void setAmpMod(TSBuffer* pAmpMod) { mpIns[0] = pAmpMod; }
	void setFreqMod(TSBuffer* pFreqMod) { mpIns[1] = pFreqMod; }

	float getNext() {
		float ret = 0.0f;
		switch (mOscType) {
		case OscType::Sine:
			ret = std::sinf(2 * M_PI * mTheta);
			break;
		case OscType::Saw:
			ret = mTheta;
			break;
		case OscType::RevSaw:
			ret = 1 - mTheta;
			break;
		default:
			ret = 0;
		}
		if (getAmpIn()) {
			float f;
			getAmpIn()->pull(f);
			ret *= f;
		}
		ret *= mAmp;
		step(mTheta);
		return ret;
	}

private:
	void step(float& theta) {
		float f = 0.0f;
		if (getFreqIn()) {
			getFreqIn()->pull(f);
		}
		mTheta += TS_TIMESTEP * (mFreq + f);
		if (mTheta > 1.0f) {
			mTheta = fmodf(mTheta, 1.0f);
		}
	}

	TSBuffer* getAmpIn() { return mpIns[0]; }
	TSBuffer* getFreqIn() { return mpIns[1]; }

	float mTheta;
	float mAmp;
	float mFreq;
	OscType mOscType;

	TSBuffer mOut;
};