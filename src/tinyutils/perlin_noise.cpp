
#include <tinyutils/perlin_noise.hpp>

namespace tiny {
namespace utils {

std::unique_ptr<PerlinNoise> PerlinNoise::s_Instance = nullptr;

void PerlinNoise::Init() {
    if (!s_Instance)
        s_Instance = std::unique_ptr<PerlinNoise>(new PerlinNoise());

    s_Instance->m_NumOctaves = DEFAULT_NUM_OCTAVES;
    s_Instance->m_Persistance = DEFAULT_PERSISTANCE;
    s_Instance->m_Lacunarity = DEFAULT_LACUNARITY;
    s_Instance->m_NoiseScale = DEFAULT_NOISE_SCALE;

    s_Instance->m_OctavesOffsets.clear();
    for (size_t o = 0; o < s_Instance->m_NumOctaves; o++)
        s_Instance->m_OctavesOffsets.push_back(
            std::make_pair(UNIF_RANDOM(-10000.0f, 10000.0f),
                           UNIF_RANDOM(-10000.0f, 10000.0f)));

    // Initialize the permutation vector with ken perlin's reference permutation
    // and double it to avoid overflow
    s_Instance->m_Permutations = {
        151, 160, 137, 91,  90,  15,  131, 13,  201, 95,  96,  53,  194, 233,
        7,   225, 140, 36,  103, 30,  69,  142, 8,   99,  37,  240, 21,  10,
        23,  190, 6,   148, 247, 120, 234, 75,  0,   26,  197, 62,  94,  252,
        219, 203, 117, 35,  11,  32,  57,  177, 33,  88,  237, 149, 56,  87,
        174, 20,  125, 136, 171, 168, 68,  175, 74,  165, 71,  134, 139, 48,
        27,  166, 77,  146, 158, 231, 83,  111, 229, 122, 60,  211, 133, 230,
        220, 105, 92,  41,  55,  46,  245, 40,  244, 102, 143, 54,  65,  25,
        63,  161, 1,   216, 80,  73,  209, 76,  132, 187, 208, 89,  18,  169,
        200, 196, 135, 130, 116, 188, 159, 86,  164, 100, 109, 198, 173, 186,
        3,   64,  52,  217, 226, 250, 124, 123, 5,   202, 38,  147, 118, 126,
        255, 82,  85,  212, 207, 206, 59,  227, 47,  16,  58,  17,  182, 189,
        28,  42,  223, 183, 170, 213, 119, 248, 152, 2,   44,  154, 163, 70,
        221, 153, 101, 155, 167, 43,  172, 9,   129, 22,  39,  253, 19,  98,
        108, 110, 79,  113, 224, 232, 178, 185, 112, 104, 218, 246, 97,  228,
        251, 34,  242, 193, 238, 210, 144, 12,  191, 179, 162, 241, 81,  51,
        145, 235, 249, 14,  239, 107, 49,  192, 214, 31,  181, 199, 106, 157,
        184, 84,  204, 176, 115, 121, 50,  45,  127, 4,   150, 254, 138, 236,
        205, 93,  222, 114, 67,  29,  24,  72,  243, 141, 128, 195, 78,  66,
        215, 61,  156, 180};
    s_Instance->m_Permutations.insert(s_Instance->m_Permutations.end(),
                                      s_Instance->m_Permutations.begin(),
                                      s_Instance->m_Permutations.end());
}

void PerlinNoise::Release() { s_Instance = nullptr; }

void PerlinNoise::Config(size_t num_octaves, float persistance,
                         float lacunarity, float noise_scale) {
    LOG_CORE_ASSERT(s_Instance,
                    "PerlinNoise::Config >>> Must initialize perlin-noise "
                    "module before using it");
    s_Instance->_Config(num_octaves, persistance, lacunarity, noise_scale);
}

float PerlinNoise::Sample1d(float x) {
    LOG_CORE_ASSERT(s_Instance,
                    "PerlinNoise::Sample1d >>> Must initialize perlin-noise "
                    "module before using it");
    return s_Instance->_Sample1d(x);
}

float PerlinNoise::Sample2d(float x, float y) {
    LOG_CORE_ASSERT(s_Instance,
                    "PerlinNoise::Sample2d >>> Must initialize perlin-noise "
                    "module before using it");
    return s_Instance->_Sample2d(x, y);
}

void PerlinNoise::_Config(size_t num_octaves, float persistance,
                          float lacunarity, float noise_scale) {
    m_NumOctaves = num_octaves;
    m_Persistance = persistance;
    m_Lacunarity = lacunarity;
    m_NoiseScale = noise_scale;

    m_OctavesOffsets.clear();
    for (size_t o = 0; o < m_NumOctaves; o++)
        m_OctavesOffsets.push_back(
            std::make_pair(UNIF_RANDOM(-10000.0f, 10000.0f),
                           UNIF_RANDOM(-10000.0f, 10000.0f)));
}

float PerlinNoise::_Sample1d(float x) { return _Sample2d(x, 0.0f); }

float PerlinNoise::_Sample2d(float x, float y) {
    float ampl = 1.0f;
    float freq = 1.0f;
    float noise_value = 0.0f;

    for (size_t i = 0; i < m_NumOctaves; i++) {
        float sample_x = freq * (x / m_NoiseScale) + m_OctavesOffsets[i].first;
        float sample_y = freq * (y / m_NoiseScale) + m_OctavesOffsets[i].second;
        noise_value += ampl * _Perlin(sample_x, sample_y);
        ampl *= m_Persistance;
        freq *= m_Lacunarity;
    }
    return noise_value;
}

float PerlinNoise::_Fade(float t) {
    return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
}

float PerlinNoise::_Lerp(float a, float b, float t) {
    return (1 - t) * a + t * b;
}

float PerlinNoise::_DotGrad(size_t hash, float x, float y) {
    // Because we are in 2d, there are just 4 options for the gradient vectors
    // and so, just 4 options for the dot product
    switch (hash & 0xf) {
        case 0x0:
            return x + y;
        case 0x1:
            return -x + y;
        case 0x2:
            return x - y;
        case 0x3:
            return -x - y;
        case 0x4:
            return x + y;
        case 0x5:
            return -x + y;
        case 0x6:
            return x - y;
        case 0x7:
            return -x - y;
        case 0x8:
            return x + y;
        case 0x9:
            return -x + y;
        case 0xa:
            return x - y;
        case 0xb:
            return -x - y;
        case 0xc:
            return x + y;
        case 0xd:
            return -x + y;
        case 0xe:
            return x - y;
        case 0xf:
            return -x - y;
    }

    return 0.0f;
}

float PerlinNoise::_Perlin(float x, float y) {
    // Calculate unit square position in grid (wrap around by 256)
    size_t X = (size_t)floor(x) & 255;
    size_t Y = (size_t)floor(y) & 255;

    // Calculate relative position [0-1] inside unit-square
    float x_f = x - floor(x);
    float y_f = y - floor(y);

    // Smooth using easing-function for smooth interpolation
    float u = _Fade(x_f);
    float v = _Fade(y_f);

    size_t hash_00 = m_Permutations[m_Permutations[X] + Y];
    size_t hash_01 = m_Permutations[m_Permutations[X] + Y + 1];
    size_t hash_10 = m_Permutations[m_Permutations[X + 1] + Y];
    size_t hash_11 = m_Permutations[m_Permutations[X + 1] + Y + 1];

    float d_00 = _DotGrad(hash_00, x_f, y_f);
    float d_01 = _DotGrad(hash_01, x_f, y_f - 1.0f);
    float d_10 = _DotGrad(hash_10, x_f - 1.0f, y_f);
    float d_11 = _DotGrad(hash_11, x_f - 1.0f, y_f - 1.0f);

    return _Lerp(_Lerp(d_00, d_10, u), _Lerp(d_01, d_11, u), v);
}

}  // namespace utils
}  // namespace tiny
