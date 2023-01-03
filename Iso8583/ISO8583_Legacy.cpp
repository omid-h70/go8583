// ISO8583_Legacy.cpp

#include "string.h"
#include "stdio.h"
#include "ctype.h"
#include "stdlib.h"
#include "Types.h"
#include "ISO8583_Legacy.h"
#include "HexPair.h"

// Constants

#define ISO8583_FIXED		0
#define ISO8583_LLVAR		2
#define ISO8583_LLLVAR		3
#define ISO8583_LLLLVAR		4
#define ISO8583_CONTVAR		0

#define ISO8583_N			0 // Ascii
#define ISO8583_NS			1
#define ISO8583_XN			2
#define ISO8583_A			3
#define ISO8583_AN			4
#define ISO8583_ANS			5
#define ISO8583_ANSB		6
#define ISO8583_ANP			7
#define ISO8583_B			8
#define ISO8583_Z			9
#define ISO8583_BMP			10

// Macro Functions

#define BYTES_TO_UINT32(b)	((((uint32_t)((b)[0]) & 0xFF) << 24) + (((uint32_t)((b)[1]) & 0xFF) << 16) + (((uint32_t)((b)[2]) & 0xFF) <<  8) + ((uint32_t)((b)[3]) & 0xFF))
#define BIT_TEST(v, b)		((((uint32_t)(v)) & ((uint32_t)1 << (b))) ? 1 : 0)

//+++++++++++++++++++++++++++++++++++++++++ <Thread Safe Functions> +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

static int init_iso8583_map(t_ISO8583_MSG_STRUCT* iso_struct, int nFieldID, int nFieldType, void* pLink, int nLength, int nLengthType, int nLimit)
{
	int i = iso_struct->ISO8583_FIELD_TBL_EX_INDEX;

	iso_struct->ISO8583_FIELD_TBL_EX[i].nFieldID    = nFieldID;
	iso_struct->ISO8583_FIELD_TBL_EX[i].nFieldType  = nFieldType;
	iso_struct->ISO8583_FIELD_TBL_EX[i].pLink		= pLink;
	iso_struct->ISO8583_FIELD_TBL_EX[i].nLength	    = nLength;
	iso_struct->ISO8583_FIELD_TBL_EX[i].nLengthType = nLengthType;
	iso_struct->ISO8583_FIELD_TBL_EX[i].nLimit	    = nLimit;
	iso_struct->ISO8583_FIELD_TBL_EX_INDEX = ++i;

	return i;
}

