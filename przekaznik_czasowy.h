/*
 * przekaznik_czasowy.h
 *
 *  Created on: 17.10.2018
 *      Author: witek
 */

#ifndef PRZEKAZNIK_CZASOWY_H_
#define PRZEKAZNIK_CZASOWY_H_

#define ENC_A 9						// encoder PIN
#define ENC_B 10					// encoder PIN
#define TIME_UP_PIN A0				// time up button PIN
#define TIME_DOWN_PIN 7				// time down button PIN
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

void show_time_up();
void show_time_down();

#endif /* PRZEKAZNIK_CZASOWY_H_ */
