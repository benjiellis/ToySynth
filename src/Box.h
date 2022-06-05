#pragma once
#include <thread>
#include <vector>
#include <atomic>
#include "Buffer.h"
#include "TSUtilities.h"

class IBox {
public:
	virtual void start() = 0;
	virtual void stop() = 0;
private:
};

class Box : public IBox {
public:
	Box() : mRunning(false), mName("") {}
	Box(const std::string& name) : mName(name) {}

	virtual void start() override {
		setup();
		mRunning = true;
		mThread = std::thread(&Box::run, this);
		mThread.detach();
		log(mName + " started.");
	}
	virtual void stop() override {
		mRunning = false;
		teardown();
		log(mName + " stopped.");
	}

	virtual void process() = 0;

	virtual void setup() {};
	virtual void teardown() {};

private:
	void run() {
		while (mRunning) {
			process();
		}
	}
	std::thread mThread;
	std::atomic<bool> mRunning;
	std::string mName;
};

class Generator : public Box {
public:
	Generator() {}
	Generator(const std::string& name) : Box(name), mOut(TS_BUFFERSIZE) {}
	TSBuffer* getOutput() { return &mOut; }
protected:
	TSBuffer mOut;
};