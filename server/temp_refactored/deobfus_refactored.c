#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#ifndef DICT
#define DICT "dict"
#endif

#define X 0

struct a {
    char *w, *c, *s, *x, *t, *u, **h, *S, *N, *A, *(*v[9])(struct a *), *y;
    struct a *e, *a, *z, *k, *j;
    int l;
} *f, *c;

char *B[][2] = {
    {"A", "WOL-LA-CHEE"}, {"B", "SHUSH"}, {"C", "MOASI"}, {"D", "CHINDI"},
    {"E", "DZEH"}, {"F", "CHUO"}, {"G", "JEHA"}, {"H", "TSE-GAH"},
    {"I", "YEH-HES"}, {"J", "AH-YA-TSINNE"}, {"K", "BA-AH-NE-DI-TININ"}, {"L", "NASH-DOIE-TSO"},
    {"M", "BE-TAS-TNI"}, {"N", "TSAH"}, {"O", "A-KHA"}, {"P", "CLA-GI-AIH"},
    {"Q", "CA-YEILTH"}, {"R", "DAH-NES-TSA"}, {"S", "KLESH"}, {"T", "D-AH"},
    {"U", "NO-DA-IH"}, {"V", "A-KEH-DI-GLINI"}, {"W", "GLOE-IH"},
    {"X", "AL-NA-AS-DZOH"}, {"Y", "TSAH-AS-ZIH"}, {"Z", "BESH-DO-TLIZ"},
}, *Y(struct a *);

int z, J, C, k, H, D, s, L;
size_t n, o;
FILE *g;

char *I(struct a *a) {
    for (a->a = a->z, z = 0; a->a; a->a = a->a->e) {
        if (a->v[2](&(struct a){.w = a->a->w, .c = a->x})) {
            while (!isalpha(*a->x) && *(a->x)) {
                L = putchar(*a->x);
                ++a->x;
            }
            while ((z++)[a->a->c]) {
                s = z - 1;
                L = putchar(!s && isupper(a->x[0]) ? a->a->c[s] : tolower(a->a->c[s]));
            }
            goto output_space;
        }
    }

    for (z = 0, s = 1; z[a->x]; ++z) {
        if (!isalpha(z[a->x]))
            L = putchar(z[a->x]);
        else {
            C = islower(z[a->x]);
            for (J = 0; J < 26 && B[J][X ? 0 : 1][0]; ++J) {
                if (*B[J][0] == toupper(z[a->x])) {
                    for (k = 0; (H = B[J][X ? 1 : 0][k]); ++k)
                        putchar(C ? tolower(H) : toupper(H));
                    L = putchar(' ');
                    break;
                }
            }
        }
    }

output_space:
    if (!isspace(L))
        putchar(' ');
    return a->s;
}

char *Z(struct a *a) {
    while (a->h[a->l++]) {
        if ((*a->h[--a->l] == '+') &&
            (*a->v)(&(struct a){.v[0] = I, .s = a->l[a->h] + 1, .j = a->k}) &&
            ++a->l) ||
            (a->v[1](&(struct a){.v[2] = Y, .x = a->l[a->h], .z = a->j}), ++a->l);
    }
    return "";
}

char *w(struct a *a) {
    for (D = 0; a && a->s && a->s[D] && (a->s[D] = isupper(a->s[D]) ? "ZYXWVUTSRQPONMLKJIHGFEDCBA"[a->s[D] - 'A'] : a->s[D]); ++D);
    return a ? a->s : "";
}

char *Y(struct a *a) {
    if (*a->w && *a->c && a->w[1] && a->c[1]) {
        while (*a->w && *a->c) {
            while (!isalpha(*a->w) && *a->w++);
            while (!isalpha(*a->c) && *a->c++);
            if (!*a->c || !*a->w || tolower(*a->w) != tolower(*a->c))
                break;
            ++a->w;
            ++a->c;
        }
    }
    return !*a->w && !*a->c ? "" : 0;
}

char *F(struct a *a) {
    g = fopen(DICT, "r");
    if (g) {
        while (getline(&a->A, &n, g) != -1) {
            if ((a->N = strtok(a->A, "\t")) && (a->S = strtok(0, "\t"))) {
                c = calloc(1, sizeof *c);
                c->w = strdup(X ? a->v[0](&(struct a){.s = a->S}) : a->v[0](&(struct a){.s = a->N}));
                c->c = strdup(X ? a->v[1](&(struct a){.s = a->N}) : a->v[0](&(struct a){.s = a->S}));
                c->e = f;
                f = c;
            }
            free(a->A);
            a->A = 0;
        }
        fclose(g);
        g = 0;
    } else {
        puts("couldn't open dict");
    }
    return "";
}

char *t(struct a *a) {
    o = 0;
    g = fopen(a->s, "r");
    a->A = 0;
    while (g && getline(&a->A, &o, g) != -1) {
        a->S = strtok(a->A, " \t");
        while (a->S) {
            if ((a->y = strdup(a->S))) {
                (*a->v)(&(struct a){.v[2] = Y, .x = a->y});
                a->S = strtok(0, " \t");
                free(a->y);
                a->y = 0;
                if (L != '\n')
                    putchar(' ');
            }
        }
    }
    if (g) {
        clearerr(g);
        fclose(g);
        g = 0;
        free(a->A);
        a->A = 0;
    }
    return "";
}

int main(int c, char **a) {
    F((c = ((113), c &= 9, &(struct a){.l = 1, .s = a[c], .v = {&w, &w}})));
    Z(&(struct a){.v = {t, I}, .k = 0, .l = c, .h = a, .j = f});
    puts("");
    return 0;
}