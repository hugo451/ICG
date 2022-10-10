#include <cstdlib>
#include <iostream>
#include <GL/glut.h>
#include <math.h>

void init();
void draw_test();

void mouse_test(GLint button, GLint action, GLint x, GLint y);
void mouse_test2(GLint x, GLint y);
void mouse_test3(GLint x, GLint y);

void keybord_test(GLubyte key, GLint x, GLint y);
void keybord_test2(GLint key, GLint x, GLint y);

GLint WINDOW_WIDTH = 500,
      WINDOW_HEIGHT = 500;

// armazena os vértices de um objeto
struct VERTEX
{
  int x;
  int y;
};
// armazena a descrição geométrica de um objeto
struct OBJECT
{
  VERTEX *vertices;
  int nrvertices;
};

int t_x = 0;
int t_y = 0;

int t_x_inc = 0;
int t_y_inc = 0;

float scale_x = 1;
float scale_y = 1;

int angulo = 0;
double s = 0.0; // armazenar seno (da matriz de rotacao)
double c = 1.0;

OBJECT *object; // objeto global que será desenhado

OBJECT *aux; // objeto auxiliar

void ObjetoTransladado(OBJECT **obj, VERTEX ponto);
void ObjetoEscalado(OBJECT **obj, float sx, float sy);
void ObjetoRotacionado(OBJECT **obj, double angulo);

OBJECT *create_object()
{
  OBJECT *obj = (OBJECT *)malloc(sizeof(OBJECT));
  obj->nrvertices = 5;
  obj->vertices = (VERTEX *)malloc(obj->nrvertices * sizeof(VERTEX));
  obj->vertices[0].x = 250;
  obj->vertices[0].y = 130;
  obj->vertices[1].x = 250;
  obj->vertices[1].y = 170;
  obj->vertices[2].x = 220;
  obj->vertices[2].y = 190;
  obj->vertices[3].x = 190;
  obj->vertices[3].y = 170;
  obj->vertices[4].x = 190;
  obj->vertices[4].y = 130;
  return obj;
}

VERTEX calculate_centroid(OBJECT *obj)
{
  int i;
  VERTEX cent;
  cent.x = 0;
  cent.y = 0;
  for (i = 0; i < obj->nrvertices; i++)
  {
    cent.x += obj->vertices[i].x;
    cent.y += obj->vertices[i].y;
  }
  cent.x /= obj->nrvertices;
  cent.y /= obj->nrvertices;
  return cent;
}

void init()
{
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glMatrixMode(GL_PROJECTION);

  gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);
  object = create_object(); // cria o objeto
}

void draw_object(OBJECT *obj)
{
  int i;
  glBegin(GL_POLYGON); // desenha uma linha
  for (i = 0; i < obj->nrvertices; i++)
  {
    glVertex2i(obj->vertices[i].x + t_x_inc, obj->vertices[i].y + t_y_inc);
  }
  glEnd();
}

/* Grupo: André Hugo; Franklin Anthony; Miquéias Luis*/
void draw_test()
{
  glClear(GL_COLOR_BUFFER_BIT); // desenha o fundo (limpa a janela)
  glMatrixMode(GL_MODELVIEW);   // garante que a matrix seja a ModelView
  glLoadIdentity();             // carrega a matrix identidade

  glColor3f(0.0, 0.0, 1.0); // altera o atributo de cor

  VERTEX cent = calculate_centroid(object); // calcula o centrÃ³ide
  VERTEX ponto;
  ponto.x = -cent.x;
  ponto.y = -cent.y;

  aux = object;
  ObjetoTransladado(&aux, ponto);
  ObjetoRotacionado(&aux, angulo);
  ObjetoEscalado(&aux, scale_x, scale_y);
  ObjetoTransladado(&aux, cent);

  draw_object(aux); // desenha o objeto

  glFlush(); // processa as rotinas OpenGL o mais rÃ¡pido possÃ­vel
}

