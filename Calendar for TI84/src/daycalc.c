#include "daycalc.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Return 1 = Sunday, 2 = Monday, 7 = Saturday
//Zeller’s Congruence
int dayOfWeek(int year, int month, int day) {
    int y = year;
    int m = month;
    if (m < 3) {
        m += 12;
        y -= 1;
    }
    int K = y % 100;
    int J = y / 100;
    int h = (day + (13 * (m + 1)) / 5  + K + K/4 + J/4 + 5*J) % 7;
    int dow = ((h + 6) % 7) + 1;  
    return dow;
}

int weekNumber(int year, int month, int day) {
    int first_day = dayOfWeek(year, month, 1) - 1;

    int days_before;
    if (first_day == 0) { 
        days_before = 0;
    } else {
        days_before = first_day;
    }

    //partial first week
    int adjusted_day = day + days_before;

    //Calculate the week number
    return (adjusted_day - 1) / 7 + 1;
}