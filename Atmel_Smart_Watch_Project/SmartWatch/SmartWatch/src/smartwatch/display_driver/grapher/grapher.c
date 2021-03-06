/* UConn Senior Design Team 1814, January 2018
*/

#include "grapher.h"
#include "util.h"

static int get_min(int size);
static int get_max(int size);

#ifdef DEFAULT_MIN
    static int data_min = DEFAULT_MIN;
#else
    static int data_min = 0;
#endif
static int min_index = 0;

#ifdef DEFAULT_MAX
    static int data_max = DEFAULT_MAX;
#else
    static int data_max = 0;
#endif
static int max_index = 0;

const int data_size = GRAPH_WIDTH/BAR_WIDTH;
static int data_index = 0, data_length = 0, data_start = 0;
static int data[GRAPH_WIDTH/BAR_WIDTH];
static short bar_cache[GRAPH_WIDTH/BAR_WIDTH];
static char is_changed = 0;

int graph_get_recent_trend(void) {
    if (data_length<2) return 0;
    int idx1 = (data_start+data_length-1)%data_size;
    int idx2 = (data_start+data_length-2)%data_size;
    if (bar_cache[idx1] > bar_cache[idx2]) return 1;
    if (bar_cache[idx1] < bar_cache[idx2]) return -1;
    return 0;
}

unsigned char graph_changed(void) {
    return is_changed;
}

int graph_length(void){
    return data_length;
}

int graph_min(void) {
    return data_min;
}

int graph_max(void) {
    return data_max;
}

static int get_min(int size) {
    if (size<=0) return 0;

    #ifdef DEFAULT_MIN
        int min = DEFAULT_MIN;
        min_index = -1;
        int i=0;
    #else
        int min = data[0];
        min_index = 0;
        int i=1;
    #endif

    for (; i<size; i++)
        if (data[i]<min) {
            min = data[i];
            min_index = i;
        }
    return min;
}

static int get_max(int size) {
    if (size<=0) return 0;

    #ifdef DEFAULT_MAX
        int max = DEFAULT_MAX;
        max_index = -1;
        int i=0;
    #else
        int max = data[0];
        max_index = 0;
        int i=1;
    #endif

    for (; i<size; i++)
        if (data[i]>max) {
            max = data[i];
            max_index = i;
        }
    return max;
}

void reset_graph(void) {
    is_changed = 1;
    data_index = 0;
    data_length = 0;
    data_start = 0;

    #ifdef DEFAULT_MIN
        data_min = DEFAULT_MIN;
    #else
        data_min = 0;
    #endif

    #ifdef DEFAULT_MAX
        data_max = DEFAULT_MAX;
    #else
        data_max = 0;
    #endif

    min_index = 0;
    max_index = 0;
}

int add_to_graph(int val) {
    int refresh_axis = 0;
    is_changed = 1;
    data[data_index] = val;
    if (data_length == 0 || val<data_min) {
        data_min = val;
        min_index = data_index;
        refresh_axis = 1;
    } else if (min_index == data_index) {
        data_min = get_min(data_length);
        refresh_axis = 1;
    }

    if (data_length == 0 || val>data_max) {
        data_max = val;
        max_index = data_index;
        refresh_axis = 1;
    } else if (max_index == data_index) {
        data_max = get_max(data_length);
        refresh_axis = 1;
    }

    int min = data_min - BAR_PADDING_LOW;
    int max = data_max + BAR_PADDING_HIGH;

    #define CONVERT(xx) ((short)(((float)(xx-min)/(float)(max-min))*(float)(MAX_BAR_HEIGHT-MIN_BAR_HEIGHT)) + MIN_BAR_HEIGHT)
    if (refresh_axis) {
        for (int i = 0; i <= data_length; i++)
            bar_cache[i] = CONVERT(data[i]);

    } else {
        bar_cache[data_index] = CONVERT(val);
    }

    data_index++;
    if (data_index >= data_size) data_index = 0;
    if (data_length < data_size) {
        data_length++;
        data_start = 0;
    } else
        data_start = data_index;

    return data_index;
}

