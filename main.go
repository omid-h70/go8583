package main

/*
#cgo CFLAGS: -std=c99 -I /go8583/Iso8583
#include <stdio.h>
#include "Iso8583/Types.h"
#include "Iso8583/HexPair.h"
#include "Iso8583/HexPair.cpp"
#include "Iso8583/ISO8583_Legacy.h"
#include "Iso8583/ISO8583_Legacy.cpp"
*/
import "C"
import (
	"encoding/hex"
	"fmt"
	"unsafe"
)

func main() {
	var test C.t_ISO8583_MSG_STRUCT
	transAmount := "1000"
	msgType := "0200"
	processingCode := "0999990"
	nii := "007"

	//just Some Notes
	//C.CBytes() Function Makes a Copy From your Buffer
	//unsafe.Pointer Passes the pointer Directly

	C.ISO8583_ResetEx(&test)
	C.ISO8583_SetFieldEx(&test, C.F00_MSG_TYPE_ID, (*C.uint8_t)(C.CBytes([]byte(msgType))), C.int(4))
	C.ISO8583_SetFieldEx(&test, C.F04_TRXN_AMOUNT, (*C.uint8_t)(C.CBytes([]byte(transAmount))), C.int(4))
	C.ISO8583_SetFieldEx(&test, C.F03_PROCESSING_CODE, (*C.uint8_t)(C.CBytes([]byte(processingCode))), C.int(6))
	C.ISO8583_SetFieldEx(&test, C.F24_NII, (*C.uint8_t)(C.CBytes([]byte(nii))), C.int(3))

	var buffer [C.ISO8583_MSGLEN_MAX]byte = [C.ISO8583_MSGLEN_MAX]byte{0xA1, 0xB1}
	msgLen := C.ISO8583_BuildEx(&test, (*C.uint8_t)(unsafe.Pointer(&buffer[0])), C.int(len(buffer)))

	var hexBuff [C.ISO8583_MSGLEN_MAX]byte
	hex.Encode(hexBuff[:], buffer[:msgLen])
	fmt.Println("HexBuff =>", fmt.Sprintf("Len %d %s", msgLen, hexBuff))
	fmt.Println("Done")
}
