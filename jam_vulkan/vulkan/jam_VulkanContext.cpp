//
// Created by SachiSakurane on 2020/12/30.
//

class VulkanContext::CachedImage
        : public juce::CachedComponentImage,
          private juce::ThreadPoolJob {
public:
    CachedImage (VulkanContext& c, juce::Component& comp)
              : juce::ThreadPoolJob {"Vulkan Rendering"}, context {c}, component {comp},
              nativeContext{std::make_unique<NativeContext>(component)} {
    }
    
    ~CachedImage () {
        stop();
    }
    
    void start() {
        if (nativeContext) {
            renderThread = std::make_unique<juce::ThreadPool>(1);
            resume();
        }
    }
              
    void stop() {
        if (renderThread) {

        }
    }

    void resume()
    {
      if (renderThread != nullptr)
          renderThread->addJob (this, false);
    }
              
    void triggerRepaint()
    {
        needsUpdate = true;
        repaintEvent.signal();
    }

    void paint (juce::Graphics&) override
    {
        updateViewportSize (false);
    }

    bool invalidateAll() override
    {
        validArea.clear();
        triggerRepaint();
        return false;
    }

    bool invalidate (const juce::Rectangle<int>& area) override
    {
        validArea.subtract (area.toFloat().transformedBy (transform).getSmallestIntegerContainer());
        triggerRepaint();
        return false;
    }

    void releaseResources() override
    {
        stop();
    }

private:
    VulkanContext& context;
    juce::Component& component;
    
    std::unique_ptr<juce::ThreadPool> renderThread;
    std::unique_ptr<NativeContext> nativeContext {nullptr};
    std::atomic<bool> needsUpdate{ false };

    juce::WaitableEvent repaintEvent;

#if JUCE_MAC
    struct CVDisplayLinkWrapper {
        CVDisplayLinkWrapper (CachedImage* im)
        {
            CVDisplayLinkCreateWithActiveCGDisplays (&displayLink);
            CVDisplayLinkSetOutputCallback (displayLink, &displayLinkCallback, im);
            CVDisplayLinkStart (displayLink);
        }

        ~CVDisplayLinkWrapper()
        {
            CVDisplayLinkStop (displayLink);
            CVDisplayLinkRelease (displayLink);
        }

        static CVReturn displayLinkCallback (CVDisplayLinkRef, const CVTimeStamp*, const CVTimeStamp*,
                                             CVOptionFlags, CVOptionFlags*, void* displayLinkContext)
        {
            auto* self = (CachedImage*) displayLinkContext;
            self->repaintEvent.signal();
            return kCVReturnSuccess;
        }

        CVDisplayLinkRef displayLink;
     };

    std::unique_ptr<CVDisplayLinkWrapper> cvDisplayLinkWrapper;
#endif

    JobStatus runJob() override {
        return ThreadPoolJob::jobHasFinished;
    }
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CachedImage)
};

class VulkanContext::Attachment : public juce::ComponentMovementWatcher {
public:
    Attachment (VulkanContext& c, juce::Component& comp)
            : juce::ComponentMovementWatcher {&comp}, context {c}
    {
        if (canBeAttached (comp))
            attach();
    }

    ~Attachment() override
    {
        detach();
    }
    
    using ComponentMovementWatcher::componentMovedOrResized;

    void componentMovedOrResized (bool /*wasMoved*/, bool /*wasResized*/) override
    {
        auto& comp = *getComponent();

        if (isAttached (comp) != canBeAttached (comp))
            componentVisibilityChanged();

        if (comp.getWidth() > 0 && comp.getHeight() > 0)
        {
//            if (auto* c = CachedImage::get (comp))
//                c->handleResize();

            if (auto* peer = comp.getTopLevelComponent()->getPeer())
                context.nativeContext->updateWindowPosition (peer->getAreaCoveredBy (comp));
        }
    }

    void componentPeerChanged() override
    {
        detach();
        componentVisibilityChanged();
    }

    void componentVisibilityChanged() override
    {
        if (auto& comp = *getComponent(); canBeAttached (comp))
        {
            if (isAttached (comp))
                comp.repaint(); // (needed when windows are un-minimised)
            else
                attach();
        }
        else
        {
            detach();
        }
    }

private:
    VulkanContext& context;

    bool canBeAttached (const juce::Component& comp) noexcept
    {
        return comp.getWidth() > 0 && comp.getHeight() > 0 && isShowingOrMinimised (comp);
    }

    static bool isShowingOrMinimised (const juce::Component& c)
    {
        if (! c.isVisible())
            return false;

        if (auto* p = c.getParentComponent())
            return isShowingOrMinimised (*p);

        return c.getPeer() != nullptr;
    }

    static bool isAttached (const juce::Component& comp) noexcept
    {
        return comp.getCachedComponentImage() != nullptr;
    }
    
    void attach()
    {
        start();
    }

    void detach()
    {
        auto& comp = *getComponent();
        stop();
        comp.setCachedComponentImage (nullptr);
    }
    
    void start() {}
    void stop() {}
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Attachment)
};

VulkanContext::VulkanContext() {
}

VulkanContext::~VulkanContext() {
}

void VulkanContext::setRenderer(VulkanRenderer *r) {
    if (r) {
        renderer = r;
    }
}

void VulkanContext::attachTo(juce::Component& component) {
    component.repaint();

    if (getTargetComponent() != &component)
    {
        detach();
        attachment = std::make_unique<Attachment>(*this, component);
        cachedImage = std::make_unique<CachedImage>(*this, component);
        component.setCachedComponentImage (cachedImage.get());
    }
}

void VulkanContext::detach() {
    attachment.reset();
    cachedImage.reset();
}

juce::Component* VulkanContext::getTargetComponent() const noexcept {
    return attachment != nullptr ? attachment->getComponent() : nullptr;
}

void VulkanContext::setContinuousRepainting(bool flag) {
    
}

void VulkanContext::triggerRepaint() {
    if (cachedImage)
        cachedImage->triggerRepaint();
}