void ObjetoRotacionado(OBJECT **obj, double angulo){
  s = sin( ((angulo)*M_PI/180.0));
  c = cos( ((angulo)*M_PI/180.0));

  OBJECT *objeto = (OBJECT *)malloc(sizeof(OBJECT));
  objeto->nrvertices = (*obj)->nrvertices;
  objeto->vertices = (VERTEX *)malloc(objeto->nrvertices * sizeof(VERTEX));

  double matrix[2][2] = {{c, -s},
                        {s, c}};

  for (int i = 0; i < (*obj)->nrvertices; i++)
  {
    int matrixP[2][1] = {{(*obj)->vertices[i].x},
                         {(*obj)->vertices[i].y}};
    double matrixR[2][1] = {{0},
                           {0}};

    for (int k = 0; k < 2; k++)
    {

      for (int j = 0; j < 2; j++)
      {
        matrixR[k][0] += matrix[k][j] * matrixP[j][0];
      }
    }

    objeto->vertices[i].x = matrixR[0][0];
    objeto->vertices[i].y = matrixR[1][0];
  }

  *obj = objeto;

}

void ObjetoEscalado(OBJECT **obj, float sx, float sy)
{
  OBJECT *objeto = (OBJECT *)malloc(sizeof(OBJECT));
  objeto->nrvertices = (*obj)->nrvertices;
  objeto->vertices = (VERTEX *)malloc(objeto->nrvertices * sizeof(VERTEX));

  float matrix[3][3] = {{sx, 0, 0},
                        {0, sy, 0},
                        {0, 0, 1}};

  for (int i = 0; i < (*obj)->nrvertices; i++)
  {
    int matrixP[3][1] = {{(*obj)->vertices[i].x},
                         {(*obj)->vertices[i].y},
                         {1}};
    float matrixR[2][1] = {{0},
                           {0}};

    for (int k = 0; k < 2; k++)
    {

      for (int j = 0; j < 3; j++)
      {
        matrixR[k][0] += matrix[k][j] * matrixP[j][0];
      }
    }

    objeto->vertices[i].x = matrixR[0][0];
    objeto->vertices[i].y = matrixR[1][0];
  }

  *obj = objeto;
}

void ObjetoTransladado(OBJECT **obj, VERTEX ponto)
{
  OBJECT *objeto = (OBJECT *)malloc(sizeof(OBJECT));
  objeto->nrvertices = (*obj)->nrvertices;
  objeto->vertices = (VERTEX *)malloc(objeto->nrvertices * sizeof(VERTEX));

  int matrix[3][3] = {{1, 0, ponto.x},
                      {0, 1, ponto.y},
                      {0, 0, 1}};

  for (int i = 0; i < (*obj)->nrvertices; i++)
  {
    int matrixP[3][1] = {{(*obj)->vertices[i].x},
                         {(*obj)->vertices[i].y},
                         {1}};
    int matrixR[2][1] = {{0},
                         {0}};

    for (int k = 0; k < 2; k++)
    {

      for (int j = 0; j < 3; j++)
      {
        matrixR[k][0] += matrix[k][j] * matrixP[j][0];
      }
    }

    objeto->vertices[i].x = matrixR[0][0];
    objeto->vertices[i].y = matrixR[1][0];
  }
  *obj = objeto;
}

void mouse_test(GLint button, GLint action, GLint x, GLint y)
{
  switch (button)
  {
  case GLUT_LEFT_BUTTON:
  {
    std::cout << "Esquerda ";
    angulo += 5.0;
    std::cout << angulo;
    break;
  }
  case GLUT_MIDDLE_BUTTON:
  {
    std::cout << "Meio";
    break;
  }
  case GLUT_RIGHT_BUTTON:
  {
    std::cout << "Direita ";
    angulo -=5.0;
    std::cout << angulo;
    break;
  }
  default:
    break;
  }

  if (action == GLUT_DOWN)
    std::cout << " preciona";
  else // GLUT_UP
    std::cout << " libera";

  // x cresce da esquerda pra direita. O y cresce de cima para baixo
  std::cout << " em (x:" << x << ", y:" << y << ")";

  std::cout << "\n";
  glutPostRedisplay();
}

