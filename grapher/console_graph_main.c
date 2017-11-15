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
    int diff = 1, k = 0, i = 0;

    if (args==2)
        fp = fopen(argv[1], "r");

    while (1) {
        printf("Press <ENTER> to continue: ");
        c = fgetc(stdin);
        if (c != '\n') break;

        if (args!=2) {
            d = k*i;
            k+=diff;
            if (k*i>DANGER_HIGH || k*i<DANGER_LOW) {
                diff = -diff;
                k/=i/2;
            }
        } else
            fscanf(fp, "%lf",&d);

        i = add_to_graph(d);
        graph(0);
    }

    if (args==2)
        fclose(fp);
}
