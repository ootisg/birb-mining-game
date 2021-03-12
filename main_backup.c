#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GL/glut.h>

#include "inputs.h"
  
static const GLfloat g_vertex_buffer_data[] = { 
    -1.0f, -1.0f,
     1.0f, -1.0f,
    -1.0f,  1.0f,
     1.0f,  1.0f
};
static const GLushort g_element_buffer_data[] = { 0, 1, 2, 3 };

static struct {
	GLuint vertex_buffer;
	GLuint element_buffer;
	GLuint vertex_shader;
	GLuint frag_shader;
	GLuint program;
	GLuint textures[2];
	struct {
		GLuint position;
	} attributes;
} g_resources;

static void show_info_log(
    GLuint object,
    PFNGLGETSHADERIVPROC glGet__iv,
    PFNGLGETSHADERINFOLOGPROC glGet__InfoLog
)
{
	//COPY PASTED
    GLint log_length;
    char *log;

    glGet__iv(object, GL_INFO_LOG_LENGTH, &log_length);
    log = malloc(log_length);
    glGet__InfoLog(object, log_length, NULL, log);
    fprintf(stderr, "%s", log);
    free(log);
}

GLchar* file_contents (const char* filename, int* length) {
	//Open file
	FILE* f = fopen (filename, "r");
	
	//Get the file length
	fseek (f, 0, SEEK_END);
	*length = ftell (f);
	fseek (f, 0, SEEK_SET);
	
	GLchar* fcontents = malloc ((*length + 1) * sizeof(GLchar));
	//Make a buffer and read the file to it
	int i;
	for (i = 0; i < *length; i++) {
		int read_char = fgetc (f);
		if (read_char == EOF) {
			*length = i;
			fcontents[i] = 0; //Add a null terminator
			break;
		}
		fcontents[i] = (GLchar)read_char;
	}
	printf ("%d\n%s\n", *length, fcontents);
	
	//Return the buffer ptr
	return fcontents;
}

static GLuint make_buffer (GLenum target, const void* buffer_data, GLsizei buffer_size) {
	GLuint buffer;
	glGenBuffers (1, &buffer);
	glBindBuffer (target, buffer);
	glBufferData (target, buffer_size, buffer_data, GL_STATIC_DRAW);
	return buffer;
}

static GLuint make_shader (GLenum type, const char* filename) {
	
	//Read in the shader
	GLint length;
	GLchar* source = file_contents (filename, &length);
	GLuint shader;
	GLuint shader_ok;
	
	//Terminate if file reading failed
	if (!source) {
		return 0;
	}
	
	//Make the shader
	shader = glCreateShader (type);
	glShaderSource (shader, 1, (const GLchar**)&source, &length);
	free (source);
	glCompileShader (shader);
	
	//Handle any shader related mishaps
	glGetShaderiv (shader, GL_COMPILE_STATUS, &shader_ok);
	if (!shader_ok) {
		//Copy-pasted, don't judge
		fprintf(stderr, "Failed to compile %s:\n", filename);
		show_info_log(shader, glGetShaderiv, glGetShaderInfoLog);
        glDeleteShader(shader);
        return 0;
	}
}

static GLuint make_program (GLuint vertex_shader, GLuint frag_shader) {
	GLint program_ok;
	GLuint program = glCreateProgram ();
	glAttachShader (program, vertex_shader);
	glAttachShader (program, frag_shader);
	glLinkProgram (program);
	//Handle any program related mishaps (copy-pasted, oof)
	glGetProgramiv(program, GL_LINK_STATUS, &program_ok);
    if (!program_ok) {
        fprintf(stderr, "Failed to link shader program:\n");
        show_info_log(program, glGetProgramiv, glGetProgramInfoLog);
        glDeleteProgram(program);
        return 0;
    }
    return program;
}

static int make_resources () {
	g_resources.vertex_buffer = make_buffer (GL_ARRAY_BUFFER, g_vertex_buffer_data, sizeof (g_vertex_buffer_data));
	g_resources.element_buffer = make_buffer (GL_ARRAY_BUFFER, g_element_buffer_data, sizeof (g_element_buffer_data));
	g_resources.vertex_shader = make_shader (GL_VERTEX_SHADER, "hello-gl.v.glsl");
	g_resources.frag_shader = make_shader (GL_FRAGMENT_SHADER, "hello-gl.f.glsl");
	g_resources.program = make_program (g_resources.vertex_shader, g_resources.frag_shader);
	g_resources.attributes.position = glGetAttribLocation (g_resources.program, "position");
	printf ("Attributes handle: %d\n", g_resources.attributes.position);
}

void init () {
	make_resources ();
}

void render () {
	glUseProgram (g_resources.program);
	glBindBuffer(GL_ARRAY_BUFFER, g_resources.vertex_buffer);
    glVertexAttribPointer(
        g_resources.attributes.position,  /* attribute */
        2,                                /* size */
        GL_FLOAT,                         /* type */
        GL_FALSE,                         /* normalized? */
        sizeof(GLfloat)*2,                /* stride */
        (void*)0                          /* array buffer offset */
    );
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_resources.element_buffer);
    glDrawElements(
        GL_TRIANGLE_STRIP,  /* mode */
        4,                  /* count */
        GL_UNSIGNED_SHORT,  /* type */
        (void*)0            /* element array buffer offset */
    );
	glDisableVertexAttribArray(g_resources.attributes.position);
	glutSwapBuffers ();
}

void display () {
	//Do nothing, this is gay
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	render ();
	glutSwapBuffers ();
	int i = 0;
	while (i < 20000000) {
		i++;
	}
	swap_input_buffers ();
	//printf ("frame");
	//glutPostRedisplay ();
}

int main (int argc, char** argv) {
	printf ("glchar %d\n", sizeof(char));
	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow ("Hello World");
	glewInit ();
	printf ("OpenGL %s\n", glGetString (GL_VERSION));
	init_inputs ();
	glutDisplayFunc (render);
	glutKeyboardFunc (key_down_handler);
	glutKeyboardUpFunc (key_up_handler);
	glutMouseFunc (mouse_handler);
	glutMotionFunc (mouse_motion_handler);
	glutPassiveMotionFunc (passive_mouse_motion_handler);
	init ();
	glutMainLoop ();
	printf ("Hello World");
	return 0;
}