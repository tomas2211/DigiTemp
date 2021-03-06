/**
 *  @filename   :   epdpaint.h
 *  @brief      :   Header file for epdpaint.cpp
 *  @author     :   Yehui from Waveshare
 *  
 *  Copyright (C) Waveshare     July 28 2017
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documnetation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to  whom the Software is
 * furished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef EPDPAINT_H
#define EPDPAINT_H

// Display orientation
#define ROTATE_0            0
#define ROTATE_90           1
#define ROTATE_180          2
#define ROTATE_270          3

// Color inverse. 1 or 0 = set or reset a bit if set a colored pixel
#define IF_INVERT_COLOR     1

#include "Print.h"
//#include "fonts.h"
#include "gfxfont.h"
#include "Arduino.h"


class Paint : public Print {
public:
    Paint(unsigned char* image, int width, int height);
    ~Paint();
    void Clear(int colored);
    int  GetWidth(void);
    void SetWidth(int width);
    int  GetHeight(void);
    void SetHeight(int height);
    int  GetRotate(void);
    void SetRotate(int rotate);
    unsigned char* GetImage(void);
    void DrawAbsolutePixel(int x, int y, int colored);
    void DrawPixel(int x, int y, int colored);
//    void DrawCharAt(int x, int y, char ascii_char, sFONT* font, int colored);
//    void DrawStringAt(int x, int y, const char* text, sFONT* font, int colored);
    void DrawLine(int x0, int y0, int x1, int y1, int colored);
    void DrawHorizontalLine(int x, int y, int width, int colored);
    void DrawVerticalLine(int x, int y, int height, int colored);
    void DrawRectangle(int x0, int y0, int x1, int y1, int colored);
    void DrawFilledRectangle(int x0, int y0, int x1, int y1, int colored);
    void DrawCircle(int x, int y, int radius, int colored);
    void DrawFilledCircle(int x, int y, int radius, int colored);

    void DrawCharAt(int16_t x, int16_t y, unsigned char c);
    void setFont(const GFXfont *f = NULL);
    void PutBMPAt(int16_t x, int16_t y, int16_t width, int16_t height, const uint8_t *bmp, uint8_t invert);
    void PutBMP4bitCompAt(int16_t x, int16_t y, int16_t width, int16_t height, const uint8_t *bmp, uint8_t invert);

    using Print::write;

    virtual size_t write(uint8_t);


    void setTextColor(int c) { textcolor = c; }

    void setCursor(int16_t x, int16_t y) {
        cursor_x = x;
        cursor_y = y;
    }

private:
    unsigned char* image;
    int width;
    int height;
    int rotate;
    int textcolor;
    int16_t cursor_x;       ///< x location to start print()ing text
    int16_t cursor_y;       ///< y location to start print()ing text
    GFXfont *gfxFont;   ///< Pointer to special font
    uint8_t textsize_x;  ///< Desired magnification in X-axis of text to print()
    uint8_t textsize_y;     ///< Desired magnification in Y-axis of text to print()
    boolean wrap;       ///< If set, 'wrap' text at right edge of display


};

#endif

/* END OF FILE */

