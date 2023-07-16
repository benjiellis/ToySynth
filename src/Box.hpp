#pragma once
#include <thread>
#include <vector>
#include <atomic>
#include "Buffer.h"
#include "TSUtilities.h"

class Box {
public:
	Box() : mRunning(false), mName("") {}
	Box(const std::string& name) : mName(name) {}

	void start() {
		setup();
		mRunning = true;
		mThread = std::thread(&Box::run, this);
		mThread.detach();
		log(mName + " started.");
	}
	void stop() {
		mRunning = false;
		teardown();
		log(mName + " stopped.");
	}

	virtual void process() = 0;

	virtual void setup() {};
	virtual void teardown() {};

protected:
	std::vector<TSBuffer*> mpIns;
	std::string mName;

	bool allReady() const {
		bool ret = true;
		for (auto& pIn : mpIns) {
			if (pIn) {
				ret &= pIn->size() >= TS_FRAMESIZE;
			}
		}
		return ret;
	}

private:
	void run() {
		while (mRunning) {
			process();
		}
	}
	std::thread mThread;
	std::atomic<bool> mRunning;
};