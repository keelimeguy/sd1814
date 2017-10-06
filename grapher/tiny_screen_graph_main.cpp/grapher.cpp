#include "grapher.h"
#include "util.h"

static double get_min(double* arr, int size);
static double get_max(double* arr, int size);

static double get_min(double* arr, int size) {
    if (size<=0) return 0;

    #ifdef DEFAULT_MIN
        double min = DEFAULT_MIN;
        int i=0;
    #else
        double min = arr[0];
        int i=1;
    #endif

    for (; i<size; i++)
        if (arr[i]<min) min = arr[i];
    return min;
}

static double get_max(double* arr, int size) {
    if (size<=0) return 0;

    #ifdef DEFAULT_MAX
        double max = DEFAULT_MAX;
        int i=0;
    #else
        double max = arr[0];
        int i=1;
    #endif

    for (; i<size; i++)
        if (arr[i]>max) max = arr[i];
    return max;
}

extern "C" void graph(double* data, int size, int start, int bar_width) {
    int i, x, y, index;
    int bar[size];
    int min = get_min(data, MIN(size, GRAPH_WIDTH/bar_width)) - BAR_PADDING_LOW;
    int max = get_max(data, MIN(size, GRAPH_WIDTH/bar_width)) + BAR_PADDING_HIGH;

    #define CONVERT(x) ((int)(((x-min)/(max-min))*(double)(MAX_BAR_HEIGHT-MIN_BAR_HEIGHT)) + MIN_BAR_HEIGHT)

    if (start>size) return;
    for (y=0, i=start; i<size; i++, y++)
        bar[y] = CONVERT(data[i]);
    for (y=size-start, i=0; i<start; i++, y++)
        bar[y] = CONVERT(data[i]);

    GRAPH_RESET();
    for (y=MAX_BAR_HEIGHT; y>0; y--) {
        for (x=0; x<GRAPH_WIDTH-(GRAPH_WIDTH%bar_width)-size*bar_width; x++) {
            GRAPH_PIXEL_OFF(x, y);
        }
        for (x=GRAPH_WIDTH-(GRAPH_WIDTH%bar_width)-size*bar_width, i=0; i<GRAPH_WIDTH-(GRAPH_WIDTH%bar_width); i++, x++) {
            if ((i/bar_width)>=size) break;
            index = (start+i/bar_width)%size;
            if (bar[i/bar_width]>=y)
                if (data[index]>=DANGER_HIGH)
                    GRAPH_PIXEL_DANGER(x, y);
                else if (data[index]>=WARNING_HIGH)
                    GRAPH_PIXEL_WARNING(x, y);
                else if (data[index]<=DANGER_LOW)
                    GRAPH_PIXEL_DANGER(x, y);
                else if (data[index]<=WARNING_LOW)
                    GRAPH_PIXEL_WARNING(x, y);
                else
                    GRAPH_PIXEL_ON(x, y);
            else
                GRAPH_PIXEL_OFF(x, y);
        }
        GRAPH_NEXT_ROW();
    }
}


