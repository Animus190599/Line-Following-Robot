#define F_CPU 16000000UL

#include <avr/io.h>
// #include "Motors.h"
// #include "Sensors.h"
#include <avr/delay.h>
#include <avr/interrupt.h>
#include <math.h>

#define THRESHOLD 100
#define FULL_SPEED 50
#define HALF_SPEED FULL_SPEED/2
#define STOP 0
#define SLOW_ZONE_FACTOR 0.6
#define NORMAL_SPEED 50

// This function Clears all ADC registers
void clear_ADC() {
	ADCSRB = 0b00000000;
	ADMUX = 0b00000000;
	ADCSRA = 0b00000000;	
}

// define a function to read a value from a specified adc input
uint8_t read_sensor(int id){
	
		if (id == 1){
			
			//Clear ADC 
			clear_ADC();
			//reset ADC
			ADC_Setup();
			
			
		// adc4 - ensure 000100
			ADMUX |= (1<<MUX2);		//set up mux for adc4 - S1

		}
		
		else if (id ==2){
			
			//Clear ADC 
			clear_ADC();
			//reset ADC
			ADC_Setup();
			
			
		// adc5 - 000101
			ADMUX |= (1<<MUX2);		//set up mux for adc5 - S2
			ADMUX |= (1<<MUX0);
		}
		
		else if (id ==3){
			
			//Clear ADC 
			clear_ADC();
			//reset ADC
			ADC_Setup();
			
		// adc6 - 000110
			ADMUX |= (1<<MUX2);		//set up mux for adc6 - S3
			ADMUX |= (1<<MUX1);

		}
		else if (id ==4){
			
			//Clear ADC 
			clear_ADC();
			//reset ADC
			ADC_Setup();
			
		// adc7 - 000111
			ADMUX |= (1<<MUX2);		//set up mux for adc7 - S4
			ADMUX |= (1<<MUX1);
			ADMUX |= (1<<MUX0);
			
		}
		
		
		else if (id ==5){
			
			//Clear ADC 
			clear_ADC();
			//reset ADC
			ADC_Setup();
			
		// adc11 - 100011
			ADCSRB |= (1<<MUX5);		//set up mux for adc11 - S5
			ADMUX |= (1<<MUX1);
			ADMUX |= (1<<MUX0);
		}
		else if (id ==6){
			
			//Clear ADC 
			clear_ADC() ;
			//reset ADC
			ADC_Setup();
			
		// adc10 - 100010
			ADCSRB |= (1<<MUX5);		//set up mux for adc10 - S6
			ADMUX |= (1<<MUX1);

		}
		else if (id ==7){
			
			//Clear ADC 
			clear_ADC();
			//reset ADC
			ADC_Setup();
			
		// adc9 - 100001
			ADCSRB |= (1<<MUX5);		//set up mux for adc9 - S7
			ADMUX |= (1<<MUX0);
		}
		else if (id ==8){
			
			//Clear ADC 
			clear_ADC();
			//reset ADC
			ADC_Setup();
			
		// adc8 - 100000
			ADCSRB |= (1<<MUX5);		//set up mux for adc8 - S8

		}

		// ADD SIDE MARKERS
		else if (id ==9){
			
			//Clear ADC 
			clear_ADC() ;
			//reset ADC
			ADC_Setup();
			
		// adc1 - 000001
			ADMUX |= (1<<MUX0); //set up mux for adc1 - Right marker (Black N white)
		}

		else if (id ==10){
			
			//Clear ADC 
			clear_ADC() ;
			//reset ADC
			ADC_Setup();
			
		// adc0 - 000000		//set up mux for adc0 - Left marker (Color)
		}
		
    // start conversion
    ADCSRA |= (1<<ADSC);

    // wait until conversion is complete
    while(~ADCSRA&(1<<ADIF)){}

    // return the value from the adc
    return ADCH;

}


// define a function to set up buttons
int Button_Setup() {
	
	DDRC &= ~(1<<7);
	DDRC &= ~(1<<6);
	
	PORTC |= (1<<7);
	PORTC |= (1<<6);
}


