
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

  modelTransform();

  // Activem l'Array a pintar i pintem l'escena (VAOs 1 i 2)
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES, 0, 3);
}

void MyGLWidget::resizeGL (int w, int h)
{
  ample = w;
  alt = h;
}

void MyGLWidget::creaBuffers ()
{
  // Objecte
  glm::vec3 Vertices[3];
  Vertices[0] = glm::vec3(-1.0, -1.0, 0.0);
  Vertices[1] = glm::vec3(1.0, -1.0, 0.0);
  Vertices[2] = glm::vec3(0.0, 1.0, 0.0);
  
  // VAO
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  
  // VBO
  GLuint VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);  

  // Atributs
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // Desactivem el VAO
  glBindVertexArray(0);
}

void MyGLWidget::modelTransform() {
	glm::mat4 TG(1.0);
	TG = glm::translate(TG, glm::vec3(right, up, 0.0));
	TG = glm::rotate(TG, angle, glm::vec3(0, 0, 1));
	glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::keyPressEvent(QKeyEvent *e) {
	makeCurrent();
	switch (e->key()) {
		case Qt::Key_S:
			scl += 0.1;
			if (scl > 2) scl = 2;
			glUniform1f(vertexSize, scl);
			break;
		case Qt::Key_D:
			scl -= 0.1;
			if (scl < 0.1) scl = 0.1;
			glUniform1f(vertexSize, scl);
			break;
		case Qt::Key_Z:
			angle += M_PI/30;
			break;
		case Qt::Key_X:
			angle -= M_PI/30;
			break;
		case Qt::Key_Left:
			right -= 0.1;
			if (right < -1) right = -1;
			break;
		case Qt::Key_Right:
			right += 0.1;
			if (right > 1) right = 1;
			break;
		case Qt::Key_Up:
			up += 0.1;
			if (up > 1) up = 1;
			break;
		case Qt::Key_Down:
			up -= 0.1;
			if (up < -1) up = -1;
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
  up = 0.5;
  right = 0;
  scl = 0.5;
  angle = 0;
  glUniform1f(vertexSize, scl);
}
