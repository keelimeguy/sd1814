/* UConn Senior Design Team 1814, January 2018
*/

#ifndef DATE_CALC_H
#define DATE_CALC_H

// https://alcor.concordia.ca/~gpkatch/gdate-algorithm.html

#define SECONDS_IN_A_DAY 86400UL
#define seconds_to_days(seconds) ((seconds)/SECONDS_IN_A_DAY)
#define days_to_seconds(days) ((days)*SECONDS_IN_A_DAY)
#define date_hex_get_year(hx) ((hx)>>12)
#define date_hex_get_month(hx) (((hx)>>8)&0xf)
#define date_hex_get_day(hx) ((hx)&0xff)

static double date_to_day_number(unsigned int y, unsigned int m, unsigned int d) {
    m = (m + 9) % 12;
    double yy = y - m/10;
    return 365*yy + yy/4 - yy/100 + yy/400 + (m*306 + 5)/10 + ( d - 1 );
}

// static double date_hex_to_day_number(unsigned int da) {
//     return date_to_day_number(date_hex_get_year(da), date_hex_get_month(da), date_hex_get_day(da));
// }

static void day_number_to_date_uchar(double g, short unsigned int *y, unsigned char *m, unsigned char *d) {
    unsigned int uiy = (10000*g + 14780)/3652425;
    int ddd = g - (365*uiy + uiy/4 - uiy/100 + uiy/400);
    if (ddd < 0) {
        uiy = uiy - 1;
        ddd = g - (365*uiy + uiy/4 - uiy/100 + uiy/400);
    }
    int mi = (100*ddd + 52)/3060;
    *m = (unsigned char)((mi + 2)%12 + 1);
    *y = (short unsigned int)(uiy + (mi + 2)/12);
    *d = (unsigned char)(ddd - (mi*306 + 5)/10 + 1);
}

// static void day_number_to_date(double g, unsigned int *y, unsigned int *m, unsigned int *d) {
//     *y = (10000*g + 14780)/3652425;
//     int ddd = g - (365*(*y) + (*y)/4 - (*y)/100 + (*y)/400);
//     if (ddd < 0) {
//         *y = *y - 1;
//         ddd = g - (365*(*y) + (*y)/4 - (*y)/100 + (*y)/400);
//     }
//     int mi = (100*ddd + 52)/3060;
//     *m = (mi + 2)%12 + 1;
//     *y = *y + (mi + 2)/12;
//     *d = ddd - (mi*306 + 5)/10 + 1;
// }

// static unsigned int day_number_to_date_hex(double g) {
//     int y = (10000*g + 14780)/3652425;
//     int ddd = g - (365*y + y/4 - y/100 + y/400);
//     if (ddd < 0) {
//         y = y - 1;
//         ddd = g - (365*y + y/4 - y/100 + y/400);
//     }
//     int mi = (100*ddd + 52)/3060;
//     int mm = (mi + 2)%12 + 1;
//     y = y + (mi + 2)/12;
//     int dd = ddd - (mi*306 + 5)/10 + 1;
//     return (y<<12)|(mm<<8)|(dd);
// }

// static void add_to_date(unsigned int days, unsigned int *y, unsigned int *m, unsigned int *d) {
//     double g = date_to_day_number(*y, *m, *d) + (double)days;
//     day_number_to_date(g, y, m, d);
// }

static void add_to_date_uchar(unsigned int days, short unsigned int *y, unsigned char *m, unsigned char *d) {
    double g = date_to_day_number(*y, *m, *d) + (double)days;
    day_number_to_date_uchar(g, y, m, d);
}

inline int get_default_time_second(void) {
    char time[] = __TIME__; // Format: "hh:mm:ss"
    int d1 = (int)(time[7]-'0');
    int d10 = (int)(time[6]-'0');
    return d1+10*d10;
}

inline int get_default_time_minute(void) {
    char time[] = __TIME__; // Format: "hh:mm:ss"
    int d1 = (int)(time[4]-'0');
    int d10 = (int)(time[3]-'0');
    return d1+10*d10;
}

inline int get_default_time_hour(void) {
    char time[] = __TIME__; // Format: "hh:mm:ss"
    int d1 = (int)(time[1]-'0');
    int d10 = (int)(time[0]-'0');
    int hr = d1+10*d10;
    if (hr > 12) return hr-12;
    if (hr == 0) return 12;
    return hr;
}

inline int get_default_time_pm(void) {
    char time[] = __TIME__; // Format: "hh:mm:ss"
    int d1 = (int)(time[1]-'0');
    int d10 = (int)(time[0]-'0');
    int hr = d1+10*d10;
    if (hr == 24) return 0;
    if (hr >= 12) return 1;
    return 0;
}

inline int get_default_time_year(void) {
    char date[] = __DATE__; // Format: "Mmm dd yyyy"
    int d1 = (int)(date[10]-'0');
    int d10 = (int)(date[9]-'0');
    int d100 = (int)(date[8]-'0');
    int d1000 = (int)(date[7]-'0');
    return d1+10*d10+100*d100+1000*d1000;
}

inline int get_default_time_day(void) {
    char date[] = __DATE__; // Format: "Mmm dd yyyy"
    int d1 = (int)(date[5]-'0');
    int d10 = (int)(date[4]-'0');
    if (d10>0)
        return d1+10*d10;
    return d1;
}

inline int get_default_time_month(void) {
    char date[] = __DATE__; // Format: "Mmm dd yyyy"
    // "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", and "Dec"
    switch (date[0]) {
        case 'J':
            if (date[1] == 'a') return 1;
            if (date[1] == 'n') return 6;
            return 7;

        case 'F':
            return 2;

        case 'M':
            if (date[2] == 'r') return 3;
            return 5;

        case 'A':
            if (date[1] == 'p') return 4;
            return 8;

        case 'S':
            return 9;

        case 'O':
            return 10;

        case 'N':
            return 11;

        case 'D':
            return 12;
    }
    return 0;
}

#endif
