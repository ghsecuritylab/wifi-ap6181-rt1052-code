/*
 * CDC network driver ioctl/indication encoding
 * Broadcom 802.11abg Networking Device Driver
 *
 * Definitions subject to change without notice.
 *
 * $Copyright Open Cypress Semiconductor$
 *
 * $Id: bcmcdc.h 357414 2012-09-18 02:50:45Z baysal $
 */
#ifndef _bcmcdc_h_
#define	_bcmcdc_h_
#include <proto/ethernet.h>

typedef struct cdc_ioctl {
	uint32 cmd;      /* ioctl command value */
	uint32 len;      /* lower 16: output buflen; upper 16: input buflen (excludes header) */
	uint32 flags;    /* flag defns given below */
	uint32 status;   /* status code returned from the device */
} cdc_ioctl_t;

/* Max valid buffer size that can be sent to the dongle */
#define CDC_MAX_MSG_SIZE   ETHER_MAX_LEN

/* len field is divided into input and output buffer lengths */
#define CDCL_IOC_OUTLEN_MASK   0x0000FFFF  /* maximum or expected response length, */
					   /* excluding IOCTL header */
#define CDCL_IOC_OUTLEN_SHIFT  0
#define CDCL_IOC_INLEN_MASK    0xFFFF0000   /* input buffer length, excluding IOCTL header */
#define CDCL_IOC_INLEN_SHIFT   16

/* CDC flag definitions */
#define CDCF_IOC_ERROR		0x01	/* 0=success, 1=ioctl cmd failed */
#define CDCF_IOC_SET		0x02	/* 0=get, 1=set cmd */
#define CDCF_IOC_IF_MASK	0xF000	/* I/F index */
#define CDCF_IOC_IF_SHIFT	12
#define CDCF_IOC_ID_MASK	0xFFFF0000	/* used to uniquely id an ioctl req/resp pairing */
#define CDCF_IOC_ID_SHIFT	16		/* # of bits of shift for ID Mask */

#define CDC_IOC_IF_IDX(flags)	(((flags) & CDCF_IOC_IF_MASK) >> CDCF_IOC_IF_SHIFT)
#define CDC_IOC_ID(flags)	(((flags) & CDCF_IOC_ID_MASK) >> CDCF_IOC_ID_SHIFT)

#define CDC_GET_IF_IDX(hdr) \
	((int)((((hdr)->flags) & CDCF_IOC_IF_MASK) >> CDCF_IOC_IF_SHIFT))
#define CDC_SET_IF_IDX(hdr, idx) \
	((hdr)->flags = (((hdr)->flags & ~CDCF_IOC_IF_MASK) | ((idx) << CDCF_IOC_IF_SHIFT)))

/*
 * BDC header
 *
 *   The BDC header is used on data packets to convey priority across USB.
 */

#define	BDC_HEADER_LEN		4

#define BDC_PROTO_VER		2	/* Protocol version */

#define BDC_FLAG_VER_MASK	0xf0	/* Protocol version mask */
#define BDC_FLAG_VER_SHIFT	4	/* Protocol version shift */

#ifndef EXT_STA
#define BDC_FLAG__UNUSED	0x02	/* Unassigned */
#else
#define BDC_FLAG_EXEMPT		0x03	/* Vista/EXT_STA: use for encryption exemption */
#endif /* EXT_STA */
#define BDC_FLAG_80211_PKT	0x01	/* Packet is in 802.11 format (dongle -> host) */

#define BDC_FLAG_SUM_GOOD	0x04	/* Dongle has verified good RX checksums */
#define BDC_FLAG_SUM_NEEDED	0x08	/* Dongle needs to do TX checksums: host->device */
#define BDC_FLAG_EVENT_MSG	0x08	/* Payload contains an event msg: device->host */

#define BDC_PRIORITY_MASK	0x7

#define BDC_FLAG2_IF_MASK	0x0f	/* APSTA: interface on which the packet was received */
#define BDC_FLAG2_IF_SHIFT	0

#define BDC_GET_IF_IDX(hdr) \
	((int)((((hdr)->flags2) & BDC_FLAG2_IF_MASK) >> BDC_FLAG2_IF_SHIFT))
#define BDC_SET_IF_IDX(hdr, idx) \
	((hdr)->flags2 = (((hdr)->flags2 & ~BDC_FLAG2_IF_MASK) | ((idx) << BDC_FLAG2_IF_SHIFT)))

struct bdc_header {
	uint8	flags;			/* Flags */
	uint8	priority;		/* 802.1d Priority (low 3 bits) */
	uint8	flags2;
	uint8	dataOffset;		/* Offset from end of BDC header to packet data, in
					 * 4-byte words.  Leaves room for optional headers.
					 */
};

/* XXX: Just to keep support for old twigs */
#define BDC_PROTO_VER_1		1	/* Old Protocol version */

#endif /* _bcmcdc_h_ */