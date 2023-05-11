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

int val[6];
int val_min[6];
int val_max[6];

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
void readAllAnalogs(){
	for(int i=0; i<6; i++){
		glcdFillRect(32,  i*15+35, 25, 10, GLCD_BLACK);
		glcdFillRect(64,  i*15+35, 25, 10, GLCD_BLACK);
		glcdFillRect(96,  i*15+35, 25, 10, GLCD_BLACK);
		glcdFillRect(128, i*15+35, 25, 10, GLCD_BLACK);

		if(i==0) val[i] = analog(AN0);
		if(i==1) val[i] = analog(AN1);
		if(i==2) val[i] = analog(AN2);
		if(i==3) val[i] = analog(AN3);
		if(i==4) val[i] = analog(AN4);
		if(i==5) val[i] = analog(AN5);

		if(val[i] < val_min[i])  val_min[i] = val[i];
		if(val[i] > val_max[i])  val_max[i] = val[i];

		glcd(32,  i*15+35,  "%4d", val[i]);
		glcd(64,  i*15+35,  "%4d", val_min[i]);
		glcd(96,  i*15+35,  "%4d", val_max[i]);
		glcd(128,  i*15+35,  "%4d", (int)((val_max[i]-val_min[i]) / 2) + val_min[i]);
	}
}

void demo(){
	int angle;
	int speed;
	
	sw_OK();
	
	setTextSize(1);
	glcd(0,0, "RoboFi by N Academy");
	glcd(0,10,"ANALOG INPUT DASHBOARD");
	
	glcd(0,   20,  "AN");
	glcd(32,  20,  "PV");
	glcd(64,  20,  "MIN");
	glcd(96,  20,  "MAX");
	glcd(128, 20,  "AVG");

	glcd(0,   35,  "A0");
	glcd(0,   50,  "A1");
	glcd(0,   65,  "A2");
	glcd(0,   80,  "A3");
	glcd(0,   95,  "A4");
	glcd(0,   110,  "A5");
	glcdHLine(0, 30, ST7735_TFTHEIGHT);

	// Initial min
	val_min[0] = analog(AN0);
	val_min[1] = analog(AN1);
	val_min[2] = analog(AN2);
	val_min[3] = analog(AN3);
	val_min[4] = analog(AN4);
	val_min[5] = analog(AN5);

	// Initial max
	val_max[0] = analog(AN0);
	val_max[1] = analog(AN1);
	val_max[2] = analog(AN2);
	val_max[3] = analog(AN3);
	val_max[4] = analog(AN4);
	val_max[5] = analog(AN5);
	
	while(1){
		for(int i=1; i<=2; i++){

			servo(1, 0);
			servo(2, 0);
			servo(3, 0);

			speed = 50*i;
			readAllAnalogs();
			fd(speed);
			delay(1000);
			motor_stop(ALL);  delay(1000);

			servo(1, 30);
			servo(2, 30);
			servo(3, 30);

			readAllAnalogs();
			bk(speed);
			delay(1000);
			motor_stop(ALL);  delay(1000);

			servo(1, 60);
			servo(2, 60);
			servo(3, 60);

			readAllAnalogs();
			sll(speed);
			delay(1000);
			motor_stop(ALL);  delay(1000);

			servo(1, 90);
			servo(2, 90);
			servo(3, 90);

			readAllAnalogs();
			slr(speed);
			delay(1000);
			motor_stop(ALL);  delay(1000);

			servo(1, 120);
			servo(2, 120);
			servo(3, 120);

			readAllAnalogs();
			sl(speed);
			delay(1000);
			motor_stop(ALL);  delay(1000);

			servo(1, 150);
			servo(2, 150);
			servo(3, 150);

			readAllAnalogs();
			sr(speed);
			delay(1000);
			motor_stop(ALL);  delay(1000);

			servo(1, 180);
			servo(2, 180);
			servo(3, 180);
		}
	}
}



#endif
