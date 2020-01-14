
C_COMPILER   = arm-none-eabi-gcc
CXX_COMPILER = arm-none-eabi-g++
ASM_COMPILER = arm-none-eabi-as

CP_UTIL   = arm-none-eabi-objcopy
CLSS_UTIL = arm-none-eabi-objdump

BIN_UTIL  = $(CP_UTIL) -O ihex 
BINX_UTIL = $(CP_UTIL) -O binary 

PROJECT = blackcat-opentx

OBJDIR = ..\temp
BLDDIR = ..\build

ARCH = ARM
MCU  = cortex-m3
SDCARD = 1
HAPTIC = 1

VERSION = 2.22

GUI_DIR = 128x64
THIRDPARTY_DIR = thirdparty
LUA_DIR = $(THIRDPARTY_DIR)/Lua/src
COOS_DIR = $(THIRDPARTY_DIR)/CoOS
FATFS_DIR = $(THIRDPARTY_DIR)/FatFs
TARGET_DIR = targets/sky9x
ARM_DIR = targets/common/arm

CPU_TYPE_FULL = ATSAM3S8C
DEBUG = 0

CPPDEFS += 	-Dat91sam3s8 -DREVX -DPCBSKY9X -DTRANSLATIONS_EN -DTRANSLATIONS=EN \
			-DPPM_PIN_SERIAL -DAUDIO_DUAL_BUFFER -DAUDIO -DVOICE -DRTCLOCK \
			-DRUN_FROM_FLASH=1 -DEEPROM -DEEPROM_RAW -DEEPROM_CONVERSIONS \
			-DFLAVOUR="\"9xrpro"\" -DSPLASH -DTIMERS=2 -DHAPTIC -DMULTIMODULE -DCPUARM -DVIRTUAL_INPUTS \
			-DTELEMETRY_FRSKY -DTELEMETRY_FRSKY_SPORT -DFRSKY_HUB -DGPS -DPXX -DDSM2 -DBOLD_FONT \
			-DBATTGRAPH -DTHRTRACE -DGAUGES -DREQUIRED_SDCARD_VERSION="\"$(VERSION)"\" \
			-DVARIO -DGRAPHICS -DFLIGHT_MODES -DEEPROM_VARIANT=1 \
			-DCURVES -DGVARS -DAUTOSOURCE -DAUTOSWITCH -DSDCARD -DPPM_UNIT_PERCENT_PREC0 \
			-DPPM_CENTER_ADJUSTABLE -DPPM_LIMITS_SYMETRICAL -DOVERRIDE_CHANNEL_FUNCTION \
			-DDANGEROUS_MODULE_FUNCTIONS -DFONT=SQT5 -DGUI -DCORRECT_NEGATIVE_SHIFTS
			
LDSCRIPT = $(TARGET_DIR)/sam3s8c_flash.ld
 
FULL_PRJ = $(BLDDIR)/$(PROJECT)_rom

SRC  =  	$(COOS_DIR)/kernel/core.c \
			$(COOS_DIR)/kernel/hook.c \
			$(COOS_DIR)/kernel/task.c \
			$(COOS_DIR)/kernel/event.c \
			$(COOS_DIR)/kernel/time.c \
			$(COOS_DIR)/kernel/timer.c \
			$(COOS_DIR)/kernel/flag.c \
			$(COOS_DIR)/kernel/mutex.c \
			$(COOS_DIR)/kernel/serviceReq.c \
			$(COOS_DIR)/portable/GCC/port.c \
			$(COOS_DIR)/portable/arch.c \
			\
			$(FATFS_DIR)/ff.c \
			$(FATFS_DIR)/fattime.c \
			$(FATFS_DIR)/option/ccsbcs.c \
			\
			$(TARGET_DIR)/core_cm3.c \
			$(TARGET_DIR)/board_lowlevel.c \
			$(TARGET_DIR)/crt.c \
			$(TARGET_DIR)/MEDSdcard.c \
			$(TARGET_DIR)/vectors_sam3s.c\
			\
			$(TARGET_DIR)/usb/device/core/USBD_UDP.c \
			$(TARGET_DIR)/usb/device/core/USBDDriver.c \
			$(TARGET_DIR)/usb/device/hid-joystick/HIDDJoystickDriver.c \
			$(TARGET_DIR)/usb/device/hid-joystick/HIDDJoystickDriverDescriptors.c \
			$(TARGET_DIR)/usb/device/hid-joystick/HIDDJoystickInputReport.c \
			$(TARGET_DIR)/usb/common/core/USBEndpointDescriptor.c \
			$(TARGET_DIR)/usb/common/core/USBGenericRequest.c \
			$(TARGET_DIR)/usb/common/core/USBFeatureRequest.c \
			$(TARGET_DIR)/usb/common/core/USBInterfaceRequest.c \
			$(TARGET_DIR)/usb/common/core/USBGetDescriptorRequest.c \
			$(TARGET_DIR)/usb/common/core/USBSetAddressRequest.c \
			$(TARGET_DIR)/usb/common/core/USBSetConfigurationRequest.c \
			$(TARGET_DIR)/usb/common/core/USBConfigurationDescriptor.c \
			$(TARGET_DIR)/usb/common/core/USBGenericDescriptor.c \
			$(TARGET_DIR)/usb/common/hid/HIDIdleRequest.c \
			$(TARGET_DIR)/usb/common/hid/HIDReportRequest.c \
			\
			syscalls.c

