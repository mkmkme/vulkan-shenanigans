module;

#define GLFW_INCLUDE_VULKAN
#include <iostream>

#include <GLFW/glfw3.h>

import vulkan_hpp;

export module HelloTriangleApplication;

export class HelloTriangleApplication {
public:
    void run()
    {
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }

private:
    void initWindow()
    {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan window", nullptr, nullptr);
    }

    void initVulkan() { createInstance(); }

    void createInstance()
    {
        vk::ApplicationInfo appInfo {};
        appInfo.sType = vk::StructureType::eApplicationInfo;
        appInfo.pApplicationName = "Hello Triangle";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        vk::InstanceCreateInfo createInfo {};
        createInfo.sType = vk::StructureType::eInstanceCreateInfo;
        createInfo.pApplicationInfo = &appInfo;

        uint32_t glfwExtensionCount = 0;
        const char **glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

#if 1
        std::cout << "GLFW extensions: ";
        for (uint32_t i = 0; i < glfwExtensionCount; i++) {
            std::cout << glfwExtensions[i] << ' ';
        }
        std::cout << std::endl;

        uint32_t extensionCount = 0;
        (void) vk::enumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
        std::vector<vk::ExtensionProperties> extensions(extensionCount);
        (void) vk::enumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
        std::cout << "available vulkan extensions: ";
        for (const auto &extension : extensions) {
            std::cout << extension.extensionName << " ";
        }
        std::cout << std::endl;
#endif

        createInfo.enabledExtensionCount = glfwExtensionCount;
        createInfo.ppEnabledExtensionNames = glfwExtensions;

        createInfo.enabledLayerCount = 0;

        vk::Result result = vk::createInstance(&createInfo, nullptr, &instance);
        if (result != vk::Result::eSuccess) {
            throw std::runtime_error("failed to create instance!");
        }
    }

    void mainLoop()
    {
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
        }
    }

    void cleanup()
    {
        vkDestroyInstance(instance, nullptr);
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;
    GLFWwindow *window;
    vk::Instance instance;
};
