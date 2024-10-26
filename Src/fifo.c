
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "fifo.h"


//-----------------------------------------------------------------------------
/* FIFO Register API */
bool fifo_register (fifo_t* fifo, uint8_t* buf, uint32_t size) {
  if (fifo == NULL || buf == NULL || size == 0) {
    return false;
  }
  fifo->totalSize   = 0;
  fifo->occupySize  = 0;
	fifo->buf	        = buf;
	fifo->pWrite      = buf;
	fifo->pRead       = buf;
  fifo->mutexRead   = false;
  fifo->mutexWrite  = false;
  fifo->totalSize   = size;
  return true;
}
//-----------------------------------------------------------------------------
/* FIFO Flush */
bool fifo_flush (fifo_t* fifo) {
  bool status = false;
  #ifdef FIFO_DISABLE_IRQ
  FIFO_DISABLE_IRQ();
  #endif 
  if (fifo == NULL || fifo->mutexRead == true || fifo->mutexWrite == true) {
    goto END;
  }
  fifo->occupySize = 0;
	fifo->pWrite     = fifo->buf;
	fifo->pRead      = fifo->buf;
  status = true;
  END:
  #ifdef FIFO_ENABLE_IRQ
  FIFO_ENABLE_IRQ();
  #endif
  return status;
}
//-----------------------------------------------------------------------------
/* FIFO Write API */
uint32_t fifo_write (fifo_t* fifo, const uint8_t* BUF, uint32_t size) {
	uint32_t wSize = 0;
	if (size == 0 || fifo == NULL || BUF == NULL) {
		return 0;
	}
  uint32_t freeSize = fifo_get_freeSize(fifo);
  if (freeSize == 0) {
    return 0;
  }
  if (freeSize < size) {
    size = freeSize;
  }
  if (fifo->mutexWrite == true) {
    return 0;
  }
  fifo->mutexWrite = true;
	wSize = size;
	while (wSize-- > 0) {
    *fifo->pWrite++ = *BUF++;
    if (fifo->pWrite >= (fifo->buf + fifo->totalSize)) {
      fifo->pWrite = fifo->buf;
    }
    fifo->occupySize++;
	}
  fifo->mutexWrite = false;
	return size;
}
//-----------------------------------------------------------------------------
/* FIFO Write API */
uint32_t  fifo_read (fifo_t* fifo, uint8_t* buf, uint32_t size) {
  uint32_t rSize = 0;
	uint32_t occupySize = 0;
	if (size == 0 || fifo == NULL || buf == NULL) {
		return 0;
	}
  occupySize = fifo_get_occupySize(fifo);
  if (occupySize == 0) {
    return 0;
  }
  if (occupySize < size) {
    size = occupySize;
  }
  if (fifo->mutexRead == true) {
    return 0;
  }
  fifo->mutexRead = true;
	rSize = size;
	while (rSize-- > 0) {
		*buf++ = *fifo->pRead++;
		if (fifo->pRead >= (fifo->buf + fifo->totalSize)) {
			fifo->pRead = fifo->buf;
		}
    fifo->occupySize--;
	}
  fifo->mutexRead = false;
	return size;
}
//-----------------------------------------------------------------------------
/* FIFO Total Size Inquiry API */
uint32_t fifo_get_totalSize (const fifo_t* FIFO) {
  if (FIFO == NULL) {
    return 0;
  }
  return FIFO->totalSize;
}
//-----------------------------------------------------------------------------
/* FIFO Free Size Available Inquiry API */
uint32_t fifo_get_freeSize (const fifo_t* FIFO) {
  if (FIFO == NULL) {
    return 0;
  }
	return FIFO->totalSize - fifo_get_occupySize(FIFO);
}
//-----------------------------------------------------------------------------
/* FIFO Occupy Size Inquiry API */
uint32_t fifo_get_occupySize (const fifo_t* FIFO) {
  if (FIFO == NULL) {
    return 0;
  }
	return FIFO->occupySize;
}
//-----------------------------------------------------------------------------
/* FIFO Release API */
void fifo_release(fifo_t* fifo) {
	/* This API isn't Implemented */
}
