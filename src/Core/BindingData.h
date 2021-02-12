#pragma once
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

namespace Tristeon::Core
{
	struct FrameBufferAttachment
	{
		vk::Image image = nullptr;
		vk::DeviceMemory memory = nullptr;
		vk::ImageView view = nullptr;
	};
	
	struct BindingData
	{
		GLFWwindow* window = nullptr;
		unsigned int width = 0;
		unsigned int height = 0;

		//Core
		vk::Instance instance = nullptr;
		vk::PhysicalDevice physical = nullptr;
		vk::Device device = nullptr;

		uint32_t graphicsFamily = 0;
		vk::Queue graphicsQueue = nullptr;
		vk::CommandPool graphicsPool = nullptr;

		uint32_t presentFamily = 0;
		vk::Queue presentQueue = nullptr;

		uint32_t transferFamily = 0;
		vk::Queue transferQueue = nullptr;
		vk::CommandPool transferPool = nullptr;
		
		vk::SurfaceKHR surface = nullptr;

		vk::SurfaceFormatKHR format;
		vk::PresentModeKHR presentMode;
		vk::Extent2D extent;

		vk::SwapchainKHR swapchain = nullptr;
		std::vector<vk::Image> swapchainImages{};
		std::vector<vk::ImageView> swapchainImageViews{};
		std::vector<vk::Framebuffer> swapchainFramebuffers;
		
		bool debug_messenger_enabled = true;
		vk::DebugUtilsMessengerEXT debugMessenger;

		vk::DescriptorPool descriptorPool = nullptr;

		//Deferred first pass
		vk::RenderPass offscreenPass = nullptr;
		vk::CommandBuffer offscreenBuffer = nullptr;
		vk::Semaphore semaOffscreenFinished;
		vk::Framebuffer offscreenFramebuffer;
		FrameBufferAttachment offscreenColor, offscreenNormal, offscreenDepth;
		
		//Deferred output pass
		vk::RenderPass outputPass = nullptr;
		std::vector<vk::CommandBuffer> outputCommandBuffers{};
		vk::Semaphore semaImageAvailable = nullptr;
		vk::Semaphore semaOutputFinished = nullptr;
	};

	inline BindingData binding_data;
}