// define a function to set up Leds
int LED_Setup(){
	
	// Set up LEDS
	DDRE |= (1<<6); //led0
    DDRB |= (1<<0); //led1
    DDRB |= (1<<1); //led2
	
	// Color Marker Sensors LEDS
	DDRB |= (1<<2); //Left Red   ------ USE
    DDRB |= (1<<3); //Left Green ------ USE
    // DDRD |= (1<<0); //Right Red ------ NOT USING
	// DDRB |= (1<<6); //Right Green ------ NOT USING
	
	
	// Turn off all LEDS
	PORTE &= ~(1<<6); //led0
    PORTB &= ~(1<<0); //led1
    PORTB &= ~(1<<1); //led2

		
	// Turn off Color Marker Sensors LEDS
	PORTB &= ~(1<<2); //Left Red   ------ USE
    PORTB &= ~(1<<3); //Left Green ------ USE
    // PORTD &= ~(1<<0); //Right Red ------ NOT USING
	// PORTB &= ~(1<<6); //Right Green ------ NOT USING
	
	_delay_ms(500);
	
	PORTE ^= (1<<6); //led0
	PORTB ^= (1<<1); //led2
	
	_delay_ms(500);
	
	PORTE ^= (1<<6); //led0
	PORTB ^= (1<<1); //led2
	
}

// define a function to set ADC's
void ADC_Setup(){
	
	// set up ADC
   // use internal 2.56V ref, left-adjust result
    ADMUX |= (1<<REFS0)|(1<<REFS1)|(1<<ADLAR); //|(1<<MUX2)
	
	
	ADCSRA |= (1<<ADEN)|(1<<ADATE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);

}

// define a function to set ADC's
int LED_Shutdown(){
	
	PORTE &= ~(1<<6); //led0
	PORTB &= ~(1<<0); //led1
	PORTB &= ~(1<<1); //led2

}

int LED_LightUp() {
	
	PORTE |= (1<<6); //led0
	PORTB |= (1<<0); //led1
	PORTB |= (1<<1); //led2
	
}

// define a function to turn all Color LEDS OFF
int Color_LED_Shutdown(){
	
	PORTB &= ~(1<<2); //Left Red   ------ USE
    PORTB &= ~(1<<3); //Left Green ------ USE
    // PORTD &= ~(1<<0); //Right Red ------ NOT USING
	// PORTB &= ~(1<<6); //Right Green ------ NOT USING
}

// define a function to turn all Color LEDS ON
int Color_LED_LightUp() {
	
	PORTB |= (1<<2); //Left Red   ------ USE
    PORTB |= (1<<3); //Left Green ------ USE
    // PORTD |= (1<<0); //Right Red ------ NOT USING
	// PORTB |= (1<<6); //Right Green ------ NOT USING
	
}

int LED_On(int LEDNumber){
	
	if (LEDNumber == 0) {
		PORTE |= (1<<6); //led0
	}
	
	if (LEDNumber == 1) {
		PORTB |= (1<<0); //led1
	}
	
	if (LEDNumber == 2) {
		PORTB |= (1<<1); //led2
	}
	
	// Turn ON Color Marker Sensor's LEDS
	if (LEDNumber == 3) {
		PORTB |= (1<<2); //Left Red   ------ USE
	}

	if (LEDNumber == 4) {
		PORTB |= (1<<3); //Left Green ------ USE
	}

	// if (LEDNumber == 5) {
		// PORTD |= (1<<0); //Right Red ------ NOT USING
	// }

	// if (LEDNumber == 6) {
		// PORTB |= (1<<6); //Right Green ------ NOT USING
	// }
}

int LED_Toggle(int LEDNumber){
	
	if (LEDNumber == 0) {
		PORTE ^= (1<<6); //led0
	}
	
	if (LEDNumber == 1) {
		PORTB ^= (1<<0); //led1
	}
	
	if (LEDNumber == 2) {
		PORTB ^= (1<<1); //led2
	}
	
}

