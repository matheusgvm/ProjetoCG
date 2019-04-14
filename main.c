#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

GLuint fundo = 0;

void carregaTextura(int *textura, char *arquivo) { //colocar fundo tela
      *textura = SOIL_load_OGL_texture(
      arquivo,
      SOIL_LOAD_AUTO,
      SOIL_CREATE_NEW_ID,
      SOIL_FLAG_INVERT_Y);

    if (textura == 0) {
        printf("Erro do SOIL: '%s'\n", SOIL_last_result());
    }
}

//Variáveis globais
GLint controleTecla[256]; //1 é quando está pressionado e 0 quando solta 
GLfloat incrementar = 0.3;
/*GLint meta = ;
GLint score = ;
GLint tempoLevel = ; //cada fase terá 60 segundos
GLint blink = ;*/

typedef struct{ //composição base de alguns objetos do jogo
  float x; //coordenada x
  float y; //coordenada y
  float altura; 
  float largura; 
}estrutura; 

estrutura BaseDisco; //(40,80,0),(60,80,0),(60,70,0),(40,70,0)
estrutura FundoTela; //(0,0,0),(100,0,0),(100,100,0),(0,100,0)
estrutura Obstaculo; //gerar obstáculos aleatórios

void parametrosIniciais(){//define parâmetros iniciais do jogo

  BaseDisco.x = 40;
  BaseDisco.y = 80;
  BaseDisco.largura = 5;
  BaseDisco.altura = 5;
  FundoTela.x = 0;
  FundoTela.y = 0;
  FundoTela.altura = 100;
  FundoTela.largura = 100;
  Obstaculo.x = 20;
  Obstaculo.y = 40;
  Obstaculo.largura = 3;
  Obstaculo.altura = 3;
}

void moverBaseDiscoX(){ //para fazer a base do disco andar de um lado para o outro

  if(controleTecla['a']==1 || controleTecla['A']==1){ //vai para a esquerda 
      BaseDisco.x = -1;
      //se chegar no final do canto esquerdo
      if(BaseDisco.x <= 0){ 
        //não vai executar o comando que o usuário está pedindo
        glTranslatef(BaseDisco.x, 0, 0);
      }
      glFlush();
  }

  if(controleTecla['d'] || controleTecla['D']==1){ //vai para a direita 
      BaseDisco.x = 1;
      //se chegar no final do canto direito
      if((BaseDisco.x + BaseDisco.largura) >= 100){ 
        //não vai executar o comando que o usuário está pedindo
        glTranslatef(-BaseDisco.x, 0, 0);
      }
      glFlush();
  }

}

void moverBaseDiscoY(){ //para fazer a base do disco ir para baixo/cima

  if((BaseDisco.y + BaseDisco.altura) >= 100){ //se chegar na parte de cima
    incrementar = -0.3; //decrementa o y quando estiver indo para a esquerda
  }
  if(BaseDisco.y <= 0){ //se chegar no final do canto esquerdo
    incrementar = 0.3; //incrementa o x quando estiver indo para a direita
  }
  BaseDisco.y = BaseDisco.y + incrementar;
}

void desenhaObjeto(float x, float y, float altura, float largura){ 

  glBegin(GL_POLYGON); //composição da maioria os objetos do jogo 
    glVertex3f(x, y, 0);
    glVertex3f(x+largura, y, 0);
    glVertex3f(x+largura, y+altura, 0);
    glVertex3f(x, y+altura, 0);
  glEnd();
}

void desenhaFundo() { //textura fundo
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f (1, 1, 1);

    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);

    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, fundo);
    glBegin(GL_TRIANGLE_FAN);
        // Associamos um canto da textura para cada vértice
        // Para preencher a tela toda pensamos no tamanho da janela, coordenadas fundo
        // e nos quadrantes para colocar as coordenadas positivas e negativas
        glTexCoord2f(0, 0); glVertex3f(0, 0,  0); 
        glTexCoord2f(1, 0); glVertex3f(100, 0,  0);
        glTexCoord2f(1, 1); glVertex3f(100,  100,  0);
        glTexCoord2f(0, 1); glVertex3f(0,  100,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    glutSwapBuffers(); // Diz ao OpenGL para colocar o que desenhamos na tela
}

