#pragma once
#include <cstdint>

static const size_t TS_SAMPLERATE = 22050;
static const size_t TS_BUFFERSIZE = 128; 
static const size_t TS_FRAMESIZE = 64;
static constexpr float TS_TIMESTEP = 1.0f / float(TS_SAMPLERATE);

template <typename T>
class CircleBuffer {
public:
	CircleBuffer() : mCapacity(0), mpData(nullptr), mSize(0), mHead(0), mTail(0) {}
	CircleBuffer(size_t initialSize) : mCapacity(initialSize), mpData(nullptr), mSize(0), mHead(0), mTail(0) {
		mpData = (T*) malloc(initialSize * sizeof(T));
	}
	~CircleBuffer() {
		free(mpData);
	}
	void clear() {
		mHead = 0;
		mTail = 0;
		mSize = 0;
	}
	bool empty() const { return mSize == 0; }
	bool full() const { return mSize == mCapacity; }
	size_t size() const { return mSize; }
	size_t space() const { return mCapacity - mSize; }
	void resize(size_t n) {
		T* pNewData = (T*) malloc(n * sizeof(T));
		memcpy(pNewData, mpData, ((n > (size_t)mSize) ? (size_t)mSize : n) * sizeof(T));
		free(mpData);
		mpData = pNewData;
		mCapacity = n;
	}
	bool push(const T& dat) {
		bool ret = false;
		if (mSize < mCapacity) {
			ret = true;
			++mSize;
			mpData[mHead] = dat;
			step(mHead);
		}
		return ret;
	}
	bool push(T* data, size_t n) {
		bool ret = false;
		if (mSize + n <= mCapacity) {
			ret = true;
			mSize += n;
#if 0
			if (mHead + n < mCapacity) {
				memcpy(mpData[mHead], n * sizeof(T));
				mHead += n;
			}
			else {
				memcpy(mpData[mHead], (mCapacity - mHead) * sizeof(T));
				mHead = mHead + n - mCapacity;
				memcpy(mpData, (mHead) * sizeof(T));
			}
#endif
			for (size_t i = 0; i < n; ++i) {
				mpData[mHead] = data[i];
				step(mHead);
			}
		}
		return ret;
	}
	bool pull(T& dat) {
		bool ret = false;
		if (mSize > 0) {
			ret = true;
			--mSize;
			dat = mpData[mTail];
			step(mTail);
		}
		return ret;
	}
	bool pull(T* dat) {
		bool ret = false;
		if (mSize > 0) {
			ret = true;
			--mSize;
			*dat = mpData[mTail];
			step(mTail);
		}
		return ret;
	}
	bool pull(T* data, size_t n) {
		bool ret = false;
		if (mSize >= n) {
			ret = true;
			mSize -= n;
			for (size_t i = 0; i < n; ++i) {
				data[i] = mpData[mTail];
				step(mTail);
			}
		}
		return ret;
	}

	void step(size_t& n) {
		if (n != mCapacity) {
			n++;
		}
		else {
			n = 0;
		}
	}
private:
	T* mpData;
	size_t mCapacity;
	std::atomic<size_t> mSize;
	size_t mHead;
	size_t mTail;
};

using TSBuffer = CircleBuffer<float>;
