#include <glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <filesystem.h>
#include <shader_m.h>
#include <Camera.h>
#include <animator.h>
#include "model.h"
#include <iostream>
#include "Renderer.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void LoadCity(float deltaTime,Shader ModelShader,Model ourModel,glm::vec3 pointLightPositions[]);
void LoadSkybox(Shader skyboxShader,unsigned int skyboxVAO,unsigned int cubemapTexture);

// settings
const unsigned int SCR_WIDTH = 1900;
const unsigned int SCR_HEIGHT = 1050;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(0.0f, 5.0f, 0.0f);

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

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
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
     glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Enables Cull Facing
	glEnable(GL_CULL_FACE);
	// Keeps front faces
	glCullFace(GL_FRONT);
	// Uses counter clock-wise standard
	glFrontFace(GL_CCW);
	glEnable(GL_TEXTURE_2D);
	glClearColor(0.1, 0.1, 0.1, 1);

    stbi_set_flip_vertically_on_load(true);

    Renderer renderer;
	ParticleSystem *particleSystem = new ParticleSystem;
	renderer.particleSystem = particleSystem;
    renderer.setShaders();
    // build and compile our shader zprogram
    // ------------------------------------
    //Shader ModelShader("6.multiple_lights.vs", "6.multiple_lights.fs");
    Shader ModelShader("6.multiple_lights.vs", "6.multiple_lights.fs");
    Shader AnimShader("anim_model.vs", "anim_model.fs");
    //Shader lightCubeShader("6.light_cube.vs", "6.light_cube.fs");
    //Model SpaceCraft(FileSystem::getPath("resources/Objects/SF_Fighter/SciFi_Fighter_lowScale.fbx"));
    Model ourModel(FileSystem::getPath("resources/Objects/DublinCity/City.obj"));
    Model AlienModel(FileSystem::getPath("resources/Objects/Gunplay/Gunplay.dae"));
    Animation WallClimbAnimation(FileSystem::getPath("resources/Objects/Gunplay/Gunplay.dae"),
        &AlienModel);
    Animator animator(&WallClimbAnimation);

    Shader skyboxShader("skybox.vert", "skybox.frag");
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    float skyboxVertices[] =
{
	//   Coordinates
	-10.0f, -10.0f,  10.0f,//        7--------6
	 10.0f, -10.0f,  10.0f,//       /|       /|
	 10.0f, -10.0f, -10.0f,//      4--------5 |
	-10.0f, -10.0f, -10.0f,//      | |      | |
	-10.0f,  10.0f,  10.0f,//      | 3------|-2
	 10.0f,  10.0f,  10.0f,//      |/       |/
	 10.0f,  10.0f, -10.0f,//      0--------1
	-10.0f,  10.0f, -10.0f
};
unsigned int skyboxIndices[] =
{
	// Right
	1, 2, 6,
	6, 5, 1,
	// Left
	0, 4, 7,
	7, 3, 0,
	// Top
	4, 5, 6,
	6, 7, 4,
	// Bottom
	0, 3, 2,
	2, 1, 0,
	// Back
	0, 1, 5,
	5, 4, 0,
	// Front
	3, 7, 6,
	6, 2, 3
};

    // positions of the point lights
    glm::vec3 pointLightPositions[] = {
        glm::vec3( 0.7f,  0.2f,  2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3( 0.0f,  0.0f, -3.0f)
    };

    // //PointLights
    // unsigned int VBO;
    // unsigned int lightCubeVAO;
    // glGenVertexArrays(1, &lightCubeVAO);
    // glGenBuffers(1, &VBO);

    // glBindVertexArray(lightCubeVAO);

    // glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    // glEnableVertexAttribArray(0);
    // load textures (we now use a utility function to keep the code more organized)
    // -----------------------------------------------------------------------------
    // unsigned int diffuseMap = loadTexture(FileSystem::getPath("resources/textures/container2.png").c_str());
    // unsigned int specularMap = loadTexture(FileSystem::getPath("resources/textures/container2_specular.png").c_str());

    // shader configuration
    // --------------------
    // ModelShader.use();
    // ModelShader.setInt("material.diffuse", 0);
    // ModelShader.setInt("material.specular", 1);


    //render loop
    //-----------
    skyboxShader.use();
    glUniform1i(glGetUniformLocation(skyboxShader.ID, "skybox"), 0);

    	// Create VAO, VBO, and EBO for the skybox
	unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glGenBuffers(1, &skyboxEBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// All the faces of the cubemap (make sure they are in this exact order)
	std::string facesCubemap[6] =
	{
		FileSystem::getPath("resources/Cubemap/right.jpg").c_str(),
		FileSystem::getPath("resources/Cubemap/left.jpg").c_str(),
		FileSystem::getPath("resources/Cubemap/top.jpg").c_str(),
		FileSystem::getPath("resources/Cubemap/bottom.jpg").c_str(),
		FileSystem::getPath("resources/Cubemap/front.jpg").c_str(),
		FileSystem::getPath("resources/Cubemap/back.jpg").c_str()
	};

    	// Creates the cubemap texture object
	unsigned int cubemapTexture;
	glGenTextures(1, &cubemapTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// These are very important to prevent seams
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    //Cycles through all the textures and attaches them to the cubemap object
for (unsigned int i = 0; i < 6; i++)
{
	int width, height, nrChannels;
	unsigned char* data = stbi_load(facesCubemap[i].c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		stbi_set_flip_vertically_on_load(false);
		glTexImage2D
		(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			0,
			GL_RGB,
			width,
			height,
			0,
			GL_RGB,
			GL_UNSIGNED_BYTE,
			data
		);
		stbi_image_free(data);
	}
	else
	{
		std::cout << "Failed to load texture: " << facesCubemap[i] << std::endl;
		stbi_image_free(data);
	}
}

    while (!glfwWindowShouldClose(window))
    {

        //per-frame time logic
        //--------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //input
        //-----
        processInput(window);
        animator.UpdateAnimation(deltaTime);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        LoadSkybox(skyboxShader,skyboxVAO,cubemapTexture);
        LoadCity(deltaTime,ModelShader,ourModel,pointLightPositions);
        //LoadCity(deltaTime,ModelShader,SpaceCraft,pointLightPositions,"spacecraft");
        // be sure to activate shader when setting uniforms/drawing objects
        AnimShader.use();
       // view/projection transformations
		glm::mat4 Animprojection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 Animview = camera.GetViewMatrix();
		AnimShader.setMat4("projection", Animprojection);
		AnimShader.setMat4("view", Animview);

        // world transformation
        // glm::mat4 model = glm::mat4(1.0f);
        // ModelShader.setMat4("model", model);

        //  // also draw the lamp object(s)
        //  ModelShader.use();
        //  ModelShader.setMat4("projection", projection);
        //  ModelShader.setMat4("view", view);

        //  // we now draw as many light bulbs as we have point lights.
        //  glBindVertexArray(lightCubeVAO);
        //  for (unsigned int i = 0; i < 4; i++)
        //  {
        //      model = glm::mat4(1.0f);
        //      model = glm::translate(model, pointLightPositions[i]);
        //      model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
        //      ModelShader.setMat4("model", model);
        //      glDrawArrays(GL_TRIANGLES, 0, 36);
        //  }
        // also draw the lamp object(s)



        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        auto transforms = animator.GetFinalBoneMatrices();
		for (int i = 0; i < transforms.size(); ++i)
			AnimShader.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);


		// // render the loaded model
		glm::mat4 Animmodel = glm::mat4(1.0f);
		Animmodel = glm::translate(Animmodel, glm::vec3(3.0f, 0.0f, -0.93f));
        float angleInRadians = glm::radians(150.0f);
        //Animmodel = glm::rotate(glm::mat4(1.0f), angleInRadians, glm::vec3(0.0f, 1.0f, 0.0f)); // translate it down so it's at the center of the scene
		Animmodel = glm::scale(Animmodel, glm::vec3(.1f, .1f, .1f));
        	// it's a bit too big for our scene, so scale it down
		AnimShader.setMat4("model", Animmodel);
		AlienModel.Draw(AnimShader);

        renderer.renderParticles();
        glfwSwapBuffers(window);
	    particleSystem->update();
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

//process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
//---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

//glfw: whenever the window size changed (by OS or user resize) this callback function executes
//---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    //make sure the viewport matches the new window dimensions; note that width and
    //height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

//glfw: whenever the mouse moves, this callback is called
//-------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

//glfw: whenever the mouse scroll wheel scrolls, this callback is called
//----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}


void LoadCity(float deltaTime,Shader ModelShader,Model ourModel,glm::vec3 pointLightPositions[]){

    ModelShader.use();
    ModelShader.setVec3("viewPos", camera.Position);
    ModelShader.setFloat("material.shininess", 10.0f);
    // also draw the lamp object(s)


    /*
        Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
        the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
        by defining light types as classes and set their values in there, or by using a more efficient uniform approach
        by using 'Uniform buffer objects', but that is something we'll discuss in the 'Advanced GLSL' tutorial.
    */
    // directional light
    ModelShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
    ModelShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
    ModelShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
    ModelShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
    // // point light 1
    ModelShader.setVec3("pointLights[0].position", pointLightPositions[0]);
    ModelShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
    ModelShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
    ModelShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
    ModelShader.setFloat("pointLights[0].constant", 1.0f);
    ModelShader.setFloat("pointLights[0].linear", 0.09f);
    ModelShader.setFloat("pointLights[0].quadratic", 0.032f);
    // point light 2
    ModelShader.setVec3("pointLights[1].position", pointLightPositions[1]);
    ModelShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
    ModelShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
    ModelShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
    ModelShader.setFloat("pointLights[1].constant", 1.0f);
    ModelShader.setFloat("pointLights[1].linear", 0.09f);
    ModelShader.setFloat("pointLights[1].quadratic", 0.032f);
    // point light 3
    ModelShader.setVec3("pointLights[2].position", pointLightPositions[2]);
    ModelShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
    ModelShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
    ModelShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
    ModelShader.setFloat("pointLights[2].constant", 1.0f);
    ModelShader.setFloat("pointLights[2].linear", 0.09f);
    ModelShader.setFloat("pointLights[2].quadratic", 0.032f);
    // point light 4
    ModelShader.setVec3("pointLights[3].position", pointLightPositions[3]);
    ModelShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
    ModelShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
    ModelShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
    ModelShader.setFloat("pointLights[3].constant", 1.0f);
    ModelShader.setFloat("pointLights[3].linear", 0.09f);
    ModelShader.setFloat("pointLights[3].quadratic", 0.032f);
    // spotLight
    ModelShader.setVec3("spotLight.position", camera.Position);
    ModelShader.setVec3("spotLight.direction", camera.Front);
    ModelShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
    ModelShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
    ModelShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
    ModelShader.setFloat("spotLight.constant", 1.0f);
    ModelShader.setFloat("spotLight.linear", 0.09f);
    ModelShader.setFloat("spotLight.quadratic", 0.032f);
    ModelShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(13.5f)));
    ModelShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
      // render the loaded model
    glm::mat4 DublinCityModel = glm::mat4(1.0f);
    DublinCityModel = glm::translate(DublinCityModel, glm::vec3(0.0f, 0.0f, 1.0f));
    DublinCityModel = glm::scale(DublinCityModel, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
    ModelShader.setMat4("model", DublinCityModel);
    glm::mat4 Modelprojection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 150.0f);
    glm::mat4 Modelview = camera.GetViewMatrix();
    ModelShader.setMat4("projection", Modelprojection);
    ModelShader.setMat4("view", Modelview);
    ourModel.Draw(ModelShader);
}
void LoadSkybox(Shader skyboxShader,unsigned int skyboxVAO,unsigned int cubemapTexture){
    // Since the cubemap will always have a depth of 1.0, we need that equal sign so it doesn't get discarded
    glDepthFunc(GL_LEQUAL);

    skyboxShader.use();
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = glm::mat4(1.0f);
    // We make the mat4 into a mat3 and then a mat4 again in order to get rid of the last row and column
    // The last row and column affect the translation of the skybox (which we don't want to affect)
    projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
    glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // Draws the cubemap as the last object so we can save a bit of performance by discarding all fragments
    // where an object is present (a depth of 1.0f will always fail against any object's depth value)
    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // Switch back to the normal depth function
    glDepthFunc(GL_LESS);

}
