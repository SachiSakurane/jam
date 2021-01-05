//
// Created by SachiSakurane on 2020/12/30.
//

#include "jam_VulkanContext.hpp"

class VulkanComponent : public juce::Component, private VulkanRenderer {
public:
    VulkanComponent();
    ~VulkanComponent();

    void initialize();

    VulkanContext context;

private:

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VulkanComponent)
};
