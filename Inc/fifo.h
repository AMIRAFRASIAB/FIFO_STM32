/* يَا أَيُّهَا الَّذِينَ آمَنُوا اسْتَعِينُوا بِالصَّبْرِ وَالصَّلَاةِ ۚ إِنَّ اللَّهَ مَعَ الصَّابِرِينَ */

#ifndef __FIFO_H_INCLUDED__
#define __FIFO_H_INCLUDED__

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
  extern "C" {
#endif


/**
 * @file fifo.h
 * @author AmirHosein Bagheri (whiteWolf97@yahoo.com)
 * @brief 
 * @version 0.1
 * @date 2024-10-29
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <arm_compat.h>
#define FIFO_DISABLE_IRQ     __disable_irq/* Place Your Macro Here */
#define FIFO_ENABLE_IRQ      __enable_irq /* Place Your Macro Here */

#if !defined(FIFO_DISABLE_IRQ) || !defined(FIFO_ENABLE_IRQ)
  #Error "Set FIFO_DISABLE_IRQ And FIFO_ENABLE_IRQ Macros"
#endif




typedef struct {
	uint8_t*  buf;
	uint8_t*  pWrite;
	uint8_t*  pRead;
	uint32_t  totalSize;
  uint32_t  occupySize;
  bool      mutexWrite;
  bool      mutexRead;  
} fifo_t;


bool     fifo_register (fifo_t* fifo, uint8_t* buf, uint32_t size);
bool     fifo_flush (fifo_t* fifo);
void     fifo_release (fifo_t* fifo);
uint32_t fifo_write (fifo_t* fifo, const uint8_t* BUF, uint32_t size);
uint32_t fifo_read (fifo_t* fifo, uint8_t* buf, uint32_t size);
uint32_t fifo_get_totalSize (const fifo_t* FIFO);
uint32_t fifo_get_freeSize (const fifo_t* FIFO);
uint32_t fifo_get_occupySize (const fifo_t* FIFO);


#ifdef __cplusplus
  }
#endif 

#endif