void gerarObstaculos(){ //gera obstáculos aleatórios

  
}

void desenhaCena(void){
   
   // Limpa a tela (com a cor definida por glClearColor(r,g,b)) para desenhar
   glClear(GL_COLOR_BUFFER_BIT);

   desenhaFundo();

  //glClearColor(255,218,185); //PeachPuff
  //Desenha um quadrado para ficar no fundo 
  //desenhaObjeto(FundoTela.x, FundoTela.y, FundoTela.altura, FundoTela.largura);

  // Desenha a base do disco voador de ficará movimentando de um lado para o outro
  glColor3f(0.29, 0, 0.51); //índigo
  desenhaObjeto(BaseDisco.x, BaseDisco.y, BaseDisco.altura, BaseDisco.largura);

  //Obstáculo
  glColor3f(0, 206, 209); //DarkTurquoise
  desenhaObjeto(Obstaculo.x, Obstaculo.y, Obstaculo.altura, Obstaculo.largura);
  
  glFlush(); 

}

// Inicia algumas variáveis de estado
void inicializa(void){

  // cor para limpar a tela
  glClearColor(1, 1, 1, 1); // branco
  // habilita mesclagem de cores, para termos suporte a texturas com transparência
  glEnable(GL_BLEND );
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  carregaTextura(&fundo, "space.png");
}

void desenha() { //para desenhar disco voador
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f (1, 1, 1);

    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);

    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, fundo);
    glBegin(GL_TRIANGLE_FAN);
        // Associamos um canto da textura para cada vértice
        // Para preencher a tela toda pensamos no tamanho da janela, coordenadas fundo
        // e nos quadrantes para colocar as coordenadas positivas e negativas
        glTexCoord2f(0, 0); glVertex3f(-640, -640,  0); 
        glTexCoord2f(1, 0); glVertex3f( 640, -640,  0);
        glTexCoord2f(1, 1); glVertex3f( 640,  640,  0);
        glTexCoord2f(0, 1); glVertex3f(-640,  640,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    glutSwapBuffers(); // Diz ao OpenGL para colocar o que desenhamos na tela
}

// Callback de redimensionamento
void redimensiona(int largura, int altura){

  //It just tells which part of the window will be used for rendering(visible)
  glViewport(0, 0, largura, altura); 
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, 100, 0, 100, -1, 1);
  //glOrtho(-largura/2, largura/2, -altura/2, altura/2, -1, 1); 
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void atualizaCena(){ // altera algo na cena
  
  desenhaCena();
  moverBaseDiscoY();
  moverBaseDiscoX();
  // atualiza a tela (desenha() será invocada novamente)
  glutPostRedisplay(); 
  // registra a callback novamente
  glutTimerFunc(33, atualizaCena, 0); //Podemos registrar uma callback para ser invocada daí a x ms.
  //por que 33? 1000/33 = 30fps
}

// Callback de evento de teclado
void teclado(unsigned char key, int x, int y){ //quando pressiona uma das teclas

  controleTecla[key] = 1;

  switch(key){
    case 27: // Tecla ESC
      exit(0);
      break;  
  
    default:
       break;
  }
}

void soltaTecla(unsigned char key, int x, int y){ //quando solta uma das teclas

  controleTecla[key] = 0;
  
}

// Rotina principal
int main(int argc, char **argv){

  parametrosIniciais();
  // Acordando o GLUT
  glutInit(&argc, argv);

  // Definindo a versão do OpenGL que vamos usar
  glutInitContextVersion(1, 1);
  glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

  // Configuração inicial da janela do GLUT
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(640, 640);
  glutInitWindowPosition(100, 100);

  // Abre a janela
  glutCreateWindow("jogo");

  // Registra callbacks para alguns eventos
  //glutDisplayFunc(desenha); //textura fundo
  glutDisplayFunc(desenhaCena); //chama outras funções para compor a cena principal
  glutReshapeFunc(redimensiona);
  glutTimerFunc(0, atualizaCena, 0);
  glutKeyboardFunc(teclado);
  glutKeyboardUpFunc(soltaTecla);

  inicializa();

  // Entra em loop e nunca sai
  glutMainLoop();

  return 0;
}