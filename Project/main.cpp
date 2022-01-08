#include <iostream>
#include <stdio.h>
#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGl/glu.h>
#include <GLUT/glut.h>
#elif __linux
#include <GL/glut.h>
#endif

#include <math.h>
#include <time.h>

// Definir cores
#define BLUE 0.0, 0.0, 1.0, 1.0
#define RED 1.0, 0.0, 0.0, 1.0
#define YELLOW 1.0, 1.0, 0.0, 1.0
#define GREEN 0.0, 1.0, 0.0, 1.0
#define WHITE 1.0, 1.0, 1.0, 1.0
#define BLACK 0.0, 0.0, 0.0, 1.0
#define PI 3.14159

#include "RgbImage.h"
#include "materiais.h"

#define frand()            ((float)rand()/RAND_MAX)
#define MAX_PARTICULAS  50000

//---------------------------------------- Particle attributes
typedef struct {
    float   size;        // tamanho
    float    life;        // vida
    float    fade;        // fade
    float    r, g, b;    // color
    GLfloat x, y, z;    // posicao
    GLfloat vx, vy, vz; // velocidade
    GLfloat ax, ay, az; // aceleracao
    int isSplash;
} Particle;

Particle  particula1[MAX_PARTICULAS];

GLint    milisec = 25;

// janela (pixeis)
GLint wScreen=1920, hScreen=1080;

GLboolean frenteVisivel=1;
int count = 0;

GLfloat  xC=750.0, yC=750.0, zC=750.0;        // Mundo  (unidades mundo)
GLfloat  rVisao=1000, aVisao=0.5*PI, incVisao=0.05;
GLfloat  angZoom=90;
GLfloat  incZoom=3;
GLfloat  obsP[] ={rVisao*cos(aVisao), 3.0, rVisao*sin(aVisao)};
GLfloat yTrans = 0.0;
GLfloat zTrans = 0.0;

GLuint poligono[4];

//------------------------------------------------------------ Skybox
GLUquadricObj*  sky  = gluNewQuadric ( );
GLfloat         sizeSky  = 1200;

//texturas
GLuint   texture[5];
RgbImage imag;

//------------------------------------------------------------ Iluminacao Ambiente
//GLint ligaLuz = 0;
GLfloat intensidade=1.0;
GLfloat luzGlobalCorAmb[4]={intensidade,intensidade,intensidade,1.0};   //


//…………………………………………………………………………………………………………………………………………… Lanterna (local) - FOCO
GLint   ligaFoco=0;
GLfloat rFoco=1.1, aFoco=aVisao;
GLfloat incH =0.0, incV=0.0;
GLfloat incMaxH =0.5, incMaxV=0.35;   
GLfloat focoPini[]= { obsP[0], obsP[1], obsP[2], 1.0 };
GLfloat focoDir[] = { 0-obsP[0], 0-obsP[1], 0-obsP[2]};
GLfloat focoExp   = 10.0;
GLfloat focoCut   = 40.0;

GLfloat focoCorDif[4] ={ 0.9, 0.9, 0.9, 1.0}; 
GLfloat focoCorEsp[4] ={ 1.0, 1.0, 1.0, 1.0};

//-------------------------------------------------------------------------------

int flagDirection = 0;
struct tm *current_time;
GLint   msecDelay=30;
GLint second, mlsecond;

