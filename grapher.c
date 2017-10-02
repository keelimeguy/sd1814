#include <stdio.h>

#define GRAPH_WIDTH 80 // pixels
#define BAR_WIDTH 3.0 // pixels
#define MIN_BAR_HEIGHT 0 // pixels
#define MAX_BAR_HEIGHT 25 // pixels

#define MIN(a, b) (a<b?a:b)

double get_min(double* arr, int size) {
    double min;
    int i;
    for (i=0; i<size; i++) {
        if (i>=GRAPH_WIDTH) break;
        if (i==0 || arr[i]<min) min = arr[i];
    }
    return min;
}

double get_max(double* arr, int size) {
    double max;
    int i;
    for (i=0; i<size; i++)
        if (i==0 || arr[i]>max) max = arr[i];
    return max;
}

void graph(double* data, int size, int bar_width) {
    int i, j;
    int bar[size];
    int min = get_min(data, MIN(size/bar_width, GRAPH_WIDTH/bar_width));
    int max = get_max(data, MIN(size/bar_width, GRAPH_WIDTH/bar_width));
    for (i=0; i<size; i++)
        bar[i] = (int)(((data[i]-min)/(max-min))*(double)(MAX_BAR_HEIGHT-MIN_BAR_HEIGHT)) + MIN_BAR_HEIGHT;

    for (j=MAX_BAR_HEIGHT; j>0; j--) {
        for (i=0; i<GRAPH_WIDTH; i++) {
            if ((i/bar_width)>=size) break;
            if (bar[i/bar_width]>=j && i<GRAPH_WIDTH - (GRAPH_WIDTH%bar_width))
                printf("%c", 219);
            else
                printf(" ");
        }
        printf("\n");
    }
}

int main() {
    int i, size = 200;
    double data[size];
    for (i=1; i<=size; i++) data[i-1] = i*i;
    graph(data, size, BAR_WIDTH);
}
