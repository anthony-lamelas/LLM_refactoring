#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

typedef void q;
typedef int _;
typedef char p;

// Structure to hold various character arrays and counters
struct {
    p a[257], b[9][5], c[5], d[5];
    _ e, f;
} i;

p t[78557][5];
_ s, a[] = {
     9,  9,  9, 10, 13, 18, 27, 39, 55,  1, 18,  2,  1,  1,
    35,  2, 19, 18,  1, 52, 36, 35, 20,  3,  2, 19, 18, 18,
    69,  2, 53,  1, 52, 37,  3, 20, 36, 19, 35, 18, 52, 86,
     2, 53, 70,  1, 69, 38, /*  36  */   4, 21,  3, 37, 19,
    36, 35, 25, 10, 17, 15, 26,  9, 11, 12, 23,  2,  6, 18,
    13, 20, 22, 14,  1, 24, 19, 21, 16,  5,  7,  4,  8,  3
}, u = ' ';

// Copies 5 characters from source to destination
_ W(p *destination, p *source) {
    for (p *end = destination + 5; destination < end; )
        *destination++ = *source++;
    return 0;
}

// Finds a character in an array and returns the position if found
_ O(p *array, p character) {
    p *end = array + 5;
    while (array < end && *array != character)
        ++array;
    return array < end ? 5 - (array - end) : 0;
}

// Prints an error message and exits
_ R(p *format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    exit(1);
}

// Adds a word to the list if there's space
q D(p *word) {
    if (s == sizeof t / 5)
        R("limit %d\n", s);

    p *current_entry = t[s];
    int i, j;
    for (i = j = 0; i < 7 && !j; ++i)
        if (!(j = *word == 10 ? i : 0))
            *current_entry++ = u | *word++;
    if (j - 5) ++s;
}

// Filters words based on a given pattern
q L(p *pattern, _ pattern_length) {
    for (_ index = 0, match_count = 0, char_index; index < s; match_count = 0) {
        for (_ j = 0; !match_count && j < 5 && (char_index = t[index][j]); ++j)
            if (i.c[j] && char_index != i.c[j]) ++match_count;
        for (_ j = 0; !match_count && j < 5 && (char_index = t[index][j]); ++j)
            if (i.a[char_index]) ++match_count;
        for (_ j = 0; !match_count && j < 5 && (char_index = t[index][j]); ++j)
            if (i.d[j] && !O(t[index], i.d[j])) ++match_count;
        for (_ f = i.e; f-- && !match_count; )
            for (_ j = 0; !match_count && j < 5 && (char_index = t[index][j]); ++j)
                if (i.b[f][j] == char_index) ++match_count;
        if (!match_count) {
            for (_ j = 0; j < 5; ++j)
                match_count += t[index][j] == pattern[j];
            match_count = match_count == 5 ? pattern_length != 5 : 0;
        }
        match_count ? W(t[index], t[--s]) : ++index;
    }
}

// A helper function for merging arrays
_ e(_ W, _ O, _ R, _ D, _ *L, _ *E) {
    for (; O < D; )
        L[R++] = E[O++ + D * W];
    return R;
}

// Sorts a segment of an array
q S(_ O, _ *l) {
    for (_ *start = &a[a[O]], *end = a[-~O] + start - a[O]; start < end; ) {
        _ i = *start++, j = i & 15, k = l[i >>= 4] < l[j] ? (k = l[i], l[i] = l[j], l[j] = k) : i;
    }
}

// Merges and sorts two arrays
_ o(_ W, _ O, _ R, _ D, _ *L, _ *E) {
    _ o = 0, w = 0;
    for (; o < D && w < D; )
        L[R++] = E[o + D * W] >= E[w + D * O] ? E[o++ + D * W] : E[w++ + D * O];
    return e(O, w, e(W, o, R, D, L, E), D, L, E);
}

// Processes an array with given constraints
q l(_ n, _ *f) {
    for (_ *end = f + 26, i, j; f < end && *f > u; f += i) {
        for (i = 1, j = *f / u; i < n && f[i] >> 5 == j; )
            ++i;
        for (j = 0; j < i; ++j)
            f[j] &= 31,
            f[j] |= a[f[j] + a[~-9]] << 5;
        S(i, f);
    }
}

// Performs a series of operations on arrays
q v(_ a, _ *w, _ *k) {
    o(2, 3, o(0, 1, 0, a, w, k), a, w, k);
    o(0, 1, 0, a + a, k, w);
    l(a, k);
}

// Evaluates and returns a pointer based on conditions
p *E(p *d) {
    _ l, j, k = u, b[u], c[k];
    p *e[97][5] = { 0 };
    for (l = 0; l < k; ++l)
        c[l] = l;
    for (l = 0; l < s; ++l)
        for (_ j = 0; j < 5; ++j)
            c[t[l][j] - 97] += k;
    for (l = j = 7; j < 22; j += l)
        S(l, c + j - l);
    v(l, b, c);
    for (l = 0; l < k; ++l)
        b[c[l] & ~-k] = l;
    for (k *= 3, l = 0; l < s; ++l) {
        _ i = 0;
        for (_ j = 0; j < 5; ++j)
            i += b[t[l][j] - 97];
        if (i < k)
            for (_ j = 0; j < 5; ++j)
                e[i][j] || (e[i][j] = t[l], j = 4);
    }
    for (l = 0; l < k; ++l)
        for (p *b = 0, k = j = 0; j < 5 && (b = e[l][j]); k = !++j) {
            p c[32] = { 0 };
            d = *d - 120 ? d : b;
            if (s > 2)
                for (_ j = 0; j < 5; ++j)
                    k += ++c[b[j] - 97] > 1;
            if (!k)
                return b;
        }
    return d;
}

// Processes an input string and returns a result
p *r(p *b) {
    _ k, l;
    for (_ j = k = l = 0; j < 5; ++j) {
        signed char n = b[j], o = b[j - ~5] + ~0x66;
        (o ? i.b[i.e] : i.c)[j] = n;
        !o ? i.a[n] = !++k : O(i.d, n) || (*(o < 0 ? &i.a[n] : &i.d[i.f++]) = l = n);
    }
    i.e += !!l;
    L(b, k);
    return E("xyzzy");
}

// Main function
_ main(_ n, char **v) {
    p *j = i.c - 97, *l = j - 11;
    if (n - 2) {
        R("usage: %s dict\n", *v);
    }
    FILE *k = fopen(*++v, "r");
    if (!k) {
        perror(*v);
        return 1;
    }
    while (fgets(j, u ^ n, k)) {
        D(j);
    }
    fclose(k);
    setbuf(stdout, 0);
    while (~-s) {
        printf("? ");
        while (~(n = getchar()) && n - 10 && l < j) {
            *l++ = n;
        }
        if (l < j) {
            R("input: guess color\n");
        }
        printf("%.5s\n", r(l = j + ~10));
    }
    return 0;
}