static GLfloat vertices[]={
-600.000000, 0.000000, 128.000000,
    600.000000, 0.000000, 128.000000,
    600.000000, 0.000000, 0.000000,
    -600.000000, 0.000000, 0.000000,
    
    -400.000000, 0.000000, 96.000000,
    400.000000, 0.000000, 96.000000,
    400.000000, 16.000000, 96.000000,
    -400.000000, 16.000000, 96.000000,
    
    -400.000000, 16.000000, 128.000000,
    400.000000, 16.000000, 128.000000,
    400.000000, 16.000000, 96.000000,
    -400.000000, 16.000000, 96.000000,
    
    -400.000000, 16.000000, 128.000000, 400.000000, 16.000000, 128.000000, 400.000000, 32.000000, 128.000000, -400.000000, 32.000000, 128.000000, -400.000000, 32.000000, 160.000000, 400.000000, 32.000000, 160.000000, 400.000000, 32.000000, 128.000000, -400.000000, 32.000000, 128.000000, -400.000000, 32.000000, 160.000000, 400.000000, 32.000000, 160.000000, 400.000000, 48.000000, 160.000000, -400.000000, 48.000000, 160.000000, -400.000000, 48.000000, 192.000000, 400.000000, 48.000000, 192.000000, 400.000000, 48.000000, 160.000000, -400.000000, 48.000000, 160.000000, -400.000000, 48.000000, 192.000000, 400.000000, 48.000000, 192.000000, 400.000000, 64.000000, 192.000000, -400.000000, 64.000000, 192.000000, -400.000000, 64.000000, 224.000000, 400.000000, 64.000000, 224.000000, 400.000000, 64.000000, 192.000000, -400.000000, 64.000000, 192.000000, -400.000000, 64.000000, 224.000000, 400.000000, 64.000000, 224.000000, 400.000000, 80.000000, 224.000000, -400.000000, 80.000000, 224.000000, -400.000000, 80.000000, 256.000000, 400.000000, 80.000000, 256.000000, 400.000000, 80.000000, 224.000000, -400.000000, 80.000000, 224.000000, -400.000000, 80.000000, 256.000000, 400.000000, 80.000000, 256.000000, 400.000000, 96.000000, 256.000000, -400.000000, 96.000000, 256.000000, -400.000000, 96.000000, 288.000000, 400.000000, 96.000000, 288.000000, 400.000000, 96.000000, 256.000000, -400.000000, 96.000000, 256.000000, -400.000000, 96.000000, 288.000000, 400.000000, 96.000000, 288.000000, 400.000000, 112.000000, 288.000000, -400.000000, 112.000000, 288.000000, -400.000000, 112.000000, 320.000000, 400.000000, 112.000000, 320.000000, 400.000000, 112.000000, 288.000000, -400.000000, 112.000000, 288.000000, -400.000000, 112.000000, 320.000000, 400.000000, 112.000000, 320.000000, 400.000000, 128.000000, 320.000000, -400.000000, 128.000000, 320.000000,
    -600.000000, 128.000000, 416.000000,
    600.000000, 128.000000, 416.000000,
    600.000000, 128.000000, 320.000000,
    -600.000000, 128.000000, 320.000000
};

static GLfloat normais[] = {
0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000
};

static GLfloat arrayTexture[]={ 
0,0, 1,0, 1,1, 0,1, 0,0, 1,0, 1,1, 0,1, 0,0, 1,0, 1,1, 0,1, 0,0, 1,0, 1,1, 0,1, 0,0, 1,0, 1,1, 0,1, 0,0, 1,0, 1,1, 0,1, 0,0, 1,0, 1,1, 0,1, 0,0, 1,0, 1,1, 0,1,  0,0, 1,0, 1,1, 0,1, 0,0, 1,0, 1,1, 0,1, 0,0, 1,0, 1,1, 0,1, 0,0, 1,0, 1,1, 0,1, 0,0, 1,0, 1,1, 0,1
};

static GLfloat cores[]={
    1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000,
    1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000,
    1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000,
    1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000,
    1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000,
    1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000,
    1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000,
    1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000,
    1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000,
    1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000,
    1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000,
    1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000,
    1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000,
    1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000,
    1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000,
    1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000,
    1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000,
};

void Timer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(msecDelay,Timer, 1);
}

