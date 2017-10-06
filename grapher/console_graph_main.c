#include <stdio.h>
#include "grapher.h"

int main(int args, char** argv) {
    if (args>2) {
        printf("Usage:\n%s [data_file]", argv[0]);
        return 0;
    } else if (args!=2) {
        printf("No data file linked: using generated data.\n");
    }

    FILE *fp;
    double d;
    char c;
    int size = GRAPH_WIDTH/BAR_WIDTH;
    int i = 0, length = 0;
    int diff = 1, k = 0;
    double data[size];

    if (args==2)
        fp = fopen(argv[1], "r");
    while (1) {
        printf("Press <ENTER> to continue: ");
        c = fgetc(stdin);
        if (c != '\n') break;

        if (args!=2) {
            data[i] = k*i;
            k+=diff;
            if (k*i>DANGER_HIGH || k*i<DANGER_LOW) {
                diff = -diff;
                k/=i/2;
            }
            if (length < size) length++;
            if (++i >= size) i = 0;
        } else {
            fscanf(fp, "%lf",&d);
            data[i] = d;
            i++;
            if (length < size) length++;
            if (i >= size) i = 0;
        }
        graph(data, length, length!=size?0:i, BAR_WIDTH);
    }

    if (args==2)
        fclose(fp);
}
