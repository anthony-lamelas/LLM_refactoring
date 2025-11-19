```c
// Refactored code with clear comments
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef JIT
#if defined(__x86_64__) || defined(__i386__) || defined(__e2k__) || defined(__aarch64__)
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
#define PLATFORM_SPECIFIC(a, b) b
#else
#define PLATFORM_SPECIFIC(a, b) a
#define _GNU_SOURCE
#include <unistd.h>
#if JIT
#include <sys/mman.h>
#endif
#endif

#ifdef SDL
#include <SDL.h>
#if SDL_MAJOR_VERSION < 2
#define SDL_VERSION_SPECIFIC(a, b) a
#else
#define SDL_VERSION_SPECIFIC(a, b) b
#endif
#define SDL_SPECIFIC(a, b) a
#else
#define SDL_SPECIFIC(a, b) b
#ifdef _WIN32
#include <windows.h>
#define QUERY_PERFORMANCE(x) (QueryPerformance##x(&q), q.QuadPart)
#else
#include <sys/time.h>
#include <X11/Xlib.h>
#ifdef TERMIOS
#define TERMIOS_SPECIFIC(a, b) a
#include <termios.h>
#include <sys/ioctl.h>
#else
#define TERMIOS_SPECIFIC(a, b) b
#include <X11/Xutil.h>
#endif
#endif
#endif

typedef struct {
#if JIT
    char **b, *i, *q, *p;
#endif
    unsigned *m, k, y, z;
    char s[33];
    void *h[8];
    SDL_SPECIFIC(SDL_Surface *S SDL_VERSION_SPECIFIC(; ,; SDL_Window *W; ),
    PLATFORM_SPECIFIC(Display *D; TERMIOS_SPECIFIC(struct termios t[2]; char I[64],
    Window W; GC G; XImage *I; Atom A),
    HWND W; HDC D; double f);)
} ctx_t;

PLATFORM_SPECIFIC(; ,; static int exit_flag = 0;
static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    exit_flag |= uMsg == WM_CLOSE;
    return DefWindowProcA(hwnd, uMsg, wParam, lParam);
})

static int sys(unsigned n, int c, unsigned b, ctx_t *ctx) {
    void *f;
    unsigned a, d;
    SDL_SPECIFIC(SDL_Event event, PLATFORM_SPECIFIC(struct timeval tv; LARGE_INTEGER q; MSG msg));
    switch (n) {
        case 1:
            b = ctx->h[c & 7] ? (c < 0 ? fgetc(ctx->h[c & 7]) : fputc(b, ctx->h[c & 7])) : -1;
            break;
        case 2:
            SDL_SPECIFIC(
                b = SDL_GetTicks(),
                PLATFORM_SPECIFIC(
                    gettimeofday(&tv, 0);
                    b = tv.tv_sec * 1000 + tv.tv_usec / 1000,
                    b = QUERY_PERFORMANCE(Counter) * ctx->f
                )
            );
            break;
        case 3:
            SDL_SPECIFIC(
                SDL_Delay(b),
                PLATFORM_SPECIFIC(
                    tv.tv_sec = 0;
                    tv.tv_usec = b * 1000;
                    select(0, 0, 0, 0, &tv),
                    Sleep(b)
                )
            );
            break;
        case 4:
            ctx->s[c & 31] = (b && b - 48 > 9 && (b | 32) - 97 > 25 && b - 46) ? 95 : b;
            break;
        case 5:
            b = !(ctx->h[c & 7] = ctx->h[c & 7] ? fclose(ctx->h[c & 7]), f = 0 : fopen(ctx->s, b ? "wb" : "rb"));
            break;
        case 6:
            b = ctx->h[c & 7] ? (c < 0 ? ftell(ctx->h[c & 7]) : fseek(ctx->h[c & 7], (int)b, c / 8)) : -1;
            break;
        case 7:
#ifdef DUMMY
            fprintf(stderr, "!!! dummy\n");
            b = 6;
            break;
        case 8:
            b = 2;
            break;
        case 9:
#elif defined SDL
            if (!(ctx->S || ~(-c & -b) >> 12)) {
                SDL_Init(SDL_INIT_VIDEO);
                SDL_VERSION_SPECIFIC(
                    ctx->S = SDL_SetVideoMode(ctx->y = c, ctx->z = b, 32, SDL_SWSURFACE);
                    SDL_WM_SetCaption(ctx->s, 0),
                    int n = SDL_WINDOWPOS_UNDEFINED;
                    ctx->S = SDL_GetWindowSurface(ctx->W = SDL_CreateWindow(ctx->s, n, n, ctx->y = c, ctx->z = b, 0))
                );
            }
            b = ctx->S ? SDL_VERSION_SPECIFIC(12, 16) + (ctx->S->format->Rmask * 513 >> 24 & 3) : 0;
            break;
        case 8:
            while (b = 2, ctx->S && SDL_PollEvent(&event)) {
                c = event.type;
                b = c == SDL_KEYUP;
                if (b || c == SDL_KEYDOWN) {
                    ctx->m[1] = event.key.keysym.sym;
                    break;
                }
                b = c == SDL_MOUSEBUTTONUP;
                if (c == SDL_MOUSEBUTTONDOWN || b) {
                    ctx->m[1] = event.button.x | event.button.y << 14 | event.button.button << 28;
                    b += 4;
                    break;
                }
                if (c == SDL_MOUSEMOTION) {
                    ctx->m[1] = event.motion.x | event.motion.y << 14 | event.motion.state << 28;
                    b = 6;
                    break;
                }
                if (c == SDL_QUIT) {
                    b = 3;
                    break;
                }
            }
            break;
        case 9:
            b &= ctx->k;
            if (ctx->S && b + ctx->y * ctx->z < ctx->k) {
                for (n = 0; n < ctx->z; n++) {
                    memcpy((char *)ctx->S->pixels + n * ctx->S->pitch, ctx->m + b + n * ctx->y, ctx->y * 4);
                }
                SDL_VERSION_SPECIFIC(SDL_Flip(ctx->S), SDL_UpdateWindowSurface(ctx->W));
            }
#elif defined(_WIN32)
            if (!(ctx->W || ~(-c & -b) >> 12)) {
                WNDCLASSA C = {0, WindowProc, 0, 0, 0, 0, 0, 0, 0, ctx->s};
                RECT r = {0, 0, ctx->y = c, ctx->z = b};
                RegisterClassA(&C);
                int n = 1 << 31;
                AdjustWindowRect(&r, c = WS_CAPTION | WS_SYSMENU | WS_VISIBLE, 0);
                ctx->D = GetDC(ctx->W = CreateWindowExA(0, ctx->s, ctx->s, c, n, n, r.right - r.left, r.bottom - r.top, 0, 0, 0, 0));
            }
            b = ctx->W ? 10 : 0;
            break;
        case 8:
            b = exit_flag * 3;
            if (!b) while (b = 2, ctx->W && PeekMessageA(&msg, ctx->W, 0, 0, 1)) {
                b = msg.message - 512;
                if (b < 11) {
                    ctx->m[1] = msg.lParam + (msg.wParam & (b > 9) << 30);
                    b += 9;
                    break;
                }
                b = (b | 4) + 252;
                if (b < 2) {
                    ctx->m[1] = msg.wParam;
                    break;
                }
                DispatchMessageA(&msg);
            }
            break;
        case 9:
            b &= ctx->k;
            if (ctx->W && b + ctx->y * ctx->z < ctx->k) {
                int B[10] = {40, ctx->y, -ctx->z, 1 << 21 | 1};
                SetDIBitsToDevice(ctx->D, 0, 0, ctx->y, ctx->z, 0, 0, 0, ctx->z, ctx->m + b, &B, 0);
                SwapBuffers(ctx->D);
            }
#elif defined(TERMIOS)
            if (!(ctx->y | ~(-c & -b) >> 12)) {
                ctx->D = XOpenDisplay(0);
                tcgetattr(0, ctx->t);
                ctx->t[1] = *ctx->t;
                ctx->t->c_lflag &= ~ICANON & ~ECHO;
                ctx->t->c_cc[VMIN] = ctx->t->c_cc[VTIME] = 0;
                tcsetattr(0, TCSANOW, ctx->t);
                ctx->y = c;
                ctx->z = b;
                printf("\33[?25l\33[2J");
            }
            b = ctx->D ? 4 : 0;
            break;
        case 8: {
            static int i, n;
            while (read(0, &b, 1) > 0);
            if (!(i &= 255)) XQueryKeymap(ctx->D, &ctx->I[n ^= 32]);
            for (b = 2; i < 256; i++) {
                if ((ctx->I[i >> 3] ^ ctx->I[i >> 3 | 32]) & (a = 1 << (i & 7))) {
                    ctx->m[1] = XKeycodeToKeysym(ctx->D, i, 0);
                    b = !(ctx->I[n | i++ >> 3] & a);
                    break;
                }
            }
        } break;
        case 9:
            b &= ctx->k;
            if (b + ctx->y * ctx->z < ctx->k) {
                unsigned char *p, *r, *g;
                struct winsize s;
                int W, Q, w = ctx->y, h = ctx->z, q, e, i, j;
                ioctl(0, TIOCGWINSZ, &s);
                W = s.ws_col;
                Q = s.ws_row * 2;
                p = malloc(W * Q * 3);
                e = w * Q < W * h ? q = (w * Q + h / 2) / h, Q : (q = W, (h * W + w / 2) / w);
                r = f = ctx->m + b;
                {
                    int x, y, u, v, a, b, c, k, l, z = 16, Y, Z;
                    for (v = y = 0; y < Q; v = c) {
                        c = ++y * z * h / e;
                        for (u = x = 0; x < W; u = a) {
                            a = ++x * z * w / q;
                            for (j = 0; j < 3; p[(y * q + x - q) * 3 - 3 + j++] = (l + b / 2) / b) {
                                l = 0;
                                for (i = v; Z = ((i ^ c) < z ? c % z : z) - i % z, i < c; i += Z) {
                                    for (k = u; Y = ((k ^ a) < z ? a % z : z) - k % z, k < a; k += Y) {
                                        l += r[i / z * w * 4 + k / z * 4 + j] * Y * Z;
                                    }
                                }
                                b = a - u;
                                b *= c - v;
                            }
                        }
                    }
                }
                for (j = 0; j < e; j += 2) {
                    printf("\33[%dH", j / 2 + 1);
                    for (i = 0; i < q; i++) {
                        r = p + (j * q + i) * 3;
                        g = r + q * 3;
                        printf("\33[");
                        if (j + 1 < e) printf("48;2;%d;%d;%d", *g, g[1], g[2]);
                        printf("m\33[38;2;%d;%d;%dm\342\226\200", *r, r[1], r[2]);
                    }
                    printf("\33[m%*s", W - q, "");
                }
                free(p);
                printf(j < Q ? "\n\33[J" : "\33[F\n");
            }
#else
            if (!(ctx->I || ~(-c & -b) >> 12)) {
                XSizeHints h;
                h.flags = PMinSize | PMaxSize;
                h.min_width = h.max_width = c;
                h.min_height = h.max_height = b;
                ctx->G = XDefaultGC(ctx->D = XOpenDisplay(0), 0);
                ctx->W = XCreateSimpleWindow(ctx->D, RootWindow(ctx->D, 0), 0, 0, c, b, 1, 0, 0);
                XSelectInput(ctx->D, ctx->W, KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask);
                XSetStandardProperties(ctx->D, ctx->W, ctx->s, 0, 0, 0, 0, &h);
                ctx->A = XInternAtom(ctx->D, "WM_DELETE_WINDOW", 0);
                XSetWMProtocols(ctx->D, ctx->W, &ctx->A, 1);
                XMapWindow(ctx->D, ctx->W);
                ctx->I = XCreateImage(ctx->D, DefaultVisual(ctx->D, 0), 24, ZPixmap, 0, 0, ctx->y = c, ctx->z = b, 32, 0);
            }
            b = ctx->I ? 6 : 0;
            break;
        case 8:
            while (b = 2, ctx->I && XPending(ctx->D)) {
                XEvent event;
                XNextEvent(ctx->D, &event);
                c = event.type;
                if (c - ClientMessage || *event.xclient.data.l - ctx->A) {
                    b = c == KeyRelease;
                    if (b || c == KeyPress) {
                        ctx->m[1] = XLookupKeysym(&event.xkey, 0);
                        break;
                    }
                    b = c == ButtonRelease;
                    if (c == ButtonPress || b) {
                        ctx->m[1] = event.xbutton.x | event.xbutton.y << 14 | event.xbutton.button << 28;
                        b += 4;
                        break;
                    }
                    if (c == MotionNotify) {
                        ctx->m[1] = event.xmotion.x | event.xmotion.y << 14 | event.xmotion.state << 28;
                        b = 6;
                        break;
                    }
                } else {
                    b = 3;
                    break;
                }
            }
            break;
        case 9:
            b &= ctx->k;
            if (ctx->I && b + ctx->y * ctx->z < ctx->k) {
                ctx->I->data = f = ctx->m + b;
                XPutImage(ctx->D, ctx->W, ctx->G, ctx->I, 0, 0, 0, 0, ctx->y, ctx->z);
                XSync(ctx->D, 0);
            }
#endif
            break;
        case 10:
            if (b) {
#if JIT
                ctx->p = ctx->q;
                d = ctx->k / 4;
                for (a = 0; a <= d; a++) ctx->b[a] = ctx->i;
#endif
            } else {
                b = ctx->h[c & 7] ? _setmode(_fileno(ctx->h[c & 7]), O_BINARY) : 0;
            }
            break;
        default:
            SDL_SPECIFIC(; ,PLATFORM_SPECIFIC(
                TERMIOS_SPECIFIC(
                    if (ctx->D) tcsetattr(0, TCSANOW, ctx->t + 1), printf("\33[?25h"),
                    if (ctx->I) ctx->I->data = 0
                ); ,; ))
            d = (*ctx->m - 4) & ctx->k;
            a = ctx->m[d] >> 14;
            if (a) {
                fprintf(stderr, "bad opcode at 0x%x: 0x%x\n", d * 4, ctx->m[d]);
            }
#if JIT && DUMP == 2
            if ((f = fopen("codedump.bin", "wb"))) {
                char *p = ctx->p, *p0 = (char *)(ctx->b + ctx->k / 4 + 1);
                fwrite(p0, 1, p - p0, f);
                fclose(f);
            }
#endif
            exit(a ? 1 : c);
    }
    return b;
}

#ifdef __e2k__
#define E2K_WAIT(flags) __asm__("wait " flags : : : "memory")
#define __clear_cache __clear_cache_e2k
static void __clear_cache_e2k(void *begin, void *end) {
    unsigned long step = 256, ptr = (long)begin & -step;
    E2K_WAIT("st_c=1");
    for (; ptr < (unsigned long)end; ptr += step)
        __builtin_storemas_64u(0