#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "../LcdDriver/Crystalfontz128x128_ST7735.h"
#include <ti/grlib/grlib.h>
#include "view.h"
#include <stdio.h>

tRectangle bigRect = { 2, 2, 125, 125};

/* Graphic library context */
Graphics_Context context;

void graphicsInit()
{
    /* Initializes display */
       Crystalfontz128x128_Init();

       /* Set default screen orientation */
       Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);

       /* Initializes graphics context */
       Graphics_initContext(&context, &g_sCrystalfontz128x128, &g_sCrystalfontz128x128_funcs);
           Graphics_setForegroundColor(&context, GRAPHICS_COLOR_BLACK);
           Graphics_setBackgroundColor(&context, GRAPHICS_COLOR_WHITE);
           GrContextFontSet(&context, &g_sFontCm12b);
           Graphics_clearDisplay(&context);
           Graphics_drawStringCentered(&context,
                                           (int8_t *)"Robotic Hand Menu:",
                                           AUTO_STRING_LENGTH,
                                           64,
                                           10,
                                           TRANSPARENT_TEXT);

           Graphics_drawCircle(&context, 116, 50, 5);
           Graphics_drawCircle(&context, 116, 70, 5);
           Graphics_drawCircle(&context, 116, 90, 5);

           Graphics_drawRectangle(&context, &bigRect);
           Graphics_drawLineH(&context, 2, 125, 40);
           Graphics_drawLineH(&context, 2, 125, 60);
           Graphics_drawLineH(&context, 2, 125, 80);
           Graphics_drawLineH(&context, 2, 125, 100);

           Graphics_drawStringCentered(&context,
                                           (int8_t *)"Manual Controller",
                                           AUTO_STRING_LENGTH,
                                           56,
                                           50,
                                           OPAQUE_TEXT);
           Graphics_drawStringCentered(&context,
                                           (int8_t *)"OpenCV Controller",
                                           AUTO_STRING_LENGTH,
                                           56,
                                           70,
                                           OPAQUE_TEXT);

           Graphics_drawStringCentered(&context,
                                              (int8_t *)"Sign Recorder",
                                              AUTO_STRING_LENGTH,
                                              56,
                                              90,
                                              OPAQUE_TEXT);

}

void drawIdleMenu()
{
    Graphics_clearDisplay(&context);
            Graphics_drawStringCentered(&context,
                                            (int8_t *)"Robotic Hand Menu:",
                                            AUTO_STRING_LENGTH,
                                            64,
                                            10,
                                            TRANSPARENT_TEXT);

            Graphics_drawCircle(&context, 116, 50, 5);
            Graphics_drawCircle(&context, 116, 70, 5);
            Graphics_drawCircle(&context, 116, 90, 5);

            Graphics_drawRectangle(&context, &bigRect);
            Graphics_drawLineH(&context, 2, 125, 40);
            Graphics_drawLineH(&context, 2, 125, 60);
            Graphics_drawLineH(&context, 2, 125, 80);
            Graphics_drawLineH(&context, 2, 125, 100);

            Graphics_drawStringCentered(&context,
                                            (int8_t *)"Manual Controller",
                                            AUTO_STRING_LENGTH,
                                            56,
                                            50,
                                            OPAQUE_TEXT);
            Graphics_drawStringCentered(&context,
                                            (int8_t *)"OpenCV Controller",
                                            AUTO_STRING_LENGTH,
                                            56,
                                            70,
                                            OPAQUE_TEXT);

            Graphics_drawStringCentered(&context,
                                               (int8_t *)"Sign Recorder",
                                               AUTO_STRING_LENGTH,
                                               56,
                                               90,
                                               OPAQUE_TEXT);
}

