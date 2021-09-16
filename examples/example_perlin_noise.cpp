#include <fstream>
#include <tinyutils/common.hpp>
#include <tinyutils/logging.hpp>
#include <tinyutils/perlin_noise.hpp>

int main() {
    tiny::utils::Logger::Init();
    tiny::utils::PerlinNoise::Init();

    const size_t width = 128;
    const size_t height = 128;
    const size_t num_bytes = 3 * width * height;
    auto data = std::make_unique<uint8_t[]>(num_bytes);
    for (size_t i = 0; i < height; i++) {
        for (size_t j = 0; j < width; j++) {
            float noise_value =
                100.0f * tiny::utils::PerlinNoise::Sample2d(j, i);
            data[3 * (j + i * width) + 0] = std::max(0.0f, noise_value);
            data[3 * (j + i * width) + 1] = std::max(0.0f, noise_value);
            data[3 * (j + i * width) + 2] = std::max(0.0f, noise_value);
        }
    }

    std::ofstream f_handle("perlin_noise_image.ppm", std::ofstream::binary);
    f_handle << "P6\n" << width << " " << height << "\n255\n";
    f_handle.write((const char*)data.get(), num_bytes);
    f_handle.close();

    tiny::utils::PerlinNoise::Release();
    tiny::utils::Logger::Release();
    return 0;
}
