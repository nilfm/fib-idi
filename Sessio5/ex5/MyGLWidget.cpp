#include "MyGLWidget.h"

#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent), program(NULL)
{
  setFocusPolicy(Qt::StrongFocus);  // per rebre events de teclat
  scale = 1.0f;
  angle = 0.0f;
  perspective = true;
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
  glEnable(GL_DEPTH_TEST);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  min_scene = glm::vec3(-2.5, 0, -2.5);
  max_scene = glm::vec3(2.5, 4.0, 2.5);
  calcCenterRadius();
  calcModelBox();
  iniCamera();
}

void MyGLWidget::iniCamera() {
	dist = radius*2.0;
	
	OBS = glm::vec3(0, 2.0, dist);
	VRP = glm::vec3(center);
	VUP = glm::vec3(0, 1, 0);
	viewTransform();
	
	znear = dist-radius;
	zfar = dist+radius;
	fov = 2.0*asin(radius/dist);
	fov_ini = fov;
	ra = 1.0f;
	projectTransform();
}

void MyGLWidget::paintGL () 
{
// En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
// useu els paràmetres que considereu (els que hi ha són els de per defecte)
//  glViewport (0, 0, ample, alt);
  
  // Esborrem el frame-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Activem el VAO per a pintar la caseta 
  glBindVertexArray(VAO_Model);

  // Carreguem la transformació de model
  modelTransform(false);
  projectTransform();
  viewTransform();

  // pintem
  glDrawArrays(GL_TRIANGLES, 0, m.faces().size()*3);


  glBindVertexArray(VAO_Terra);
  modelTransform(true);
  glDrawArrays(GL_TRIANGLES, 0, 6);

  glBindVertexArray (0);
}

void MyGLWidget::modelTransform(bool terra) 
{
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  if (not terra) {
	  transform = glm::rotate(transform, angle, glm::vec3(0, 1, 0));
	  transform = glm::scale(transform, glm::vec3(scale));
	  transform = glm::scale(transform, glm::vec3(scale2));
	  transform = glm::translate(transform, -patricio_center_base);
  }
  else {
	transform = glm::scale(transform, glm::vec3(scale));
  }
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::projectTransform() {
	//Arguments: FOV (radians), aspect ratio window, znear, zfar
	glm::mat4 Proj(1.0f);
	if (perspective) Proj = glm::perspective(fov, ra, znear, zfar);
	else {
		Proj = glm::ortho(min_scene.x, max_scene.x, min_scene.z, max_scene.z, znear, zfar);
	 }
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::viewTransform() {
	glm::mat4 View = glm::lookAt(OBS, VRP, VUP);
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::calcCenterRadius() {
	center = (min_scene+max_scene)/glm::vec3(2.0);
	radius = distance(min_scene, max_scene)/2.0;
}

void MyGLWidget::calcModelBox() {
  // Càlcul capsa contenidora i valors transformacions inicials
  float minx, miny, minz, maxx, maxy, maxz;
  minx = maxx = m.vertices()[0];
  miny = maxy = m.vertices()[1];
  minz = maxz = m.vertices()[2];
  for (unsigned int i = 3; i < m.vertices().size(); i+=3)
  {
    if (m.vertices()[i+0] < minx)
      minx = m.vertices()[i+0];
    if (m.vertices()[i+0] > maxx)
      maxx = m.vertices()[i+0];
    if (m.vertices()[i+1] < miny)
      miny = m.vertices()[i+1];
    if (m.vertices()[i+1] > maxy)
      maxy = m.vertices()[i+1];
    if (m.vertices()[i+2] < minz)
      minz = m.vertices()[i+2];
    if (m.vertices()[i+2] > maxz)
      maxz = m.vertices()[i+2];
  }
  
  
  std::cout << std::endl << 
  "MIN(x,y,z) = (" << minx << "," << miny << "," << minz << ")" << std::endl <<
  "MAX(x,y,z) = (" << maxx << "," << maxy << "," << maxz << ")" << std::endl;
   scale2 = 4.0/(maxy-miny);
   patricio_center_base = glm::vec3((minx+maxx)/2, miny, (minz+maxz)/2);
 }

void MyGLWidget::resizeGL (int w, int h) 
{
  float rav = float(w)/float(h);
  ra = rav;
  if (rav < 1.0) fov = 2.0*atan(tan(fov_ini/2.0)/rav);
  projectTransform();
  glViewport(0, 0, w, h);
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_S: { // escalar a més gran
      scale += 0.05;
      break;
    }
    case Qt::Key_D: { // escalar a més petit
      scale -= 0.05;
      break;
    }
    case Qt::Key_R: {
	  angle += M_PI/4.0f;
	  break;
	}
    case Qt::Key_O: {
      perspective = !perspective;
	  break;
	}
    default: event->ignore(); break;
  }
  update();
}

void MyGLWidget::creaBuffers () 
{
  m.load("../../models/Patricio.obj");

  glGenVertexArrays(1, &VAO_Model);
  glBindVertexArray(VAO_Model);

  GLuint VBO_Model_Pos;
  glGenBuffers(1, &VBO_Model_Pos);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Model_Pos);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m.faces().size()*3*3, m.VBO_vertices(), GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  GLuint VBO_Model_Col;
  glGenBuffers(1, &VBO_Model_Col);

  glBindBuffer(GL_ARRAY_BUFFER, VBO_Model_Col);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m.faces().size()*3*3, m.VBO_matdiff(), GL_STATIC_DRAW);
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc); 

  glm::vec3 posicio[6] = {
	glm::vec3(2.5, 0, 2.5),
	glm::vec3(2.5, 0, -2.5),
	glm::vec3(-2.5, 0, -2.5),
	
	glm::vec3(2.5, 0, 2.5),
	glm::vec3(-2.5, 0, -2.5),
	glm::vec3(-2.5, 0, 2.5)
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
  
  GLuint VBO_Terra_Pos;
  glGenBuffers(1, &VBO_Terra_Pos);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra_Pos);
  glBufferData(GL_ARRAY_BUFFER, sizeof(posicio), posicio, GL_STATIC_DRAW);
  
  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);
  
  GLuint VBO_Terra_Col;
  glGenBuffers(1, &VBO_Terra_Col);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra_Col);
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

