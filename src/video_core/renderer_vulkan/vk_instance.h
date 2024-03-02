// Copyright 2022 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#pragma once

#include <span>

#include "video_core/renderer_vulkan/vk_platform.h"

namespace Frontend {
class WindowSDL;
}

VK_DEFINE_HANDLE(VmaAllocator)

namespace Vulkan {

class Instance {
public:
    explicit Instance(bool validation = false, bool dump_command_buffers = false);
    explicit Instance(Frontend::WindowSDL& window, u32 physical_device_index);
    ~Instance();

    /// Returns a formatted string for the driver version
    std::string GetDriverVersionName();

    /// Returns the Vulkan instance
    vk::Instance GetInstance() const {
        return *instance;
    }

    /// Returns the current physical device
    vk::PhysicalDevice GetPhysicalDevice() const {
        return physical_device;
    }

    /// Returns the Vulkan device
    vk::Device GetDevice() const {
        return *device;
    }

    /// Returns the VMA allocator handle
    VmaAllocator GetAllocator() const {
        return allocator;
    }

    /// Returns a list of the available physical devices
    std::span<const vk::PhysicalDevice> GetPhysicalDevices() const {
        return physical_devices;
    }

    /// Retrieve queue information
    u32 GetGraphicsQueueFamilyIndex() const {
        return queue_family_index;
    }

    u32 GetPresentQueueFamilyIndex() const {
        return queue_family_index;
    }

    vk::Queue GetGraphicsQueue() const {
        return graphics_queue;
    }

    vk::Queue GetPresentQueue() const {
        return present_queue;
    }

    /// Returns true when a known debugging tool is attached.
    bool HasDebuggingToolAttached() const {
        return has_renderdoc || has_nsight_graphics;
    }

    /// Returns true if anisotropic filtering is supported
    bool IsAnisotropicFilteringSupported() const {
        return features.samplerAnisotropy;
    }

    /// Returns true when VK_EXT_custom_border_color is supported
    bool IsCustomBorderColorSupported() const {
        return custom_border_color;
    }

    /// Returns true when VK_EXT_index_type_uint8 is supported
    bool IsIndexTypeUint8Supported() const {
        return index_type_uint8;
    }

    /// Returns true when VK_EXT_fragment_shader_interlock is supported
    bool IsFragmentShaderInterlockSupported() const {
        return fragment_shader_interlock;
    }

    /// Returns true when VK_KHR_image_format_list is supported
    bool IsImageFormatListSupported() const {
        return image_format_list;
    }

    /// Returns true when VK_EXT_pipeline_creation_cache_control is supported
    bool IsPipelineCreationCacheControlSupported() const {
        return pipeline_creation_cache_control;
    }

    /// Returns true when VK_EXT_shader_stencil_export is supported
    bool IsShaderStencilExportSupported() const {
        return shader_stencil_export;
    }

    /// Returns true when VK_EXT_external_memory_host is supported
    bool IsExternalMemoryHostSupported() const {
        return external_memory_host;
    }

    /// Returns the vendor ID of the physical device
    u32 GetVendorID() const {
        return properties.vendorID;
    }

    /// Returns the device ID of the physical device
    u32 GetDeviceID() const {
        return properties.deviceID;
    }

    /// Returns the driver ID.
    vk::DriverId GetDriverID() const {
        return driver_id;
    }

    /// Returns the current driver version provided in Vulkan-formatted version numbers.
    u32 GetDriverVersion() const {
        return properties.driverVersion;
    }

    /// Returns the current Vulkan API version provided in Vulkan-formatted version numbers.
    u32 ApiVersion() const {
        return properties.apiVersion;
    }

    /// Returns the vendor name reported from Vulkan.
    std::string_view GetVendorName() const {
        return vendor_name;
    }

    /// Returns the list of available extensions.
    std::span<const std::string> GetAvailableExtensions() const {
        return available_extensions;
    }

    /// Returns the device name.
    std::string_view GetModelName() const {
        return properties.deviceName;
    }

    /// Returns the pipeline cache unique identifier
    const auto GetPipelineCacheUUID() const {
        return properties.pipelineCacheUUID;
    }

    /// Returns the minimum required alignment for uniforms
    vk::DeviceSize UniformMinAlignment() const {
        return properties.limits.minUniformBufferOffsetAlignment;
    }

    /// Returns the minimum alignemt required for accessing host-mapped device memory
    vk::DeviceSize NonCoherentAtomSize() const {
        return properties.limits.nonCoherentAtomSize;
    }

    /// Returns the maximum supported elements in a texel buffer
    u32 MaxTexelBufferElements() const {
        return properties.limits.maxTexelBufferElements;
    }

    /// Returns true if shaders can declare the ClipDistance attribute
    bool IsShaderClipDistanceSupported() const {
        return features.shaderClipDistance;
    }

    /// Returns the minimum imported host pointer alignment
    u64 GetMinImportedHostPointerAlignment() const {
        return min_imported_host_pointer_alignment;
    }

private:
    /// Creates the logical device opportunistically enabling extensions
    bool CreateDevice();

    /// Creates the VMA allocator handle
    void CreateAllocator();

    /// Collects telemetry information from the device.
    void CollectDeviceParameters();
    void CollectToolingInfo();

private:
    vk::DynamicLoader dl;
    vk::UniqueInstance instance;
    vk::PhysicalDevice physical_device;
    vk::UniqueDevice device;
    vk::PhysicalDeviceProperties properties;
    vk::PhysicalDeviceFeatures features;
    vk::DriverIdKHR driver_id;
    vk::UniqueDebugUtilsMessengerEXT debug_callback;
    std::string vendor_name;
    VmaAllocator allocator{};
    vk::Queue present_queue;
    vk::Queue graphics_queue;
    std::vector<vk::PhysicalDevice> physical_devices;
    std::vector<std::string> available_extensions;
    u32 queue_family_index{0};
    bool image_view_reinterpretation{true};
    bool timeline_semaphores{};
    bool custom_border_color{};
    bool index_type_uint8{};
    bool fragment_shader_interlock{};
    bool image_format_list{};
    bool pipeline_creation_cache_control{};
    bool fragment_shader_barycentric{};
    bool shader_stencil_export{};
    bool external_memory_host{};
    u64 min_imported_host_pointer_alignment{};
    bool tooling_info{};
    bool debug_utils_supported{};
    bool has_nsight_graphics{};
    bool has_renderdoc{};
};

} // namespace Vulkan