CPPSRC = 	io/frsky_sport.cpp \
			telemetry/telemetry.cpp \
			telemetry/telemetry_holders.cpp \
			telemetry/telemetry_sensors.cpp \
			telemetry/frsky.cpp \
			telemetry/frsky_d_arm.cpp \
			telemetry/frsky_sport.cpp \
			telemetry/flysky_ibus.cpp \
			telemetry/multi.cpp \
			telemetry/spektrum.cpp \
			pulses/pulses_arm.cpp \
			pulses/ppm_arm.cpp \
			pulses/pxx_arm.cpp \
			pulses/dsm2_arm.cpp \
			pulses/sbus_arm.cpp \
			pulses/multi_arm.cpp \
			\
			$(ARM_DIR)/loadboot.cpp \
			\
			$(TARGET_DIR)/board.cpp \
			$(TARGET_DIR)/telemetry_driver.cpp \
			$(TARGET_DIR)/adc_driver.cpp \
			$(TARGET_DIR)/pulses_driver.cpp \
			$(TARGET_DIR)/keys_driver.cpp \
			$(TARGET_DIR)/audio_driver.cpp \
			$(TARGET_DIR)/buzzer_driver.cpp \
			$(TARGET_DIR)/haptic_driver.cpp \
			$(TARGET_DIR)/sdcard_driver.cpp \
			$(TARGET_DIR)/serial2_driver.cpp \
			$(TARGET_DIR)/trainer_driver.cpp \
			$(TARGET_DIR)/eeprom_driver.cpp \
			$(TARGET_DIR)/lcd_driver.cpp \
			$(TARGET_DIR)/pwr_driver.cpp \
			$(TARGET_DIR)/diskio.cpp \
			$(TARGET_DIR)/rtc_driver.cpp \
			$(TARGET_DIR)/joystick.cpp \
			\
			storage/storage_common.cpp \
			storage/eeprom_common.cpp \
			storage/eeprom_raw.cpp \
			storage/eeprom_conversions.cpp \
			\
			gui/gui_common.cpp \
			gui/gui_common_arm.cpp \
			gui/$(GUI_DIR)/lcd.cpp \
			gui/$(GUI_DIR)/splash.cpp \
			gui/$(GUI_DIR)/fonts.cpp \
			gui/$(GUI_DIR)/navigation.cpp \
			gui/$(GUI_DIR)/popups.cpp \
			gui/$(GUI_DIR)/menus.cpp \
			gui/$(GUI_DIR)/menu_model.cpp \
			gui/$(GUI_DIR)/model_select.cpp \
			gui/$(GUI_DIR)/model_setup.cpp \
			gui/$(GUI_DIR)/model_logical_switches.cpp \
			gui/$(GUI_DIR)/model_special_functions.cpp \
			gui/$(GUI_DIR)/model_telemetry.cpp \
			gui/$(GUI_DIR)/menu_radio.cpp \
			gui/$(GUI_DIR)/radio_setup.cpp \
			gui/$(GUI_DIR)/radio_trainer.cpp \
			gui/$(GUI_DIR)/radio_version.cpp \
			gui/$(GUI_DIR)/radio_calibration.cpp \
			gui/$(GUI_DIR)/model_outputs_arm.cpp \
			gui/$(GUI_DIR)/model_input_edit.cpp \
			gui/$(GUI_DIR)/model_mix_edit.cpp \
			gui/$(GUI_DIR)/model_display.cpp \
			gui/$(GUI_DIR)/model_gvars.cpp \
			gui/$(GUI_DIR)/model_curve_edit.cpp \
			gui/$(GUI_DIR)/model_flightmodes.cpp \
			gui/$(GUI_DIR)/radio_diagkeys.cpp \
			gui/$(GUI_DIR)/radio_diaganas.cpp \
			gui/$(GUI_DIR)/radio_hardware.cpp \
			gui/$(GUI_DIR)/view_telemetry.cpp \
			gui/$(GUI_DIR)/view_about.cpp \
			gui/$(GUI_DIR)/view_main.cpp \
			gui/$(GUI_DIR)/view_statistics.cpp\
			gui/$(GUI_DIR)/widgets.cpp \
			gui/common/widgets.cpp \
			gui/common/navigation.cpp \
			gui/common/stdlcd/widgets.cpp \
			gui/common/stdlcd/popups.cpp \
			gui/common/stdlcd/model_inputs.cpp \
			gui/common/stdlcd/model_mixes.cpp \
			gui/common/stdlcd/view_text.cpp \
			gui/common/stdlcd/model_curves.cpp \
			gui/common/stdlcd/radio_sdmanager.cpp \
			gui/common/arm/widgets.cpp \
			\
			translations/tts_en.cpp \
			translations/tts_ru.cpp \
			\
			opentx.cpp \
			functions.cpp \
			strhelpers.cpp \
			switches.cpp \
			mixer.cpp \
			stamp.cpp \
			timers.cpp \
			trainer_input.cpp \
			main_arm.cpp \
			tasks_arm.cpp \
			audio_arm.cpp \
			haptic.cpp \
			serial.cpp \
			sdcard.cpp \
			rtc.cpp \
			logs.cpp \
			gvars.cpp \
			keys.cpp \
			maths.cpp \
			translations.cpp \
			curves.cpp \
			vario.cpp

