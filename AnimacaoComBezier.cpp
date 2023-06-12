// **********************************************************************
// PUCRS/Escola Politécnica
// COMPUTAÇÃO GRÁFICA
//
// Programa basico para criar aplicacoes 2D em OpenGL
//
// Marcio Sarroglia Pinho
// pinho@pucrs.br
// **********************************************************************

// Para uso no Xcode:
// Abra o menu Product -> Scheme -> Edit Scheme -> Use custom working directory
// Selecione a pasta onde voce descompactou o ZIP que continha este arquivo.

#include <vector>
#include <iostream>
#include <cmath>
#include <ctime>
#include <fstream>


using namespace std;

#ifdef WIN32
#include <windows.h>
#include <glut.h>
#else
#include <sys/time.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#ifdef __linux__
#include <glut.h>
#endif

#include "Ponto.h"
#include "Poligono.h"
#include "InstanciaBZ.h"

#include "Temporizador.h"
#include "ListaDeCoresRGB.h"

Temporizador T;
double AccumDeltaT=0;
Temporizador T2;

InstanciaBZ Personagens[11];

int nCurvas;

// Limites lógicos da área de desenho
Ponto Min, Max;

Bezier Curvas[40];

Poligono Mastro, PontosdeControle;

Poligono Envelope[11];

bool para = true;

int nInstancias=0;
int Escolhe;
float angulo=160.0;

// **********************************************************************

void CarregaModelos()
{
    PontosdeControle.LePoligono("PontosdeControle.txt");
}

// **********************************************************************

void volta(){
    int tempDirecao = Personagens[0].direcao;
    
    if (tempDirecao == 0){
        Personagens[0].direcao = 1;
    }
    if (tempDirecao == 1){
        Personagens[0].direcao = 0;
    }
}

// **********************************************************************

void LeCurvas(const char *nome)
{
    ifstream input;            // ofstream arq;
    input.open(nome, ios::in); //arq.open(nome, ios::out);
    if (!input)
    {
        cout << "Erro ao abrir " << nome << ". " << endl;
        exit(0);
    }
    string S;
    int nLinha = 0;
    unsigned int qtdVertices;
    
    input >> qtdVertices;  // arq << qtdVertices
    
    for (int i=0; i< qtdVertices; i++)
    {
        double x,y,z;
        // Le cada elemento da linha
        input >> x >> y >> z;
        if(!input)
            break;
        nLinha++;
        Curvas[i] = Bezier(PontosdeControle.getVertice(x), PontosdeControle.getVertice(y), PontosdeControle.getVertice(z));
    }
    nCurvas = nLinha;
    
}

// **********************************************************************

double nFrames=0;
double TempoTotal=0;

// **********************************************************************

void animate()
{
    double dt;
    dt = T.getDeltaT();
    AccumDeltaT += dt;
    TempoTotal += dt;
    nFrames++;

    if (AccumDeltaT > 1.0/30) // fixa a atualização da tela em 30
    {
        AccumDeltaT = 0;
        angulo+=2;
        glutPostRedisplay();
    }
    
}

// **********************************************************************

