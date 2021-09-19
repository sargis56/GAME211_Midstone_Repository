#include <glew.h>
/// This is the only function you need to call the rest are internal within Noise.cpp
GLuint CreateNoise3D();
void DeleteNoise3D();

/// Prototypes of all the other c-style functions
GLuint init3DNoiseTexture(int texSize, GLubyte* texPtr);
void make3DNoiseTexture();
void setNoiseFrequency(int frequency);
double noise1(double arg);
double noise2(double vec[2]);
double noise3(double vec[3]);
void normalize2(double v[2]);
void normalize3(double v[3]);
void initNoise();
double PerlinNoise1D(double x, double alpha, double beta, int n);
double PerlinNoise2D(double x, double y, double alpha, double beta, int n);
double PerlinNoise3D(double x, double y, double z, double alpha, double beta, int n);

#define MAXB 0x100
#define N 0x1000
#define NP 12   // 2^N
#define NM 0xfff

#define s_curve(t) ( t * t * (3. - 2. * t) )
#define lerp(t, a, b) ( a + t * (b - a) )
#define setup(i, b0, b1, r0, r1)\
        t = vec[i] + N;\
        b0 = ((int)t) & BM;\
        b1 = (b0+1) & BM;\
        r0 = t - (int)t;\
        r1 = r0 - 1.;
#define at2(rx, ry) ( rx * q[0] + ry * q[1] )
#define at3(rx, ry, rz) ( rx * q[0] + ry * q[1] + rz * q[2] )