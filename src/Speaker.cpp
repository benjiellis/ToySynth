#include "Speaker.h"
#include <algorithm>

void Speaker::startStream() {
	PaError err;
	err = Pa_Initialize();
	if (err != paNoError) {
		throw std::exception("Cannot initialize PortAudio");
	}
	mOutputParams.device = Pa_GetDefaultOutputDevice();
	if (mOutputParams.device == paNoDevice) {
		throw std::exception("Cannot find default audio output");
	}

	mOutputParams.channelCount = 1;
	mOutputParams.sampleFormat = paFloat32;
	mOutputParams.suggestedLatency = Pa_GetDeviceInfo(mOutputParams.device)->defaultLowOutputLatency;
	mOutputParams.hostApiSpecificStreamInfo = nullptr;

	err = Pa_OpenStream(&mAudioStream,
		nullptr, &mOutputParams,
		TS_SAMPLERATE, TS_FRAMESIZE, paClipOff,
		callback, &mBuff);

	if (err != paNoError) {
		throw std::exception("Failed to open audio stream");
	}

	err = Pa_SetStreamFinishedCallback(mAudioStream, &callback_finished);

	if (err != paNoError) {
		throw std::exception("Failed to set finished callback");
	}

	err = Pa_StartStream(mAudioStream);
	if (err != paNoError) {
		throw std::exception("Failed to start audio stream");
	}
}

void Speaker::process() {
	// to avoid starving the audio thread, we're going to shut down
	if (!mStarted && mBuff.size() > TSSPEAKER_STARTTHRESHOLD) {
		mStarted = true;
		startStream();
	}
	else if (mStarted && mBuff.size() < TSSPEAKER_STOPTHRESHOLD) {
		mStarted = false;
		stopStream();
	}

	while (allReady() && mBuff.size() < TSSPEAKER_BUFFERSIZE) {
		float sum = 0.0f;
		for (auto& pIn : mpIns) {
			if (pIn) {
				float f;
				pIn->pull(f);
				sum += f;
			}
		}
		sum *= mVol;
		//sum = std::clamp(sum, -1.0f, 1.0f);
		if (sum > 1.0f) {
			sum = 1.0f;
		}
		else if (sum < -1.0f) {
			sum = -1.0f;
		}
		mBuff.push(sum);
	}
}

void Speaker::stopStream() {
	PaError err;
	err = Pa_StopStream(mAudioStream);
	if (err != paNoError) {
		throw std::exception("Failed to stop audio stream");
	}
	err = Pa_CloseStream(mAudioStream);
}

int Speaker::callback(const void* inputBuffer, void* outputBuffer, unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData) {

	CircleBuffer<float>* pAudioData = (CircleBuffer<float>*)userData;
	float* out = (float*)outputBuffer;
	pAudioData->pull(out, framesPerBuffer);
	return paContinue;
}

void Speaker::callback_finished(void* userData) {

}