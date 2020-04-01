
#include "board_lowlevel.h"

#if defined(BOOT)

static void lowLevelUsbCheck( void )
{
  PMC->PMC_PCER0 = (1<<ID_PIOC) ;       // Enable clock to PIOC
  PIOC->PIO_PER = PIO_PC25 ;              // Enable bit C25 (USB-detect)

  uint32_t i ;
  for ( i = 0 ; i < 50 ; i += 1 ) {
    __asm("nop") ;
  }

  for ( i = 0 ; i < 10 ; i += 1 ) {
    if ( PIOC->PIO_PDSR & 0x02000000 ) {
      PMC->PMC_PCDR0 = (1<<ID_PIOC)   ;       // Disable clock to PIOC
      dispUSB() ;
      sam_bootx() ;
    }
  }

  uint32_t x = initReadTrims() ;
  if ((x & 0x42) != 0x42) {
    run_application();
  }
}
#endif