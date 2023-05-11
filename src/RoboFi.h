#ifndef RoboFi_h
#define RoboFi_h

#include <Adafruit_GFX.h> 
#include <XTronical_ST7735.h>
#include <SPI.h>
#include <Servo.h>

//.. LCD ............................
#define TFT_DC     		15 
#define TFT_RST   		-1
#define TFT_CS   		5 
#define GLCD_RED		0x001f
#define GLCD_GREEN		0x07e0
#define GLCD_BLUE		0xf800
#define GLCD_BLACK		0x0000
#define GLCD_WHITE		0xffff

// Need to check
#define GLCD_YELLOW		0xffe0
#define GLCD_CYAN		0x07ff
#define GLCD_MAGENTA	0xf81f

//.. ADC ............................
#define AN0				36								
#define AN1				39								
#define AN2				34							
#define AN3				35								
#define AN4				14								
#define AN5				2		

//.. MOTOR ..........................
#define M1_EN 			4
#define M1_DIR 			21
#define M2_EN 			22
#define M2_DIR 			25
#define M3_EN 			26
#define M3_DIR 			27
#define M4_EN 			32
#define M4_DIR 			33	
#define M1_PWM_CH		10					
#define M2_PWM_CH		11				
#define M3_PWM_CH		12					
#define M4_PWM_CH		13
#define FREQ 			5000
#define RESOLUTION		8
#define ALL				1234

//.. DIRECTION FOR MECANUM WHEEL.....


					
//.. SERVO ..........................
#define SERVO1			19
#define SERVO2			16
#define SERVO3			12
Servo _servo1, _servo2, _servo3;

//.. SERVO ..........................
#define SW				13

//.. SERVO ..........................
#define BUZZER     		17 


bool				_glcd_init	= false;
bool				_buzzer_init= false;
bool 				_motor_init	= false;
bool 				_servo_init	= false;
volatile uint8_t 	_textSize 	= 1;

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);  

void beep(){
	if(!_buzzer_init){
		pinMode(BUZZER, OUTPUT);
		_buzzer_init = true;
	}
	digitalWrite(BUZZER, HIGH);	delay(100);
	digitalWrite(BUZZER, LOW);	
}

void glcdConfig(){
	if(!_glcd_init){
		tft.init();   // initialize a ST7735S chip,
		tft.setRotation(3);
		tft.fillScreen(ST7735_BLACK);
		//tft.setTextColor(ST7735_WHITE);
		tft.setTextWrap(true);
		
		_glcd_init = true;
	}
}

void glcdClear(void)
{
	glcdConfig();
    tft.fillScreen(ST7735_BLACK);
}

void setTextColor(uint16_t color){
	glcdConfig();
	tft.setTextColor(color);
}

/*

	Text size 1 - height 15px
	Text size 2 - height 20px
*/
void setTextSize(uint newSize)
{
	glcdConfig();
	tft.setTextSize(newSize);
}

void glcdFillRect(int16_t x, int16_t y, int16_t w, int16_t h,uint16_t color){
	glcdConfig();
	tft.fillRect(x, y , w, h, color);
	
}

void glcd(uint8_t x, uint8_t y, const char* format, ...) {
	char str[100]; 
	
	va_list args;
	va_start(args, format);
	vsnprintf(str, sizeof(str), format, args);
	va_end(args);
	
	glcdConfig();
	tft.setCursor(x,y);
	tft.print(str);
}

void glcdVLine(int16_t x, int16_t y, int16_t h) {
	glcdConfig();
	tft.drawFastVLine(x, y, h, ST7735_WHITE);
}

void glcdHLine(int16_t x, int16_t y, int16_t w) {
	glcdConfig();
	tft.drawFastHLine(x, y, w, ST7735_WHITE);
}

unsigned int analog(char ch){
	analogRead(ch);
	return(analogRead(ch));         
}

void motorConfig(){
	if(!_motor_init){
		pinMode(M1_EN,	OUTPUT);
		pinMode(M1_DIR,	OUTPUT);
		pinMode(M2_EN,	OUTPUT);
		pinMode(M2_DIR,	OUTPUT);
		pinMode(M3_EN,	OUTPUT);
		pinMode(M3_DIR,	OUTPUT);
		pinMode(M4_EN,	OUTPUT);
		pinMode(M4_DIR,	OUTPUT);
		
		ledcSetup(M1_PWM_CH, FREQ, RESOLUTION);
		ledcAttachPin(M1_EN, M1_PWM_CH);
		
		ledcSetup(M2_PWM_CH, FREQ, RESOLUTION);
		ledcAttachPin(M2_EN, M2_PWM_CH);
		
		ledcSetup(M3_PWM_CH, FREQ, RESOLUTION);
		ledcAttachPin(M3_EN, M3_PWM_CH);
		
		ledcSetup(M4_PWM_CH, FREQ, RESOLUTION);
		ledcAttachPin(M4_EN, M4_PWM_CH);
		
		_motor_init = true;
	}
}

