
//#include <GL/glew.h>
#include "MyGLWidget.h"

#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent), program(NULL)
{
  setFocusPolicy(Qt::StrongFocus);  // per rebre events de teclat
}

MyGLWidget::~MyGLWidget ()
{
  if (program != NULL)
    delete program;
}

void MyGLWidget::initializeGL ()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();
  
  glClearColor (0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  creaBuffers();
}

void MyGLWidget::paintGL ()
{
// Aquest codi és necessari únicament per a MACs amb pantalla retina.
#ifdef __APPLE__
  GLint vp[4];
  glGetIntegerv (GL_VIEWPORT, vp);
  ample = vp[2];
  alt = vp[3];
#endif

  // Esborrem el frame-buffer
  glClear (GL_COLOR_BUFFER_BIT);  

  // Viewport 1
  glViewport (0, 0, ample, alt);

  
  glBindVertexArray(VAO1);
  modelTransform(0);
  glDrawArrays(GL_TRIANGLES, 0, 3);
 
  
  glBindVertexArray(VAO2);
  modelTransform(1);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  
  
}

void MyGLWidget::resizeGL (int w, int h)
{
  ample = w;
  alt = h;
}

void MyGLWidget::creaBuffers ()
{
  // Objecte 1
  glm::vec3 Vertices[3];
  Vertices[0] = glm::vec3(-1.0, -1.0, 0.0);
  Vertices[1] = glm::vec3(1.0, -1.0, 0.0);
  Vertices[2] = glm::vec3(0.0, 1.0, 0.0);
  
  // VAO1
  glGenVertexArrays(1, &VAO1);
  glBindVertexArray(VAO1);
  
  // VBO1
  GLuint VBO1;
  glGenBuffers(1, &VBO1);
  glBindBuffer(GL_ARRAY_BUFFER, VBO1);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);  

  // Atributs1
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);
  
  // Objecte 2
  glm::vec3 Vertices2[6];
  Vertices2[0] = glm::vec3(-0.5, -0.5, 0.0);
  Vertices2[1] = glm::vec3(0.5, -0.5, 0.0);
  Vertices2[2] = glm::vec3(-0.5, 0.5, 0.0);
  
  Vertices2[3] = glm::vec3(-0.5, 0.5, 0.0);
  Vertices2[4] = glm::vec3(0.5, -0.5, 0.0);
  Vertices2[5] = glm::vec3(0.5, 0.5, 0.0);
  
  // VAO2
  glGenVertexArrays(1, &VAO2);
  glBindVertexArray(VAO2);
  
  // VBO2
  GLuint VBO2;
  glGenBuffers(1, &VBO2);
  glBindBuffer(GL_ARRAY_BUFFER, VBO2);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices2), Vertices2, GL_STATIC_DRAW);  

  // Atributs2
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // Desactivem el VAO
  glBindVertexArray(0);
}

void MyGLWidget::modelTransform(int i) {
	glm::mat4 TG(1.0);
	TG = glm::scale(TG, glm::vec3(scl[i], scl[i], 1));
	TG = glm::rotate(TG, angle[i], glm::vec3(0, 0, 1));
	TG = glm::translate(TG, glm::vec3(right[i], up[i], 0.0));
	glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::keyPressEvent(QKeyEvent *e) {
	makeCurrent();
	switch (e->key()) {
		//Object 1
		case Qt::Key_E:
			scl[0] += 0.1;
			if (scl[0] > 2) scl[0] = 2;
			glUniform1f(vertexSize, scl[0]);
			break;
		case Qt::Key_R:
			scl[0] -= 0.1;
			if (scl[0] < 0.1) scl[0] = 0.1;
			glUniform1f(vertexSize, scl[0]);
			break;
		case Qt::Key_A:
			right[0] -= 0.1;
			if (right[0] < -2) right[0] = -2;
			break;
		case Qt::Key_D:
			right[0] += 0.1;
			if (right[0] > 2) right[0] = 2;
			break;
		case Qt::Key_W:
			up[0] += 0.1;
			if (up[0] > 2) up[0] = 2;
			break;
		case Qt::Key_S:
			up[0] -= 0.1;
			if (up[0] < -2) up[0] = -2;
			break;
			
		//Object 2
		case Qt::Key_Y:
			scl[1] += 0.1;
			if (scl[1] > 2) scl[1] = 2;
			glUniform1f(vertexSize, scl[1]);
			break;
		case Qt::Key_U:
			scl[1] -= 0.1;
			if (scl[1] < 0.1) scl[1] = 0.1;
			glUniform1f(vertexSize, scl[1]);
			break;
		case Qt::Key_J:
			right[1] -= 0.1;
			if (right[1] < -2) right[1] = -2;
			break;
		case Qt::Key_L:
			right[1] += 0.1;
			if (right[1] > 2) right[1] = 2;
			break;
		case Qt::Key_I:
			up[1] += 0.1;
			if (up[1] > 2) up[1] = 2;
			break;
		case Qt::Key_K:
			up[1] -= 0.1;
			if (up[1] < -2) up[1] = -2;
			break;
		
		// Both	
		case Qt::Key_P:
			angle[0] += M_PI/30;
			angle[1] -= M_PI/30;
			break;
		default:
			e->ignore();
	}
	update();
}

void MyGLWidget::carregaShaders()
{
  // Creem els shaders per al fragment shader i el vertex shader
  QOpenGLShader fs (QOpenGLShader::Fragment, this);
  QOpenGLShader vs (QOpenGLShader::Vertex, this);
  // Carreguem el codi dels fitxers i els compilem
  fs.compileSourceFile("shaders/basicShader.frag");
  vs.compileSourceFile("shaders/basicShader.vert");
  // Creem el program
  program = new QOpenGLShaderProgram(this);
  // Li afegim els shaders corresponents
  program->addShader(&fs);
  program->addShader(&vs);
  // Linkem el program
  program->link();
  // Indiquem que aquest és el program que volem usar
  program->bind();

  // Obtenim identificador per a l'atribut “vertex” del vertex shader
  vertexLoc = glGetAttribLocation(program->programId(), "vertex");
  vertexSize = glGetUniformLocation(program->programId(), "val");
  transLoc = glGetUniformLocation(program->programId(), "TG");
  up[0] = up[1] = 0.5;
  right[0] = right[1] = 0;
  scl[0] = scl[1] = 0.5;
  angle[0] = angle[1] = 0;
  glUniform1f(vertexSize, scl[0]);
}