void mouse_test2(GLint x, GLint y)
{
  std::cout << "Movendo mouse sem clicar para posicao (x:" << x << ", y:" << y << ")\n";
}

void mouse_test3(GLint x, GLint y)
{
  std::cout << "Arrastando o mouse para posicao (x:" << x << ", y:" << y << ")\n";
}

// funcao para processar eventos de teclado
void keybord_test(GLubyte key, GLint x, GLint y)
{
  GLint m = glutGetModifiers();
  VERTEX cent = calculate_centroid(object); // calcula o centróide

  switch (key)
  {

  case 119:
    scale_y += 0.1;
    break;

  case 97:
    scale_x -= 0.1;
    break;

  case 115:
    scale_y -= 0.1;
    break;

  case 100:
    scale_x += 0.1;
    break;
  }

  if (m == GLUT_ACTIVE_SHIFT)
    std::cout << "Shift ou Caps ";
  else if (m == GLUT_ACTIVE_CTRL)
    std::cout << "Control ";
  else if (m == GLUT_ACTIVE_ALT)
    std::cout << "Alt ";

  // VERIFICAR TABELA ASCII QUANDO O CTRL ESTIVER PRECIONADO COM ALGUMA
  // LETRA
  if (m == GLUT_ACTIVE_CTRL && (GLint)key == 4)
    exit(EXIT_SUCCESS);

  std::cout << "Tecla: " << (GLint)key << " (x:" << x << ", y:" << y << ")\n";
  printf("\nscale_x = %.2f", scale_x);
  printf("\nscale_y = %.2f", scale_y);

  // ESC = 27
  if (key == 27)
    glutReshapeWindow(WINDOW_WIDTH, WINDOW_HEIGHT);

  glutPostRedisplay();
}

void keybord_test2(GLint key, GLint x, GLint y)
{
  // GLUT_KEY_F1 .. GLUT_KEY_F12
  // GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_LEFT, GLUT_KEY_RIGHT
  // GLUT_KEY_PAGE_DOWN, GLUT_KEY_HOME etc.

  std::cout << "\nTecla especial: " << key << " (x:" << x << ", y:" << y << ")\n";

  if (key == GLUT_KEY_F11)
    glutFullScreen();

  if (key == 100)
  {
    t_x_inc -= 1; // tecla para esquerda
  }
  if (key == 102)
  {
    t_x_inc += 1; // tecla para direita
  }
  if (key == 103)
  {
    t_y_inc -= 1; // tecla para baixo
  }
  if (key == 101)
  {
    t_y_inc += 1; // tecla para cima
  }
  if (key == 112)
  {
    glColor3f(1.0, 1.0, 0.0); // altera o atributo de cor
  }

  printf("\nt_x_inc = %d", t_x_inc);
  printf("\nt_y_inc = %d", t_y_inc);
  glutPostRedisplay();
}

int main(int argc, char *argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

  GLint screen_width = glutGet(GLUT_SCREEN_WIDTH),
        screen_height = glutGet(GLUT_SCREEN_HEIGHT);

  glutInitWindowPosition((screen_width - WINDOW_WIDTH) / 2, (screen_height - WINDOW_WIDTH) / 2);
  glutInitWindowSize(WINDOW_WIDTH, WINDOW_WIDTH);
  glutCreateWindow("OpenGL - Transformacoes");

  init();
  glutDisplayFunc(draw_test);

  glutKeyboardFunc(keybord_test);
  glutMouseFunc(mouse_test);

  glutSpecialFunc(keybord_test2);

  glutMainLoop();

  return EXIT_SUCCESS;
}