void motor(uint8_t ch, int8_t speed){
	signed int speedMotor;
	
	motorConfig();
	
	if(speed>=-100 && speed<=100){
		speedMotor = abs((speed*255)/100);
		
		if(ch == 1){
			if(speed>0){
			  digitalWrite(M1_DIR, LOW);
			}
			else{
			  digitalWrite(M1_DIR, HIGH);
			}
			ledcWrite(M1_PWM_CH, speedMotor);
		}
		
		if(ch == 2){
			if(speed>0){
			  digitalWrite(M2_DIR, LOW);
			}
			else{
			  digitalWrite(M2_DIR, HIGH);
			}
			ledcWrite(M2_PWM_CH, speedMotor);
		}
		
		if(ch == 3){
			if(speed>0){
			  digitalWrite(M3_DIR, LOW);
			}
			else{
			  digitalWrite(M3_DIR, HIGH);
			}
			ledcWrite(M3_PWM_CH, speedMotor);
		}
		
		if(ch == 4){
			if(speed>0){
			  digitalWrite(M4_DIR, LOW);
			}
			else{
			  digitalWrite(M4_DIR, HIGH);
			}
			ledcWrite(M4_PWM_CH, speedMotor);
		}
		
	}
}

void motor_stop(uint16_t ch){
    motorConfig();
    if(ch==1){  
	  motor(1, 0);
    }
    if(ch==2){  
	  motor(2, 0);
    }   
	if(ch==3){  
	  motor(3, 0);
    }   
	if(ch==4){  
	  motor(4, 0);
    }   
    if((ch==ALL)||(ch==1234))
    {
	  motor(1,0);
	  motor(2,0);
	  motor(3,0);
	  motor(4,0);
    }
	
	beep();
}

void servoConfig(){
	if(!_servo_init){
		pinMode(SERVO1,	OUTPUT);
		pinMode(SERVO2,	OUTPUT);
		pinMode(SERVO3,	OUTPUT);
		
		ledcSetup(5, 50, 8);
		ledcAttachPin(SERVO1, 5);
		
		_servo_init = true;
	}
}

void servo(uint8_t servo, uint8_t angle){
	// servoConfig();
	if(servo == ALL){
		if(angle == -1){
			_servo1.detach();
			_servo2.detach();
			_servo3.detach();
		}
		else{
			if(!_servo1.attached()){
				_servo1.attach(SERVO1);
			}
			_servo1.write(angle);
			
			if(!_servo2.attached()){
				_servo2.attach(SERVO2);
			}
			_servo2.write(angle);
			
			if(!_servo3.attached()){
				_servo3.attach(SERVO3);
			}
			_servo3.write(angle);
		}
	}
	
	
	if(servo == 1){
		if(angle==-1){
			_servo1.detach();
		}
		else{
			if(!_servo1.attached()){
				_servo1.attach(SERVO1);
			}
			_servo1.write(angle);
		}
	}
	
	if(servo == 2){
		if(angle==-2){
			_servo2.detach();
		}
		else{
			if(!_servo2.attached()){
				_servo2.attach(SERVO2);
			}
			_servo2.write(angle);
		}
	}
	
	if(servo == 3){
		if(angle==-3){
			_servo3.detach();
		}
		else{
			if(!_servo3.attached()){
				_servo3.attach(SERVO3);
			}
			_servo3.write(angle);
		}
	}
}



bool sw_OK_press(){
	if(digitalRead(SW)){
		return true;
	}
	else{
		return false;
	}
}


void moveMotors(int speedRF, int speedLF, int speedRR, int speedLR) {
	motor(1, speedLF);
	motor(2, speedLR);
	motor(3, speedRF);
	motor(4, speedRR);
}

void sw_OK(){
	glcdClear();
	setTextSize(2);
	glcd(0,0,"Press OK...");
	while(sw_OK_press()){
		delay(100);
	}
	beep();
	glcdClear();
}

void fd(uint8_t speed){
	motor(1, speed);
	motor(2, speed);
	motor(3, speed);
	motor(4, speed);
	
}

void bk(uint8_t speed){
	motor(1, -speed);
	motor(2, -speed);
	motor(3, -speed);
	motor(4, -speed);
	
}

void sl(uint8_t speed){
	motor(1, -speed);
	motor(2, -speed);
	motor(3, speed);
	motor(4, speed);
	
}
void sll(uint8_t speed){
	motor(1, -speed);
	motor(2, speed);
	motor(3, -speed);
	motor(4, speed);
	
}

void sr(uint8_t speed){
	motor(1, speed);
	motor(2, speed);
	motor(3, -speed);
	motor(4, -speed);
	
}
void slr(uint8_t speed){
	motor(1, speed);
	motor(2, -speed);
	motor(3, speed);
	motor(4, -speed);
	
}

void Demo(){
	
	
	//glcd(0, 10, "Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.");
}



#endif
