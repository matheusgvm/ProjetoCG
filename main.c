//TP1 Computação Gráfica: uailien
//Última modificação: 23/04/2019
//Lara Galvani e Matheus Vilaça

#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

//Texturas
GLuint fundo = 0;
GLuint meteoro = 0;
GLuint disco = 0;
GLuint paoDeQueijo = 0;
GLuint aviao = 0;
GLuint gameover = 0;
GLuint pause = 0;
GLuint fase1 = 0;
GLuint fase2 = 0;
GLuint fase3 = 0;
GLuint naveinimiga = 0;
GLuint naveinimigaEsq = 0;
GLuint fundoL2 = 0;
GLuint fundoL3 = 0;
GLuint balao = 0;
GLuint venceu = 0;
GLuint menu = 0;
GLuint fechar = 0;
GLuint comojogar = 0;
GLuint reiniciar = 0;
GLuint encerrar = 0;

//Variáveis globais
GLint controleTecla[256]; //1 é quando está pressionado e 0 quando solta 
GLint score = 300;
GLint mover = 1;
GLfloat moverFundo = 0;
GLint pausa = 0; //1 é quando estiver pausado e 0 não está pausado 
GLint auxiliar = 0; //para o jogo não pausar logo quando inicia
char scoreCerto[15]; //tem que converter o score (int) para string para exibir
GLint jogando = 0; 
GLint proporcao = 2; //proporção imagem fundo
GLint sumir[5] = {0, 0, 0, 0, 0}; //para o pão de queijo coletado sumir
GLint piscar = 0;
GLint fase = 0;
GLint iniciando = 1; //aspectos iniciais da fase
GLint metaL1 = 1000;
GLint metaL2 = 1500;
GLint metaL3 = 3000;
GLint zerou = 0; //quando vencer o jogo a variável será 1
GLint modificarScore = 1; //não contabilizar pontos enquanto o jogador não estiver jogando
GLint habilitarMovimento = 0; //habilita o movimento do disco sentido cima/baixo
GLint menuP = 1; 
GLint comoJogar = 0; 
GLint iniciarJogo = 0; 
GLint esc = 0; 
GLint reinicia = 0; 
GLint reiniciando = 0; 


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
estrutura GameOver;
estrutura Pausar;
estrutura Fase1;
estrutura Fase2;
estrutura Fase3;
estrutura NaveInimiga;
estrutura NaveInimigaEsq;
estrutura FundoL2;
estrutura FundoL3;
estrutura Balao[50];
estrutura posicaoMouse;
estrutura Venceu;
estrutura Menu;
estrutura Fechar;
estrutura ComoJogar;
estrutura Encerrar;
estrutura Reiniciar;

void parametrosIniciais(){//define parâmetros iniciais do jogo

  srand(time(0));
  BaseDisco.x = 40;
  BaseDisco.y = 80;
  BaseDisco.largura = 7;
  BaseDisco.altura = 7;
  GameOver.x = 30;
  GameOver.y = 30;
  GameOver.altura = 40;
  GameOver.largura = 40;
  Pausar.x = 30;
  Pausar.y = 30;
  Pausar.altura = 40;
  Pausar.largura = 40;
  Fundo.x = 0;
  Fundo.y = 0;
  Fundo.largura = 100;
  Fundo.altura = 100;
  Fase1.x = 25;
  Fase1.y = 30;
  Fase1.altura = 40;
  Fase1.largura = 50;
  Fase2.x = 25;
  Fase2.y = 30;
  Fase2.altura = 40;
  Fase2.largura = 50;
  Fase3.x = 25;
  Fase3.y = 30;
  Fase3.largura = 50;
  Fase3.altura = 40;  
  FundoL2.x = 0;
  FundoL2.y = 0;
  FundoL2.largura = 100;
  FundoL2.altura = 100;
  NaveInimiga.x = 50;
  NaveInimiga.y = 50;
  NaveInimiga.largura = 12;
  NaveInimiga.altura = 12;
  NaveInimigaEsq.x = 50;
  NaveInimigaEsq.y = 50;
  NaveInimigaEsq.largura = 12;
  NaveInimigaEsq.altura = 12;
  FundoL3.x = 0;
  FundoL3.y = 0;
  FundoL3.largura = 100;
  FundoL3.altura = 200;
  for(int i=0; i<50; i++){
    Balao[i].x = rand()%101;
    Balao[i].y = rand()%101;
    Balao[i].largura = 8;
    Balao[i].altura = 16;
  }
  Venceu.x = 25;
  Venceu.y = 30;
  Venceu.largura = 50;
  Venceu.altura = 40;
  Menu.x = 0;
  Menu.y = 0;
  Menu.largura = 100;
  Menu.altura = 100;
  Fechar.x = 90;
  Fechar.y = 90;
  Fechar.altura = 5;
  Fechar.largura = 5;
  ComoJogar.x = 0;
  ComoJogar.y = 0;
  ComoJogar.largura = 100;
  ComoJogar.altura = 100;
  Encerrar.x = 0;
  Encerrar.y = 0;
  Encerrar.largura = 100;
  Encerrar.altura = 100;
  Reiniciar.x = 0;
  Reiniciar.y = 0;
  Reiniciar.largura = 100;
  Reiniciar.altura = 100;
}

