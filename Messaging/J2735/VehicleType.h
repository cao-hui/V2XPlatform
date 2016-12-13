/*
 * Generated by asn1c-0.9.23 (http://lionet.info/asn1c)
 * From ASN.1 module "DSRC"
 * 	found in "DSRC_R36_Source.ASN"
 * 	`asn1c -S ../../../../tools/asn1c/skeletons -pdu=BasicSafetyMessage -fcompound-names`
 */

#ifndef	_VehicleType_H_
#define	_VehicleType_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum VehicleType {
	VehicleType_none	= 0,
	VehicleType_unknown	= 1,
	VehicleType_special	= 2,
	VehicleType_moto	= 3,
	VehicleType_car	= 4,
	VehicleType_carOther	= 5,
	VehicleType_bus	= 6,
	VehicleType_axleCnt2	= 7,
	VehicleType_axleCnt3	= 8,
	VehicleType_axleCnt4	= 9,
	VehicleType_axleCnt4Trailer	= 10,
	VehicleType_axleCnt5Trailer	= 11,
	VehicleType_axleCnt6Trailer	= 12,
	VehicleType_axleCnt5MultiTrailer	= 13,
	VehicleType_axleCnt6MultiTrailer	= 14,
	VehicleType_axleCnt7MultiTrailer	= 15
	/*
	 * Enumeration is extensible
	 */
} e_VehicleType;

/* VehicleType */
typedef long	 VehicleType_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_VehicleType;
asn_struct_free_f VehicleType_free;
asn_struct_print_f VehicleType_print;
asn_constr_check_f VehicleType_constraint;
ber_type_decoder_f VehicleType_decode_ber;
der_type_encoder_f VehicleType_encode_der;
xer_type_decoder_f VehicleType_decode_xer;
xer_type_encoder_f VehicleType_encode_xer;

#ifdef __cplusplus
}
#endif

#endif	/* _VehicleType_H_ */
#include <asn_internal.h>
