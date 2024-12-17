
add_executable(vulkan_shenanigans main.cpp)
target_link_libraries(vulkan_shenanigans PRIVATE glfw Vulkan::Vulkan)