void drawManualSelected()
{
    Graphics_clearDisplay(&context);
           Graphics_drawStringCentered(&context,
                                         (int8_t *)"Robotic Hand Menu:",
                                         AUTO_STRING_LENGTH,
                                         64,
                                         10,
                                         TRANSPARENT_TEXT);

            Graphics_fillCircle(&context, 116, 50, 5);
            Graphics_drawCircle(&context, 116, 70, 5);
            Graphics_drawCircle(&context, 116, 90, 5);

            Graphics_drawRectangle(&context, &bigRect);
            Graphics_drawLineH(&context, 2, 125, 40);
            Graphics_drawLineH(&context, 2, 125, 60);
            Graphics_drawLineH(&context, 2, 125, 80);
            Graphics_drawLineH(&context, 2, 125, 100);

            Graphics_drawStringCentered(&context,
                                         (int8_t *)"Manual Controller",
                                         AUTO_STRING_LENGTH,
                                         56,
                                         50,
                                         OPAQUE_TEXT);
            Graphics_drawStringCentered(&context,
                                         (int8_t *)"OpenCV Controller",
                                         AUTO_STRING_LENGTH,
                                         56,
                                         70,
                                         OPAQUE_TEXT);

            Graphics_drawStringCentered(&context,
                                               (int8_t *)"Sign Recorder",
                                               AUTO_STRING_LENGTH,
                                               56,
                                               90,
                                               OPAQUE_TEXT);
}

void drawVisionSelected()
{

    Graphics_clearDisplay(&context);
    Graphics_drawStringCentered(&context,
                                          (int8_t *)"Robotic Hand Menu:",
                                          AUTO_STRING_LENGTH,
                                          64,
                                          10,
                                          TRANSPARENT_TEXT);

    Graphics_drawCircle(&context, 116, 50, 5);
    Graphics_fillCircle(&context, 116, 70, 5);
    Graphics_drawCircle(&context, 116, 90, 5);

    Graphics_drawRectangle(&context, &bigRect);
    Graphics_drawLineH(&context, 2, 125, 40);
    Graphics_drawLineH(&context, 2, 125, 60);
    Graphics_drawLineH(&context, 2, 125, 80);
    Graphics_drawLineH(&context, 2, 125, 100);

    Graphics_drawStringCentered(&context,
                                  (int8_t *)"Manual Controller",
                                  AUTO_STRING_LENGTH,
                                  56,
                                  50,
                                  OPAQUE_TEXT);
    Graphics_drawStringCentered(&context,
                                  (int8_t *)"OpenCV Controller",
                                  AUTO_STRING_LENGTH,
                                  56,
                                  70,
                                  OPAQUE_TEXT);

    Graphics_drawStringCentered(&context,
                                       (int8_t *)"Sign Recorder",
                                       AUTO_STRING_LENGTH,
                                       56,
                                       90,
                                       OPAQUE_TEXT);
}

void drawGestureSelected()
{

    Graphics_clearDisplay(&context);
    Graphics_drawStringCentered(&context,
                                          (int8_t *)"Robotic Hand Menu:",
                                          AUTO_STRING_LENGTH,
                                          64,
                                          10,
                                          TRANSPARENT_TEXT);

    Graphics_drawCircle(&context, 116, 50, 5);
    Graphics_drawCircle(&context, 116, 70, 5);
    Graphics_fillCircle(&context, 116, 90, 5);

    Graphics_drawRectangle(&context, &bigRect);
    Graphics_drawLineH(&context, 2, 125, 40);
    Graphics_drawLineH(&context, 2, 125, 60);
    Graphics_drawLineH(&context, 2, 125, 80);
    Graphics_drawLineH(&context, 2, 125, 100);

    Graphics_drawStringCentered(&context,
                                  (int8_t *)"Manual Controller",
                                  AUTO_STRING_LENGTH,
                                  56,
                                  50,
                                  OPAQUE_TEXT);
    Graphics_drawStringCentered(&context,
                                  (int8_t *)"OpenCV Controller",
                                  AUTO_STRING_LENGTH,
                                  56,
                                  70,
                                  OPAQUE_TEXT);

    Graphics_drawStringCentered(&context,
                                       (int8_t *)"Sign Recorder",
                                       AUTO_STRING_LENGTH,
                                       56,
                                       90,
                                       OPAQUE_TEXT);
}

