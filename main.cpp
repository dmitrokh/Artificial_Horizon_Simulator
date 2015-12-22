#include "mbed.h"
#include "ST7735_TFT.h"
#include "Arial24x23i.h"
#include "Arial11x11.h"
#include "Arial9x9.h"
#include "MPU6050.h"
#include "MMA8451Q.h"

#define MMA8451_I2C_ADDRESS (0x1d<<1)

void compFilter();
void preparePeriferals();
void airplaneLine();
void rollPitchLines();

Serial pc(USBTX,USBRX);    // default baud rate: 9600
MPU6050 mpu6050;           // class: MPU6050, object: mpu6050 
PinName const SDA = PTE25;
PinName const SCL = PTE24;
MMA8451Q acc(SDA, SCL, MMA8451_I2C_ADDRESS); //on-board accelerometer
ST7735_TFT lcd(PTD6, NC, PTD5, PTA13, PTD2, PTD4, "TFT"); // TFT; sda, miso (not connected), sck, cs, AO(rs), reset
Ticker systick; 

float pitchAngle = 0;
float rollAngle = 0;
float rollX = 0;
float pitchY = 0;
float xC = lcd.height()/2;
float yC = lcd.width()/2;

int main()
{   
    void preparePeriferals();
                
    while (true) 
    {
        lcd.cls();
        //pc.printf("Accelerometer (onboard)   X = %1.2f, Y = %1.2f, Z = %1.2f\r\n", acc.getAccX(), acc.getAccY(), acc.getAccZ());
        //pc.printf("Accelerometer MPU6050(g)  X = %.3f,  Y = %.3f,  Z = %.3f\r\n", ax, ay, az);
        //pc.printf("Gyroscope MPU6050(deg/s) gx = %.3f, gy = %.3f,  gz = %.3f\r\n", gx, gy, gz);
        //wait(1.0f);
                
        airplaneLine();
        systick.attach(&compFilter, 0.005);    // calls the complementaryFilter func. every 5 ms (200 Hz sampling period)
        //pc.printf("Pitch: %.3f   \r\n", pitchAngle); //displays the pitch angle in terminal
        //pc.printf("Roll:  %.3f   \r\n", rollAngle); // displays the roll angle in terminal
        rollPitchLines();                                                     
    }    
}

void preparePeriferals()
{
    //getting MPU6050 ready   
    pc.baud(9600);                              // baud rate: 9600
    mpu6050.whoAmI();                           // Communication test: WHO_AM_I register reading 
    wait(1);
    mpu6050.calibrate(accelBias,gyroBias);      // Calibrate MPU6050 and load biases into bias registers
    pc.printf("Calibration is completed. \r\n");
    wait(0.5);
    mpu6050.init();                             // Initialize the sensor
    wait(1);
    pc.printf("MPU6050 is initialized for operation.. \r\n\r\n");
    wait_ms(500);
    
    //getting TFT ready
    lcd.cls();
    lcd.background(Cyan);
    lcd.set_orientation(0);
}

void airplaneLine()
{
   //line representing the aircraft, right half, three lines in order to make it thicker and more visible
   lcd.line(yC, xC - lcd.height()*5/16, yC, xC - lcd.height()/16, Black);
   lcd.line(yC + 1, xC - lcd.height()*5/16, yC + 1, xC - lcd.height()/16, Black);
   lcd.line(yC - 1, xC - lcd.height()*5/16, yC - 1, xC - lcd.height()/16, Black);
   lcd.line(yC, xC - lcd.height()/16, yC + 5, xC - lcd.height()/16, Black);
   //line representing the aircraft, left half
   lcd.line(yC, xC + lcd.height()/16, yC + 5, xC + lcd.height()/16, Black);
   lcd.line(yC, xC + lcd.height()/16, yC, xC + lcd.height()*5/16, Black);
   lcd.line(yC + 1, xC + lcd.height()/16, yC+1, xC + lcd.height()*5/16, Black);
   lcd.line(yC - 1, xC + lcd.height()/16, yC-1, xC + lcd.height()*5/16, Black);
   //line representing the aircraft, center dot
   lcd.fillcircle(yC, xC, 3, Black);   
}

