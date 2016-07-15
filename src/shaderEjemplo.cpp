#include <GL/glew.h> /* include GLEW and new version of GL on Windows */
#include <GLFW/glfw3.h> /* GLFW helper library */
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdio.h>
#include <iostream>

using namespace std;

const GLuint WIDTH = 800, HEIGHT = 600;
GLfloat anguloX = 0.0f;
GLfloat anguloY = 0.0f;
int maxValuePoint = 0.0f;
float treshold = 0.0;

float *imagen;

GLint X = 64;
GLint Y = 64;
GLint Z = 32;
const unsigned int pixels = X * Y * Z;

GLfloat *points;
GLfloat *colors;

// Function prototypes
void leerImagen(char *ruta);
void setPoints(float *image, float *points);
void maxValue(float *p);
void setColor(float *p, float *color);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

int main() {

	GLFWwindow* window = NULL;
	const GLubyte* renderer;
	const GLubyte* version;
	GLuint vao;
	GLuint vboPoints, vboColor;

	points = (GLfloat*) malloc(sizeof(GLfloat) * pixels * 3);
	colors = (GLfloat*) malloc(sizeof(GLfloat) * pixels * 3);
	char ruta[] = "salidaOPETCOO.img";

	leerImagen(ruta);
	maxValue(imagen);
	setPoints(imagen, points);
	setColor(imagen, colors);

	cout << maxValuePoint << endl;

//	for (unsigned int i = 0; i < pixels; i++) {
//		int ind = i * 3;
//		cout << "X: " << color[ind] << " Y: " << color[ind + 1] << " Z: " << color[ind + 2] << endl;
//
//	}

	/* these are the strings of code for the shaders
	 the vertex shader positions each vertex point */
	const char* vertex_shader = " #version 410\n"
			"in vec3 point;"
			"in vec3 color;"
			"out vec3 pass_Color;"
			"uniform mat4 model;"
			"uniform mat4 view;"
			"uniform mat4 projection;"
			"void main () {"
			"	pass_Color = color;"
			"	gl_Position = projection * view * model * vec4 (point, 1.0);"
			"}";
	/* the fragment shader colours each fragment (pixel-sized area of the
	 triangle) */
	const char* fragment_shader = " #version 410\n"
			"in vec3 pass_Color;"
			"out vec4 outColor;"
			"void main () {"
//			"	outColor = vec4 (0.0, 0.0, 255.0, 1.0);"
			"	outColor = vec4 (pass_Color, 1.0);"
			"}";
	/* GL shader objects for vertex and fragment shader [components] */
	GLuint vs, fs;
	/* GL shader programme object [combined, to link] */
	GLuint shader_programme;

	/* start GL context and O/S window using the GLFW helper library */
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return 1;
	}

	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	window = glfwCreateWindow(WIDTH, HEIGHT, "Cubo puntos", NULL, NULL);
	if (!window) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return 1;
	}

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);

	glfwMakeContextCurrent(window);
	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit();

	/* get version info */
	renderer = glGetString(GL_RENDERER); /* get renderer string */
	version = glGetString(GL_VERSION); /* version as a string */
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	// depth-testing interprets a smaller value as "closer"
	glDepthFunc(GL_LESS);

	glGenBuffers(1, &vboPoints);
	glBindBuffer(GL_ARRAY_BUFFER, vboPoints);
	glBufferData(GL_ARRAY_BUFFER, 3 * pixels * sizeof(GLfloat), points, GL_STATIC_DRAW);

	glGenBuffers(1, &vboColor);
	glBindBuffer(GL_ARRAY_BUFFER, vboColor);
	/* here we copy the shader strings into GL shaders, and compile them. we
	 then create an executable shader 'program' and attach both of the compiled
	 shaders. we link this, which matches the outputs of the vertex shader to
	 the inputs of the fragment shader, etc. and it is then ready to use */
	vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, NULL);
	glCompileShader(vs);
	fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader, NULL);
	glCompileShader(fs);
	shader_programme = glCreateProgram();
	glBindAttribLocation(shader_programme, 0, "point");
	glBindAttribLocation(shader_programme, 1, "color");

	glAttachShader(shader_programme, fs);
	glAttachShader(shader_programme, vs);
	glLinkProgram(shader_programme);

	/* this loop clears the drawing surface, then draws the geometry described
	 by the VAO onto the drawing surface. we 'poll events' to see if the window
	 was closed, etc. finally, we 'swap the buffers' which displays our drawing
	 surface onto the view area. we use a double-buffering system which means
	 that we have a 'currently displayed' surface, and 'currently being drawn'
	 surface. hence the 'swap' idea. in a single-buffering system we would see
	 stuff being drawn
	 one-after-the-other */

	while (!glfwWindowShouldClose(window)) {
		cout << "limite: " << treshold << endl;

		/* a vertex buffer object (VBO) is created here. this stores an array of
		 data on the graphics adapter's memory. in our case - the vertex points */
		//coordenadas
		//colores
		glBufferData(GL_ARRAY_BUFFER, 3 * pixels * sizeof(GLfloat), colors, GL_DYNAMIC_DRAW);

		/* the vertex array object (VAO) is a little descriptor that defines which
		 data from vertex buffer objects should be used as input variables to vertex
		 shaders. in our case - use our only VBO, and say 'every three floats is a
		 variable' */

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vboPoints);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, vboColor);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;

		// wipe the drawing surface clear
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shader_programme);

		// Get their uniform location
		GLint modelLoc = glGetUniformLocation(shader_programme, "model");
		GLint viewLoc = glGetUniformLocation(shader_programme, "view");
		GLint projLoc = glGetUniformLocation(shader_programme, "projection");

		model = glm::rotate(model, glm::radians(anguloX), glm::vec3(1.0f, 0.0f, .0f));
		model = glm::rotate(model, glm::radians(anguloY), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-32.0f, -32.0f, -16.0f));

		glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 100.0f);
		glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);

		glm::vec3 cameraUp = glm::vec3(0.0f, 32.0f, 0.0f);

		view = glm::lookAt(cameraPos, cameraTarget, cameraUp);

		projection = glm::perspective(45.0f, (GLfloat) WIDTH / (GLfloat) HEIGHT, 0.1f, 300.0f);

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		// Note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		//posición
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(vao);
		glPointSize(2.0f);
		// draw points 0-3 from the currently bound VAO with current shader
		glDrawArrays(GL_POINTS, 0, pixels);
		// update other events like input handling
		glfwPollEvents();
		// put the stuff we've been drawing onto the display
		glfwSwapBuffers(window);
	}

	// close GL context and any other GLFW resources
	glfwTerminate();
	return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	switch (key) {
	case GLFW_KEY_UP:
		anguloX += -50.0f;
		break;
	case GLFW_KEY_DOWN:
		anguloX += 50.0f;
		break;
	case GLFW_KEY_LEFT:
		anguloY += 50.0f;
		break;
	case GLFW_KEY_RIGHT:
		anguloY += -50.0f;
		break;
	case GLFW_KEY_C:
		anguloX = 0.0f;
		anguloY = 0.0f;
		break;
	case GLFW_KEY_I:
		treshold += 5.0;
		setColor(imagen, colors);
		break;
	case GLFW_KEY_O:
		if (treshold != 0) {
			treshold -= 5.0;
		} else {
			treshold = 0.0;
		}
		setColor(imagen, colors);
		break;
	default:
		break;
	}

}