int LED_Off(int LEDNumber){
	
	if (LEDNumber == 0) {
		PORTE &= ~(1<<6); //led0
	}
	
	if (LEDNumber == 1) {
		PORTB &= ~(1<<0); //led1
	}
	
	if (LEDNumber == 2) {
		PORTB &= ~(1<<1); //led2
	}
	
	// Turn ON Color Marker Sensor's LEDS
	if (LEDNumber == 3) {
		PORTB &= ~(1<<2); //Left Red   ------ USE
	}

	if (LEDNumber == 4) {
		PORTB &= ~(1<<3); //Left Green ------ USE
	}

	// if (LEDNumber == 5) {
		// PORTD &= ~(1<<0); //Right Red ------ NOT USING
	// }

	// if (LEDNumber == 6) {
		// PORTB &= ~(1<<6); //Right Green ------ NOT USING
	// }
}

// ISR(TIMER0_OVF_vect){ //interrupt called when timer0 overflows
	
	// if(MS_L != old_MS_L){
		// OCR0A = (MS_L*max_speed*255);
		// OCR0B = 0;
		// old_MS_L = MS_L;
	// }
	
// }

// ISR(TIMER1_OVF_vect){ //interrupt called when timer1 overflows
	
	// if(MS_R != old_MS_R){
		// OCR1A = (MS_R*max_speed*255);
		// OCR1B = 0;
		// old_MS_R = MS_R;
	// }
	
// }

void timer0_Init(){

	//set up timer0 to output fast PWM, 256 prescaler, clear on compare match, set on top
	DDRB |= (1<<7); //Configure OC0A/B7 an ouput
	TCCR0A |= (1<<COM0A1)|(1<<WGM01)|1; //clear OC0A on compare match, Set OC0A at TOP, clear OC0B on compare match, Set OC0B at TOP, Fast PWM
	// TIMSK0 |= (1<<0); //enable overflow interrupt
	OCR0A = 0;
	TCCR0B |= (1<<2); // 256 Prescaler
}

void timer1_Init(){

//set up timer1 to output fast PWM (8-bit), 256 prescaler, clear on compare match, set on top
	
	DDRB |= (1<<5);
	TCCR1A |= (1<<7)|(1<<5)|1;
	// TCCR1B |= (1<<3);
	// TIMSK1 |= (1<<0); ////enable overflow interrupt
	OCR1A = 0;
	TCCR1B |= (1<<2); // 256 Prescaler
}

void SetMotorSpeeds(uint8_t MotorSpeeds[2]){

	OCR0A = MotorSpeeds[0]; //M2
	OCR1A = MotorSpeeds[1]; // M1

	return;
}




