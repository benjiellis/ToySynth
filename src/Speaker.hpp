#pragma once
#include <portaudio.h>
#include "Box.h"
const size_t TSSPEAKER_BUFFERSIZE = TS_BUFFERSIZE * 8;
const size_t TSSPEAKER_STARTTHRESHOLD = TS_BUFFERSIZE * 7;
const size_t TSSPEAKER_STOPTHRESHOLD = TS_BUFFERSIZE;

class Speaker : public Box {
public:
	Speaker() : mVol(0.0f), mStarted(false), Box("Speaker"), mBuff(TSSPEAKER_BUFFERSIZE) {

	}

	virtual void process() override;

	static int callback(const void* inputBuffer, void* outputBuffer, unsigned long framesPerBuffer,
		const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData);
	static void callback_finished(void* userData);

	void setVol(float f) { mVol = f; }
	void addInput(TSBuffer* pIn) { 
		mpIns.push_back(pIn);
	}
private:
	void startStream();
	void stopStream();
	TSBuffer mBuff;
	float mVol;
	PaStream* mAudioStream;
	PaStreamParameters mOutputParams;
	bool mStarted;
};