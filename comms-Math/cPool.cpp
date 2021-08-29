#include "comms-Math.h"

namespace comms {


// cPool::Alloc
byte * cPool::Alloc(const int Size) {
    if(Size <= 0) {
        return NULL;
    }
    byte *r = NULL;
    cPool_Store *P = GetPoolBySize(Size);
    if(P != NULL) {
        r = P->Allocate();
    }
    if(NULL == r) {
        r = new byte[Size];
    }
    return r;
}

// cPool::Free
void cPool::Free(byte *Ptr, const int Size) {
    cPool_Store *P = GetPoolBySize(Size);
    if(P != NULL) {
        P->Free(Ptr);
        Ptr = NULL;
    }
    if(Ptr != NULL) {
        delete[] Ptr;
    }
}

cPool_Store * COMMS_POOL[5] = { // 8, 16, 32, 64, 128
	NULL, NULL, NULL, NULL, NULL
};

int cPool_Debug::UsedCount = 0;

} // comms
