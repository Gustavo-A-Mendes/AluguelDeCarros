#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./geral/geral.c"

int main(void) {

    // char str1[20] = "Carlos";
    // char str2[20] = "Carla";
    // char str3[20] = "Carlucho";
    // int len = strlen("Car");
    // printf("%d\n", len);
    // printf("%d", strncmp(str1, "Car", len));
    // printf("%d", strncmp(str2, "Car", len));
    // printf("%d", strncmp(str3, "Car", len));
    // char val[41];
    // scanf(" %[^\n]", val);

    // printf("%d", teste_formato(val));

    // time_t ltime = 0;
    // if (time(&ltime) == -1)
    // {
    //     printf("Calendar time not available.\n");
    //     exit(1);
    // }
    // printf("The time is %s\n", ctime(&ltime));

    int data1 = 0;
    char *data2 = "01/01/0002";
    char *data;
    int dat;
    // for (int i = 0; i < 731; i++)
    // {
    //     data = num_para_data(i);
    //     printf("%s\n", data);
    // }
    char c[3];
    int i = 0;
    while ((c[i] = getchar()) != '\n') i++;
    c[i] = '\0';
    printf("%s", c);
    // printf("%d", scanf("%d", &dat));

    // free(data);
    return 0;
}