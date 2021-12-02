#include <stdio.h>
#include <stdint.h>
#include <memory.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

struct uint1024_t {
    uint32_t a[32];
};


int hex_digit_to_dec(char digit){
    if (digit - '0' >= 10) {
        return (digit - 'a' + 10);
    } else {
        return(digit - '0');
    }
}


char dec_to_hex_digit(int val){
    if (val < 10){
        return((char)(val + '0'));
    }
    else{
        return ((char)(val - 10 + 'a'));
    }
}


void to_string(char ans[128], struct uint1024_t a){
    uint64_t rank_val = 0;
    int8_t highest_rank_ind = -1;

    for (int i = 31; i >= 0; i--) {
        rank_val = a.a[i];
        if (rank_val) {
            highest_rank_ind = i;
            break;
        }
    }

    if (highest_rank_ind + 1) {
        uint64_t val = 0;
        char s[128] = "";
        sprintf(s, "%x", a.a[highest_rank_ind]);
        strcat(ans, s);



        for (int i = highest_rank_ind - 1; i >= 0; i--) {
            val = a.a[i];
            uint64_t cnt = 0;

            while (val > 0) {
                val = val - (val % 16);
                val = val / 16;
                cnt++;
            }
            for (int j = 8; j > cnt; j--) {
                strcat(ans, "0");
            }
            char s[128] = "";
            sprintf(s, "%x", a.a[i]);
            strcat(ans, s);
        }
    } else {
        strcat(ans, "0");
    }
}


struct uint1024_t from_string(char str[128]){
    uint8_t n = strlen(str);
    uint8_t p = 0;
    int64_t val = 0;
    uint32_t ans[32];

    for (int i = 0; i < 32; i++) {
        ans[i] = 0;
    }
    uint8_t ind_res = 0;

    for (int i = n - 1; i >= 0; i--) {
        char c = str[i];

        val = val + hex_digit_to_dec(c) * (int)(pow(16, p));

        p++;
        if (p == 8) {
            p = 0;
            ans[ind_res] = val;
            val = 0;
            ind_res++;
        }
    }
    ans[ind_res] = val;

    struct uint1024_t ans1024 = {*ans};
    memcpy(ans1024.a, ans, 128);
    return ans1024;
}


struct uint1024_t from_uint(uint32_t a) {
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


int comp(struct uint1024_t first, struct uint1024_t second){
    int ans = 1;
    for (int i = 0; i < 32; i++){
        if (first.a[i] == second.a[i]){
            continue;
        }
        else{
            ans = 0;
            break;
        }
    }
    return ans;
}

struct uint1024_t add_op(struct uint1024_t first, struct uint1024_t second) {
    uint8_t carryover = 0;
    uint8_t carried;
    uint32_t ans[32];
    for (int i = 0; i < 32; i++) {
        carried = 0;
        if (carryover) {
            carried = 1;
            carryover = 0;
        }
        ans[i] = first.a[i] + second.a[i];
        if (ans[i] < first.a[i]) {
            carryover = 1;
            if (carried) ans[i]++;
        } else {
            if (carried) {
                if (!(ans[i] + 1)) carryover = 1;
                ans[i]++;
            }
        }
    }

    struct uint1024_t ans1024 = {*ans};
    memcpy(ans1024.a, ans, 128);
    printf("=====add_op=====\n");
    for (int i = 31; i >= 0; i--) printf("%x ", ans1024.a[i]);
    printf("\n\n");
    return ans1024;
}


struct uint1024_t subtr_op(struct uint1024_t first, struct uint1024_t second) {
    uint8_t carryover = 0;
    uint8_t carried = 0;
    uint32_t ans[32];
    for (int i = 0; i < 32; i++){
        ans[i] = 0;
    }

    for (int i = 0; i < 32; i++) {
        carried = 0;
        if (carryover) {
            carried = 1;
            carryover = 0;
        }
        ans[i] = first.a[i] - second.a[i];
        if (ans[i] > first.a[i]) {
            carryover = 1;
            if (carried) ans[i]--;
        } else {
            if (carried) {
                if (!ans[i]) carryover = 1;
                ans[i]--;
            }
        }
    }
    struct uint1024_t ans1024 = {*ans};
    memcpy(ans1024.a, ans, 128);
    printf("=====subtr_op=====\n");
    for (int i = 31; i >= 0; i--) printf("%x ", ans1024.a[i]);
    printf("\n\n");
    return ans1024;
}


struct uint1024_t mult_op(struct uint1024_t first, struct uint1024_t second){