void graph(int clear) {
    int i, x, y, index;
    short bar[data_length];

    if (data_start>data_length) return;
    for (y=0, i=data_start; i<data_length; i++, y++)
        bar[y] = bar_cache[i];
    for (y=data_length-data_start, i=0; i<data_start; i++, y++)
        bar[y] = bar_cache[i];

    if (clear)
        GRAPH_RESET();
    for (y=MAX_BAR_HEIGHT; y>0; y--) {
        for (x=0; x<GRAPH_WIDTH-(GRAPH_WIDTH%BAR_WIDTH)-data_length*BAR_WIDTH; x++) {
            GRAPH_PIXEL_OFF(x, y);
        }
        for (x=GRAPH_WIDTH-(GRAPH_WIDTH%BAR_WIDTH)-data_length*BAR_WIDTH, i=0; i<GRAPH_WIDTH-(GRAPH_WIDTH%BAR_WIDTH); i++, x++) {
            if ((i/BAR_WIDTH)>=data_length) break;
            index = (data_start+i/BAR_WIDTH)%data_length;
            if (bar[i/BAR_WIDTH]>=y)
                if (data[index]>=DISP_DANGER_HIGH)
                    GRAPH_PIXEL_DANGER(x, y);
                else if (data[index]>=DISP_WARNING_HIGH)
                    GRAPH_PIXEL_WARNING(x, y);
                else if (data[index]<=DISP_DANGER_LOW)
                    GRAPH_PIXEL_DANGER(x, y);
                else if (data[index]<=DISP_WARNING_LOW)
                    GRAPH_PIXEL_WARNING(x, y);
                else
                    GRAPH_PIXEL_ON(x, y);
            else
                GRAPH_PIXEL_OFF(x, y);
        }
    }
    is_changed = 0;
}

void graph_smart_sizing(unsigned short* xret, unsigned short* yret, unsigned short* widthret, unsigned short* heightret) {
    int i, index;
    short x, y;
    short bar[data_length];

    *xret = 0;
    *yret = 0;
    *widthret = 0;
    *heightret = 0;

    if (data_start>data_length) return;
    for (y=0, i=data_start; i<data_length; i++, y++)
        bar[y] = bar_cache[i];
    for (y=data_length-data_start, i=0; i<data_start; i++, y++)
        bar[y] = bar_cache[i];

    *xret = DISP_GRAPH_X + GRAPH_WIDTH-(GRAPH_WIDTH%BAR_WIDTH)-data_length*BAR_WIDTH;
    *widthret = GRAPH_WIDTH-(GRAPH_WIDTH%BAR_WIDTH);
    for (x=*xret-DISP_GRAPH_X, i=0; i<*widthret; i++, x++) {
        if ((i/BAR_WIDTH)>=data_length) {
            *widthret = i;
            break;
        }
        for (y=0; y<bar[i/BAR_WIDTH]; y++) {
            index = (data_start+i/BAR_WIDTH)%data_length;
            if (data[index]>=DISP_DANGER_HIGH)
                GRAPH_PIXEL_DANGER(x, y);
            else if (data[index]>=DISP_WARNING_HIGH)
                GRAPH_PIXEL_WARNING(x, y);
            else if (data[index]<=DISP_DANGER_LOW)
                GRAPH_PIXEL_DANGER(x, y);
            else if (data[index]<=DISP_WARNING_LOW)
                GRAPH_PIXEL_WARNING(x, y);
            else
                GRAPH_PIXEL_ON(x, y);
        }
        if (bar[i/BAR_WIDTH] > (*heightret))
            *heightret = bar[i/BAR_WIDTH];
    }

    *yret = DISP_GRAPH_Y+DISP_GRAPH_HEIGHT-1-*heightret;
    *heightret = *heightret + 1;
    is_changed = 0;
}
