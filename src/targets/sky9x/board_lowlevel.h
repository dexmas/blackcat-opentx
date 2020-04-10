#ifndef _BOARD_H_
#define _BOARD_H_

#include <stdint.h>
#include "sam4s16c.h"

#if !defined(SIMU)
#include "core_cm4.h"
#endif

#define BLOCK_SIZE                     512
#define MSD_BUFFER_SIZE                (12*BLOCK_SIZE) // Size of the MSD IO buffer in bytes (6K, more the better)
#define SD_CARD_PRESENT()              ( (PIOB->PIO_PDSR & PIO_PB7) == 0 )

// States for initialising card
#define SD_ST_ERR               -1
#define SD_ST_STARTUP           0
#define SD_ST_EMPTY             1
#define SD_ST_INIT1             2
#define SD_ST_INIT2             3
#define SD_ST_IDLE              4
#define SD_ST_READY             5
#define SD_ST_IDENT             6
#define SD_ST_STBY              7
#define SD_ST_TRAN              8
#define SD_ST_DATA              9
#define SD_ST_MOUNTED           10

extern uint32_t Cmd_A41_resp;
extern int32_t Card_state;
extern volatile uint32_t Card_initialized;
extern uint32_t Card_CSD[4];
extern uint32_t transSpeed;

#define OCR_SD_CCS             (1UL << 30)

#define SD_CSD(csd, bitfield, bits)   ((csd[3-(bitfield)/32] >> ((bitfield)%32)) & ((1 << (bits)) - 1))

#define SD_CSD_TRAN_SPEED(pSd)         SD_CSD(pSd, 96,  8) ///< Max. data transfer rate

#define SD_CSD_C_SIZE(pSd)             ((SD_CSD(pSd, 72,  2) << 10) + \
                                        (SD_CSD(pSd, 64,  8) << 2)  + \
                                        SD_CSD(pSd, 62,  2)) ///< Device size

#define SD_CSD_C_SIZE_HC(csd)          ((SD_CSD(csd, 64,  6) << 16) + \
                                        (SD_CSD(csd, 56,  8) << 8)  + \
                                        SD_CSD(csd, 48,  8)) ///< Device size v2.0 High Capacity

#define SD_CSD_C_SIZE_MULT(pSd)        SD_CSD(pSd, 47,  3) ///< Device size multiplier

#define SD_CSD_MULT(pSd)               (1 << (SD_CSD_C_SIZE_MULT(pSd) + 2))

#define SD_CSD_READ_BL_LEN(pSd)        SD_CSD(pSd, 80,  4) ///< Max. read data block length

#define SD_CSD_BLKLEN(pSd)             (1 << (SD_CSD_READ_BL_LEN(pSd) - 9))

#define SD_CSD_BLOCKNR(pSd)            ((SD_CSD_C_SIZE(pSd) + 1) * SD_CSD_MULT(pSd)) * SD_CSD_BLKLEN(pSd)

#define SD_CSD_BLOCKNR_HC(pSd)         ((SD_CSD_C_SIZE_HC(pSd) + 1) * 1024)

#if !defined(SIMU)
  #define SD_IS_HC()                     (Cmd_A41_resp & OCR_SD_CCS)
  #define SD_GET_BLOCKNR()               (SD_IS_HC() ? (SD_CSD_BLOCKNR_HC(Card_CSD)) : (SD_CSD_BLOCKNR(Card_CSD)))
  #define SD_GET_SPEED()                 (transSpeed)
#else
  #define SD_IS_HC()              (0)
  #define SD_GET_BLOCKNR()        (0)
  #define SD_GET_SPEED()          (0)
#endif

// BlueTooth
#define BT_USART       UART1
#define BT_ID          ID_UART1

extern uint32_t Master_frequency;
extern volatile unsigned char g_interrupt_enabled;

struct t_serial_tx
{
  uint8_t *buffer ;
  uint16_t size ;
  volatile uint16_t ready ;
};


