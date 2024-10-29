
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "fifo.h"


/**
 * @brief This function creates a FIFO object and initializes it
 * 
 * @param fifo fifo instance pointer
 * @param buf The memory used by FIFO
 * @param size The size of the memory used by FIFA in bytes
 * @return true If the FIFO object is created and initialized correctly
 * @return false If the FIFO object is not created
 */
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

/**
 * @brief This function completely unloads the memory of a FIFO
 * 
 * @param fifo fifo instance pointer
 * @return true If the FIFO memory is flushed properly
 * @return false If it is not possible to clear the FIFO memory area
 */
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

/**
 * @brief This function saves a buffer inside the FIFO.
 *    If the size of the desired buffer is greater than the amount of free space in the FIFO,
 *    it only stores as much as it can fit.
 * 
 * @param fifo fifo instance pointer
 * @param BUF The buffer to be stored in FIFO
 * @param size The size of the buffer to be stored in FIFO in bytes
 * @return uint32_t Number of bytes stored in the fifo
 */
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

/**
 * @brief This function reads desired number of bytes from FIFO
 * 
 * @param fifo fifo instance pointer
 * @param buf The buffer to be stored from FIFO
 * @param size Destination buffer size in bytes
 * @return uint32_t Number of bytes read from FIFO
 */
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

/**
 * @brief This function returns the FIFO total size in bytes
 * 
 * @param FIFO fifo instance pointer
 * @return uint32_t Total FIFO size in bytes
 */
uint32_t fifo_get_totalSize (const fifo_t* FIFO) {
  if (FIFO == NULL) {
    return 0;
  }
  return FIFO->totalSize;
}

/**
 * @brief This function returns free available space of a FIFO in bytes
 * 
 * @param FIFO fifo instance pointer
 * @return uint32_t Number of free spaces in bytes
 */
uint32_t fifo_get_freeSize (const fifo_t* FIFO) {
  if (FIFO == NULL) {
    return 0;
  }
	return FIFO->totalSize - fifo_get_occupySize(FIFO);
}

/**
 * @brief This function returns number of occupied spaces in bytes
 * 
 * @param FIFO fifo instance pointer
 * @return uint32_t Number of Occupied size of a FIFO in bytes
 */
uint32_t fifo_get_occupySize (const fifo_t* FIFO) {
  if (FIFO == NULL) {
    return 0;
  }
	return FIFO->occupySize;
}
//-----------------------------------------------------------------------------
/**
 * @brief This API removes a FIFO object completely
 * 
 * @param fifo fifo instance pointer
 * @attention This API is not implemented at the moment
 */
void fifo_release(fifo_t* fifo) {
	
}