void initLights(void){
    //…………………………………………………………………………………………………………………………………………… Ambiente
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzGlobalCorAmb);
    //…………………………………………………………………………………………………………………………………………… Foco
    glLightfv(GL_LIGHT1, GL_POSITION,      focoPini );
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION,focoDir );
    glLightf (GL_LIGHT1, GL_SPOT_EXPONENT ,focoExp);
    glLightf (GL_LIGHT1, GL_SPOT_CUTOFF,   focoCut);
    glLightfv(GL_LIGHT1, GL_DIFFUSE,       focoCorDif );   
    glLightfv(GL_LIGHT1, GL_SPECULAR,      focoCorEsp  );
    
    if(intensidade > 0)
        glEnable(GL_LIGHT0);
    else
        glDisable(GL_LIGHT0);
    
    if (ligaFoco)
       glEnable(GL_LIGHT1);
    else
        glDisable(GL_LIGHT1);
}

void initTexture(){
//----------------------------------------- Chao
    glGenTextures(1, &texture[0]);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    imag.LoadBmpFile("vidro.bmp");
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, 3, 
        imag.GetNumCols(),
        imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
        imag.ImageData()); 

//----------------------------------------- SKY
    glGenTextures(1, &texture[1]);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    imag.LoadBmpFile("skyBox.bmp");
    glTexImage2D(GL_TEXTURE_2D, 0, 3, 
        imag.GetNumCols(),
        imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
        imag.ImageData());  

//-------------------------------------------- Plataforma

    glGenTextures(1, &texture[2]);
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    imag.LoadBmpFile("escalator-steps.bmp");
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, 3, 
        imag.GetNumCols(),
        imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
        imag.ImageData()); 

//----------------------------- Escadas

    glGenTextures(1, &texture[3]);
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    imag.LoadBmpFile("escalator-texture.bmp");
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, 3, 
        imag.GetNumCols(),
        imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
        imag.ImageData()); 

//------------------------------ Agua

    glGenTextures(1, &texture[4]);
    for (int i=0;i<1;i++)
    {
        imag.LoadBmpFile("agua.bmp");
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexImage2D(GL_TEXTURE_2D, 0, 3,
                     imag.GetNumCols(),
                     imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                     imag.ImageData());
    }
}

void initParticula(Particle *particula, int i) {
    GLfloat v, theta, phi;
    v     = 1*frand()+0.02;
    theta = 2.0*frand()*M_PI;   // [0..2pi]
    phi   = frand()*M_PI;        // [0.. pi]
    if(particula[i].isSplash == 0) {
        GLfloat px, py, pz;
        GLfloat ps;
        
        px = -120.0;
        py = 200.0;
        pz = -200.0;
        ps = 2.5;
        
        particula[i].size = ps ;        // tamanh de cada particula
        particula[i].x      = px + 10.0*frand()*px;    // [-200 200]
        particula[i].y      = py + 0.1*frand()*py;    // [-200 200]
        particula[i].z      = pz + 0.1*frand()*pz;    // [-200 200]
        
        particula[i].vx = 0;    // esferico
        particula[i].vy = -0.1;
        particula[i].vz = 0;
        particula[i].ax = 0.0f;
        particula[i].ay = -0.1f;
        particula[i].az = 0.0f;
        
        particula[i].r = 0.0f;
        particula[i].g = 0.0f;
        particula[i].b = 1.0f;
        particula[i].life = 1.0f * frand();
        particula[i].fade = 0.01f;    // Em 100=1/0.01 iteracoes desaparece
        particula[i].isSplash = 1;
    } else {
        particula[i].size   = particula[i].size ;        // tamanh de cada particula
        particula[i].x      = particula[i].x ;    // [-200 200]
        particula[i].y      = particula[i].y ;    // [-200 200]
        particula[i].z      = particula[i].z ;    // [-200 200]
        
        particula[i].vx = v * cos(theta) * sin(phi);    // esferico
        particula[i].vy = v * cos(phi);
        particula[i].vz = v * sin(theta) * sin(phi);
        particula[i].ax = 0.01f;
        particula[i].ay = -0.01f;
        particula[i].az = 0.015f;
        
        particula[i].r = 0.0f;
        particula[i].g = 0.0f;
        particula[i].b = 1.0f;
        particula[i].life = 3.0f * frand();
        particula[i].fade = 0.1f;    // Em 100=1/0.01 iteracoes desaparece
        particula[i].isSplash = 0;
    }
}

