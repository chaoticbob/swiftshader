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

#include "HaikuSurface.hpp"

#include "System/Debug.hpp"
#include "Vulkan/VkDeviceMemory.hpp"

#include <string.h>
#include <iostream>

namespace {

VkExtent2D getViewSize(BView* view)
{
	ASSERT(view != nullptr);

    uint32_t width = 0;
    uint32_t height = 0;
    if (view->LockLooper()) {
        BRect bounds = view->Bounds();
	    width = static_cast<uint32_t>(floor(bounds.Width()) + 1);
    	height = static_cast<uint32_t>(floor(bounds.Height()) + 1);
        view->UnlockLooper();
    }

	return { width, height };
}

}  // namespace

namespace vk {

HaikuSurface::HaikuSurface(const VkHaikuSurfaceCreateInfoEXT *pCreateInfo, void *mem)
    : view(pCreateInfo->pView)
{
	ASSERT(view != nullptr);
	lazyCreateFrameBuffer();
}

void HaikuSurface::destroySurface(const VkAllocationCallbacks *pAllocator)
{
	destroyFrameBuffer();
}

size_t HaikuSurface::ComputeRequiredAllocationSize(const VkHaikuSurfaceCreateInfoEXT *pCreateInfo)
{
	return 0;
}

VkResult HaikuSurface::getSurfaceCapabilities(VkSurfaceCapabilitiesKHR *pSurfaceCapabilities) const
{
	setCommonSurfaceCapabilities(pSurfaceCapabilities);

	if(view == nullptr)
	{
		VkExtent2D extent = { 0, 0 };
		pSurfaceCapabilities->currentExtent = extent;
		pSurfaceCapabilities->minImageExtent = extent;
		pSurfaceCapabilities->maxImageExtent = extent;
		return VK_ERROR_SURFACE_LOST_KHR;
	}

	VkExtent2D extent = getViewSize(view);
	pSurfaceCapabilities->currentExtent = extent;
	pSurfaceCapabilities->minImageExtent = extent;
	pSurfaceCapabilities->maxImageExtent = extent;
	return VK_SUCCESS;
}

void HaikuSurface::attachImage(PresentImage *image)
{
	// Nothing to do here, the current implementation based on BView blits on
	// present instead of associating the image with the surface.
}

void HaikuSurface::detachImage(PresentImage *image)
{
	// Nothing to do here, the current implementation based on BView blits on
	// present instead of associating the image with the surface.
}

VkResult HaikuSurface::present(PresentImage *image)
{
	// Recreate frame buffer in case window size has changed
	lazyCreateFrameBuffer();

	if(!framebuffer)
	{
		// e.g. window width or height is 0
		return VK_SUCCESS;
	}

	const VkExtent3D &extent = image->getImage()->getExtent();

	if(windowExtent.width != extent.width || windowExtent.height != extent.height)
	{
		return VK_ERROR_OUT_OF_DATE_KHR;
	}

	uint32 bytesPerRow = bufferBitmap->BytesPerRow();

    image->getImage()->copyTo(reinterpret_cast<uint8_t *>(framebuffer), bytesPerRow);

   	char *s_ptr = framebuffer;
   	char *d_ptr = (char*)bufferBitmap->Bits();

    for (uint32_t i = 0; i < extent.height; ++i, s_ptr += bytesPerRow, d_ptr += bytesPerRow) {
        memcpy(d_ptr, s_ptr, bytesPerRow);
    }

	if (view->LockLooperWithTimeout(10000) == B_OK) {
		view->SetDrawingMode(B_OP_COPY);
		view->DrawBitmap(bufferBitmap);
		view->UnlockLooper();
	}

	return VK_SUCCESS;
}

void HaikuSurface::lazyCreateFrameBuffer()
{
	auto currWindowExtent = getViewSize(view);
	if(currWindowExtent.width == windowExtent.width && currWindowExtent.height == windowExtent.height)
	{
		return;
	}

	windowExtent = currWindowExtent;

	if(framebuffer)
	{
		destroyFrameBuffer();
	}

	if(windowExtent.width == 0 || windowExtent.height == 0)
	{
		return;
	}

	BRect fbRect = BRect(0, 0, windowExtent.width - 1, windowExtent.height - 1);
	bufferBitmap = new BBitmap(fbRect, B_RGB32, true);
	framebuffer = (char*)malloc(bufferBitmap->BitsLength());
}

void HaikuSurface::destroyFrameBuffer()
{
    if (view->LockLooper()) { 
        delete bufferBitmap;
        free(framebuffer);
    	framebuffer = nullptr;
        view->UnlockLooper();
    }
}

}  // namespace vk
