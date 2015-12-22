 //  ST7735 8 Bit SPI Library 
 
#ifndef MBED_ST7735_TFT_H
#define MBED_ST7735_TFT_H

#include "mbed.h"
#include "GraphicsDisplay.h"
#include "BurstSPI.h"

#define RGB(r,g,b)  (((r&0xF8)<<8)|((g&0xFC)<<3)|((b&0xF8)>>3)) //5 red | 6 green | 5 blue

/*define ST7735 Commands */

#define ST7735_NOP 0x0
#define ST7735_SWRESET 0x01
#define ST7735_RDDID 0x04
#define ST7735_RDDST 0x09

#define ST7735_SLPIN  0x10
#define ST7735_SLPOUT  0x11
#define ST7735_PTLON  0x12
#define ST7735_NORON  0x13

#define ST7735_INVOFF 0x20
#define ST7735_INVON 0x21
#define ST7735_DISPOFF 0x28
#define ST7735_DISPON 0x29
#define ST7735_CASET 0x2A
#define ST7735_RASET 0x2B
#define ST7735_RAMWR 0x2C
#define ST7735_RAMRD 0x2E

#define ST7735_COLMOD 0x3A
#define ST7735_MADCTL 0x36


#define ST7735_FRMCTR1 0xB1
#define ST7735_FRMCTR2 0xB2
#define ST7735_FRMCTR3 0xB3
#define ST7735_INVCTR 0xB4
#define ST7735_DISSET5 0xB6

#define ST7735_PWCTR1 0xC0
#define ST7735_PWCTR2 0xC1
#define ST7735_PWCTR3 0xC2
#define ST7735_PWCTR4 0xC3
#define ST7735_PWCTR5 0xC4
#define ST7735_VMCTR1 0xC5

#define ST7735_RDID1 0xDA
#define ST7735_RDID2 0xDB
#define ST7735_RDID3 0xDC
#define ST7735_RDID4 0xDD

#define ST7735_PWCTR6 0xFC

#define ST7735_GMCTRP1 0xE0
#define ST7735_GMCTRN1 0xE1

// some RGB color definitions         RED GREEN BLUE                          

#define Black           0x0000      //   0,   0,   0 
#define Navy            0x8000      //   0,   0, 128 
#define DarkGreen       0x0400      //   0, 128,   0 
#define DarkCyan        0x8400      //   0, 128, 128 
#define Maroon          0x0010      // 128,   0,   0 
#define Purple          0x8010      // 128,   0, 128 
#define Olive           0x0410      // 128, 128,   0 
#define LightGrey       0xC618      // 192, 192, 192 
#define DarkGrey        0x7BEF      // 128, 128, 128 
#define Red             0x001F      // 255,   0,   0 
#define Green           0x07E0      //   0, 255,   0 
#define Yellow          0x07FF      // 255, 255,   0 
#define Blue            0xF800      //   0,   0, 255 
#define Magenta         0xF81F      // 255,   0, 255 
#define Cyan            0xFFE0      //   0, 255, 255 
#define White           0xFFFF      // 255, 255, 255 
#define Orange          0x053F      // 255, 165,   0 
#define GreenYellow     0x2FF5      // 173, 255,  47 

    /*   Use this to Calculate and define any colour
    
         #define RGB565CONVERT(red, green, blue) (uint16_t)( (( blue   >> 3 ) << 11 ) | (( green >> 2 ) << 5  ) | ( red  >> 3 ))
         lcd.printf("%4X  ",RGB565CONVERT(red value 0-255,green value 0-255,blue value 0-255));
    */
    
class ST7735_TFT : public GraphicsDisplay {
 public:

