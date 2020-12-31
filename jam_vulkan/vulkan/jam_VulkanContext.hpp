//
// Created by SachiSakurane on 2020/12/30.
//

#pragma once

#include <memory>

#include <juce_gui_extra/juce_gui_extra.h>

class VulkanContext {
public:
    class NativeContext;

    explicit VulkanContext(juce::Component& comp);
    ~VulkanContext();

private:
    std::unique_ptr<NativeContext> context;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VulkanContext)
};