void drawFingersMenu()
{
    Graphics_clearDisplay(&context);
    Graphics_drawStringCentered(&context,
                                          (int8_t *)"Fingers Menu:",
                                          AUTO_STRING_LENGTH,
                                          64,
                                          10,
                                          TRANSPARENT_TEXT);

    Graphics_drawCircle(&context, 116, 30, 5);
    Graphics_drawCircle(&context, 116, 50, 5);
    Graphics_drawCircle(&context, 116, 70, 5);
    Graphics_drawCircle(&context, 116, 90, 5);
    Graphics_drawCircle(&context, 116, 110, 5);

    Graphics_drawRectangle(&context, &bigRect);

    Graphics_drawLineH(&context, 2, 125, 20);
    Graphics_drawLineH(&context, 2, 125, 40);
    Graphics_drawLineH(&context, 2, 125, 60);
    Graphics_drawLineH(&context, 2, 125, 80);
    Graphics_drawLineH(&context, 2, 125, 100);

    Graphics_drawStringCentered(&context,
                                    (int8_t *)"Thumb",
                                    AUTO_STRING_LENGTH,
                                    56,
                                    30,
                                    OPAQUE_TEXT);

    Graphics_drawStringCentered(&context,
                                  (int8_t *)"Index",
                                  AUTO_STRING_LENGTH,
                                  56,
                                  50,
                                  OPAQUE_TEXT);
    Graphics_drawStringCentered(&context,
                                  (int8_t *)"Middle",
                                  AUTO_STRING_LENGTH,
                                  56,
                                  70,
                                  OPAQUE_TEXT);

    Graphics_drawStringCentered(&context,
                                       (int8_t *)"Ring",
                                       AUTO_STRING_LENGTH,
                                       56,
                                       90,
                                       OPAQUE_TEXT);

    Graphics_drawStringCentered(&context,
                                          (int8_t *)"Pinky",
                                          AUTO_STRING_LENGTH,
                                          56,
                                          110,
                                          OPAQUE_TEXT);
}

void drawThumbSelected()
{
    Graphics_clearDisplay(&context);
    Graphics_drawStringCentered(&context,
                                          (int8_t *)"Fingers Menu:",
                                          AUTO_STRING_LENGTH,
                                          64,
                                          10,
                                          TRANSPARENT_TEXT);

    Graphics_fillCircle(&context, 116, 30, 5);
    Graphics_drawCircle(&context, 116, 50, 5);
    Graphics_drawCircle(&context, 116, 70, 5);
    Graphics_drawCircle(&context, 116, 90, 5);
    Graphics_drawCircle(&context, 116, 110, 5);

    Graphics_drawRectangle(&context, &bigRect);

    Graphics_drawLineH(&context, 2, 125, 20);
    Graphics_drawLineH(&context, 2, 125, 40);
    Graphics_drawLineH(&context, 2, 125, 60);
    Graphics_drawLineH(&context, 2, 125, 80);
    Graphics_drawLineH(&context, 2, 125, 100);

    Graphics_drawStringCentered(&context,
                                    (int8_t *)"Thumb",
                                    AUTO_STRING_LENGTH,
                                    56,
                                    30,
                                    OPAQUE_TEXT);

    Graphics_drawStringCentered(&context,
                                  (int8_t *)"Index",
                                  AUTO_STRING_LENGTH,
                                  56,
                                  50,
                                  OPAQUE_TEXT);
    Graphics_drawStringCentered(&context,
                                  (int8_t *)"Middle",
                                  AUTO_STRING_LENGTH,
                                  56,
                                  70,
                                  OPAQUE_TEXT);

    Graphics_drawStringCentered(&context,
                                       (int8_t *)"Ring",
                                       AUTO_STRING_LENGTH,
                                       56,
                                       90,
                                       OPAQUE_TEXT);

    Graphics_drawStringCentered(&context,
                                          (int8_t *)"Pinky",
                                          AUTO_STRING_LENGTH,
                                          56,
                                          110,
                                          OPAQUE_TEXT);
}

