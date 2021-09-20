#pragma once

#include <memory>
#include <random>
#include <tinyutils/logging.hpp>
#include <utility>
#include <vector>

constexpr float PERLIN_NOISE_RAND_MIN = -10000.0f;
constexpr float PERLIN_NOISE_RAND_MAX = 10000.0f;

namespace tiny {
namespace utils {

class PerlinNoise {
 public:
    /// Initializes the perlin-noise generator module(singleton)
    static void Init();

    /// Releases perlin-noise generator module's resources
    static void Release();

    /// Configures the noise-generator with the given settings
    static void Config(size_t num_octaves, float persistance, float lacunarity,
                       float noise_scale);

    /// Returns the noise value at a given 1d position
    static float Sample1d(float x);

    /// Returns the noise value at a given 2d position
    static float Sample2d(float x, float y);

    /// Releases the resources allocated for the noise-generator
    ~PerlinNoise() = default;

    /// Default number of octaves of the noise-generator
    static constexpr size_t DEFAULT_NUM_OCTAVES = 4;
    /// Default persistance setting of the noise generator
    static constexpr float DEFAULT_PERSISTANCE = 0.5f;
    /// Default lacunarity setting of the noise generator
    static constexpr float DEFAULT_LACUNARITY = 2.0f;
    /// Default noise-scale setting of the noise generator
    static constexpr float DEFAULT_NOISE_SCALE = 10.0f;

 private:
    /// Creates a noise-generator and allocates the required resources
    PerlinNoise() = default;

    /// Configures the noise-generator with the given settings
    void _Config(size_t num_octaves, float persistance, float lacunarity,
                 float noise_scale);

    /// Returns the noise value at a given 1d position
    float _Sample1d(float x);

    /// Returns the noise value at a given 2d position
    float _Sample2d(float x, float y);

    /// Computes the fade-function (perlin-noise helper-function)
    float _Fade(float t);

    /// Computes the lerp-function (perlin-noise helper function)
    float _Lerp(float a, float b, float t);

    /// Computes the product with the gradient at a given point (perlin-noise
    /// helper function)
    float _DotGrad(size_t hash, float x, float y);

    /// Computes the perlin-function at a given 2d position
    float _Perlin(float x, float y);

 private:
    /// Handle to instance of the noise-generator module(singleton)
    static std::unique_ptr<PerlinNoise> s_Instance;
    /// Number of octaves used for the noise generator
    size_t m_NumOctaves;
    /// Persistance parameter for the noise generator
    float m_Persistance;
    /// Lacunarity parameter for the noise generator
    float m_Lacunarity;
    /// Scaler for the generated noise
    float m_NoiseScale;
    /// Permutations used for noise generation
    std::vector<size_t> m_Permutations;
    /// Random offsets used for noise generation
    std::vector<std::pair<float, float>> m_OctavesOffsets;
    /// Create C++ random-engine
    std::default_random_engine m_RandEngine;
    /// Create C++ uniform real distribution
    std::uniform_real_distribution<float> m_RandUnifDist;
};

}  // namespace utils
}  // namespace tiny
