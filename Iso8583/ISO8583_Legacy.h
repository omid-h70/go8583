// ISO8583_Legacy.h

#pragma once

#include "Types.h"
// Constants

#ifndef t_NULL
#define t_NULL 0
#endif

#define ISO8583_MSGLEN_MAX	8192    //255
#define MAX_RESPONSE_CODE	100


typedef enum iso8583_errcodes_t
{
	ISO8583_SUCCESS,
	ISO8583_UNKNOWN,
	ISO8583_NOT_IMPLEMENTED,
	ISO8583_NOT_COMPATIBLE,
	ISO8583_INVALID_BITMAP
} ISO8583_ERRCODES;


typedef struct iso8583_fields_t
{
	int		nFieldID;
	int		nFieldType;
	void*	pLink;
	int		nLength;
	int		nLengthType;
	int		nLimit;
} ISO8583_FIELDS;

typedef struct iso8583_msg_fields_t
{
	uint8_t	MsgTypeId[F00_MSG_TYPE_ID_LEN + 1];												        // Message Type Indicator
	uint8_t	Bitmap[F01_BITMAP_LEN + 1];									    				        // Bitmap
	uint8_t	F02_Pan[F02_PAN_LEN + 1];													            // Primary Account Number
	uint8_t	F03_ProcessingCode[F03_PROCESSING_CODE_LEN + 1];									    // Processing Code
	uint8_t	F04_TrxnAmount[F04_TRXN_AMOUNT_LEN + 1];										        // Amount, Txn
	uint8_t	F05_SettlementAmount[F05_RECONCILIATION_AMOUNT_LEN + 1];								// Amount, Settlement
	uint8_t	F06_CardholderBillingAmount[F06_CARDHOLDER_BILLING_AMOUNT_LEN + 1];					    // Amount, Cardholder Billing
	uint8_t	F07_TxDateAndTime[F07_TX_DATE_AND_TIME_LEN + 1];										// Date and Time, Transmission
	uint8_t	F08_CardholderBillingFeeAmount[F08_CARDHOLDER_BILLING_FEE_AMOUNT_LEN + 1];				// Amount, Cardholder Billing Fee
	uint8_t	F09_ConversionRateSettlement[F09_CONVERSION_RATE_RECONCILIATION_LEN + 1];				// Conversion Rate, Settlement
	uint8_t	F10_ConversionRateCardholderBilling[F10_CONVERSION_RATE_CARDHOLDER_BILLING_LEN + 1];	// Conversion Rate, Cardholder Billing
	uint8_t	F11_Stan[F11_STAN_LEN + 1];															    // Systems Trace Audit Number
	uint8_t	F12_TrxnTime[F12_TRXN_TIME_LEN + 1];													// Time, Local Txn
	uint8_t	F13_TrxnDate[F13_TRXN_DATE_LEN + 1];													// Date, Effective
	uint8_t	F14_ExpiryDate[F14_EXPIRY_DATE_LEN + 1];												// Date, Expiration
	uint8_t	F15_SettlementDate[F15_SETTLEMENT_DATE_LEN + 1];										// Date, Settlement
	uint8_t	F16_ConversionDate[F16_CONVERSION_DATE_LEN + 1];										// Date, Conversion
	uint8_t	F17_CaptureDate[F17_CAPTURE_DATE_LEN + 1];												// Date, Capture
	uint8_t	F18_MerchantType[F18_MERCHANT_TYPE_LEN + 1];											// Merchant Type
	uint8_t F19_AcquiringInstitutionCountryCode[F19_ACQUIRING_INSTITUTION_COUNTRY_CODE_LEN + 1];
	uint8_t	F20_PrimaryAccountNumberCountryCode[F20_PRIMARY_ACCOUNT_NUMBER_COUNTRY_CODE_LEN + 1];	// Country Code, Primary Account Number
	uint8_t	F21_ForwardingInstitutionCountryCode[F21_FORWARDING_INSTITUTION_COUNTRY_CODE_LEN + 1];  // Country Code, Forwarding Inst
	uint8_t	F22_POSEntryMode[F22_POS_ENTRY_MODE_LEN + 1];											// Point of Service Data Code
	uint8_t	F23_AppPanNumber[F23_APP_PAN_NUMBER_LEN + 1];											// Application PAN Number
	uint8_t	F24_NetworkInternationalID[F24_NII_LEN + 1];		    							    // NII
	uint8_t	F25_POSConditionCode[F25_POS_CONDITION_CODE_LEN + 1];									// POS Condition Code
	uint8_t	F26_PINCaptureCode[F26_PIN_CAPURE_CODE_LEN + 1];										// PIN Capture Code
	uint8_t	F27_AuthIdRespLength[F27_APPROVAL_CODE_LENGTH_LEN + 1];									// Approval Code Length
	uint8_t	F28_TrxnAmountFee[F28_TRANSACTION_AMOUNT_FEE_LEN + 1];									// Transaction Amount Fee
	uint8_t	F29_SettlementAmountFee[F29_SETTLEMENT_AMOUNT_FEE_LEN + 1];								// Reconciliation Indicator
	uint8_t	F30_TransactionAmountProcessingFee[F30_TRANSACTION_AMOUNT_PROCESSING_FEE_LEN + 1];      // Amount, Transaction Processing Fee
	uint8_t	F31_SettlementAmountProcessingFee[F31_SETTLEMENT_AMOUNT_PROCESSING_FEE_LEN + 1];		// Amount, Settlement Processing Fee
	uint8_t	F32_AcquiringInstitutionIdCode[F32_ACQUIRING_INSTITUTION_ID_CODE_LEN + 1];		    	// Acquire Inst Id Code
	uint8_t	F33_ForwardingInstitutionIdCode[F33_FORWARDING_INSTITUTION_ID_CODE_LEN + 1];			// Forwarding Inst Id Code
	uint8_t	F34_ExtendedPrimaryAccountNumber[F34_EXTENDED_PRIMARY_ACCOUNT_NUMBER_LEN + 1];			// Primary Account Number, Extended
	uint8_t	F35_Track2Data[F35_TRACK_2_DATA_LEN + 1];												// Track 2 Data
	uint8_t	F36_Track3Data[F36_TRACK_3_DATA_LEN + 1];												// Track 3 Data=> Original Len = 104 + 1 = 105
	uint8_t	F37_Rrn[F37_RRN_LEN + 1];																// Retrieval Reference Number
	uint8_t	F38_AuthIdResp[F38_AUTH_ID_RESPONSE_LEN + 1];											// Authorization Identification Response
	uint8_t	F39_ResponseCode[F39_RESPONSE_CODE_LEN + 1];											// Response Code
	uint8_t	F40_ServiceCode[F40_SERVICE_CODE_LEN + 1];												// Service Code
	uint8_t	F41_CardAcceptorTerminalId[F41_CARD_ACCEPTOR_TERMINAL_ID_LEN + 1];						// Card Acceptor Terminal Id
	uint8_t	F42_CardAcceptorId[F42_CARD_ACCEPTOR_ID_LEN + 1];										// Card Acceptor Id Code
	uint8_t	F43_CardAcceptorNameLocation[F43_CARD_ACCEPTOR_NAME_LOCATION_LEN + 1];					// Card Acceptor Name/Location
	uint8_t	F44_AdditionalResponseData[F44_ADDITIONAL_RESPONSE_DATA_LEN + 1];						// Additional Response Data
	uint8_t	F45_Track1Data[F45_TRACK_1_DATA_LEN + 1];												// Track 1 Data
	uint8_t	F46_AdditionalDataIso[F46_ADDITIONAL_DATA_ISO_LEN + 1];									// Additional Data - ISO
	uint8_t	F47_AddDataNational[F47_ADD_DATA_NATIONAL_LEN + 1];										// Additional Data - National
	uint8_t	F48_AddDataPrivate[F48_ADD_DATA_PRIVATE_LEN + 1];										// Additional Data - Private
	uint8_t	F49_TransactionCurrencyCode[F49_TRANSACTION_CURRENCY_CODE_LEN + 1];						// Currency Code, Txn
	uint8_t	F50_SettlementCurrencyCode[F50_SETTLEMENT_CURRENCY_CODE_LEN + 1];						// Currency Code, Settlement
	uint8_t	F51_CardholderBillingCurrencyCode[F51_CARDHOLDER_BILLING_CURRENCY_CODE_LEN + 1];		// Currency Code, Cardholder Billing
	uint8_t	F52_PinData[F52_PIN_DATA_LEN + 1];														// Personal Id Number (PIN) Data
	uint8_t	F53_SecurityControlInfo[F53_SECURITY_CONTROL_INFO_LEN + 1];								// Security Related Control Information
	uint8_t	F54_AdditionalAmount[F54_ADDITIONAL_AMOUNT_LEN + 1];									// Amounts, Additional
	uint8_t	F55_ReservedISO[F55_RESERVED_ISO_LEN + 1];												// Reserved ISO
	uint8_t	F56_ReservedISO[F56_RESERVED_ISO_LEN + 1];												// Reserved ISO
	uint8_t	F57_ReservedNational[F57_RESERVED_NATIONAL_LEN + 1];									// Reserved National                - 201 -> 1000
	uint8_t	F58_ReservedNational[F58_RESERVED_NATIONAL_LEN + 1];									// Reserved National                - 201 -> 1000
	uint8_t	F59_Transportdata[F59_TRANSPORT_DATA_LEN + 1];											// Transport Data                   - 201 -> 1000
	uint8_t	F60_PrivateUse[F60_PRIVATE_USE_LEN + 1];												// Reserved for National use        - 201 -> 1000
	uint8_t	F61_PrivateUse[F61_PRIVATE_USE_LEN + 1];												// Reserved for National use        - 201 -> 1000
	uint8_t	F62_PrivateUse[F62_PRIVATE_USE_LEN + 1];												// Reserved for Private use         - 201 -> 1000
	uint8_t	F63_PrivateUse[F63_PRIVATE_USE_LEN + 1];												// Reserved for Private use         - 256 -> 1000
	uint8_t	F64_MessageAuthCode[F64_MESSAGE_AUTH_CODE_LEN + 1];										// Message Authentication Code Field- 201 -> 1000
} ISO8583_MSG_FIELDS;

