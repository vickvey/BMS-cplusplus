#include <stdio.h>

int main() {
    // code here
    char s[1001];
    scanf("%s", s);

    int size = 0;
    for(int i = 0; i<1001; i++) {
        if(s[i] == '\0') {
            size = i;
            break;
        }
    }
    printf("%d", size);
    return 0;
}