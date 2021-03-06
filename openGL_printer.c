#include "printer.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#define WINDOW_TITLE_PREFIX "OpenEmblem"

typedef struct
{
	float coord[4];
	float col[4];
} vertex;


int     current_width  = 600,
	current_height = 600,
	window         = 0;

unsigned int frame_count = 0;

GLuint  vert_shader_id = 0,
	frag_shader_id = 0,
	program_id = 0,
	vao_id = 0,// vertex array
	vbo_id = 0,// vertex buffer
	ibo_id[2] = { 0 },// index buffer
	active_ibo = 0;

const GLchar *vertex_shader = 
{
	"#version 400\n"\

	"layout(location=0) in vec4 in_pos;\n"\
	"layout(location=1) in vec4 in_col;\n"\
	"out vec4 ex_col;\n"\

	"void main(void)\n"\
	"{\n"\
	"   gl_Position = in_pos;\n"\
	"   ex_col = in_col;\n"\
	"}\n"
};

const GLchar* fragment_shader = 
{
	"#version 400\n"\

	"in vec4 ex_col;\n"\
	"out vec4 out_col;\n"\

	"void main(void)\n"\
	"{\n"\
	"   out_col = ex_col;\n"\
	"}\n"\
};

void initialize(int argc, char *argv[]);
void init_window(int argc, char *argv[]);

void resize_func(int width, int heigth);
void render_func(void);
void timer_func(int);
void idle_func(void);
void keyboard_func(unsigned char key, int x, int y);

void cleanup(void);
void create_vbo(void);
void destroy_vbo(void);
void create_shaders(void);
void destroy_shaders(void);

int main(int argc, char *argv[])
{
	initialize(argc, argv);

	glutMainLoop();

	exit(EXIT_SUCCESS);
}

void initialize(int argc, char *argv[])
{
	GLenum glew_init_result;

	init_window(argc, argv);

	glewExperimental = GL_TRUE;
	glew_init_result = glewInit();

	if (GLEW_OK != glew_init_result) {
		fprintf(
			stderr,
			"ERROR: %s\n",
			glewGetErrorString(glew_init_result)
			);
		exit(EXIT_FAILURE);
	}

	fprintf(
		stdout,
		"INFO: OpenGL Version %s\n",
		glGetString(GL_VERSION)
		);

	create_shaders();
	create_vbo();

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void init_window(int argc, char *argv[])
{
	glutInit(&argc, argv);

	glutInitContextVersion(4, 0);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutSetOption(
		GLUT_ACTION_ON_WINDOW_CLOSE,
		GLUT_ACTION_GLUTMAINLOOP_RETURNS
		);

	glutInitWindowSize(current_width, current_height);

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

	window = glutCreateWindow(WINDOW_TITLE_PREFIX);

	if (window < 1) {
		fprintf(
			stderr,
			"ERROR: Could not create a new rendering window.\n"
			);

		exit(EXIT_FAILURE);
	}

	glutReshapeFunc(resize_func);
	glutDisplayFunc(render_func);
	glutIdleFunc(idle_func);
	glutTimerFunc(0, timer_func, 0);
	glutCloseFunc(cleanup);
	glutKeyboardFunc(keyboard_func);
}

void resize_func(int w, int h)
{
	current_width = w;
	current_height = h;

	glViewport(0,0,current_width, current_height);
}

void render_func(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (active_ibo == 0) {
		glDrawElements(GL_TRIANGLES, 48, GL_UNSIGNED_BYTE,  (GLvoid *) 0);
	} else {
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE,  (GLvoid *) 0);
	}

	glutSwapBuffers();

	++frame_count;
}

void idle_func(void)
{
	glutPostRedisplay();
}

void keyboard_func(unsigned char key, int x, int y)
{
	switch(key)
	{
	case 'T' :
	case 't' :
	{
		active_ibo = (active_ibo+1) % 2;
		//printf("%d\n", active_ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_id[active_ibo]);
		break;
	}
	default :
		break;
	}
}

void timer_func(int value)
{
	if (0 != value) {
		char *tmp = malloc(512 + strlen(WINDOW_TITLE_PREFIX));
		sprintf(
			tmp,
			"%s: %d frames per second @ %d x %d",
			WINDOW_TITLE_PREFIX,
			frame_count*4,
			current_width,
			current_height
			);
		glutSetWindowTitle(tmp);
		free(tmp);
	}

	frame_count = 0;
	glutTimerFunc(250, timer_func, 1);
}