int main()
{
	//Setup timers
	timer0_Init();
	timer1_Init();
	
	//Setup Buttons
	Button_Setup();
	
	//Setup LEDS
	LED_Setup();
		
	// Setup Color sensor
	int balanceSet = 0;
	int whiteArrayRed = 0;
	int whiteArrayGreen = 0;
	int blackArrayRed = 0;
	int blackArrayGreen = 0;
	int colorArrayRed = 0;
	int colorArrayGreen = 0;

	//****SET UP VARIABLES***
	uint8_t motorSpeeds[2] = {0,0};
	int NumCrossroads = 4; //include all corssroads that it will pass over and how many times it will (all directions)
	// E.g. 1 cross road that you pass over twice from 2 directions, is counted as 2. (number of crossroads * 2)
	
	// Main variables
	int On = 0;
	int RightCounter = 0;
	int LeftCounter = 0;
	int GreenCounter = 0;
	int RedCounter = 0;
	int RightOn = 0;
	int x = 0;
	int iteration = 0;
	int Cross_iteration = 0;
	int SetMotor_White = 0;
	int FirstCross = 0;

	// declare ADC value variable array (size 2 as we are reading from 2 different sensors)
    uint8_t sensor_out[9];
	
	/*
	// ------------------------- CALIBRATE COLOR SENSOR ------------------------//
	if(balanceSet == 0){
	 
		_delay_ms(3000);
	
		// Find white Values (Top range)
			int i;
			for(i = 0;i<2;i++){
		
				LED_On(i+3);
				_delay_ms(100);
					
				int j;
				int reading =0;
				int tally = 0;
				int avgReadwhite = 0;
					
				for(j = 0;j < 5;j++){
					reading = read_sensor(10);
					tally = reading + tally;
					_delay_ms(10); //10
				}
	
				avgReadwhite = (tally)/5;
				
				// First iteration - RED
				if (i == 0) {
					whiteArrayRed = avgReadwhite;

				}
				
				// Second iteration - GREEN
				else {
					whiteArrayGreen= avgReadwhite;

				}
					
				LED_Off(i+3);
				_delay_ms(100);
			}
				
				
		_delay_ms(5000);
				
		// Find Black Values (Bottom range)
		int k;
		for(k = 0;k<2;k++){
	
			LED_On(k+3);
			_delay_ms(100);
		
			int jj;
			int reading1 =0;
			int tally1 = 0;
			int avgReadblack = 0;
			
			for(jj = 0;jj < 5;jj++){
				reading1 = read_sensor(10);
				tally1 = reading1 + tally1;
				_delay_ms(10); //10
			}
			
			avgReadblack = (tally1)/5;
			
			// First iteration - RED
			if (k == 0) {
				blackArrayRed = avgReadblack;
			}
			
			// Second iteration - GREEN
			else {
				blackArrayGreen = avgReadblack;

			}
			
			LED_Off(k+3);
			_delay_ms(100);
		}
				
		balanceSet = 1;
		_delay_ms(3000);
	}
	
	_delay_ms(2000);
	
	*/
	while(1) {
		
		// Button 1 (SW1) starts the code
		if(PINC&(1<<7) && On == 0){
			LED_LightUp();
			
			_delay_ms(500);
			
			LED_Shutdown();
			
			// _delay_ms(500);
			
			On = 1;
			// PORTC |= (1<<7);
		}
			
			while (On == 1) {
				
				
				//count iterations
				x++;
				


				// read a value from each ADC and assign to array using function
				sensor_out[0] = read_sensor(1); //adc4 - S1
				sensor_out[1] = read_sensor(2); //adc5 - S2
				sensor_out[2] = read_sensor(3); //adc6 - S3
				sensor_out[3] = read_sensor(4); //adc7 - S4
				sensor_out[4] = read_sensor(5); //adc11 -S5
				sensor_out[5] = read_sensor(6); //adc10 -S6
				sensor_out[6] = read_sensor(7); //adc9 - S7
				sensor_out[7] = read_sensor(8); //adc8 - S8
				sensor_out[8] = read_sensor(9); //adc0 - RIGHT MARKER [Black N white]
		
			
			// ********************* TESTING SENSOR LIGHTS *******************************
			
			// check value of adc, if less than 100 turn on
			// if ( sensor_out[6]<THRESHOLD) { // || sensor_out[1]<THRESHOLD ) { // ***** S8 *** 
				// LED_On(0);
			// }
			// else {
				// LED_Off(0);
			// }
			
			// if (sensor_out[7]<THRESHOLD) { //|| sensor_out[7]<THRESHOLD) { // ***** S1 ***
				// LED_On(1);
			// } 
			// else {
				// LED_Off(1);
			// }
			
			// if (sensor_out[8]<THRESHOLD) { // || sensor_out[6]<THRESHOLD) { // ***** S1 ***
				// LED_On(2);
			// } 
			// else {
				// LED_Off(2);
			// }
			
			// if (sensor_out[4]<THRESHOLD || sensor_out[5]<THRESHOLD) { // ***** S1 ***
				// LED_On(3);
			// } 
			// else {
				// LED_Off(3);
			// }
			
			// ********************* END OF TESTING SENSOR LIGHTS *******************************
			
			// if (read_sensor(9) < 300) {
				// LED_On(3);
			// } else {
				// LED_Off(3);
			// }
			
			// if (read_sensor(9) > 300) {
				// LED_On(4);
			// } else {
				// LED_Off(4);
			// }
			
			
			// if ((sensor_out[9] > THRESHOLD) && (PORTB |= (1<<2)) { 
				// LED_On(
			
			// }
			
			
			// ------------------------- Right Marker - Black N WHite ------------------------//
			
						
			if (sensor_out[8] < THRESHOLD && RightOn == 1 && (x > (iteration + 50))) {
				if (FirstCross = 1) {
					FirstCross = 0;
					Cross_iteration = x;
					NumCrossroads--;
				}
					
				else if (NumCrossroads >= 1 && (x > Cross_iteration + 20)) {
					Cross_iteration = x;
					NumCrossroads--;
				}
				// if (sensor_out[1]<THRESHOLD && sensor_out[2]<THRESHOLD && sensor_out[3]<THRESHOLD && sensor_out[4]<THRESHOLD && sensor_out[5]<THRESHOLD && sensor_out[6]<THRESHOLD && sensor_out[8]<THRESHOLD) {
				// if (sensor_out[0]<THRESHOLD){
					// motorSpeeds[0] = 50;
					// motorSpeeds[1] = 50; 
				// }
				else {		
					RightOn = 0;
					Color_LED_LightUp();
					RightOn = 0;
					LED_LightUp();
					Color_LED_LightUp();
					motorSpeeds[0] = 0; // 50
					motorSpeeds[1] = 0; // 50
					SetMotorSpeeds(motorSpeeds);
					On = 0;
					break;
				}
				
				// SetMotor_White = 1;
				// motorSpeeds[0] = 30; // 50
				// motorSpeeds[1] = 30; // 50
				// SetMotorSpeeds(motorSpeeds);

			}
			
			else if (sensor_out[8] < THRESHOLD) {
				RightOn = 1;
				iteration = x;
				LED_On(1);
				//FirstCross = 1;
				// SetMotor_White = 0;
				// motorSpeeds[0] = 30; // 50
				// motorSpeeds[1] = 30; // 50
				// SetMotorSpeeds(motorSpeeds);
			} 
			
			else if (sensor_out[8] > THRESHOLD) {
				LED_Off(1);
			}
			
			// if (sensor_out[8] < THRESHOLD && RightOn == 1 && x > (iteration + 150)) {
				// RightOn = 0;
				// LED_LightUp();
				// Color_LED_LightUp();
				// motorSpeeds[0] = 0; // 50
				// motorSpeeds[1] = 0; // 50
				// SetMotorSpeeds(motorSpeeds);
				// On = 0;
				// break;
			// }
			
			
			/*
			// ------------------------- Left Hand side marker - COLOR MARKER ------------------------//
			
				int p;
			for(p = 0;p<2;p++){
				LED_On(p+3);
	
				_delay_ms(5); // Make smaller to speed up - like 10
				
				// sensor_out[8] = read_sensor(9); //adc0 - LEFT MARKER [COLOR]
				
				// Find average reading
				// AverageReturn(read_sensor(9));
				
					int col;
					int reading_col = 0;
					int tally_col = 0;
					int avgReadcolor_col = 0;
					
					for(col = 0;col < 5;col++){
						reading_col = read_sensor(10);
						tally_col = reading_col + tally_col;
						_delay_ms(5); //10
					}
					
					avgReadcolor_col = (tally_col)/5;
				
				
				// First iteration - RED
				if (p == 0) {
					
					int colorArray = 0;
	
					colorArray = avgReadcolor_col;	
					
					float greyDiffRed = whiteArrayRed - blackArrayRed;					  
					colorArray = (colorArray - blackArrayRed);				  
					float colorArrayDiv = colorArray/greyDiffRed;			  
					colorArray = colorArrayDiv*255;
					
					colorArrayRed = colorArray;
				}
				
				// Second iteration - GREEN
				else {
					
					int colorArray2 = 0;

	
					colorArray2 = avgReadcolor_col;						  
				    float greyDiffGreen = whiteArrayGreen - blackArrayGreen;					  
					colorArray2 = (colorArray2 - blackArrayGreen);
					float colorArrayDiv2 = colorArray2/greyDiffGreen;				  
					colorArray2 = colorArrayDiv2*255;

					
					colorArrayGreen = colorArray2;
				}		
				
				
				LED_Off(p+3);
				_delay_ms(5); // Make smaller to speed up - like 10
			}
			
			
			// ------------------------- React to color detection ------------------------//
			
			// char String5[]="COLOR RED VALUES\n"; 
				  // sprintf(buff, "%d", colorArrayRed);
				  // USART_putstring(String5); 
				  // USART_putstring(buff);
				  // USART_putstring(space);
				  // int colorArrayGreen1 = (int) colorArrayGreen;
				  // char String6[]="COLOR GREEN VALUES\n"; 
				  // sprintf(buff, "%d", colorArrayGreen1);
				  // USART_putstring(String6); 
				  // USART_putstring(buff);
				  // USART_putstring(space);
				  
				  // _delay_ms(200);
			
			
			// If it detects white - count up 
			if (colorArrayRed > 200 && colorArrayGreen > 200 ) {
				LED_On(1);				
				LED_Off(0);
				LED_Off(2);				
				
			} 
			//Detects Black - Turn all off
			else if (colorArrayRed < 80) {
				LED_Off(1);
				LED_Off(0);
				LED_Off(2);
			}
			// Detects Green
			else if (colorArrayGreen > 200 && colorArrayRed < 200) {
				LED_On(0);
				LED_Off(2);
				LED_Off(1);
			}
			// Detects Red 
			// else if (colorArrayRed > 240 && colorArrayGreen > 240 ) {
				// LED_On(2);
				// LED_Off(0);
				// LED_Off(1);
			// }
			/*
			
			// If it detects Green - Slow down and count 
			else if (colorArrayGreen > colorArrayRed) {
				 LED_On(0);
				 LED_Off(2);
				// motorSpeeds[0] = NORMAL_SPEED * SLOW_ZONE_FACTOR; // 50
				// motorSpeeds[1] = NORMAL_SPEED * SLOW_ZONE_FACTOR; // 50
				// GreenCounter++;
			} 
			
			// If it detects Red - Speed up to normal speed and count 			
			else if (colorArrayGreen < colorArrayRed) {
				LED_On(2);
				LED_Off(0);
				// motorSpeeds[0] = NORMAL_SPEED; // 50
				// motorSpeeds[1] = NORMAL_SPEED; // 50
				// RedCounter++;
			}
			
			*/
			// Going Straight
			if (sensor_out[3]<THRESHOLD && sensor_out[4]<THRESHOLD && sensor_out[2]>THRESHOLD && sensor_out[5]>THRESHOLD ) {
			
				if (SetMotor_White == 0) {
					motorSpeeds[0] = 50; // 50
					motorSpeeds[1] = 50; // 50
					LED_On(2);
					LED_On(0);
				}
				else if (SetMotor_White == 1) {
					motorSpeeds[0] = 20; // 50
					motorSpeeds[1] = 20; // 50
					LED_On(2);
					LED_On(0);
				}
			// LED_On(1);
			// LED_Off(2);
			// LED_Off(0);
			}
			// else {
			// LED_Off(2);
			// LED_Off(0);
			// }
		
			
			
			// ------------------------- Dealing with Cross roads ------------------------
			else if (sensor_out[1]<THRESHOLD && sensor_out[2]<THRESHOLD && sensor_out[3]<THRESHOLD && sensor_out[4]<THRESHOLD && sensor_out[5]<THRESHOLD && sensor_out[6]<THRESHOLD && sensor_out[7]<THRESHOLD && sensor_out[8]<THRESHOLD) {
			motorSpeeds[0] = 50;
			motorSpeeds[1] = 50;
			LED_On(1);
			LED_Off(2);
			LED_Off(0);
			}
			

			
			// ------------------------- Turning Right -----------------------------
			else if (sensor_out[4]<THRESHOLD && sensor_out[3]<THRESHOLD && sensor_out[5]<THRESHOLD)  {
					
						motorSpeeds[0] = 40; // 40
						motorSpeeds[1] = 25; // 25
						// LED_On(2);
						// LED_Off(1);
						// LED_Off(0);
					
			}
				
			else if (sensor_out[4]<THRESHOLD && sensor_out[3]<THRESHOLD && sensor_out[5]>THRESHOLD)  {
				
					motorSpeeds[0] = 50; //50
					motorSpeeds[1] = 20; // 20
					// LED_Off(1);
					// LED_On(2);
					// LED_Off(0);
				
			}
				
			else if (sensor_out[4]>THRESHOLD && sensor_out[3]<THRESHOLD && sensor_out[5]>THRESHOLD && sensor_out[2]>THRESHOLD)  {
				
					motorSpeeds[0] = 50; // 50
					motorSpeeds[1] = 10; // 10
					// LED_On(2);
					// LED_On(1);
					// LED_Off(0);
				
			}	
			else if (sensor_out[3]<THRESHOLD && sensor_out[2]<THRESHOLD)  {
				
					motorSpeeds[0] = 80; // 80
					motorSpeeds[1] = 0; // 0 
					// LED_On(2);
					// LED_On(1);
					// LED_Off(0);
				
			}	
			// else if (sensor_out[3]>THRESHOLD && sensor_out[2]<THRESHOLD)  {
				
					// motorSpeeds[0] = 180;
					// motorSpeeds[1] = 5;
					// LED_On(0);
					// LED_Off(2);
					// LED_Off(1);
					// LED_Off(3);
				
			// }	
			
			// else if (sensor_out[2]>THRESHOLD && sensor_out[1]<THRESHOLD)  {
				
					// motorSpeeds[0] = 200;
					// motorSpeeds[1] = 0;
					// LED_On(1);
					// LED_Off(2);
					// LED_Off(0);
					// LED_Off(3);
				
			// }			
			
			
			// --------------------------------- Turning Left ---------------------------
			else if(sensor_out[4]<THRESHOLD && sensor_out[6]<THRESHOLD && sensor_out[5]<THRESHOLD){
				
					motorSpeeds[0] = 25;
					motorSpeeds[1] = 40;
					// LED_On(0);
					// LED_Off(2);
					// LED_Off(1);
				}
			else if (sensor_out[6]<THRESHOLD && sensor_out[5]<THRESHOLD && sensor_out[4]>THRESHOLD)  {
				
					motorSpeeds[0] = 20;
					motorSpeeds[1] = 50;
					// LED_Off(2);
					// LED_On(0);
					// LED_Off(1);
				
			}
			
			else if (sensor_out[5]>THRESHOLD && sensor_out[6]<THRESHOLD && sensor_out[4]>THRESHOLD && sensor_out[7]>THRESHOLD)  {
				
					motorSpeeds[0] = 10;
					motorSpeeds[1] = 50;
					// LED_On(0);
					// LED_On(1);
					// LED_Off(2);
				
			}		
			
			else if (sensor_out[6]<THRESHOLD && sensor_out[7]<THRESHOLD)  {
			
				motorSpeeds[0] = 0;
				motorSpeeds[1] = 80;
				// LED_On(0);
				// LED_Off(2);
				// LED_On(1);
			
			}	
			
			// else if (sensor_out[6]>THRESHOLD && sensor_out[7]<THRESHOLD)  {
			
				// motorSpeeds[0] = 5;
				// motorSpeeds[1] = 180;
				// LED_On(3);
				// LED_Off(2);
				// LED_Off(1);
				// LED_Off(0);
				
			// }	
			
			// else if (sensor_out[7]>THRESHOLD && sensor_out[0]<THRESHOLD)  {
				
					// motorSpeeds[0] = 0;
					// motorSpeeds[1] = 200;
					// LED_On(2);
					// LED_Off(1);
					// LED_Off(0);
					// LED_Off(3);
				
			// }			
			
			// if the white stripe ends, so does Ruby's journey 
			else if (sensor_out[3]>THRESHOLD && sensor_out[4]>THRESHOLD && sensor_out[5]>THRESHOLD && sensor_out[6]>THRESHOLD && sensor_out[7]>THRESHOLD && sensor_out[2]>THRESHOLD) {
			motorSpeeds[0] = STOP;
			motorSpeeds[1] = STOP;
			// LED_LightUp();
			}
			
			// set the motor speeds	
			SetMotorSpeeds(motorSpeeds);
			
		
			if(PINC&(1<<6) && On == 1){
				LED_Shutdown();
				Color_LED_Shutdown();
				motorSpeeds[0] = STOP;
				motorSpeeds[1] = STOP;
				SetMotorSpeeds(motorSpeeds);
				On = 0;
				break;
				PORTC |= (1<<6);
			}
		
		}
    }
}

