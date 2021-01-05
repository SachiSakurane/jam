//
// Created by SachiSakurane on 2020/12/30.
//

VulkanComponent::VulkanComponent() {
    setOpaque (true);
    context.setRenderer (this);
    context.attachTo (*this);
    context.setContinuousRepainting (true);
}

VulkanComponent::~VulkanComponent() {
    context.detach();
}

void VulkanComponent::initialize() {

}
