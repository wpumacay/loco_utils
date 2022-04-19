#include <fstream>
#include <loco/utils/logging.hpp>
#include <loco/utils/perlin_noise.hpp>

auto main() -> int {
    loco::utils::Logger::Init();
    loco::utils::PerlinNoise::Init();

    constexpr size_t WIDTH = 128;
    constexpr size_t HEIGHT = 128;
    constexpr size_t NUM_BYTES = 3 * WIDTH * HEIGHT;
    auto buffer = std::array<uint8_t, NUM_BYTES>();
    for (size_t i = 0; i < HEIGHT; i++) {
        for (size_t j = 0; j < WIDTH; j++) {
            float noise_value =
                100.0F * loco::utils::PerlinNoise::Sample2d(
                             static_cast<float>(j), static_cast<float>(i));
            buffer.at(3 * (j + i * WIDTH) + 0) =
                static_cast<uint8_t>(std::max(0.0F, noise_value));
            buffer.at(3 * (j + i * WIDTH) + 1) =
                static_cast<uint8_t>(std::max(0.0F, noise_value));
            buffer.at(3 * (j + i * WIDTH) + 2) =
                static_cast<uint8_t>(std::max(0.0F, noise_value));
        }
    }

    std::ofstream f_handle("perlin_noise_image.ppm", std::ofstream::binary);
    f_handle << "P6\n" << WIDTH << " " << HEIGHT << "\n255\n";
    // NOLINTNEXTLINE : allow just for this
    f_handle.write(reinterpret_cast<const char*>(buffer.data()), NUM_BYTES);
    f_handle.close();

    loco::utils::PerlinNoise::Release();
    loco::utils::Logger::Release();
    return 0;
}
