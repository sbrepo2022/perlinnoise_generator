#include "noise.h"

float noise::PerlinNoise::Curve(float t, uint type) {
    switch (type) {
        case LINEAR:
          return t;

        case CUBIC:
          return Cubic(t);

        case COSINE:
          return Cosine(t);

        case QUINTIC:
          return Quintic(t);
    }
}

noise::vec2f noise::PerlinNoise::getRandomGradientVector() {
    int num;
    vec2f node_vec;

    num = rand() % 4;
    switch (num) {
        case 0:
            node_vec = vec2f(1.0f, 0.0f);
            break;

        case 1:
            node_vec = vec2f(-1.0f, 0.0f);
            break;

        case 2:
            node_vec = vec2f(0.0f, 1.0f);
            break;

        case 3:
            node_vec = vec2f(0.0f, -1.0f);
            break;
    }
    return node_vec;
}

void noise::PerlinNoise::initNoise(int width, int height, int accuracy, int seed, uint interpolation) {
    srand(seed);

    this->accuracy = accuracy;
    this->interpolation = interpolation;
    gradient_table.clear();
    OldHeight = height;
    OldWidth = width;

    std::vector<vec2f> line;
    for (int i = 0; i < width + 1; i++) {
        for (int j = 0; j < height + 1; j++) {
            line.push_back(getRandomGradientVector());
        }
        gradient_table.push_back(line);
        line.clear();
    }
}

float noise::PerlinNoise::noisePoint(float px, float py) {
    // префиксы i - int, l - local, p - point, c - curved
    int ix = (int)px;
    int iy = (int)py;

    float lx = px - (float)ix;
    float ly = py - (float)iy;

    vec2f LeftTopGradient = gradient_table[ix][iy];
    vec2f LeftBottomGradient = gradient_table[ix][iy + 1];
    vec2f RightTopGradient = gradient_table[ix + 1][iy];
    vec2f RightBottomGradient = gradient_table[ix + 1][iy + 1];

    vec2f LeftTopDist = vec2f(lx, ly);
    vec2f LeftBottomDist = vec2f(lx, ly - 1.0f);
    vec2f RightTopDist = vec2f(lx - 1.0f, ly);
    vec2f RightBottomDist = vec2f(lx - 1.0f, ly - 1.0f);


    float LeftTopDot = Dot(LeftTopGradient, LeftTopDist);
    float LeftBottomDot = Dot(LeftBottomGradient, LeftBottomDist);
    float RightTopDot = Dot(RightTopGradient, RightTopDist);
    float RightBottomDot = Dot(RightBottomGradient, RightBottomDist);

    float cx = Curve(lx, this->interpolation);
    float cy = Curve(ly, this->interpolation);

    float LerpedTop = Lerp(LeftTopDot, RightTopDot, cx);
    float LerpedBottom = Lerp(LeftBottomDot, RightBottomDot, cx);
    float HeightValue = Lerp(LerpedTop, LerpedBottom, cy);

    return HeightValue;
}

QImage noise::PerlinNoise::getNoiseTexture() {
    int imgWidth = OldWidth * accuracy;
    int imgHeight = OldHeight * accuracy;
    QImage texture(imgWidth, imgHeight, QImage::Format_ARGB32 );
    float HeightValue = 0.0f;

    for (int x = 0; x < imgWidth; x++) {
        for (int y = 0; y < imgHeight; y++) {
            HeightValue = noisePoint((float)x / (float)accuracy, (float)y / (float)accuracy);
            HeightValue = ((HeightValue + 1.0f) / 2) * 255;
            texture.setPixel(x, y, qRgb(HeightValue, HeightValue, HeightValue));
        }
    }
    return texture;
}

