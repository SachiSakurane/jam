//
// Created by SachiSakurane on 2020/12/31.
//

/*******************************************************************************
 The block below describes the properties of this module, and is read by
 the Projucer to automatically generate project code that uses it.
 For details about the syntax and how to create or use a module, see the
 JUCE Module Format.md file.


 BEGIN_JUCE_MODULE_DECLARATION

  ID:                 jam_vulkan
  vendor:             sachi
  version:            0.0.1
  name:               Jam Vulkan classes
  description:        Classes for rendering Vulkan in a JUCE window.
  website:            http://www.juce.com/juce
  license:            BSL

  dependencies:       juce_gui_extra

 END_JUCE_MODULE_DECLARATION

*******************************************************************************/

#pragma once
#define JAM_VULKAN_H_INCLUDED

#include <juce_gui_extra/juce_gui_extra.h>

#undef JAM_VULKAN
#define JAM_VULKAN 1

#include "vulkan/jam_VulkanComponent.hpp"
#include "vulkan/jam_VulkanContext.hpp"
