#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

GLuint idFundo;

GLuint carregaTextura(const char* arquivo) { //colocar fundo tela
    GLuint idTextura = SOIL_load_OGL_texture(
      arquivo,
      SOIL_LOAD_AUTO,
      SOIL_CREATE_NEW_ID,
      SOIL_FLAG_INVERT_Y);

    if (idTextura == 0) {
        printf("Erro do SOIL: '%s'\n", SOIL_last_result());
    }

    return idTextura;
}

//Variáveis globais
GLint controleTecla = 1; //1 é quando está pressionado e 0 quando solta 
GLfloat incrementar = 0.0003;
GLint pressionado = 1;
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
  //Obstaculo.x=500;
  //Obstaculo.y=100;
  //Obstaculo.largura = 20;
  //Obstaculo.altura = 30;
}

void moverBaseDiscoX(){ //para fazer a base do disco andar de um lado para o outro

  if((BaseDisco.x + BaseDisco.largura) >= 100){ //se chegar no final do canto direito 
    incrementar = -0.0003; //decrementa o x quando estiver indo para a esquerda
  }
  if(BaseDisco.x <= 0){ //se chegar no final do canto esquerdo
    incrementar = 0.0003; //incrementa o x quando estiver indo para a direita
  }
  BaseDisco.x = BaseDisco.x + incrementar;
}

void moverBaseDiscoY(){ //para fazer a base do disco ir para baixo/cima

  if((BaseDisco.y + BaseDisco.altura) >= 100){ //se chegar na parte de cima
    incrementar = -0.0003; //decrementa o y quando estiver indo para a esquerda
  }
  if(BaseDisco.y <= 0){ //se chegar no final do canto esquerdo
    incrementar = 0.0003; //incrementa o x quando estiver indo para a direita
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

void desenhaDisco() { //para desenhar disco voador
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f (1, 1, 1);

    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);

    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idFundo);
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

void desenhaCena(void){

  //desenhaDisco(); //VER ONDE ESSA FUNÇÃO VAI FICAR P DESENHAR FUNDO TELA
  // Limpa a tela (com a cor definida por glClearColor(r,g,b)) para desenhar
  glClear(GL_COLOR_BUFFER_BIT);

  // Começa a usar a cor índigo
  //glClearColor(0.29, 0, 0.51); 
  // Desenha um quadrado para ficar no fundo 
  //desenhaObjeto(FundoTela.x, FundoTela.y, FundoTela.altura, FundoTela.largura);

  //glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(1, 0, 1); //rosa
  // Desenha a base do disco voador de ficará movimentando de um lado para o outro
  desenhaObjeto(BaseDisco.x, BaseDisco.y, BaseDisco.altura, BaseDisco.largura);

  // Diz ao OpenGL para colocar o que desenhamos na tela
  glFlush();

  /*while(1){ //vai ficar andando de um lado pro outro infinitamente
    glTranslatef(-5, 0, 0);
    if()
    glutTimerFunc(atualizaCena);
  }*/
}

// Inicia algumas variáveis de estado
void inicializa(void){

  // cor para limpar a tela
  glClearColor(1, 1, 1, 1); // branco
  // habilita mesclagem de cores, para termos suporte a texturas com transparência
  glEnable(GL_BLEND );
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  idFundo = carregaTextura("fundo.jpg");
}

void desenha() { //para desenhar disco voador
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f (1, 1, 1);

    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);

    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idFundo);
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

  /*if((w/h)>=1){ //GABRIEL
    glViewport(0, 0, h, h);
  }else{
    glViewport(0, 0, w, w);
  }*/
  //It just tells which part of the window will be used for rendering(visible)
  glViewport(0, 0, largura, altura); 
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, 100, 0, 100, -1, 1);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void atualizaCena(){ // altera algo na cena
  
  moverBaseDiscoY();
  // atualiza a tela (desenha() será invocada novamente)
  glutPostRedisplay(); 
  // registra a callback novamente
  glutTimerFunc(33, atualizaCena, 0); //Podemos registrar uma callback para ser invocada daí a x ms.
  //por que 33? 1000/33 = 30fps
}

// Callback de evento de teclado
void teclado(unsigned char key, int x, int y){ 

  switch(key){
    case 27: // Tecla ESC
      exit(0);
      break;
  /*case 'a': //seta para baixo
      if(setaBaixo == 1){
        abduz(setaBaixo);
        setaBaixo = 0; 
      }else{
        glutPostRedisplay(); //atualiza a tela (desenha() será invocada novamente)
        setaBaixo = 1; 
    }*/
    default:
       break;
  }
}

void teclado2(int key, int x, int y){ 

  pressionado = 0;
  if(key == GLUT_KEY_LEFT){ //seta para esquerda faz o objeto ir para esquerda
    pressionado = -1;
    BaseDisco.x--; //faz mover para a esquerda
    atualizaCena(); 
  }
  if(key == GLUT_KEY_RIGHT){ //seta para direita faz o objeto ir para direita
      pressionado=1;
      BaseDisco.x++; //faz mover para a direira
      atualizaCena(); 
  }
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
  glutIdleFunc(atualizaCena);
  glutKeyboardFunc(teclado);
  glutSpecialFunc(teclado2);

  inicializa();

  // Entra em loop e nunca sai
  glutMainLoop();

  return 0;
}