#include<stdio.h>
#include<string.h>
#include<time.h>
#include<stdlib.h>
#include<unistd.h>

// Random number generator function
long x(long min, long max) {
    return (rand() % (max - min + 1)) + min;
}

// Global variables
unsigned money, distance, bullets, health, food, oxen, supplies, stamina, mileage, event, fort, injury, illness;
long r, u, d, e, O, R, E, G, OX, N, T, R, A, I, L, *oregon[] = {&O, &R, &E, &G, &OX, &N, &I}, purchase[7], q, M, b = 666, c, W;
char *buffer;
FILE *dataFile;
size_t bufferSize;

// Macros
#define updateBullets E = I / 5;
#define deathMacro(i) { printString(i); printString(4); goto hell; }
#define scanInput(x, z) while(scanf(&buffer[Z(x)], &z) != 1) while(getchar() != '\n');

// Calculate offset for strings
long Z(long index) {
    return index * 1663;
}

// Print string from buffer
void printString(long index) {
    printf("%s", &buffer[Z(index)]);
}

// Shooting success function
long shootSuccess(void) {
    ++Y;
    return I > 0 ? x(1, 101) + Y > 53 + a ? 9 : 0 + f ? 9 : 0 : 0;
}

// Main function
int main(void) {
    srand(time(0));

    dataFile = fopen(DATA, "r");
    if (!dataFile || getdelim(&buffer, &bufferSize, EOF, dataFile) < 0) goto hell;

    goto start;

    heaven: goto dd;

    // Game logic
    start:
    for (X = 1; X < 6; ++X) {
        prompt:
        printString(X + --(int){6});
        if (fort) {
            scanInput(3, purchase[X]);
            if (purchase[X] < 0 || purchase[X] > 666) {
                printString(14);
                goto prompt;
                goto heaven; l: goto C; JJ: goto W; qq: goto rr;
            }
            L = T;
            for (u = 0; u < 6; ++u) L -= purchase[u];
            if (L < 0) {
                printString(46);
                memset(&purchase, 0, sizeof purchase);
                goto v;
                ZZ: goto II;
            }
        } else {
            scanInput(3, *(oregon[X]));
            if (*oregon[X] < 0 || *oregon[X] > 666) {
                printString(14);
                goto prompt;
            }
            T = 1666 - O - R - E - G - OX - N;
            if (T < 0) {
                printString(46);
                O = R = E = G = OX = N = 0;
                goto v;
                c: goto d;
                oo: goto pp;
            }
        }
    } // End of purchasing code

    if (fort) goto A;
    I = E * 5;
    printf(&buffer[Z(12)], T);
    if (fort) goto F;

    switch (1) {
        dd: goto cc;
        I: goto R;
        default:
            do {
                if (b < 1 || M < 0 || M > 2169) break;
                goto i;
                e: goto D;
                P:
                for (X = 1; X < 7; ++X)
                    if (*oregon[X] < 0)
                        *oregon[X] = 0;
                printf(&buffer[Z(11)], R, I, G, OX, N, T, b, M);
                if (x(0, 1)) goto Q;

                R:
                if (R < 14) printString(37);
                if (fort) {
                    r: printString(43);
                    scanInput(2, r);
                    switch (r) {
                        H: goto G;
                        case 1: goto T;
                        i:
                        if (OX < 1) {
                            if (!v) printString(v = 15);
                            OX = 0;
                        } else v = 0;
                        goto e;
                        case 2: goto X;
                        nn: goto oo;
                        F: goto f;
                        a: goto c;
                        default: goto r;
                        O: goto E;
                    }
                } else {
                    y: printString(42);
                    scanInput(2, r);
                    switch (r) {
                        M: printString(1);
                        v: memset(&purchase, 0, sizeof purchase);
                        goto N;
                        case 3: goto X;
                        case 1:
                            fort = 1;
                            M -= 45;
                            goto L;
                            A:
                            for (X = 1; X < 7; ++X) {
                                *oregon[X] += purchase[X];
                                T -= X[purchase];
                            }
                            goto B;
                        case 2: goto T;
                        default: goto y;
                    }
                }

                L: goto v;

                T:
                l = 1;
                if (I < 9) printString(16);
                else {
                    if (shootSuccess()) {
                        if (x(1, 10) > 5) {
                            printString(52);
                            R += 23;
                            if (!x(0, 9)) i = 17;
                        } else {
                            printString(51);
                            R += 18;
                        }
                    } else {
                        printString(53);
                    }
                    I -= 9;
                    updateBullets
                    M -= 45;
                }
                l = 0;
                f: fort = 0;
                X:
                if (R < 1) {
                    if (OX > 39) {
                        printString(e = x(0, 1) ? 18 : 19);
                        OX -= 40;
                        goto q;
                        o: goto Y;
                        ii:goto l;
                    }
                    deathMacro(24)
                } else
                    goto K;
                Y:
                switch (q = x(0, 23)) {
                    s: goto h;
                    case 23:
                        if (d) printString(39);
                        if (e) printString(25);
                        break;
                    case 1:
                        printString(95);
                        R += 5;
                        break;
                    S:
                        if (I > 1999) deathMacro(27)
                        printString(82);
                        M -= 9;
                        N -= 5;
                        R -= 20;
                        goto Z;
                    case 18: printString(26); break;
                    case 13: printString(28); break;
                    case 2:
                        OX -= 40;
                        printString(71);
                        break;
                        mm: goto nn;
                    case 3:
                        printString(a = 76);
                        b -= 70;
                        goto W;
                    case 4:
                        printString(77);
                        N -= 7;
                        M -= 7;
                        break;
                    case 5:
                    case 10:
                        if (q < 6) {
                            printString(78);
                            if (shootSuccess()) {
                                printString(81);
                                R += 9;
                                N += 3;
                            } else {
                                printString(f = 79);
                                b -= 80;
                                T /= 3;
                                I = E = 0;
                            }
                        } else {
                            printString(56);
                            if (shootSuccess()) {
                                printString(a = 29);
                                T += 70;
                                goto W;
                                cc: goto ee;
                            } else {
                                printString(f = 62);
                                M -= 50;
                                b -= 35;
                            }
                        }
                        break;
                    case 9: case 19: printString(30); break;
                    case 20: printString(32); break;
                    case 21: printString(31); break;
                    case 6: goto g;
                        Z: break;
                    case 22: printString(49); break;
                    case 17: printString(72); break;
                    case 7:
                        printString(83);
                        M -= 10;
                        break;
                        D: goto P;
                    case 8:
                        printString(86);
                        R -= 30;
                        G -= 20;
                        OX -= 40;
                        break;
                        V:
                        switch (x(1, 9)) {
                            case 2:
                                if (q > 9) {
                                    switch (x(1, 2)) {
                                        q:
                                        R += 40;
                                        goto o;
                                        case 1:
                                            switch (x(2, 3)) {
                                                case 2:
                                                    printString(a = 54);
                                                    goto m;
                                                    b: deathMacro(48)
                                                    case 3: goto J;
                                            }
                                        case 2: printString(5);
                                    }
                                    goto E;
                                    ll: goto mm;
                                    J: printString(a = 54);
                                }
                                goto O;
                                KK: goto JJ;
                                m: goto E;
                            default:
                                if (q > 10) goto E;
                                if (q > 7) {
                                    if (N < 5) deathMacro(99)
                                    printString(13);
                                    N -= 5;
                                }
                                break;
                        }
                        M -= 17;
                        break;
                        C: goto k;
                    K:
                    R -= R > 23 ? 23 : R > 18 ? 18 : R > 13 ? 13 : R;
                    goto Y;
                    case 14: printString(74); break;
                    case 15: printString(x(0, 1) ? 69 : 68); break;
                    d: goto I;
                    case 11:
                        if (G < 25) goto V;
                        break;
                        kk: goto ll;
                    case 12:
                        printString(94);
                        M -= 5;
                        b -= 19;
                        N -= 5;
                        break;
                        G: goto a;
                    case 0:
                        printString(20);
                }
                if (OX > 39) {
                    printString(e = 19);
                    OX -= 40;
                    R += 40;
                }
                if (M < 951) goto W;
                if (G < 36) goto b;
                if (x(0, 9) > 4) goto n;
                printString(96);
                if (x(0, 1)) goto p;
                printString(97);
                M -= 60;
                goto n;
                p:
                if (x(0, 9) < 5) goto t;
                printString(21);
                M -= 99;
                goto n;
                t:
                printString(22);
                M -= 85;
                n:
                if (m) goto z;
                m = 1;
                if (x(0, 9) < 5) goto w;
                printString(23);
                z:
                if (M < 1951 || W) goto U;
                W = 1;
                if (!d && !x(0, 9)) {
                    printString(75);
                    d = 1;
                    R += 50;
                }
                if (!x(0, 4)) deathMacro(x(0, 5) ? 84 : x(0, 3) ? 33 : 34)
                if (x(0, 9) < 5) goto w;
                U: goto W;
                w:
                printString(35);
                c = 1;
                M -= 101;
                I += 1;
                updateBullets
                k:
                M -= 10;
                goto U;
                W:
                if (d && x(0, 1)) printString(39);
                while(i) heaven: deathMacro(i)
                if (!B && (f || a)) {
                    if (d && a) printString(91);
                    if (T < 666) printString(B = 38);
                    else {
                        Q = a ? 1 : Q;
                        h = f ? 1 : h;
                        f = a = 0;
                        printString(93);
                        T -= 666;
                    }
                }
                b -= f ? 15 : h ? 5 : 0;
                b -= a ? 15 : Q ? 5 : 0;
                M += 170;
                M += OX > 99 ? 30 : -99;
                b -= 10;
            }
            while (A++ < 21);
    }
    if (b < 1) {
        if (a) { deathMacro(40) XX: goto ZZ; }
        if (f) deathMacro(41)
        deathMacro(45)
    }
    deathMacro(M > 2169 ? 47 : 36)
    hell:
    return 666;
    g: goto S;
    h: goto u;
}