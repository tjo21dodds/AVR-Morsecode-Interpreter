//Contributing code from: Steve Gunn, Peter Dannegger (http://www.mikrocontroller.net/articles/Drehgeber)
/*
La Fortuna Morse Code interpreter (c) by Thomas Dodds

La Fortuna Morse Code interpreter is licensed under a
Creative Commons Attribution 3.0 Unported License.

You should have received a copy of the license along with this
work.  If not, see <http://creativecommons.org/licenses/by/3.0/>.
*/

/* Morse code reader */
//Based heavily off the rotary reader for task 1

/* For La Fortuna board

   | Port | Pin | Function         |
   |------+-----+------------------|
   | B    |   7 | Green LED        |
   | D    |   0 | Morse Listen Pin |

*/

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>
#include "lcd.h"
#include "morse.h"

#define ROTA	PE4
#define ROTB	PE5
#define STEP_DELAY_MS 5
#define MIN_STEP    2    /* > 0 */
#define MAX_STEP  255
#define RANGE_WIDTH 100

void debugData(uint8_t data[]);
void init(void);

int8_t enc_delta(void);
volatile int8_t delta;

uint16_t max = 8000;

uint8_t arr[10] = {0,0,0,0,0,0,0,0,0,0};

int x;

void main(void) {
	uint8_t cnt = MAX_STEP/2;
	uint8_t i;
	int16_t res;
	int yMax = 10;
	char str[yMax];
	init();

  sei();
	uint8_t data[4];
	int b = 0;

	for (int k = 0; k<yMax; k=k+1){
		str[k] = ' ';
	}
	for (int j = 0; j < 4; j= j+1){
		data[j] = 0;
	}

	int y = 0;
	for (;;) {
		for (i=cnt; i > 0; --i) {
		   _delay_ms(STEP_DELAY_MS);
		   res = cnt + enc_delta();
		   if (res > MAX_STEP) {
			   cnt = MAX_STEP;
		   } else if (res < MIN_STEP) {
			   cnt = MIN_STEP;
		   } else {
			   cnt = res;
		   }
		}
		if (x==1 || x==0){
			PINB |= (1 << PINB7);

		}
		if (x == 9){
			data[b] = interpret(arr);
			if (data[b] == 2){
				display_string(": DASH; ");
				display_char(parse(data));
				display_string("\n");
			}
			else if (data[b] == 1){
				display_string(": DOT; ");
				display_char(parse(data));
				display_string("\n");
			}
			else{
				display_string(": BREAK\n");
				if (b!=0){

					str[y] = parse(data);
					y = y+1;

					for (int j = 0; j < 4; j= j+1){
						data[j] = 0;
					}

				}
				b = -1;
				display_string(str);
				display_string("\n");
			}
			for (int h =0; h < 10; h=h+1){
				arr[h] = 0;
			}
			b = b + 1;
			x = -1;
			if (b >=4){ //Max word lenght
				b = 0;

				str[y] = parse(data);
				y = y+1;
				display_string(str);
				display_string("\n");
				for (int j = 0; j < 4; j= j+1){
					data[j] = 0;
				}
			}
			if (y > yMax){
				y = 0;
			}
		}
		else if (PIND & (1 << PIND0)){
			display_string("-");
			arr[x] = 1;
		}
		else{
			display_string("_");
			arr[x] = 0;
		}
		x = x + 1;


	}
}

void debugData(uint8_t data[]){ //Useful for seeing data
	display_char('|');
	for (int u = 0; u < 4; u = u + 1){
		if (data[u]==1){
			display_char('.');
		}
		else if (data[u] == 2){
			display_char('-');
		}
		else{
			display_char(' ');
		}
	}
	display_char('|');
}


/* Configure I/O Ports */
void init(void) {

	/* 8MHz clock, no prescaling (DS, p. 48) */
	CLKPR = (1 << CLKPCE);
	CLKPR = 0;


        /* Configure I/O Ports */
	DDRB  |=  _BV(PB7);   /* LED pin out */
	PORTB &= ~_BV(PB7);   /* LED off */
  PORTE |= _BV(PE4);

  DDRE  &= ~_BV(PE5);
  PORTE |= _BV(PE5);


	/* ENABLE ENCODER INPUTS AND PULL-UPS */


	/* Timer 0 for switch scan interrupt: */
	TCCR0A = _BV(WGM01);
  TCCR0B = _BV(CS01)
         | _BV(CS00);   /* F_CPU / 64 */

  OCR0A = (int8_t) (F_CPU / (64.0*100) - 1);


        /* SET OCR0A FOR A 1 MS PERIOD */

        /* ENABLE TIMER INTERRUPT */
  TIMSK0 |= _BV(OCIE0A);
	init_lcd();
	x = 0;
}


 ISR( TIMER0_COMPA_vect ) {
     static int8_t last;
     int8_t new, diff;
     uint8_t wheel;

     wheel = PINE;
     new = 0;
     if( wheel  & _BV(PE4) ) new = 3;
     if( wheel  & _BV(PE5) )
	 new ^= 1;		       	/* convert gray to binary */
     diff = last - new;
     if( diff & 1 ){			/* bit 0 = value (1) */
	     last = new;
	     delta += (diff & 2) - 1;	/* bit 1 = direction (+/-) */
     }

}
/* read two step encoder */
int8_t enc_delta() {
    int8_t val;

    cli();
    val = delta;
    delta &= 1;
    sei();

    return val >> 1;
}
