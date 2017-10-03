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

void graph(double* data, int size, int start, int bar_width) {
    int i, j, index;
    int bar[size];
    int min = get_min(data, MIN(size, GRAPH_WIDTH/bar_width)) - BAR_PADDING_LOW;
    int max = get_max(data, MIN(size, GRAPH_WIDTH/bar_width)) + BAR_PADDING_HIGH;

    #define CONVERT(x) ((int)(((x-min)/(max-min))*(double)(MAX_BAR_HEIGHT-MIN_BAR_HEIGHT)) + MIN_BAR_HEIGHT)

    if (start>size) return;
    for (j=0, i=start; i<size; i++, j++)
        bar[j] = CONVERT(data[i]);
    for (j=size-start, i=0; i<start; i++, j++)
        bar[j] = CONVERT(data[i]);

    GRAPH_RESET();
    for (j=MAX_BAR_HEIGHT; j>0; j--) {
        for (i=GRAPH_WIDTH; i>size*bar_width; i--) {
            GRAPH_PIXEL_OFF(i, j);
        }
        for (i=0; i<GRAPH_WIDTH-(GRAPH_WIDTH%bar_width); i++) {
            if ((i/bar_width)>=size) break;
            index = (start+i/bar_width)%size;
            if (bar[i/bar_width]>=j)
                if (data[index]>=DANGER_HIGH)
                    GRAPH_PIXEL_DANGER(i, j);
                else if (data[index]>=WARNING_HIGH)
                    GRAPH_PIXEL_WARNING(i, j);
                else if (data[index]<=DANGER_LOW)
                    GRAPH_PIXEL_DANGER(i, j);
                else if (data[index]<=WARNING_LOW)
                    GRAPH_PIXEL_WARNING(i, j);
                else
                    GRAPH_PIXEL_ON(i, j);
            else
                GRAPH_PIXEL_OFF(i, j);
        }
        GRAPH_NEXT_ROW();
    }
}