static void map_iso_fields(t_ISO8583_MSG_STRUCT* iso_struct)
{
	init_iso8583_map(iso_struct, 0,  ISO8583_N,	   (void*)iso_struct->msgFields.MsgTypeId,						      F00_MSG_TYPE_ID_LEN,							 ISO8583_FIXED,		sizeof(iso_struct->msgFields.MsgTypeId));
	init_iso8583_map(iso_struct, 1,  ISO8583_BMP,  (void*)iso_struct->msgFields.Bitmap,						          F01_BITMAP_LEN,								 ISO8583_CONTVAR,	sizeof(iso_struct->msgFields.Bitmap));
	init_iso8583_map(iso_struct, 2,  ISO8583_N,	   (void*)iso_struct->msgFields.F02_Pan,						      F02_PAN_LEN,									 ISO8583_LLVAR,		sizeof(iso_struct->msgFields.F02_Pan));
	init_iso8583_map(iso_struct, 3,  ISO8583_N,	   (void*)iso_struct->msgFields.F03_ProcessingCode,                   F03_PROCESSING_CODE_LEN,						 ISO8583_FIXED,		sizeof(iso_struct->msgFields.F03_ProcessingCode));
	init_iso8583_map(iso_struct, 4,  ISO8583_N,	   (void*)iso_struct->msgFields.F04_TrxnAmount,				          F04_TRXN_AMOUNT_LEN,							 ISO8583_FIXED,		sizeof(iso_struct->msgFields.F04_TrxnAmount));
	init_iso8583_map(iso_struct, 5,  ISO8583_N,	   (void*)iso_struct->msgFields.F05_SettlementAmount,                 F05_RECONCILIATION_AMOUNT_LEN,			     ISO8583_FIXED,		sizeof(iso_struct->msgFields.F05_SettlementAmount));
	init_iso8583_map(iso_struct, 6,  ISO8583_N,	   (void*)iso_struct->msgFields.F06_CardholderBillingAmount,          F06_CARDHOLDER_BILLING_AMOUNT_LEN,			 ISO8583_FIXED,		sizeof(iso_struct->msgFields.F06_CardholderBillingAmount));
	init_iso8583_map(iso_struct, 7,  ISO8583_N,	   (void*)iso_struct->msgFields.F07_TxDateAndTime,	                  F07_TX_DATE_AND_TIME_LEN,					     ISO8583_FIXED,		sizeof(iso_struct->msgFields.F07_TxDateAndTime));
	init_iso8583_map(iso_struct, 8,  ISO8583_N,	   (void*)iso_struct->msgFields.F08_CardholderBillingFeeAmount,       F08_CARDHOLDER_BILLING_FEE_AMOUNT_LEN,		 ISO8583_FIXED,		sizeof(iso_struct->msgFields.F08_CardholderBillingFeeAmount));
	init_iso8583_map(iso_struct, 9,  ISO8583_N,	   (void*)iso_struct->msgFields.F09_ConversionRateSettlement,         F09_CONVERSION_RATE_RECONCILIATION_LEN,		 ISO8583_FIXED,		sizeof(iso_struct->msgFields.F09_ConversionRateSettlement));
	init_iso8583_map(iso_struct, 10, ISO8583_N,    (void*)iso_struct->msgFields.F10_ConversionRateCardholderBilling,  F10_CONVERSION_RATE_CARDHOLDER_BILLING_LEN,	 ISO8583_FIXED,		sizeof(iso_struct->msgFields.F10_ConversionRateCardholderBilling));
	init_iso8583_map(iso_struct, 11, ISO8583_N,	   (void*)iso_struct->msgFields.F11_Stan,						      F11_STAN_LEN,									 ISO8583_FIXED,		sizeof(iso_struct->msgFields.F11_Stan));
	init_iso8583_map(iso_struct, 12, ISO8583_N,	   (void*)iso_struct->msgFields.F12_TrxnTime,						  F12_TRXN_TIME_LEN,						     ISO8583_FIXED,		sizeof(iso_struct->msgFields.F12_TrxnTime));
	init_iso8583_map(iso_struct, 13, ISO8583_N,	   (void*)iso_struct->msgFields.F13_TrxnDate,						  F13_TRXN_DATE_LEN,							 ISO8583_FIXED,		sizeof(iso_struct->msgFields.F13_TrxnDate));
	init_iso8583_map(iso_struct, 14, ISO8583_N,	   (void*)iso_struct->msgFields.F14_ExpiryDate,					      F14_EXPIRY_DATE_LEN,							 ISO8583_FIXED,		sizeof(iso_struct->msgFields.F14_ExpiryDate));
	init_iso8583_map(iso_struct, 15, ISO8583_N,	   (void*)iso_struct->msgFields.F15_SettlementDate,				      F15_SETTLEMENT_DATE_LEN,						 ISO8583_FIXED,		sizeof(iso_struct->msgFields.F15_SettlementDate));
	init_iso8583_map(iso_struct, 16, ISO8583_N,	   (void*)iso_struct->msgFields.F16_ConversionDate,				      F16_CONVERSION_DATE_LEN,						 ISO8583_FIXED,		sizeof(iso_struct->msgFields.F16_ConversionDate));
	init_iso8583_map(iso_struct, 17, ISO8583_N,	   (void*)iso_struct->msgFields.F17_CaptureDate,					  F17_CAPTURE_DATE_LEN,					         ISO8583_FIXED,		sizeof(iso_struct->msgFields.F17_CaptureDate));
	init_iso8583_map(iso_struct, 18, ISO8583_N,	   (void*)iso_struct->msgFields.F18_MerchantType,					  F18_MERCHANT_TYPE_LEN,					     ISO8583_FIXED,		sizeof(iso_struct->msgFields.F18_MerchantType));
	init_iso8583_map(iso_struct, 19, ISO8583_N,	   (void*)iso_struct->msgFields.F19_AcquiringInstitutionCountryCode,  F19_ACQUIRING_INSTITUTION_COUNTRY_CODE_LEN,    ISO8583_FIXED,		sizeof(iso_struct->msgFields.F19_AcquiringInstitutionCountryCode));
	init_iso8583_map(iso_struct, 20, ISO8583_N,	   (void*)iso_struct->msgFields.F20_PrimaryAccountNumberCountryCode,  F20_PRIMARY_ACCOUNT_NUMBER_COUNTRY_CODE_LEN,   ISO8583_FIXED,		sizeof(iso_struct->msgFields.F20_PrimaryAccountNumberCountryCode));
	init_iso8583_map(iso_struct, 21, ISO8583_N,	   (void*)iso_struct->msgFields.F21_ForwardingInstitutionCountryCode, F21_FORWARDING_INSTITUTION_COUNTRY_CODE_LEN,   ISO8583_FIXED,		sizeof(iso_struct->msgFields.F21_ForwardingInstitutionCountryCode));
	init_iso8583_map(iso_struct, 22, ISO8583_N,	   (void*)iso_struct->msgFields.F22_POSEntryMode,					  F22_POS_ENTRY_MODE_LEN,					     ISO8583_FIXED,		sizeof(iso_struct->msgFields.F22_POSEntryMode));
	init_iso8583_map(iso_struct, 23, ISO8583_N,	   (void*)iso_struct->msgFields.F23_AppPanNumber,					  F23_APP_PAN_NUMBER_LEN,						 ISO8583_FIXED,		sizeof(iso_struct->msgFields.F23_AppPanNumber));
	init_iso8583_map(iso_struct, 24, ISO8583_N,	   (void*)iso_struct->msgFields.F24_NetworkInternationalID,		      F24_NII_LEN,									 ISO8583_FIXED,		sizeof(iso_struct->msgFields.F24_NetworkInternationalID));
	init_iso8583_map(iso_struct, 25, ISO8583_N,	   (void*)iso_struct->msgFields.F25_POSConditionCode,				  F25_POS_CONDITION_CODE_LEN,				     ISO8583_FIXED,		sizeof(iso_struct->msgFields.F25_POSConditionCode));
	init_iso8583_map(iso_struct, 26, ISO8583_N,	   (void*)iso_struct->msgFields.F26_PINCaptureCode,					  F26_PIN_CAPURE_CODE_LEN,						 ISO8583_FIXED,		sizeof(iso_struct->msgFields.F26_PINCaptureCode));
	init_iso8583_map(iso_struct, 27, ISO8583_N,	   (void*)iso_struct->msgFields.F27_AuthIdRespLength,				  F27_APPROVAL_CODE_LENGTH_LEN,					 ISO8583_FIXED,		sizeof(iso_struct->msgFields.F27_AuthIdRespLength));
	init_iso8583_map(iso_struct, 28, ISO8583_N,	   (void*)iso_struct->msgFields.F28_TrxnAmountFee,				      F28_TRANSACTION_AMOUNT_FEE_LEN,				 ISO8583_FIXED,		sizeof(iso_struct->msgFields.F28_TrxnAmountFee));
	init_iso8583_map(iso_struct, 29, ISO8583_N,	   (void*)iso_struct->msgFields.F29_SettlementAmountFee,			  F29_SETTLEMENT_AMOUNT_FEE_LEN,			     ISO8583_FIXED,		sizeof(iso_struct->msgFields.F29_SettlementAmountFee));
	init_iso8583_map(iso_struct, 30, ISO8583_N,	   (void*)iso_struct->msgFields.F30_TransactionAmountProcessingFee,   F30_TRANSACTION_AMOUNT_PROCESSING_FEE_LEN,	 ISO8583_FIXED,		sizeof(iso_struct->msgFields.F30_TransactionAmountProcessingFee));
	init_iso8583_map(iso_struct, 31, ISO8583_ANS,  (void*)iso_struct->msgFields.F31_SettlementAmountProcessingFee,    F31_SETTLEMENT_AMOUNT_PROCESSING_FEE_LEN,		 ISO8583_LLVAR,		sizeof(iso_struct->msgFields.F31_SettlementAmountProcessingFee));
	init_iso8583_map(iso_struct, 32, ISO8583_N,	   (void*)iso_struct->msgFields.F32_AcquiringInstitutionIdCode,       F32_ACQUIRING_INSTITUTION_ID_CODE_LEN,		 ISO8583_LLVAR,		sizeof(iso_struct->msgFields.F32_AcquiringInstitutionIdCode));
	init_iso8583_map(iso_struct, 33, ISO8583_N,	   (void*)iso_struct->msgFields.F33_ForwardingInstitutionIdCode,      F33_FORWARDING_INSTITUTION_ID_CODE_LEN,		 ISO8583_LLVAR,		sizeof(iso_struct->msgFields.F33_ForwardingInstitutionIdCode));
	init_iso8583_map(iso_struct, 34, ISO8583_NS,   (void*)iso_struct->msgFields.F34_ExtendedPrimaryAccountNumber,     F34_EXTENDED_PRIMARY_ACCOUNT_NUMBER_LEN,		 ISO8583_LLVAR,		sizeof(iso_struct->msgFields.F34_ExtendedPrimaryAccountNumber));
	init_iso8583_map(iso_struct, 35, ISO8583_Z,    (void*)iso_struct->msgFields.F35_Track2Data,					      F35_TRACK_2_DATA_LEN,					         ISO8583_LLVAR,		sizeof(iso_struct->msgFields.F35_Track2Data));
	init_iso8583_map(iso_struct, 36, ISO8583_Z,    (void*)iso_struct->msgFields.F36_Track3Data,						  F36_TRACK_3_DATA_LEN,					         ISO8583_LLLVAR,    sizeof(iso_struct->msgFields.F36_Track3Data));
	init_iso8583_map(iso_struct, 37, ISO8583_ANP,  (void*)iso_struct->msgFields.F37_Rrn,							  F37_RRN_LEN,								     ISO8583_FIXED,		sizeof(iso_struct->msgFields.F37_Rrn));
	init_iso8583_map(iso_struct, 38, ISO8583_ANP,  (void*)iso_struct->msgFields.F38_AuthIdResp,					      F38_AUTH_ID_RESPONSE_LEN,						 ISO8583_FIXED,		sizeof(iso_struct->msgFields.F38_AuthIdResp));
	init_iso8583_map(iso_struct, 39, ISO8583_AN,   (void*)iso_struct->msgFields.F39_ResponseCode,					  F39_RESPONSE_CODE_LEN,					     ISO8583_FIXED,		sizeof(iso_struct->msgFields.F39_ResponseCode));
	init_iso8583_map(iso_struct, 40, ISO8583_N,    (void*)iso_struct->msgFields.F40_ServiceCode,					  F40_SERVICE_CODE_LEN,						     ISO8583_FIXED,		sizeof(iso_struct->msgFields.F40_ServiceCode));
	init_iso8583_map(iso_struct, 41, ISO8583_ANS,  (void*)iso_struct->msgFields.F41_CardAcceptorTerminalId,			  F41_CARD_ACCEPTOR_TERMINAL_ID_LEN,			 ISO8583_FIXED,		sizeof(iso_struct->msgFields.F41_CardAcceptorTerminalId));
	init_iso8583_map(iso_struct, 42, ISO8583_ANS,  (void*)iso_struct->msgFields.F42_CardAcceptorId,				      F42_CARD_ACCEPTOR_ID_LEN,						 ISO8583_FIXED,		sizeof(iso_struct->msgFields.F42_CardAcceptorId));
	init_iso8583_map(iso_struct, 43, ISO8583_ANS,  (void*)iso_struct->msgFields.F43_CardAcceptorNameLocation,		  F43_CARD_ACCEPTOR_NAME_LOCATION_LEN,			 ISO8583_LLVAR,		sizeof(iso_struct->msgFields.F43_CardAcceptorNameLocation));
	init_iso8583_map(iso_struct, 44, ISO8583_ANS,  (void*)iso_struct->msgFields.F44_AdditionalResponseData,			  F44_ADDITIONAL_RESPONSE_DATA_LEN,				 ISO8583_LLVAR,		sizeof(iso_struct->msgFields.F44_AdditionalResponseData));
	init_iso8583_map(iso_struct, 45, ISO8583_ANS,  (void*)iso_struct->msgFields.F45_Track1Data,						  F45_TRACK_1_DATA_LEN,							 ISO8583_LLVAR,		sizeof(iso_struct->msgFields.F45_Track1Data));
	init_iso8583_map(iso_struct, 46, ISO8583_ANS,  (void*)iso_struct->msgFields.F46_AdditionalDataIso,				  F46_ADDITIONAL_DATA_ISO_LEN,				     ISO8583_LLLVAR,	sizeof(iso_struct->msgFields.F46_AdditionalDataIso));
	init_iso8583_map(iso_struct, 47, ISO8583_ANS,  (void*)iso_struct->msgFields.F47_AddDataNational,				  F47_ADD_DATA_NATIONAL_LEN,					 ISO8583_LLLVAR,	sizeof(iso_struct->msgFields.F47_AddDataNational));
	init_iso8583_map(iso_struct, 48, ISO8583_ANS,  (void*)iso_struct->msgFields.F48_AddDataPrivate,					  F48_ADD_DATA_PRIVATE_LEN,						 ISO8583_LLLVAR,	sizeof(iso_struct->msgFields.F48_AddDataPrivate));
	init_iso8583_map(iso_struct, 49, ISO8583_AN,   (void*)iso_struct->msgFields.F49_TransactionCurrencyCode,		  F49_TRANSACTION_CURRENCY_CODE_LEN,			 ISO8583_FIXED,		sizeof(iso_struct->msgFields.F49_TransactionCurrencyCode));
	init_iso8583_map(iso_struct, 50, ISO8583_AN,   (void*)iso_struct->msgFields.F50_SettlementCurrencyCode,			  F50_SETTLEMENT_CURRENCY_CODE_LEN,				 ISO8583_FIXED,		sizeof(iso_struct->msgFields.F50_SettlementCurrencyCode));
	init_iso8583_map(iso_struct, 51, ISO8583_AN,   (void*)iso_struct->msgFields.F51_CardholderBillingCurrencyCode,	  F51_CARDHOLDER_BILLING_CURRENCY_CODE_LEN,		 ISO8583_FIXED,		sizeof(iso_struct->msgFields.F51_CardholderBillingCurrencyCode));
	init_iso8583_map(iso_struct, 52, ISO8583_B,    (void*)iso_struct->msgFields.F52_PinData,						  F52_PIN_DATA_LEN,								 ISO8583_FIXED,		sizeof(iso_struct->msgFields.F52_PinData));
	init_iso8583_map(iso_struct, 53, ISO8583_B,    (void*)iso_struct->msgFields.F53_SecurityControlInfo,			  F53_SECURITY_CONTROL_INFO_LEN,				 ISO8583_LLVAR,		sizeof(iso_struct->msgFields.F53_SecurityControlInfo));
	init_iso8583_map(iso_struct, 54, ISO8583_ANS,  (void*)iso_struct->msgFields.F54_AdditionalAmount,				  F54_ADDITIONAL_AMOUNT_LEN,					 ISO8583_LLLVAR,	sizeof(iso_struct->msgFields.F54_AdditionalAmount));
	init_iso8583_map(iso_struct, 55, ISO8583_B,    (void*)iso_struct->msgFields.F55_ReservedISO,					  F55_RESERVED_ISO_LEN,							 ISO8583_LLLVAR,	sizeof(iso_struct->msgFields.F55_ReservedISO));
	init_iso8583_map(iso_struct, 56, ISO8583_N,    (void*)iso_struct->msgFields.F56_ReservedISO,					  F56_RESERVED_ISO_LEN,							 ISO8583_LLVAR,		sizeof(iso_struct->msgFields.F56_ReservedISO));
	init_iso8583_map(iso_struct, 57, ISO8583_N,    (void*)iso_struct->msgFields.F57_ReservedNational,				  F57_RESERVED_NATIONAL_LEN,					 ISO8583_FIXED,		sizeof(iso_struct->msgFields.F57_ReservedNational));
	init_iso8583_map(iso_struct, 58, ISO8583_ANS,  (void*)iso_struct->msgFields.F58_ReservedNational,				  F58_RESERVED_NATIONAL_LEN,					 ISO8583_LLLVAR,	sizeof(iso_struct->msgFields.F58_ReservedNational));
	init_iso8583_map(iso_struct, 59, ISO8583_ANS,  (void*)iso_struct->msgFields.F59_Transportdata,					  F59_TRANSPORT_DATA_LEN,					     ISO8583_LLLVAR,    sizeof(iso_struct->msgFields.F59_Transportdata));
	init_iso8583_map(iso_struct, 60, ISO8583_ANS,  (void*)iso_struct->msgFields.F60_PrivateUse,						  F60_PRIVATE_USE_LEN,							 ISO8583_LLLVAR,    sizeof(iso_struct->msgFields.F60_PrivateUse));
	init_iso8583_map(iso_struct, 61, ISO8583_ANS,  (void*)iso_struct->msgFields.F61_PrivateUse,						  F61_PRIVATE_USE_LEN,							 ISO8583_LLLVAR,    sizeof(iso_struct->msgFields.F61_PrivateUse));
	init_iso8583_map(iso_struct, 62, ISO8583_ANS,  (void*)iso_struct->msgFields.F62_PrivateUse,						  F62_PRIVATE_USE_LEN,						     ISO8583_LLLVAR,    sizeof(iso_struct->msgFields.F62_PrivateUse));
	init_iso8583_map(iso_struct, 63, ISO8583_ANS,  (void*)iso_struct->msgFields.F63_PrivateUse,						  F63_PRIVATE_USE_LEN,						     ISO8583_LLLVAR,    sizeof(iso_struct->msgFields.F63_PrivateUse));
	init_iso8583_map(iso_struct, 64, ISO8583_B,    (void*)iso_struct->msgFields.F64_MessageAuthCode,		          F64_MESSAGE_AUTH_CODE_LEN,				     ISO8583_FIXED,     sizeof(iso_struct->msgFields.F64_MessageAuthCode));
}