void escreveTexto(void *font, char *s, float x, float y, float z){

    int i;
    glRasterPos3f(x, y, z);

    for (i = 0; i < strlen(s); i++) {
        glutBitmapCharacter(font, s[i]);
    }
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

  float incrementarY;

  if(controleTecla['S']==1 || controleTecla['s']==1){ //vai para baixo 
      incrementarY = -1;
      //se chegar no final do canto esquerdo
      if(BaseDisco.y <= 0){ 
        //vai aparecer do outro lado
        incrementarY = 0;
        BaseDisco.y = 100;
      }
      BaseDisco.y = BaseDisco.y + incrementarY;
  }

  if(controleTecla['W']==1 || controleTecla['w']==1){ //vai para cima 
      incrementarY = 1;
      //se chegar no final do canto direito
      if(BaseDisco.y + BaseDisco.altura >= 100){ 
        //vai aparecer do outro lado
        incrementarY = 0;
        BaseDisco.y = 0;
      }
      BaseDisco.y = BaseDisco.y + incrementarY;
  }
}

void moverObstaculos(){

  if(fase == 1){
    for (int i = 0; i < 20; i++){
      Obstaculo[i].y++;
    }
  }

  if(fase == 3){
    for (int i = 0; i < 50; i++){
      Balao[i].y++;
    }
  }
  
  for(int i = 0; i < 5; i++){
      PaoDeQueijo[i].y++;
  }
}


void tocar_musica(char const nome[40], int loop){

    Mix_Chunk *som = NULL;
    int canal;
    int canal_audio=2;
    int taxa_audio = 22050;
    Uint16 formato_audio = AUDIO_S16SYS;
    int audio_buffers = 4096;
    if(Mix_OpenAudio(taxa_audio, formato_audio, canal_audio, audio_buffers) != 0) {
            printf("Não pode inicializar audio: %s\n", Mix_GetError());
    }
   som = Mix_LoadWAV(nome);
    if(som == NULL) {
            printf("Não pode inicializar audio: %s\n", Mix_GetError());
    }
   Mix_HaltChannel(-1);
   canal = Mix_PlayChannel( -1, som, loop);
    if(canal == -1) {
            printf("Não pode inicializar audio: %s\n", Mix_GetError());
    }
}