void showParticulas(Particle *particula, int sistema) {
    int i;
    glPushMatrix();
    glScalef(3,1,1);
    glRotatef (90, 0, -1, 0);
    glTranslatef(700,300,-20);
    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    for (i=0; i<MAX_PARTICULAS; i++) {
        glBlendFunc(GL_SRC_ALPHA,GL_ONE);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glBindTexture(GL_TEXTURE_2D,texture[4]);
        glColor4f(1,1,1, particula[i].life);
        glBegin(GL_QUADS);
        glTexCoord2d(0,0); glVertex3f(particula[i].x -particula[i].size, particula[i].y -particula[i].size, particula[i].z);
        glTexCoord2d(1,0); glVertex3f(particula[i].x +particula[i].size, particula[i].y -particula[i].size, particula[i].z);
        glTexCoord2d(1,1); glVertex3f(particula[i].x +particula[i].size, particula[i].y +particula[i].size, particula[i].z);
        glTexCoord2d(0,1); glVertex3f(particula[i].x -particula[i].size, particula[i].y +particula[i].size, particula[i].z);
        glEnd();
        
        particula[i].x += particula[i].vx;
        particula[i].y += particula[i].vy;
        particula[i].z += particula[i].vz;
        particula[i].vx += particula[i].ax;
        particula[i].vy += particula[i].ay;
        particula[i].vz += particula[i].az;
        particula[i].life -= particula[i].fade;
    }
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
    glPopMatrix();

}

void initMaterials(int material) {
    switch (material) {
        case 0:
            glMaterialfv(GL_FRONT,GL_AMBIENT,  whitePlasticAmb  );
            glMaterialfv(GL_FRONT,GL_DIFFUSE,  whitePlasticDif );
            glMaterialfv(GL_FRONT,GL_SPECULAR, whitePlasticSpec);
            glMaterialf (GL_FRONT,GL_SHININESS,whitePlasticCoef);
            break;
        case 1:
            glMaterialfv(GL_FRONT,GL_AMBIENT,  brassAmb  );
            glMaterialfv(GL_FRONT,GL_DIFFUSE,  brassDif );
            glMaterialfv(GL_FRONT,GL_SPECULAR, brassSpec);
            glMaterialf (GL_FRONT,GL_SHININESS,brassCoef); 
            break;  
        case 2:
            glMaterialfv(GL_FRONT,GL_AMBIENT,  turquoiseAmb  );
            glMaterialfv(GL_FRONT,GL_DIFFUSE,  turquoiseDif );
            glMaterialfv(GL_FRONT,GL_SPECULAR, turquoiseSpec);
            glMaterialf (GL_FRONT,GL_SHININESS,turquoiseCoef);
            break;   
        case 3:
            glMaterialfv(GL_FRONT,GL_AMBIENT,  yellowRubberAmb  );
            glMaterialfv(GL_FRONT,GL_DIFFUSE,  yellowRubberDif );
            glMaterialfv(GL_FRONT,GL_SPECULAR, yellowRubberSpec);
            glMaterialf (GL_FRONT,GL_SHININESS,yellowRubberCoef);    
            break;      
        case 4:
            glMaterialfv(GL_FRONT,GL_AMBIENT,  rubyAmb  );
            glMaterialfv(GL_FRONT,GL_DIFFUSE,  rubyDif );
            glMaterialfv(GL_FRONT,GL_SPECULAR, rubySpec);
            glMaterialf (GL_FRONT,GL_SHININESS,rubyCoef);
            break;
        case 5:
            glMaterialfv(GL_FRONT,GL_AMBIENT,  whitePlasticAmb  );
            glMaterialfv(GL_FRONT,GL_DIFFUSE,  whitePlasticDif );
            glMaterialfv(GL_FRONT,GL_SPECULAR, whitePlasticSpec);
            glMaterialf (GL_FRONT,GL_SHININESS,whitePlasticCoef);

            
        default:
            break;
    }
}