static BOOL ValidNumber(uint8_t* s)
{
	while (*s)
	{
		if (!isdigit(*s))
		{
			return FALSE;
		}

		s++;
	}

	return TRUE;
}

void ISO8583_SetBitmap(uint8_t* lpBitmap, int nField)
{
	if (nField != F00_MSG_TYPE_ID)
	{
		lpBitmap[(nField % 8) ? (nField / 8) : (nField / 8) - 1]  |= (0x80 >> (nField % 8 ? (nField % 8) - 1 : 8 - 1));
	}
}

void ISO8583_ResetEx(t_ISO8583_MSG_STRUCT* iso_struct)
{
	memset(iso_struct, 0, sizeof(t_ISO8583_MSG_STRUCT));
	map_iso_fields(iso_struct);
}

void ISO8583_SetFieldEx(t_ISO8583_MSG_STRUCT* iso_struct, int nField, uint8_t* lpData, int nLength)
{
	if (nLength == 0)
	{
		iso_struct->bUseField[nField] = FALSE;
		return;
	}

	if (nLength > iso_struct->ISO8583_FIELD_TBL_EX[nField].nLength)
	{
		nLength = iso_struct->ISO8583_FIELD_TBL_EX[nField].nLength;
	}

	iso_struct->bUseField[nField] = TRUE;

	memcpy(iso_struct->ISO8583_FIELD_TBL_EX[nField].pLink, lpData, nLength);
	//_LOG("lpData Len %d Got %s \r\n", nLength, iso_struct->ISO8583_FIELD_TBL_EX[nField].pLink);

	ISO8583_SetBitmap(iso_struct->msgFields.Bitmap, nField);
}

