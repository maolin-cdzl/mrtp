#ifndef __MRTP_SCHEDULER_H__
#define __MRTP_SCHEDULER_H__

#include "mrtp/port.h"


typedef struct _MRTP_Scheduler MRTP_Scheduler;

typedef void (*MRTP_Scheduler_CB)(MRTP_Scheduler* scheduler,uint32_t event,void* user);

struct _MRTP_Scheduler {
	void (*destroy)(MRTP_Scheduler** p_self);
	
	int (*schedule)(MRTP_Scheduler* self,uint64_t uptime,MRTP_Scheduler_CB cb,void* user);
};

MRTP_EXTERN_METHOD MRTP_Scheduler* mrtp_scheduler_new();

#endif

