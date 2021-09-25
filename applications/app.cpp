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

int main (int, char**) 
{
    glfwInit ();
    
    auto instance = Instance {};
    auto gpu = GPU {instance};
    auto window = Window {100, 100};
    window.open ();
    cout << "hej" << endl;
    
    
    
    
    glfwTerminate ();
    return 0;
}