  /** Create a ST7735_TFT object connected to SPI and three pins. ST7735 requires rs pin to toggle between data/command
   *
   * @param mosi(SDA),miso(NC),sclk(SCK) SPI
   * @param cs pin connected to CS of display (called SS for 'Slave Select' in ST7735 datasheet)
   * @param rs pin connected to RS (A0)of display (called D/CX in ST7735 datasheet)
   * @param reset pin connected to RESET of display
   *
   */ 
  ST7735_TFT(PinName mosi, PinName miso, PinName sclk, PinName cs, PinName rs, PinName reset,const char* name ="TFT");
    
  /** Get the width of the screen in pixel
   *
   * @param 
   * @returns width of screen in pixel
   *
   */    
  virtual int width();

  /** Get the height of the screen in pixel
   *
   * @returns height of screen in pixel 
   *
   */     
  virtual int height();
    
  /** Draw a pixel at x,y with color 
   *  
   * @param x horizontal position
   * @param y vertical position
   * @param color 16 bit pixel color
   */    
  virtual void pixel(int x, int y, int colour);
   
  /** Get colour of pixel at x,y 
   *  
   * @param x horizontal position
   * @param y vertical position
   */    
  
  int getpixel(unsigned int x, unsigned int y);
      
  /** draw a circle
   *
   * @param x0,y0 center
   * @param r radius
   * @param color 16 bit color                                                                 *
   *
   */
       
  void circle(int x, int y, int r, int colour); 
  
  /** draw a filled circle
   *
   * @param x0,y0 center
   * @param r radius
   * @param color 16 bit color                                                                 *
   *
   * use circle with different radius,
   * can miss some pixel
   */    
  void fillcircle(int x, int y, int r, int colour); 
  
  /** draw a 1 pixel line
   *
   * @param x0,y0 start point
   * @param x1,y1 stop point
   * @param color 16 bit color
   *
   */    
  void line(int x0, int y0, int x1, int y1, int colour);
    
  /** draw a rect
   *
   * @param x0,y0 top left corner
   * @param x1,y1 down right corner
   * @param color 16 bit color
   *                                                   *
   */    
  void rect(int x0, int y0, int x1, int y1, int colour);
    
  /** draw a filled rect
   *
   * @param x0,y0 top left corner
   * @param x1,y1 down right corner
   * @param color 16 bit color
   *
   */    
  void fillrect(int x0, int y0, int x1, int y1, int colour);
    
  /** setup cursor position
   *
   * @param x x-position (top left)
   * @param y y-position 
   */   
  virtual void locate(int x, int y);
    
  /** Fill the screen with _backgroun color
   *
   */   
  virtual void cls (void);   
  
     
  /** calculate the max number of char in a line
   *
   * @returns max columns
   * depends on actual font size
   *
   */    
  virtual int columns(void);
    
  /** calculate the max number of columns
   *
   * @returns max column
   * depends on actual font size
   *
   */   
  virtual int rows(void);
    
  /** put a char on the screen
   *
   * @param value char to print
   * @returns printed char
   *
   */
  virtual int _putc(int value);
    
  /** draw a character on given position out of the active font to the TFT
   *
   * @param x x-position of char (top left) 
   * @param y y-position
   * @param c char to print
   *
   */    
  virtual void character(int x, int y, int c);
    
  /** paint a bitmap on the TFT 
   *
   * @param x,y : upper left corner 
   * @param w width of bitmap
   * @param h high of bitmap
   * @param *bitmap pointer to the bitmap data
   *
   *   bitmap format: 16 bit R5 G6 B5
   * 
   *   use Gimp to create / load , save as BMP, option 16 bit R5 G6 B5            
   *   use winhex to load this file and mark data stating at offset 0x46 to end
   *   use edit -> copy block -> C Source to export C array
   *   paste this array into your program
   * 
   *   define the array as static const unsigned char to put it into flash memory
   *   cast the pointer to (unsigned char *) :
   *   tft.Bitmap(10,40,309,50,(unsigned char *)scala);
   */    
  void Bitmap(unsigned int x, unsigned int y, unsigned int w, unsigned int h,unsigned char *bitmap);
    
    
   /** paint a 16 bit BMP from local filesytem on the TFT (slow) 
   *
   * @param x,y : upper left corner 
   * @param *Name_BMP name of the BMP file
   * @returns 1 if bmp file was found and painted
   * @returns -1 if bmp file was found not found
   * @returns -2 if bmp file is not 16bit
   * @returns -3 if bmp file is to big for screen 
   * @returns -4 if buffer malloc go wrong
   *
   *   bitmap format: 16 bit R5 G6 B5
   * 
   *   use Gimp to create / load , save as BMP, option 16 bit R5 G6 B5
   *   copy to internal file system            
   * 
   */      
    
