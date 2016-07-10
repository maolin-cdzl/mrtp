#ifndef __MRTP_TRANSPORT_H__
#define __MRTP_TRANSPORT_H__

#include "mrtp/port.h"
#include "mrtp/mblk.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MRTP_NETWORK_FAMILY_IPV4		0x1
#define MRTP_NETWORK_FAMILY_IPV6		0x2
#define MRTP_NETWORK_MULTICAST			0x4
#define MRTP_NETWORK_UNICAST			0x8
#define MRTP_NETWORK_REUSE_ADDR			0x10
#define MRTP_NETWORK_BLOCKING			0x20

typedef struct _MRTP_Endpoint {
	uint16_t	flags;
	uint16_t	port;
	byte_t		ip[6];
} MRTP_Endpoint;


int mrtp_endpoint_init(MRTP_Endpoint* endpoint,uint16_t flags,const char* ip,uint16_t port);


typedef struct _MRTP_Transport MRTP_Transport;

typedef void (*MRTP_Transport_ReadableCB)(MRTP_Transport* transport);

struct _MRTP_Transport {
	void (*destroy)(MRTP_Transport** p_self);
	void (*readable)(MRTP_Transport* self,MRTP_Transport_ReadableCB cb);

	int (*sendto)(MRTP_Transport* self,mblk_t* msg,MRTP_Endpoint* endpoint);
	int (*recvfrom)(MRTP_Transport* self,mblk_t* msg,MRTP_Endpoint* endpoint);
	void (*close)(MRTP_Transport* self);

};

MRTP_EXTERN_METHOD MRTP_Transport* mrtp_transport_connect(uint16_t flags,const MRTP_Endpoint* endpoint);
MRTP_EXTERN_METHOD MRTP_Transport* mrtp_transport_bind(uint16_t flags,const MRTP_Endpoint* endpoint);


#ifdef __cplusplus
}
#endif
#endif //#ifndef __MRTP_TRANSPORT_H__