    char str1[128];
    char str2[128];
    int8_t ans[128];
    for (int i = 0; i < 128; i++){
        ans[i] = 0;
        str1[i] = 0;
        str2[i] = 0;
    }
    to_string(str1, first);
    to_string(str2, second);
    uint8_t len1 = strlen(str1);
    uint8_t len2 = strlen(str2);

    int val = 0;
    int carryover = 0;
    int8_t ans_ind = 0;
    int8_t rank_cnt = -1;

    for (int i = len1 - 1; i >= 0; i--){
        rank_cnt++;
        ans_ind = rank_cnt;

        for (int j = len2 - 1; j >= 0; j--){
            char dig1 = str1[i];
            char dig2 = str2[j];
            val = hex_digit_to_dec(dig1) * hex_digit_to_dec(dig2) + carryover;
            carryover = 0;

            if (val >= 16){
                carryover = ((val - val % 16) / 16);
            }

            ans[ans_ind] = ans[ans_ind] + val % 16;

            if(ans[ans_ind] >= 16){
                carryover++;
                ans[ans_ind] = ans[ans_ind] % 16;
            }

            ans_ind++;
        }
        ans[ans_ind] = ans[ans_ind] + carryover;
        carryover = 0;
    }

    char ans_str[129];
    for (int i = 0; i < 128; i++){
        ans_str[i] = 0;
    }
    for (int i = 0; i < 128; i++){
        ans_str[i] = dec_to_hex_digit(ans[127 - i]);
    }
    ans_str[128] = '\0';

    struct uint1024_t ans1024 = from_string(ans_str);

    printf("=====multiplied=====\n");
    for(int i = 31; i >= 0; i--){
        printf("%x ", ans1024.a[i]);
    }
    printf("\n");
    return (ans1024);
}


struct uint1024_t scan_value_hex() {
    char line[128];
    scanf("%128s", line);
    printf(line);

    struct uint1024_t ans1024 = from_string(line);
    printf("=====scanned=====\n");
    for (int i = 31; i >= 0; i--) printf("%x ", ans1024.a[i]);
    printf("\n\n");
    return ans1024;
}


void print_value_hex(struct uint1024_t a) {
    char ans[128] = "";
    to_string(ans, a);
    printf("%s\n", ans);
}

/*
int main() {
    char request[16];
    while(request[0] - '0'){

        printf("\nrequest:\n");
        scanf("%16s", request);

        if (request[0] == 'a'){
            printf("input two uint1024_t strings\n");
            struct uint1024_t a;
            struct uint1024_t b;
            struct uint1024_t sum;
            a = scan_value_hex();
            b = scan_value_hex();
            sum = add_op(a,b);

            printf("response: ");
            print_value_hex(sum);
            printf("______________\n");
            continue;
        }

        if (request[0] == 's'){
            printf("input two uint1024_t strings\n");
            struct uint1024_t a;
            struct uint1024_t b;
            struct uint1024_t diff;
            a = scan_value_hex();
            b = scan_value_hex();
            diff = subtr_op(a,b);

            printf("response: ");
            print_value_hex(diff);
            printf("______________\n");
            continue;
        }

        if (request[0] == 'm'){
            printf("input two uint1024_t strings\n");
            struct uint1024_t a;
            struct uint1024_t b;
            struct uint1024_t product;
            a = scan_value_hex();
            b = scan_value_hex();
            product = mult_op(a,b);

            printf("response: ");
            print_value_hex(product);
            printf("______________\n");
            continue;
        }

        if (request[0] == 'f'){
            printf("input uint32_t\n");
            uint32_t a;
            scanf("%d", &a);
            struct uint1024_t res = from_uint(a);

            printf("response: ");
            print_value_hex(res);
            printf("______________\n");
            continue;
        }
        printf("unknown request.\n");
    }
}
*/