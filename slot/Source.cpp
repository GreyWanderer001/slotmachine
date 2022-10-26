#include "include/glm/gtc/matrix_transform.hpp"
#include "include/glad/glad.h"
#include "include/GLFW/glfw3.h"
#include "stb_image.h"
#include "src/shader.h"
#include <iostream>
#include <map>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void RenderText(Shader& shader, std::string text, float x, float y, float scale, glm::vec3 color);
void changebet(int& bet, GLFWwindow* window, int& timer, int &balance);
void processwin(int cuberotations[], int item[3], int& balance, bool& flag, int& bet);
void prokrut(int(&cubeRotations)[3], GLFWwindow* window, int(&item)[3], int& balance, int bet, bool& flag);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

struct Character {
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2   Size;      // Size of glyph
    glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Horizontal offset to advance to next glyph
};

std::map<GLchar, Character> Characters;
unsigned int VAO[5], VBO[5];

int main()
{


    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Casino", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure VAO/VBO for texture quads
    glGenVertexArrays(5, VAO);
    glGenBuffers(5, VBO);

    // configure global opengl state
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    Shader ourShader("src/6.2.coordinate_systems.vs", "src/6.2.coordinate_systems.fs");
    Shader ourShader1("src/6.2.coordinate_systems.vs", "src/6.2.coordinate_systems.fs");
    Shader ourShader2("src/6.2.coordinate_systems.vs", "src/6.2.coordinate_systems.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    float vertices[] = {

         0.25f, -0.5f, 0.5f,   0.0f, 0.0f,
         0.25f, -0.5f, -0.5f, -1.0f, 0.0f,
         0.5f,  0.0f, -0.5f,  -1.0f, 0.16f,   
         0.5f,  0.0f, -0.5f,  -1.0f, 0.16f,
         0.5f,  0.0f, 0.5f,    0.0f, 0.16f,
         0.25f, -0.5f, 0.5f,   0.0f, 0.0f,

         0.5f,  0.0f, 0.5f,    0.0f, 0.16f,
         0.5f,  0.0f, -0.5f,  -1.0f, 0.16f,
         0.25f, 0.5f, -0.5f,  -1.0f, 0.33f,    
         0.25f, 0.5f, -0.5f,  -1.0f, 0.33f,
         0.25f, 0.5f, 0.5f,    0.0f, 0.33f,
         0.5f,  0.0f, 0.5f,    0.0f, 0.16f,


        -0.25f, 0.5f, -0.5f,    0.0f, 0.5f,
        -0.25f,  0.5f, 0.5f,   -1.0f, 0.5f,
         0.25f,  0.5f, -0.5f,   0.0f, 0.33f,   
         0.25f,  0.5f, -0.5f,   0.0f, 0.33f,
         0.25f,  0.5f,  0.5f,  -1.0f, 0.33f,
         -0.25f,  0.5f, 0.5f,  -1.0f, 0.5f,

         -0.25f, 0.5f, -0.5f,  0.0f, 0.5f,
         -0.25f, 0.5f, 0.5f,  -1.0f, 0.5f,
         -0.5f,  0.0f, 0.5f,  -1.0f, 0.66f,   
         -0.5f,  0.0f, 0.5f,  -1.0f, 0.66f,
         -0.5f,  0.0f, -0.5f,  0.0f, 0.66f,
         -0.25f, 0.5f, -0.5f,  0.0f, 0.5f,

         -0.5f,  0.0f, -0.5f,  0.0f, 0.66f,
         -0.5f,  0.0f, 0.5f,   1.0f, 0.66f,
         -0.25f, -0.5f, 0.5f,  1.0f, 0.83f,   
         -0.25f, -0.5f, 0.5f,  1.0f, 0.83f,
         -0.25f, -0.5f,-0.5f,  0.0f, 0.83f,
         -0.5f,  0.0f, -0.5f,  0.0f, 0.66f,


         0.25f, -0.5f, 0.5f,    0.0f, 1.0f,
        0.25f,  -0.5f, -0.5f,  -1.0f, 1.0f,
         -0.25f, -0.5f, 0.5f,   0.0f, 0.83f,   
         -0.25f, -0.5f, 0.5f,   0.0f, 0.83f,
         -0.25f, -0.5f, -0.5f, -1.0f, 0.83f,
         0.25f, -0.5f, -0.5f,  -1.0f, 1.0f
    };

    glBindVertexArray(VAO[0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int texture1;
    int width, height, nrChannels;

    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load image, create texture and generate mipmaps
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char* data = stbi_load("src/Pattern01.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    float angle = 90.0f * 1;

    glm::vec3 cubePositions[] = {
        glm::vec3(1.5f, 0.f, -3.0f),
        glm::vec3(.0f, .0f, -3.0f),
        glm::vec3(-1.5f, 0.f, -3.0f),
    };

    int cubeRotations[] = {
      0, 0, 0
    };

    int item[] = { 0, 0, 0 };

    float time[] = { 16.2, 15.2, 14.2, 13.1, 12.1, 17.3 };

    int balance = 100;
    int bet = 5;
    int timer = 0;
    bool isspinin = false;

    std::cout << "Your balance: " << balance << std::endl;
    std::cout << "Your bet: " << bet << std::endl;
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        changebet(bet, window, timer, balance);
        prokrut(cubeRotations, window, item, balance, bet, isspinin);
        processwin(cubeRotations, item, balance, isspinin, bet);
       
        timer--;

        if (cubeRotations[0] >= item[0]) {
            if (cubeRotations[0] == item[0]) {
                cubeRotations[0] = 0;
            }
            else {
                cubeRotations[0] = cubeRotations[0] - 1;
            }
        }
        if (cubeRotations[1] >= item[1]) {
            if (cubeRotations[1] == item[1]) {
                cubeRotations[1] = 0;
            }
            else {
                cubeRotations[1] = cubeRotations[1] - 1;
            }
        }
        if (cubeRotations[2] >= item[2]) {
            if (cubeRotations[2] == item[2]) {
                cubeRotations[2] = 0;
            }
            else {
                cubeRotations[2] = cubeRotations[2] - 1;
            }
        }

        // render
        glClearColor(0.7f, 0.2f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

        // activate shader
        ourShader.use();

        // create transformations
        glm::mat4 view = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 projection = glm::mat4(1.0f);
        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        // pass transformation matrices to the shader
        // 
        // note: currently we set the projection matrix each frame, 
        // but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        ourShader.setMat4("projection", projection); 
        ourShader.setMat4("view", view);
        ourShader.setMat4("transform", trans);

        // render boxes
        glBindVertexArray(VAO[0]);
        for (unsigned int i = 0; i < 3; i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f);

            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 5.0f, 0.0f));
            model = glm::rotate(model, (float)glfwGetTime() * cubeRotations[i], glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::rotate(model, time[item[i]], glm::vec3(0.0f, 0.0f, 2.0f));
            ourShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
        if (isspinin == false && balance == 0) {
            break;
        }
    }

    glDeleteVertexArrays(5, VAO);
    glDeleteBuffers(5, VBO);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void changebet(int &bet, GLFWwindow* window, int &timer, int &balance) {

    if (timer <= 0) {
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && ( bet <= balance-5)) {
            bet = bet + 5;
            system("cls");
            std::cout << "Your balance: " << balance << std::endl;
            std::cout << "Your bet: " << bet << std::endl;

        }
        if ((glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) && (bet > 5)) {
            bet = bet - 5;
            system("cls");
            std::cout << "Your balance: " << balance << std::endl;
            std::cout << "Your bet: " << bet << std::endl;

        }
        if ((glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)) {
            bet = balance;
            system("cls");
            std::cout << "Your balance: " << balance << std::endl;
            std::cout << "Your bet: " << bet << std::endl;

        }
        if ((glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) && (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)) {
            balance += 1000;
            system("cls");
            std::cout << "Your balance: " << balance << std::endl;
            std::cout << "Your bet: " << bet << std::endl;

        }
        if ((glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)) {
            bet = balance/2;
            system("cls");
            std::cout << "Your balance: " << balance << std::endl;
            std::cout << "Your bet: " << bet << std::endl;

        }
        timer = 300;
    }   
    

}

void processwin(int cuberotations[], int item[3], int& balance, bool& flag, int& bet) {

    if (cuberotations[0] == 0 && cuberotations[1] == 0 && cuberotations[2] == 0 && flag == true) {
        if ((item[0] == 0 && item[1] == 0) || (item[1] == 0 && item[2] == 0) || (item[0] == 0 && item[2] == 0)) {
            balance += bet * 2;
        }
        if (item[0] == 0 && item[1] == 0 && item[2] == 0) {
            balance += bet * 3;
        }

        if ((item[0] == 1 && item[1] == 1) || (item[1] == 1 && item[2] == 1) || (item[0] == 1 && item[2] == 1)) {
            balance += bet * 3;
        }
        if (item[0] == 1 && item[1] == 1 && item[2] == 1) {
            balance += bet * 5;
        }

        if ((item[0] == 2 && item[1] == 2) || (item[1] == 2 && item[2] == 2) || (item[0] == 2 && item[2] == 2)) {
            balance += bet * 2;
        }
        if (item[0] == 2 && item[1] == 2 && item[2] == 2) {
            balance += bet * 3;
        }

        if ((item[0] == 3 && item[1] == 3) || (item[1] == 3 && item[2] == 3) || (item[0] == 3 && item[2] == 3)) {
            balance += bet * 2;
        }
        if (item[0] == 3 && item[1] == 3 && item[2] == 3) {
            balance += bet * 3;
        }

        if ((item[0] == 4 && item[1] == 4) || (item[1] == 4 && item[2] == 4) || (item[0] == 4 && item[2] == 4)) {
            balance += bet * 4;
        }
        if (item[0] == 4 && item[1] == 4 && item[2] == 4) {
            balance += bet * 10;
        }

        if ((item[0] == 5 && item[1] == 5) || (item[1] == 5 && item[2] == 5) || (item[0] == 5 && item[2] == 5)) {
            balance += bet * 2;
        }
        if (item[0] == 5 && item[1] == 5 && item[2] == 5) {
            balance += bet * 3;
        }
        if (balance < bet) {
            bet = balance;
        }

        flag = false;
        system("cls");
        std::cout << "Your balance: " << balance << std::endl;
        std::cout << "Your bet: " << bet << std::endl;
    }
}

void prokrut(int(&cubeRotations)[3], GLFWwindow* window, int(&item)[3], int &balance, int bet, bool& flag)
{
    if (((glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) or (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)) && flag == false) {
        system("cls");
        srand((int)time(0));
        balance = balance - bet;
        int v1 = rand() % 6000 + 4000;
        int v2 = rand() % 3900 + 1500;
        int v3 = rand() % 1400 + 1000;
        int v4 = rand() % 6;
        int v5 = rand() % 6;
        int v6 = rand() % 6;
        cubeRotations[0] = v1;
        cubeRotations[1] = v2;
        cubeRotations[2] = v3;
        item[0] = v4;
        item[1] = v5;
        item[2] = v6;
        flag = true;
        std::cout << "Spinning...";
    }
}