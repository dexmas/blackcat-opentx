#include "opentx.h"
#include "debug.h"
#include "usb_driver.h"

extern "C" {
#include "usb/device/cdc-serial/CDCDSerialDriver.h"
#include "usb/device/cdc-serial/CDCDSerialDriverDescriptors.h"
}

void startCDCSerial()
{
	CDCDSerialDriver_Initialize();
}

void usbSerialPutc(uint8_t c)
{
	CDCDSerialDriver_Write(&c, 1, 0, 0);
}