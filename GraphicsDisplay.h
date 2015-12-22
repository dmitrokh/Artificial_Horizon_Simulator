 //  ST7735 8 Bit SPI Library
 
#ifndef MBED_GRAPHICSDISPLAY_H
#define MBED_GRAPHICSDISPLAY_H

#include "TextDisplay.h"

class GraphicsDisplay : public TextDisplay {

public:         
          
    GraphicsDisplay(const char* name);
     
    virtual void pixel(int x, int y, int colour) = 0;
    virtual int width() = 0;
    virtual int height() = 0;
        
    virtual void gwindow(int x, int y, int w, int h);
    virtual void putp(int colour);
    
    virtual void cls();
    virtual void fill(int x, int y, int w, int h, int colour);
    virtual void blit(int x, int y, int w, int h, const int *colour);    
    virtual void blitbit(int x, int y, int w, int h, const char* colour);
    
    virtual void character(int column, int row, int value);
    virtual int columns();
    virtual int rows();
    
protected:

    // pixel location
    short _x;
    short _y;
    
    // window location
    short _x1;
    short _x2;
    short _y1;
    short _y2;

};

#endif
