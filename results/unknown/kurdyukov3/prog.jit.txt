```c
#ifndef JIT
#if defined(__x86_64__) || defined(__i386__) || \
    defined(__e2k__) || defined(__aarch64__)
#define JIT 1
#else
#define JIT 0
#endif
#endif

#ifndef DUMP
#define DUMP 0
#endif

#ifdef _WIN32
#include <fcntl.h>
#define U(a,b) b
#else
#define U(a,b) a
#define _GNU_SOURCE
#include <unistd.h>
#if JIT
#include <sys/mman.h>
#endif
#endif

#ifdef SDL
#include <SDL.h>
#if SDL_MAJOR_VERSION < 2
#define V(a,b) a
#else
#define V(a,b) b
#endif
#define L(a,b) a
#else
#define L(a,b) b
#ifdef _WIN32
#include <windows.h>
#define Q(x) (QueryPerformance##x(&q),q.QuadPart)
#else
#include <sys/time.h>
#include <X11/Xlib.h>
#ifdef TERMIOS
#define T(a,b) a
#include <termios.h>
#include <sys/ioctl.h>
#else
#define T(a,b) b
#include <X11/Xutil.h>
#endif
#endif
#endif
#include <stdio.h>
#include <stdlib.h>

#define H x->h[c&7]

typedef struct {
#if JIT
    char** b, *i, *q, *p;
#endif
    unsigned* m, k, y, z; 
    char s[33]; 
    void* h[8];
    L(SDL_Surface* S V(; ,; SDL_Window* W; ),
    U(Display* D; T(struct termios t[2]; char I[64],
        Window W; GC G; XImage* I; Atom A),
        HWND W; HDC D; double f);)
} ctx_t;

U(; ,;
static int E = 0;
static LRESULT CALLBACK P(HWND a, UINT b, WPARAM c, LPARAM d) {
    E |= b == 2; 
    return DefWindowProcA(a, b, c, d); 
})

static int sys(unsigned n, int c, unsigned b, ctx_t* x) {
    void* f; 
    unsigned a, d;
    L(SDL_Event E, U(struct timeval t, LARGE_INTEGER q; MSG M));

    switch (n) {
        case 1:
            b = H ? (c < 0 ? fgetc(H) : fputc(b, H)) : -1;
            break;
        case 2:
            L(b = SDL_GetTicks(), U(gettimeofday(&t, 0); b = t.tv_sec * 1000 + t.tv_usec / 1000, b = Q(Counter) * x->f));
            break;
        case 3:
            L(SDL_Delay(b), U(t.tv_sec = 0; t.tv_usec = b * 1000; select(0, 0, 0, 0, &t), Sleep(b)));
            break;
        case 4:
            x->s[c & 31] = b && b - 48 > 9 && (b | 32) - 97 > 25 && b - 46 ? 95 : b;
            break;
        case 5:
            b = !(H = H ? fclose(H), f = 0 : fopen(x->s, b ? "wb" : "rb"));
            break;
        case 6:
            b = H ? (c < 0 ? ftell(H) : fseek(H, (int)b, c / 8)) : -1;
            break;
        case 7:
#define B(i,a,j) if(c == i) { x->m[1] = E.a.x | E.a.y << 14 | E.a.j << 28; b
#ifdef DUMMY
            fprintf(stderr, "!!! dummy\n"); 
            b = 6; 
            break;
        case 8: 
            b = 2; 
            break;
        case 9:
#elif defined SDL
            if (!(x->S || ~(-c & -b) >> 12)) {
                SDL_Init(SDL_INIT_VIDEO);
                V(x->S = SDL_SetVideoMode(x->y = c, x->z = b, 32, SDL_SWSURFACE); SDL_WM_SetCaption(x->s, 0),
                n = SDL_WINDOWPOS_UNDEFINED;
                x->S = SDL_GetWindowSurface(x->W = SDL_CreateWindow(x->s, n, n, x->y = c, x->z = b, 0)));
            }
            b = x->S ? V(12, 16) + (x->S->format->Rmask * 513 >> 24 & 3) : 0;
            break;
        case 8:
            while (b = 2, x->S && SDL_PollEvent(&E)) {
                c = E.type; 
                b = c == SDL_KEYUP;
                if (b || c == SDL_KEYDOWN) { 
                    x->m[1] = E.key.keysym.sym; 
                    break; 
                }
                b = c == SDL_MOUSEBUTTONUP; 
                B(SDL_MOUSEBUTTONDOWN || b, button, button) += 4; 
                break; 
            }
            B(SDL_MOUSEMOTION, motion, x) = 6; 
            break;
            if (c == SDL_QUIT) { 
                b = 3; 
                break; 
            }
            break;
        case 9:
            b &= x->k; 
            if (x->S && b + x->y * x->z < x->k) {
                for (n = 0; n < x->z; n++) {
                    memcpy((char*)x->S->pixels + n * x->S->pitch, x->m + b + n * x->y, x->y * 4);
                }
                V(SDL_Flip(x->S), SDL_UpdateWindowSurface(x->W));
            }
#elif defined(_WIN32)
            if (!(x->W || ~(-c & -b) >> 12)) {
                WNDCLASSA C = {0, P, 0, 0, 0, 0, 0, 0, 0, x->s};
                RECT r = {0, 0, x->y = c, x->z = b};
                RegisterClassA(&C);
                n = 1 << 31;
                AdjustWindowRect(&r, c = WS_CAPTION | WS_SYSMENU | WS_VISIBLE, 0);
                x->D = GetDC(x->W = CreateWindowExA(0, x->s, x->s, c, n, n, r.right - r.left, r.bottom - r.top, 0, 0, 0, 0));
            }
            b = x->W ? 10 : 0;
            break;
        case 8:
            b = E * 3;
            if (!b)
                while (b = 2, x->W && PeekMessageA(&M, x->W, 0, 0, 1)) {
                    b = M.message - 512;
                    if (b < 11) { 
                        x->m[1] = M.lParam + (M.wParam & (b > 9) << 30); 
                        b += 9; 
                        break; 
                    }
                    b = (b | 4) + 252; 
                    if (b < 2) { 
                        x->m[1] = M.wParam; 
                        break; 
                    }
                    DispatchMessageA(&M);
                }
            break;
        case 9:
            b &= x->k; 
            if (x->W && b + x->y * x->z < x->k) {
                int B[10] = {40, x->y, -x->z, 1 << 21 | 1};
                SetDIBitsToDevice(x->D, 0, 0, x->y, x->z, 0, 0, 0, x->z, x->m + b, f = &B, 0);
                SwapBuffers(x->D);
            }
#elif defined(TERMIOS)
            if (!(x->y | ~(-c & -b) >> 12)) {
                x->D = XOpenDisplay(0);
                tcgetattr(0, x->t); 
                x->t[1] = *x->t; 
                x->t->c_lflag &= ~ICANON & ~ECHO;
                x->t->c_cc[VMIN] = x->t->c_cc[VTIME] = 0; 
                tcsetattr(0, TCSANOW, x->t);
                x->y = c; 
                x->z = b;
                printf("\33[?25l\33[2J");
            }
            b = x->D ? 4 : 0;
            break;
        case 8:
        { 
            static int i, n;
            while (read(0, &b, 1) > 0);
            if (!(i &= 255))
                XQueryKeymap(x->D, &x->I[n ^= 32]);
            for (b = 2; i < 256; i++)
                if ((x->I[i >> 3] ^ x->I[i >> 3 | 32]) & (a = 1 << (i & 7))) {
                    x->m[1] = XKeycodeToKeysym(x->D, i, 0); 
                    b = !(x->I[n | i++ >> 3] & a);
                    break;
                }
        }
        break;
        case 9:
            b &= x->k; 
            if (b + x->y * x->z < x->k) {
                unsigned char* p, * r, * g;
                struct winsize s;
                int W, Q, w = x->y, h = x->z, q, e, i, j;
                ioctl(0, TIOCGWINSZ, &s);
                W = s.ws_col; 
                Q = s.ws_row * 2;
                p = malloc(W * Q * 3);
                e = w * Q < W * h ? q = (w * Q + h / 2) / h, Q : (q = W, (h * W + w / 2) / w);
                r = f = x->m + b;
                {
                    int x, y, u, v, a, b, c, k, l, z = 16, Y, Z;
                    for (v = y = 0; y < Q; v = c) { 
                        c = ++y * z * h / e;
                        for (u = x = 0; x < W; u = a) { 
                            a = ++x * z * w / q;
                            for (j = 0; j < 3; p[(y * q + x - q) * 3 - 3 + j++] = (l + b / 2) / b) { 
                                l = 0;
                                for (i = v; Z = ((i ^ c) < z ? c % z : z) - i % z, i < c; i += Z)
                                    for (k = u; Y = ((k ^ a) < z ? a % z : z) - k % z, k < a; k += Y)
                                        l += r[i / z * w * 4 + k / z * 4 + j] * Y * Z;
                                b = a - u; 
                                b *= c - v; 
                            } 
                        }
                    }
                }
                for (j = 0; j < e; j += 2) {
                    printf("\33[%dH", j / 2 + 1);
                    for (i = 0; i < q; i++) {
                        r = p + (j * q + i) * 3, g = r + q * 3;
                        printf("\33[");
                        if (j + 1 < e) 
                            printf("48;2;%d;%d;%d", *g, g[1], g[2]);
                        printf("m\33[38;2;%d;%d;%dm\342\226\200", *r, r[1], r[2]);
                    }
                    printf("\33[m%*s", W - q, "");
                }
                free(p);
                printf(j < Q ? "\n\33[J" : "\33[F\n");
            }
#else
            if (!(x->I || ~(-c & -b) >> 12)) {
                XSizeHints h; 
                h.flags = PMinSize | PMaxSize;
                h.min_width = h.max_width = c; 
                h.min_height = h.max_height = b;
                x->G = XDefaultGC(x->D = XOpenDisplay(0), 0);
                x->W = XCreateSimpleWindow(x->D, RootWindow(x->D, 0), 0, 0, c, b, 1, 0, 0);
                XSelectInput(x->D, x->W, KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask);
                XSetStandardProperties(x->D, x->W, x->s, 0, 0, 0, 0, &h);
                x->A = XInternAtom(x->D, "WM_DELETE_WINDOW", 0); 
                XSetWMProtocols(x->D, x->W, &x->A, 1);
                XMapWindow(x->D, x->W);
                x->I = XCreateImage(x->D, DefaultVisual(x->D, 0), 24, ZPixmap, 0, 0, x->y = c, x->z = b, 32, 0);
            }
            b = x->I ? 6 : 0;
            break;
        case 8:
            while (b = 2, x->I && XPending(x->D)) {
                XEvent E; 
                XNextEvent(x->D, &E); 
                c = E.type;
                if (c - ClientMessage || *E.xclient.data.l - x->A) {
                    b = c == KeyRelease; 
                    if (b || c == KeyPress) { 
                        x->m[1] = XLookupKeysym(&E.xkey, 0); 
                        break; 
                    }
                    b = c == ButtonRelease; 
                    B(ButtonPress || b, xbutton, button) += 4; 
                    break; 
                }
                B(MotionNotify, xmotion, x) = 6; 
                break;
            } else { 
                b = 3; 
                break; 
            }
            break;
        case 9:
            b &= x->k; 
            if (x->I && b + x->y * x->z < x->k) {
                x->I->data = f = x->m + b; 
                XPutImage(x->D, x->W, x->G, x->I, 0, 0, 0, 0, x->y, x->z); 
                XSync(x->D, 0);
            }
#endif
            break;
        case 10:
            if (b) {
#if JIT
                x->p = x->q; 
                d = x->k / 4; 
                for (a = 0; a <= d; a++) 
                    x->b[a] = x->i;
#endif
            } 
            U(; ,else b = H ? _setmode(_fileno(H), O_BINARY) : 0; )
            break;
        default:
            L(; ,U(
                T(x->D ? (tcsetattr(0, TCSANOW, x->t + 1), printf("\33[?25h")) : 0,
                x->I ? x->I->data = 0 : 0); ,; ))
            d = (*x->m - 4) & x->k; 
            a = x->m[d] >> 14;
            if (a) 
                fprintf(stderr, "bad opcode at 0x%x: 0x%x\n", d * 4, x->m[d]);
#if JIT && DUMP == 2
            if ((f = fopen("codedump.bin", "wb"))) {
                char* p = x->p, * p0 = (char*)(x->b + x->k / 4 + 1);
                fwrite(p0, 1, p - p0, f); 
                fclose(f);
            }
#endif
            exit(a ? 1 : c);
    }
    return b;
}

#ifdef __e2k__
#define E2K_WAIT(flags) __asm__ ("wait " flags : : : "memory")
#define __clear_cache __clear_cache_e2k
static void __clear_cache_e2k(void* begin, void* end) {
    unsigned long step = 256, ptr = (long)begin & -step;
    E2K_WAIT("st_c=1");
    for (; ptr < (unsigned long)end; ptr += step)
        __builtin_storemas_64u(0, (void*)ptr, 15, __LCC_CHAN_ANY);
    E2K_WAIT("fl_c=1");
}
#endif

int main(int c, char** x) {
    void* f = c-- > 1 ? fopen(x[1], "rb") : 0;
    unsigned a, b = 0, d, n = 0, k = (1 << 24) - 1, * m = calloc(k + 32, 4);
    ctx_t C = {0};
#if JIT
#ifdef __x86_64__
#define S(a,b) a
#define F E
#elif defined(__i386__)
#define S(a,b) b
#define F(x)
#elif defined(__e2k__)
    long* P;
#elif defined(__aarch64__)
    int* P;
#else
#error
#endif
    char** A, * p, * p0, * pr; 
    int An = 1 << 24; /* 1<<30 max for