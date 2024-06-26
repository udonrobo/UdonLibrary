/* Copyright (C) 2013 Kristian Lauszus, TKJ Electronics. All rights reserved.

 This software may be distributed and modified under the terms of the GNU
 General Public License version 2 (GPL2) as published by the Free Software
 Foundation and appearing in the file GPL2.TXT included in the packaging of
 this file. Please note that GPL2 Section 2[b] requires that all works based
 on this software must also be made publicly available under the terms of
 the GPL2 ("Copyleft").

 Contact information
 -------------------

 Kristian Lauszus, TKJ Electronics
 Web      :  http://www.tkjelectronics.com
 e-mail   :  kristianl@tkjelectronics.com
 */

#include "XBOXOLD.h"
// To enable serial debugging see "settings.h"
//#define EXTRADEBUG // Uncomment to get even more debugging data
//#define PRINTREPORT // Uncomment to print the report send by the Xbox controller

/** Buttons on the controllers */
const uint8_t XBOXOLD_BUTTONS[] PROGMEM = {
        0x01, // UP
        0x08, // RIGHT
        0x02, // DOWN
        0x04, // LEFT

        0x20, // BACK
        0x10, // START
        0x40, // L3
        0x80, // R3

        // A, B, X, Y, BLACK, WHITE, L1, and R1 are analog buttons
        4, // BLACK
        5, // WHTIE
        6, // L1
        7, // R1

        1, // B
        0, // A
        2, // X
        3, // Y
};

XBOXOLD::XBOXOLD(USB *p) :
pUsb(p), // pointer to USB class instance - mandatory
bAddress(0), // device address - mandatory
bPollEnable(false) { // don't start polling before dongle is connected
        for(uint8_t i = 0; i < XBOX_MAX_ENDPOINTS; i++) {
                epInfo[i].epAddr = 0;
                epInfo[i].maxPktSize = (i) ? 0 : 8;
                epInfo[i].bmSndToggle = 0;
                epInfo[i].bmRcvToggle = 0;
                epInfo[i].bmNakPower = (i) ? USB_NAK_NOWAIT : USB_NAK_MAX_POWER;
        }

        if(pUsb) // register in USB subsystem
                pUsb->RegisterDeviceClass(this); //set devConfig[] entry
}