typedef struct t_msg_tmp_buff
{
	uint8_t	cBuffer[ISO8583_MSGLEN_MAX];
	uint8_t	cWorker[ISO8583_MSGLEN_MAX];
	uint8_t	cPrefix[5];

} T_msg_tmp_buff;

typedef struct t_ISO8583_MSG_STRUCT
{
	uint8_t     	        cBitmap[8];
	ISO8583_FIELDS          ISO8583_FIELD_TBL_EX[FIELD_MAX];
	int                     ISO8583_FIELD_TBL_EX_INDEX;
	ISO8583_MSG_FIELDS		msgFields;
	BOOL				    bUseField[FIELD_MAX];

} t_ISO8583_MSG_STRUCT;

//BOOL ValidNumber(uint8_t* s);
void ISO8583_SetBitmap(uint8_t* lpBitmap, int nField);

//+++++++++++++++++++++++++++++++++++++++++ <Thread Safe Functions> +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void			    ISO8583_ResetEx(t_ISO8583_MSG_STRUCT* iso_struct);
void			    ISO8583_SetFieldEx(t_ISO8583_MSG_STRUCT* iso_struct, int nField, uint8_t* lpData, int nLength);
int					ISO8583_BuildEx(t_ISO8583_MSG_STRUCT* iso_struct, uint8_t* lpBuffer, int lpBufferLen);
BOOL			    ISO8583_GetFieldEx(t_ISO8583_MSG_STRUCT* iso_struct, int nField, uint8_t* lpData, int* nLength);
ISO8583_ERRCODES	ISO8583_ParseEx(t_ISO8583_MSG_STRUCT* iso_struct, uint8_t* lpBuffer, ushort_t uBufferSize);
ISO8583_ERRCODES	ISO8583_Field_UnpackEx(t_ISO8583_MSG_STRUCT* iso_struct, ushort_t nField, uint8_t** pReference);
ISO8583_ERRCODES	ISO8583_Bitmap_UnpackEx(uint8_t* cBitmap, ushort_t nField, uint8_t** pReference);

//+++++++++++++++++++++++++++++++++++++++++ </Thread Safe Functions> +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
