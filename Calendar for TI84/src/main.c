#include "daycalc.h"

#include <ti/getcsc.h>
#include <graphx.h>
#include <keypadc.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/timers.h>
#include <sys/rtc.h>

uint8_t key = 0;

int date[3] = {1, 1, 25};

const uint8_t longDates[] = {1, 3, 5, 7, 8, 10, 12};
const uint8_t shortDates[] = {4, 6, 9, 11};

void gfx_PrintIntXY(int num, int x, int y, int length) {
    gfx_SetTextXY(x,y);
    gfx_PrintInt(num, length);
}

int Start();
void End();
bool Calander();
void Draw(int year, int month);

int main(void) {
    
    //int screen_width = GFX_LCD_WIDTH;
    //int screen_height = GFX_LCD_HEIGHT;

    gfx_Begin();

    gfx_palette[0] = gfx_RGBTo1555(255, 255, 255);
    gfx_palette[1] = gfx_RGBTo1555(0, 0, 0);
    gfx_palette[2] = gfx_RGBTo1555(255, 0, 0);
    gfx_SetTextFGColor(1);  // black
    gfx_SetTextBGColor(0);  // white
    gfx_SetDrawScreen();    // set buffer to draw (screen)
    gfx_FillScreen(0);      // fill screen with white

    int status = Start();
    
    
    if (status == 0) {
        End();
        return 0;
    }
}
    
void End(void)
{
    gfx_palette[0] = gfx_RGBTo1555(0, 0, 0);
    gfx_FillScreen(0);
    gfx_End();
}

int Start(void)
{
    
    uint8_t date1 = 0;
    uint8_t date2 = 0;
    uint16_t date3 = 0;
    
    boot_GetDate(&date1, &date2, &date3); //day month year
    
    Draw(date3, date2);
    bool quit = false;
    quit = Calander();   
    //Reminders();
    if (quit) {
        return 0;
    } else {
        Start();
        return 0;
    }
}

bool Calander() {
    int dayOffset;
    int daysInMonth = 31;
        
    uint8_t date1 = 0;
    uint8_t date2 = 0;
    uint16_t date3 = 0;
        
    int selectWeek = 1;
    int selectDay = 1;
        
    boot_GetDate(&date1, &date2, &date3); //day month year
    
    selectDay = date1;
    
    bool repeat = true;
    
    while (repeat) {
        
        key = os_GetCSC();
        
        if (key != 0) {
            gfx_SetColor(0);
            gfx_Rectangle(106 + 18 * (dayOffset - 1) - 2, 72 + 22 * selectWeek - 3, 19, 13);
            
            if (key == sk_Left) {
                selectDay--;
            }
            if (key == sk_Right) {
                selectDay++;
            }
            if (key == sk_Up) {
                selectDay -= 7;
            }
            if (key == sk_Down) {
                selectDay += 7;
            }
            if (key == sk_Clear) {
                repeat = false;
            }
            
            
            if (selectDay > daysInMonth) {
                date2++;
                selectDay = 1;
                if (date2 > 12) {
                    date3++;
                    date2 = 1;
                }
                Draw(date3, date2);
            }
            if (selectDay < 1) {
                date2--;
                if (date2 < 1) {
                    date3--;
                    date2 = 12;
                }
                if (date2 == 2) {
                    if (date3 % 4 == 0) {
                        daysInMonth = 29;
                    } else {
                        daysInMonth = 28;
                    }
                }
                for (size_t i = 0; i < sizeof(longDates) / sizeof(longDates[0]); i++) {
                    if (longDates[i] == date2) {
                        daysInMonth = 31;
                        break;
                    }
                }
                for (size_t i = 0; i < sizeof(shortDates) / sizeof(shortDates[0]); i++) {
                    if (shortDates[i] == date2) {
                        daysInMonth = 30;
                        break;
                    }
                }
                selectDay = daysInMonth;
                Draw(date3, date2);
            }
        
            if (date2 == 2) {
                if (date3 % 4 == 0) {
                    daysInMonth = 29;
                } else {
                    daysInMonth = 28;
                }
            }
            for (size_t i = 0; i < sizeof(longDates) / sizeof(longDates[0]); i++) {
                if (longDates[i] == date2) {
                    daysInMonth = 31;
                    break;
                }
            }
            for (size_t i = 0; i < sizeof(shortDates) / sizeof(shortDates[0]); i++) {
                if (shortDates[i] == date2) {
                    daysInMonth = 30;
                    break;
                }
            }
            
            dayOffset = dayOfWeek(date3, date2, selectDay);
            
            selectWeek = weekNumber(date3, date2, selectDay) - 2;
            
            gfx_SetColor(1);
            gfx_Rectangle(106 + 18 * (dayOffset - 1) - 2, 72 + 22 * selectWeek - 3, 19, 13);
        }
    }
    
    return true;
}

