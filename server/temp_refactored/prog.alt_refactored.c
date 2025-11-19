// Refactored code with clear comments
#include <stdio.h>
#include <math.h>

// Constants for array sizes and other configurations
#define L 551368
#define M 6400
#define C 15168
#define J 949
#define K 12
#define D 80
#define X 79
#define Y 64

// Function prototypes
float* get_w(int t, int i, int a, int T);
float calculate_b(float x, float y, float n);
void normalize_vector(float x, float y, float n, float *k, float *h, float *c);
void smooth(int x, int S);
float* get_d(int o);
float absolute_value(float k);
void copy_w(int x, int y);
void bound_value(float *z);
void update_w(int E, int y, int u, int v, int w);
void calculate_v(int x, int E, int k, int b, int c);
void process_w(int u, int T, int w, int E);

// Global variables
float x, T = 240, B, j = 0.53, f, y, buffer[4426112], Z, w, R, n, m, E, i, U, g = 0;
char output[60737];

float* get_w(int t, int i, int a, int T) {
    return &buffer[(t + 82 * i + 6724 * a) + (C + T * L)];
}

float calculate_b(float x, float y, float n) {
    x = x * x + y * y + n * n;
    int s = 0x1fbb4000 + (*(int*)&x >> 1);
    return *(float*)&s;
}

void normalize_vector(float x, float y, float n, float *k, float *h, float *c) {
    U = calculate_b(x, y, n);
    *k = x / U;
    *h = y / U;
    *c = n / U;
}

void smooth(int x, int S) {
    for (int F = 0; F < L; ++F) {
        buffer[C + x * L + F] += 0.4 * buffer[C + S * L + F];
    }
}

float* get_d(int o) {
    return &buffer[(t * X + I) * 3 + o];
}

float absolute_value(float k) {
    return k < 0 ? -k : k;
}

void copy_w(int x, int y) {
    for (int F = 0; F < M * D; ++F) {
        int G = F % M;
        t = G % D + 1;
        I = G / D + 1;
        a = F / M + 1;
        *get_w(t, I, a, x) = *get_w(t, I, a, y);
    }
}

void bound_value(float *z) {
    *z = *z < 0.5 ? 0.5 : *z > 85 ? 85 : *z;
}

void update_w(int E, int y, int u, int v, int w) {
    for (int F = 0; F < M * D; ++F) {
        int G = F % M;
        t = G % D + 1;
        I = G / D + 1;
        a = F / M + 1;
        float U = t - D * 0.4 * *get_w(t, I, a, u);
        float J = I - D * 0.4 * *get_w(t, I, a, v);
        float e = a - D * 0.4 * *get_w(t, I, a, w);
        bound_value(&U);
        bound_value(&J);
        bound_value(&e);
        float F = U - (int)U, j = J - (int)J, l = 1 - j, i = e - (int)e, c = 1 - i;
        *get_w(t, I, a, E) = (1 - F) * (l * c * *get_w(U, J, e, y) + j * c * *get_w(U, 1 + J, e, y) +
                       l * i * *get_w(U, J, e + 1, y) + j * i * *get_w(U, 1 + J, 1 + e, y)) +
                     F * (l * c * *get_w(U + 1, J, e, y) + j * c * *get_w(1 + U, 1 + J, e, y) +
                          l * i * *get_w(1 + U, J, 1 + e, y) + j * i * *get_w(1 + U, J + 1, e + 1, y));
    }
}

void calculate_v(int x, int E, int k, int b, int c) {
    *get_w(t, I, a, x) -= 40 * (*get_w(t + k, I + b, a + c, E) - *get_w(t - k, I - b, a - c, E));
}

void process_w(int u, int T, int w, int E) {
    for (int F = 0; F < M * D; ++F) {
        int G = F % M;
        t = G % D + 1;
        I = G / D + 1;
        a = F / M + 1;
        *get_w(t, I, a, E) = -1.0 / 3 * ((get_w(1 + t, I, a, u) - get_w(t - 1, I, a, u)) / D +
            (get_w(t, I + 1, a, T) - get_w(t, I - 1, a, T)) / D + 
            (get_w(t, I, 1 + a, w) - get_w(t, I, a - 1, w)) / D);
    }
    for (int F = 0; F < M * D; ++F) {
        int G = F % M;
        t = G % D + 1;
        I = G / D + 1;
        a = F / M + 1;
        calculate_v(u, E, 1, 0, 0);
        calculate_v(T, E, 0, 1, 0);
        calculate_v(w, E, 0, 0, 1);
    }
}