void cleanup(void)
{
	destroy_shaders();
	destroy_vbo();
}

void create_vbo(void)
{
	vertex vertices[] =
	{
		{ { 0.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
// Top
		{ { -0.2f, 0.8f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
		{ { 0.2f, 0.8f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
		{ { 0.0f, 0.8f, 0.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f } },
		{ { 0.0f, 1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
// Bottom
		{ { -0.2f, -0.8f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
		{ { 0.2f, -0.8f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
		{ { 0.0f, -0.8f, 0.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f } },
		{ { 0.0f, -1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
// Left
		{ { -0.8f, -0.2f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
		{ { -0.8f, 0.2f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
		{ { -0.8f, 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f } },
		{ { -1.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
// Right
		{ { 0.8f, -0.2f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
		{ { 0.8f, 0.2f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
		{ { 0.8f, 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f } },
		{ { 1.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } }
	};					

	GLubyte indices[] = {
// Top
		0, 1, 3,
		0, 3, 2,
		3, 1, 4,
		3, 4, 2,
// Bottom
		0, 5, 7,
		0, 7, 6,
		7, 5, 8,
		7, 8, 6,
// Left
		0, 9, 11,
		0, 11, 10,
		11, 9, 12,
		11, 12, 10,
// Right
		0, 13, 15,
		0, 15, 14,
		15, 13, 16,
		15, 16, 14
	};

	GLubyte alt_indices[] = {
// Outer square border:
		3, 4, 16,
		3, 15, 16,
		15, 16, 8,
		15, 7, 8,
		7, 8, 12,
		7, 11, 12,
		11, 12, 4,
		11, 3, 4,
 
// Inner square
		0, 11, 3,
		0, 3, 15,
		0, 15, 7,
		0, 7, 11
	};

	GLenum error = glGetError();

	const size_t buffer_size = sizeof(vertices);
	const size_t vertex_size = sizeof(vertices[0]);
	const size_t col_offset  = sizeof(vertices[0].coord);

	glGenBuffers(1, &vbo_id);

	glGenVertexArrays(1, &vao_id);
	glBindVertexArray(vao_id);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, vertex_size, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, vertex_size, (GLvoid *) col_offset);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glGenBuffers(2, ibo_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_id[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_id[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(alt_indices), alt_indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_id[0]);

	error = glGetError();
	if (GL_NO_ERROR != error) {
		fprintf(
			stderr,
			"ERROR: Could not create a vbo: %s \n",
			gluErrorString(error)
			);

		exit(EXIT_FAILURE);
	}
}

void destroy_vbo(void)
{
	GLenum error = glGetError();

	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDeleteBuffers(1, &vbo_id);
	
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &vao_id);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteBuffers(2, ibo_id);

	error = glGetError();
	if (GL_NO_ERROR != error) {
		fprintf(
			stderr,
			"ERROR: Could not destroy the vbo: %s \n",
			gluErrorString(error)
			);

		exit(EXIT_FAILURE);
	}
}

void create_shaders(void)
{
	GLenum error = glGetError();

	vert_shader_id = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert_shader_id, 1, &vertex_shader, NULL);
	glCompileShader(vert_shader_id);

	frag_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag_shader_id, 1, &fragment_shader, NULL);
	glCompileShader(frag_shader_id);

	program_id = glCreateProgram();
	glAttachShader(program_id, vert_shader_id);
	glAttachShader(program_id, frag_shader_id);
	glLinkProgram(program_id);
	glUseProgram(program_id);

	error = glGetError();
	if (GL_NO_ERROR != error) {
		fprintf(
			stderr,
			"ERROR: Could not create the shaders: %s \n",
			gluErrorString(error)
			);

		exit(-1);
	}
}

void destroy_shaders(void)
{
	GLenum error = glGetError();

	error = glGetError();
	if (GL_NO_ERROR != error) {
		fprintf(
			stderr,
			"ERROR: Could not destroy the shaders: %s \n",
			gluErrorString(error)
			);
		exit(EXIT_FAILURE);
	}
}