uint8_t XBOXOLD::Init(uint8_t parent, uint8_t port, bool lowspeed) {
        uint8_t buf[sizeof (USB_DEVICE_DESCRIPTOR)];
        USB_DEVICE_DESCRIPTOR * udd = reinterpret_cast<USB_DEVICE_DESCRIPTOR*>(buf);
        uint8_t rcode;
        UsbDevice *p = NULL;
        EpInfo *oldep_ptr = NULL;
        uint16_t PID;
        uint16_t VID;

        // get memory address of USB device address pool
        AddressPool &addrPool = pUsb->GetAddressPool();
#ifdef EXTRADEBUG
        Notify(PSTR("\r\nXBOXUSB Init"), 0x80);
#endif
        // check if address has already been assigned to an instance
        if(bAddress) {
#ifdef DEBUG_USB_HOST
                Notify(PSTR("\r\nAddress in use"), 0x80);
#endif
                return USB_ERROR_CLASS_INSTANCE_ALREADY_IN_USE;
        }

        // Get pointer to pseudo device with address 0 assigned
        p = addrPool.GetUsbDevicePtr(0);

        if(!p) {
#ifdef DEBUG_USB_HOST
                Notify(PSTR("\r\nAddress not found"), 0x80);
#endif
                return USB_ERROR_ADDRESS_NOT_FOUND_IN_POOL;
        }

        if(!p->epinfo) {
#ifdef DEBUG_USB_HOST
                Notify(PSTR("\r\nepinfo is null"), 0x80);
#endif
                return USB_ERROR_EPINFO_IS_NULL;
        }

        // Save old pointer to EP_RECORD of address 0
        oldep_ptr = p->epinfo;

        // Temporary assign new pointer to epInfo to p->epinfo in order to avoid toggle inconsistence
        p->epinfo = epInfo;

        p->lowspeed = lowspeed;

        // Get device descriptor
        rcode = pUsb->getDevDescr(0, 0, sizeof (USB_DEVICE_DESCRIPTOR), (uint8_t*)buf); // Get device descriptor - addr, ep, nbytes, data
        // Restore p->epinfo
        p->epinfo = oldep_ptr;

        if(rcode)
                goto FailGetDevDescr;

        VID = udd->idVendor;
        PID = udd->idProduct;

        if(!VIDPIDOK(VID, PID)) // Check if VID and PID match
                goto FailUnknownDevice;

        // Allocate new address according to device class
        bAddress = addrPool.AllocAddress(parent, false, port);

        if(!bAddress)
                return USB_ERROR_OUT_OF_ADDRESS_SPACE_IN_POOL;

        // Extract Max Packet Size from device descriptor
        epInfo[0].maxPktSize = udd->bMaxPacketSize0;

        // Assign new address to the device
        rcode = pUsb->setAddr(0, 0, bAddress);
        if(rcode) {
                p->lowspeed = false;
                addrPool.FreeAddress(bAddress);
                bAddress = 0;
#ifdef DEBUG_USB_HOST
                Notify(PSTR("\r\nsetAddr: "), 0x80);
                D_PrintHex<uint8_t > (rcode, 0x80);
#endif
                return rcode;
        }
#ifdef EXTRADEBUG
        Notify(PSTR("\r\nAddr: "), 0x80);
        D_PrintHex<uint8_t > (bAddress, 0x80);
#endif
        //delay(300); // Spec says you should wait at least 200ms

        p->lowspeed = false;

        //get pointer to assigned address record
        p = addrPool.GetUsbDevicePtr(bAddress);
        if(!p)
                return USB_ERROR_ADDRESS_NOT_FOUND_IN_POOL;

        p->lowspeed = lowspeed;

        // Assign epInfo to epinfo pointer - only EP0 is known
        rcode = pUsb->setEpInfoEntry(bAddress, 1, epInfo);
        if(rcode)
                goto FailSetDevTblEntry;

        /* The application will work in reduced host mode, so we can save program and data
           memory space. After verifying the VID we will use known values for the
           configuration values for device, interface, endpoints and HID for the XBOX controllers */

        /* Initialize data structures for endpoints of device */
        epInfo[ XBOX_INPUT_PIPE ].epAddr = 0x01; // XBOX report endpoint
        epInfo[ XBOX_INPUT_PIPE ].epAttribs = USB_TRANSFER_TYPE_INTERRUPT;
        epInfo[ XBOX_INPUT_PIPE ].bmNakPower = USB_NAK_NOWAIT; // Only poll once for interrupt endpoints
        epInfo[ XBOX_INPUT_PIPE ].maxPktSize = EP_MAXPKTSIZE;
        epInfo[ XBOX_INPUT_PIPE ].bmSndToggle = 0;
        epInfo[ XBOX_INPUT_PIPE ].bmRcvToggle = 0;
        epInfo[ XBOX_OUTPUT_PIPE ].epAddr = 0x02; // XBOX output endpoint
        epInfo[ XBOX_OUTPUT_PIPE ].epAttribs = USB_TRANSFER_TYPE_INTERRUPT;
        epInfo[ XBOX_OUTPUT_PIPE ].bmNakPower = USB_NAK_NOWAIT; // Only poll once for interrupt endpoints
        epInfo[ XBOX_OUTPUT_PIPE ].maxPktSize = EP_MAXPKTSIZE;
        epInfo[ XBOX_OUTPUT_PIPE ].bmSndToggle = 0;
        epInfo[ XBOX_OUTPUT_PIPE ].bmRcvToggle = 0;

        rcode = pUsb->setEpInfoEntry(bAddress, 3, epInfo);
        if(rcode)
                goto FailSetDevTblEntry;

        delay(200); // Give time for address change

        rcode = pUsb->setConf(bAddress, epInfo[ XBOX_CONTROL_PIPE ].epAddr, 1);
        if(rcode)
                goto FailSetConfDescr;

#ifdef DEBUG_USB_HOST
        Notify(PSTR("\r\nXbox Controller Connected\r\n"), 0x80);
#endif
        if(pFuncOnInit)
                pFuncOnInit(); // Call the user function
        XboxConnected = true;
        bPollEnable = true;
        return 0; // Successful configuration

        /* Diagnostic messages */
FailGetDevDescr:
#ifdef DEBUG_USB_HOST
        NotifyFailGetDevDescr();
        goto Fail;
#endif

FailSetDevTblEntry:
#ifdef DEBUG_USB_HOST
        NotifyFailSetDevTblEntry();
        goto Fail;
#endif

FailSetConfDescr:
#ifdef DEBUG_USB_HOST
        NotifyFailSetConfDescr();
#endif
        goto Fail;

FailUnknownDevice:
#ifdef DEBUG_USB_HOST
        NotifyFailUnknownDevice(VID, PID);
#endif
        rcode = USB_DEV_CONFIG_ERROR_DEVICE_NOT_SUPPORTED;

Fail:
#ifdef DEBUG_USB_HOST
        Notify(PSTR("\r\nXbox Init Failed, error code: "), 0x80);
        NotifyFail(rcode);
#endif
        Release();
        return rcode;
}

