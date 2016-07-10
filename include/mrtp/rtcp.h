#ifndef __MRTP_RTCP_H__
#define __MRTP_RTCP_H__

#include "mrtp/port.h"
#include "mrtp/byteorder.h"


#define RTCP_MAX_RECV_BUFSIZE 1500

#define RTCP_SENDER_INFO_SIZE 20
#define RTCP_REPORT_BLOCK_SIZE 24
#define RTCP_COMMON_HEADER_SIZE 4
#define RTCP_SSRC_FIELD_SIZE 4

typedef enum {
	RTCP_SR = 200,
	RTCP_RR = 201,
	RTCP_SDES = 202,
	RTCP_BYE = 203,
	RTCP_APP = 204,
	RTCP_RTPFB = 205,
	RTCP_PSFB = 206,
	RTCP_XR = 207
} rtcp_type_t;


typedef struct rtcp_common_header
{
#ifdef MRTP_BIGENDIAN
	uint16_t version:2;
	uint16_t padbit:1;
	uint16_t rc:5;
	uint16_t packet_type:8;
#else
	uint16_t rc:5;
	uint16_t padbit:1;
	uint16_t version:2;
	uint16_t packet_type:8;
#endif
	uint16_t length:16;
} rtcp_common_header_t;

#define rtcp_common_header_set_version(ch,v) (ch)->version=v
#define rtcp_common_header_set_padbit(ch,p) (ch)->padbit=p
#define rtcp_common_header_set_rc(ch,rc) (ch)->rc=rc
#define rtcp_common_header_set_packet_type(ch,pt) (ch)->packet_type=pt
#define rtcp_common_header_set_length(ch,l)	(ch)->length=htons(l)

#define rtcp_common_header_get_version(ch) ((ch)->version)
#define rtcp_common_header_get_padbit(ch) ((ch)->padbit)
#define rtcp_common_header_get_rc(ch) ((ch)->rc)
#define rtcp_common_header_get_packet_type(ch) ((ch)->packet_type)
#define rtcp_common_header_get_length(ch)	ntohs((ch)->length)


/* SR or RR  packets */

typedef struct sender_info
{
	uint32_t ntp_timestamp_msw;
	uint32_t ntp_timestamp_lsw;
	uint32_t rtp_timestamp;
	uint32_t senders_packet_count;
	uint32_t senders_octet_count;
} sender_info_t;

static MRTP_INLINE uint64_t sender_info_get_ntp_timestamp(const sender_info_t *si) {
  return ((((uint64_t)mrtp_ntohl(si->ntp_timestamp_msw)) << 32) +
          ((uint64_t) mrtp_ntohl(si->ntp_timestamp_lsw)));
}


#define sender_info_get_rtp_timestamp(si)	mrtp_ntohl((si)->rtp_timestamp)
#define sender_info_get_packet_count(si) \
	mrtp_ntohl((si)->senders_packet_count)
#define sender_info_get_octet_count(si) \
	mrtp_ntohl((si)->senders_octet_count)


typedef struct report_block
{
	uint32_t ssrc;
	uint32_t fl_cnpl;/*fraction lost + cumulative number of packet lost*/
	uint32_t ext_high_seq_num_rec; /*extended highest sequence number received */
	uint32_t interarrival_jitter;
	uint32_t lsr; /*last SR */
	uint32_t delay_snc_last_sr; /*delay since last sr*/
} report_block_t;

static MRTP_INLINE uint32_t report_block_get_ssrc(const report_block_t * rb) {
	return mrtp_ntohl(rb->ssrc);
}
static MRTP_INLINE uint32_t report_block_get_high_ext_seq(const report_block_t * rb) {
	return mrtp_ntohl(rb->ext_high_seq_num_rec);
}
static MRTP_INLINE uint32_t report_block_get_interarrival_jitter(const report_block_t * rb) {
	return mrtp_ntohl(rb->interarrival_jitter);
}

static MRTP_INLINE uint32_t report_block_get_last_SR_time(const report_block_t * rb) {
	return mrtp_ntohl(rb->lsr);
}
static MRTP_INLINE uint32_t report_block_get_last_SR_delay(const report_block_t * rb) {
	return mrtp_ntohl(rb->delay_snc_last_sr);
}
static MRTP_INLINE uint32_t report_block_get_fraction_lost(const report_block_t * rb) {
	return (mrtp_ntohl(rb->fl_cnpl)>>24);
}
static MRTP_INLINE int32_t report_block_get_cum_packet_lost(const report_block_t * rb){
	int cum_loss = mrtp_ntohl(rb->fl_cnpl);
	if (((cum_loss>>23)&1)==0)
		return 0x00FFFFFF & cum_loss;
	else
		return 0xFF000000 | (cum_loss-0xFFFFFF-1);
}

static MRTP_INLINE void report_block_set_fraction_lost(report_block_t * rb, int fl){
	rb->fl_cnpl = mrtp_htonl( (mrtp_ntohl(rb->fl_cnpl) & 0xFFFFFF) | (fl&0xFF)<<24);
}

static MRTP_INLINE void report_block_set_cum_packet_lost(report_block_t * rb, int64_t cpl) {
	uint32_t clamp = (uint32_t)((1<<24) + ((cpl>=0) ? (cpl>0x7FFFFF?0x7FFFFF:cpl) : (-cpl>0x800000?-0x800000:cpl)));
	rb->fl_cnpl=mrtp_htonl(
			(mrtp_ntohl(rb->fl_cnpl) & 0xFF000000) |
			(cpl >= 0 ? clamp&0x7FFFFF : clamp|0x800000)
		);
}

/* SDES packets */

typedef enum {
	RTCP_SDES_END = 0,
	RTCP_SDES_CNAME = 1,
	RTCP_SDES_NAME = 2,
	RTCP_SDES_EMAIL = 3,
	RTCP_SDES_PHONE = 4,
	RTCP_SDES_LOC = 5,
	RTCP_SDES_TOOL = 6,
	RTCP_SDES_NOTE = 7,
	RTCP_SDES_PRIV = 8,
	RTCP_SDES_MAX = 9
} rtcp_sdes_type_t;

typedef struct sdes_chunk {
	uint32_t csrc;
} sdes_chunk_t;


#define sdes_chunk_get_csrc(c)	mrtp_ntohl((c)->csrc)

typedef struct sdes_item {
	uint8_t item_type;
	uint8_t len;
	char content[1];
} sdes_item_t;

#define RTCP_SDES_MAX_STRING_SIZE 255
#define RTCP_SDES_ITEM_HEADER_SIZE 2
#define RTCP_SDES_CHUNK_DEFAULT_SIZE 1024
#define RTCP_SDES_CHUNK_HEADER_SIZE (sizeof(sdes_chunk_t))

/* RTCP bye packet */

typedef struct rtcp_bye_reason {
	uint8_t len;
	char content[1];
} rtcp_bye_reason_t;

typedef struct rtcp_bye {
	rtcp_common_header_t ch;
	uint32_t ssrc[1];  /* the bye may contain several ssrc/csrc */
} rtcp_bye_t;

#define RTCP_BYE_HEADER_SIZE sizeof(rtcp_bye_t)
#define RTCP_BYE_REASON_MAX_STRING_SIZE 255



#endif

