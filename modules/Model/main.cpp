/***************************************************/
/*                                                 */
/* RODRIGO LUIS DE SOUZA DA SILVA    22/12/2005    */
/*                                                 */
/*	   				Object Viewer.cpp                */
/*                                                 */
/***************************************************/
/*
Le e mostra arquivos .obj utilizando o os arquivos glm*.*
Objetos retirados do Tutorial do Nate Robins
*/

#include <windows.h>
#include <stdio.h>
#include <ctype.h>

#include <GL/freeglut.h>
#include <avr/model/Model.hpp>

using namespace avr;

// Globals
SPtr<Model> pmodel = NULL;

char objectList[7][50] = {"../data/obj/soccerball.obj",
								  "../data/obj/dolphins.obj",
								  "../data/obj/f-16.obj",
								  "../data/obj/flowers.obj",
                          "../data/obj/porsche.obj",
                          "../data/obj/rose+vase.obj",
                          "../data/obj/al.obj"};
char objectName[100] = {"../data/obj/soccerball.obj"};

//float dist = 8.0;
float dist = 20.0;
int width = 600;
int height = 600;
float scale = 1;
//float luzX = 0.7f, luzY = 0.75f, luzZ = 1.5f;
float luzX = 0.7f, luzY = 0.75f, luzZ = 8.5f;
GLfloat light0_position[] = {luzX, luzY, luzZ, 1.0f};
float rotationX = 0.0, rotationY = 0.0;
int   last_x, last_y;

// Menu
void showMenu()
{
   printf("\nOBJECT VIEWER\n=============\n\n");
   printf("Mouse\n");
   printf("* Mouse movimenta o objeto.\n");
   printf("Teclado\n");
   printf("* Direcionais + PgUp/PgDown movimentam a luz no plano (start: [0, 0, 5])\n");
   printf("* + e - para efeitos de zoom\n");
   printf("* 1 a 7 muda o modelo\n");
   printf("* ESC - Para sair\n");
}

//Reshape Func
void reshape( int w, int h)
{
	width = w;
	height = h;
	glViewport(0,0,w,h);
}

void keyboard(unsigned char key, int x, int y)
{
   switch (key)
   {
      case 27 :
         exit(0);
      break;
      case '+':
         dist-=0.1;
      break;
      case '-' :
         dist+=0.2;
      break;
      case 's':
         scale-=0.5;
      break;
      case 'S' :
         scale+=0.5;
      break;

      // temporary
   }

   if(isdigit(key))
   {
		int model = int(key) - 48;
		if(model > 0 && model <= 7)
		{
	      pmodel = NULL;
   	   strcpy(objectName, objectList[model-1]);
	   }
   }
   glutPostRedisplay();
}

// Motion callback
void motion(int x, int y )
{
  rotationY += (float) (y - last_y);
  rotationX += (float) (x - last_x);

  last_x = x;
  last_y = y;

  glutPostRedisplay();
}

// Mouse callback
void mouse(int button, int state, int x, int y)
{
  if ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN ) {
    last_x = x;
    last_y = y;
  }
}

void special(int key, int x, int y)
{
	float step = 0.05f;
   switch (key)
   {
      case GLUT_KEY_LEFT:
         luzX-=step;
      break;
      case GLUT_KEY_RIGHT:
         luzX+=step;
      break;
      case GLUT_KEY_UP:
         luzY+=step;
      break;
      case GLUT_KEY_DOWN:
         luzY-=step;
      break;
      case GLUT_KEY_PAGE_UP:
         luzZ+=step;
      break;
      case GLUT_KEY_PAGE_DOWN:
         luzZ-=step;
      break;
   }
   light0_position[0] = luzX;
   light0_position[1] = luzY;
   light0_position[2] = luzZ;
   //printf("\nPosição da luz [x,y,z]: [%.2f, %.2f, %.2f]",light0_position[0], light0_position[1], light0_position[2]);
   glutPostRedisplay();
}

void display(void)
{
   // Limpar todos os pixels
  	glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

   // Sistema de Projeção
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(40.0f,(GLfloat)width/(GLfloat)height,0.1f,200.0f);
   gluLookAt (0.0, 0.0, dist, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

   // Sistema de Visualização
   glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

   // Movimenta o objeto e camera fica parada
   glRotatef( rotationY, 1.0, 0.0, 0.0 );
   glRotatef( rotationX, 0.0, 1.0, 0.0 );

	// Código de leitura do Objeto (.obj)
	if (!pmodel) {
		pmodel = FactoryModel::OBJ(objectName);
		if(!pmodel) exit(0);
		pmodel->ComputeFacetNormals();
		pmodel->ComputeVertexNormals(Deg(90));
		pmodel->Scale(5.0f);
	}

	// Desenha posição da luz
	glPushMatrix();
		glLoadIdentity();
		//glDisable(GL_LIGHTING);
		glTranslatef(light0_position[0], light0_position[1], light0_position[2]);
		glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
		glColor3f(1.0, 1.0, 1.0);
		glutWireSphere(0.02, 10, 10);
	glPopMatrix();

	pmodel->Draw();

 	glutSwapBuffers ();
   glutPostRedisplay();
}


void init (void)
{
   // selecionar cor de fundo (preto)
   glClearColor (0.2, 0.2, 0.2, 0.0);

   glEnable(GL_LIGHT0);                   // habilita luz 0
	glEnable(GL_COLOR_MATERIAL);           // Utiliza cor do objeto como material
	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);

   glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

	glEnable(GL_LIGHTING);                 // Habilita luz
	glEnable(GL_DEPTH_TEST);               // Habilita Z-buffer
	glEnable(GL_CULL_FACE);                // Habilita Backface-Culling

	glEnable(GL_LIGHT0);
   glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
   glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION,0.0f);
   glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0f);
   glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION , 1.0f);
}


int main(int argc, char** argv)
{
   showMenu();
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE|GLUT_DEPTH|GLUT_RGB);
   glutInitWindowSize (width, height);
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("Object Viewer");
   init ();
   glutMouseFunc( mouse );
   glutKeyboardFunc(keyboard);
   glutSpecialFunc(special);
   glutMotionFunc( motion );
	glutReshapeFunc( reshape );
   glutDisplayFunc(display);
   glutMainLoop();

   return 0;
}
