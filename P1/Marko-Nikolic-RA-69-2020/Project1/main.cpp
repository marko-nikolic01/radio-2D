
// Marko Nikolic
// RA 69/2020

#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define CRES 100

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);

unsigned int compileShader(GLenum type, const char* source);
unsigned int createShader(const char* vsSource, const char* fsSource);
static unsigned loadImageToTexture(const char* filePath);

bool radioOn = false;
float anthenaOffset = -0.55f;
float AMFMScaleIndicatorOffset = 0.0f;
float volumeBarIndicatorOffset = 0.0f;
float speakerMembraneScale = 1.0f;
bool FMon = true;

int main(void) {

	if (!glfwInit()) {
		std::cout << "GLFW Biblioteka se nije ucitala! :(\n";
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window;
	unsigned int wWidth = 500;
	unsigned int wHeight = 500;
	const char wTitle[] = "[Generic Title]";
	window = glfwCreateWindow(wWidth, wHeight, wTitle, NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Prozor nije napravljen! :(\n";
		glfwTerminate();
		return 2;
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, keyCallback);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW nije mogao da se ucita! :'(\n";
		return 3;
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// SHADDERS
	unsigned int radioShadder = createShader("basic.vert", "basic.frag");
	unsigned int radioOutlineShadder = createShader("basic.vert", "basic.frag");
	unsigned int radioOnOffButtonShadder = createShader("basic.vert", "basic.frag");
	unsigned int radioOnOffButtonIndicatorShadder = createShader("basic.vert", "basic.frag");
	unsigned int radioOnOffButtonIndicatorLineShadder = createShader("basic.vert", "basic.frag");
	unsigned int radioOnOffLampShadder = createShader("basic.vert", "basic.frag");
	unsigned int AMFMScaleShadder = createShader("basic.vert", "basic.frag");
	unsigned int AMFMScaleValuesShadder = createShader("basic.vert", "basic.frag");
	unsigned int anthenaBaseShadder = createShader("basic.vert", "basic.frag");
	unsigned int anthenaBaseOutlineShadder = createShader("basic.vert", "basic.frag");
	unsigned int anthenaShadder = createShader("basic.vert", "basic.frag");
	unsigned int anthenaOutlineShadder = createShader("basic.vert", "basic.frag");
	unsigned int speakerShadder = createShader("basic.vert", "basic.frag");
	unsigned int speakerMembraneBigShadder = createShader("membrane.vert", "basic.frag");
	unsigned int speakerMembraneSmallShadder = createShader("basic.vert", "basic.frag");
	unsigned int speakerMeshShadder = createShader("texture.vert", "texture.frag");
	unsigned int speakerOutlineShadder = createShader("basic.vert", "basic.frag");
	unsigned int volumeBarShadder = createShader("basic.vert", "basic.frag");
	unsigned int volumeBarIndicatorShadder = createShader("basic.vert", "basic.frag");
	unsigned int volumeBarHiderShadder = createShader("basic.vert", "basic.frag");
	unsigned int volumeBarOutlineShadder = createShader("basic.vert", "basic.frag");
	unsigned int volumeRailShadder = createShader("basic.vert", "basic.frag");
	unsigned int volumeSliderShadder = createShader("basic.vert", "basic.frag");
	unsigned int volumeSliderOutlineShadder = createShader("basic.vert", "basic.frag");
	unsigned int AMFMRailShadder = createShader("basic.vert", "basic.frag");
	unsigned int AMFMRailOutlineShadder = createShader("basic.vert", "basic.frag");
	unsigned int AMFMSwitchShadder = createShader("basic.vert", "basic.frag");
	unsigned int AMFMSwitchOutlineShadder = createShader("basic.vert", "basic.frag");
	unsigned int AMShadder = createShader("texture.vert", "texture.frag");
	unsigned int FMShadder = createShader("texture.vert", "texture.frag");
	unsigned int tableShadder = createShader("texture.vert", "texture.frag");
	unsigned int tableOutlineShadder = createShader("basic.vert", "basic.frag");
	unsigned int signatureShadder = createShader("texture.vert", "texture.frag");

	// VAO/VBO
	const unsigned int VAOVBOCount = 42;
	unsigned int VAO[VAOVBOCount];
	glGenVertexArrays(VAOVBOCount, VAO);
	unsigned int VBO[VAOVBOCount];
	glGenBuffers(VAOVBOCount, VBO);
	unsigned int stride;

	// RADIO
	float verticesRadio[] =
	{
		// X      Y
		-0.9, -0.75,
		0.9, -0.75,
		-0.9,  0.25,
		0.9,  0.25,
		-0.525, 0.25,
		-0.425, 0.25,
		-0.525, 0.6,
		-0.425, 0.6,
		0.425, 0.25,
		0.525, 0.25,
		0.425, 0.6,
		0.525, 0.6,
		-0.425, 0.5,
		0.425, 0.5,
		-0.425, 0.6,
		0.425, 0.6
	};
	stride = 2 * sizeof(float);
	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesRadio), verticesRadio, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);

	// RADIO OUTLINES
	float verticesRadioOutlines[] =
	{
		// X      Y
		-0.9, -0.75,
		0.9, -0.75,
		0.9,  0.25,
		-0.9,  0.25,
		-0.9, -0.75,
		-0.9, 0.0,
		0.9, 0.0,
		-0.525, 0.25,
		-0.525, 0.6,
		0.525, 0.6,
		0.525, 0.25,
		-0.425, 0.25,
		-0.425, 0.5,
		0.425, 0.5,
		0.425, 0.25,
	};
	stride = 2 * sizeof(float);
	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesRadioOutlines), verticesRadioOutlines, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);

	// RADIO ON/OFF BUTTON
	float radioOnOffButton[CRES * 2 + 4];
	float r = 0.05;
	radioOnOffButton[0] = 0.8;
	radioOnOffButton[1] = 0.125;
	for (int i = 0; i <= CRES; i++)
	{

		radioOnOffButton[2 + 2 * i] = radioOnOffButton[0] + r * cos((3.141592 / 180) * (i * 360 / CRES));
		radioOnOffButton[2 + 2 * i + 1] = radioOnOffButton[1] + r * sin((3.141592 / 180) * (i * 360 / CRES));
	}
	glBindVertexArray(VAO[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(radioOnOffButton), radioOnOffButton, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// RADIO ON/OFF BUTTON INDICATOR
	float radioOnOffButtonIndicator[CRES * 2 + 4];
	r = 0.04;
	radioOnOffButtonIndicator[0] = 0.8;
	radioOnOffButtonIndicator[1] = 0.125;
	for (int i = 0; i <= CRES; i++)
	{
		radioOnOffButtonIndicator[2 + 2 * i] = radioOnOffButtonIndicator[0] + r * cos((3.141592 / 180) * (i * 360 / CRES));
		radioOnOffButtonIndicator[2 + 2 * i + 1] = radioOnOffButtonIndicator[1] + r * sin((3.141592 / 180) * (i * 360 / CRES));
	}
	glBindVertexArray(VAO[3]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(radioOnOffButtonIndicator), radioOnOffButtonIndicator, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// RADIO ON/OFF BUTTON CONTENT INDICATOR INNER
	float radioOnOffButtonIndicatorInner[CRES * 2 + 4];
	r = 0.035;
	radioOnOffButtonIndicatorInner[0] = 0.8;
	radioOnOffButtonIndicatorInner[1] = 0.125;
	for (int i = 0; i <= CRES; i++)
	{
		radioOnOffButtonIndicatorInner[2 + 2 * i] = radioOnOffButtonIndicatorInner[0] + r * cos((3.141592 / 180) * (i * 360 / CRES));
		radioOnOffButtonIndicatorInner[2 + 2 * i + 1] = radioOnOffButtonIndicatorInner[1] + r * sin((3.141592 / 180) * (i * 360 / CRES));
	}
	glBindVertexArray(VAO[4]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(radioOnOffButtonIndicatorInner), radioOnOffButtonIndicatorInner, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// RADIO ON/OFF BUTTON INDICATOR LINE
	float verticesRadioButtonIndicatorLine[] =
	{
		// X      Y
		0.8, 0.125,
		0.8, 0.165
	};
	stride = 2 * sizeof(float);
	glBindVertexArray(VAO[5]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[5]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesRadioButtonIndicatorLine), verticesRadioButtonIndicatorLine, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);

	// RADIO ON/OFF LAMP
	float radioOnOffLamp[CRES * 2 + 4];
	r = 0.02;
	radioOnOffLamp[0] = 0.725;
	radioOnOffLamp[1] = 0.125;
	for (int i = 0; i <= CRES; i++)
	{
		radioOnOffLamp[2 + 2 * i] = radioOnOffLamp[0] + r * cos((3.141592 / 180) * (i * 360 / CRES));
		radioOnOffLamp[2 + 2 * i + 1] = radioOnOffLamp[1] + r * sin((3.141592 / 180) * (i * 360 / CRES));
	}
	glBindVertexArray(VAO[6]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[6]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(radioOnOffLamp), radioOnOffLamp, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// RADIO ON/OFF LAMP LIGHT
	float radioOnOffLampLight[CRES * 2 + 4];
	r = 0.01;
	radioOnOffLampLight[0] = 0.725;
	radioOnOffLampLight[1] = 0.125;
	for (int i = 0; i <= CRES; i++)
	{
		radioOnOffLampLight[2 + 2 * i] = radioOnOffLampLight[0] + r * cos((3.141592 / 180) * (i * 360 / CRES));
		radioOnOffLampLight[2 + 2 * i + 1] = radioOnOffLampLight[1] + r * sin((3.141592 / 180) * (i * 360 / CRES));
	}
	glBindVertexArray(VAO[7]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[7]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(radioOnOffLampLight), radioOnOffLampLight, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// AM/FM SCALE
	float verticesAMFMScale[] =
	{
		// X      Y
		-0.85, 0.075,
		0.65, 0.075,
		-0.85,  0.175,
		0.65,  0.175
	};
	stride = 2 * sizeof(float);
	glBindVertexArray(VAO[8]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[8]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesAMFMScale), verticesAMFMScale, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);

	// AM/FM SCALE VALUES
	float verticesAMFMScaleValues[134] =
	{
		// X      Y
		-0.85, 0.125,
		0.65, 0.125
	};
	float position = 0.8;
	for (int i = 1; i <= 29; i ++) {
		verticesAMFMScaleValues[4 * i] = -0.8 + (i - 1) * 0.05;
		verticesAMFMScaleValues[4 * i + 1] = 0.1375;
		verticesAMFMScaleValues[4 * i + 2] = -0.8 + (i - 1) * 0.05;
		verticesAMFMScaleValues[4 * i + 3] = 0.1125;
	}
	verticesAMFMScaleValues[120] = -0.80;
	verticesAMFMScaleValues[121] = 0.075;
	verticesAMFMScaleValues[122] = -0.80;
	verticesAMFMScaleValues[123] = 0.1625;
	verticesAMFMScaleValues[124] = -0.85;
	verticesAMFMScaleValues[125] = 0.075;
	verticesAMFMScaleValues[126] = 0.65;
	verticesAMFMScaleValues[127] = 0.075;
	verticesAMFMScaleValues[128] = 0.65;
	verticesAMFMScaleValues[129] = 0.175;
	verticesAMFMScaleValues[130] = -0.85;
	verticesAMFMScaleValues[131] = 0.175;
	verticesAMFMScaleValues[132] = -0.85;
	verticesAMFMScaleValues[133] = 0.075;
	stride = 2 * sizeof(float);
	glBindVertexArray(VAO[9]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[9]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesAMFMScaleValues), verticesAMFMScaleValues, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);

	// ANTHENA BASE
	float verticesAnthenaBase[] =
	{
		// X      Y
		-0.8, 0.25,
		-0.75, 0.25,
		-0.8,  0.35,
		-0.75,  0.35
	};
	stride = 2 * sizeof(float);
	glBindVertexArray(VAO[10]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[10]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesAnthenaBase), verticesAnthenaBase, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);

	// ANTHENA BASE OUTLINE
	float verticesAnthenaBaseOutline[] =
	{
		// X      Y
		-0.75, 0.25,
		-0.75,  0.35,
		-0.8,  0.35,
		-0.8, 0.25
	};
	stride = 2 * sizeof(float);
	glBindVertexArray(VAO[11]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[11]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesAnthenaBaseOutline), verticesAnthenaBaseOutline, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);

	// ANTHENA
	float verticesAnthena[] =
	{
		// X      Y
		-0.7875, 0.35,
		-0.7625, 0.35,
		-0.7875,  0.90,
		-0.7625,  0.90
	};
	stride = 2 * sizeof(float);
	glBindVertexArray(VAO[12]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[12]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesAnthena), verticesAnthena, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);

	// ANTHENA OUTLINE
	float verticesAnthenaOutline[] =
	{
		// X      Y
		-0.7625, 0.35,
		-0.7625,  0.90,
		-0.7875,  0.90,
		-0.7875, 0.35
	};
	stride = 2 * sizeof(float);
	glBindVertexArray(VAO[13]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[13]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesAnthenaOutline), verticesAnthenaOutline, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);

	// ANTHENA TIP
	float verticesAnthenaTip[] =
	{
		// X      Y
		-0.8, 0.90,
		-0.75, 0.90,
		-0.8,  0.95,
		-0.75,  0.95
	};
	stride = 2 * sizeof(float);
	glBindVertexArray(VAO[14]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[14]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesAnthenaTip), verticesAnthenaTip, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);

	// ANTHENA TIP OUTLINE
	float verticesAnthenaTipOutline[] =
	{
		// X      Y
		-0.8, 0.90,
		-0.8, 0.95,
		-0.75, 0.95,
		-0.75,  0.90,
		-0.8, 0.90
	};
	stride = 2 * sizeof(float);
	glBindVertexArray(VAO[15]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[15]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesAnthenaTipOutline), verticesAnthenaTipOutline, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);

	// SPEAKER
	float verticesSpeaker[] =
	{
		// X      Y
		0.25, -0.65,
		0.8, -0.65,
		0.25,  -0.1,
		0.8,  -0.1
	};
	stride = 2 * sizeof(float);
	glBindVertexArray(VAO[16]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[16]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesSpeaker), verticesSpeaker, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);

	// SPEAKER MEMBRANE BIG
	float speakerMembraneBig[CRES * 2 + 4];
	r = 0.25;
	speakerMembraneBig[0] = 0.525;
	speakerMembraneBig[1] = -0.375;
	for (int i = 0; i <= CRES; i++)
	{

		speakerMembraneBig[2 + 2 * i] = speakerMembraneBig[0] + r * cos((3.141592 / 180) * (i * 360 / CRES));
		speakerMembraneBig[2 + 2 * i + 1] = speakerMembraneBig[1] + r * sin((3.141592 / 180) * (i * 360 / CRES));
	}
	glBindVertexArray(VAO[17]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[17]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(speakerMembraneBig), speakerMembraneBig, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// SPEAKER MEMBRANE SMALL
	float speakerMembraneSmall[CRES * 2 + 4];
	r = 0.1;
	speakerMembraneSmall[0] = 0.525;
	speakerMembraneSmall[1] = -0.375;
	for (int i = 0; i <= CRES; i++)
	{

		speakerMembraneSmall[2 + 2 * i] = speakerMembraneSmall[0] + r * cos((3.141592 / 180) * (i * 360 / CRES));
		speakerMembraneSmall[2 + 2 * i + 1] = speakerMembraneSmall[1] + r * sin((3.141592 / 180) * (i * 360 / CRES));
	}
	glBindVertexArray(VAO[18]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[18]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(speakerMembraneSmall), speakerMembraneSmall, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// SPEAKER MESH
	float speakerMeshVertices[] =
	{   //X    Y      S    T 
		0.25, -0.65, 0.0, 0.0,
		0.8, -0.65, 1.0, 0.0,
		0.25,  -0.1, 0.0, 1.0,
		0.8,  -0.1, 1.0, 1.0
	};
	stride = (2 + 2) * sizeof(float);
	glBindVertexArray(VAO[19]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[19]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(speakerMeshVertices), speakerMeshVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	unsigned meshTexture = loadImageToTexture("res/mesh.png");
	glBindTexture(GL_TEXTURE_2D, meshTexture);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
	unsigned uTexLoc0 = glGetUniformLocation(speakerMeshShadder, "uTex");
	glUniform1i(uTexLoc0, 0);

	// SPEAKER OUTLINE
	float speakerOutlineVertices[] =
	{
		// X      Y
		0.25, -0.65,
		0.8, -0.65,
		0.8,  -0.1,
		0.25,  -0.1,
		0.25, -0.65
	};
	stride = 2 * sizeof(float);
	glBindVertexArray(VAO[20]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[20]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(speakerOutlineVertices), speakerOutlineVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);

	// SPEAKER
	float verticesSpeaker2[] =
	{
		// X      Y
		-0.8, -0.65,
		-0.25, -0.65,
		-0.8,  -0.1,
		-0.25,  -0.1
	};
	stride = 2 * sizeof(float);
	glBindVertexArray(VAO[28]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[28]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesSpeaker2), verticesSpeaker2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);

	// SPEAKER MEMBRANE BIG
	float speakerMembraneBig2[CRES * 2 + 4];
	r = 0.25;
	speakerMembraneBig2[0] = -0.525;
	speakerMembraneBig2[1] = -0.375;
	for (int i = 0; i <= CRES; i++)
	{

		speakerMembraneBig2[2 + 2 * i] = speakerMembraneBig2[0] + r * cos((3.141592 / 180) * (i * 360 / CRES));
		speakerMembraneBig2[2 + 2 * i + 1] = speakerMembraneBig2[1] + r * sin((3.141592 / 180) * (i * 360 / CRES));
	}
	glBindVertexArray(VAO[29]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[29]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(speakerMembraneBig2), speakerMembraneBig2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// SPEAKER MEMBRANE SMALL
	float speakerMembraneSmall2[CRES * 2 + 4];
	r = 0.1;
	speakerMembraneSmall2[0] = -0.525;
	speakerMembraneSmall2[1] = -0.375;
	for (int i = 0; i <= CRES; i++)
	{

		speakerMembraneSmall2[2 + 2 * i] = speakerMembraneSmall2[0] + r * cos((3.141592 / 180) * (i * 360 / CRES));
		speakerMembraneSmall2[2 + 2 * i + 1] = speakerMembraneSmall2[1] + r * sin((3.141592 / 180) * (i * 360 / CRES));
	}
	glBindVertexArray(VAO[30]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[30]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(speakerMembraneSmall2), speakerMembraneSmall2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// SPEAKER MESH
	float speakerMeshVertices2[] =
	{   //X    Y      S    T 
		-0.8, -0.65, 0.0, 0.0,
		-0.25, -0.65, 1.0, 0.0,
		-0.8,  -0.1, 0.0, 1.0,
		-0.25,  -0.1, 1.0, 1.0
	};
	stride = (2 + 2) * sizeof(float);
	glBindVertexArray(VAO[31]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[31]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(speakerMeshVertices2), speakerMeshVertices2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindTexture(GL_TEXTURE_2D, meshTexture);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(uTexLoc0, 0);

	// SPEAKER OUTLINE
	float speakerOutlineVertices2[] =
	{
		// X      Y
		-0.8, -0.65,
		-0.25, -0.65,
		-0.25,  -0.1,
		-0.8,  -0.1,
		-0.8, -0.65
	};
	stride = 2 * sizeof(float);
	glBindVertexArray(VAO[32]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[32]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(speakerOutlineVertices2), speakerOutlineVertices2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);

	// VOLUME BAR
	float volumeBarVertices[] =
	{
		// X      Y
		0.15, -0.65,
		0.2, -0.65,
		0.15,  -0.1,
		0.2,  -0.1
	};
	stride = 2 * sizeof(float);
	glBindVertexArray(VAO[21]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[21]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(volumeBarVertices), volumeBarVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);

	// VOLUME BAR INDICATOR
	float volumeBarIndicatorVertices[] =
	{
		// X      Y
		0.15, -1.2,
		0.2, -1.2,
		0.15,  -0.65,
		0.2,  -0.65
	};
	stride = 2 * sizeof(float);
	glBindVertexArray(VAO[22]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[22]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(volumeBarIndicatorVertices), volumeBarIndicatorVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);

	// VOLUME BAR INDICATOR HIDER
	float volumeBarHiderVertices[] =
	{
		// X      Y
		0.15, -0.75,
		0.2, -0.75,
		0.15,  -0.65,
		0.2,  -0.65
	};
	stride = 2 * sizeof(float);
	glBindVertexArray(VAO[23]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[23]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(volumeBarHiderVertices), volumeBarHiderVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);

	// VOLUME BAR OUTLINE
	float volumeBarOutlineVertices[] =
	{
		// X      Y
		0.15, -0.65,
		0.2, -0.65,
		0.2,  -0.1,
		0.15,  -0.1,
		0.15, -0.65
	};
	stride = 2 * sizeof(float);
	glBindVertexArray(VAO[24]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[24]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(volumeBarOutlineVertices), volumeBarOutlineVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);

	// VOLUME RAIL
	float volumeRailVertices[] =
	{
		// X      Y
		0.075, -0.65,
		0.075, -0.1
	};
	stride = 2 * sizeof(float);
	glBindVertexArray(VAO[25]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[25]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(volumeRailVertices), volumeRailVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);

	// VOLUME BAR SLIDER
	float volumeBarSliderVertices[] =
	{
		// X      Y
		0.0375, -0.65,
		0.1125, -0.65,
		0.0375,  -0.575,
		0.1125,  -0.575
	};
	stride = 2 * sizeof(float);
	glBindVertexArray(VAO[26]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[26]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(volumeBarSliderVertices), volumeBarSliderVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);

	// VOLUME BAR SLIDER OUTLINE
	float volumeBarSliderOutlineVertices[] =
	{
		// X      Y
		0.0375, -0.65,
		0.1125, -0.65,
		0.1125,  -0.575,
		0.0375,  -0.575,
		0.0375, -0.65
	};
	stride = 2 * sizeof(float);
	glBindVertexArray(VAO[27]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[27]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(volumeBarSliderOutlineVertices), volumeBarSliderOutlineVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);

	// AM/FM RAIL
	float AMFMRailVertices[] =
	{
		// X      Y
		-0.2, -0.5,
		-0.05, -0.5,
		-0.2,  -0.25,
		-0.05,  -0.25
	};
	stride = 2 * sizeof(float);
	glBindVertexArray(VAO[33]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[33]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(AMFMRailVertices), AMFMRailVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);

	// AM/FM RAIL OUTLINE
	float AMFMRailOutlineVertices[] =
	{
		// X      Y
		-0.2, -0.5,
		-0.05, -0.5,
		-0.05,  -0.25,
		-0.2,  -0.25,
		-0.2, -0.5
	};
	stride = 2 * sizeof(float);
	glBindVertexArray(VAO[34]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[34]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(AMFMRailOutlineVertices), AMFMRailOutlineVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);

	// AM/FM SWITCH
	float AMFMSwitchVertices[] =
	{
		// X      Y
		-0.2, -0.5,
		-0.05, -0.5,
		-0.2,  -0.4,
		-0.05,  -0.4
	};
	stride = 2 * sizeof(float);
	glBindVertexArray(VAO[35]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[35]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(AMFMSwitchVertices), AMFMSwitchVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);

	// AM/FM SWITCH OUTLINE
	float AMFMSwitchOutlineVertices[] =
	{
		// X      Y
		-0.2, -0.5,
		-0.05, -0.5,
		-0.05,  -0.4,
		-0.2,  -0.4,
		-0.2, -0.5
	};
	stride = 2 * sizeof(float);
	glBindVertexArray(VAO[36]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[36]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(AMFMSwitchOutlineVertices), AMFMSwitchOutlineVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);

	// AM
	float AMVertices[] =
	{   //X    Y      S    T 
		-0.2, -0.2, 0.0, 0.0,
		-0.05, -0.2, 1.0, 0.0,
		-0.2,  -0.1, 0.0, 1.0,
		-0.05,  -0.1, 1.0, 1.0
	};
	stride = (2 + 2) * sizeof(float);
	glBindVertexArray(VAO[37]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[37]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(AMVertices), AMVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	unsigned AMTexture = loadImageToTexture("res/am.png");
	glBindTexture(GL_TEXTURE_2D, AMTexture);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
	unsigned uTexLoc1 = glGetUniformLocation(AMShadder, "uTex");
	glUniform1i(uTexLoc1, 0);

	// FM
	float FMVertices[] =
	{   //X    Y      S    T 
		-0.2, -0.65, 0.0, 0.0,
		-0.05, -0.65, 1.0, 0.0,
		-0.2,  -0.55, 0.0, 1.0,
		-0.05,  -0.55, 1.0, 1.0
	};
	stride = (2 + 2) * sizeof(float);
	glBindVertexArray(VAO[38]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[38]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(FMVertices), FMVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	unsigned FMTexture = loadImageToTexture("res/fm.png");
	glBindTexture(GL_TEXTURE_2D, FMTexture);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
	unsigned uTexLoc2 = glGetUniformLocation(FMShadder, "uTex");
	glUniform1i(uTexLoc2, 0);

	// TABLE
	float tableVertices[] =
	{   //X    Y      S    T 
		-1.0, -1.0, 0.0, 0.0,
		1.0, -1.0, 1.0, 0.0,
		-1.0,  -0.75, 0.0, 1.0,
		1.0,  -0.75, 1.0, 1.0
	};
	stride = (2 + 2) * sizeof(float);
	glBindVertexArray(VAO[39]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[39]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tableVertices), tableVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	unsigned tableTexture = loadImageToTexture("res/table.jpg");
	glBindTexture(GL_TEXTURE_2D, tableTexture);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
	unsigned uTexLoc3 = glGetUniformLocation(tableShadder, "uTex");
	glUniform1i(uTexLoc3, 0);

	// TABLE OUTLINE
	float tableOutlineVertices[] =
	{
		// X      Y
		-0.1, -0.75,
		1, -0.75
	};
	stride = 2 * sizeof(float);
	glBindVertexArray(VAO[40]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[40]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tableOutlineVertices), tableOutlineVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);

	// SIGNATURE
	float signatureVertices[] =
	{   //X    Y      S    T 
		0.0, 0.85, 0.0, 0.0,
		1.0, 0.85, 1.0, 0.0,
		0.0,  1.0, 0.0, 1.0,
		1.0,  1.0, 1.0, 1.0
	};
	stride = (2 + 2) * sizeof(float);
	glBindVertexArray(VAO[41]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[41]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(signatureVertices), signatureVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	unsigned signatureTexture = loadImageToTexture("res/signature.png");
	glBindTexture(GL_TEXTURE_2D, signatureTexture);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
	unsigned uTexLoc4 = glGetUniformLocation(signatureShadder, "uTex");
	glUniform1i(uTexLoc4, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window))
	{
		glLineWidth(1.0f);

		int indexRGB = 0;
		if (radioOn) {
			indexRGB = 1;
		}

		float onOffRGB[3] = { 0.0f, 0.0f, 0.0f };
		float lampRGB[3] = { 0.0f, 0.0f, 0.0f };
		onOffRGB[indexRGB] = 0.4f;
		lampRGB[indexRGB] = 1.0f;

		float membraneAmplitude;
		if (volumeBarIndicatorOffset <= 0 || !radioOn) membraneAmplitude = 0.0f;
		else if(volumeBarIndicatorOffset <= 0.15 && radioOn) membraneAmplitude = 0.02f;
		else if (volumeBarIndicatorOffset > 0.15 && volumeBarIndicatorOffset <= 0.3 && radioOn) membraneAmplitude = 0.03f;
		else membraneAmplitude = 0.05f;
		speakerMembraneScale = 1 + membraneAmplitude / 2 * (1 - sin(glfwGetTime() * 25));

		// BACKGROUND
		glClearColor(0.9, 0.9, 0.9, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		// ANTHENA
		glUseProgram(anthenaShadder);
		glUniform2f(glGetUniformLocation(anthenaShadder, "offset"), 0.0f, round(anthenaOffset * 100.0) / 100.0);
		glUniform3f(glGetUniformLocation(anthenaShadder, "color"), 0.8f, 0.8f, 0.8f);
		glBindVertexArray(VAO[12]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		// ANTHENA OUTLINE
		glUseProgram(anthenaOutlineShadder);
		glUniform2f(glGetUniformLocation(anthenaOutlineShadder, "offset"), 0.0f, round(anthenaOffset * 100.0) / 100.0);
		glUniform3f(glGetUniformLocation(anthenaOutlineShadder, "color"), 0.0f, 0.0f, 0.0f);
		glBindVertexArray(VAO[13]);
		glDrawArrays(GL_LINES, 0, 2);
		glDrawArrays(GL_LINES, 1, 2);
		glDrawArrays(GL_LINES, 2, 2);

		// RADIO
		glUseProgram(radioShadder);
		glUniform3f(glGetUniformLocation(radioShadder, "color"), 0.5f, 0.5f, 0.5f);
		glBindVertexArray(VAO[0]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
		glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
		glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);
		
		// RADIO ON/OFF BUTTON
		glUseProgram(radioOnOffButtonShadder);
		glUniform3f(glGetUniformLocation(radioOnOffButtonShadder, "color"), 0.2f, 0.2f, 0.2f);
		glBindVertexArray(VAO[2]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(radioOnOffButton) / (2 * sizeof(float)));

		// RADIO ON/OFF BUTTON INDICATOR
		glUseProgram(radioOnOffButtonIndicatorShadder);
		glUniform3f(glGetUniformLocation(radioOnOffButtonIndicatorShadder, "color"), onOffRGB[0], onOffRGB[1], onOffRGB[2]);
		glBindVertexArray(VAO[3]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(radioOnOffButtonIndicator) / (2 * sizeof(float)));

		// RADIO ON/OFF BUTTON CONTENT INDICATOR INNER
		glUniform3f(glGetUniformLocation(radioOnOffButtonIndicatorShadder, "color"), 0.2f, 0.2f, 0.2f);
		glBindVertexArray(VAO[4]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(radioOnOffButtonIndicatorInner) / (2 * sizeof(float)));

		// RADIO ON/OFF BUTTON INDICATOR LINE
		glUseProgram(radioOnOffButtonIndicatorLineShadder);
		glUniform3f(glGetUniformLocation(radioOnOffButtonIndicatorLineShadder, "color"), onOffRGB[0], onOffRGB[1], onOffRGB[2]);
		glBindVertexArray(VAO[5]);
		glLineWidth(2.0f);
		glDrawArrays(GL_LINES, 0, 2);

		// RADIO ON/OFF LAMP
		glUseProgram(radioOnOffLampShadder);
		glUniform3f(glGetUniformLocation(radioOnOffLampShadder, "color"), 0.2f, 0.2f, 0.2f);
		glBindVertexArray(VAO[6]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(radioOnOffLamp) / (2 * sizeof(float)));

		// RADIO ON/OFF LAMP LIGHT
		glUseProgram(radioOnOffLampShadder);
		glUniform3f(glGetUniformLocation(radioOnOffLampShadder, "color"), lampRGB[0], lampRGB[1], lampRGB[2]);
		glBindVertexArray(VAO[7]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(radioOnOffLamp) / (2 * sizeof(float)));

		// AM/FM SCALE
		glUseProgram(AMFMScaleShadder);
		glUniform3f(glGetUniformLocation(AMFMScaleShadder, "color"), 1.0f, 1.0f, 1.0f);
		glBindVertexArray(VAO[8]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		// AM/FM SCALE VALUES 
		glUseProgram(AMFMScaleValuesShadder);
		glUniform3f(glGetUniformLocation(AMFMScaleValuesShadder, "color"), 0.0f, 0.0f, 0.0f);
		glBindVertexArray(VAO[9]);
		glLineWidth(3.0f);
		glDrawArrays(GL_LINES, 0, 2);
		glLineWidth(2.0f);
		glDrawArrays(GL_LINES, 2, 2);
		glDrawArrays(GL_LINES, 4, 2);
		glDrawArrays(GL_LINES, 6, 2);
		glDrawArrays(GL_LINES, 8, 2);
		glDrawArrays(GL_LINES, 10, 2);
		glDrawArrays(GL_LINES, 12, 2);
		glDrawArrays(GL_LINES, 14, 2);
		glDrawArrays(GL_LINES, 16, 2);
		glDrawArrays(GL_LINES, 18, 2);
		glDrawArrays(GL_LINES, 20, 2);
		glDrawArrays(GL_LINES, 22, 2);
		glDrawArrays(GL_LINES, 24, 2);
		glDrawArrays(GL_LINES, 26, 2);
		glDrawArrays(GL_LINES, 28, 2);
		glDrawArrays(GL_LINES, 30, 2);
		glDrawArrays(GL_LINES, 32, 2);
		glDrawArrays(GL_LINES, 34, 2);
		glDrawArrays(GL_LINES, 36, 2);
		glDrawArrays(GL_LINES, 38, 2);
		glDrawArrays(GL_LINES, 40, 2);
		glDrawArrays(GL_LINES, 42, 2);
		glDrawArrays(GL_LINES, 44, 2);
		glDrawArrays(GL_LINES, 46, 2);
		glDrawArrays(GL_LINES, 48, 2);
		glDrawArrays(GL_LINES, 50, 2);
		glDrawArrays(GL_LINES, 52, 2);
		glDrawArrays(GL_LINES, 54, 2);
		glDrawArrays(GL_LINES, 56, 2);
		glDrawArrays(GL_LINES, 58, 2);
		glUniform3f(glGetUniformLocation(AMFMScaleValuesShadder, "color"), 1.0f, 0.0f, 0.0f);
		glUniform2f(glGetUniformLocation(AMFMScaleValuesShadder, "offset"), round(AMFMScaleIndicatorOffset * 100.0) / 100.0, 0.0f);
		glLineWidth(3.0f);
		glDrawArrays(GL_LINES, 60, 2);
		glUniform3f(glGetUniformLocation(AMFMScaleValuesShadder, "color"), 0.0f, 0.0f, 0.0f);
		glUniform2f(glGetUniformLocation(AMFMScaleValuesShadder, "offset"), 0.0f, 0.0f);
		glLineWidth(1.0f);
		glDrawArrays(GL_LINES, 62, 2);
		glDrawArrays(GL_LINES, 63, 2);
		glDrawArrays(GL_LINES, 64, 2);
		glDrawArrays(GL_LINES, 65, 2);

		// ANTHENA BASE
		glUseProgram(anthenaBaseShadder);
		glUniform3f(glGetUniformLocation(anthenaBaseShadder, "color"), 0.5f, 0.5f, 0.5f);
		glBindVertexArray(VAO[10]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		// ANTHENA BASE OUTLINE
		glUseProgram(anthenaBaseOutlineShadder);
		glUniform3f(glGetUniformLocation(anthenaBaseOutlineShadder, "color"), 0.0f, 0.0f, 0.0f);
		glBindVertexArray(VAO[11]);
		glDrawArrays(GL_LINES, 0, 2);
		glDrawArrays(GL_LINES, 1, 2);
		glDrawArrays(GL_LINES, 2, 2);

		// ANTHENA TIP
		glUseProgram(anthenaShadder);
		glUniform2f(glGetUniformLocation(anthenaShadder, "offset"), 0.0f, round(anthenaOffset * 100.0) / 100.0);
		glUniform3f(glGetUniformLocation(anthenaShadder, "color"), 0.8f, 0.8f, 0.8f);
		glBindVertexArray(VAO[14]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		// ANTHENA TIP OUTLINE
		glUseProgram(anthenaOutlineShadder);
		glUniform2f(glGetUniformLocation(anthenaOutlineShadder, "offset"), 0.0f, round(anthenaOffset * 100.0) / 100.0);
		glUniform3f(glGetUniformLocation(anthenaOutlineShadder, "color"), 0.0f, 0.0f, 0.0f);
		glBindVertexArray(VAO[15]);
		glDrawArrays(GL_LINES, 0, 2);
		glDrawArrays(GL_LINES, 1, 2);
		glDrawArrays(GL_LINES, 2, 2);
		glDrawArrays(GL_LINES, 3, 2);

		// SPEAKER
		glUseProgram(speakerShadder);
		glUniform3f(glGetUniformLocation(speakerShadder, "color"), 0.2f, 0.2f, 0.2f);
		glBindVertexArray(VAO[16]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(VAO[28]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		// SPEAKER MEMBRANE BIG
		glUseProgram(speakerMembraneBigShadder);
		glUniform2f(glGetUniformLocation(speakerMembraneBigShadder, "center"), 0.525, -0.375);
		glUniform1f(glGetUniformLocation(speakerMembraneBigShadder, "scale"), speakerMembraneScale);
		glUniform3f(glGetUniformLocation(speakerMembraneBigShadder, "color"), 0.1f, 0.1f, 0.1f);
		glBindVertexArray(VAO[17]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(speakerMembraneBig) / (2 * sizeof(float)));
		glUniform2f(glGetUniformLocation(speakerMembraneBigShadder, "center"), -0.525, -0.375);
		glBindVertexArray(VAO[29]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(speakerMembraneBig) / (2 * sizeof(float)));

		// SPEAKER MEMBRANE SMALL
		glUseProgram(speakerMembraneSmallShadder);
		glUniform3f(glGetUniformLocation(speakerMembraneSmallShadder, "color"), 0.05f, 0.05f, 0.05f);
		glBindVertexArray(VAO[18]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(speakerMembraneSmall) / (2 * sizeof(float)));
		glBindVertexArray(VAO[30]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(speakerMembraneSmall) / (2 * sizeof(float)));

		// SPEAKER MESH
		glUseProgram(speakerMeshShadder);
		glBindVertexArray(VAO[19]);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, meshTexture);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(VAO[31]);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, meshTexture);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindTexture(GL_TEXTURE_2D, 0);

		// SPEAKER OUTLINE
		glUseProgram(speakerOutlineShadder);
		glUniform3f(glGetUniformLocation(speakerOutlineShadder, "color"), 0.0f, 0.0f, 0.0f);
		glBindVertexArray(VAO[20]);
		glDrawArrays(GL_LINES, 0, 2);
		glDrawArrays(GL_LINES, 1, 2);
		glDrawArrays(GL_LINES, 2, 2);
		glDrawArrays(GL_LINES, 3, 2);
		glBindVertexArray(VAO[32]);
		glDrawArrays(GL_LINES, 0, 2);
		glDrawArrays(GL_LINES, 1, 2);
		glDrawArrays(GL_LINES, 2, 2);
		glDrawArrays(GL_LINES, 3, 2);

		// VOLUME BAR
		glUseProgram(volumeBarShadder);
		glUniform3f(glGetUniformLocation(volumeBarShadder, "color"), 0.0f, 0.0f, 0.0f);
		glBindVertexArray(VAO[21]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		// VOLUME BAR INDICATOR
		glUseProgram(volumeBarIndicatorShadder);
		glUniform2f(glGetUniformLocation(volumeBarIndicatorShadder, "offset"), 0.0f, round(volumeBarIndicatorOffset * 100.0) / 100.0);
		glUniform3f(glGetUniformLocation(volumeBarIndicatorShadder, "color"), 0.0f, radioOn ? 1.0f : 0.0f, 0.0f);
		glBindVertexArray(VAO[22]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		// VOLUME BAR HIDER
		glUseProgram(volumeBarHiderShadder);
		glUniform3f(glGetUniformLocation(volumeBarIndicatorShadder, "color"), 0.5f, 0.5f, 0.5f);
		glBindVertexArray(VAO[23]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		// RADIO - OUTLINES
		glUseProgram(radioOutlineShadder);
		glUniform3f(glGetUniformLocation(radioOutlineShadder, "color"), 0.0f, 0.0f, 0.0f);
		glBindVertexArray(VAO[1]);
		glDrawArrays(GL_LINES, 0, 2);
		glDrawArrays(GL_LINES, 1, 2);
		glDrawArrays(GL_LINES, 2, 2);
		glDrawArrays(GL_LINES, 3, 2);
		glDrawArrays(GL_LINES, 5, 2);
		glDrawArrays(GL_LINES, 7, 2);
		glDrawArrays(GL_LINES, 8, 2);
		glDrawArrays(GL_LINES, 9, 2);
		glDrawArrays(GL_LINES, 11, 2);
		glDrawArrays(GL_LINES, 12, 2);
		glDrawArrays(GL_LINES, 13, 2);

		// VOLUME BAR OUTLINES
		glUseProgram(volumeBarOutlineShadder);
		glUniform3f(glGetUniformLocation(volumeBarOutlineShadder, "color"), 0.0f, 0.0f, 0.0f);
		glBindVertexArray(VAO[24]);
		glDrawArrays(GL_LINES, 0, 2);
		glDrawArrays(GL_LINES, 1, 2);
		glDrawArrays(GL_LINES, 2, 2);
		glDrawArrays(GL_LINES, 3, 2);

		// VOLUME RAIL
		glUseProgram(volumeRailShadder);
		glUniform3f(glGetUniformLocation(volumeRailShadder, "color"), 0.0f, 0.0f, 0.0f);
		glBindVertexArray(VAO[25]);
		glLineWidth(5.0f);
		glDrawArrays(GL_LINES, 0, 2);

		// VOLUME SLIDER
		glUseProgram(volumeSliderShadder);
		glUniform2f(glGetUniformLocation(volumeSliderShadder, "offset"), 0.0f, round((volumeBarIndicatorOffset / 0.55) * (0.6125 - 0.1375) * 100) / 100);
		glUniform3f(glGetUniformLocation(volumeSliderShadder, "color"), 0.5f, 0.5f, 0.5f);
		glBindVertexArray(VAO[26]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		// VOLUME SLIDER OUTLINES
		glUseProgram(volumeSliderOutlineShadder);
		glUniform2f(glGetUniformLocation(volumeSliderOutlineShadder, "offset"), 0.0f, round((volumeBarIndicatorOffset / 0.55) * (0.6125 - 0.1375) * 100) / 100);
		glUniform3f(glGetUniformLocation(volumeSliderOutlineShadder, "color"), 0.0f, 0.0f, 0.0f);
		glBindVertexArray(VAO[27]);
		glLineWidth(1.0f);
		glDrawArrays(GL_LINES, 0, 2);
		glDrawArrays(GL_LINES, 1, 2);
		glDrawArrays(GL_LINES, 2, 2);
		glDrawArrays(GL_LINES, 3, 2);

		// AM/FM RAIL
		glUseProgram(AMFMRailShadder);
		glUniform3f(glGetUniformLocation(AMFMRailShadder, "color"), 0.45f, 0.45f, 0.45f);
		glBindVertexArray(VAO[33]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		// VOLUME SLIDER OUTLINES
		glUseProgram(AMFMRailOutlineShadder);
		glUniform3f(glGetUniformLocation(AMFMRailOutlineShadder, "color"), 0.0f, 0.0f, 0.0f);
		glBindVertexArray(VAO[34]);
		glDrawArrays(GL_LINES, 0, 2);
		glDrawArrays(GL_LINES, 1, 2);
		glDrawArrays(GL_LINES, 2, 2);
		glDrawArrays(GL_LINES, 3, 2);

		// AM/FM SWITCH
		glUseProgram(AMFMSwitchShadder);
		glUniform2f(glGetUniformLocation(AMFMSwitchShadder, "offset"), 0.0f, FMon ? 0.0f : 0.15f);
		glUniform3f(glGetUniformLocation(AMFMSwitchShadder, "color"), 0.5f, 0.5f, 0.5f);
		glBindVertexArray(VAO[35]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		// VOLUME SWITCH OUTLINES
		glUseProgram(AMFMSwitchOutlineShadder);
		glUniform2f(glGetUniformLocation(AMFMSwitchShadder, "offset"), 0.0f, FMon ? 0.0f : 0.15f);
		glUniform3f(glGetUniformLocation(AMFMSwitchOutlineShadder, "color"), 0.0f, 0.0f, 0.0f);
		glBindVertexArray(VAO[36]);
		glDrawArrays(GL_LINES, 0, 2);
		glDrawArrays(GL_LINES, 1, 2);
		glDrawArrays(GL_LINES, 2, 2);
		glDrawArrays(GL_LINES, 3, 2);

		// AM
		glUseProgram(AMShadder);
		glBindVertexArray(VAO[37]);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, AMTexture);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindTexture(GL_TEXTURE_2D, 0);

		// FM
		glUseProgram(AMShadder);
		glBindVertexArray(VAO[38]);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, FMTexture);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindTexture(GL_TEXTURE_2D, 0);

		// TABLE
		glUseProgram(tableShadder);
		glBindVertexArray(VAO[39]);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tableTexture);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindTexture(GL_TEXTURE_2D, 0);

		// TABLE OUTLINE
		glUseProgram(tableOutlineShadder);
		glUniform3f(glGetUniformLocation(tableOutlineShadder, "color"), 0.0f, 0.0f, 0.0f);
		glBindVertexArray(VAO[40]);
		glDrawArrays(GL_LINES, 0, 2);

		// SIGNATURE
		glUseProgram(signatureShadder);
		glBindVertexArray(VAO[41]);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, signatureTexture);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindTexture(GL_TEXTURE_2D, 0);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glDeleteBuffers(VAOVBOCount, VBO);
	glDeleteVertexArrays(VAOVBOCount, VAO);
	glDeleteProgram(radioShadder);
	glfwTerminate();
	return 0;
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (action == GLFW_RELEASE) return;
	bool isDown = action == GLFW_PRESS;
	switch (key) {
		case GLFW_KEY_ESCAPE: 
			glfwSetWindowShouldClose(window, GL_TRUE); 
			break;
		case GLFW_KEY_ENTER: 
			radioOn = !radioOn;
			break;
		case GLFW_KEY_UP: 
			anthenaOffset = round((anthenaOffset + (anthenaOffset < 0.0f ? 0.01f : 0.0f)) * 100.0) / 100.0;
			break;
		case GLFW_KEY_DOWN:
			anthenaOffset = round((anthenaOffset - (anthenaOffset > -0.55f ? 0.01f : 0.0f)) * 100.0) / 100.0;
			break;
		case GLFW_KEY_RIGHT:
			AMFMScaleIndicatorOffset = round((AMFMScaleIndicatorOffset + (AMFMScaleIndicatorOffset < 1.4f ? 0.01f : 0.0f)) * 100.0) / 100.0;
			break;
		case GLFW_KEY_LEFT:
			AMFMScaleIndicatorOffset = round((AMFMScaleIndicatorOffset - (AMFMScaleIndicatorOffset > 0.0f ? 0.01f : 0.0f)) * 100.0) / 100.0;
			break;
		case GLFW_KEY_KP_ADD:
			volumeBarIndicatorOffset = round((volumeBarIndicatorOffset + (volumeBarIndicatorOffset < 0.55f ? 0.01f : 0.0f)) * 100.0) / 100.0;
			break;
		case GLFW_KEY_KP_SUBTRACT:
			volumeBarIndicatorOffset = round((volumeBarIndicatorOffset - (volumeBarIndicatorOffset > 0.0f ? 0.01f : 0.0f)) * 100.0) / 100.0;
			break;
		case GLFW_KEY_F:
			FMon = true;
			break;
		case GLFW_KEY_A:
			FMon = false;
			break;
	}
}

unsigned int compileShader(GLenum type, const char* source) {

	std::string content = "";
	std::ifstream file(source);
	std::stringstream ss;
	if (file.is_open())
	{
		ss << file.rdbuf();
		file.close();
		std::cout << "Uspjesno procitao fajl sa putanje \"" << source << "\"!" << std::endl;
	}
	else {
		ss << "";
		std::cout << "Greska pri citanju fajla sa putanje \"" << source << "\"!" << std::endl;
	}
	std::string temp = ss.str();
	const char* sourceCode = temp.c_str();

	int shader = glCreateShader(type);

	int success;
	char infoLog[512];
	glShaderSource(shader, 1, &sourceCode, NULL);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		if (type == GL_VERTEX_SHADER)
			printf("VERTEX");
		else if (type == GL_FRAGMENT_SHADER)
			printf("FRAGMENT");
		printf(" sejder ima gresku! Greska: \n");
		printf(infoLog);
	}
	return shader;
}

unsigned int createShader(const char* vsSource, const char* fsSource) {

	unsigned int program;
	unsigned int vertexShader;
	unsigned int fragmentShader;

	program = glCreateProgram();

	vertexShader = compileShader(GL_VERTEX_SHADER, vsSource); 
	fragmentShader = compileShader(GL_FRAGMENT_SHADER, fsSource);

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	glLinkProgram(program);
	glValidateProgram(program);

	int success;
	char infoLog[512];
	glGetProgramiv(program, GL_VALIDATE_STATUS, &success); 
	if (success == GL_FALSE)
	{
		glGetShaderInfoLog(program, 512, NULL, infoLog);
		std::cout << "Objedinjeni sejder ima gresku! Greska: \n";
		std::cout << infoLog << std::endl;
	}

	glDetachShader(program, vertexShader);
	glDeleteShader(vertexShader);
	glDetachShader(program, fragmentShader);
	glDeleteShader(fragmentShader);

	return program;
}

static unsigned loadImageToTexture(const char* filePath) {
	int TextureWidth;
	int TextureHeight;
	int TextureChannels;
	unsigned char* ImageData = stbi_load(filePath, &TextureWidth, &TextureHeight, &TextureChannels, 0);
	if (ImageData != NULL)
	{
		stbi__vertical_flip(ImageData, TextureWidth, TextureHeight, TextureChannels);

		GLint InternalFormat = -1;
		switch (TextureChannels) {
		case 1: InternalFormat = GL_RED; break;
		case 3: InternalFormat = GL_RGB; break;
		case 4: InternalFormat = GL_RGBA; break;
		default: InternalFormat = GL_RGB; break;
		}

		unsigned int Texture;
		glGenTextures(1, &Texture);
		glBindTexture(GL_TEXTURE_2D, Texture);
		glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, TextureWidth, TextureHeight, 0, InternalFormat, GL_UNSIGNED_BYTE, ImageData);
		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(ImageData);
		return Texture;
	}
	else
	{
		std::cout << "Textura nije ucitana! Putanja texture: " << filePath << std::endl;
		stbi_image_free(ImageData);
		return 0;
	}
}