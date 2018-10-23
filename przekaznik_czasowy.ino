/*
  LiquidCrystal Library - Hello World

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD
 and shows the time.

  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe
 modified 7 Nov 2016
 by Arturo Guadalupi

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystalHelloWorld

*/
/*
 * simple time relay based on atmega328 (arduino)
 * by witek sp3jdz
 * 2018-10-17
 *
 */
/*
 * ToDo
 * - ok obsługa wyjścia
 * - ok sprawdzenie DEBUG
 * - blokada enkodera po pewnym czasie
 * - ok sygnalizowane bipem
 * - ok naciśnięcie klawisza - sygnalizacja dźwiękowa
 * - ok ograniczenie czasów min-max (np. 40-999)
 * - polskie litery na wł i wył (ł)
 * - element wykonawczy - IRFZ44
 */


// include the library code:
#include <EEPROM.h>
#include <LiquidCrystal.h>
#include <TimerOne.h>
#include "Bounce2.h"
#include "przekaznik_czasowy.h"

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = A2, en = A3, d4 = A4, d5 = A5, d6 = 2, d7 = 4;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
Bounce key_up = Bounce();
Bounce key_down = Bounce();
boolean now_up = false;
boolean now_down = false;
unsigned int time_up = 100;
unsigned int time_down = 100;
boolean byla_zmiana = false;
unsigned long czas_zmiany;
unsigned long time_start;		// start time of change

int8_t enc_delta;							// -128 ... 127
void encode_read()
{
	static int8_t last;
	static boolean output_is_on;
	int8_t nowy;
	int8_t diff;
	nowy = 0;
	if (digitalRead(ENC_A_PIN) == LOW)
		nowy = 3;
	if (digitalRead(ENC_B_PIN) == LOW)
		nowy ^= 1;								// convert gray to binary
	diff = last - nowy;						// difference last - nowy
	if (diff & 1)
	{							// bit 0 = value (1)
		last = nowy;							// store nowy as next last
		enc_delta += (diff & 2) - 1;		// bit 1 = direction (+/-)
	}
	if (output_is_on)
	{
		if (millis() - time_start >= time_up)
		{
			digitalWrite(OUTPUT_PIN, LOW);
			time_start = millis();
			output_is_on = false;
		}
	}
	else
	{
		if (millis() - time_start >= time_down)
		{
			digitalWrite(OUTPUT_PIN, HIGH);
			time_start = millis();
			output_is_on = true;
		}
	}
}
int8_t encode_read4(void)// read four step encoders; funkcja dla enkodera kwadraturowego
{
	int8_t val;
	noInterrupts();
	val = enc_delta;
	enc_delta &= 3;
	interrupts();
	return val >> 2;
}
void show_time_up()
{
	char bufor[8];
	sprintf(bufor, "%3d ms", time_up);
	lcd.setCursor(0, 1);
	lcd.print(bufor);
}
void show_time_down()
{
	char bufor[8];
	sprintf(bufor, "%3d ms", time_down);
	lcd.setCursor(8, 1);
	lcd.print(bufor);
}
void setup()
{
#if defined(DEBUG)
	Serial.begin(1200);
	Serial.println("time relay starting...");
#endif
	byte coldstart;
	  coldstart = EEPROM.read(0);              // Grab the coldstart byte indicator in EEPROM for
	                                           // comparison with the COLDSTART_REFERENCE
	  // Initialize frequency and position memories if first upload, COLDSTART_REF has been modified in ML.h
	  // since last upload OR if the "Clear All" command has been issued through the Controller Menu functions (0xfe)
	  if (coldstart != COLDSTART_REF)
	  {

	    EEPROM.write(TIME_UP_ADDRESS, time_up);           // writing running time up into eeprom
	    EEPROM.write(TIME_DOWN_ADDRESS, time_down);			// writing running time down into EEPROM
	    EEPROM.write(0, COLDSTART_REF);               // COLDSTART_REF in first byte indicates all initialized
#if defined(DEBUG)
	Serial.println("writing initial values into memory");
#endif
	  }
	  else                                           // EEPROM contains stored data, retrieve the data
	  {
	    //EEPROM_readAnything(TLUMIENIE_ADRES, tlumienie);            // read the current attenuation
	    time_up = EEPROM.read(TIME_UP_ADDRESS);
	    time_down = EEPROM.read(TIME_DOWN_ADDRESS);
#if defined(DEBUG)
	Serial.println("reading from memory: ");
	Serial.println(time_up);
	Serial.println(time_down);
#endif

	  }
	// set up the LCD's number of columns and rows:
	lcd.begin(16, 2);
	// Print a message to the LCD.
	lcd.print("AKS rulez!");
	pinMode(ENC_A_PIN, INPUT_PULLUP);					// wejście enkodera encoder input init
	pinMode(ENC_B_PIN, INPUT_PULLUP);					// wejście enkodera encoder input init
	pinMode(TIME_UP_PIN, INPUT_PULLUP);					// button up init
	pinMode(TIME_DOWN_PIN, INPUT_PULLUP);				// button down init
	pinMode(OUTPUT_PIN, OUTPUT);						// output
	digitalWrite(OUTPUT_PIN, LOW);
	key_up.attach(TIME_UP_PIN, INPUT_PULLUP);
	key_down.attach(TIME_DOWN_PIN, INPUT_PULLUP);
	delay(2000);
	time_start = millis();
	Timer1.initialize(250); // set a timer of length 250us - odczyt wejść enkodera będzie się odbywał co 0,25ms
	Timer1.attachInterrupt(encode_read); // attach the service routine here
	lcd.setCursor(0, 0);
	lcd.print("czas wl czas wyl");
	show_time_up();
	show_time_down();
}

void loop()
{
	key_up.update();
	if (key_up.read() == LOW)
	{
		now_up = true;
		now_down = false;
		tone(3, 1000, 100);
	}
	key_down.update();
	if (key_down.read() == LOW)
	{
		now_down = true;
		now_up = false;
		tone(3, 1000, 100);
	}
	int enc = encode_read4();
	if (enc != 0)
	{
		if (now_up)
		{
			byla_zmiana = true;
			time_up = time_up + enc;
			time_up = constrain(time_up, TIME_MIN, 999);
			show_time_up();
			czas_zmiany = millis();
		}
		if (now_down)
		{
			byla_zmiana = true;
			time_down = time_down + enc;
			time_down = constrain(time_down, TIME_MIN, 999);
			show_time_down();
			czas_zmiany = millis();
		}
	}
	if (byla_zmiana && (millis() - czas_zmiany > CZAS_REAKCJI))
	{
	    EEPROM.write(TIME_UP_ADDRESS, time_up);           // writing running time up into eeprom
	    EEPROM.write(TIME_DOWN_ADDRESS, time_down);			// writing running time down into EEPROM
		byla_zmiana = false;
		tone(3, 1000, 300);
#if defined(DEBUG)
		Serial.println("writing current timings to EEPROM: ");
		Serial.println(time_up);
		Serial.println(time_down);
#endif
	}

}

