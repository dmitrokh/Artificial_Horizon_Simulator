 //  ST7735 8 Bit SPI Library


#include "ST7735_TFT.h"
#include "mbed.h"

#define BPP         16                  // Bits per pixel                

ST7735_TFT::ST7735_TFT(PinName mosi, PinName miso, PinName sclk, PinName cs, PinName rs, PinName reset, const char *name)
        : GraphicsDisplay(name),_spi(mosi, miso, sclk), _cs(cs), _rs(rs), _reset(reset)
         {
    tft_reset();
    orientation = 2;
    char_x = 0;
}
int ST7735_TFT::width() {
    if (orientation == 0 || orientation == 2) return 128;
    else return 160; 
}
int ST7735_TFT::height() {
    if (orientation == 0 || orientation == 2) return 160;
    else return 128; 
}
void ST7735_TFT::set_orientation(unsigned int o) {
    orientation = o;
    switch (orientation) {
        case 0:
            wr_reg(ST7735_MADCTL, 0x0008);
            break;
        case 1:
            wr_reg(ST7735_MADCTL, 0x0068);
            break;
        case 2:
            wr_reg(ST7735_MADCTL, 0x00C8);
            break;
        case 3:
            wr_reg(ST7735_MADCTL, 0x00A8);
            break;
    }
}
void ST7735_TFT::wr_cmd(int cmd) {
    _rs = 0; // rs low, cs low for transmitting command
    _cs = 0;
    _spi.write(cmd);
    _cs = 1;
}
void ST7735_TFT::wr_dat(int dat) {
    _rs = 1; // rs high, cs low for transmitting data
    _cs = 0;                         
    _spi.write(dat);                                                           
    _cs = 1;
}
void ST7735_TFT::wr_dat_start(void) {
    _rs = 1; //  rs high, cs low for transmitting data
    _cs = 0;
}
void ST7735_TFT::wr_dat_stop (void) {
    _cs = 1;
}
void ST7735_TFT::wr_dat_only (unsigned short dat) {  
    _spi.write(dat);                                            
}
unsigned short ST7735_TFT::rd_dat (void) {
    unsigned short val = 0;
    _cs = 0;
    _spi.write(0);                                /* Dummy read 1                 */
    val   = _spi.write(0);                        /* Read D8..D15                 */
    val <<= 8;
    val  |= _spi.write(0);                        /* Read D0..D7                  */
    _cs = 1;
    return (val);
}
void ST7735_TFT::wr_reg (unsigned char reg, unsigned short val) {

    wr_cmd(reg);
    wr_dat(val);
}
unsigned short ST7735_TFT::rd_reg (unsigned char reg) {

    wr_cmd(reg);
    return(rd_dat());
}
void ST7735_TFT::read_area(unsigned int x, unsigned int y, unsigned int w, unsigned int h,unsigned char *buffer) {
    // BEWARE !
    // DOES NOT WORK CORRECTLY YET !!!
    int val;
    window(x,y,w,h);
    wr_cmd(ST7735_RAMRD);  // write to RAM
    _cs = 0;
    _rs = 1;
    _spi.write(0);                                /* Dummy read 1                 */
    
    val   = _spi.write(0);                        /* Read D8..D15                 */
    val <<= 8;
    val  |= _spi.write(0);                        /* Read D0..D7                  */
    _cs = 1;
}
int ST7735_TFT::getpixel(unsigned int x, unsigned int y) {
    // BEWARE !
    // DOES NOT WORK CORRECTLY YET !!!
    int val;
   // _spi.format(8,3);
    wr_cmd(ST7735_CASET);  // column addr set
    wr_dat(0x00);
    wr_dat(x+2);   // XSTART 
    wr_dat(0x00);
    wr_dat(x+2+2);   // XEND

    wr_cmd(ST7735_RASET);  // row addr set
    wr_dat(0x00);
    wr_dat(y+1);    // YSTART
    wr_dat(0x00);
    wr_dat(y+1+1);    // YEND
    
    _rs = 0; // rs low, cs low for transmitting command
    _cs = 0;
    _spi.write(0x2E);
    _rs = 1;
    _spi.write(0x00);                                /* Dummy read 1                 */
    
    val   = _spi.write(0x00);                        /* Read D8..D15                 */
    val <<= 8;
    val  |= _spi.write(0x00);                        /* Read D0..D7                  */

    _cs = 1;
    return val;
}
void ST7735_TFT::tft_reset() {
     // init SPI
    _spi.format(8,3);                 // 8 bit spi mode 3
    _spi.frequency(15000000);         // 16Mhz SPI clock ... 15Mhz is maximum for display, but it seems to work
    _cs = 0;
    _reset = 1;                       // reset
    wait_ms(10);
    _reset = 0;                       // reset
    wait_ms(10);
    _reset = 1;                       // reset
    wait_ms(10);
    
    /* Start Initial Sequence ----------------------------------------------------*/
    wr_cmd(ST7735_SWRESET);                         /* SW Reset                       */
    wait_ms(150);
    wr_cmd(ST7735_SLPOUT);                         /* Out of sleepmode               */
    wait_ms(500);
    
    wr_cmd(ST7735_FRMCTR1);                         /* Frame rate in normal mode            */
    wr_dat(0x01);                              
    wr_dat(0x2C);
    wr_dat(0x2D);
    
    wr_cmd(ST7735_FRMCTR2);                         /* Frame rate in idle mode            */
    wr_dat(0x01);                              
    wr_dat(0x2C);
    wr_dat(0x2D);

    wr_cmd(ST7735_FRMCTR3);                         /* Frame rate in partial mode            */
    wr_dat(0x01);                              
    wr_dat(0x2C);
    wr_dat(0x2D);
    wr_dat(0x01);   // inversion mode settings                              
    wr_dat(0x2C);
    wr_dat(0x2D);
    
    wr_cmd(ST7735_INVCTR);   // Inverted mode off
    wr_dat(0x07);   

    wr_cmd(ST7735_PWCTR1);   // POWER CONTROL 1   
    wr_dat(0xA2); 
    wr_dat(0x02);           // -4.6V
    wr_dat(0x84);           // AUTO mode 
    
    wr_cmd(ST7735_PWCTR2);   // POWER CONTROL 2   
    wr_dat(0xC5);            // VGH25 = 2.4C VGSEL =-10 VGH = 3*AVDD
    
    wr_cmd(ST7735_PWCTR3);  // POWER CONTROL 3   
    wr_dat(0x0A);           // Opamp current small
    wr_dat(0x00);           // Boost freq

    wr_cmd(ST7735_PWCTR4);   // POWER CONTROL 4   
    wr_dat(0x8A);            // BCLK/2, Opamp current small / medium low 
    wr_dat(0x2A);            // 
  
    wr_cmd(ST7735_PWCTR5);   // POWER CONTROL 5   
    wr_dat(0x8A);            // BCLK/2, Opamp current small / medium low 
    wr_dat(0xEE);            //
    
    wr_cmd(ST7735_VMCTR1);   // POWER CONTROL 6   
    wr_dat(0x0E);            // 

    wr_cmd(ST7735_INVOFF);   // INVOFF
    
    wr_cmd(ST7735_MADCTL);   // ORIENTATION   
    wr_dat(0xC8);   // 
    
    wr_cmd(ST7735_COLMOD);   // COLOR MODE   
    wr_dat(0x05);            //      
    
    wr_cmd(ST7735_CASET);   // COLUMN ADDR SET   
    wr_dat(0x00);   //
    wr_dat(0x00);   // xstart = 0
    wr_dat(0x00);   //
    wr_dat(0x7F);   // xend = 127
    
    wr_cmd(ST7735_RASET);   // ROW ADDR SET   
    wr_dat(0x00);   //
    wr_dat(0x00);   // ystart = 0
    wr_dat(0x00);   //
    wr_dat(0x9F);   // yend = 159            
    
    /* Gamma settings  -----------------------------------------------------------*/

  wr_cmd(0xE0); // GMCTRP1
  wr_dat(0x02);
  wr_dat(0x1c);
  wr_dat(0x07);
  wr_dat(0x12);
  wr_dat(0x37);
  wr_dat(0x32);
  wr_dat(0x29);
  wr_dat(0x2d);
  wr_dat(0x29);
  wr_dat(0x25);
  wr_dat(0x2B);
  wr_dat(0x39);
  wr_dat(0x00);
  wr_dat(0x01);
  wr_dat(0x03);
  wr_dat(0x10);
  wr_cmd(0xE1); // GMCTRN1
  wr_dat(0x03); 
  wr_dat(0x1d); 
  wr_dat(0x07); 
  wr_dat(0x06); 
  wr_dat(0x2E); 
  wr_dat(0x2C); 
  wr_dat(0x29); 
  wr_dat(0x2D); 
  wr_dat(0x2E); 
  wr_dat(0x2E); 
  wr_dat(0x37); 
  wr_dat(0x3F); 
  wr_dat(0x00); 
  wr_dat(0x00); 
  wr_dat(0x02); 
  wr_dat(0x10); 
  
  wr_cmd(ST7735_DISPON); // display ON
  wait_ms(100);

  wr_cmd(ST7735_NORON);  // normal display on
  wait_ms(10);
  
  switch (orientation) {
        case 0:
            wr_reg(0xC8, 0x0008);
            break;
        case 1:
            wr_reg(0xC8, 0x0068);
            break;
        case 2:
            wr_reg(0xC8, 0x00C8);
            break;
        case 3:
            wr_reg(0xC8, 0x00A8);
            break;
    }
    WindowMax ();
}
void ST7735_TFT::pixel(int x, int y, int color) {
  if ((x >= width()) || (y >= height())) return;
  window(x,y,x+1,y+1);  
   _rs = 1; 
   _cs = 0;
   _spi.write(color);
   _spi.write(color>>8);       
   _cs = 1;
}
void ST7735_TFT::window (unsigned int x, unsigned int y, unsigned int w, unsigned int h) {
  wr_cmd(ST7735_CASET);  // column addr set
  wr_dat(0x00);
  wr_dat(x);   // XSTART 
  wr_dat(0x00);
  wr_dat(x+w-1);   // XEND
  wr_cmd(ST7735_RASET);  // row addr set
  wr_dat(0x00);
  wr_dat(y);    // YSTART
  wr_dat(0x00);
  wr_dat(y+h-1);    // YEND
  wr_cmd(ST7735_RAMWR);  // write to RAM
}
void ST7735_TFT::WindowMax (void) {
    window (0, 0, width(),  height());
}

