/*
 * przekaznik_czasowy.h
 *
 *  Created on: 17.10.2018
 *      Author: witek
 */

#ifndef PRZEKAZNIK_CZASOWY_H_
#define PRZEKAZNIK_CZASOWY_H_

//#define DEBUG
#define ENC_A_PIN 10						// encoder PIN
#define ENC_B_PIN 9					// encoder PIN
#define TIME_UP_PIN (unsigned char)A0				// time up button PIN
#define TIME_DOWN_PIN 7				// time down button PIN
#define OUTPUT_PIN 5				// output PIN - driving power MOSFET
#define COLDSTART_REF      0x12   // When started, the firmware examines this "Serial Number
                                  // and enforces factory reset to clear all
                                  // settings, as well as frequency and position memories.
                                  // To roll this value is useful if there is chance of a
                                  // mismatch due to restructuring of the EEPROM.
                                  // Else - the stepper may take on on epic journey... :)
                                  // COLDSTART_REF can be any unique number between
                                  // 0x01 and 0xfd.  0xfe is reserved for use by the firmware
                                  // to clear frequency/position memory data while retaining
                                  // controller_settings.
#define TIME_UP_ADDRESS 1
#define TIME_DOWN_ADDRESS 5
#define CZAS_REAKCJI 1000		// the time [ms] after which the writing into EEPROM takes place
#define TIME_MIN 1					// minimal value of time_up and time_down in ms
#define WY_H_BRIDGE_1	0			// pierwsze wyjście do sterowania układu mostkowego
#define WY_H_BRIDGE_2	1			// drugie wyjście do sterowania układu mostkowego

void show_time_up();
void show_time_down();

#endif /* PRZEKAZNIK_CZASOWY_H_ */
