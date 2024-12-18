module;

#define GLFW_INCLUDE_VULKAN
#include <cstring>
#include <iostream>

#include <GLFW/glfw3.h>

export module HelloTriangleApplication;

import vulkan_hpp;

import :Constants;
import :Debug;

export class HelloTriangleApplication {
private:
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
        window = glfwCreateWindow(Constants::WIDTH, Constants::HEIGHT, "Vulkan window", nullptr, nullptr);
    }

    void initVulkan()
    {
        createInstance();
        setupDebugMessenger();
    }

    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo)
    {
        createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = Debug::callback;
    }

    void setupDebugMessenger()
    {
        if (!Constants::enableValidationLayers) {
            return;
        }

        VkDebugUtilsMessengerCreateInfoEXT createInfo {};
        populateDebugMessengerCreateInfo(createInfo);

        const VkResult result = Debug::CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger);
        if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to set up debug messenger!");
        }
    }

    std::vector<const char *> getRequiredExtensions()
    {
        uint32_t glfwExtensionCount = 0;
        const char **glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

        if (Constants::enableValidationLayers) {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }
#if 1
        std::cout << "GLFW extensions: ";
        for (uint32_t i = 0; i < glfwExtensionCount; i++) {
            std::cout << glfwExtensions[i] << ' ';
        }
        std::cout << std::endl;

        uint32_t extensionCount = 0;
        (void) vk::enumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
        std::vector<vk::ExtensionProperties> vkExtensions(extensionCount);
        (void) vk::enumerateInstanceExtensionProperties(nullptr, &extensionCount, vkExtensions.data());
        std::cout << "available vulkan extensions: ";
        for (const auto &extension : vkExtensions) {
            std::cout << extension.extensionName << " ";
        }
        std::cout << std::endl;
#endif

        return extensions;
    }

    bool checkValidationLayerSupport()
    {
        uint32_t layerCount;
        (void) vk::enumerateInstanceLayerProperties(&layerCount, nullptr);
        std::vector<vk::LayerProperties> availableLayers(layerCount);
        (void) vk::enumerateInstanceLayerProperties(&layerCount, availableLayers.data());

#if 1
        std::cout << "available validation layers: ";
        for (const auto &layer : availableLayers) {
            std::cout << layer.layerName << " ";
        }
        std::cout << std::endl;
#endif

        for (const auto &layerName : Constants::validationLayers) {
            bool layerFound = false;

            for (const auto &layerProperties : availableLayers) {
                if (strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound) {
                return false;
            }
        }

        return true;
    }

    void createInstance()
    {
        std::cout << "validation layers enabled: " << Constants::enableValidationLayers << std::endl;
        if (Constants::enableValidationLayers && !checkValidationLayerSupport()) {
            throw std::runtime_error("validation layers requested, but not available!");
        }
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

        auto extensions = getRequiredExtensions();
        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();

        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo {};
        if (Constants::enableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(Constants::validationLayers.size());
            createInfo.ppEnabledLayerNames = Constants::validationLayers.data();

            populateDebugMessengerCreateInfo(debugCreateInfo);
            createInfo.pNext = reinterpret_cast<VkDebugUtilsMessengerCreateInfoEXT *>(&debugCreateInfo);
        } else {
            createInfo.enabledLayerCount = 0;
            createInfo.pNext = nullptr;
        }

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
        if (Constants::enableValidationLayers) {
            Debug::DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
        }
        vkDestroyInstance(instance, nullptr);
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    GLFWwindow *window;
    vk::Instance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
};
