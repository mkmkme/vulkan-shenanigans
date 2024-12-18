# Treat Vulkan as a module.
# The code is taken from https://github.com/KhronosGroup/Vulkan-Hpp/blob/main/README.md#usage-with-cmake
add_library(VulkanHppModule)
target_sources(VulkanHppModule PUBLIC
  FILE_SET CXX_MODULES
  BASE_DIRS ${Vulkan_INCLUDE_DIR}
  FILES ${Vulkan_INCLUDE_DIR}/vulkan/vulkan.cppm
)
target_link_libraries(VulkanHppModule PUBLIC Vulkan::Vulkan)

add_executable(vulkan_shenanigans)
target_sources(vulkan_shenanigans PUBLIC
    main.cpp)
target_sources(vulkan_shenanigans PUBLIC
    FILE_SET CXX_MODULES
    FILES
      HelloTriangleApplication.cppm
      HelloTriangleApplicationConstants.cppm
      HelloTriangleApplicationDebug.cppm)
target_link_libraries(vulkan_shenanigans PRIVATE glfw VulkanHppModule)
