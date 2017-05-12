#ifndef DISPLAY_H
#define DISPLAY_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>

#include <stdexcept>
#include <iostream>
#include <vector>
#include <functional>
#include <cstring>
#include <set>
#include <algorithm>
#include <fstream>
#include <array>

struct UniformBufferObject {
  glm::mat4 model;
  glm::mat4 view;
  glm::mat4 proj;
};

struct Vertex {
  glm::vec2 pos;
  glm::vec3 color;
  glm::vec2 texCoord;
  
  static VkVertexInputBindingDescription getBindingDescription() {
    VkVertexInputBindingDescription bindingDescription = {};
    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof(Vertex);
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX; // VK_VERTEX_INPUT_RATE_INSTANCE for instanced rendering
    
    return bindingDescription;
  }
  
  static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
    std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions = {};
    
    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(Vertex, pos);
    
    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(Vertex, color);
    
    attributeDescriptions[2].binding = 0;
    attributeDescriptions[2].location = 2;
    attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[2].offset = offsetof(Vertex, texCoord);
    
    return attributeDescriptions;
  }
};
/*
const std::vector<Vertex> vertices = {
    {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
    {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
    {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
};*/

const std::vector<uint16_t> indices = {
    0, 1, 2, 2, 3, 0
};

static std::vector<char> readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file!");
    }
    
    size_t fileSize = (size_t) file.tellg();
    std::vector<char> buffer(fileSize);
    
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    
    file.close();
    
    return buffer;
}

const std::vector<const char*> deviceExtensions = {
  VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

struct QueueFamilyIndices {
  int graphicsFamily = -1;
  int presentFamily = -1;
  
  bool isComplete() {
    return graphicsFamily >= 0 && presentFamily >= 0;
  }
};

struct SwapChainSupportDetails {
  VkSurfaceCapabilitiesKHR capabilities;
  std::vector<VkSurfaceFormatKHR> formats;
  std::vector<VkPresentModeKHR> presentModes;
};

class Display {
  public:
    void run();
  protected:
    static void onWindowResized(GLFWwindow* window, int width, int height) {
      if(width == 0 || height == 0) return;
   
       Display* app = reinterpret_cast<Display*>(glfwGetWindowUserPointer(window));
       app->recreateSwapChain();
    }
  
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
    
    VkCommandBuffer beginSingleTimeCommands();
    void endSingleTimeCommands(VkCommandBuffer commandBuffer);
    
    void createTextureSampler();
    void createImageView(VkImage image, VkFormat format, VkImageView &imageView);
    void createTextureImageView();
    void copyImage(VkImage srcImage, VkImage dstImage, uint32_t width, uint32_t height);
    void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
    void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage &image, VkDeviceMemory &imageMemory);
    void createTextureImage();
    void createDescriptorSet();
    void createDescriptorPool();
    void createUniformBuffer();
    void createDescriptorSetLayout();
    void recreateSwapChain();
    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer, VkDeviceMemory &bufferMemory);
    void createIndexBuffer();
    void createVertexBuffer();
    void createSemaphores();
    void createCommandBuffers();
    void createCommandPool();
    void createFramebuffers();
    void createRenderPass();
    void createShaderModule(const std::vector<char>& code, VkShaderModule& shaderModule);
    void createGraphicsPipeline();
    void createImageViews();
    void createSwapChain();
    void createSurface();
    void createLogicalDevice();
    void createInstance();
    
    bool isDeviceSuitable(VkPhysicalDevice device);
    void pickPhysicalDevice();
    
    void initWindow();
    void initVulkan();

    void updateUniformBuffer(float delta);
    void drawFrame();
    void mainLoop();
    
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    
    VkSampler textureSampler;
    VkImageView textureImageView;
    VkImage textureImage;
    VkDeviceMemory textureImageMemory;
    VkImage stagingImage;
    VkDeviceMemory stagingImageMemory;
    VkDescriptorSet descriptorSet;
    VkDescriptorPool descriptorPool;
    VkBuffer uniformStagingBuffer;
    VkDeviceMemory uniformStagingBufferMemory;
    VkBuffer uniformBuffer;
    VkDeviceMemory uniformBufferMemory;
    VkDescriptorSetLayout descriptorSetLayout;
    VkDeviceMemory indexBufferMemory;
    VkBuffer indexBuffer;
    VkDeviceMemory vertexBufferMemory;
    VkBuffer vertexBuffer;
    VkSemaphore imageAvailableSemaphore;
    VkSemaphore renderFinishedSemaphore;
    VkCommandPool commandPool;
    VkPipeline graphicsPipeline;
    VkRenderPass renderPass;
    VkPipelineLayout pipelineLayout;
    VkSwapchainKHR swapChain;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    VkPhysicalDevice physicalDevice;
    VkInstance instance;
    VkDevice device;
    VkQueue graphicsQueue;
    VkQueue presentQueue;
    VkSurfaceKHR surface;
    
    std::vector<VkCommandBuffer> commandBuffers;
    std::vector<VkFramebuffer> swapChainFramebuffers;
    std::vector<VkImage> swapChainImages;
    std::vector<VkImageView> swapChainImageViews;

    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;
    GLFWwindow* window;
    
    float x = 500.0f;
    float y = 500.0f;
    float rotation = 0.0f;
    float width = 500.0f;
    float height = 500.0f;

    //Square
    std::vector<Vertex>  square = {
    {{-250.0f, -250.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
    {{250.0f, -250.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
    {{250.0f, 250.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
    {{-250.0f, 250.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}}
};
    
    //VDeleter<VkInstance> instance{vkDestroyInstance};
};

#endif
