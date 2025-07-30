#define PLATFORM_OSX

#include <iostream>
#include <list>
#include <algorithm>

#define GLEW_STATIC 1
#include <GL/glew.h>

#include <GLFW/glfw3.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

using namespace glm;
using namespace std;

// Declare helper functions (defined later)
GLuint loadTexture(const char *filename);
const char* getVertexShaderSource();
const char* getFragmentShaderSource();
const char* getTexturedVertexShaderSource();
const char* getTexturedFragmentShaderSource();
int compileAndLinkShaders(const char* vertexShaderSource, const char* fragmentShaderSource);

// Vertex definition for a textured + colored cube
struct TexturedColoredVertex
{
    TexturedColoredVertex(vec3 _position, vec3 _color, vec2 _uv)
    : position(_position), color(_color), uv(_uv) {}
    
    vec3 position;
    vec3 color;
    vec2 uv;
};

// Textured Cube model
const TexturedColoredVertex texturedCubeVertexArray[] = {  // position,                            color
    TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f)), //left - red
    TexturedColoredVertex(vec3(-0.5f,-0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 1.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 1.0f)),
    
    TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 1.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 0.0f)),
    
    TexturedColoredVertex(vec3( 0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f)), // far - blue
    TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 1.0f)),
    
    TexturedColoredVertex(vec3( 0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f)),
    TexturedColoredVertex(vec3( 0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 0.0f)),
    TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f)),
    
    TexturedColoredVertex(vec3( 0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f)), // bottom - turquoise
    TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f)),
    TexturedColoredVertex(vec3( 0.5f,-0.5f,-0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(1.0f, 0.0f)),
    
    TexturedColoredVertex(vec3( 0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f)),
    TexturedColoredVertex(vec3(-0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(0.0f, 1.0f)),
    TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f)),
    
    TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(0.0f, 1.0f)), // near - green
    TexturedColoredVertex(vec3(-0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(0.0f, 0.0f)),
    TexturedColoredVertex(vec3( 0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(1.0f, 0.0f)),
    
    TexturedColoredVertex(vec3( 0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(1.0f, 1.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(0.0f, 1.0f)),
    TexturedColoredVertex(vec3( 0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(1.0f, 0.0f)),
    
    TexturedColoredVertex(vec3( 0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f)), // right - purple
    TexturedColoredVertex(vec3( 0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f)),
    TexturedColoredVertex(vec3( 0.5f, 0.5f,-0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(1.0f, 0.0f)),
    
    TexturedColoredVertex(vec3( 0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f)),
    TexturedColoredVertex(vec3( 0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f)),
    TexturedColoredVertex(vec3( 0.5f,-0.5f, 0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(0.0f, 1.0f)),
    
    TexturedColoredVertex(vec3( 0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(1.0f, 1.0f)), // top - yellow
    TexturedColoredVertex(vec3( 0.5f, 0.5f,-0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(1.0f, 0.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(0.0f, 0.0f)),
    
    TexturedColoredVertex(vec3( 0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(1.0f, 1.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(0.0f, 0.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(0.0f, 1.0f))
};

int createTexturedCubeVertexArrayObject();

// Uploads projection matrix to the shader
void setProjectionMatrix(int shaderProgram, mat4 projectionMatrix)
{
    glUseProgram(shaderProgram);
    GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
}

// Uploads view matrix to the shader
void setViewMatrix(int shaderProgram, mat4 viewMatrix)
{
    glUseProgram(shaderProgram);
    GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
}

// Uploads world (model) matrix to the shader
void setWorldMatrix(int shaderProgram, mat4 worldMatrix)
{
    glUseProgram(shaderProgram);
    GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
}

const int MAZE_WIDTH = 20;
const int MAZE_HEIGHT = 20;

// A 20x20 maze represented as an array of strings
// '#' = wall, 'S' = start, 'F' = finish, ' ' = path
const char* maze[MAZE_HEIGHT] = {
    "####################",
    "#S           #     #",
    "# # ##### # ### ####",
    "# #     # #     #  F",
    "# ##### ### ### # ##",
    "#     #     #   #  #",
    "##### ####### ### ##",
    "#     #     #      #",
    "# ### ### ####### ##",
    "# #   #     #      #",
    "# # ####### ###### #",
    "# #       #        #",
    "### ##### ##########",
    "#   #     #        #",
    "# ##### # ######## #",
    "#     # #      #   #",
    "##### # ########## #",
    "#     #     #      #",
    "# ######### # ######",
    "####################"
};

glm::vec3 startPosition;    // Player starting point in the maze
glm::vec3 finishPosition;   // Maze exit goal position

// Flame light settings (static at entrance)
vec3 flameLightColor(1.0f, 0.5f, 0.2f); // orange flame color
vec3 flameBasePosition; // will be initialized once we find 'S' in the maze


int main(int argc, char*argv[])
{
    // Initialize GLFW and OpenGL version
    glfwInit();
    
#if defined(PLATFORM_OSX)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
    // On windows, we set OpenGL version to 2.1, to support more hardware
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#endif

    // Create Window and rendering context using GLFW, resolution is 800x600
    GLFWwindow* window = glfwCreateWindow(800, 600, "Comp371 - Lab 03", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Set input mode to hide and disable mouse cursor (first-person camera)
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to create GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    // Load Textures for Ground and Wall
    GLuint groundTextureID = loadTexture("Textures/ground_0044_color_1k.jpg");
    GLuint wallTextureID = loadTexture("Textures/ground_0014_subsurface_1k.jpg");
    
    // Black background
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    // Compile and link shaders
    int colorShaderProgram = compileAndLinkShaders(getVertexShaderSource(), getFragmentShaderSource());
    int texturedShaderProgram = compileAndLinkShaders(getTexturedVertexShaderSource(), getTexturedFragmentShaderSource());

    // Dynamic lighting uniforms
    GLint lightPosLocation = glGetUniformLocation(texturedShaderProgram, "lightPos");
    GLint lightColorLocation = glGetUniformLocation(texturedShaderProgram, "lightColor");
    GLint viewPosLocation = glGetUniformLocation(texturedShaderProgram, "viewPos");
    
    // Initial camera setup
    vec3 cameraPosition(1.0f, 1.0f, 1.0f);
    vec3 cameraLookAt(0.0f, 0.0f, -1.0f);
    vec3 cameraUp(0.0f, 1.0f, 0.0f);
    
    // Camera settings
    float cameraSpeed = 1.5f;
    float cameraFastSpeed = 2 * cameraSpeed;
    float cameraHorizontalAngle = 90.0f;
    float cameraVerticalAngle = 0.0f;
    
    // Set projection matrix for shader
    mat4 projectionMatrix = glm::perspective(70.0f,            // field of view in degrees
                                             800.0f / 600.0f,  // aspect ratio
                                             0.01f, 100.0f);   // near and far (near > 0)
    
    // Set initial view matrix
    mat4 viewMatrix = lookAt(cameraPosition,                    // eye
                             cameraPosition + cameraLookAt,     // center
                             cameraUp );                        // up
    
    // Set View and Projection matrices on both shaders
    setViewMatrix(colorShaderProgram, viewMatrix);
    setViewMatrix(texturedShaderProgram, viewMatrix);
    setProjectionMatrix(colorShaderProgram, projectionMatrix);
    setProjectionMatrix(texturedShaderProgram, projectionMatrix);

    // Upload geometry to the GPU
    int texturedCubeVAO = createTexturedCubeVertexArrayObject();

    // Frame timing and Mouse input setup
    float lastFrameTime = glfwGetTime();    // For delta time
    int lastMouseLeftState = GLFW_RELEASE;
    double lastMousePosX, lastMousePosY;
    glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY);
    
    // Enable OpenGL States
    glEnable(GL_CULL_FACE);     // Back-face culling
    glEnable(GL_DEPTH_TEST);    // Depth testing for proper 3D rendering

    // Use Textured Cube VAO
    glBindVertexArray(texturedCubeVAO);

    // Entering Main Game Loop
    while(!glfwWindowShouldClose(window))
    {
        // Frame time calculation
        float dt = glfwGetTime() - lastFrameTime;
        lastFrameTime += dt;

        // Clear frame buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Draw textured geometry
        glUseProgram(texturedShaderProgram);

        // Bind ground texture
        glActiveTexture(GL_TEXTURE0);
        GLuint textureLocation = glGetUniformLocation(texturedShaderProgram, "textureSampler");
        glBindTexture(GL_TEXTURE_2D, groundTextureID);
        glUniform1i(textureLocation, 0);                // Set our Texture sampler to user Texture Unit 0
        
        // Draw Maze ground
        mat4 groundWorldMatrix = glm::translate(mat4(1.0f), vec3(MAZE_WIDTH / 2.0f, -0.01f, MAZE_HEIGHT / 2.0f)) *
                                 glm::scale(mat4(1.0f), vec3(MAZE_WIDTH, 0.02f, MAZE_HEIGHT));
        setWorldMatrix(texturedShaderProgram, groundWorldMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0
        
        // Draw Maze walls
        glBindTexture(GL_TEXTURE_2D, wallTextureID);
        for (int row = 0; row < MAZE_HEIGHT; ++row) {
            for (int col = 0; col < MAZE_WIDTH; ++col) {
                if (maze[row][col] == '#') {
                    // Draw wall with scaling
                    float wallHeight = 2.0f;
                    glm::mat4 wallModel = glm::translate(glm::mat4(1.0f), glm::vec3(col, wallHeight / 2.0f, row)) *
                                        glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, wallHeight, 1.0f));
                    setWorldMatrix(texturedShaderProgram, wallModel);

                    glDrawArrays(GL_TRIANGLES, 0, 36);
                }
                else if (maze[row][col] == 'S') {
                    startPosition = glm::vec3(col, 0.0f, row);
                    flameBasePosition = startPosition + vec3(0.0f, 0.5f, 0.0f); // set flame light pos
                }
                else if (maze[row][col] == 'F') {
                    finishPosition = glm::vec3(col, 0.0f, row);  // for winning logic
                    // draw a visual marker for the finish
                    glm::mat4 finishModel = glm::translate(glm::mat4(1.0f), finishPosition);
                    glUniformMatrix4fv(glGetUniformLocation(texturedShaderProgram, "worldMatrix"), 1, GL_FALSE, &finishModel[0][0]);
                    glBindTexture(GL_TEXTURE_2D, wallTextureID);
                    glDrawArrays(GL_TRIANGLES, 0, 36);
                }
            }
        }
        // Dynamic torch light (flickering + follows player)
        float time = glfwGetTime();
        float flicker = 0.8f + 0.2f * sin(glfwGetTime() * 2.0f);    // smooth glow
        vec3 dynamicLightColor = vec3(1.0f, 0.85f, 0.6f) * flicker;
        vec3 dynamicLightPos = cameraPosition + vec3(0.0f, 0.5f, 0.0f);
        dynamicLightPos.x += sin(time * 3.0f) * 0.1f;
        dynamicLightPos.z += cos(time * 2.5f) * 0.1f;

        // Set dynamic lighting uniforms
        glUseProgram(texturedShaderProgram);
        glUniform3fv(lightColorLocation, 1, &dynamicLightColor[0]);
        glUniform3fv(lightPosLocation, 1, &dynamicLightPos[0]);
        glUniform3fv(viewPosLocation, 1, &cameraPosition[0]);

        // Draw colored geometry
        glUseProgram(colorShaderProgram);
        
        // End Frame
        glfwSwapBuffers(window);
        glfwPollEvents();
        
        // ESC to quit
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
        
        // Mouse Input: update camera angles
        double mousePosX, mousePosY;
        glfwGetCursorPos(window, &mousePosX, &mousePosY);
        double dx = mousePosX - lastMousePosX;
        double dy = mousePosY - lastMousePosY;
        lastMousePosX = mousePosX;
        lastMousePosY = mousePosY;

        // Convert mouse movement to spherical angles
        const float cameraAngularSpeed = 20.0f;
        cameraHorizontalAngle -= dx * cameraAngularSpeed * dt;
        cameraVerticalAngle   -= dy * cameraAngularSpeed * dt;
        
        // Clamp vertical camera angle to [-85, 85] degrees
        cameraVerticalAngle = std::max(-85.0f, std::min(85.0f, cameraVerticalAngle));
        
        // Convert angles to direction vectors
        float theta = radians(cameraHorizontalAngle);
        float phi = radians(cameraVerticalAngle);
        cameraLookAt = vec3(cosf(phi)*cosf(theta), sinf(phi), -cosf(phi)*sinf(theta));
        vec3 cameraSideVector = glm::cross(cameraLookAt, vec3(0.0f, 1.0f, 0.0f));

        cameraSideVector = glm::normalize(cameraSideVector);
        
        // Handle keyboard input
        bool fastCam = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;
        float currentCameraSpeed = (fastCam) ? cameraFastSpeed : cameraSpeed;
        vec3 proposedPosition = cameraPosition;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            proposedPosition += cameraLookAt * dt * currentCameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            proposedPosition -= cameraLookAt * dt * currentCameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            proposedPosition += cameraSideVector * dt * currentCameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            proposedPosition -= cameraSideVector * dt * currentCameraSpeed;

        // Clamp camera Y to floor height
        proposedPosition.y = 1.0f;

        // Collision check
        int mazeX = static_cast<int>(proposedPosition.x + 0.3f);
        int mazeZ = static_cast<int>(proposedPosition.z + 0.3f);

        // Stay within bounds and avoid walls
        if (mazeX >= 0 && mazeX < MAZE_WIDTH &&
            mazeZ >= 0 && mazeZ < MAZE_HEIGHT &&
            maze[mazeZ][mazeX] != '#') {

            cameraPosition = proposedPosition;
        } else {
            // If collision, still lock to Y=1.0
            cameraPosition.y = 1.0f;
        }

        // Win Condition: Reaching Finish
        float distanceToFinish = glm::distance(glm::vec2(cameraPosition.x, cameraPosition.z), glm::vec2(finishPosition.x, finishPosition.z));
        if (distanceToFinish < 0.7f) {
            std::cout << "Congrats! You reached the end of the maze!" << std::endl;
            glfwSetWindowShouldClose(window, true); // Close the game window
        }


        // Upload view matrix to shaders
        mat4 viewMatrix(1.0f);
        viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp );
        setViewMatrix(colorShaderProgram, viewMatrix);
        setViewMatrix(texturedShaderProgram, viewMatrix);
    }

    glfwTerminate();
    
	return 0;
}

const char* getVertexShaderSource()
{
    return
        "#version 330 core\n"                          // GLSL version
        "layout (location = 0) in vec3 aPos;"          // Vertex position input
        "layout (location = 1) in vec3 aColor;"        // Vertex color input
        ""
        "uniform mat4 worldMatrix;"                    // Model transformation
        "uniform mat4 viewMatrix = mat4(1.0);"         // Default identity view
        "uniform mat4 projectionMatrix = mat4(1.0);"   // Default identity projection
        ""
        "out vec3 vertexColor;"                        // Output color to fragment shader
        "void main()"
        "{"
        "   vertexColor = aColor;"                     // Pass color to next stage
        "   mat4 modelViewProjection = projectionMatrix * viewMatrix * worldMatrix;"
        "   gl_Position = modelViewProjection * vec4(aPos.x, aPos.y, aPos.z, 1.0);" // Final screen position
        "}";
}


const char* getFragmentShaderSource()
{
    return
        "#version 330 core\n"                          // GLSL version
        "in vec3 vertexColor;"                         // Color received from vertex shader
        "out vec4 FragColor;"                          // Final pixel color
        "void main()"
        "{"
        "   FragColor = vec4(vertexColor.r, vertexColor.g, vertexColor.b, 1.0f);" // Output color with full opacity
        "}";
}

const char* getTexturedVertexShaderSource()
{
    return
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;"              // Vertex position
    "layout (location = 1) in vec3 aColor;"            // Vertex color
    "layout (location = 2) in vec2 aUV;"               // Vertex texture coordinates
    ""
    "uniform mat4 worldMatrix;"                        // Object transform
    "uniform mat4 viewMatrix;"                         // Camera transform
    "uniform mat4 projectionMatrix;"                   // Perspective projection
    ""
    "out vec3 vertexColor;"                            // Pass-through color
    "out vec2 vertexUV;"                               // Pass-through UV
    "out vec3 fragPos;"                                // Position in world space (for lighting)
    ""
    "void main()"
    "{"
    "   vertexColor = aColor;"                         // Forward color
    "   vertexUV = aUV;"                               // Forward UV
    "   vec4 worldPos = worldMatrix * vec4(aPos, 1.0);" // Compute world space position
    "   fragPos = vec3(worldPos);"                     // Store for lighting calculations
    "   gl_Position = projectionMatrix * viewMatrix * worldPos;" // Final screen-space position
    "}";
}

const char* getTexturedFragmentShaderSource()
{
    return
    "#version 330 core\n"
    "in vec3 vertexColor;"                             // Interpolated color
    "in vec2 vertexUV;"                                // UV coordinates
    "in vec3 fragPos;"                                 // Fragment's world space position
    ""
    "uniform sampler2D textureSampler;"                // Bound texture
    "uniform vec3 lightPos;"                           // Light source position
    "uniform vec3 lightColor;"                         // Light color
    "uniform vec3 viewPos;"                            // Camera/viewer position
    ""
    "out vec4 FragColor;"                              // Final output color
    ""
    "void main()"
    "{"
    "   vec3 norm = normalize(vec3(0.0, 1.0, 0.0));"     // Flat upward normal (Y-axis)
    "   vec3 lightDir = normalize(lightPos - fragPos);" // Direction to light
    "   float diff = max(dot(norm, lightDir), 0.0);"    // Lambertian (diffuse) component
    "   vec3 diffuse = diff * lightColor;"              // Final diffuse light
    "   vec3 ambient = 0.2 * lightColor;"               // Simple ambient component
    "   vec3 finalColor = (ambient + diffuse) * texture(textureSampler, vertexUV).rgb;" // Combine lighting + texture
    "   FragColor = vec4(finalColor, 1.0);"             // Output color
    "}";
}

int compileAndLinkShaders(const char* vertexShaderSource, const char* fragmentShaderSource)
{
    // compile and link shader program
    // return shader program id
    // ------------------------------------

    // vertex shader
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    // fragment shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    // link shaders
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return shaderProgram;
}

GLuint loadTexture(const char *filename)
{
    // Load Textures with dimension data
    int width, height, nrChannels;
    unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
    if (!data)
    {
        std::cerr << "Error::Texture could not load texture file: " << filename << std::endl;
        return 0;
    }

    // Create and bind textures
    GLuint textureId = 0;
    glGenTextures(1, &textureId);
    assert(textureId != 0);

    glBindTexture(GL_TEXTURE_2D, textureId);

    // Set filter parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Upload the texture to the PU
    GLenum format = 0;
    if (nrChannels == 1)
        format = GL_RED;
    else if (nrChannels == 3)
        format = GL_RGB;
    else if (nrChannels == 4)
        format = GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    // Free resources
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
    return textureId;
}

int createTexturedCubeVertexArrayObject()
{
    // Create a vertex array
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);
    
    // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texturedCubeVertexArray), texturedCubeVertexArray, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
                          3,                   // size
                          GL_FLOAT,            // type
                          GL_FALSE,            // normalized?
                          sizeof(TexturedColoredVertex), // stride - each vertex contain 2 vec3 (position, color)
                          (void*)0             // array buffer offset
                          );
    glEnableVertexAttribArray(0);
    
    
    glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(TexturedColoredVertex),
                          (void*)sizeof(vec3)      // color is offseted a vec3 (comes after position)
                          );
    glEnableVertexAttribArray(1);
    
    glVertexAttribPointer(2,                            // attribute 2 matches aUV in Vertex Shader
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(TexturedColoredVertex),
                          (void*)(2*sizeof(vec3))      // uv is offseted by 2 vec3 (comes after position and color)
                          );
    glEnableVertexAttribArray(2);
    
    return vertexArrayObject;
}