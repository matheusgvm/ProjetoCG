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
GLuint disco = 0;
GLuint paoDeQueijo = 0;
GLuint aviao = 0;

//Variáveis globais
GLint controleTecla[256]; //1 é quando está pressionado e 0 quando solta 
GLint score = 100;
GLint mover = 1;
GLint moverFundo = 0;
GLint moverObstaculos = 0;

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
estrutura Obstaculo[20];
estrutura PaoDeQueijo[5];
estrutura Fundo;


void parametrosIniciais(){//define parâmetros iniciais do jogo

  srand(time(0));
  BaseDisco.x = 40;
  BaseDisco.y = 80;
  BaseDisco.largura = 7;
  BaseDisco.altura = 7;
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

  float incrementarY = -1;
  if(mover==0){
    incrementarY = 0;
    BaseDisco.y = 100;
  }
  BaseDisco.y = BaseDisco.y + incrementarY;
}

//n=nave | o=obstaculo
int colisao(float nX, float nY, float nAlt, float nComp, float oX, float oY, float oAlt, float oComp){

  if (nY-nAlt > oY){ //o Y cresce pra cima e o x cresce pra direita
    return 0;
  }else if (nY < oY-oAlt){
    return 0;
  }else if (nX+nComp < oX){
    return 0;
  }else if (nX > oX+oComp){
    return 0;
  }
  return 1;
}

void verificarColisao(){

  for(int i = 0; i < 20; i++){
    int resultado = colisao(BaseDisco.x, BaseDisco.y, BaseDisco.altura, BaseDisco.largura,
      Obstaculo[i].x, Obstaculo[i].y, Obstaculo[i].altura, Obstaculo[i].largura);

    if(resultado == 1){
      score = score - 1;
      printf("%d\n", score);
    }
  }

  for(int i = 0; i < 5; i++){
    int resultado = colisao(BaseDisco.x, BaseDisco.y, BaseDisco.altura, BaseDisco.largura,
      PaoDeQueijo[i].x, PaoDeQueijo[i].y, PaoDeQueijo[i].altura, PaoDeQueijo[i].largura);

    if(resultado == 1){
      score = score + 5;
      printf("%d\n", score);
    }
  }
}

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
        glTexCoord2f(1, 1); glVertex3f( 640, 0, 0); //1° quadrante
        glTexCoord2f(0, 1); glVertex3f(-640, 0, 0); //2° quadrante
        glTexCoord2f(0, 0); glVertex3f(-640, -640, 0);//3° quadrante
        glTexCoord2f(1, 0); glVertex3f( 640, -640, 0);//4° quadrante
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void colocarTextura(estrutura obj, int textura){ 

    glColor3f (1, 1, 1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textura);
    glPushMatrix();
    glBegin(GL_TRIANGLE_FAN);
        glTexCoord2f(0, 0); glVertex2f(obj.x, obj.y);
        glTexCoord2f(0, 1); glVertex2f(obj.x + obj.largura, obj.y); 
        glTexCoord2f(1, 1); glVertex2f(obj.x + obj.largura, obj.y + obj.altura);
        glTexCoord2f(1, 0); glVertex2f(obj.x, obj.y + obj.altura); 
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void obstaculo1(){

  for(int posicao=0; posicao<20; posicao++){
    Obstaculo[posicao].x = rand()%101;
    Obstaculo[posicao].y = rand()%71;
    Obstaculo[posicao].largura = 5;
    Obstaculo[posicao].altura = 5;
  }
}

void obstaculo2(){

  for(int posicao=0; posicao<20; posicao++){
    Obstaculo[posicao].x = rand()%101;
    Obstaculo[posicao].y = rand()%101;
    Obstaculo[posicao].largura = 5;
    Obstaculo[posicao].altura = 5;
  }
}

void paozinho(){

  for(int posicao=0; posicao<5; posicao++){
    PaoDeQueijo[posicao].x = rand()%101;
    PaoDeQueijo[posicao].y = rand()%101;
    PaoDeQueijo[posicao].largura = 3;
    PaoDeQueijo[posicao].altura = 3;
  }
}

void atualizarObstaculos(){

  if(moverFundo > 200){
    obstaculo2(); //gera outros obstáculos
    for(int i=0; i<20; i++){
      colocarTextura(Obstaculo[i], aviao);
    }

    paozinho();
    for(int j=0; j<5; j++){
      colocarTextura(PaoDeQueijo[j], paoDeQueijo);
    }
  }
}

void desenhaCena(void){

  // Limpa a tela com a cor definida por glClearColor(r,g,b) para desenhar
  glClear(GL_COLOR_BUFFER_BIT);

  glPushMatrix();
    glTranslatef(0, moverFundo, 0);
    desenhaFundo();
  glPopMatrix();
  moverFundo++;
  printf("%d\n", moverFundo);
  if(moverFundo == 640){
    moverFundo = 0;
  }

  // Desenha a base do disco voador de ficará movimentando de um lado para o outro
  //glColor3f(0.29, 0, 0.51); //índigo
  //desenhaObjeto(BaseDisco.x, BaseDisco.y, BaseDisco.altura, BaseDisco.largura);
  glPushMatrix();
    glTranslatef(0, moverObstaculos, 0);
  if(moverFundo <= 200){
    for(int i=0; i<20; i++){
      colocarTextura(Obstaculo[i], meteoro);
    }

    for(int i=0; i<5; i++){
      colocarTextura(PaoDeQueijo[i], paoDeQueijo);
    }
  }
  glPopMatrix();

  colocarTextura(BaseDisco, disco);
  
  glFlush(); 
}

// Inicia algumas variáveis de estado
void inicializa(void){
  
  glEnable(GL_BLEND );
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  carregaTextura(&meteoro, "meteoro.png");
  carregaTextura(&fundo, "ceu.png");
  carregaTextura(&disco, "spaceship.png");
  carregaTextura(&paoDeQueijo, "pao.png");

  obstaculo1();
  
  paozinho();

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
  //moverBaseDiscoY();
  moverBaseDiscoX();
  //moverObstaculos();
  atualizarObstaculos();
  // atualiza a tela 
  glutPostRedisplay(); 
  // registra a callback novamente
  glutTimerFunc(33, atualizaCena, 0); //Podemos registrar uma callback para ser invocada daí a x ms.
  //por que 33? 1000/33 = 30fps
  verificarColisao();
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

void MouseClick (int button, int state, int x, int y){

  if(state == GLUT_DOWN){
    printf("Pressionado na posição: ");
  }
  if(state == GLUT_UP){
    printf("Liberado na posição: ");
  }
  printf("(%d, %d)\n", x,y);
}

void MouseAndandoNaoPressionado(int x, int y){
    //printf("Mouse ANDANDO solto na janela. Posição: (%d, %d)\n", x,y);
}

void MouseAndandoPressionado(int x, int y){
    //printf("Mouse ANDANDO pressionado na janela. Posição: (%d, %d)\n", x,y);
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

  // movimento SEM botão pressionado
  glutPassiveMotionFunc(MouseAndandoNaoPressionado);

  // movimento COM botão pressionado
  glutMotionFunc(MouseAndandoPressionado);

  // Click em um botão
  glutMouseFunc(MouseClick);

  inicializa();

  // Entra em loop e nunca sai
  glutMainLoop();

  return 0;
}
