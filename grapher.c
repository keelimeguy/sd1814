#include <stdio.h>

#define GRAPH_WIDTH 80 // pixels
#define BAR_WIDTH 2.0 // pixels
#define MIN_BAR_HEIGHT 0 // pixels
#define MAX_BAR_HEIGHT 10 // pixels
#define BAR_PADDING 1 // pixels

#define MIN(a, b) (a<b?a:b)
#define MAX(a, b) (a>b?a:b)

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

void graph(double* data, int size, int start, int bar_width) {
    int i, j;
    int bar[size];
    int min = get_min(data, MIN(size, GRAPH_WIDTH/bar_width)) - BAR_PADDING;
    int max = get_max(data, MIN(size, GRAPH_WIDTH/bar_width));

    if (start>size) return;
    for (j=0, i=start; i<size; i++, j++)
        bar[j] = (int)(((data[i]-min)/(max-min))*(double)(MAX_BAR_HEIGHT-MIN_BAR_HEIGHT)) + MIN_BAR_HEIGHT;
    for (j=size-start, i=0; i<start; i++, j++)
        bar[j] = (int)(((data[i]-min)/(max-min))*(double)(MAX_BAR_HEIGHT-MIN_BAR_HEIGHT)) + MIN_BAR_HEIGHT;

    for (i=0; i<size; i++)
        printf("%.1f, ", data[i]);
    printf("\n");

    for (j=MAX_BAR_HEIGHT; j>0; j--) {
        for (i=GRAPH_WIDTH; i>size*bar_width; i--) {
            printf("-");
        }
        for (i=0; i<GRAPH_WIDTH-(GRAPH_WIDTH%bar_width); i++) {
            if ((i/bar_width)>=size) break;
            if (bar[i/bar_width]>=j)
                printf("%c", 219);
            else
                printf("-");
        }
        printf("\n");
    }
}

int main(int args, char** argv) {
    if (args!=2) {
        printf("Usage:\n%s <data_file>", argv[0]);
        return 0;
    }

    FILE *fp;
    double d;
    char c;
    int size = GRAPH_WIDTH/BAR_WIDTH;
    int i = 0, length = 0;
    double data[size];

    fp = fopen(argv[1], "r");
    while (1) {
        printf("Press <ENTER> to continue: ", argv[0]);
        c = fgetc(stdin);
        if (c != '\n') break;

        fscanf(fp, "%lf",&d);
        data[i] = d;
        i++;
        if (length < size) length++;
        if (i >= size) i = 0;

        graph(data, length, length!=size?0:i, BAR_WIDTH);
    }

    fclose(fp);
}
