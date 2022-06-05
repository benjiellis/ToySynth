#pragma once
#include "Box.h"
#define _USE_MATH_DEFINES
#include <math.h>

class Oscillator : public Box {
public:
	enum class OscType { Sine, Saw, RevSaw };

	Oscillator() : mOscType(OscType::Sine), mAmp(0.5f), mTheta(0.0f), mFreq(200.0f), 
		Box("Oscillator"), mOut(TS_BUFFERSIZE), mpAmpIn(nullptr), mpFreqIn(nullptr) {}

	virtual void process() override {
		const auto allReady = [&]() {
			bool ret = true;
			if (mpAmpIn) {
				ret &= mpAmpIn->size() >= 64;
			}
			if (mpFreqIn) {
				ret &= mpFreqIn->size() >= 64;
			}
			return ret;
		};

		if (mOut.space() > 64 && allReady()) {
			for (size_t i = 0; i < 64; ++i) {
				mOut.push(getNext());
			}
		}
	}

	void setFreq(float f) { mFreq = f; }
	void setAmp(float f) { mAmp = f; }
	void setType(OscType type) { mOscType = type; mTheta = 0.0f; }

	TSBuffer* getOutput() { return &mOut; }

	void setFreqMod(TSBuffer* pFreqMod) { mpFreqIn = pFreqMod; }
	void setAmpMod(TSBuffer* pAmpMod) { mpAmpIn = pAmpMod; }

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
		if (mpAmpIn) {
			float f;
			mpAmpIn->pull(f);
			ret *= f;
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
		float f = 0.0f;
		if (mpFreqIn) {
			mpFreqIn->pull(f);
		}
		mTheta += TS_TIMESTEP * (mFreq + f);
		if (mTheta > 1.0f) {
			mTheta = fmodf(mTheta, 1.0f);
		}
	}

	float mTheta;
	float mAmp;
	float mFreq;
	OscType mOscType;

	TSBuffer mOut;
	
	TSBuffer* mpFreqIn;
	TSBuffer* mpAmpIn;
};