//
// Created by SachiSakurane on 2020/12/30.
//

#pragma once

#include <memory>

#include <juce_gui_extra/juce_gui_extra.h>

class VulkanContext {
public:
    VulkanContext();
    ~VulkanContext();

    void setRenderer(VulkanRenderer* renderer);
    void attachTo (juce::Component&);
    void detach();
    
    void setContinuousRepainting(bool flag);

private:
    class NativeContext;
    
    class CachedImage;
    class Attachment;

    std::unique_ptr<CachedImage> cachedImage {nullptr};
    std::unique_ptr<Attachment> attachment {nullptr};
    VulkanRenderer* renderer {nullptr};

    juce::Component* getTargetComponent() const noexcept;
    void triggerRepaint();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VulkanContext)
};