void updateVisao(){
    obsP[0]=rVisao*cos(aVisao);
    obsP[2]=rVisao*sin(aVisao);
    
    focoPini[0] = obsP[0];
    focoPini[2] = obsP[2];

    focoDir[0] =0-obsP[0];
    focoDir[1] =0-obsP[1];
    focoDir[2] =0-obsP[2];
    
    glutPostRedisplay();        
}

//============================================================ SkyBox
void drawSkySphere(){
    
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D,texture[1]);
    initMaterials(0);
    glPushMatrix();     
        glRotatef (       90, -1, 0, 0);
        gluQuadricDrawStyle ( sky, GLU_FILL   );
        gluQuadricNormals   ( sky, GLU_SMOOTH );
        gluQuadricTexture   ( sky, GL_TRUE    );
        gluSphere ( sky, sizeSky*1, 150, 150);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);           
};

//============================================================ chao

void drawChao(){

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Chao y=0
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D,texture[0]);
    initMaterials(0);
    glPushMatrix();
        glTranslatef(0,-5,0);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f,0.0f);     glVertex3i( -xC,  0, -xC ); 
            glTexCoord2f(1.0f, 0.0f);    glVertex3i( -xC,   0,  xC ); 
            glTexCoord2f(1.0f, 1.0f);    glVertex3i(  xC,   0,  xC); 
            glTexCoord2f(0.0f, 1.0f);    glVertex3i(  xC,     0,  -xC); 
        glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}


void inicializa(void)
{
    glClearColor(BLACK);        //………………………………………………………………………………Apagar
    glEnable(GL_DEPTH_TEST);    //………………………………………………………………………………Profundidade
    glShadeModel(GL_SMOOTH);    //………………………………………………………………………………Interpolacao de cores
    glEnable(GL_NORMALIZE);
    
    //…………………………………………………………………………………………………………………………… ILUMINACAO
    glEnable(GL_LIGHTING);  
    if(intensidade > 0) glEnable(GL_LIGHT0 );
    else glDisable(GL_LIGHT0);

    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);    // dos dois lados
    //textura
    initTexture();

    //particulas
    int i = 0;
    for(i = 0; i < MAX_PARTICULAS; i++) {
        //particula1[i].isSplash = 1;
        initParticula(particula1, i);
    }   
    
    //------------------------------- BIND ESCADA
    glVertexPointer(3, GL_FLOAT, 0, vertices); 
    glEnableClientState(GL_VERTEX_ARRAY);
    glNormalPointer(GL_FLOAT, 0, normais);
    glEnableClientState(GL_NORMAL_ARRAY);
    glColorPointer(3, GL_FLOAT, 0, cores);
    glEnableClientState(GL_COLOR_ARRAY);            
    glTexCoordPointer(2, GL_FLOAT, 0, arrayTexture); 
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);    
}

void drawEixos()
{
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Eixo X
    glColor4f(RED);
    glBegin(GL_LINES);
    glVertex3i( 0, 0, 0);
    glVertex3i(750, 0, 0);
    glEnd();
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Eixo Y
    glColor4f(GREEN);
    glBegin(GL_LINES);
    glVertex3i(0,  0, 0);
    glVertex3i(0, 750, 0);
    glEnd();
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Eixo Z
    glColor4f(BLUE);
    glBegin(GL_LINES);
    glVertex3i( 0, 0, 0);
    glVertex3i( 0, 0,750);
    glEnd();
    
}

void desenhaQuadrado()
{  
    glPushMatrix();
    glScalef (1.0, 1.0, 1.0);
    //glColor3f(0, 1, 1);
    glRotatef(90,1,0,0);
    glTranslatef(0,400,0);
    glBegin(GL_QUADS);                    
        glVertex2f(-250, -1150);
        glVertex2f(-250, 350);
        glVertex2f(250, 350);
        glVertex2f(250, -1150);
    glEnd();
    glPopMatrix();
}

