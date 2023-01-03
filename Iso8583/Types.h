/*------------------------------------------------------------
* FileName: ISO8583_Types.h
* Author: oHosseini
------------------------------------------------------------*/
#ifndef ISO8583_TYPES_H
#define ISO8583_TYPES_H

// ============================================ Constants
#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef BOOL
#define BOOL char
#endif

#ifndef ushort_t
#define ushort_t unsigned short int
#endif

#ifndef int8_t
#define int8_t  char
#endif

#ifndef uint8_t
#define uint8_t unsigned char
#endif

#ifndef uint16_t
#define uint16_t unsigned int
#endif

#ifndef int32_t
#define int32_t long
#endif

#ifndef uint32_t
#define uint32_t unsigned long
#endif

#ifndef int64_t
#define int64_t long long int
#endif

#ifndef uint64_t
#define uint64_t unsigned long long int
#endif

#ifndef uint
#define uint unsigned int
#endif

#define _LOG printf

//==============================================

#define ISO8583_MSGLEN_MAX	8192    //255
#define MAX_RESPONSE_CODE	100

// =============================== Field Number and ID

#define FIELD_MAX								0x41
#define F00_MSG_TYPE_ID							0x00
#define F01_BITMAP								0x01
#define F02_PAN									0x02
#define F03_PROCESSING_CODE						0x03
#define F04_TRXN_AMOUNT							0x04
#define F05_RECONCILIATION_AMOUNT				0x05
#define F06_CARDHOLDER_BILLING_AMOUNT			0x06
#define F07_TX_DATE_AND_TIME					0x07
#define F08_CARDHOLDER_BILLING_FEE_AMOUNT		0x08
#define F09_CONVERSION_RATE_RECONCILIATION		0x09
#define F10_CONVERSION_RATE_CARDHOLDER_BILLING	0x0A
#define F11_STAN								0x0B
#define F12_TRXN_TIME							0x0C
#define F13_TRXN_DATE							0x0D
#define F14_EXPIRY_DATE							0x0E
#define F15_SETTLEMENT_DATE						0x0F
#define F16_CONVERSION_DATE						0x10
#define F17_CAPTURE_DATE						0x11
#define F18_MERCHANT_TYPE						0x12
#define F19_ACQUIRING_INSTITUTION_COUNTRY_CODE	0x13
#define F20_PRIMARY_ACCOUNT_NUMBER_COUNTRY_CODE	0x14
#define F21_FORWARDING_INSTITUTION_COUNTRY_CODE	0x15
#define F22_POS_ENTRY_MODE						0x16
#define F23_APP_PAN_NUMBER						0x17
#define F24_NII									0x18
#define F25_POS_CONDITION_CODE					0x19
#define F26_PIN_CAPURE_CODE						0x1A
#define F27_APPROVAL_CODE_LENGTH				0x1B
#define F28_TRANSACTION_AMOUNT_FEE				0x1C
#define F29_SETTLEMENT_AMOUNT_FEE				0x1D
#define F30_TRANSACTION_AMOUNT_PROCESSING_FEE	0x1E
#define F31_SETTLEMENT_AMOUNT_PROCESSING_FEE	0x1F
#define F32_ACQUIRING_INSTITUTION_ID_CODE		0x20
#define F33_FORWARDING_INSTITUTION_ID_CODE		0x21
#define F34_EXTENDED_PRIMARY_ACCOUNT_NUMBER		0x22
#define F35_TRACK_2_DATA						0x23
#define F36_TRACK_3_DATA						0x24
#define F37_RRN									0x25
#define F38_AUTH_ID_RESPONSE					0x26
#define F39_RESPONSE_CODE						0x27
#define F40_SERVICE_CODE						0x28
#define F41_CARD_ACCEPTOR_TERMINAL_ID			0x29
#define F42_CARD_ACCEPTOR_ID					0x2A
#define F43_CARD_ACCEPTOR_NAME_LOCATION			0x2B
#define F44_ADDITIONAL_RESPONSE_DATA			0x2C
#define F45_TRACK_1_DATA						0x2D
#define F46_ADDITIONAL_DATA_ISO					0x2E
#define F47_ADD_DATA_NATIONAL					0x2F
#define F48_ADD_DATA_PRIVATE					0x30
#define F49_TRANSACTION_CURRENCY_CODE			0x31
#define F50_SETTLEMENT_CURRENCY_CODE			0x32
#define F51_CARDHOLDER_BILLING_CURRENCY_CODE	0x33
#define F52_PIN_DATA							0x34
#define F53_SECURITY_CONTROL_INFO				0x35
#define F54_ADDITIONAL_AMOUNT					0x36
#define F55_RESERVED_ISO						0x37
#define F56_RESERVED_ISO						0x38
#define F57_RESERVED_NATIONAL					0x39
#define F58_RESERVED_NATIONAL					0x3A
#define F59_TRANSPORT_DATA						0x3B
#define F60_PRIVATE_USE							0x3C
#define F61_PRIVATE_USE							0x3D
#define F62_PRIVATE_USE							0x3E
#define F63_PRIVATE_USE							0x3F
#define F64_MESSAGE_AUTH_CODE					0x40