int main() {
    int e = M * D;
    int I = C + L * 8;
    output[J * Y] = '\0';

    // Initialize buffer
    for (int F = 0; F < I; ++F) {
        buffer[F] = 0;
    }

    // Prepare initial output
    for (t = 0; t < Y; ++t) {
        output[t * J + X * K] = '\n';
        for (I = 0; I < X;) {
            normalize_vector(I++ - 40, t - 32, -145, &w, &Z, &R);
            *get_d(0) = w;
            *get_d(1) = Z;
            *get_d(2) = R;
        }
    }

    // Main loop
    while (1) {
        for (int F = 0; F < L; ++F) {
            buffer[C + 3 * L + F] = buffer[C + 4 * L + F] = buffer[C + 5 * L + F] = buffer[C + 7 * L + F] = 0;
        }

        for (int F = 0; F < M * D; ++F) {
            int G = F % M;
            t = G % D + 1;
            I = G / D + 1;
            a = F / M + 1;

            x = t - L;
            y = a - L;
            U = x * x + y * y;
            G = 0x1fbb4000 + (*(int*)&U >> 1);
            i = *(float*)&G;

            if (I < 13 && I > 10) {
                if (i < 2) {
                    *get_w(t, I, a, 4) = 1.5;
                    m = g + (I / D);
                    U = m * m;
                    w = m * m;
                    m = m - (U * m) / 6 + (U * U * m) / 120;
                    E = 1 - w / 2 + (w * w) / 24;
                    *get_w(t, I, a, 3) = m / 2 + 0.0275;
                    *get_w(t, I, a, 5) = E * 0.7 - 0.35;
                }
            }

            if (I < 2 && i < 3) {
                *get_w(t, I, a, 7) = 0.1;
            }
        }

        smooth(0, 3);
        smooth(1, 4);
        smooth(2, 5);
        copy_w(5, 2);
        copy_w(4, 1);
        copy_w(3, 0);
        process_w(3, 4, 5, 1);
        update_w(1, 4, 3, 4, 5);
        update_w(0, 3, 3, 4, 5);
        update_w(2, 5, 3, 4, 5);
        process_w(0, 1, 2, 4);
        smooth(6, 7);
        copy_w(7, 6);
        update_w(6, 7, 0, 1, 2);

        x = T * 0.9998 + B * 0.02;
        B = 0.9998 * B - T * 0.02;
        T = x;

        normalize_vector(-T, 0, -B, &R, &w, &Z);
        g += 0.1;
        g = g > 3.14 ? -g : g;
        normalize_vector(Z, 0, -R, &m, &E, &i);

        for (int F = 0; F < 5056; ++F) {
            int G = F % 5056;
            t = G % Y;
            I = G / Y;
            U = *get_d(0);
            float v = *get_d(1), g = *get_d(2);

            float V = U * m + v * (E * Z - w * i) + g * -R;
            float s = U * E + v * (i * R - Z * m) + g * -w;
            U = U * i + v * (m * w - R * E) + g * -Z;

            f = 0;
            int q = t * J + I * K + 8;
            for (a = 0; a < K; ++a) {
                output[q + a - 8] = "+";
            }

            while (f < D * 5) {
                x = absolute_value(T + V * f) - L;
                y = absolute_value(s * f) - L;
                n = absolute_value(B + U * f) - L;

                float k = y > n ? y : n;
                float c = x > k ? x : k;
                float F = (c < 0 ? c : 0) + calculate_b(x < 0 ? 0 : x, y < 0 ? 0 : y, n > 0 ? n : 0);
                if (F < 0.01) {
                    x = T + f * V + L;
                    y = f * s + L;
                    n = B + f * U + L;

                    f = 1;
                    a = 0;
                    while ((a++ < 1) || (D > x && y < D && n < D && x > 0 && 0 < y && n > 0)) {
                        f *= 1 - *get_w(n + 1, 1 + y, x + 1, 6) * j;
                        x += V * j;
                        y += s * j;
                        n += U * j;
                    }

                    a = (1 - f) * 35;
                    a = a < 0 ? 0 : a > 34 ? 34 : a;
                    int c = 240 + 0.44 * a;
                    output[q] = (c - 200) / 10 + 48;
                    output[q + 1] = c % 10 + 48;
                    output[q + 3] = "+";
                    break;
                }
                f += F;
            }
        }
        puts(output);
    }
}