void desenhaParede(){
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D,texture[0]);
    glPushMatrix();
    initMaterials(2);
    glScalef (1.0, 1.0, 1.0);
    //glColor3f(0, 1, 1);
    glRotatef(90,0,1,0);
    glTranslatef(-405,175,730);
    glBegin(GL_QUADS);                    
        glVertex2f(-180, -200);
        glVertex2f(-180, 350);
        glVertex2f(+1030, 350);
        glVertex2f(+1030, -200);
        glEnd();
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void drawEscada(){

    glEnable(GL_TEXTURE_2D);
  
    glScalef(0.4, 1.5, 2);
    initMaterials(count);
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D,texture[2]);
    //1a Plataforma
    poligono[0] = 0;
    poligono[1] = 1;
    poligono[2] = 2;
    poligono[3] = 3;
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, poligono);

    //2a Plataforma
    poligono[0] = 16*4+0;
    poligono[1] = 16*4+1;
    poligono[2] = 16*4+2;
    poligono[3] = 16*4+3;
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, poligono);
    glPopMatrix();
    
    
    initMaterials(count);
    glPushMatrix();
    //Ultimo degrau
    poligono[0] = 15*4+0;
    poligono[1] = 15*4+1;
    poligono[2] = 15*4+2;
    poligono[3] = 15*4+3;
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, poligono);

    //primeiro degrau
    poligono[0] = 1*4+0;
    poligono[1] = 1*4+1;
    poligono[2] = 1*4+2;
    poligono[3] = 1*4+3;
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, poligono);

    glBindTexture(GL_TEXTURE_2D,texture[3]);
    //plataforma auxiliar
    poligono[0] = 2*4+0;
    poligono[1] = 2*4+1;
    poligono[2] = 2*4+2;
    poligono[3] = 2*4+3;
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, poligono);
  
    //ESCADA
    glTranslatef(0,yTrans,zTrans);
    for(int p = 1; p < 15; p++) {
        glBindTexture(GL_TEXTURE_2D,texture[3]);
        poligono[0] = p*4+0;
        poligono[1] = p*4+1;
        poligono[2] = p*4+2;
        poligono[3] = p*4+3;

        glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, poligono);
    }
    
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

}

void display(void){
    
    // Apaga ecran/profundidade
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    //Janela
    glViewport (0, 0, wScreen, hScreen);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(angZoom, (float)wScreen/hScreen, 0.1, 3*zC);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    //observador
    gluLookAt(obsP[0], obsP[1], obsP[2], 0,0,0, 0, 1, 0);

    //drawEixos();

    time_t timer = time(0);
    current_time = localtime(&timer);
    second = current_time->tm_sec;

    //Movimento da escada
    if(flagDirection == 0) {
        yTrans+=0.2;
        zTrans+=0.5;
        if(zTrans > 32) {
            yTrans = 0;
            zTrans = 0;
        }
    } else {
        yTrans-=0.2;
        zTrans-=0.5;
        if(zTrans < 0) {
            yTrans = 12.8000;
            zTrans = 32.0000;
        }
    }
    //luz
    initLights();
    //textura
    drawChao();
    //sky
    drawSkySphere();
    //cascata
    showParticulas(particula1, 1);
    desenhaParede();
    ///////////////1-DESENHAR FORMA NO STENCIL BUFFER 
    glColorMask(0, 0, 0, 0);
    glDisable(GL_DEPTH_TEST); // Não escreve no colorBuf
    // nao considear profundidade
    //................................................................................................
    glEnable(GL_STENCIL_TEST);
    glClear(GL_STENCIL_BUFFER_BIT);
    glStencilMask(0xFF); // Usar StencilBuf
    // Garantir que está a zeros
    // Define valores a escrever 1=(0xFF)
    //....................................................................................................
    glStencilFunc(GL_ALWAYS,1 ,1);
    // passa sempre teste
    glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE); // substitui por ref=1
    desenhaQuadrado();
    /////////////2-DESENHAR ESCADA NA ZONA LIMITADA AO STENCIL BUFFER
    glColorMask( 1, 1, 1, 1);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);
    // Escrever colorBuffer
    // Enable Zbuffer
    // Garantir apaga zbuffer
    //----------------------------------------------
    glStencilFunc(GL_EQUAL, 1 , 1);
    // Desenha so na zona do StencilBuff
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    // Stencil fica igual !
    glPushMatrix();
    //::: Transformação geométrica que implement o mirror
    glScalef(1,-1,1);
    drawEscada();
    glPopMatrix();
    /////////////3-DESENHAR ESCADA    
    glDisable(GL_STENCIL_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);
    drawEscada();
    ////////4-BLENDING
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //Actualizacao
    glutSwapBuffers();
}