#define F00_MSG_TYPE_ID_LEN							4
#define F01_BITMAP_LEN								8
#define F02_PAN_LEN									19
#define F03_PROCESSING_CODE_LEN						6
#define F04_TRXN_AMOUNT_LEN							12
#define F05_RECONCILIATION_AMOUNT_LEN				12
#define F06_CARDHOLDER_BILLING_AMOUNT_LEN			12
#define F07_TX_DATE_AND_TIME_LEN					10
#define F08_CARDHOLDER_BILLING_FEE_AMOUNT_LEN		8
#define F09_CONVERSION_RATE_RECONCILIATION_LEN		8
#define F10_CONVERSION_RATE_CARDHOLDER_BILLING_LEN	8
#define F11_STAN_LEN								6
#define F12_TRXN_TIME_LEN							6
#define F13_TRXN_DATE_LEN							4
#define F14_EXPIRY_DATE_LEN							4
#define F15_SETTLEMENT_DATE_LEN						4
#define F16_CONVERSION_DATE_LEN						4
#define F17_CAPTURE_DATE_LEN						4
#define F18_MERCHANT_TYPE_LEN						4
#define F19_ACQUIRING_INSTITUTION_COUNTRY_CODE_LEN	3
#define F20_PRIMARY_ACCOUNT_NUMBER_COUNTRY_CODE_LEN	3
#define F21_FORWARDING_INSTITUTION_COUNTRY_CODE_LEN	3
#define F22_POS_ENTRY_MODE_LEN						3
#define F23_APP_PAN_NUMBER_LEN						3
#define F24_NII_LEN									3
#define F25_POS_CONDITION_CODE_LEN					2
#define F26_PIN_CAPURE_CODE_LEN						2
#define F27_APPROVAL_CODE_LENGTH_LEN				1
#define F28_TRANSACTION_AMOUNT_FEE_LEN				8
#define F29_SETTLEMENT_AMOUNT_FEE_LEN				8
#define F30_TRANSACTION_AMOUNT_PROCESSING_FEE_LEN	8
#define F31_SETTLEMENT_AMOUNT_PROCESSING_FEE_LEN	8
#define F32_ACQUIRING_INSTITUTION_ID_CODE_LEN		11
#define F33_FORWARDING_INSTITUTION_ID_CODE_LEN		11
#define F34_EXTENDED_PRIMARY_ACCOUNT_NUMBER_LEN		28
#define F35_TRACK_2_DATA_LEN						40
#define F36_TRACK_3_DATA_LEN						50
#define F37_RRN_LEN									12
#define F38_AUTH_ID_RESPONSE_LEN					6
#define F39_RESPONSE_CODE_LEN						2
#define F40_SERVICE_CODE_LEN						3
#define F41_CARD_ACCEPTOR_TERMINAL_ID_LEN			8
#define F42_CARD_ACCEPTOR_ID_LEN					15
#define F43_CARD_ACCEPTOR_NAME_LOCATION_LEN			40
#define F44_ADDITIONAL_RESPONSE_DATA_LEN			25
#define F45_TRACK_1_DATA_LEN						76
#define F46_ADDITIONAL_DATA_ISO_LEN					79
#define F47_ADD_DATA_NATIONAL_LEN					200
#define F48_ADD_DATA_PRIVATE_LEN					200
#define F49_TRANSACTION_CURRENCY_CODE_LEN			3
#define F50_SETTLEMENT_CURRENCY_CODE_LEN			3
#define F51_CARDHOLDER_BILLING_CURRENCY_CODE_LEN	3
#define F52_PIN_DATA_LEN							8
#define F53_SECURITY_CONTROL_INFO_LEN				18
#define F54_ADDITIONAL_AMOUNT_LEN					120
#define F55_RESERVED_ISO_LEN						999
#define F56_RESERVED_ISO_LEN						999
#define F57_RESERVED_NATIONAL_LEN					999
#define F58_RESERVED_NATIONAL_LEN					999
#define F59_TRANSPORT_DATA_LEN						999
#define F60_PRIVATE_USE_LEN							999
#define F61_PRIVATE_USE_LEN							999
#define F62_PRIVATE_USE_LEN							999
#define F63_PRIVATE_USE_LEN							999
#define F64_MESSAGE_AUTH_CODE_LEN					8

#endif
