#include "core/powerSave.h"
#include "core/utils.h"
#include <Arduino.h>
#include <LittleFS.h>
#include <cmath>
#include <globals.h>
#include <interface.h>

namespace {
constexpr const char *TouchCalibrationFile = "/calData";

void setHighIfValid(int pin) {
    if (pin < 0) return;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
}

void setOutputIfValid(int pin, int level) {
    if (pin < 0) return;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, level);
}
} // namespace

/***************************************************************************************
** Function name: _setup_gpio()
** Location: main.cpp
** Description:   initial setup for the device
***************************************************************************************/
void _setup_gpio() {
    setHighIfValid(TFT_CS);
    setOutputIfValid(TFT_DC, HIGH);
    setOutputIfValid(TFT_RST, HIGH);
    setOutputIfValid(TFT_BL, HIGH);

#if TOUCH_CS >= 0
    setHighIfValid(TOUCH_CS);
#endif

    setHighIfValid(CC1101_SS_PIN);
    setHighIfValid(NRF24_SS_PIN);
    setHighIfValid(SDCARD_CS);
    setHighIfValid(W5500_SS_PIN);

    bruceConfig.colorInverted = 0;
}

/***************************************************************************************
** Function name: _post_setup_gpio()
** Location: main.cpp
** Description:   second stage gpio setup to make a few functions work
***************************************************************************************/
void _post_setup_gpio() {
#if defined(HAS_TOUCH) && defined(USE_TFT_eSPI_TOUCH)
    uint16_t calData[5];
    File caldata = LittleFS.open(TouchCalibrationFile, "r");

    if (!caldata) {
        tft.setRotation(ROTATION);
        tft.calibrateTouch(calData, TFT_WHITE, TFT_BLACK, 10);

        caldata = LittleFS.open(TouchCalibrationFile, "w");
        if (caldata) {
            caldata.printf("%d\n%d\n%d\n%d\n%d\n", calData[0], calData[1], calData[2], calData[3], calData[4]);
            caldata.close();
        }
    } else {
        Serial.print("\ntft Calibration data: ");
        for (int i = 0; i < 5; i++) {
            String line = caldata.readStringUntil('\n');
            calData[i] = line.toInt();
            Serial.printf("%d, ", calData[i]);
        }
        Serial.println();
        caldata.close();
    }
    tft.setTouch(calData);
#endif
}

/***************************************************************************************
** Function name: getBattery()
** location: display.cpp
** Description:   Delivers the battery value from 1-100
***************************************************************************************/
int getBattery() { return 0; }

/***************************************************************************************
** Function name: isCharging()
** Description:   Default implementation that returns false
***************************************************************************************/
bool isCharging() { return false; }

/*********************************************************************
** Function: setBrightness
** location: settings.cpp
** set brightness value
**********************************************************************/
void _setBrightness(uint8_t brightval) {
#if TFT_BL >= 0
    if (brightval == 0) {
        analogWrite(TFT_BL, 0);
        return;
    }

    int dutyCycle = MINBRIGHT + round(((255 - MINBRIGHT) * brightval / 100));
    analogWrite(TFT_BL, dutyCycle);
#else
    (void)brightval;
#endif
}

/*********************************************************************
** Function: InputHandler
** Handles the variables PrevPress, NextPress, SelPress, AnyKeyPress and EscPress
**********************************************************************/
void InputHandler(void) {
    static unsigned long tm = 0;
    if (millis() - tm < 200 && !LongPress) return;

#if defined(HAS_TOUCH) && defined(USE_TFT_eSPI_TOUCH)
    TouchPoint t;
    checkPowerSaveTime();

    if (tft.getTouch(&t.x, &t.y)) {
        NextPress = false;
        PrevPress = false;
        UpPress = false;
        DownPress = false;
        SelPress = false;
        EscPress = false;
        AnyKeyPress = false;
        NextPagePress = false;
        PrevPagePress = false;
        touchPoint.pressed = false;

        if (!wakeUpScreen()) AnyKeyPress = true;
        else return;

        touchPoint.x = t.x;
        touchPoint.y = t.y;
        touchPoint.pressed = true;
        touchHeatMap(touchPoint);
        tm = millis();
    }
#endif
}

/*********************************************************************
** Function: powerOff
** location: mykeyboard.cpp
** Turns off the device (or try to)
**********************************************************************/
void powerOff() {}

/*********************************************************************
** Function: checkReboot
** location: mykeyboard.cpp
** Btn logic to turnoff the device (name is odd btw)
**********************************************************************/
void checkReboot() {}
