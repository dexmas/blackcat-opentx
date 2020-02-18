#ifndef OPENTX_USB_DRIVER_H
#define OPENTX_USB_DRIVER_H

// USB driver
enum usbMode {
    USB_UNSELECTED_MODE,
    USB_JOYSTICK_MODE,
    USB_MASS_STORAGE_MODE,
    USB_SERIAL_MODE,
#if defined(USB_SERIAL)
    USB_MAX_MODE = USB_SERIAL_MODE
#else
    USB_MAX_MODE = USB_MASS_STORAGE_MODE
#endif
};

int usbPlugged();
void usbInit();
void usbStart();
void usbStop();
bool usbStarted();
int getSelectedUsbMode();
void setSelectedUsbMode(int mode);

void usbSerialPutc(uint8_t c);

// Used in view_statistics.cpp
#if defined(DEBUG) && !defined(BOOT)
extern uint16_t usbWraps;
extern uint16_t charsWritten;
extern volatile uint32_t APP_Rx_ptr_in;
extern volatile uint32_t APP_Rx_ptr_out;
#endif

#endif