void reshape( int w, int h )
{
    // Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Define a area a ser ocupada pela area OpenGL dentro da Janela
    glViewport(0, 0, w, h);
    // Define os limites logicos da area OpenGL dentro da Janela
    glOrtho(Min.x,Max.x, Min.y,Max.y, -10,+10);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// **********************************************************************

void CriaInstancias()
{
    Personagens[0] = InstanciaBZ(&Curvas[0]);
    
    Personagens[1] = InstanciaBZ(&Curvas[rand()%nCurvas]);
    Personagens[1].tAtual = 0.5;
    Personagens[1].direcao = 1;
    
    Personagens[2] = InstanciaBZ(&Curvas[rand()%nCurvas]);
    Personagens[2].tAtual = 0.5;
    Personagens[2].direcao = 1;
    
    Personagens[3] = InstanciaBZ(&Curvas[rand()%nCurvas]);
    Personagens[3].tAtual = 0.5;
    Personagens[3].direcao = 1;
    
    Personagens[4] = InstanciaBZ(&Curvas[rand()%nCurvas]);
    Personagens[4].tAtual = 0.5;
    Personagens[4].direcao = 1;
    
    Personagens[5] = InstanciaBZ(&Curvas[rand()%nCurvas]);
    Personagens[5].tAtual = 0.5;
    Personagens[5].direcao = 1;
    
    Personagens[6] = InstanciaBZ(&Curvas[rand()%nCurvas]);
    Personagens[6].tAtual = 0.5;
    Personagens[6].direcao = 0;
    
    Personagens[7] = InstanciaBZ(&Curvas[rand()%nCurvas]);
    Personagens[7].tAtual = 0.5;
    Personagens[7].direcao = 0;
    
    Personagens[8] = InstanciaBZ(&Curvas[rand()%nCurvas]);
    Personagens[8].tAtual = 0.5;
    Personagens[8].direcao = 0;
    
    Personagens[9] = InstanciaBZ(&Curvas[rand()%nCurvas]);
    Personagens[9].tAtual = 0.5;
    Personagens[9].direcao = 0;
    
    Personagens[10] = InstanciaBZ(&Curvas[rand()%nCurvas]);
    Personagens[10].tAtual = 0.5;
    Personagens[10].direcao = 0;
    
    nInstancias = 11;
}

// **********************************************************************

void init()
{
    glClearColor(0.0,0.0,0,40.0);

    CarregaModelos();

    LeCurvas("Curvas.txt");
    
    CriaInstancias();
    
    float d = 26;
    Min = Ponto(-d,-d);
    Max = Ponto(d,d);
}

// **********************************************************************

void DesenhaPersonagens(float tempoDecorrido)
{
    defineCor(White);
    
    if (para){
        Personagens[0].AtualizaPosicao(tempoDecorrido);
    }
    
    Envelope[0].pintaPoligono();
    
    for(int i=1; i<nInstancias;i++)
    {
        Personagens[i].AtualizaPosicao(tempoDecorrido);
        defineCor(GreenYellow);
        glLineWidth(1);
        Envelope[i].pintaPoligono();
    }
}

// **********************************************************************

void InimigoAnda(int k){
    vector <int> v;
    for(int j=0; j<nCurvas; j++){
        for(int i=0; i<nCurvas; i++){
            if ((Personagens[k].InstanciaBZ::ObtemPosicao() == Curvas[i].getPC(0) || Personagens[k].InstanciaBZ::ObtemPosicao() == Curvas[i].getPC(2)) && Personagens[k].Curva != &Curvas[i]){
                v.push_back(i);
            }
        }
        if(Personagens[k].InstanciaBZ::ObtemPosicao() == Curvas[j].getPC(0)){
            int aleatorio = rand()%v.size();
            int elemA = v[aleatorio];

            Personagens[k].Curva = &Curvas[elemA];
            Personagens[k].direcao = 1;
            Personagens[k].tAtual = 0;
            if (Personagens[k].InstanciaBZ::ObtemPosicao() == Curvas[elemA].getPC(2)){
                Personagens[k].direcao = 0;
                Personagens[k].tAtual = 1;
            }
        }
    }
}

int a = 0;


// **********************************************************************

void Jogador(){
    if (Personagens[0].direcao == 1 && Personagens[0].tAtual > 0.5){
        Bezier* curva = Personagens[0].Curva;
        Ponto ponto = curva->getPC(2);
        vector <int> v;
            for (int i=0; i<nCurvas; i++){
                if ((ponto == Curvas[i].getPC(0) || ponto == Curvas[i].getPC(2)) && Personagens[0].Curva != &Curvas[i]){
                    v.push_back(i);
                }
        }
        int elem = v[a%v.size()];
        for (int k=0; k<v.size(); k++){
            defineCor(White);
            glLineWidth(0.25);
            Curvas[v[k]].Traca();
            
            
            defineCor(Green);
            glLineWidth(3);
            Curvas[elem].Traca();
        }
        
        if(Personagens[0].tAtual == 1){
            Personagens[0].Curva = &Curvas[elem];
            Personagens[0].direcao = 1;
            Personagens[0].tAtual = 0;
            if (Personagens[0].InstanciaBZ::ObtemPosicao() == Curvas[elem].getPC(2)){
                Personagens[0].direcao = 0;
                Personagens[0].tAtual = 1;
            }
        }
        
        
    }
    if (Personagens[0].direcao == 0 && Personagens[0].tAtual < 0.5){
        Bezier* curva = Personagens[0].Curva;
        Ponto ponto = curva->getPC(0);
        vector <int> v;
            for (int i=0; i<nCurvas; i++){
                if ((ponto == Curvas[i].getPC(0) || ponto == Curvas[i].getPC(2)) && Personagens[0].Curva != &Curvas[i]){
                    v.push_back(i);
                }
        }
        for (int k=0; k<v.size(); k++){
            defineCor(White);
            glLineWidth(0.25);
            Curvas[v[k]].Traca();
        }
        
        int elem = v[a%v.size()];
        for (int k=0; k<v.size(); k++){
            defineCor(White);
            glLineWidth(0.25);
            Curvas[v[k]].Traca();
            
            defineCor(Green);
            glLineWidth(3);
            Curvas[elem].Traca();
        }
        
        if(Personagens[0].tAtual == 0){
            Personagens[0].Curva = &Curvas[elem];
            Personagens[0].direcao = 1;
            Personagens[0].tAtual = 0;
            if (Personagens[0].InstanciaBZ::ObtemPosicao() == Curvas[elem].getPC(2)){
                Personagens[0].direcao = 0;
                Personagens[0].tAtual = 1;
            }
        }
    }
}

// **********************************************************************

void CriaEnvelope(int i){
    Ponto posicao = Personagens[i].ObtemPosicao();
    Poligono tempEnvelope;
    
    for (int j=0; j<Personagens[i].poligono.getNVertices(); j++){
        Ponto aux = Personagens[i].poligono.getVertice(j);
        aux.rotacionaZ(Personagens[i].Rotacao);
        tempEnvelope.insereVertice(aux + posicao);
    }
    Envelope[i] = tempEnvelope;
}

// **********************************************************************
//



void Colisao(){
    Ponto v1 = operator-(Envelope[0].getVertice(1), Envelope[0].getVertice(0));
    Ponto v2 = operator-(Envelope[0].getVertice(2), Envelope[0].getVertice(1));
    Ponto v3 = operator-(Envelope[0].getVertice(0), Envelope[0].getVertice(2));
    for (int i=0; i<nInstancias;i++){
        for (int j=0; j<Envelope[i].getNVertices(); j++){
            Ponto va = operator-(Envelope[i].getVertice(j), Envelope[0].getVertice(0));
            if ((v1.x * va.y) - (v1.y * va.x) < 0){
                va = operator-(Envelope[i].getVertice(j), Envelope[0].getVertice(1));
                if ((v2.x * va.y) - (v2.y * va.x) < 0){
                    va = operator-(Envelope[i].getVertice(j), Envelope[0].getVertice(2));
                    if ((v3.x * va.y) - (v3.y * va.x) < 0){
                        cout << "Fim de jogo, o Seu Personagem Colidiu!!!!" << endl;

                    }
                }
            }
        }
    }
}






// **********************************************************************
void DesenhaCurvas()
{
    for(int i=0; i<nCurvas;i++)
    {
        glLineWidth(2);
        defineCor(Red);
        Curvas[i].Traca();
    }
}
// **********************************************************************
//  void display( void )
// **********************************************************************
void display( void )
{

	// Limpa a tela coma cor de fundo
	glClear(GL_COLOR_BUFFER_BIT);

    // Define os limites lÛgicos da ·rea OpenGL dentro da Janela
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	// Coloque aqui as chamadas das rotinas que desenham os objetos
	// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	glLineWidth(1);
	glColor3f(1,1,1); // R, G, B  [0..1]
    
    DesenhaCurvas();
    
    for(int i=1;i<nInstancias;i++){
        InimigoAnda(i);
    }
    
    for(int i=0; i<nInstancias; i++){
        CriaEnvelope(i);
    }
    Jogador();
    
    Colisao();
    
    DesenhaPersonagens(T2.getDeltaT());
    
	glutSwapBuffers();
}

// **********************************************************************
//  void keyboard ( unsigned char key, int x, int y )
// **********************************************************************
void keyboard ( unsigned char key, int x, int y )
{

	switch ( key )
	{
		case 27:        // Termina o programa qdo
			exit ( 0 );   // a tecla ESC for pressionada
			break;
        case 'z':
            volta();
            break;
        case 'x':
            para = !para;
            break;
        case ' ':
        break;
		default:
			break;
	}
}
// **********************************************************************
//  void arrow_keys ( int a_keys, int x, int y )
// **********************************************************************
void arrow_keys ( int a_keys, int x, int y )
{
	switch ( a_keys )
	{
        case GLUT_KEY_LEFT:       // Se pressionar LEFT
            a = a+1;
            break;
        case GLUT_KEY_RIGHT:       // Se pressionar LEFT
            a = a-1;
            break;
        default:
            break;
	}
}

// **********************************************************************
//  void main ( int argc, char** argv )
//
// **********************************************************************
int  main ( int argc, char** argv )
{
    cout << "Programa OpenGL" << endl;

    glutInit            ( &argc, argv );
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB );
    glutInitWindowPosition (0,0);

    // Define o tamanho inicial da janela grafica do programa
    glutInitWindowSize  ( 700, 700);

    // Cria a janela na tela, definindo o nome da
    // que aparecera na barra de tÌtulo da janela.
    glutCreateWindow    ( "Animacao com Bezier" );

    // executa algumas inicializaÁıes
    init ();

    glutDisplayFunc ( display );

    glutIdleFunc(animate);

    glutReshapeFunc ( reshape );

    glutKeyboardFunc ( keyboard );

    glutSpecialFunc ( arrow_keys );

    // inicia o tratamento dos eventos
    glutMainLoop ( );

    return 0;
}
