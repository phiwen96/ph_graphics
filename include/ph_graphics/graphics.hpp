#pragma once
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <ph_concepts/concepts.hpp>

namespace ph::graphics
{
   
    auto getdevices (auto& instance) -> std::vector <VkPhysicalDevice>
    {
        auto devices = std::vector <VkPhysicalDevice> {};
        uint32_t deviceCount = 0;
        vkEnumeratedevices (instance, &deviceCount, nullptr);
        
        if (devices.size() <= 0)
        {
            return {};
        }
        
        return std::move (devices);
    }
    
    auto instanceExtensions ()
    {
        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties (nullptr, &extensionCount, nullptr);
        std::vector <VkExtensionProperties> extensions (extensionCount);
        vkEnumerateInstanceExtensionProperties (nullptr, &extensionCount, extensions.data());
        
        return extensions;
    }
    
    
    struct Instance
    {
        Instance ()
        {
            auto appinfo = VkApplicationInfo
            {
                .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
                .pApplicationName = "Hello Triangle",
                .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
                .pEngineName = "No Engine",
                .engineVersion = VK_MAKE_VERSION(1, 0, 0),
                .apiVersion = VK_API_VERSION_1_0
            };
            
            
            uint32_t glfwExtensionCount = 0;
            const char** glfwExtensions;
            
            glfwExtensions = glfwGetRequiredInstanceExtensions (&glfwExtensionCount);
            
            
            
            auto info = VkInstanceCreateInfo
            {
                .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
                .pApplicationInfo = &appinfo,
                .enabledExtensionCount = glfwExtensionCount,
                .ppEnabledExtensionNames = glfwExtensions,
                .enabledLayerCount = 0
            };
            
            auto instance = VkInstance {};
            
            
            
            auto result = vkCreateInstance (&info, nullptr, &m_instance);
            
            if (result != VK_SUCCESS)
            {
                throw std::runtime_error ("failed to create instance!");
            }
        }
        
        ~Instance ()
        {
            vkDestroyInstance (m_instance, nullptr);
        }
        
        operator VkInstance& ()
        {
            return m_instance;
        }
        
    public:
        VkInstance m_instance;
    };
    

    
    
    
    
    
    
    struct GPU
    {
        GPU (auto& instance) : m_gpu {VK_NULL_HANDLE}
        {
            uint32_t deviceCount = 0;
            vkEnumeratePhysicalDevices (instance, &deviceCount, nullptr);
            if (deviceCount == 0) {
                throw "no GPUs!!!";
            }
            auto devices = std::vector<VkPhysicalDevice> {deviceCount};
            vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
            std::cout << deviceCount << std::endl;
        
            m_gpu = devices [0];
            if (m_gpu == VK_NULL_HANDLE) {
                throw std::runtime_error("failed to find a suitable GPU!");
            }
        }
        
        st
        
        auto queueFamilies ()
        {
            uint32_t queueFamilyCount = 0;
            vkGetPhysicalDeviceQueueFamilyProperties (m_gpu, &queueFamilyCount, nullptr);

            std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
            vkGetPhysicalDeviceQueueFamilyProperties (m_gpu, &queueFamilyCount, queueFamilies.data());
            
            
        }
        
        operator VkPhysicalDevice& ()
        {
            return m_gpu;
        }
    
    protected:
        VkPhysicalDevice m_gpu;
        std::vector <VkQueueFamilyProperties> m_queueFam;
    };
    
    
    
    
    
    
    struct Window
    {
        Window (auto width, auto height)
        {
            glfwWindowHint (GLFW_RESIZABLE, GLFW_FALSE);
            m_window = glfwCreateWindow (width, height, "Vulkan", nullptr, nullptr);
        }
        
        ~Window ()
        {
            glfwDestroyWindow (m_window);
        }
        
        auto open ()
        {
            while (!glfwWindowShouldClose (m_window))
            {
                    glfwPollEvents();
            }
        }
        
        operator GLFWwindow* ()
        {
            return m_window;
        }
        
    private:
        GLFWwindow* m_window;
    };
    
    
    
    
    
}
