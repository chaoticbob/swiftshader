#ifndef VULKAN_HAIKU_H_
#define VULKAN_HAIKU_H_ 1

/*
** Copyright 2015-2021 <unset>
**
** SPDX-License-Identifier: Apache-2.0
*/

#ifdef __cplusplus
extern "C" {
#endif

#define VK_EXT_haiku_surface 1
#define VK_EXT_HAIKU_SURFACE_SPEC_VERSION 1
#define VK_EXT_HAIKU_SURFACE_EXTENSION_NAME "VK_EXT_haiku_surface"
typedef VkFlags VkHaikuSurfaceCreateFlagsEXT;
typedef struct VkHaikuSurfaceCreateInfoEXT {
    VkStructureType                     sType;
    const void*                         pNext;
    VkHaikuSurfaceCreateFlagsEXT        flags;
    BView*                              pView;
} VkHaikuSurfaceCreateInfoEXT;

typedef VkResult (VKAPI_PTR *PFN_vkCreateHaikuSurfaceEXT)(VkInstance instance, const VkHaikuSurfaceCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);

#ifndef VK_NO_PROTOTYPES
VKAPI_ATTR VkResult VKAPI_CALL vkCreateHaikuSurfaceEXT(
    VkInstance                          instance,
    const VkHaikuSurfaceCreateInfoEXT*  pCreateInfo,
    const VkAllocationCallbacks*        pAllocator,
    VkSurfaceKHR*                       pSurface);
#endif

#ifdef __cplusplus
}
#endif

#endif