/* Performs a cleanup after failed Init() attempt */
uint8_t XBOXOLD::Release() {
        XboxConnected = false;
        pUsb->GetAddressPool().FreeAddress(bAddress);
        bAddress = 0;
        bPollEnable = false;
        return 0;
}

uint8_t XBOXOLD::Poll() {
        if(!bPollEnable)
                return 0;
        uint16_t BUFFER_SIZE = EP_MAXPKTSIZE;
        pUsb->inTransfer(bAddress, epInfo[ XBOX_INPUT_PIPE ].epAddr, &BUFFER_SIZE, readBuf); // input on endpoint 1
        readReport();
#ifdef PRINTREPORT
        printReport(BUFFER_SIZE); // Uncomment "#define PRINTREPORT" to print the report send by the Xbox controller
#endif
        return 0;
}

void XBOXOLD::readReport() {
        ButtonState = readBuf[2];

        for(uint8_t i = 0; i < sizeof (buttonValues); i++)
                buttonValues[i] = readBuf[i + 4]; // A, B, X, Y, BLACK, WHITE, L1, and R1

        hatValue[(size_t)AnalogHatEnum::LeftHatX] = (int16_t)(((uint16_t)readBuf[12] << 8) | readBuf[13]);
        hatValue[(size_t)AnalogHatEnum::LeftHatY] = (int16_t)(((uint16_t)readBuf[14] << 8) | readBuf[15]);
        hatValue[(size_t)AnalogHatEnum::RightHatX] = (int16_t)(((uint16_t)readBuf[16] << 8) | readBuf[17]);
        hatValue[(size_t)AnalogHatEnum::RightHatY] = (int16_t)(((uint16_t)readBuf[18] << 8) | readBuf[19]);

        //Notify(PSTR("\r\nButtonState"), 0x80);
        //PrintHex<uint8_t>(ButtonState, 0x80);

        if(ButtonState != OldButtonState || memcmp(buttonValues, oldButtonValues, sizeof (buttonValues)) != 0) {
                ButtonClickState = ButtonState & ~OldButtonState; // Update click state variable
                OldButtonState = ButtonState;

                for(uint8_t i = 0; i < sizeof (buttonValues); i++) {
                        if(oldButtonValues[i] == 0 && buttonValues[i] != 0)
                                buttonClicked[i] = true; // Update A, B, X, Y, BLACK, WHITE, L1, and R1 click state
                        oldButtonValues[i] = buttonValues[i];
                }
        }
}

void XBOXOLD::printReport(uint16_t length __attribute__((unused))) { //Uncomment "#define PRINTREPORT" to print the report send by the Xbox controller
#ifdef PRINTREPORT
        if(readBuf == NULL)
                return;
        for(uint8_t i = 0; i < length; i++) {
                D_PrintHex<uint8_t > (readBuf[i], 0x80);
                Notify(PSTR(" "), 0x80);
        }
        Notify(PSTR("\r\n"), 0x80);
#endif
}

