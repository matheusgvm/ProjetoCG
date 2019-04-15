#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

//Texturas
GLuint fundo = 0;
GLuint meteoro = 0;

//Variáveis globais
GLint controleTecla[256]; //1 é quando está pressionado e 0 quando solta 
//GLint score = ;

void carregaTextura(int *textura, char *arquivo) { 
    *textura = SOIL_load_OGL_texture(
      arquivo,
      SOIL_LOAD_AUTO,
      SOIL_CREATE_NEW_ID,
      SOIL_FLAG_INVERT_Y);

    if (textura == 0) {
        printf("Erro do SOIL: '%s'\n", SOIL_last_result());
    }
}

typedef struct{ //composição base de alguns objetos do jogo
  float x; //coordenada x
  float y; //coordenada y
  float altura; 
  float largura; 
}estrutura; 

estrutura BaseDisco; //(40,80,0),(60,80,0),(60,70,0),(40,70,0)
estrutura FundoTela; //(0,0,0),(100,0,0),(100,100,0),(0,100,0)
estrutura Obstaculo[20];

void parametrosIniciais(){//define parâmetros iniciais do jogo

  srand(time(0));
  BaseDisco.x = 40;
  BaseDisco.y = 80;
  BaseDisco.largura = 3;
  BaseDisco.altura = 3;
  FundoTela.x = 0;
  FundoTela.y = 0;
  FundoTela.altura = 100;
  FundoTela.largura = 100;
}

void moverBaseDiscoX(){ //para fazer a base do disco andar de um lado para o outro

  float incrementarX;

  if(controleTecla['a']==1 || controleTecla['A']==1){ //vai para a esquerda 
      incrementarX = -1;
      //se chegar no final do canto esquerdo
      if(BaseDisco.x <= 0){ 
        //vai aparecer do outro lado
        incrementarX = 0;
        BaseDisco.x = 100;
      }
      BaseDisco.x = BaseDisco.x + incrementarX;
  }

  if(controleTecla['d']==1 || controleTecla['D']==1){ //vai para a direita 
      incrementarX = 1;
      //se chegar no final do canto direito
      if(BaseDisco.x + BaseDisco.largura >= 100){ 
        //vai aparecer do outro lado
        incrementarX = 0;
        BaseDisco.x = 0;
      }
      BaseDisco.x = BaseDisco.x + incrementarX;
  }
}

void moverBaseDiscoY(){ //para fazer a base do disco ir para baixo

  float incrementarY = -0.3;

  /*if((BaseDisco.y + BaseDisco.altura) >= 100){ 
    incrementar = -0.3; 
  }
  if(BaseDisco.y <= 0){ 
    incrementar = 0.3; 
  }*/
  BaseDisco.y = BaseDisco.y + incrementarY;
}

/*void moverObstaculos(){

  float incrementar_x = 1;

  printf("%f\n", Obstaculo.x);
  if(Obstaculo.x + Obstaculo.largura >= 100){
    incrementar_x = 0;
    Obstaculo.x = 0;
  }
  if(Obstaculo.x <= 0){
    incrementar_x = 1;
  }
  Obstaculo.x = Obstaculo.x + incrementar_x;  
}*/

void desenhaObjeto(float x, float y, float altura, float largura){ 

  glPushMatrix();                 
    glTranslatef(x, y, 0);
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(-largura/2, -altura/2); // v3---v2
        glVertex2f(largura/2, -altura/2); //  |     |
        glVertex2f(largura/2, altura/2); //   |     |
        glVertex2f(-largura/2, altura/2); //  v0---v1
    glEnd();
  glPopMatrix();
}

