#pragma once

// cPool_Debug
struct cPool_Debug {
	static int UsedCount;
};

// cPool_Store
class cPool_Store {
private:
	int m_QuantSize;
	int m_SegmentSize;
	cList<byte *> m_Pool;
	cList<byte *> m_FreeList;
public:
	cPool_Store(const int QuantSize) {
		m_QuantSize = QuantSize;
		m_SegmentSize = 512;
	}
	~cPool_Store() {
		int i;
		for(i = 0; i < m_Pool.Count(); i++) {
			cAssert(m_Pool[i] != NULL);
			delete[] m_Pool[i];
		}
	}
	byte * Allocate() {
		int i;
		byte *t;
		if(m_FreeList.IsEmpty()) {
			t = new byte[m_SegmentSize * m_QuantSize];
			m_Pool.Add(t);
			for(i = 0; i < m_SegmentSize; i++) {
				m_FreeList.Add(t + i * m_QuantSize);
			}
		}
		byte *e = m_FreeList.GetLast();
		m_FreeList.RemoveLast();
		cPool_Debug::UsedCount++;
		return e;
	}
	void Free(byte *Ptr) {
		if(Ptr != NULL) {
			cPool_Debug::UsedCount--;
			m_FreeList.Add(Ptr);
		}
	}
};

extern cPool_Store * COMMS_POOL[5]; // 8, 16, 32, 64, 128

// cPool
class cPool {
private:
	static inline cPool_Store * GetPoolBySize(const int Size) {
		if(NULL == COMMS_POOL[0]) {
			COMMS_POOL[0] = new cPool_Store(8);
			COMMS_POOL[1] = new cPool_Store(16);
			COMMS_POOL[2] = new cPool_Store(32);
			COMMS_POOL[3] = new cPool_Store(64);
			COMMS_POOL[4] = new cPool_Store(128);
		}
		if(Size <= 8) {
			return COMMS_POOL[0]; // 8
		}
		if(Size <= 16) {
			return COMMS_POOL[1]; // 16;
		}
		if(Size <= 32) {
			return COMMS_POOL[2]; // 32;
		}
		if(Size <= 64) {
			return COMMS_POOL[3]; // 64;
		}
		if(Size <= 128) {
			return COMMS_POOL[4]; // 128;
		}
		return NULL;
	}
public:
	static byte * Alloc(const int Size);
        static void Free(byte *Ptr, const int Size);
};
