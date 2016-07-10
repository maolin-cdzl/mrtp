#ifndef __MRTP_MBLK_H__
#define __MRTP_MBLK_H__

#include "mrtp/port.h"

typedef struct _mblk_t mblk_t;
typedef struct _mblk_allocator mblk_allocator;


struct _mblk_t {
	mblk_allocator*		alloc;
	byte_t*				base;
	byte_t*				limit;
	byte_t*				rptr;
	byte_t*				wptr;
	int					ref;
};

struct _mblk_allocator {
	void (*destroy)(mblk_allocator** p_self);

	mblk_t* (*alloc)(size_t size);
	void (*release)(mblk_t** p_mblk);
};

MRTP_EXTERN_METHOD mblk_allocator* mblk_allocator_new();

#endif

