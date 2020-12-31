//
// Created by SachiSakurane on 2020/12/30.
//

#import <AppKit/AppKit.h>
#import <QuartzCore/CAMetalLayer.h>

#include <juce_gui_extra/juce_gui_extra.h>
#include <juce_graphics/native/juce_mac_CoreGraphicsHelpers.h>

#if JUCE_MAC
static CVReturn DisplayLinkCallback(CVDisplayLinkRef displayLink,
                                    const CVTimeStamp* now,
                                    const CVTimeStamp* outputTime,
                                    CVOptionFlags flagsIn,
                                    CVOptionFlags* flagsOut,
                                    void* target) {
   //((ShellMVK*)target)->update_and_draw();
    return kCVReturnSuccess;
}

@interface VulkanViewController : NSViewController
@end

@implementation VulkanViewController {
    CVDisplayLinkRef    displayLink;
}

-(void) dealloc {
    CVDisplayLinkRelease(displayLink);
    [super dealloc];
}

-(void) viewDidLoad {
    [super viewDidLoad];

    self.view.wantsLayer = YES;        // Back the view with a layer created by the makeBackingLayer method.

//    std::vector<std::string> args;
//    args.push_back("-p");           // Uncomment to use push constants
////  args.push_back("-s");           // Uncomment to use a single thread
//    _game = new Hologram(args);
//
//    _shell = new ShellMVK(*_game);
//    _shell->run(self.view.layer);

    CVDisplayLinkCreateWithActiveCGDisplays(&displayLink);
    CVDisplayLinkSetOutputCallback(displayLink, &DisplayLinkCallback, nullptr);
    CVDisplayLinkStart(displayLink);
}
@end

class VulkanContext::NativeContext {
public:
    explicit NativeContext(juce::Component& component) : component{component} {
        if (auto* peer = component.getPeer()) {
            auto bounds = peer->getAreaCoveredBy (component);

            controller = [[VulkanViewController alloc] init];
            controller.view.frame = juce::convertToCGRect (bounds);

            [((NSView*) peer->getNativeHandle()) addSubview:controller.view];
        }
    }

    ~NativeContext();

private:
    juce::Component& component;
    VulkanViewController *controller = nil;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NativeContext)
};

VulkanContext::NativeContext::~NativeContext() {
    [controller release];
}

#endif
