#include "grapher.h"
#include "util.h"

static double get_min(int size);
static double get_max(int size);
static double min = 0;
static double max = 0;

const int data_size = GRAPH_WIDTH/BAR_WIDTH;
int data_index = 0, data_length = 0, data_start = 0;
double data[data_size];

double graph_min() {
    return min + BAR_PADDING_LOW;
}

double graph_max() {
    return max - BAR_PADDING_HIGH;
}

static double get_min(int size) {
    if (size<=0) return 0;

    #ifdef DEFAULT_MIN
        double min = DEFAULT_MIN;
        int i=0;
    #else
        double min = data[0];
        int i=1;
    #endif

    for (; i<size; i++)
        if (data[i]<min) min = data[i];
    return min;
}

static double get_max(int size) {
    if (size<=0) return 0;

    #ifdef DEFAULT_MAX
        double max = DEFAULT_MAX;
        int i=0;
    #else
        double max = data[0];
        int i=1;
    #endif

    for (; i<size; i++)
        if (data[i]>max) max = data[i];
    return max;
}

extern "C" int add_to_graph(int val) {
    data[data_index] = val;
    data_index++;
    if (data_index >= data_size) data_index = 0;
    if (data_length < data_size) {
        data_length++;
        data_start = 0;
    } else
        data_start = data_index;

    int i;
    for (i=0;i<data_size;i++)
        printf("%2.2f, ", data[i]);
    printf("\n");

    return data_index;
}

extern "C" void graph(int clear) {
    int i, x, y, index;
    int bar[data_length];
    min = get_min(data_length) - BAR_PADDING_LOW;
    max = get_max(data_length) + BAR_PADDING_HIGH;

    #define CONVERT(xx) ((int)(((xx-min)/(max-min))*(double)(MAX_BAR_HEIGHT-MIN_BAR_HEIGHT)) + MIN_BAR_HEIGHT)

    if (data_start>data_length) return;
    for (y=0, i=data_start; i<data_length; i++, y++)
        bar[y] = CONVERT(data[i]);
    for (y=data_length-data_start, i=0; i<data_start; i++, y++)
        bar[y] = CONVERT(data[i]);

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
