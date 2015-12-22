In this project I attempted to create a backup avionics device ("Artificial horizon") based on a microcontroller (Freedom board FRDM-KL46Z), a Motion Trackig device that combines a 3-axis gyroscope, 3-axis accelerometer, and a Digital Motion Processor™ (InvenSense MPU-6050), and a simple 1.8-inches LCD monitor with SPI interface. 
The code is written in C. 
The raw data has been read from MPU-6050 through I2C interface, processed with complementary filter, and then displayed on an LCD through SPI interface. The GUI of the project imitates the behavior of a real "Artificial horizon" device with respect to the roll and pitch angles of an aircraft. 

Used libraries and brief description:
MMA8451Q - used for communication with the on-board accelerometer (on FRDM-KL46Z)
MPU6050, MPU6050RegDef - used for communication with the MPU-6050 board
ST7735, GraphicsDisplay, TextDisplay - used for comminication with the LCD

More information on libraries can be found on mbed.org. I'm very grateful to all people who contributed to the development of these libraries and shared them to the public.

