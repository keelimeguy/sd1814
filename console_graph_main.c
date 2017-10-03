#include <stdio.h>
#include "grapher.h"

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
