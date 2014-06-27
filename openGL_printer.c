#include "printer.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GL/glew.h>
#include <GL/freeglut.h>


#define WINDOW_TITLE_PREFIX "OpenEmblem"

int     current_width  = 800,
	current_height = 600,
	window         = 0;

unsigned int frame_count = 0;

GLuint  vert_shader_id,
	frag_shader_id,
	program_id,
	vao_id,
	vbo_id,
	col_buf_id;

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
	"   out_col = ex_col\n"\
	"}\n"\
};

void initialize(int argc, char *argv[]);
void init_window(int argc, char *argv[]);

void resize_func(int width, int heigth);
void render_func(void);
void timer_func(int);
void idle_func(void);

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

	glDrawArrays(GL_TRIANGLES, 0 ,3);

	glutSwapBuffers();

	++frame_count;
}

void idle_func(void)
{
	glutPostRedisplay();
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
	GLfloat vert[] = {
		-0.8f, -0.8f, 0.0f, 1.0f,
		0.0f, 0.8f, 0.0f, 1.0f,
		0.8f, -0.8f, 0.0f, 1.0f
	};

	GLfloat col[] = {
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f
	};

	GLenum error = glGetError();
	
	glGenVertexArrays(1, &vao_id);
	glBindVertexArray(vao_id);

	glGenBuffers(1, &vbo_id);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &col_buf_id);
	glBindBuffer(GL_ARRAY_BUFFER, col_buf_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(col), col, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4 ,GL_FLOAT, GL_FALSE, 0 ,0);
	glEnableVertexAttribArray(1);

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
	glDeleteBuffers(1, &col_buf_id);
	glDeleteBuffers(1, &vbo_id);

	glBindVertexArray(0);
	glDeleteVertexArrays(1, &vao_id);

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

		exit(EXIT_FAILURE);
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