void drawIndexSelected()
{
    Graphics_clearDisplay(&context);
       Graphics_drawStringCentered(&context,
                                             (int8_t *)"Fingers Menu:",
                                             AUTO_STRING_LENGTH,
                                             64,
                                             10,
                                             TRANSPARENT_TEXT);

       Graphics_drawCircle(&context, 116, 30, 5);
       Graphics_fillCircle(&context, 116, 50, 5);
       Graphics_drawCircle(&context, 116, 70, 5);
       Graphics_drawCircle(&context, 116, 90, 5);
       Graphics_drawCircle(&context, 116, 110, 5);

       Graphics_drawRectangle(&context, &bigRect);

       Graphics_drawLineH(&context, 2, 125, 20);
       Graphics_drawLineH(&context, 2, 125, 40);
       Graphics_drawLineH(&context, 2, 125, 60);
       Graphics_drawLineH(&context, 2, 125, 80);
       Graphics_drawLineH(&context, 2, 125, 100);

       Graphics_drawStringCentered(&context,
                                       (int8_t *)"Thumb",
                                       AUTO_STRING_LENGTH,
                                       56,
                                       30,
                                       OPAQUE_TEXT);

       Graphics_drawStringCentered(&context,
                                     (int8_t *)"Index",
                                     AUTO_STRING_LENGTH,
                                     56,
                                     50,
                                     OPAQUE_TEXT);
       Graphics_drawStringCentered(&context,
                                     (int8_t *)"Middle",
                                     AUTO_STRING_LENGTH,
                                     56,
                                     70,
                                     OPAQUE_TEXT);

       Graphics_drawStringCentered(&context,
                                          (int8_t *)"Ring",
                                          AUTO_STRING_LENGTH,
                                          56,
                                          90,
                                          OPAQUE_TEXT);

       Graphics_drawStringCentered(&context,
                                             (int8_t *)"Pinky",
                                             AUTO_STRING_LENGTH,
                                             56,
                                             110,
                                             OPAQUE_TEXT);
}

void drawMiddleSelected()
{
    Graphics_clearDisplay(&context);
      Graphics_drawStringCentered(&context,
                                            (int8_t *)"Fingers Menu:",
                                            AUTO_STRING_LENGTH,
                                            64,
                                            10,
                                            TRANSPARENT_TEXT);

      Graphics_drawCircle(&context, 116, 30, 5);
      Graphics_drawCircle(&context, 116, 50, 5);
      Graphics_fillCircle(&context, 116, 70, 5);
      Graphics_drawCircle(&context, 116, 90, 5);
      Graphics_drawCircle(&context, 116, 110, 5);

      Graphics_drawRectangle(&context, &bigRect);

      Graphics_drawLineH(&context, 2, 125, 20);
      Graphics_drawLineH(&context, 2, 125, 40);
      Graphics_drawLineH(&context, 2, 125, 60);
      Graphics_drawLineH(&context, 2, 125, 80);
      Graphics_drawLineH(&context, 2, 125, 100);

      Graphics_drawStringCentered(&context,
                                      (int8_t *)"Thumb",
                                      AUTO_STRING_LENGTH,
                                      56,
                                      30,
                                      OPAQUE_TEXT);

      Graphics_drawStringCentered(&context,
                                    (int8_t *)"Index",
                                    AUTO_STRING_LENGTH,
                                    56,
                                    50,
                                    OPAQUE_TEXT);
      Graphics_drawStringCentered(&context,
                                    (int8_t *)"Middle",
                                    AUTO_STRING_LENGTH,
                                    56,
                                    70,
                                    OPAQUE_TEXT);

      Graphics_drawStringCentered(&context,
                                         (int8_t *)"Ring",
                                         AUTO_STRING_LENGTH,
                                         56,
                                         90,
                                         OPAQUE_TEXT);

      Graphics_drawStringCentered(&context,
                                            (int8_t *)"Pinky",
                                            AUTO_STRING_LENGTH,
                                            56,
                                            110,
                                            OPAQUE_TEXT);
}

void drawRingSelected()
{
    Graphics_clearDisplay(&context);
      Graphics_drawStringCentered(&context,
                                            (int8_t *)"Fingers Menu:",
                                            AUTO_STRING_LENGTH,
                                            64,
                                            10,
                                            TRANSPARENT_TEXT);

      Graphics_drawCircle(&context, 116, 30, 5);
      Graphics_drawCircle(&context, 116, 50, 5);
      Graphics_drawCircle(&context, 116, 70, 5);
      Graphics_fillCircle(&context, 116, 90, 5);
      Graphics_drawCircle(&context, 116, 110, 5);

      Graphics_drawRectangle(&context, &bigRect);

      Graphics_drawLineH(&context, 2, 125, 20);
      Graphics_drawLineH(&context, 2, 125, 40);
      Graphics_drawLineH(&context, 2, 125, 60);
      Graphics_drawLineH(&context, 2, 125, 80);
      Graphics_drawLineH(&context, 2, 125, 100);

      Graphics_drawStringCentered(&context,
                                      (int8_t *)"Thumb",
                                      AUTO_STRING_LENGTH,
                                      56,
                                      30,
                                      OPAQUE_TEXT);

      Graphics_drawStringCentered(&context,
                                    (int8_t *)"Index",
                                    AUTO_STRING_LENGTH,
                                    56,
                                    50,
                                    OPAQUE_TEXT);
      Graphics_drawStringCentered(&context,
                                    (int8_t *)"Middle",
                                    AUTO_STRING_LENGTH,
                                    56,
                                    70,
                                    OPAQUE_TEXT);

      Graphics_drawStringCentered(&context,
                                         (int8_t *)"Ring",
                                         AUTO_STRING_LENGTH,
                                         56,
                                         90,
                                         OPAQUE_TEXT);

      Graphics_drawStringCentered(&context,
                                            (int8_t *)"Pinky",
                                            AUTO_STRING_LENGTH,
                                            56,
                                            110,
                                            OPAQUE_TEXT);
}

