#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include <ph_graphics/graphics.hpp>


using std::cout, std::endl;

using namespace ph::graphics;


auto a () noexcept (false)
{
    cout << "a" << endl;
}

auto b (auto f) noexcept (f ())
{
    f ();
}

struct c
{
    ~c()
    {
        cout << "C" << endl;
    }
};

struct d : c
{
    ~d()
    {
        cout << "D" << endl;
    }
};

int main (int, char**) 
{
    glfwInit ();
    auto aa = d {};
    
    auto instance = Instance {"app0"};
    auto debugger = Debugger {instance};
//    auto gpu = GPU {instance};
    auto window = Window {100, 100};
    
    auto& gpus = instance.physicalDevices();
    
    auto gpu = GPU {gpus [0], instance};
    
    
    
//    auto surface = Surface (instance, window);
//    window.open ();
    cout << "hej" << endl;
    
    
    
    
//    glfwTerminate ();
    return 0;
}