void desenhaFundo(){ //textura fundo

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
        glTexCoord2f(1, 1); glVertex3f( 640, 100, 0); //1° quadrante
        glTexCoord2f(0, 1); glVertex3f(-640, 100, 0); //2° quadrante
        glTexCoord2f(0, 0); glVertex3f(-640, -450, 0);//3° quadrante
        glTexCoord2f(1, 0); glVertex3f( 640, -450, 0);//4° quadrante
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void colocarTextura(estrutura obj, int textura){ 

    glColor3f (1, 1, 1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, fundo);
    glBegin(GL_TRIANGLE_FAN);
        glTexCoord2f(0, 0); glVertex2f(obj.x, obj.y);
        glTexCoord2f(1, 0); glVertex2f(obj.x+obj.largura, obj.y); 
        glTexCoord2f(1, 1); glVertex2f(obj.x+obj.largura, obj.y+obj.altura);
        glTexCoord2f(0, 1); glVertex2f(obj.x, obj.y+obj.altura); 
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void obstaculos(){

  glColor3f(0, 206, 209); //DarkTurquoise
  for(int posicao=0; posicao<20; posicao++){
    Obstaculo[posicao].x = 10;
    Obstaculo[posicao].y = 70;
    Obstaculo[posicao].largura = 2;
    Obstaculo[posicao].altura = 2;
    if(posicao==5 || posicao==10 || posicao==15 || posicao==20 || 
    posicao==25 || posicao==30 || posicao==40 || posicao==45){
      Obstaculo[posicao].y = Obstaculo[posicao].y - 10;
    }
    Obstaculo[posicao].x = Obstaculo[posicao].x + 5;
    if(Obstaculo[posicao].x >= 100){
      Obstaculo[posicao].x = 0;
    }
    desenhaObjeto(Obstaculo[posicao].x, Obstaculo[posicao].y,
    Obstaculo[posicao].altura, Obstaculo[posicao].largura);
  }
}

void desenhaCena(void){

  // Limpa a tela com a cor definida por glClearColor(r,g,b) para desenhar
  glClear(GL_COLOR_BUFFER_BIT);

  for(int i=0; i<20; i++){
    colocarTextura(Obstaculo[i], meteoro);
  }

   glColor3f(255, 218, 185); //PeachPuff
  //Desenha um quadrado para ficar no fundo 
  desenhaObjeto(FundoTela.x, FundoTela.y, FundoTela.altura, FundoTela.largura);

  desenhaFundo();
  
  // Desenha a base do disco voador de ficará movimentando de um lado para o outro
  glColor3f(0.29, 0, 0.51); //índigo
  desenhaObjeto(BaseDisco.x, BaseDisco.y, BaseDisco.altura, BaseDisco.largura);

  glFlush(); 
}

// Inicia algumas variáveis de estado
void inicializa(void){

  // cor para limpar a tela
  glClearColor(1, 1, 1, 1); // branco
  // habilita mesclagem de cores, para termos suporte a texturas com transparência
  glEnable(GL_BLEND );
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  carregaTextura(&fundo, "ceu.png");
  carregaTextura(&meteoro, "meteoro.png");
  obstaculos();
  glFlush(); 
}

// Callback de redimensionamento
void redimensiona(int largura, int altura){

  //It just tells which part of the window will be used for rendering(visible)
  glViewport(0, 0, largura, altura); 
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, 100, 0, 100, -1, 1); 
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void atualizaCena(){ // altera algo na cena
  
  desenhaCena();
  moverBaseDiscoY();
  moverBaseDiscoX();
  //moverObstaculos();
  // atualiza a tela (desenha() será invocada novamente)
  glutPostRedisplay(); 
  // registra a callback novamente
  glutTimerFunc(33, atualizaCena, 0); //Podemos registrar uma callback para ser invocada daí a x ms.
  //por que 33? 1000/33 = 30fps
}

// Callback de evento de teclado
void teclado(unsigned char key, int x, int y){ //quando pressiona uma das teclas

  //controleTecla[key] = 1;

  switch(key){
    case 27: // Tecla ESC
      exit(0);
      break;  

    case 'a': case 'A':
      controleTecla['a'] = 1;
      break;

    case 'd': case 'D':
      controleTecla['d'] = 1;
      break;

    default:
       break;
  }
}

void soltaTecla(unsigned char key, int x, int y){ //quando solta uma das teclas

  switch(key){
    case 'a': case 'A':
      controleTecla['a'] = 0;
      break;

    case 'd': case 'D':
      controleTecla['d'] = 0;
      break;

    default:
       break;
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
  glutTimerFunc(0, atualizaCena, 0);
  glutKeyboardFunc(teclado);
  glutKeyboardUpFunc(soltaTecla);

  inicializa();

  // Entra em loop e nunca sai
  glutMainLoop();

  return 0;
}