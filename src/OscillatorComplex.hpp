#pragma once

#include "Box.h"
#include "Oscillator.h"
#define _USE_MATH_DEFINES
#include <math.h>

class OscillatorComplex : public Box {

	class OCOsc {
	public:
		OCOsc() : mTheta(0.0f), mAmp(0.0f), mFreq(200.0f), mType(OscType::Sine), 
			mpAmpOsc(nullptr), mpFreqOsc(nullptr) {}

		void setFreq(float f) { mFreq = f; }
		void setAmp(float f) { mAmp = f; }
		void setType(OscType t) { mType = t; }

		OCOsc* setFreqMod() {
			mpFreqOsc = std::make_unique<OCOsc>();
			return mpFreqOsc.get();
		}
		void remFreqMod() { mpFreqOsc = nullptr; }
		OCOsc* setAmpMod() {
			mpAmpOsc = std::make_unique<OCOsc>();
			return mpAmpOsc.get();
		}
		void remAmpMod() { mpAmpOsc = nullptr; }

		float getNext() {
			float ret = 0.0f;
			switch (mType) {
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
			ret *= mAmp;
			if (mpAmpOsc) {
				ret *= mpAmpOsc->getNext();
			}
			step(mTheta);
			return ret;
		}
	private:
		void step(float& theta) {
			float f = 0.0f;
			if (mpFreqOsc) {
				f = mpFreqOsc->getNext();
			}
			mTheta += TS_TIMESTEP * (mFreq + f);
			if (mTheta > 1.0f) {
				mTheta = fmodf(mTheta, 1.0f);
			}
		}

		float mTheta;
		float mAmp;
		float mFreq;
		OscType mType;
		std::unique_ptr<OCOsc> mpAmpOsc;
		std::unique_ptr<OCOsc> mpFreqOsc;
	};

public:
	OscillatorComplex() : Box("Oscillator"), mpOsc(new OCOsc()), mOut(TS_BUFFERSIZE) {
		mpOsc->setFreq(200.0f);
		mpOsc->setAmp(0.5f);
	}

	virtual void process() override {
		if (mOut.space() > TS_FRAMESIZE) {
			for (size_t i = 0; i < TS_FRAMESIZE; ++i) {
				mOut.push(mpOsc->getNext());
			}
		}
	}

	OCOsc* getOsc() { return mpOsc.get(); }

	void setFreq(float f) { mpOsc->setFreq(f); }
	void setAmp(float f) { mpOsc->setAmp(f); }
	void setType(OscType t) { mpOsc->setType(t); }

	TSBuffer* getOutput() { return &mOut; }

private:
	TSBuffer mOut;
	std::unique_ptr<OCOsc> mpOsc;
};