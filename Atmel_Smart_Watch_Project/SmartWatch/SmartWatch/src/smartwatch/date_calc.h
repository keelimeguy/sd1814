/* UConn Senior Design Team 1814, November 2017
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

double date_to_day_number(unsigned int y, unsigned int m, unsigned int d) {
    m = (m + 9) % 12;
    double yy = y - m/10;
    return 365*yy + yy/4 - yy/100 + yy/400 + (m*306 + 5)/10 + ( d - 1 );
}

double date_hex_to_day_number(unsigned int da) {
    return date_to_day_number(date_hex_get_year(da), date_hex_get_month(da), date_hex_get_day(da));
}

unsigned int day_number_to_date_hex(double g) {
    int y = (10000*g + 14780)/3652425;
    int ddd = g - (365*y + y/4 - y/100 + y/400);
    if (ddd < 0) {
        y = y - 1;
        ddd = g - (365*y + y/4 - y/100 + y/400);
    }
    int mi = (100*ddd + 52)/3060;
    int mm = (mi + 2)%12 + 1;
    y = y + (mi + 2)/12;
    int dd = ddd - (mi*306 + 5)/10 + 1;
    return (y<<12)|(mm<<8)|(dd);
}

#endif