ifeq ($(DEBUG), 1)		
	CPPSRC +=   debug.cpp 
	CPPDEFS += -DDEBUG -DUSB_SERIAL
endif

# List ASM source files here
ASRC =

# List all user directories here
UINCDIR =   $(TARGET_DIR) \
			$(THIRDPARTY_DIR) \
			$(FATFS_DIR) $(FATFS_DIR)/option \
			$(COOS_DIR) $(COOS_DIR)/kernel $(COOS_DIR)/portable \
			. \
			gui \
			gui/$(GUI_DIR) \
			../data/bitmaps \
			../data/bitmaps/$(GUI_DIR) \
			../data/fonts/sqt5


ULIBDIR =

#
# End of user defines
##############################################################################################

INCDIR  = $(patsubst %,-I%,$(DINCDIR) $(UINCDIR))
LIBDIR  = $(patsubst %,-L%,$(DLIBDIR) $(ULIBDIR))

ADEFS   = $(DADEFS) $(UADEFS)

# Generate the (raw) object files lists
AXOBJS    = $(ASRC:%.s=%.o)
CXOBJS    = $(SRC:%.c=%.o)
CPPXOBJS  = $(CPPSRC:%.cpp=%.o)

# Remove directory info from the object files lists
AOBJS    = $(addprefix $(OBJDIR)/,$(AXOBJS))
COBJS    = $(addprefix $(OBJDIR)/,$(CXOBJS))
CPPOBJS  = $(addprefix $(OBJDIR)/,$(CPPXOBJS))
DIRS = $(sort $(dir $(AXOBJS) $(CXOBJS) $(CPPXOBJS) ))

# Search path for sources
VPATH = $(DIRS)

INCLUDES = $(AOBJS:.o=.o.d) $(COBJS:.o=.o.d) $(CPPOBJS:.o=.o.d)

