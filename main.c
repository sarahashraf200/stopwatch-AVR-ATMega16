#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
void init_INT0(void);
void init_INT1(void);
void init_INT2(void);
void  Timer_init(void);
unsigned char sec=0;
unsigned char mins=0;
unsigned char hrs=0;
unsigned char temp=0;
// flag will be set when the timer count 1 second
unsigned char count_second_flag = 0;

// External INT0 Enable and Configuration function
void init_INT0(void){
	// Configure INT0/PD2 as input pin
	    DDRD &=~(1<<PD2);
	    // Enable internal pull up resistor at INT0/PD2 pin
		PORTD|=(1<<PD2);
		// Trigger INT0 with the falling edge
	    MCUCR|=(1<<ISC01);
	    // Enable external interrupt pin INT0
		GICR |=(1<<INT0);
		





}

void init_INT1(void){
	// Configure INT1/PD3 as input pin
	    DDRD &= ~(1<<PD3);
	    // Trigger INT1 with the raising edge
		MCUCR|=(1<<ISC10)|(1<<ISC11);
		// Enable external interrupt pin INT1
		GICR |=(1<<INT1);
		


}
// External INT2 Enable and Configuration function
void init_INT2(void){
	// Configure INT2/PB2 as input pin
	    DDRB &=~(1<<PB2);
	    // Enable internal pull up resistor at INT2/PB2 pin
		PORTB |= (1<<PB2);
		// Trigger INT2 with the falling edge
		MCUCR&=~(1<<ISC2);
		// Enable external interrupt pin INT2
		GICR |=(1<<INT2);
		


}


//reset watch
ISR(INT0_vect){
	sec=0;
	mins=0;
	hrs=0;

}
//stop (pause)
// External INT1 Interrupt Service Routine
ISR(INT1_vect){

	TCCR1B &= ~(1<<CS10)&~(1<<CS11)&~(1<<CS12);

}

//resume stop watch
// External INT2 Interrupt Service Routine
ISR(INT2_vect){

	TCCR1B = (1<<WGM12)|(1<<CS10)|(1<<CS11);

}

void Timer_init(void){
	/* Configure timer1 control registers
		 * 1. Non PWM mode FOC1A=1 and FOC1B=1
		 * 2. No need for OC1A & OC1B in this example so COM1A0=0 & COM1A1=0 & COM1B0=0 & COM1B1=0
		 * 3. CTC Mode and compare value in OCR1A WGM10=0 & WGM11=0 & WGM12=1 & WGM13=0*/
	    TCCR1A = (1<<FOC1A);  //enable compare interrupt for channel A
		TCCR1B = (1<<WGM12)|(1<<CS10)|(1<<CS11);
		TCNT1 = 0;  //timer initial value
		OCR1A = 15625;
		TIMSK|=(1<<OCIE1A);

}
// Interrupt Service Routine for timer1 compare mode channel A
ISR(TIMER1_COMPA_vect){
	count_second_flag = 1;
}

int main (void){
	DDRA = 0xff;
		PORTA = 0xff;
		 // configure first four pins of PORTC as output pins
		DDRC |= 0x0f;
		// Enable all the 7-Segments and initialize all of them with zero value
		PORTC &= 0xf0;
        SREG  |= (1<<7);
		init_INT0();
		init_INT1();
		init_INT2();
		// Start timer1 to generate compare interrupt every 1000 MiliSeconds(1 Second)
		Timer_init();


		while(1)
		{
  if (count_second_flag == 1){
	  //enter here every one second
	      		//increment seconds count
           sec++;
	if (sec==60){
		sec=0;
		mins++;
	}
	if(mins==60){
		sec=0;
		mins=0;
		hrs++;
	}
	if(hrs==24){
		sec=0;
		mins=0;
		hrs=0;

}            // reset the flag again
	  count_second_flag = 0;
  }

else {
	// out the number of seconds
			PORTA = (1<<5);
			PORTC = (PORTC & 0xF0) |sec % 10;
			_delay_ms(5);
			PORTA = (1<<4);
			PORTC = (PORTC & 0xF0) |sec / 10;
			_delay_ms(5);
			// out the number of minutes
			PORTA = (1<<3);
			PORTC = (PORTC & 0xF0) |mins % 10;
			_delay_ms(5);
			PORTA = (1<<2);
			PORTC = (PORTC & 0xF0) |mins / 10;
			_delay_ms(5);
			// out the number of hours
			PORTA = (1<<1);
			PORTC = (PORTC & 0xF0) |hrs % 10;
			_delay_ms(5);
			PORTA = (1<<0);
			PORTC = (PORTC & 0xF0) |hrs / 10;
			_delay_ms(5);


      }
		}




}



