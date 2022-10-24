#ifndef NOISE_H
#define NOISE_H

#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <QImage>
#include <QDebug>

namespace noise {

class vec2i {
public:
    int x, y;
    vec2i() {}
    vec2i(int x, int y) {this->x = x; this->y = y;}
};

class vec2f {
public:
    float x, y;
    vec2f() {}
    vec2f(float x, float y) {this->x = x; this->y = y;}
};

class PerlinNoise {
private:
    std::vector<std::vector<vec2f>> gradient_table;
    int accuracy, interpolation;
    int OldWidth, OldHeight;

    float Curve(float t, uint type);
    float Cubic(float t) {return -2 * t * t * t + 3 * t * t;}
    float Cosine(float t) {return (1 - cos(t * M_PI)) / 2;}
    float Quintic(float t) {return t * t * t * (t * (t * 6 - 15) + 10);}

public:
    static const uint LINEAR = 0, CUBIC = 1, COSINE = 2, QUINTIC = 3;

    float Dot(vec2f a, vec2f b) {return a.x * b.x + a.y * b.y;}
    float Lerp(float a, float b, float value) {return a + (b - a) * value;}
    vec2f getRandomGradientVector();
    void initNoise(int width, int height, int accuracy, int seed, uint interpolation = QUINTIC); //accuracy - кол - во точек интерполяции в одной ячейке
    float noisePoint(float px, float py);
    QImage getNoiseTexture();
};

}

#endif // NOISE_H
