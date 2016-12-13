/*
 * Generated by asn1c-0.9.23 (http://lionet.info/asn1c)
 * From ASN.1 module "DSRC"
 * 	found in "DSRC_R36_Source.ASN"
 * 	`asn1c -S ../../../../tools/asn1c/skeletons -pdu=BasicSafetyMessage -fcompound-names`
 */

#include "ProbeDataManagement.h"

static int
memb_dataElements_constraint_1(asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	size_t size;
	
	if(!sptr) {
		_ASN_CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	/* Determine the number of elements */
	size = _A_CSEQUENCE_FROM_VOID(sptr)->count;
	
	if((size >= 1 && size <= 32)) {
		/* Perform validation of the inner elements */
		return td->check_constraints(td, sptr, ctfailcb, app_key);
	} else {
		_ASN_CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static asn_TYPE_member_t asn_MBR_term_5[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct ProbeDataManagement__term, choice.termtime),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_TermTime,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"termtime"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct ProbeDataManagement__term, choice.termDistance),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_TermDistance,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"termDistance"
		},
};
static asn_TYPE_tag2member_t asn_MAP_term_tag2el_5[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* termtime at 262 */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* termDistance at 266 */
};
static asn_CHOICE_specifics_t asn_SPC_term_specs_5 = {
	sizeof(struct ProbeDataManagement__term),
	offsetof(struct ProbeDataManagement__term, _asn_ctx),
	offsetof(struct ProbeDataManagement__term, present),
	sizeof(((struct ProbeDataManagement__term *)0)->present),
	asn_MAP_term_tag2el_5,
	2,	/* Count of tags in the map */
	0,
	-1	/* Extensions start */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_term_5 = {
	"term",
	"term",
	CHOICE_free,
	CHOICE_print,
	CHOICE_constraint,
	CHOICE_decode_ber,
	CHOICE_encode_der,
	CHOICE_decode_xer,
	CHOICE_encode_xer,
	0, 0,	/* No PER support, use "-gen-PER" to enable */
	CHOICE_outmost_tag,
	0,	/* No effective tags (pointer) */
	0,	/* No effective tags (count) */
	0,	/* No tags (pointer) */
	0,	/* No tags (count) */
	0,	/* No PER visible constraints */
	asn_MBR_term_5,
	2,	/* Elements count */
	&asn_SPC_term_specs_5	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_snapshot_8[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct ProbeDataManagement__snapshot, choice.snapshotTime),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_SnapshotTime,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"snapshotTime"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct ProbeDataManagement__snapshot, choice.snapshotDistance),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_SnapshotDistance,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"snapshotDistance"
		},
};
static asn_TYPE_tag2member_t asn_MAP_snapshot_tag2el_8[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* snapshotTime at 268 */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* snapshotDistance at 270 */
};
static asn_CHOICE_specifics_t asn_SPC_snapshot_specs_8 = {
	sizeof(struct ProbeDataManagement__snapshot),
	offsetof(struct ProbeDataManagement__snapshot, _asn_ctx),
	offsetof(struct ProbeDataManagement__snapshot, present),
	sizeof(((struct ProbeDataManagement__snapshot *)0)->present),
	asn_MAP_snapshot_tag2el_8,
	2,	/* Count of tags in the map */
	0,
	-1	/* Extensions start */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_snapshot_8 = {
	"snapshot",
	"snapshot",
	CHOICE_free,
	CHOICE_print,
	CHOICE_constraint,
	CHOICE_decode_ber,
	CHOICE_encode_der,
	CHOICE_decode_xer,
	CHOICE_encode_xer,
	0, 0,	/* No PER support, use "-gen-PER" to enable */
	CHOICE_outmost_tag,
	0,	/* No effective tags (pointer) */
	0,	/* No effective tags (count) */
	0,	/* No tags (pointer) */
	0,	/* No tags (count) */
	0,	/* No PER visible constraints */
	asn_MBR_snapshot_8,
	2,	/* Elements count */
	&asn_SPC_snapshot_specs_8	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_dataElements_13[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_VehicleStatusRequest,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		""
		},
};
static ber_tlv_tag_t asn_DEF_dataElements_tags_13[] = {
	(ASN_TAG_CLASS_CONTEXT | (7 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_dataElements_specs_13 = {
	sizeof(struct ProbeDataManagement__dataElements),
	offsetof(struct ProbeDataManagement__dataElements, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_dataElements_13 = {
	"dataElements",
	"dataElements",
	SEQUENCE_OF_free,
	SEQUENCE_OF_print,
	SEQUENCE_OF_constraint,
	SEQUENCE_OF_decode_ber,
	SEQUENCE_OF_encode_der,
	SEQUENCE_OF_decode_xer,
	SEQUENCE_OF_encode_xer,
	0, 0,	/* No PER support, use "-gen-PER" to enable */
	0,	/* Use generic outmost tag fetcher */
	asn_DEF_dataElements_tags_13,
	sizeof(asn_DEF_dataElements_tags_13)
		/sizeof(asn_DEF_dataElements_tags_13[0]) - 1, /* 1 */
	asn_DEF_dataElements_tags_13,	/* Same as above */
	sizeof(asn_DEF_dataElements_tags_13)
		/sizeof(asn_DEF_dataElements_tags_13[0]), /* 2 */
	0,	/* No PER visible constraints */
	asn_MBR_dataElements_13,
	1,	/* Single element */
	&asn_SPC_dataElements_specs_13	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_ProbeDataManagement_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct ProbeDataManagement, msgID),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DSRCmsgID,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"msgID"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct ProbeDataManagement, sample),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Sample,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"sample"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct ProbeDataManagement, directions),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_HeadingSlice,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"directions"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct ProbeDataManagement, term),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_term_5,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"term"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct ProbeDataManagement, snapshot),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_snapshot_8,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"snapshot"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct ProbeDataManagement, txInterval),
		(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_TxTime,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"txInterval"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct ProbeDataManagement, cntTthreshold),
		(ASN_TAG_CLASS_CONTEXT | (6 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Count,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"cntTthreshold"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct ProbeDataManagement, dataElements),
		(ASN_TAG_CLASS_CONTEXT | (7 << 2)),
		0,
		&asn_DEF_dataElements_13,
		memb_dataElements_constraint_1,
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"dataElements"
		},
};
static ber_tlv_tag_t asn_DEF_ProbeDataManagement_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_TYPE_tag2member_t asn_MAP_ProbeDataManagement_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* msgID at 254 */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* sample at 257 */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* directions at 259 */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* term at 262 */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 }, /* snapshot at 268 */
    { (ASN_TAG_CLASS_CONTEXT | (5 << 2)), 5, 0, 0 }, /* txInterval at 271 */
    { (ASN_TAG_CLASS_CONTEXT | (6 << 2)), 6, 0, 0 }, /* cntTthreshold at 272 */
    { (ASN_TAG_CLASS_CONTEXT | (7 << 2)), 7, 0, 0 } /* dataElements at 274 */
};
static asn_SEQUENCE_specifics_t asn_SPC_ProbeDataManagement_specs_1 = {
	sizeof(struct ProbeDataManagement),
	offsetof(struct ProbeDataManagement, _asn_ctx),
	asn_MAP_ProbeDataManagement_tag2el_1,
	8,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	7,	/* Start extensions */
	9	/* Stop extensions */
};
asn_TYPE_descriptor_t asn_DEF_ProbeDataManagement = {
	"ProbeDataManagement",
	"ProbeDataManagement",
	SEQUENCE_free,
	SEQUENCE_print,
	SEQUENCE_constraint,
	SEQUENCE_decode_ber,
	SEQUENCE_encode_der,
	SEQUENCE_decode_xer,
	SEQUENCE_encode_xer,
	0, 0,	/* No PER support, use "-gen-PER" to enable */
	0,	/* Use generic outmost tag fetcher */
	asn_DEF_ProbeDataManagement_tags_1,
	sizeof(asn_DEF_ProbeDataManagement_tags_1)
		/sizeof(asn_DEF_ProbeDataManagement_tags_1[0]), /* 1 */
	asn_DEF_ProbeDataManagement_tags_1,	/* Same as above */
	sizeof(asn_DEF_ProbeDataManagement_tags_1)
		/sizeof(asn_DEF_ProbeDataManagement_tags_1[0]), /* 1 */
	0,	/* No PER visible constraints */
	asn_MBR_ProbeDataManagement_1,
	8,	/* Elements count */
	&asn_SPC_ProbeDataManagement_specs_1	/* Additional specs */
};