  int BMP_16(unsigned int x, unsigned int y, const char *Name_BMP);  
    
   /** Read an area from the LCD RAM to MCU RAM
   *
   * @param x,y : upper left corner 
   * @param w width of bitmap
   * @param h high of bitmap
   * @param *buffer pointer to the buffer
   */    

  void read_area(unsigned int x, unsigned int y, unsigned int w, unsigned int h,unsigned char *buffer);  
    
  /** select the font to use
   *
   * @param f pointer to font array 
   *                                                                              
   *   font array can created with GLCD Font Creator from http://www.mikroe.com
   *   you have to add 4 parameter at the beginning of the font array to use: 
   *   - the number of byte / char
   *   - the vertial size in pixel
   *   - the horizontal size in pixel
   *   - the number of byte per vertical line
   *   you also have to change the array to char[]
   *
   */  
  void set_font(unsigned char* f);
   
  /** Set the orientation of the screen
   *  x,y: 0,0 is always top left 
   *
   * @param o direction to use the screen (0-3) 90&#65533; Steps  
   *
   */  
  void set_orientation(unsigned int o);
    
  //SPI _spi;
  BurstSPI _spi;
  DigitalOut _cs;
  DigitalOut _rs;
  DigitalOut _reset;
  unsigned char* font;
   
//protected:

  /** draw a horizontal line
   *
   * @param x0 horizontal start
   * @param x1 horizontal stop
   * @param y vertical position
   * @param color 16 bit color                                               
   *
   */
  void hline(int x0, int x1, int y, int colour);
    
  /** draw a vertical line
   *
   * @param x horizontal position
   * @param y0 vertical start 
   * @param y1 vertical stop
   * @param color 16 bit color
   */
  void vline(int y0, int y1, int x, int colour);
    
  /** Set draw window region
   *
   * @param x horizontal position
   * @param y vertical position
   * @param w window width in pixel
   * @param h window height in pixels
   */
protected:   
  void window (unsigned int x, unsigned int y, unsigned int w, unsigned int h);
    
  /** Set draw window region to whole screen
   *
   */  
  void WindowMax (void);
    
  /** Init the ST7735 controller
   *
   */  
  
  void tft_reset();
    
   /** Write data to the LCD controller
   *
   * @param dat data written to LCD controller
   * 
   */   
  void wr_dat(int value);
    
  /** Write a command the LCD controller 
   *
   * @param cmd: command to be written   
   *
   */   
  void wr_cmd(int value);
    
   /** Start data sequence to the LCD controller
   * 
   */   
  void wr_dat_start();
    
  /** Stop of data writing to the LCD controller
   *   
   */  
  void wr_dat_stop();
    
  /** write data to the LCD controller
   *
   * @param data to be written 
   *                                           *
   */    
  void wr_dat_only(unsigned short dat);
    
  /** Read data from the LCD controller
   *
   * @returns data from LCD controller
   *  
   */    
  unsigned short rd_dat(void);
    
  /** Write a value to the to a LCD register
   *
   * @param reg register to be written
   * @param val data to be written
   */   
  void wr_reg (unsigned char reg, unsigned short val);
    
  /** Read a LCD register
   *
   * @param reg register to be read
   * @returns value of the register 
   */    
  unsigned short rd_reg (unsigned char reg);
        
  unsigned int orientation;
  unsigned int char_x;
  unsigned int char_y;    
};
#endif
