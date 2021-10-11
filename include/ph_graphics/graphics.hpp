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
        Instance (String auto&& appname)
        {
            auto appinfo = VkApplicationInfo
            {
                .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
                .pApplicationName = ph::c_str (appname),
                .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
                .pEngineName = "No Engine",
                .engineVersion = VK_MAKE_VERSION(1, 0, 0),
                .apiVersion = VK_API_VERSION_1_0
            };
            
            
            uint32_t _extensCount = 0;
            const char** _extens;
            
            _extens = glfwGetRequiredInstanceExtensions (&_extensCount);
            
            
            
            auto extraExtens = extensions();
            std::cout << extraExtens.size() << std::endl;
//            std::terminate();
            auto extens = std::vector <char const*> {};
            // extens.resize(extraExtens.size());
            
//            extens.resize (extraExtens.size() + extens.size());
            
            for (auto& i : extraExtens) {
                extens.push_back (i.extensionName);
				std::cout << i.extensionName << std::endl;
            }

            for (auto& i : extens)
                std::cout << i << std::endl;
            auto info = VkInstanceCreateInfo
            {
                .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
                .pApplicationInfo = &appinfo,
                .enabledExtensionCount = static_cast <uint32_t> (extens.size()),
                .ppEnabledExtensionNames = extens.data(),
                .enabledLayerCount = 0
            };
                        
            auto result = vkCreateInstance (&info, nullptr, &m_instance);
            
            if (result != VK_SUCCESS)
            {
                throw std::runtime_error ("failed to create instance!");
            }
            
            
            
            
            
            uint32_t deviceCount = 0;
            
            vkEnumeratePhysicalDevices (m_instance, &deviceCount, nullptr);
            if (deviceCount == 0) {
                throw "no GPUs!!!";
            }
            m_physical_devices.resize (deviceCount);
            vkEnumeratePhysicalDevices(m_instance, &deviceCount, m_physical_devices.data());
        }
        
        ~Instance ()
        {
            vkDestroyInstance (m_instance, nullptr);
        }
        
        operator VkInstance& ()
        {
            return m_instance;
        }
        
        static auto extensions () -> std::vector <VkExtensionProperties>
        {
            uint32_t extensionCount = 0;
            vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
            
            std::vector<VkExtensionProperties> extensions (extensionCount);

            
            vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
            return extensions;
        }
        
        static auto layers () -> std::vector <VkLayerProperties>
        {
            uint32_t layerCount;
                vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

                std::vector<VkLayerProperties> availableLayers(layerCount);
                vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

                return availableLayers;
        }
        
        auto physicalDevices () -> std::vector <VkPhysicalDevice> &
        {
            return m_physical_devices;
        }
        
    public:
        VkInstance m_instance;
        std::vector <VkPhysicalDevice> m_physical_devices;
    };
    

    
    
    
    
    
    
    struct GPU
    {
        struct QueueFamily : VkQueueFamilyProperties, VkDeviceQueueCreateInfo
        {
            constexpr QueueFamily (uint32_t queueCount = 1) noexcept : VkQueueFamilyProperties {}, VkDeviceQueueCreateInfo {.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO, .queueCount = queueCount}
            {
                float queuePriority = 1.0f;
                VkDeviceQueueCreateInfo::pQueuePriorities = &queuePriority;
            }
            
            constexpr inline bool graphics () noexcept
            {
                return VkQueueFamilyProperties::queueFlags & VK_QUEUE_GRAPHICS_BIT;
            }
            
            constexpr inline bool compute () noexcept
            {
                return VkQueueFamilyProperties::queueFlags & VK_QUEUE_COMPUTE_BIT;
            }
            
            
        
        private:
            friend class GPU;
            
            constexpr auto set_index (uint32_t i) noexcept
            {
                VkDeviceQueueCreateInfo::queueFamilyIndex = i;
            }
        };
    
        GPU (VkPhysicalDevice& physicalDevice, Instance& instance) : m_instance {instance}, m_physicalDevice {physicalDevice}
        {
            uint32_t queueFamilyCount = 0;
            
            vkGetPhysicalDeviceQueueFamilyProperties (m_physicalDevice, &queueFamilyCount, nullptr);
            m_qFams.resize (queueFamilyCount);
            
            vkGetPhysicalDeviceQueueFamilyProperties (m_physicalDevice, &queueFamilyCount, m_qFams.data());
            
            for (int i = 0; i < m_qFams.size (); ++i)
            {
                m_qFams[i].set_index (i);
            }
            
            
            

            
            
            auto deviceFeatures = VkPhysicalDeviceFeatures {};
            
            
            
            auto createInfo = VkDeviceCreateInfo
            {
                .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
                .pQueueCreateInfos = m_qFams.data (),
                .queueCreateInfoCount = static_cast <uint32_t> (m_qFams.size ()),
                .pEnabledFeatures = &deviceFeatures,
                .enabledExtensionCount = 0,
                .enabledLayerCount = 0
            };
            
            
            
            if (vkCreateDevice (m_physicalDevice, &createInfo, nullptr, &m_device) != VK_SUCCESS) {
                throw std::runtime_error ("failed to create logical device!");
            }
        }
        
        ~GPU ()
        {
            vkDestroyDevice (m_device, nullptr);
        }
        
    private:
        
   
        
        operator VkPhysicalDevice& ()
        {
            return m_physicalDevice;
        }
    
        
        std::vector <QueueFamily> m_qFams {0};
        Instance& m_instance;
        VkPhysicalDevice& m_physicalDevice;
        VkDevice m_device;
      
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
        
        operator GLFWwindow& ()
        {
            return *m_window;
        }
        
        
    private:
        GLFWwindow* m_window;
    };
    
    
    struct Surface
    {
        Surface (Instance& instance, Window& window) : m_instance {instance}, m_window {window}
        {
            if (glfwCreateWindowSurface (instance, window, nullptr, &m_surface) != VK_SUCCESS)
            {
                throw std::runtime_error ("failed to create window surface!");
            }
        }
        
        ~Surface ()
        {
            vkDestroySurfaceKHR (m_instance, m_surface, nullptr);
        }
        
        
        
    private:
        Instance& m_instance;
        Window& m_window;
        VkSurfaceKHR m_surface;
    };
    
    
    
    
    struct Debugger
    {
        Debugger (Instance& instance) : m_instance {instance}
        {
            VkDebugUtilsMessengerCreateInfoEXT createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
            createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
            createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
            createInfo.pfnUserCallback = debugCallback;
            createInfo.pUserData = nullptr; // Optional
            
            if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &m_debugMessenger) != VK_SUCCESS) {
                throw std::runtime_error("failed to set up debug messenger!");
            }
        }
        
        VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
            auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
            if (func != nullptr) {
                return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
            } else {
                return VK_ERROR_EXTENSION_NOT_PRESENT;
            }
            
            
        }
        
        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData) {

            std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

            return VK_FALSE;
        }
        
        ~Debugger ()
        {
            DestroyDebugUtilsMessengerEXT (m_instance, m_debugMessenger, nullptr);
        }
        
        void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
            auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
            if (func != nullptr) {
                func(instance, debugMessenger, pAllocator);
            }
        }
        
    private:
        Instance& m_instance;
        VkDebugUtilsMessengerEXT m_debugMessenger;
    };
    
    
}