void drawPinkySelected()
{
    Graphics_clearDisplay(&context);
      Graphics_drawStringCentered(&context,
                                            (int8_t *)"Fingers Menu:",
                                            AUTO_STRING_LENGTH,
                                            64,
                                            10,
                                            TRANSPARENT_TEXT);

      Graphics_drawCircle(&context, 116, 30, 5);
      Graphics_drawCircle(&context, 116, 50, 5);
      Graphics_drawCircle(&context, 116, 70, 5);
      Graphics_drawCircle(&context, 116, 90, 5);
      Graphics_fillCircle(&context, 116, 110, 5);

      Graphics_drawRectangle(&context, &bigRect);

      Graphics_drawLineH(&context, 2, 125, 20);
      Graphics_drawLineH(&context, 2, 125, 40);
      Graphics_drawLineH(&context, 2, 125, 60);
      Graphics_drawLineH(&context, 2, 125, 80);
      Graphics_drawLineH(&context, 2, 125, 100);

      Graphics_drawStringCentered(&context,
                                      (int8_t *)"Thumb",
                                      AUTO_STRING_LENGTH,
                                      56,
                                      30,
                                      OPAQUE_TEXT);

      Graphics_drawStringCentered(&context,
                                    (int8_t *)"Index",
                                    AUTO_STRING_LENGTH,
                                    56,
                                    50,
                                    OPAQUE_TEXT);
      Graphics_drawStringCentered(&context,
                                    (int8_t *)"Middle",
                                    AUTO_STRING_LENGTH,
                                    56,
                                    70,
                                    OPAQUE_TEXT);

      Graphics_drawStringCentered(&context,
                                         (int8_t *)"Ring",
                                         AUTO_STRING_LENGTH,
                                         56,
                                         90,
                                         OPAQUE_TEXT);

      Graphics_drawStringCentered(&context,
                                            (int8_t *)"Pinky",
                                            AUTO_STRING_LENGTH,
                                            56,
                                            110,
                                            OPAQUE_TEXT);
}

void drawVisionMenu()
{
      Graphics_clearDisplay(&context);
      Graphics_drawStringCentered(&context,
                                            (int8_t *)"Vision Menu:",
                                            AUTO_STRING_LENGTH,
                                            64,
                                            10,
                                            TRANSPARENT_TEXT);

      Graphics_drawRectangle(&context, &bigRect);
      Graphics_drawStringCentered(&context,
                                         (int8_t *)"MediaPipe Library",
                                         AUTO_STRING_LENGTH,
                                         70,
                                         30,
                                         OPAQUE_TEXT);
      Graphics_drawStringCentered(&context,
                                              (int8_t *)"OpenCV Library",
                                              AUTO_STRING_LENGTH,
                                              65,
                                              40,
                                              OPAQUE_TEXT);
      Graphics_drawStringCentered(&context,
                                                   (int8_t *)"Check Your PC!",
                                                   AUTO_STRING_LENGTH,
                                                   65,
                                                   60,
                                                   OPAQUE_TEXT);



}

void drawConfirmMenu()
{
    Graphics_clearDisplay(&context);
    Graphics_drawRectangle(&context, &bigRect);
    Graphics_drawStringCentered(&context,
                                            (int8_t *)"Are you sure to quit",
                                            AUTO_STRING_LENGTH,
                                            65,
                                            40,
                                            OPAQUE_TEXT);

    Graphics_drawStringCentered(&context,
                                                    (int8_t *)"Python connection?",
                                                    AUTO_STRING_LENGTH,
                                                    65,
                                                    60,
                                                    OPAQUE_TEXT);
}