int ISO8583_BuildEx(t_ISO8583_MSG_STRUCT* iso_struct, uint8_t* lpBuffer, int lpBufferLen)
{
	ushort_t	nPos = 0;
	ushort_t	nFieldLength = 0;
	int		    nDistance = 0;
	uint8_t*	cPrefix = t_NULL;
	uint8_t*	cWorker = t_NULL;

	cPrefix = (uint8_t*)malloc(5 * sizeof(uint8_t));
	cWorker = (uint8_t*)malloc(ISO8583_MSGLEN_MAX * sizeof(uint8_t));

	memset(lpBuffer, 0, lpBufferLen * sizeof(uint8_t));

	if (cPrefix != t_NULL && cWorker != t_NULL)
	{
		for (int i = 0; i < FIELD_MAX; i++)
		{
			memset(cWorker, 0, ISO8583_MSGLEN_MAX * sizeof(uint8_t));
			memset(cPrefix, 0, 5);

			if (iso_struct->ISO8583_FIELD_TBL_EX[i].nFieldType == ISO8583_BMP)
			{
				memcpy(&lpBuffer[nPos], iso_struct->ISO8583_FIELD_TBL_EX[i].pLink, iso_struct->ISO8583_FIELD_TBL_EX[i].nLength * sizeof(uint8_t));
				nPos += iso_struct->ISO8583_FIELD_TBL_EX[i].nLength;
			}
			else if ((iso_struct->ISO8583_FIELD_TBL_EX[i].nLengthType == ISO8583_FIXED) && (iso_struct->ISO8583_FIELD_TBL_EX[i].nFieldType == ISO8583_N))
			{
				if (iso_struct->bUseField[i])
				{
					if ((nFieldLength = strlen((const char*)iso_struct->ISO8583_FIELD_TBL_EX[i].pLink)) > 0)
					{
						nDistance = iso_struct->ISO8583_FIELD_TBL_EX[i].nLength - nFieldLength;

						if (nDistance > 0)
						{
							memset(&cWorker[0], 0x30, nDistance * sizeof(uint8_t));
						}

						memcpy(&cWorker[nDistance * sizeof(uint8_t)], iso_struct->ISO8583_FIELD_TBL_EX[i].pLink, nFieldLength * sizeof(uint8_t));

						if (iso_struct->ISO8583_FIELD_TBL_EX[i].nLength & 0x01)
						{
							cWorker[nDistance + nFieldLength] = 0x46;
							nDistance = ((iso_struct->ISO8583_FIELD_TBL_EX[i].nLength + 1) / 2);
						}
						else
						{
							nDistance = (iso_struct->ISO8583_FIELD_TBL_EX[i].nLength / 2);
						}

						StringToHexPair1(cWorker, strlen((const char*)cWorker));
						memcpy(&lpBuffer[nPos], &cWorker[0], nDistance);

						nPos += nDistance;
					}
				}
			}
			else if ((iso_struct->ISO8583_FIELD_TBL_EX[i].nLengthType == ISO8583_FIXED) && (iso_struct->ISO8583_FIELD_TBL_EX[i].nFieldType == ISO8583_B))
			{
				if (iso_struct->bUseField[i])
				{
					memcpy(&lpBuffer[nPos], iso_struct->ISO8583_FIELD_TBL_EX[i].pLink, iso_struct->ISO8583_FIELD_TBL_EX[i].nLength * sizeof(uint8_t));

					nPos += iso_struct->ISO8583_FIELD_TBL_EX[i].nLength;
				}
			}
			else if ((iso_struct->ISO8583_FIELD_TBL_EX[i].nLengthType == ISO8583_FIXED) && (iso_struct->ISO8583_FIELD_TBL_EX[i].nFieldType != ISO8583_N) && (iso_struct->ISO8583_FIELD_TBL_EX[i].nFieldType != ISO8583_B))
			{
				if (iso_struct->bUseField[i])
				{
					if ((nFieldLength = strlen((const char*)iso_struct->ISO8583_FIELD_TBL_EX[i].pLink)) > 0)
					{
						nDistance = iso_struct->ISO8583_FIELD_TBL_EX[i].nLength - nFieldLength;

						memcpy(&lpBuffer[nPos], iso_struct->ISO8583_FIELD_TBL_EX[i].pLink, nFieldLength * sizeof(uint8_t));

						if (nDistance > 0)
						{
							memset(lpBuffer + nPos + nFieldLength, 0x20, nDistance * sizeof(uint8_t));
						}

						nPos += iso_struct->ISO8583_FIELD_TBL_EX[i].nLength;
					}
				}
			}
			else if ((iso_struct->ISO8583_FIELD_TBL_EX[i].nLengthType == ISO8583_LLVAR) && (iso_struct->ISO8583_FIELD_TBL_EX[i].nFieldType == ISO8583_Z))
			{
				if (iso_struct->bUseField[i])
				{
					if ((nFieldLength = strlen((const char*)iso_struct->ISO8583_FIELD_TBL_EX[i].pLink)) > 0)
					{
						if (nFieldLength & 0x01)
						{
							snprintf((char*)cPrefix, 5, "%02d", nFieldLength + 1);
							memcpy(&cWorker[0], cPrefix, 2);
							memcpy(&cWorker[2], iso_struct->ISO8583_FIELD_TBL_EX[i].pLink, nFieldLength);

							cWorker[nFieldLength + 2] = 0x46;
							nDistance = ((nFieldLength + 1) / 2) + 1;
						}
						else
						{
							snprintf((char*)cPrefix, 5, "%02d", nFieldLength);
							sprintf((char*)cPrefix, "%02d", (int)nFieldLength);

							memcpy(&cWorker[0], cPrefix, 2);
							memcpy(&cWorker[2], iso_struct->ISO8583_FIELD_TBL_EX[i].pLink, nFieldLength);

							nDistance = (nFieldLength / 2) + 1;
						}

						StringToHexPair1(cWorker, strlen((const char*)cWorker));
						memcpy(&lpBuffer[nPos], &cWorker[0], nDistance);

						nPos += nDistance;
					}
				}
			}
			else if ((iso_struct->ISO8583_FIELD_TBL_EX[i].nLengthType == ISO8583_LLVAR) && (iso_struct->ISO8583_FIELD_TBL_EX[i].nFieldType != ISO8583_Z))
			{
				if (iso_struct->bUseField[i])
				{
					if ((nFieldLength = strlen((const char*)iso_struct->ISO8583_FIELD_TBL_EX[i].pLink)) > 0)
					{
						snprintf((char*)cPrefix, 5, "%02d", nFieldLength);

						StringToHexPair1(cPrefix, strlen((const char*)cPrefix));
						memcpy(&lpBuffer[nPos], cPrefix, 1);
						memcpy(&lpBuffer[nPos + 1], iso_struct->ISO8583_FIELD_TBL_EX[i].pLink, nFieldLength);

						nPos += (nFieldLength + 1);
					}
				}
			}
			else if (iso_struct->ISO8583_FIELD_TBL_EX[i].nLengthType == ISO8583_LLLVAR)
			{
				if (iso_struct->bUseField[i])
				{
					if ((nFieldLength = strlen((const char*)iso_struct->ISO8583_FIELD_TBL_EX[i].pLink)) > 0)
					{
						snprintf((char*)cPrefix, 5, "%04d", nFieldLength);
						StringToHexPair1(cPrefix, strlen((const char*)cPrefix));
						memcpy(&lpBuffer[nPos], cPrefix, 2);
						memcpy(&lpBuffer[nPos + 2], iso_struct->ISO8583_FIELD_TBL_EX[i].pLink, nFieldLength);

						nPos += (nFieldLength + 2);
					}
				}
			}
		}
	}

	if (cPrefix != t_NULL) free(cPrefix);
	if (cWorker != t_NULL) free(cWorker);

	return nPos;
}