void rollPitchLines()
{
   if (rollAngle > 45 || rollAngle < -45) {
            lcd.background(Red);
        }
        else {
            lcd.background(Cyan);
        }  
   rollX = tan(rollAngle * PI/180) * lcd.height()/2; //horizontal offset
   pitchY = tan(pitchAngle * PI/180) * 50; //vertical offset, 50 is just an arbitrary("magic") number which influences the speed the roll/pitch lines move up/down 
   
   //central line ("ground line") going from one edge of the screen to another, tied to the left and right side of the TFT relatively to the roll angle
   float x1 = xC - lcd.height(); //starting x-coordinate  
   float y1 = yC + pitchY; //starting y-coordinate  
   float x2 = xC + lcd.height(); //end x-coordinate 
   float y2 = yC + pitchY; //end y-coordinate  
   lcd.line((yC+(x1-xC)*sin(-rollAngle*PI/180)+(y1-yC)*cos(-rollAngle*PI/180)), (xC+(x1-xC)*cos(-rollAngle*PI/180)-(y1-yC)*sin(-rollAngle*PI/180)), (yC+(x2-xC)*sin(-rollAngle*PI/180)+(y2-yC)*cos(-rollAngle*PI/180)), (xC+(x2-xC)*cos(-rollAngle*PI/180)-(y2-yC)*sin(-rollAngle*PI/180)), Black);
      
   //The way to rotate by an arbitraty point is first substract the point coordinates, do the rotation about the origin and 
   //then add the point coordinates.
   //x2 = px + (x1-px)*cos(q)-(y1-py)*sin(q)
   //y2 = py + (x1-px)*sin(q)+(y1-py)*cos(q)
   //where px, py are the rotation point coordinates, and x1,y1 the original 2D shape vertex and 
   //x2,y2 the rotated coordinates, and q the angle in radians.
   
   //changing coordinates to draw short "roll" lines above "ground" line
   for (int i = 1; i <= 5; i++)
    {
        x1 = xC - lcd.height()*4/16; 
        y1 = yC - lcd.width()*i/8 + pitchY;  
        x2 = xC + lcd.height()*4/16; 
        y2 = yC - lcd.width()*i/8 + pitchY; 
        lcd.line((yC+(x1-xC)*sin(-rollAngle*PI/180)+(y1-yC)*cos(-rollAngle*PI/180)), (xC+(x1-xC)*cos(-rollAngle*PI/180)-(y1-yC)*sin(-rollAngle*PI/180)), (yC+(x2-xC)*sin(-rollAngle*PI/180)+(y2-yC)*cos(-rollAngle*PI/180)), (xC+(x2-xC)*cos(-rollAngle*PI/180)-(y2-yC)*sin(-rollAngle*PI/180)), Black);   
        x1 = xC - lcd.height()*4/16;  
        y1 = yC + lcd.width()*i/8 + pitchY;  
        x2 = xC + lcd.height()*4/16; 
        y2 = yC + lcd.width()*i/8 + pitchY;
        lcd.line((yC+(x1-xC)*sin(-rollAngle*PI/180)+(y1-yC)*cos(-rollAngle*PI/180)), (xC+(x1-xC)*cos(-rollAngle*PI/180)-(y1-yC)*sin(-rollAngle*PI/180)), (yC+(x2-xC)*sin(-rollAngle*PI/180)+(y2-yC)*cos(-rollAngle*PI/180)), (xC+(x2-xC)*cos(-rollAngle*PI/180)-(y2-yC)*sin(-rollAngle*PI/180)), Black);
    }   
}  

void compFilter() {mpu6050.complementaryFilter(&pitchAngle, &rollAngle);
}
