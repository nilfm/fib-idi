#define GLM_FORCE_RADIANS
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "model.h"

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core 
{
  Q_OBJECT

  public:
    MyGLWidget (QWidget *parent=0);
    ~MyGLWidget ();

  protected:
    // initializeGL - Aqui incluim les inicialitzacions del contexte grafic.
    virtual void initializeGL ( );
    // paintGL - Mètode cridat cada cop que cal refrescar la finestra.
    // Tot el que es dibuixa es dibuixa aqui.
    virtual void paintGL ( );
    // resizeGL - És cridat quan canvia la mida del widget
    virtual void resizeGL (int width, int height);
    // keyPressEvent - Es cridat quan es prem una tecla
    virtual void keyPressEvent (QKeyEvent *event);

  private:
    void creaBuffers();
    void carregaShaders();
    void modelTransform(bool terra);
    void projectTransform();
    void viewTransform();
    void iniCamera();
    void calcCenterRadius(glm::vec3& minim, glm::vec3& maxim);

    // attribute locations
    GLuint vertexLoc, colorLoc;
    // uniform locations
    GLuint transLoc;
    GLuint projLoc;
    GLuint viewLoc;
    // VAO i VBO names
    GLuint VAO_Homer, VAO_Terra;
    // Program
    QOpenGLShaderProgram *program;
    // Viewport
    GLint ample, alt;
    // Internal vars
    Model m;
    float scale, angle;
    float fov, ra, znear, zfar;
    glm::vec3 OBS, VRP, VUP;
    glm::vec3 pos;
    glm::vec3 center;
    float radius;
    glm::vec3 min_point, max_point;
};