BOOL ISO8583_GetFieldEx(t_ISO8583_MSG_STRUCT* iso_struct, int nField, uint8_t* lpData, int* nLength)
{
	ushort_t	nFieldLength = 0;

	if ((nFieldLength = strlen((const char*)iso_struct->ISO8583_FIELD_TBL_EX[nField].pLink)) == 0)
	{
		return FALSE;
	}

	if (nFieldLength > (*nLength))
	{
		return FALSE;
	}

	memset(lpData, 0, (*nLength));
	memcpy(lpData, iso_struct->ISO8583_FIELD_TBL_EX[nField].pLink, nFieldLength);

	*nLength = nFieldLength;

	return TRUE;
}

ISO8583_ERRCODES ISO8583_ParseEx(t_ISO8583_MSG_STRUCT* iso_struct, uint8_t* lpBuffer, ushort_t uBufferSize)
{
	ISO8583_ERRCODES	errCode = ISO8583_SUCCESS;
	uint8_t*			lpCur = (uint8_t*)lpBuffer;
	uint8_t*			lpEnd = lpCur + uBufferSize;
	ushort_t			nCurIndex = 0;
	ushort_t			nMaxIndex = FIELD_MAX;
	BOOL				bBitmap = FALSE;

	while ((errCode == ISO8583_SUCCESS) && (nCurIndex < nMaxIndex) && (lpCur < lpEnd) && (bBitmap == FALSE))
	{
		errCode = ISO8583_Field_UnpackEx(iso_struct, nCurIndex, &lpCur);

		nCurIndex++;

		if (iso_struct->ISO8583_FIELD_TBL_EX[nCurIndex].nFieldType == ISO8583_BMP)
		{
			bBitmap = TRUE;
		}
	}

	ISO8583_Bitmap_UnpackEx(iso_struct->cBitmap, nCurIndex, &lpCur);

	if ((iso_struct->cBitmap[0] & 0x80) == 0x80)
	{
		return ISO8583_INVALID_BITMAP;
	}

	nCurIndex++;

	while ((errCode == ISO8583_SUCCESS) && (nCurIndex < nMaxIndex) && (lpCur < lpEnd))
	{
		if ((iso_struct->cBitmap[(nCurIndex % 8) ? (nCurIndex / 8) : (nCurIndex / 8) - 1] & (0x80 >> (nCurIndex % 8 ? (nCurIndex % 8) - 1 : 8 - 1))) == (0x80 >> (nCurIndex % 8 ? (nCurIndex % 8) - 1 : 8 - 1)))
		{
			errCode = ISO8583_Field_UnpackEx(iso_struct, nCurIndex, &lpCur);
		}

		if(errCode != ISO8583_SUCCESS)
			_LOG("ISO8583_Field_UnpackEx Failed On %d - %d ", nCurIndex, *lpEnd);

		nCurIndex++;
	}

	//_HEX_LOG(iso_struct->cBitmap, 8);
	memcpy(iso_struct->msgFields.Bitmap, iso_struct->cBitmap, 8);

	if ((errCode == ISO8583_SUCCESS) && (lpCur != lpEnd))
	{
		errCode = ISO8583_UNKNOWN;
	}

	return errCode;
}

