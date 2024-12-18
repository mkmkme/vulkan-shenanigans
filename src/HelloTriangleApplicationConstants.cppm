module;

#include <cstdint>
#include <vector>

export module HelloTriangleApplication:Constants;

export namespace Constants {
const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;
const std::vector<const char *> validationLayers = { "VK_LAYER_KHRONOS_validation" };

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

} // namespace Constants
