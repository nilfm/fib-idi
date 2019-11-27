#include "MyGLWidget.h"

#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent), program(NULL)
{
  setFocusPolicy(Qt::StrongFocus);  // per rebre events de teclat
  scale = 1.0f;
  angle = 0.0f;
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

  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  creaBuffers();
  iniCamera();
  glEnable(GL_DEPTH_TEST);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void MyGLWidget::iniCamera() {
	OBS = glm::vec3(0, 0, 1);
	VRP = glm::vec3(0, 0, 0);
	VUP = glm::vec3(0, 1, 0);
	viewTransform();
	
	fov = float(M_PI)/2.0f;
	ra = 1.0f;
	znear = 0.1f;
	zfar = 10.0f;
	projectTransform();
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

// En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
// useu els paràmetres que considereu (els que hi ha són els de per defecte)
//  glViewport (0, 0, ample, alt);
  
  // Esborrem el frame-buffer
  glClear (GL_COLOR_BUFFER_BIT);

  // Carreguem la transformació de model
  modelTransform(false);
  projectTransform();
  viewTransform();

  // Activem el VAO per a pintar la caseta 
  glBindVertexArray(VAO_Homer);

  // pintem
  glDrawArrays(GL_TRIANGLES, 0, m.faces().size()*3);

  modelTransform(true);

  glBindVertexArray(VAO_Terra);
  glDrawArrays(GL_TRIANGLES, 0, 6);

  glBindVertexArray (0);
}

void MyGLWidget::modelTransform(bool terra) 
{
    // Matriu de transformació de model
    glm::mat4 transform (1.0f);
    if (not terra) transform = glm::rotate(transform, angle, glm::vec3(0, 1, 0));
    transform = glm::scale(transform, glm::vec3(scale));
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::projectTransform() {
	//Arguments: FOV (radians), aspect ratio window, znear, zfar
	glm::mat4 Proj = glm::perspective(fov, ra, znear, zfar);
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::viewTransform() {
	glm::mat4 View = glm::lookAt(OBS, VRP, VUP);
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &View[0][0]);
	std::cout << "(" << OBS.x << ", " << OBS.y << ", " << OBS.z << ")" << std::endl;
}

void MyGLWidget::resizeGL (int w, int h) 
{
  ample = w;
  alt = h;
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_Z: { // escalar a més gran
      scale += 0.05;
      break;
    }
    case Qt::Key_X: { // escalar a més petit
      scale -= 0.05;
      break;
    }
    case Qt::Key_R: {
	  angle += M_PI/4.0f;
	  break;
	}
	case Qt::Key_D: {
	  float old_long = sqrt(OBS.x*OBS.x + OBS.y*OBS.y + OBS.z*OBS.z);
	  glm::vec3 new_obs = OBS - 0.1f*glm::vec3(-OBS.z, 0, OBS.x);
	  float new_long = sqrt(new_obs.x*new_obs.x + new_obs.y*new_obs.y + new_obs.z*new_obs.z);
	  OBS = old_long/new_long*new_obs;
	  break;
	}
	case Qt::Key_A: {
	  float old_long = sqrt(OBS.x*OBS.x + OBS.y*OBS.y + OBS.z*OBS.z);
	  glm::vec3 new_obs = OBS + 0.1f*glm::vec3(-OBS.z, 0, OBS.x);
	  float new_long = sqrt(new_obs.x*new_obs.x + new_obs.y*new_obs.y + new_obs.z*new_obs.z);
	  OBS = old_long/new_long*new_obs;	 
	  break;
	}
	case Qt::Key_W: {
	  OBS /= 1.1;
	  break;
	}
	case Qt::Key_S: {
	  OBS *= 1.1;
	  break;
	}
	case Qt::Key_Up: {
	  OBS.y += 0.1f;
	  break;
	}
	case Qt::Key_Down: {
	  OBS.y -= 0.1f;
	  break;
	}
    default: event->ignore(); break;
  }
  projectTransform();
  viewTransform();
  update();
}

void MyGLWidget::creaBuffers () 
{
  m.load("../../models/HomerProves.obj");

  glGenVertexArrays(1, &VAO_Homer);
  glBindVertexArray(VAO_Homer);

  GLuint VBO_Homer[2];
  glGenBuffers(2, VBO_Homer);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Homer[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m.faces().size()*3*3, m.VBO_vertices(), GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glBindBuffer(GL_ARRAY_BUFFER, VBO_Homer[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m.faces().size()*3*3, m.VBO_matdiff(), GL_STATIC_DRAW);
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc); 

  glm::vec3 posicio[6] = {
	glm::vec3(1.0, -1.0, 1.0),
	glm::vec3(1.0, -1.0, -1.0),
	glm::vec3(-1.0, -1.0, -1.0),
	
	glm::vec3(1.0, -1.0, 1.0),
	glm::vec3(-1.0, -1.0, -1.0),
	glm::vec3(-1.0, -1.0, 1.0)
  }; 
   glm::vec3 color[6] = {
	glm::vec3(1,0,0),
	glm::vec3(0,0,1),
	glm::vec3(0,1,0),
	
	glm::vec3(1,0,0),
	glm::vec3(0,1,0),
	glm::vec3(0,0,1),
  };
  
  glGenVertexArrays(1, &VAO_Terra);
  glBindVertexArray(VAO_Terra);
  
  GLuint VBO_TerraPos;
  glGenBuffers(1, &VBO_TerraPos);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_TerraPos);
  glBufferData(GL_ARRAY_BUFFER, sizeof(posicio), posicio, GL_STATIC_DRAW);
  
  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);
  
  GLuint VBO_TerraCol;
  glGenBuffers(1, &VBO_TerraCol);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_TerraCol);
  glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);

  // Activem l'atribut colorLoc
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);
  glBindVertexArray (0);
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
  vertexLoc = glGetAttribLocation (program->programId(), "vertex");
  // Obtenim identificador per a l'atribut “color” del vertex shader
  colorLoc = glGetAttribLocation (program->programId(), "color");
  // Uniform locations
  transLoc = glGetUniformLocation(program->programId(), "TG");
  projLoc = glGetUniformLocation(program->programId(), "proj");
  viewLoc = glGetUniformLocation(program->programId(), "view");
}

