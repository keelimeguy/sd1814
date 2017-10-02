#include <stdio.h>

#define GRAPH_WIDTH 80 // pixels
#define MIN_BAR_HEIGHT 0 // pixels
#define MAX_BAR_HEIGHT 25 // pixels

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
    for (i=0; i<size; i++) {
        if (i>=GRAPH_WIDTH) break;
        if (i==0 || arr[i]>max) max = arr[i];
    }
    return max;
}

void graph(double* data, int size) {
    int i, j;
    int bar[size];
    int min = get_min(data, size);
    int max = get_max(data, size);
    for (i=0; i<size; i++)
        bar[i] = (int)(((data[i]-min)/(max-min))*(double)(MAX_BAR_HEIGHT-MIN_BAR_HEIGHT)) + MIN_BAR_HEIGHT;

    for (j=MAX_BAR_HEIGHT; j>0; j--) {
        for (i=0; i<GRAPH_WIDTH; i++) {
            if (bar[i]>=j)
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
    graph(data, size);
}
