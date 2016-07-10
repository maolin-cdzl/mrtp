#ifndef __MRTP_PORT_H__
#define __MRTP_PORT_H__

// TODO: support other platform. now just for Linux
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef uint8_t			byte_t;

#define MRTP_INLINE			inline
#define MRTP_EXTERN_METHOD extern
#define MRTP_PUBLIC			

MRTP_EXTERN_METHOD uint64_t mrtp_wallclock();
MRTP_EXTERN_METHOD uint64_t mrtp_uptime();
MRTP_EXTERN_METHOD uint64_t mrtp_rtp_ntp();

#endif
