#include<iostream>
#include<fstream>
#include<chrono>
#include<thread>
#include<future>
#include<nlohmann/json.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include "objects/hitables/sphere.hpp"
#include "objects/hitables/rect.hpp"
#include "objects/hitables/box.hpp"
#include "objects/hitables/model.hpp"
#include "objects/hitables/scene.hpp"
#include "objects/camera/camera.hpp"
#include "util/image/image.hpp"
#include "util/denoiser/denoiser.hpp"
#include "util/scene_parser/scene_parser.hpp"
#include "render_process/tile_pool.hpp"
#include "render_process/render_settings.hpp"
#include "render_process/render_worker.hpp"

using json = nlohmann::json;

// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
unsigned int threadsComplete = 0;
int maxThreads = 0;
bool renderStarted = false;
bool renderCompleted = false;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void completeThread();
void saveImage(RenderSettings renderSettings);
std::mutex writeLock;
std::mutex readLock;

void initializeRender(Scene* scene, RenderSettings renderSettings, std::shared_future<void> futureObject) {

	RenderWorker renderWorker(scene, renderSettings, std::this_thread::get_id());
	renderWorker.executeStoppable(futureObject);
	completeThread();
}

void resetCompletedThreads(){
	std::lock_guard<std::mutex> lock(writeLock);
	threadsComplete = 0;
}

void completeThread(){
	std::lock_guard<std::mutex> lock(writeLock);
	threadsComplete++;
}

unsigned int getCompletedThreads(){
	std::lock_guard<std::mutex> lock(readLock);
	return threadsComplete;
}

void joinThreads(std::vector<std::thread> &renderThreads){
	for (unsigned int i = 0; i < maxThreads; i++)
	{
		if (renderThreads[i].joinable())
		{
			renderThreads[i].join();
		}
	}
}

int main() {

	std::promise<void> exitSignal;
	std::shared_future<void> exitFutureObj(exitSignal.get_future());

	//scene setup
	auto startSceneSetupTime = std::chrono::high_resolution_clock::now();
	std::cout << "Building Scene.\n";
	SceneParser sceneParser("sample_scenes/cornell_box.json");
  auto endSceneSetupTime = std::chrono::high_resolution_clock::now();
  auto sceneBuildDuration = std::chrono::duration_cast<std::chrono::seconds>(endSceneSetupTime - startSceneSetupTime);
  std::cout << "Scene built: " << sceneBuildDuration.count() << "\n";

	std::string filePath = "img.png";
	Image image(sceneParser.getImageWidth(), sceneParser.getImageHeight());
	TilePool tilePool(image.getWidth(), image.getHeight(), 50);

	RenderSettings renderSettings{
		&image,
		&tilePool,
		sceneParser.getRenderSamples(),
		sceneParser.denoiseImage(),
		filePath
	};
  SCR_WIDTH = (unsigned int)(1.3 * sceneParser.getImageWidth());
  SCR_HEIGHT = (unsigned int)(1.3 * sceneParser.getImageHeight());

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return -1;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	ImGui::StyleColorsDark();

	maxThreads = std::thread::hardware_concurrency();
  std::cout << "Using "<<maxThreads<<" threads.\n";
  std::vector<std::thread> renderThreads;
  renderThreads.resize(maxThreads);
	auto startRenderTime = std::chrono::high_resolution_clock::now();
  while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Demo Window");
				if(renderStarted && getCompletedThreads() == maxThreads){
					resetCompletedThreads();
					auto renderEndTime = std::chrono::high_resolution_clock::now();
					auto renderDuration = std::chrono::duration_cast<std::chrono::seconds>(renderEndTime - startRenderTime);
					std::cout << "Render Complete: " << renderDuration.count() << " seconds\n";
					saveImage(renderSettings);
					joinThreads(renderThreads);
					renderStarted = false;
					renderCompleted = true;
				}
        if(ImGui::Button("Render!") && !renderStarted){
          renderStarted = true;
					startRenderTime = std::chrono::high_resolution_clock::now();
				  std::cout << "Starting Render.\n";
					for (unsigned int i = 0; i < maxThreads; i++)
					{
				    std::cout<<i<<"\n";
						renderThreads[i] = std::thread(&initializeRender, sceneParser.getScene(), renderSettings, exitFutureObj);
					}
        }
        ImGui::End();
				if(renderStarted || renderCompleted){
					GLuint image_texture;
					glGenTextures(1, &image_texture);
					glBindTexture(GL_TEXTURE_2D, image_texture);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, image.getWidth(), image.getHeight(), 0, GL_RGB, GL_FLOAT, image.getBuffer());
					ImGui::Begin("Render Result");
					ImGui::Image((void*)(intptr_t)image_texture, ImVec2(image.getWidth(), image.getHeight()));
					ImGui::End();
				}

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
		exitSignal.set_value();
		for (unsigned int i = 0; i < maxThreads; i++)
		{
			if (renderThreads[i].joinable())
			{
				renderThreads[i].join();
			}
		}

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();

}

void saveImage(RenderSettings renderSettings){
	if (renderSettings.DENOISE_IMAGE)
	{
		auto startDenoiseTime = std::chrono::high_resolution_clock::now();
		Image outputImage(renderSettings.image->getWidth(), renderSettings.image->getHeight());
		Denoiser imageDenoiser(renderSettings.image->getBuffer(), outputImage.getBuffer(), renderSettings.image->getWidth(), renderSettings.image->getHeight(), renderSettings.image->getChannels());
		imageDenoiser.executeDenoiser();
		auto denoiseCompleteTime = std::chrono::high_resolution_clock::now();
		auto denoiseDuration = std::chrono::duration_cast<std::chrono::seconds>(denoiseCompleteTime - startDenoiseTime);
		std::cout << "Denoising Complete: " << denoiseDuration.count() << " seconds\n";
		outputImage.saveImage(renderSettings.filePath);
	}
	else
	{
		renderSettings.image->saveImage(renderSettings.filePath);
	}
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