int atoiEx(char* str, int end_str_ptr_index)
{
	if(str != t_NULL)
	{
		int len = strlen(str);

		if(len < end_str_ptr_index && len > 0)
		{
			return atoi(str);
		}
	}
	return 0;
}

ISO8583_ERRCODES ISO8583_Field_UnpackEx(t_ISO8583_MSG_STRUCT* iso_struct, ushort_t nField, uint8_t** pReference)
{
	ISO8583_ERRCODES	errCode = ISO8583_SUCCESS;
	int				    nSize   = 0;
	uint8_t*			pPos	= (uint8_t*)*pReference;
	uint				uLength = 0;
	uint8_t*		    cPrefix = t_NULL;
	uint8_t*			cWorker = t_NULL;
	uint8_t*			cBuffer = t_NULL;
	BOOL                bMallocFlag = FALSE;

	cPrefix = (uint8_t*)malloc(5 * sizeof(uint8_t));
	cWorker = (uint8_t*)malloc(ISO8583_MSGLEN_MAX * sizeof(uint8_t));
	cBuffer = (uint8_t*)malloc(ISO8583_MSGLEN_MAX * sizeof(uint8_t));

	if (cPrefix != t_NULL && cWorker != t_NULL && cBuffer != t_NULL)
	{
		bMallocFlag = TRUE;
		memset(cBuffer, 0, ISO8583_MSGLEN_MAX);
		memset(cWorker, 0, ISO8583_MSGLEN_MAX);
		memset(cPrefix, 0, 5);

		switch (iso_struct->ISO8583_FIELD_TBL_EX[nField].nLengthType)
		{
		case ISO8583_FIXED:
		{
			if (iso_struct->ISO8583_FIELD_TBL_EX[nField].nFieldType == ISO8583_N)
			{
				nSize = ((iso_struct->ISO8583_FIELD_TBL_EX[nField].nLength + 1) / 2);
				uLength = ISO8583_MSGLEN_MAX;

				memcpy(cWorker, pPos, nSize);
				HexPairToString(cWorker, nSize, cBuffer, &uLength);

				*pReference += nSize;
				nSize = iso_struct->ISO8583_FIELD_TBL_EX[nField].nLength;

				if (!ValidNumber(cBuffer))
				{
					return ISO8583_NOT_COMPATIBLE;
				}
			}
			else
			{
				nSize = iso_struct->ISO8583_FIELD_TBL_EX[nField].nLength;
				*pReference += nSize;

				memcpy(cBuffer, pPos, nSize);
			}
		}
		break;
		case ISO8583_LLVAR:
		{
			uLength = 5;

			memset(cPrefix, 0, uLength);
			memset(cWorker, 0, ISO8583_MSGLEN_MAX);
			memcpy(cWorker, pPos, 1);
			HexPairToString(cWorker, 1, cPrefix, &uLength);

			if (!ValidNumber(cPrefix))
			{
				return ISO8583_NOT_COMPATIBLE;
			}

			//nSize = atoi(_C(cPrefix));
			nSize = atoiEx((char*)cPrefix, 5);
			if (nSize == 0)
			{
				*pReference += 1;
				break;
			}

			memcpy(cBuffer, &pPos[1], nSize);

			*pReference += (nSize + 1);
		}
		break;
		case ISO8583_LLLVAR:
		{
			uLength = 5;

			memset(cPrefix, 0, uLength);
			memset(cWorker, 0, ISO8583_MSGLEN_MAX);
			memcpy(cWorker, pPos, 2);
			HexPairToString(cWorker, 2, cPrefix, &uLength);

			if (!ValidNumber(cPrefix))
			{
				return ISO8583_NOT_COMPATIBLE;
			}

			//nSize = atoi(_C(cPrefix));
			nSize = atoiEx((char*)cPrefix, 5);
			if (nSize == 0)
			{
				*pReference += 2;
				break;
			}

			memcpy(cBuffer, &pPos[2], nSize);

			*pReference += (nSize + 2);

			_LOG("ISO8583_LLLVAR nField %d cBuffer %s", nField, cBuffer);
		}
		break;
		}

		ISO8583_SetFieldEx(iso_struct, nField, cBuffer, nSize);
	}
	else
	{
		bMallocFlag = FALSE;
	}

	if (cPrefix != t_NULL) free(cPrefix);
	if (cBuffer != t_NULL) free(cBuffer);
	if (cWorker != t_NULL) free(cWorker);

	if (!bMallocFlag)
	{
		return ISO8583_UNKNOWN;
	}

	return errCode;
}