OBJS    = $(ASRC:%.s=$(OBJDIR)/%.o) $(SRC:%.c=$(OBJDIR)/%.o) $(CPPSRC:%.cpp=$(OBJDIR)/%.o)
LIBS    = $(DLIBS) $(ULIBS)
WRFLAGS = -Wall -Wno-strict-aliasing -Wformat -Wreturn-type -Wunused -Wuninitialized -Wunknown-pragmas -Wno-switch -Wtype-limits
MCFLAGS = -gdwarf-2 -mcpu=$(MCU) -mthumb -fomit-frame-pointer -fverbose-asm -Wa,-ahlms=$(FULL_PRJ).lst -fno-exceptions -fdata-sections -ffunction-sections 

# Define optimisation level here
OPT = -Os

CPFLAGS = $(MCFLAGS) $(OPT) $(WRFLAGS) -Wimplicit
LDFLAGS = -mcpu=$(MCU) -mthumb -nostartfiles -T$(LDSCRIPT) -lm -Wl,-Map=$(FULL_PRJ).map,--cref,--no-warn-mismatch,--gc-sections $(LIBDIR)
CPPFLAGS = $(MCFLAGS) $(OPT) $(WRFLAGS) -std=gnu++11

CPPFLAGS += $(CPPDEFS)
CPFLAGS += $(CPPDEFS)

CPFLAGS += -MD -MP -MF $(OBJDIR)/$@.d
CPPFLAGS += -MD -MP -MF $(OBJDIR)/$@.d

all: size_before stamp $(AOBJS) $(COBJS) $(CPPOBJS) $(FULL_PRJ).elf $(FULL_PRJ).hex $(FULL_PRJ).bin $(FULL_PRJ).lss size

size_before:
	@if exist $(FULL_PRJ).elf (arm-none-eabi-size $(FULL_PRJ).elf)

size:
	@if exist $(FULL_PRJ).elf (arm-none-eabi-size $(FULL_PRJ).elf)

stamp:
	@del /q /f stamp.h 2>nul
	@echo on
	@echo Generate stamp.h:
	@echo #define DATE    "%DATE%" >> stamp.h
	@echo #define TIME    "%TIME%" >> stamp.h
	@echo #define VERSION "$(VERSION)" >> stamp.h
	@echo: >> stamp.h
	@echo #define VERSION_MAJOR     2 >> stamp.h
	@echo #define VERSION_MINOR     2 >> stamp.h
	@echo #define VERSION_REVISION  2 >> stamp.h
	@echo #define GIT_STR           "0" >> stamp.h

$(OBJDIR)/%.o: %.cpp
	@if not exist $(@D) (mkdir $(subst /,\\, $(@D)))
	$(CXX_COMPILER) -c $(CPPFLAGS) -fno-exceptions -I . $(INCDIR) $< -o $@

$(OBJDIR)/%.o: %.c
	@if not exist $(@D) (mkdir $(subst /,\\, $(@D)))
	$(C_COMPILER) -c $(CPFLAGS) -I . $(INCDIR) $< -o $@

$(OBJDIR)/%.o: %.s
	@if not exist $(@D) (mkdir $(subst /,\\, $(@D)))
	$(ASM_COMPILER) -c $(ASFLAGS) $< -o $@

%.elf: $(AOBJS) $(COBJS) $(CPPOBJS) 
	$(C_COMPILER) $(AOBJS) $(COBJS) $(CPPOBJS) $(LDFLAGS) $(LIBS) -o $@

%.lss: %.elf
	$(CLSS_UTIL) -h -S $< > $@

%.hex: %.elf
	$(BIN_UTIL) $< $@

%.bin: %.elf
	$(BINX_UTIL) $< $@

clean:
	@if exist $(OBJDIR) (rd /s /q $(OBJDIR))
	@if exist $(BLDDIR) (
	@del /q /f $(BLDDIR)\*.elf 2>nul
	@del /q /f $(BLDDIR)\*.map 2>nul
	@del /q /f $(BLDDIR)\*.hex 2>nul
	@del /q /f $(BLDDIR)\*.bin 2>nul
	@del /q /f $(BLDDIR)\*.lss 2>nul )

-include $(INCLUDES)
