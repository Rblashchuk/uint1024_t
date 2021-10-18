#include <stdio.h>
#include <stdint.h>
#include <memory.h>
#include <string.h>
#include <stdlib.h>

struct uint1024_t
{
    uint32_t a[32];
};

struct uint1024_t from_uint(uint32_t a)
{
    uint32_t ans[32];
    for (int i = 31; i >= 0; i--) ans[i] = 0;
    ans[0] = a;

    struct uint1024_t ans1024 = {*ans};
    memcpy(ans1024.a, ans, 128);
    printf("=====from_uint=====\n");
    for (int i = 31; i >= 0; i--) printf("%x ", ans[i]);
    printf("\n\n");
    return ans1024;
}
struct uint1024_t add_op(struct uint1024_t first, struct uint1024_t second)
{
    short int carryover = 0;
    short int carried;
    uint32_t ans[32];
    for (int i = 0; i < 32; i++){
        carried = 0;
        if(carryover){
            carried = 1;
            carryover = 0;
        }
        ans[i] = first.a[i] + second.a[i];
        if(ans[i] < first.a[i]) {
            carryover = 1;
            if(carried) ans[i]++;
        }
        else {
            if (carried) {
                if (!(ans[i] + 1)) carryover = 1;
                ans[i]++;
            }
        }
    }

    struct uint1024_t ans1024 = {*ans};
    memcpy(ans1024.a, ans, 128);
    return ans1024;

}
void printf_value(struct uint1024_t val) {
    short int flag = 0;

    for (int i = 31; i > 0; i--) {
        if (flag) printf("%08x.", val.a[i]);
        else {
            if (val.a[i]) {
                printf("%x.", val.a[i]);
                flag = 1;
            } else continue;
        }
    }

    if (flag) printf("%08x", val.a[0]);
    else {
        if (val.a[0]) {
            printf("%x", val.a[0]);
            flag = 1;
        }
    }
}

void scanf_valuehex(){
    char line[128];
    scanf("%[^\n]", line);
    for (int i = strlen(line) - 1; i >= 0; i = i - 8) {
        char hexline[8];
        int cnt = 0;
        char *c;
        while (cnt < 8 && line[i + cnt] != *"\0"){
            hexline[7 - cnt] = line[i + cnt];
            cnt--;
        }
        uint32_t hexnum = strtoul(hexline, NULL, 16);
        printf("%08x ", hexnum);
    }
}

int main() {
    scanf_valuehex();
    return 0;
}
