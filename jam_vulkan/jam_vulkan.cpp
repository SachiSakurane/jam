//
// Created by SachiSakurane on 2020/12/31.
//

#ifdef JAM_VULKAN_H_INCLUDED
/* When you add this cpp file to your project, you mustn't include it in a file where you've
    already included any other headers - just put it inside a file on its own, possibly with your config
    flags preceding it, but don't include anything else. That also includes avoiding any automatic prefix
    header files that the compiler may be using.
 */
    #error "Incorrect use of JUCE cpp file"
#endif

#include "jam_vulkan.h"

#if JUCE_MAC
    #import <QuartzCore/CAMetalLayer.h>
    // #include <MoltenVK/mvk_vulkan.h>
#endif

#if JUCE_MAC
    #include "native/jam_NativeContext_osx.h"
#endif

#include "vulkan/jam_VulkanComponent.cpp"
#include "vulkan/jam_VulkanContext.cpp"