void leerImagen(char *ruta) {

	FILE *f = fopen(ruta, "r");
	imagen = (float *) malloc(sizeof(float) * pixels);

	if (fread(imagen, sizeof(float), pixels, f) != pixels) {
		cout << "Error al leer el archivo" << endl;
	}

	fclose(f);
}

void setPoints(float *p, float *point) {
	for (unsigned int i = 0; i < pixels; i++) {
		int ind = i * 3;

		point[ind] = (i % X) % X;
		point[ind + 1] = ((i / X) % Y) % Y;
		point[ind + 2] = ((int) floor((double) i / (X * Y))) % Z;

		//cout << "X: " << p[ind] << " Y: " << p[ind + 1] << " Z: " << p[ind + 2] << endl;
	}
}

void setColor(float *image, float *color) {

	int aR = 0;
	int aG = 0;
	int aB = 255;  // RGB for our 1st color (blue in this case).
	int bR = 255;
	int bG = 0;
	int bB = 0;    // RGB for our 2nd color (red in this case).

	for (unsigned int i = 0; i < pixels; i++) {
		int ind = i * 3;

		if (image[i] != 0.0f && image[i] > treshold) {

			float value = (imagen[i] / maxValuePoint);

			color[ind] = (float) (bR - aR) * value + aR;      // Evaluated as -255*value + 255.
			color[ind + 1] = (float) (bG - aG) * value + aG;      // Evaluates as 0.
			color[ind + 2] = (float) (bB - aB) * value + aB;      // Evaluates as 255*value + 0.
		} else {
			color[ind] = 0.0f;      // Evaluated as -255*value + 255.
			color[ind + 1] = 0.0f;      // Evaluates as 0.
			color[ind + 2] = 0.0f;      // Evaluates as 255*value + 0.
		}

	}

}

void maxValue(float *p) {
	int highNum = 0;
	unsigned int m;
	for (m = 0; m < pixels; m++) {
		if (p[m] > highNum) {
			highNum = (int) p[m];
		}
	}
	maxValuePoint = highNum;
}