ISO8583_ERRCODES ISO8583_Bitmap_UnpackEx(uint8_t* cBitmap, ushort_t nField, uint8_t** pReference)
{
	ISO8583_ERRCODES	errCode = ISO8583_SUCCESS;
	uint8_t*			pPos = (uint8_t*)*pReference;
	ushort_t			nCurIndex = nField;
	int					i;

	do
	{
		uint32_t	lHigh = BYTES_TO_UINT32(pPos + 0);
		uint32_t	lLow = BYTES_TO_UINT32(pPos + 4);

		pPos += 8;

		nCurIndex++;

		for (i = 30; i >= 0; i--, nCurIndex++)
		{
			if (BIT_TEST(lHigh, i))
			{
				if (nCurIndex > FIELD_MAX)
				{
					return ISO8583_UNKNOWN;
				}

				ISO8583_SetBitmap(cBitmap, nCurIndex);
			}
		}

		for (i = 31; i >= 0; i--, nCurIndex++)
		{
			if (BIT_TEST(lLow, i))
			{
				if (nCurIndex > FIELD_MAX)
				{
					return ISO8583_UNKNOWN;
				}

				ISO8583_SetBitmap(cBitmap, nCurIndex);
			}
		}

		if (0 == BIT_TEST(lHigh, 31))
		{
			break;
		}
	} while (errCode == ISO8583_SUCCESS);

	*pReference = pPos;

	return errCode;
}

//+++++++++++++++++++++++++++++++++++++++++ </Thread Safe Functions> +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
