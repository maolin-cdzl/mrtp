/**
 * @file byteorder.h
 * \brief 定义了用来进行字节顺序转换的宏
 * @author MaoLin maolin@shanlitech.com
 * @version 1.0
 * @date 2012-06-04
 */

#ifndef	__MRTP_BYTEORDER_H__
#define __MRTP_BYTEORDER_H__

#include "mrtp/port.h"

// from linux source
#define SWAP_16(x) \
((uint16_t)( \
	(((uint16_t)(x) & (uint16_t)0x00ffU) << 8) | \
	(((uint16_t)(x) & (uint16_t)0xff00U) >> 8) ))
#define SWAP_32(x) \
((uint32_t)( \
	(((uint32_t)(x) & (uint32_t)0x000000ffUL) << 24) | \
	(((uint32_t)(x) & (uint32_t)0x0000ff00UL) << 8) | \
	(((uint32_t)(x) & (uint32_t)0x00ff0000UL) >> 8) | \
	(((uint32_t)(x) & (uint32_t)0xff000000UL) >> 24) ))

#if MRTP_SUPPORT_INT64
#define SWAP_64(x) \
((uint64_t)( \
	(uint64_t)(((uint64_t)(x) & C_UINT64(0x00000000000000ff)) << 56) | \
	(uint64_t)(((uint64_t)(x) & C_UINT64(0x000000000000ff00)) << 40) | \
	(uint64_t)(((uint64_t)(x) & C_UINT64(0x0000000000ff0000)) << 24) | \
	(uint64_t)(((uint64_t)(x) & C_UINT64(0x00000000ff000000)) << 8) | \
	(uint64_t)(((uint64_t)(x) & C_UINT64(0x000000ff00000000)) >> 8) | \
	(uint64_t)(((uint64_t)(x) & C_UINT64(0x0000ff0000000000)) >> 24) | \
	(uint64_t)(((uint64_t)(x) & C_UINT64(0x00ff000000000000)) >> 40) | \
	(uint64_t)(((uint64_t)(x) & C_UINT64(0xff00000000000000)) >> 56) ))
#endif

#ifdef MRTP_BIGENDIAN
#	if MRTP_SUPPORT_INT64
#		define SWAP_64_ON_BE(x)					SWAP_64(x)
#		define SWAP_64_ON_LE(x)					(x)
#	endif
#	define SWAP_32_ON_BE(x)					SWAP_32(x)
#	define SWAP_16_ON_BE(x)					SWAP_16(x)
#	define SWAP_32_ON_LE(x)					(x)
#	define SWQP_16_ON_LE(x)					(x)

static MRTP_INLINE uint32_t mrtp_htonl(uint32_t x) { return x; }
static MRTP_INLINE uint32_t mrtp_ntohl(uint32_t x) { return x; }
static MRTP_INLINE uint16_t mrtp_htons(uint16_t x) { return x; }
static MRTP_INLINE uint16_t mrtp_ntohs(uint16_t x) { return x; }

#else
#	if MRTP_SUPPORT_INT64
#		define SWAP_64_ON_BE(x)					(x)
#		define SWAP_64_ON_LE(x)					SWAP_64(x)
#	endif
#	define SWAP_32_ON_BE(x)					(x)
#	define SWAP_16_ON_BE(x)					(x)
#	define SWAP_32_ON_LE(x)					SWAP_32(x)
#	define SWAP_16_ON_LE(x)					SWAP_16(x)

static MRTP_INLINE uint32_t mrtp_htonl(uint32_t x) { return SWAP_32(x); }
static MRTP_INLINE uint32_t mrtp_ntohl(uint32_t x) { return SWAP_32(x); }
static MRTP_INLINE uint16_t mrtp_htons(uint16_t x) { return SWAP_16(x); }
static MRTP_INLINE uint16_t mrtp_ntohs(uint16_t x) { return SWAP_16(x); }

#endif


#endif


