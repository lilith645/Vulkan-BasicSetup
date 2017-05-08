VULKAN_SDK_PATH = /home/akuma/VulkanSDK/1.0.46.0/x86_64

CFLAGS = -std=c++11 -I$(VULKAN_SDK_PATH)/include
LDFLAGS = -L$(VULKAN_SDK_PATH)/lib `pkg-config --static --libs glfw3` -lvulkan
VulkanCPP = main.cpp Display.cpp

Vulkan: $(VulkanCPP)
	g++ $(CFLAGS) -o Vulkan $(VulkanCPP) $(LDFLAGS)

.PHONY: test clean

test: Vulkan
	LD_LIBRARY_PATH=$(VULKAN_SDK_PATH)/lib 
VK_LAYER_PATH=$(VULKAN_SDK_PATH)/etc/explicit_layer.d ./Vulkan

clean:
	rm -f Vulkan
