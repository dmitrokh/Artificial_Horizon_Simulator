 //  ST7735 8 Bit SPI Library
 
#include "TextDisplay.h"

TextDisplay::TextDisplay(const char *name) : Stream(name){
    _row = 0;
    _column = 0;
    if (name == NULL) {
        _path = NULL;
    } else {
        _path = new char[strlen(name) + 2];
        sprintf(_path, "/%s", name);
    }
}
    
int TextDisplay::_putc(int value) {
    if(value == '\n') {
        _column = 0;
        _row++;
        if(_row >= rows()) {
            _row = 0;
        }
    } else {
        character(_column, _row, value);
        _column++;
        if(_column >= columns()) {
            _column = 0;
            _row++;
            if(_row >= rows()) {
                _row = 0;
            }
        }
    }
    return value;
}
void TextDisplay::cls() {
    locate(0, 0);
    for(int i=0; i<columns()*rows(); i++) {
        putc(' ');
    }
}
void TextDisplay::locate(int column, int row) {
    _column = column;
    _row = row;
}
int TextDisplay::_getc() {
    return -1;
}        
void TextDisplay::foreground(uint16_t colour) {
    _foreground = colour;
}
void TextDisplay::background(uint16_t colour) {
    _background = colour;
}
bool TextDisplay::claim (FILE *stream) {
    if ( _path == NULL) {
        fprintf(stderr, "claim requires a name to be given in the instantioator of the TextDisplay instance!\r\n");
        return false;
    }
    if (freopen(_path, "w", stream) == NULL) {
        // Failed, should not happen
        return false;
    }
    // make sure we use line buffering
    setvbuf(stdout, NULL, _IOLBF, columns());
    return true;
} 
