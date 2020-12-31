//
// Created by SachiSakurane on 2020/12/30.
//

VulkanContext::VulkanContext(juce::Component& comp) : context {std::make_unique<NativeContext>(comp)} {
}

VulkanContext::~VulkanContext() = default;
