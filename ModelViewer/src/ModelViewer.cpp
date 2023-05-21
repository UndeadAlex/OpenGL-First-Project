#include <iostream>
#include <windows.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb/stb_image.h>

// ImGUI Includes.
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "Mesh.h"
#include "Model.h"


const unsigned int SCREEN_WIDTH = 1024;
const unsigned int SCREEN_HEIGHT = 1024;

int main()
{
	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object of 800 by 800 pixels, naming it "YoutubeOpenGL"
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Death", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();

	// Init ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");

	
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	
	// Generates Shader object using shaders defualt.vert and default.frag
	Shader shaderProgram("Shaders\\default.vert", "Shaders\\default.frag");
	
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	
	glfwSwapInterval(0);

	Camera camera(SCREEN_WIDTH, SCREEN_HEIGHT, glm::vec3(0.0f, 0.0f, 0.0f));

	Model mapModel("Assets/map/scene.gltf");
	Model grindModel("Assets/grindstone/scene.gltf");
	// model.SetRotation(glm::vec3(0.0f, 0.0f, -90.0f));
	mapModel.SetScale(0.1f);
	grindModel.SetPosition(glm::vec3(0.005f, 0.0f, 0.01f));
	grindModel.SetRotation(glm::vec3(0.0f, 0.0f, glm::radians(-90.0f)));
	grindModel.SetScale(0.01f);

	double prevFrameTime = 0.0;
	double crntFrameTime = 0.0;

	bool show_demo_window = false;
	
	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		crntFrameTime = glfwGetTime();
		const double frameTime = crntFrameTime - prevFrameTime;
		prevFrameTime = crntFrameTime;
		
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if(show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		ImGui::Begin("Hello, world!");

		ImGui::Text("This is some useful text.");
		ImGui::Checkbox("Demo Window", &show_demo_window);
		
		/*if (ImGui::Button("Select.."))
			ImGui::OpenPopup("my_select_popup");
		ImGui::SameLine();
		ImGui::TextUnformatted(selected_fish == -1 ? "<None>" : names[selected_fish]);
		if (ImGui::BeginPopup("my_select_popup"))
		{
			ImGui::SeparatorText("Aquarium");
			for (int i = 0; i < IM_ARRAYSIZE(names); i++)
				if (ImGui::Selectable(names[i]))
					selected_fish = i;
			ImGui::EndPopup();
		}*/
		
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		ImGui::End();
		
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera.Inputs(window, frameTime);
		camera.updateMatrix(45.0f, 0.1f, 100.0f);
		
		mapModel.Draw(shaderProgram, camera);
		grindModel.Draw(shaderProgram, camera);


		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}



	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	
	// Delete all the objects we've created
	shaderProgram.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	
	return 0;
}