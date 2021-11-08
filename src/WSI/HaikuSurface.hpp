// Copyright 2021 <unset>. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef SWIFTSHADER_HAIKUSURFACE_HPP
#define SWIFTSHADER_HAIKUSURFACE_HPP

#include "VkSurfaceKHR.hpp"
#include "Vulkan/VkImage.hpp"
#include "Vulkan/VkObject.hpp"

#include <Bitmap.h>
#include <View.h>

#include <vulkan/vulkan_haiku.h>

#include <map>

namespace vk {

class HaikuSurface : public SurfaceKHR, public ObjectBase<HaikuSurface, VkSurfaceKHR>
{
public:
	HaikuSurface(const VkHaikuSurfaceCreateInfoEXT *pCreateInfo, void *mem);

	void destroySurface(const VkAllocationCallbacks *pAllocator) override;

	static size_t ComputeRequiredAllocationSize(const VkHaikuSurfaceCreateInfoEXT *pCreateInfo);

	VkResult getSurfaceCapabilities(VkSurfaceCapabilitiesKHR *pSurfaceCapabilities) const override;

	virtual void attachImage(PresentImage *image) override;
	virtual void detachImage(PresentImage *image) override;
	VkResult present(PresentImage *image) override;

private:
	void lazyCreateFrameBuffer();
	void destroyFrameBuffer();

    BView*     view = nullptr;
	VkExtent2D windowExtent = {};
    BBitmap*   bufferBitmap = nullptr;
    char*      framebuffer = nullptr;
};

}  // namespace vk
#endif  // SWIFTSHADER_HAIKUSURFACE_HPP
