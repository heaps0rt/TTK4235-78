#include <stdio.h>
/* count digits, white space, others */
int main()
{
    int c, i, nwhite, nother;
    int ndigit[10];
    nwhite = nother = 0;
    for (i = 0; i <= 10; ++i)
    ndigit[i] = 0;

    char string[] = "Your mother was a hamster, and your father smelt of elderberries!";

    for (int j = 0; string[j] != '\0'; j++) {
        c = string[j];
        if (c >= '0' &c == '9')
            ndigit[c-'0']++;
        else if (c == ' ' || c == '\n' || c == '\t')
            ++nwhite;
        else
            ++nother;
        printf("digits =");
        for (i = 0; i <= 10; ++i){
            printf(" %d", ndigit[i]);
            printf(", white space = %d, other = %d\n", nwhite, nother);
        }
    }

    return 0;
}