#define Min(a, b) (((a) < (b)) ?  (a) : (b))
#define Max(a, b) (((a) > (b)) ?  (a) : (b))

/*! \name Bit-Field Handling
 */
 //! @{

#define ctz(u) ((u) ? __builtin_ctz(u) : 32)

/*! \brief Tests the bits of a value specified by a given bit-mask.
 *
 * \param value Value of which to test bits.
 * \param mask  Bit-mask indicating bits to test.
 *
 * \return \c 1 if at least one of the tested bits is set, else \c 0.
 */
#define Tst_bits( value, mask)  (Rd_bits(value, mask) != 0)

 /*! \brief Reads the bits of a value specified by a given bit-mask.
  *
  * \param value Value to read bits from.
  * \param mask  Bit-mask indicating bits to read.
  *
  * \return Read bits.
  */
#define Rd_bits( value, mask)        ((value) & (mask))

  /*! \brief Writes the bits of a C lvalue specified by a given bit-mask.
   *
   * \param lvalue  C lvalue to write bits to.
   * \param mask    Bit-mask indicating bits to write.
   * \param bits    Bits to write.
   *
   * \return Resulting value with written bits.
   */
#define Wr_bits(lvalue, mask, bits)  ((lvalue) = ((lvalue) & ~(mask)) |\
                                                 ((bits  ) &  (mask)))

   /*! \brief Clears the bits of a C lvalue specified by a given bit-mask.
    *
    * \param lvalue  C lvalue of which to clear bits.
    * \param mask    Bit-mask indicating bits to clear.
    *
    * \return Resulting value with cleared bits.
    */
#define Clr_bits(lvalue, mask)  ((lvalue) &= ~(mask))

    /*! \brief Sets the bits of a C lvalue specified by a given bit-mask.
     *
     * \param lvalue  C lvalue of which to set bits.
     * \param mask    Bit-mask indicating bits to set.
     *
     * \return Resulting value with set bits.
     */
#define Set_bits(lvalue, mask)  ((lvalue) |=  (mask))

     /*! \brief Reads the bit-field of a value specified by a given bit-mask.
      *
      * \param value Value to read a bit-field from.
      * \param mask  Bit-mask indicating the bit-field to read.
      *
      * \return Read bit-field.
      */
#define Rd_bitfield( value, mask)           (Rd_bits( value, mask) >> ctz(mask))

      /*! \brief Writes the bit-field of a C lvalue specified by a given bit-mask.
       *
       * \param lvalue    C lvalue to write a bit-field to.
       * \param mask      Bit-mask indicating the bit-field to write.
       * \param bitfield  Bit-field to write.
       *
       * \return Resulting value with written bit-field.
       */
#define Wr_bitfield(lvalue, mask, bitfield) (Wr_bits(lvalue, mask, (uint32_t)(bitfield) << ctz(mask)))

/**
 * \brief Initialize interrupt vectors
 *
 * For NVIC the interrupt vectors are put in vector table. So nothing
 * to do to initialize them, except defined the vector function with
 * right name.
 *
 * This must be called prior to \ref irq_register_handler.
 */
#define irq_initialize_vectors()   \
	do {                           \
	} while(0)

#define cpu_irq_enable()                     \
	do {                                     \
		g_interrupt_enabled = 1;             \
		__DMB();                             \
		__enable_irq();                      \
	} while (0)

#define cpu_irq_disable()                    \
	do {                                     \
		__disable_irq();                     \
		__DMB();                             \
		g_interrupt_enabled = 0;             \
	} while (0)

#define cpu_irq_is_enabled()    (__get_PRIMASK() == 0)

static inline uint32_t cpu_irq_save(void)
{
    volatile uint32_t flags = cpu_irq_is_enabled();
    cpu_irq_disable();
    return flags;
}

static inline void cpu_irq_restore(uint32_t flags)
{
    if (flags) {
        cpu_irq_enable();
    }
}

#endif // _BOARD_H_
