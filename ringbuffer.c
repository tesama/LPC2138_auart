//---------------------------------------------------------------------
//
//	Program written by Sathit Wanitchaikit for
//		FIBO 2008/March 10
//	Copyright Revenger, 2008. All rights reserved
//
//---------------------------------------------------------------------


//------------------------------------------
//
// STANDARD INCLUDES
//
//------------------------------------------

#include <string.h>

//------------------------------------------
//
// NON-STANDARD INCLUDES
//
//------------------------------------------

#include "ringbuffer.h"

//------------------------------------------
//
// DEFINES
//
//------------------------------------------


//------------------------------------------
//
// GLOBAL VARIABLES
//
//------------------------------------------


//------------------------------------------
//
// LOCAL FUNCTIONS
//
//------------------------------------------


// get the number of free byte before overflowing
static uint8_t get_free_bytes(RingBuffer_t *pBuffer)
{
	return RingBuffer_BUFF_SIZE - pBuffer->counts;
}


// get the number of bytes before warping
static uint8_t get_bytes_before_warping(RingBuffer_t *pBuffer)
{
	return RingBuffer_BUFF_SIZE - (uint8_t)(pBuffer->pEnd - pBuffer->buff);
}


//------------------------------------------
//
// FUNCTIONS
//
//------------------------------------------


// clear buffer
void RingBuffer_clearBuffer(RingBuffer_t *pBuffer)
{
	pBuffer->pBegin = pBuffer->pEnd = pBuffer->buff;
	pBuffer->counts = 0;
}


// push a single data to the buffer
//
//	RETURN	RingBuffer_SUCCESS	if sucess to push the data
//					RingBuffer_FAIL,	otherwise
int32_t RingBuffer_push(RingBuffer_t *pBuffer, uint8_t data)
{
	return RingBuffer_pushArray(pBuffer, &data, 1);
}


// push an array of data to buffer
//
//	RETURN	RingBuffer_SUCCESS	if sucess to push the data
//					RingBuffer_FAIL,	otherwise
int32_t RingBuffer_pushArray(RingBuffer_t *pBuffer, uint8_t *pData, uint8_t nDatas)
{
	uint8_t freeBytes, bytesBeforeWarping;
	
	freeBytes = get_free_bytes( pBuffer );
	bytesBeforeWarping = get_bytes_before_warping(pBuffer);
	
	if (nDatas > freeBytes)
		// The buffer is not enough, then nothing push to the buffer
		return RingBuffer_FAIL;
		
	if (nDatas >= bytesBeforeWarping)
		// cut some which fit the ring before warping
		{
			memcpy(pBuffer->pEnd, pData, bytesBeforeWarping);
			pBuffer->pEnd = pBuffer->buff;
			pBuffer->counts += bytesBeforeWarping;
			nDatas -= bytesBeforeWarping;
			
			pData += bytesBeforeWarping;
		}
		
	memcpy(pBuffer->pEnd, pData, nDatas);
	pBuffer->pEnd += nDatas;
	pBuffer->counts += nDatas;
	
	return RingBuffer_SUCCESS;
}


// pop a single data from the buffer
//
//	RETURN	RingBuffer_SUCCESS	if sucess to pop the data
//					RingBuffer_FAIL,	otherwise
int32_t RingBuffer_pop(RingBuffer_t *pBuffer, uint8_t *data_ret)
{
	if (pBuffer->counts == 0)
		return RingBuffer_FAIL;
	
	// pop the data from the buffer
	*data_ret = *pBuffer->pBegin++;
	if (pBuffer->pBegin >= pBuffer->buff + RingBuffer_BUFF_SIZE)
		// do warping
		pBuffer->pBegin = pBuffer->buff;
	
	pBuffer->counts--;
	
	return RingBuffer_SUCCESS;
}


//---------------------------------------------------------------------
//
//	EOF
//
//---------------------------------------------------------------------