void ST7735_TFT::cls (void) {
    unsigned int i;
    WindowMax();
    wr_dat_start();    
    for (i = 0; i < ( (width()) * (height())); i++) {
        
        //_spi.write(_background>>8);
        //_spi.write(_background); 
        
        _spi.fastWrite(_background>>8);
        _spi.fastWrite(_background);
                   
    }
    _spi.clearRX();
    wr_dat_stop();   
}
void ST7735_TFT::circle(int x0, int y0, int r, int color) {
    int draw_x0, draw_y0;
    int draw_x1, draw_y1;
    int draw_x2, draw_y2;
    int draw_x3, draw_y3;
    int draw_x4, draw_y4;
    int draw_x5, draw_y5;
    int draw_x6, draw_y6;
    int draw_x7, draw_y7;
    int xx, yy;
    int di;
    WindowMax();
    if (r == 0) {       /* no radius */
        return;
    }
    draw_x0 = draw_x1 = x0;
    draw_y0 = draw_y1 = y0 + r;
    if (draw_y0 < height()) {
        pixel(draw_x0, draw_y0, color);     /* 90 degree */
    }
    draw_x2 = draw_x3 = x0;
    draw_y2 = draw_y3 = y0 - r;
    if (draw_y2 >= 0) {
        pixel(draw_x2, draw_y2, color);    /* 270 degree */
    }
    draw_x4 = draw_x6 = x0 + r;
    draw_y4 = draw_y6 = y0;
    if (draw_x4 < width()) {
        pixel(draw_x4, draw_y4, color);     /* 0 degree */
    }
    draw_x5 = draw_x7 = x0 - r;
    draw_y5 = draw_y7 = y0;
    if (draw_x5>=0) {
        pixel(draw_x5, draw_y5, color);     /* 180 degree */
    }
    if (r == 1) {
        return;
    }
    di = 3 - 2*r;
    xx = 0;
    yy = r;
    while (xx < yy) {

        if (di < 0) {
            di += 4*xx + 6;
        } else {
            di += 4*(xx - yy) + 10;
            yy--;
            draw_y0--;
            draw_y1--;
            draw_y2++;
            draw_y3++;
            draw_x4--;
            draw_x5++;
            draw_x6--;
            draw_x7++;
        }
        xx++;
        draw_x0++;
        draw_x1--;
        draw_x2++;
        draw_x3--;
        draw_y4++;
        draw_y5++;
        draw_y6--;
        draw_y7--;

        if ( (draw_x0 <= width()) && (draw_y0>=0) ) {
            pixel(draw_x0, draw_y0, color);
        }
        if ( (draw_x1 >= 0) && (draw_y1 >= 0) ) {
            pixel(draw_x1, draw_y1, color);
        }
        if ( (draw_x2 <= width()) && (draw_y2 <= height()) ) {
            pixel(draw_x2, draw_y2, color);
        }
        if ( (draw_x3 >=0 ) && (draw_y3 <= height()) ) {
            pixel(draw_x3, draw_y3, color);
        }
        if ( (draw_x4 <= width()) && (draw_y4 >= 0) ) {
            pixel(draw_x4, draw_y4, color);
        }
        if ( (draw_x5 >= 0) && (draw_y5 >= 0) ) {
            pixel(draw_x5, draw_y5, color);
        }
        if ( (draw_x6 <=width()) && (draw_y6 <= height()) ) {
            pixel(draw_x6, draw_y6, color);
        }
        if ( (draw_x7 >= 0) && (draw_y7 <= height()) ) {
            pixel(draw_x7, draw_y7, color);
        }
    }
    return;
}
void ST7735_TFT::fillcircle(int x, int y, int r, int color) {
    int i;
    for (i = 0; i <= r; i++)
        circle(x,y,i,color);
}
void ST7735_TFT::hline(int x0, int x1, int y, int color) {
    int w;
    w = x1 - x0 + 1;
    window(x0,y,w,1);
    wr_cmd(0x2C);
    wr_dat_start();
    for (int x=0; x<w; x++) {
        //_spi.write(color);
        //_spi.write(color >> 8);
        _spi.fastWrite(color>>8);
        _spi.fastWrite(color);
    }
    _spi.clearRX();
    wr_dat_stop();
    return;
}
void ST7735_TFT::vline(int x, int y0, int y1, int color) {
    int h;
    h = y1 - y0 + 1;
    window(x,y0,1,h);
    wr_cmd(0x2C);
    wr_dat_start();
    for (int y=0; y<h; y++) {
        //_spi.write(color);
        //_spi.write(color >> 8);
        _spi.fastWrite(color>>8);
        _spi.fastWrite(color);
    }
    _spi.clearRX();
    wr_dat_stop();
    return;
}
void ST7735_TFT::line(int x0, int y0, int x1, int y1, int color) {
    WindowMax();
    int   dx = 0, dy = 0;
    int   dx_sym = 0, dy_sym = 0;
    int   dx_x2 = 0, dy_x2 = 0;
    int   di = 0;
    dx = x1-x0;
    dy = y1-y0;

    if (dx == 0) {        /* vertical line */
        if (y1 > y0) vline(x0,y0,y1,color);
        else vline(x0,y1,y0,color);
        return;
    }
    if (dx > 0) {
        dx_sym = 1;
    } else {
        dx_sym = -1;
    }
    if (dy == 0) {        /* horizontal line */
        if (x1 > x0) hline(x0,x1,y0,color);
        else  hline(x1,x0,y0,color);
        return;
    }
    if (dy > 0) {
        dy_sym = 1;
    } else {
        dy_sym = -1;
    }
    dx = dx_sym*dx;
    dy = dy_sym*dy;
    dx_x2 = dx*2;
    dy_x2 = dy*2;
    if (dx >= dy) {
        di = dy_x2 - dx;
        while (x0 != x1) {

            pixel(x0, y0, color);
            x0 += dx_sym;
            if (di<0) {
                di += dy_x2;
            } else {
                di += dy_x2 - dx_x2;
                y0 += dy_sym;
            }
        }
        pixel(x0, y0, color);
    } else {
        di = dx_x2 - dy;
        while (y0 != y1) {
            pixel(x0, y0, color);
            y0 += dy_sym;
            if (di < 0) {
                di += dx_x2;
            } else {
                di += dx_x2 - dy_x2;
                x0 += dx_sym;
            }
        }
        pixel(x0, y0, color);
    }
    return;
}
void ST7735_TFT::rect(int x0, int y0, int x1, int y1, int color) {
    if (x1 > x0) hline(x0,x1,y0,color);
    else  hline(x1,x0,y0,color);
    if (y1 > y0) vline(x0,y0,y1,color);
    else vline(x0,y1,y0,color);
    if (x1 > x0) hline(x0,x1,y1,color);
    else  hline(x1,x0,y1,color);
    if (y1 > y0) vline(x1,y0,y1,color);
    else vline(x1,y1,y0,color);
    return;
}
void ST7735_TFT::fillrect(int x0, int y0, int x1, int y1, int color) {
    int h = y1 - y0 ;
    int w = x1 - x0 ;
    int pixel = h * w;
    window(x0,y0,w,h);
    wr_cmd(0x2C);
    wr_dat_start();
    for (int p=0; p<pixel; p++) {
        //_spi.write(color);
        //_spi.write(color >> 8);
        _spi.fastWrite(color>>8);
        _spi.fastWrite(color);
        
        
    }
    _spi.clearRX();
    wr_dat_stop();
    return;
}
void ST7735_TFT::locate(int x, int y) {
    char_x = x;
    char_y = y;
}
int ST7735_TFT::columns() {
    return width() / font[1];
}
int ST7735_TFT::rows() {
    return height() / font[2];
}
int ST7735_TFT::_putc(int value) {
    if (value == '\n') {    // new line
        char_x = 0;
        char_y = char_y + font[2];
        if (char_y >= height() - font[2]) {
            char_y = 0;
        }
    } else {
        character(char_x, char_y, value);
     }
    return value;
}
void ST7735_TFT::character(int x, int y, int c) {
    unsigned int hor,vert,offset,bpl,j,i,b;
    unsigned char* sign;
    unsigned char z,w;
    if ((c < 31) || (c > 127)) return;   // test char range
    // read font parameter from start of array
    offset = font[0];                    // bytes / char
    hor = font[1];                       // get hor size of font
    vert = font[2];                      // get vert size of font
    bpl = font[3];                       // bytes per line
    if (char_x + hor > width()) {
        char_x = 0;
        char_y = char_y + vert;
       if (char_y >= height() - font[2]) {
            char_y = 0;
        }
    }
    window(char_x, char_y,hor,vert); // char box
    wr_cmd(0x2C);
    wr_dat_start();
    sign = &font[((c -32) * offset) + 4]; // start of char bitmap
    w = sign[0];                          // width of actual char
    for (j=0; j<vert; j++) {  //  vert line
        for (i=0; i<hor; i++) {   //  horz line
            z =  sign[bpl * i + ((j & 0xF8) >> 3)+1];
            b = 1 << (j & 0x07);
            if (( z & b ) == 0x00) {                  
                //_spi.write(_background>>8);
                //_spi.write(_background); 
                _spi.fastWrite(_background>>8);
                _spi.fastWrite(_background);                              
                  
            } else {                     
               // _spi.write(_foreground>>8);
               // _spi.write(_foreground); 
                _spi.fastWrite(_foreground>>8); 
                _spi.fastWrite(_foreground);     
                                 
            }
        }
    }
    _spi.clearRX();
    wr_dat_stop();
    if ((w + 2) < hor) {                   // x offset to next char
        char_x += w + 2;
    } else char_x += hor;
}
void ST7735_TFT::set_font(unsigned char* f) {
    font = f;
}
void ST7735_TFT::Bitmap(unsigned int x, unsigned int y, unsigned int w, unsigned int h,unsigned char *bitmap) {
    unsigned int    i,j; //value;
    unsigned short *bitmap_ptr = (unsigned short *)bitmap;
    window(x, y, w, h);
    wr_cmd(0x2C);
    wr_dat_start();
    for (j = 0; j < h; j++) {        //Lines
        for (i = 0; i < w; i++) {     // copy pixel data to TFT
            //_spi.write(*bitmap_ptr);    // one line
            //_spi.write(*bitmap_ptr >> 8);
            _spi.fastWrite(*bitmap_ptr);
            _spi.fastWrite(*bitmap_ptr >> 8);  
            bitmap_ptr++;
        }
    }
    _spi.clearRX();
    wr_dat_stop();
}
int ST7735_TFT::BMP_16(unsigned int x, unsigned int y, const char *Name_BMP) {
// BEWARE !
// NOT TESTED
#define OffsetPixelWidth    18
#define OffsetPixelHeigh    22
#define OffsetFileSize      34
#define OffsetPixData       10
#define OffsetBPP           28
    char filename[50];
    unsigned char BMP_Header[54];
    unsigned short BPP_t;
    unsigned int PixelWidth,PixelHeigh,start_data;
    unsigned int    i,off;
    int padd,j;
    unsigned short *line;
    // get the filename
    //LocalFileSystem local("local"); not working on FRDM-KL25Z
    sprintf(&filename[0],"/local/");
    i=7;
    while (*Name_BMP!='\0') {
        filename[i++]=*Name_BMP++;
    }
    FILE *Image = fopen((const char *)&filename[0], "r");  // open the bmp file
    if (!Image) {
        return(0);      // error file not found !
    }
    fread(&BMP_Header[0],1,54,Image);      // get the BMP Header

    if (BMP_Header[0] != 0x42 || BMP_Header[1] != 0x4D) {  // check magic byte
        fclose(Image);
        return(-1);     // error no BMP file
    }
    BPP_t = BMP_Header[OffsetBPP] + (BMP_Header[OffsetBPP + 1] << 8);
    if (BPP_t != 0x0010) {
        fclose(Image);
        return(-2);     // error no 16 bit BMP
    }
    PixelHeigh = BMP_Header[OffsetPixelHeigh] + (BMP_Header[OffsetPixelHeigh + 1] << 8) + (BMP_Header[OffsetPixelHeigh + 2] << 16) + (BMP_Header[OffsetPixelHeigh + 3] << 24);
    PixelWidth = BMP_Header[OffsetPixelWidth] + (BMP_Header[OffsetPixelWidth + 1] << 8) + (BMP_Header[OffsetPixelWidth + 2] << 16) + (BMP_Header[OffsetPixelWidth + 3] << 24);
    if (PixelHeigh > height() + y || PixelWidth > width() + x) {
        fclose(Image);
        return(-3);      // to big
    }
    start_data = BMP_Header[OffsetPixData] + (BMP_Header[OffsetPixData + 1] << 8) + (BMP_Header[OffsetPixData + 2] << 16) + (BMP_Header[OffsetPixData + 3] << 24);

    line = (unsigned short *) malloc (PixelWidth); // we need a buffer for a line
    if (line == NULL) {
        return(-4);         // error no memory
    }
    // the lines are padded to multiple of 4 bytes
    padd = -1;
    do {
        padd ++;
    } while ((PixelWidth * 2 + padd)%4 != 0);
    window(x, y,PixelWidth,PixelHeigh);
    wr_cmd(0x2C);
    wr_dat_start();
    for (j = PixelHeigh - 1; j >= 0; j--) {               //Lines bottom up
        off = j * (PixelWidth * 2 + padd) + start_data;   // start of line
        fseek(Image, off ,SEEK_SET);
        fread(line,1,PixelWidth * 2,Image);       // read a line - slow !
        for (i = 0; i < PixelWidth; i++) {        // copy pixel data to TFT
            //_spi.write(line[i]);                  // 2 x 8 bit pixel
            //_spi.write(line[i]>>8);
            _spi.fastWrite(line[i]);
            _spi.fastWrite(line[i]>>8);
        } 
    }
    _spi.clearRX();
    wr_dat_stop();
    free (line);
    fclose(Image);
    return(1);
}