//======================================================= EVENTOS
void keyboard(unsigned char key, int x, int y){
    switch (key) {
        case 'l':
        case 'L':
            intensidade = intensidade + 1.0;
            if(intensidade > 1.1) {
                intensidade = 0;
            }
           // printf("%f\n", intensidade);
            
            luzGlobalCorAmb[0] = intensidade;
            luzGlobalCorAmb[1] = intensidade;
            luzGlobalCorAmb[2] = intensidade;
            glutPostRedisplay();
            //--------------------------- Observador
        case 'f':
        case 'F':
            ligaFoco=!ligaFoco;
            glutPostRedisplay();    
        break;
            
        //Esquerda
        case 'A':
        case 'a':
            flagDirection=0;
            //yTrans +=1;
            //zTrans +=1;
            glutPostRedisplay();
            break;
            
        //Baixo
        case 'S':
        case 's':
            angZoom += 2;
            glutPostRedisplay();
            break;
        
        //Cima
        case 'w':
        case 'W':
            angZoom -= 2;
            glutPostRedisplay();
            break;
        
        //Direita
        case 'd':
        case 'D':
            flagDirection=1;
            //yTrans -=1;
            //zTrans -=1;
            glutPostRedisplay();
            break;

        case 'm':
        case 'M':
            if(count >= 4){
                count = -1;
            }
            count++;
            initMaterials(count);
            break;
        // ESC
        case 27:
            exit(0);
            break;
    }
    
}

void Timer2(int value) {
    int i;
    for(i = 0; i < MAX_PARTICULAS; i++) {
        if(particula1[i].life < 0.0f) {
            initParticula(particula1, i);
        }
    }
    glutPostRedisplay();
    glutTimerFunc(milisec ,Timer2, 1);   //.. Espera msecDelay milisegundos
}



void teclasNotAscii(int key, int x, int y){
    
    if(key == GLUT_KEY_UP)
        obsP[1] = (obsP[1]+ 50) ;
    if(key == GLUT_KEY_DOWN)
        obsP[1] = (obsP[1]- 50) ;
    
    if (obsP[1]>yC)
        obsP[1]=yC;
    if (obsP[1]<-yC)
        obsP[1]=-yC;
    
    if(key == GLUT_KEY_LEFT)
        aVisao = (aVisao + 0.1) ;
    if(key == GLUT_KEY_RIGHT)
        aVisao = (aVisao - 0.1) ;
    
    updateVisao();
    
}

int main(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
    glutInitWindowSize (wScreen, hScreen);
    glutInitWindowPosition (300, 300);
    glutCreateWindow ("Projecto CG|Setas: Observador|WS: Zoom|AD: DireccaoEscadas|F: Foco|L: Ambiente|M: Material");

    
    inicializa();
    
    glutSpecialFunc(teclasNotAscii);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    //glutTimerFunc(msecDelay, Timer, 1);
    glutTimerFunc(milisec , Timer2, 1);     //callback
    glutMainLoop();
    
    return 0;
}
