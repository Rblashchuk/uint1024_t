#include <stdio.h>
#include <stdint.h>
#include <memory.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

struct uint1024_t {
    uint32_t a[32];
};


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
    uint8_t carried;
    uint32_t ans[32];
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


struct uint1024_t scanf_value_hex() {
    char line[128];
    scanf("%128s", line);
    printf(line);
    uint8_t n = strlen(line);
    uint8_t p = 0;
    int64_t val = 0;
    uint32_t ans[32];
    for (int i = 0; i < 32; i++) {
        ans[i] = 0;
    }
    uint8_t ind_res = 0;

    for (int i = n - 1; i >= 0; i--) {
        char c = line[i];
        if (c - '0' >= 10) {
            val = val + (c - 'a' + 10) * (int) (pow(16, p));
        } else {
            val = val + (c - '0') * (int) (pow(16, p));
        }

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
    printf("=====scanned=====\n");
    for (int i = 31; i >= 0; i--) printf("%x ", ans1024.a[i]);
    printf("\n\n");
    return ans1024;
}


void print_value_hex(struct uint1024_t a) {
    printf("=====print_hex=====\n");
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
        printf("%x", a.a[highest_rank_ind]);

        for (int i = highest_rank_ind - 1; i >= 0; i--) {
            val = a.a[i];
            uint64_t cnt = 0;

            while (val > 0) {
                val = val - (val % 16);
                val = val / 16;
                cnt++;
            }
            for (int j = 8; j > cnt; j--) {
                printf("0");
            }
            printf("%x", a.a[i]);
        }
    } else {
        printf("0");
    }
}


int main() {
    struct uint1024_t a = scanf_value_hex();
    struct uint1024_t b = scanf_value_hex();
    struct uint1024_t c = subtr_op(a, b);
    print_value_hex(c);
    return 0;
}