int8_t XBOXOLD::getAnalogIndex(ButtonEnum b) {
        // For legacy reasons these mapping indices not match up,
        // as the original code uses L1/R1 for the triggers and
        // L2/R2 for the white/black buttons. To fix these new enums
        // we have to transpose the keys before passing them through
        // the button index function
        switch (b) {
        case(ButtonEnum::LT): b = ButtonEnum::L1; break;  // normally L2
        case(ButtonEnum::RT): b = ButtonEnum::R1; break;  // normally R2
        case(ButtonEnum::LB): b = ButtonEnum::WHITE; break;  // normally L1
        case(ButtonEnum::RB): b = ButtonEnum::BLACK; break;  // normally R1
        default: break;
        }

        // A, B, X, Y, BLACK, WHITE, L1, and R1 are analog buttons
        const int8_t index = ButtonIndex(b);

        switch (index) {
        case ButtonIndex(ButtonEnum::A):
        case ButtonIndex(ButtonEnum::B):
        case ButtonIndex(ButtonEnum::X):
        case ButtonIndex(ButtonEnum::Y):
        case ButtonIndex(ButtonEnum::BLACK):
        case ButtonIndex(ButtonEnum::WHITE):
        case ButtonIndex(ButtonEnum::L1):
        case ButtonIndex(ButtonEnum::R1):
                return index;
        default: break;
        }

        return -1;
}

int8_t XBOXOLD::getDigitalIndex(ButtonEnum b) {
        // UP, DOWN, LEFT, RIGHT, START, BACK, L3, and R3 are digital buttons
        const int8_t index = ButtonIndex(b);

        switch (index) {
        case ButtonIndex(ButtonEnum::UP):
        case ButtonIndex(ButtonEnum::DOWN):
        case ButtonIndex(ButtonEnum::LEFT):
        case ButtonIndex(ButtonEnum::RIGHT):
        case ButtonIndex(ButtonEnum::START):
        case ButtonIndex(ButtonEnum::BACK):
        case ButtonIndex(ButtonEnum::L3):
        case ButtonIndex(ButtonEnum::R3):
                return index;
        default: break;
        }

        return -1;
}

uint8_t XBOXOLD::getButtonPress(ButtonEnum b) {
        const int8_t analogIndex = getAnalogIndex(b);
        if (analogIndex >= 0) {
                const uint8_t buttonIndex = pgm_read_byte(&XBOXOLD_BUTTONS[analogIndex]);
                return buttonValues[buttonIndex];
        }
        const int8_t digitalIndex = getDigitalIndex(b);
        if (digitalIndex >= 0) {
                const uint8_t buttonMask = pgm_read_byte(&XBOXOLD_BUTTONS[digitalIndex]);
                return (ButtonState & buttonMask);
        }
        return 0;
}

bool XBOXOLD::getButtonClick(ButtonEnum b) {
        const int8_t analogIndex = getAnalogIndex(b);
        if (analogIndex >= 0) {
                const uint8_t buttonIndex = pgm_read_byte(&XBOXOLD_BUTTONS[analogIndex]);
                if (buttonClicked[buttonIndex]) {
                        buttonClicked[buttonIndex] = false;
                        return true;
                }
                return false;
        }
        const int8_t digitalIndex = getDigitalIndex(b);
        if (digitalIndex >= 0) {
                const uint8_t mask = pgm_read_byte(&XBOXOLD_BUTTONS[digitalIndex]);
                const bool click = (ButtonClickState & mask);
                ButtonClickState &= ~mask;
                return click;
        }
        return 0;
}

int16_t XBOXOLD::getAnalogHat(AnalogHatEnum a) {
        return hatValue[(size_t)a];
}

/* Xbox Controller commands */
void XBOXOLD::XboxCommand(uint8_t* data, uint16_t nbytes) {
        //bmRequest = Host to device (0x00) | Class (0x20) | Interface (0x01) = 0x21, bRequest = Set Report (0x09), Report ID (0x00), Report Type (Output 0x02), interface (0x00), datalength, datalength, data)
        pUsb->ctrlReq(bAddress, epInfo[XBOX_CONTROL_PIPE].epAddr, bmREQ_HID_OUT, HID_REQUEST_SET_REPORT, 0x00, 0x02, 0x00, nbytes, nbytes, data, NULL);
}

void XBOXOLD::setRumbleOn(uint8_t lValue, uint8_t rValue) {
        uint8_t writeBuf[6];

        writeBuf[0] = 0x00;
        writeBuf[1] = 0x06;
        writeBuf[2] = 0x00;
        writeBuf[3] = rValue; // small weight
        writeBuf[4] = 0x00;
        writeBuf[5] = lValue; // big weight

        XboxCommand(writeBuf, 6);
}