void parar_musica(){

  Mix_HaltChannel(-1);

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

  if(modificarScore == 1){

    if(fase == 1){
      for(int i = 0; i < 20; i++){
        int resultado = colisao(BaseDisco.x, BaseDisco.y, BaseDisco.altura, 
          BaseDisco.largura, Obstaculo[i].x, Obstaculo[i].y, Obstaculo[i].altura,
          Obstaculo[i].largura);

        if(resultado == 1){
          score = score - 3;
          piscar = 1;
          //printf("%d\n", score);
        }
      }
    }

    if(fase == 3){
      for(int i=0; i<50; i++){
        int resultado = colisao(BaseDisco.x, BaseDisco.y, BaseDisco.altura, 
          BaseDisco.largura, Balao[i].x, Balao[i].y, Balao[i].altura, Balao[i].largura);

        if(resultado == 1){
          score = score - 1;
          piscar = 1;
          //printf("%d\n", score);
        }
      }
    }

    for(int i = 0; i < 5; i++){
      int resultado = colisao(BaseDisco.x, BaseDisco.y, BaseDisco.altura, BaseDisco.largura,
      PaoDeQueijo[i].x, PaoDeQueijo[i].y, PaoDeQueijo[i].altura, PaoDeQueijo[i].largura);

      if(resultado == 1){
        if(fase == 3){
          score = score + 10;
        }else{
          score = score + 5;
        }
        sumir[i] = 1;
        //printf("%d\n", score);
      }else{
        sumir[i] = 0;
      }
    }

    if(fase == 2){
      int resultadoN = colisao(BaseDisco.x, BaseDisco.y, BaseDisco.altura, 
        BaseDisco.largura,NaveInimiga.x, NaveInimiga.y, NaveInimiga.altura, 
        NaveInimiga.largura);
      if(resultadoN == 1){
        score = score - 10;
        piscar = 1;
      }
    }
  }
  
  if(fase == 1){
    if(score >= metaL1){
      fase++;
      modificarScore = 0;
      iniciando = 1;
    }
  }

  if(fase == 2){
    if(score >= metaL2){
      fase++;
      modificarScore = 0;
      iniciando = 1;
    }
  }

  if(fase == 3){
    if(score >= metaL3){
      zerou = 1;
      modificarScore = 0;
      iniciando = 1;
      parar_musica();
      tocar_musica("somvitoria.ogg", 1);

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
        glTexCoord2f(0, 0); glVertex2f(Fundo.x+Fundo.largura, Fundo.y+Fundo.altura); 
        glTexCoord2f(1, 0); glVertex2f(0, Fundo.y+Fundo.altura); 
        glTexCoord2f(1, 1); glVertex2f(0, 0);
        glTexCoord2f(0, 1); glVertex2f(Fundo.x+Fundo.largura, 0);
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
        glTexCoord2f(1, 0); glVertex2f(obj.x + obj.largura, obj.y); 
        glTexCoord2f(1, 1); glVertex2f(obj.x + obj.largura, obj.y + obj.altura);
        glTexCoord2f(0, 1); glVertex2f(obj.x, obj.y + obj.altura); 
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void obstaculo1(){

  for(int posicao=0; posicao<20; posicao++){
    Obstaculo[posicao].x = rand()%101;
    Obstaculo[posicao].y = rand()%71;
    Obstaculo[posicao].largura = 5;
    Obstaculo[posicao].altura = 10;
  }
}

void paozinho(){

  for(int posicao=0; posicao<5; posicao++){
    PaoDeQueijo[posicao].x = rand()%101;
    PaoDeQueijo[posicao].y = rand()%71;
    PaoDeQueijo[posicao].largura = 5;
    PaoDeQueijo[posicao].altura = 5;
  }
}

void atualizarObstaculos(){

    for(int i=0; i<20; i++){
      if(Obstaculo[i].y > 105){
        Obstaculo[i].y = rand()%21;
        Obstaculo[i].x = rand()%101;
        colocarTextura(Obstaculo[i], meteoro);
      }
    }

    for(int i=0; i<5; i++){
      if(PaoDeQueijo[i].y > 105){
        PaoDeQueijo[i].y = rand()%21;
        PaoDeQueijo[i].x = rand()%101;
        colocarTextura(Obstaculo[i], paoDeQueijo);
      }
    }

    for(int i=0; i<50; i++){
      if(Balao[i].y > 105){
        Balao[i].y = rand()%21;
        Balao[i].x = rand()%101;
        colocarTextura(Balao[i], balao);
      }
    }  
}

void level1(){

  if(iniciando == 0){

    if(pausa == 1){ //se o jogo for pausado então a pontuação não mudará
      modificarScore = 0;
    }else{
      modificarScore = 1;
    }

    verificarColisao();

    for (int i = 0; i < 5; i++){
      if(sumir[i] == 1){ 
        colocarTextura(PaoDeQueijo[i], paoDeQueijo);
      }
    }

    glPushMatrix();
      glTranslatef(0, moverFundo, 0);
      desenhaFundo();
    glPopMatrix();
    //moverFundo+=0.1;
      
    Fundo.y+=3;
    if(Fundo.y >= 100){
      Fundo.y=0;
      desenhaFundo();
    }
      
    for(int i=0; i<20; i++){
      colocarTextura(Obstaculo[i], meteoro);
    }

    for(int i=0; i<5; i++){
      if(sumir[i] == 0){ //se não colidiu então vai aparecer na tela normalmente
        colocarTextura(PaoDeQueijo[i], paoDeQueijo);
      }
    }
    
    //if(piscar == 0){
    // Optamos por não fazer o disco piscar, mas se quiser ver isso é só tirar os comentários das linhas 536 e 539
      colocarTextura(BaseDisco, disco); 
    //}
    piscar = 0;

    glColor3f(1, 1, 1); 
    //converte o score (int) para string a ser exibida na tela
    sprintf(scoreCerto,"Score: %d", score);
    //desenha a string com o score
    escreveTexto(GLUT_BITMAP_HELVETICA_18, scoreCerto, 82, 95, 0); 
    escreveTexto(GLUT_BITMAP_HELVETICA_18, "Meta: 1000", 82, 90, 0);
  }

  if(iniciando == 1){
    colocarTextura(Fase1, fase1);
    glColor3f(1, 1, 1);
    escreveTexto(GLUT_BITMAP_HELVETICA_18, "Pressione <enter> para continuar", 30, 20, 0);
    BaseDisco.x = 50;
    BaseDisco.y = 80;  
  }

  glFlush();
}

void level2(){

  if(iniciando == 0){

    if(pausa == 1){
      modificarScore = 0;
    }else{
      modificarScore = 1;
    }

    verificarColisao();    

    glPushMatrix();
      glTranslatef(0, moverFundo, 0);
      colocarTextura(FundoL2, fundoL2);
    glPopMatrix();
    //moverFundo+=0.1;

    for(int i=0; i<5; i++){
      if(sumir[i] == 0){ //se não colidiu então vai aparecer na tela normalmente
        colocarTextura(PaoDeQueijo[i], paoDeQueijo);
      }
    }

    NaveInimiga.x += (BaseDisco.x - NaveInimiga.x)/50;
    NaveInimiga.y += (BaseDisco.y - NaveInimiga.y)/50;
    NaveInimigaEsq.x += (BaseDisco.x - NaveInimigaEsq.x)/50;
    NaveInimigaEsq.y += (BaseDisco.y - NaveInimigaEsq.y)/50;
    if(NaveInimiga.x >= 100){
    	colocarTextura(NaveInimigaEsq, naveinimigaEsq);
    }else if(NaveInimigaEsq.y <= 0){
    	colocarTextura(NaveInimiga, naveinimiga);
    }else if(BaseDisco.x >= 50){
    	colocarTextura(NaveInimiga, naveinimiga);
    }else{
    	colocarTextura(NaveInimigaEsq, naveinimigaEsq);
    }
        
    //if(piscar == 0){
      colocarTextura(BaseDisco, disco);
    //}
    piscar = 0;

    glColor3f(1, 1, 1); 
    //converte o score (int) para string a ser exibida na tela
    sprintf(scoreCerto,"Score: %d", score);
    //desenha a string com o score
    escreveTexto(GLUT_BITMAP_HELVETICA_18, scoreCerto, 82, 95, 0); 
    escreveTexto(GLUT_BITMAP_HELVETICA_18, "Meta: 1500", 82, 90, 0);
  }

  if(iniciando == 1){
    colocarTextura(Fase2, fase2);
    glColor3f(1, 1, 1);
    escreveTexto(GLUT_BITMAP_HELVETICA_18, "Pressione <enter> para continuar", 30, 20, 0);
    BaseDisco.x = 50;
    BaseDisco.y = 80;  
    escreveTexto(GLUT_BITMAP_HELVETICA_10, "Nessa fase <w> pode ser usado para ir para cima e <s> para ir para baixo", 25, 15, 0);
  }

  glFlush();
}

void movimentoMouse(int x, int y) {
    posicaoMouse.x = x;
    //printf("x %d\n", posicaoMouse.x);
    posicaoMouse.y = y;
    //printf("y %d\n", posicaoMouse.y);
}

void level3(){

  if(iniciando == 0){

    if(pausa == 1){
      modificarScore = 0;
    }else{
      modificarScore = 1;
    }

    verificarColisao();    

    glPushMatrix();
      glTranslatef(0, moverFundo, 0);
      colocarTextura(FundoL3, fundoL3);
    glPopMatrix();
    //moverFundo+=0.1;

    for(int i=0; i<50; i++){
      colocarTextura(Balao[i], balao);
    }

    for(int i=0; i<5; i++){
      if(sumir[i] == 0){ //se não colidiu então vai aparecer na tela normalmente
        colocarTextura(PaoDeQueijo[i], paoDeQueijo);
      }
    }
       
    //if(piscar == 0){
      colocarTextura(BaseDisco, disco);
    //}
    piscar = 0;

    glColor3f(1, 1, 1); 
    //converte o score (int) para string a ser exibida na tela
    sprintf(scoreCerto,"Score: %d", score);
    //desenha a string com o score
    escreveTexto(GLUT_BITMAP_HELVETICA_18, scoreCerto, 82, 95, 0); 
    escreveTexto(GLUT_BITMAP_HELVETICA_18, "Meta: 3000", 82, 90, 0);
  }

  if(iniciando == 1){
    colocarTextura(Fase3, fase3);
    glColor3f(1, 1, 1);
    escreveTexto(GLUT_BITMAP_HELVETICA_18, "Pressione <enter> para continuar", 30, 20, 0);
    BaseDisco.x = 50;
    BaseDisco.y = 80; 
  }

  glFlush();
}

void desenhaCena(void){

  // Limpa a tela com a cor definida por glClearColor(r,g,b) para desenhar
  glClear(GL_COLOR_BUFFER_BIT);

  if(jogando == 1 && esc!=1 && reinicia!=1){
    if(fase == 1){
      level1();
    }

    if(fase == 2){
      habilitarMovimento = 1; //faz com que o disco possa mover em todas as direções
      level2();
    }

    if(fase == 3){
      habilitarMovimento = 0;
      level3();
    }
  }

  if(zerou == 1){
      colocarTextura(Venceu, venceu);
      jogando = 0;
  }

  if(score <= 0){ //game over
    colocarTextura(GameOver, gameover);
    jogando = 0;
    parar_musica();
    tocar_musica("somderrota.ogg", 1);
  }

  if(pausa == 1){
    colocarTextura(Pausar, pause);
    moverFundo = 0;
    modificarScore = 0;
    habilitarMovimento = 0;
  }

  if(menuP == 1){
    colocarTextura(Menu, menu);
  }

  if(jogando == 0){
    if(iniciarJogo == 1){
      jogando = 1;
      fase++;
    }
  }

  if(comoJogar == 1){
    colocarTextura(ComoJogar, comojogar);
    glColor3f(1, 1, 1);
    
    
    escreveTexto(GLUT_BITMAP_HELVETICA_18, "A: mover para esquerda", 35, 80, 0);
    escreveTexto(GLUT_BITMAP_HELVETICA_18, "D: mover para direita", 37, 70, 0);
    escreveTexto(GLUT_BITMAP_HELVETICA_18, "W: mover para cima (apenas na fase 2)", 27, 60, 0);
    escreveTexto(GLUT_BITMAP_HELVETICA_18, "S: mover para baixo (apenas na fase 2)", 27, 50, 0);
    escreveTexto(GLUT_BITMAP_HELVETICA_18, "Pressione <s> para sair do jogo", 30, 40, 0);
    escreveTexto(GLUT_BITMAP_HELVETICA_18, "Pressione <r> para reiniciar o jogo", 30, 30, 0);
    escreveTexto(GLUT_BITMAP_HELVETICA_18, "Leve pao de queijo para os mineiros e complete a meta de cada fase", 7, 20, 0);
    escreveTexto(GLUT_BITMAP_HELVETICA_18, "Pressione <espaco> para iniciar o jogo", 25, 5, 0);
  }

  if(esc == 1){
  	modificarScore = 0;
  	pausa = 1;
  	glColor3f(1, 1, 1);
  	colocarTextura(Encerrar, encerrar);
  	escreveTexto(GLUT_BITMAP_HELVETICA_18, "Pressione <e>", 17, 20, 0);
  	escreveTexto(GLUT_BITMAP_HELVETICA_18, "Pressione <n>", 67, 20, 0);
  }

  if(reinicia == 1){
  	modificarScore = 0;
  	pausa = 1;
  	colocarTextura(Reiniciar, reiniciar);
  	glColor3f(1, 1, 1);
  	escreveTexto(GLUT_BITMAP_HELVETICA_18, "Pressione <y>", 17, 20, 0);
  	escreveTexto(GLUT_BITMAP_HELVETICA_18, "Pressione <n>", 67, 20, 0);
  	if(reiniciando == 1){
  		jogando = 0;
  		menuP = 1;
  		fase = 0;
  		score = 100;
  		parametrosIniciais();
  		reiniciando = 0;
  		reinicia = 0;
  	}
  }

  
  glFlush(); 
}



// Inicia algumas variáveis de estado
void inicializa(void){

  tocar_musica("musicaMain.ogg", -1);
  
  glEnable(GL_BLEND );
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  carregaTextura(&meteoro, "meteoro.png");
  carregaTextura(&fundo, "fundofase1-1.png");
  carregaTextura(&disco, "spaceship.png");
  carregaTextura(&paoDeQueijo, "pao.png");
  carregaTextura(&gameover, "gameover.png");
  carregaTextura(&pause, "pause.png");
  carregaTextura(&fase1, "fase1.png");
  carregaTextura(&fase2, "fase2.png");
  carregaTextura(&fase3, "fase3.png");
  carregaTextura(&fundoL2, "fundofase2-1.png");
  carregaTextura(&fundoL3, "fundofase3.png");
  carregaTextura(&naveinimiga, "naveinimiga.png");
  carregaTextura(&naveinimigaEsq, "naveinimigaEsquerda.png");
  carregaTextura(&balao, "ballon.png");
  carregaTextura(&venceu, "vocevenceu.png");
  carregaTextura(&menu, "Menu.png");
  carregaTextura(&comojogar, "fundocomojogar.png");
  carregaTextura(&encerrar, "encerrar.png");
  carregaTextura(&reiniciar, "reiniciar.png");

  obstaculo1();
  
  paozinho();  

  desenhaCena();

  glFlush();

  auxiliar++;
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
  
  if(pausa == 0){ //se a tecla 'p' não for pressionada o jogo segue normalmente
    desenhaCena();
    if(habilitarMovimento == 1){
      moverBaseDiscoY();
    }
    moverBaseDiscoX();
    moverObstaculos();
    atualizarObstaculos();
  }

  // atualiza a tela 
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
      controleTecla[' '] = 1;
      esc = 1;
      break;  

    case 13: //enter
      controleTecla[' '] = 1;
      iniciando = 0;
      break;

    case 'e': case 'E':
    	controleTecla['e'] = 1;
    	if(esc == 1){
    		exit(0);
    	}
    	break;

   	case 'n': case 'N':
    	controleTecla['n'] = 1;
    	esc = 0;
    	if(reinicia == 1){
    		pausa = 0;
    		reinicia = 0;
    	}
    	break;

    case 32: //space 
      controleTecla[' '] = 1;
      iniciarJogo = 1;
      menuP = 0;
      comoJogar = 0;
      break;

    case 'a': case 'A':
      controleTecla['a'] = 1;
      break;

    case 'd': case 'D':
      controleTecla['d'] = 1;
      break;

    case 'w': case 'W':
      controleTecla['w'] = 1;
      break;

    case 's': case 'S':
      controleTecla['s'] = 1;
      break;

    case 'r': case 'R':
      controleTecla['r'] = 1;
      reinicia = 1;
      //inicializa();
      //score = 100;
      break;

    case 'y': case 'Y':
      controleTecla['y'] = 1;
      reiniciando = 1;
      break;

    case 'p': case 'P':
    if(auxiliar != 0){
      if(pausa == 0){
        pausa = 1;
      }else{
        pausa = 0;
      }
    }
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

    case 'r': case 'R':
      controleTecla['r'] = 0;
      break;

    case 'w': case 'W':
      controleTecla['w'] = 0;
      break;

    case 's': case 'S':
      controleTecla['s'] = 0;
      break;

    case 13: 
      controleTecla[' '] = 0;
      break;

    case 'y': case 'Y':
      controleTecla['y'] = 0;
      break;

    case 32: //space 
      controleTecla[' '] = 0;
      break;

    case 27: // Tecla ESC
      controleTecla[' '] = 0;
      break;

    case 'n': case 'N':
    	controleTecla['n'] = 0;
    	break;

    default:
       break;
  }
}

void MouseClick (int button, int state, int x, int y){

  if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){ //se clicar em algum dos botões
    if(x>=200 && x<=447 && y>=270 && y<=377){ //coordenadas "jogar"
      menuP = 0;
      iniciarJogo = 1;
    }
    if(x>=200 && x<=447 && y>=403 && y<=564){ //coordenadas "como jogar"
      comoJogar = 1;
    }
    if(x>=34 && x<=58 && y==0){
      fechar = 1;
      printf("a\n");
    }
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
  glutCreateWindow("uailien");

  // Registra callbacks para alguns eventos
  //glutDisplayFunc(desenha); //textura fundo
  glutDisplayFunc(desenhaCena); //chama outras funções para compor a cena principal
  glutReshapeFunc(redimensiona);
  glutTimerFunc(0, atualizaCena, 0);
  glutKeyboardFunc(teclado);
  glutKeyboardUpFunc(soltaTecla);

  // movimento SEM botão pressionado
  glutPassiveMotionFunc(movimentoMouse);

  // Click em um botão
  glutMouseFunc(MouseClick);

  inicializa();

  // Entra em loop e nunca sai
  glutMainLoop();

  return 0;
}