void Draw(int year, int month) {

    gfx_SetDrawBuffer();

    gfx_ZeroScreen();
    
    uint8_t date1 = 0;
    uint8_t date2 = 0;
    uint16_t date3 = 0;
    
    boot_GetDate(&date1, &date2, &date3); //day month year

    date[0] = date1;
    date[1] = date2;
    date[2] = date3;
    gfx_PrintStringXY(" ",0,0);
    gfx_PrintInt(date[0],2);
    gfx_PrintStringXY(" ",0,8);
    gfx_PrintInt(date[1],2);
    gfx_PrintStringXY(" ",0,16);
    gfx_PrintInt(date[2],2);
    gfx_PrintStringXY(" ",0,24);
    gfx_PrintInt(rtc_Days,2);
    
    int dayOffset = 0;
    int daysInMonth = 30;
    
    int drawWeek = -1;
    int drawDay = 1;
    
    if (month == 2) {
        if (year % 4 == 0) {
            daysInMonth = 29;
        } else {
            daysInMonth = 28;
        }
    }
    for (size_t i = 0; i < sizeof(longDates) / sizeof(longDates[0]); i++) {
        if (longDates[i] == month) {
            daysInMonth = 31;
            break;
        }
    }
    for (size_t i = 0; i < sizeof(shortDates) / sizeof(shortDates[0]); i++) {
        if (shortDates[i] == month) {
            daysInMonth = 30;
            break;
        }
    }

	char monthName[16] = "12";
        int monthNameLength = 0;

	switch (month) {
            case 1:
                strcpy(monthName, "January");
                monthNameLength = 7;
                break;
            case 2:
                strcpy(monthName, "February");
                monthNameLength = 8;
                break;
            case 3:
                strcpy(monthName, "March");
                monthNameLength = 5;
                break;
            case 4:
                strcpy(monthName, "April");
                monthNameLength = 5;
                break;
            case 5:
                strcpy(monthName, "May");
                monthNameLength = 3;
                break;
            case 6:
                strcpy(monthName, "June");
                monthNameLength = 4;
                break;
            case 7:
                strcpy(monthName, "July");
                monthNameLength = 4;
                break;
            case 8:
                strcpy(monthName, "August");
                monthNameLength = 6;
                break;
            case 9:
                strcpy(monthName, "September");
                monthNameLength = 9;
                break;
            case 10:
                strcpy(monthName, "October");
                monthNameLength = 7;
                break;
            case 11:
                strcpy(monthName, "November");
                monthNameLength = 8;
                break;
            case 12:
                strcpy(monthName, "December");
                monthNameLength = 8;
                break;
            default:
                strcpy(monthName, "Err");
                monthNameLength = 3;
                break;
        }
        gfx_PrintStringXY(monthName, 160 - monthNameLength * 6 / 2, 28);
        gfx_PrintIntXY(year, 160 + monthNameLength * 6 / 2 + 16, 28, 4);

    
    while (drawDay != daysInMonth + 1) {
        dayOffset = dayOfWeek(year, month, drawDay);
        if (dayOffset == 1 && drawDay != 1) {
            drawWeek++;
        }
        if (drawDay == date1 && month == date2 && year == date3) {
            gfx_SetTextFGColor(2);
        } else {
            gfx_SetTextFGColor(1);
        }
        gfx_PrintIntXY(drawDay, 106 + 18 * (dayOffset - 1), 72 + 22 * drawWeek, 2);
	
        drawDay++;
    }
    
    gfx_SwapDraw();
    gfx_SetDrawScreen();

    //dayOffset = dayOfWeek(date3, date2, 1);
    //gfx_SetColor(1);
    //gfx_Rectangle(106 + 18 * (dayOffset - 1) - 2, 72 + 22 * selectWeek - 3, 19, 13);
        
}
