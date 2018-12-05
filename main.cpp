// Um programa OpenGL que exemplifica a visualização de objetos 3D com a inserção de uma fonte de luz.
// Este código está baseado nos códigos:
// ==> "Iluminacao.c" de Isabel H. Manssour (https://www.inf.pucrs.br/~manssour/OpenGL/Iluminacao.html).
// ==> "Interacao.c" de Isabel H. Manssour (https://www.inf.pucrs.br/~manssour/OpenGL/Eventos.html).
// ==> "braco.h" de Agostinho de Medeiros Brito Júnior (https://www.dca.ufrn.br/~ambj/opengl/transformacoes.html).

#include <iostream>
#include <GL/glut.h>
#include "FILE.h"
#include "Objeto.h"
#include <array>
#include <iterator>

using namespace std;

#define TEAPOT 0
#define CUBE 1
#define SPHERE 2
#define CONE 3
#define TORUS 4
#define ICOSAHEDRON 5
#define OCTAHEDRON 6
#define TETRAHEDRON 7
#define DODECAHEDRON 8

// declaração de variáveis globais
static int shape = 1, translationX = 0, translationY = 0, rotationX = 0, rotationY = 0, rotationZ = 0;
static float scaleX = 1, scaleY = 1, scaleZ = 1;
GLfloat angle, f_aspect;

char texto[30];
GLfloat win, r, g, b;
GLint view_w, view_h, primitiva;

float corAtual[3] = {0.0f, 0.0f, 1.0f};

bool visualizacaoOrtografica = false;

// Variáveis controladoras de elementos
ObjetoNativo elementosNativos[150000];
ObjetoArquivo elementosArquivos[150000];
int qtdElementosNativos = 0, qtdElementosArquivos = 0;

void MenuCor(int op);
void MenuAddElemento(int op);
void MenuPrincipal(int op);
void MenuTela(int op);
void MenuRemoverElemento(int op);
void MenuTelaBackground(int op);
void MenuEscala(int op);
void MenuArquivo(int op);
void CriaMenu();
void visualization();

// Gerenciamento do menu com as opções de cores

void MenuCor(int op) {
    switch (op) {
        case 0: // Preto
            corAtual[0] = 0.0f;
            corAtual[1] = 0.0f;
            corAtual[2] = 0.0f;
            break;
        case 1: // Branco
            corAtual[0] = 1.0f;
            corAtual[1] = 1.0f;
            corAtual[2] = 1.0f;
            break;
        case 2: // Vermelho
            corAtual[0] = 1.0f;
            corAtual[1] = 0.0f;
            corAtual[2] = 0.0f;
            break;
        case 3: // Verde
            corAtual[0] = 0.0f;
            corAtual[1] = 1.0f;
            corAtual[2] = 0.0f;
            break;
        case 4: // Azul
            corAtual[0] = 0.0f;
            corAtual[1] = 0.0f;
            corAtual[2] = 1.0f;
            break;
    }
    glutPostRedisplay();
}

// Gerenciamento do menu com as opções de cores

void MenuAddElemento(int op) {

    if (op >= 0 && op <= 8) {
        ObjetoNativo objeto;
        objeto.id = op;
        objeto.cor[0] = corAtual[0];
        objeto.cor[1] = corAtual[1];
        objeto.cor[2] = corAtual[2];
        objeto.escala[0] = scaleX;
        objeto.escala[1] = scaleY;
        objeto.escala[2] = scaleZ;
        elementosNativos[qtdElementosNativos] = objeto;
        qtdElementosNativos++;
    }

    glutPostRedisplay();
}

void MenuEscala(int op) {
    switch (op) {
        case 0: // X
            cout << "Escala em X: ";
            cin >> scaleX;
            break;
        case 1: // Y
            cout << "Escala em Y: ";
            cin >> scaleY;
            break;
        case 2: // Z
            cout << "Escala em Z: ";
            cin >> scaleZ;
            break;
        case 3: // X,Y,Z
            cout << "Escala em X, Y e Z: ";
            float escala;
            cin >> escala;
            scaleX = escala;
            scaleY = escala;
            scaleZ = escala;
            break;
    }
    glutPostRedisplay();
}

void MenuArquivo(int op) {
    char nomeArq[50];
    ObjetoArquivo objeto;
    switch (op) {
        case 0: //Importar
            cout << "Nome do arquivo (.obj) de entrada: ";
            cin >> nomeArq;
            lerArquivo(nomeArq);

            objeto.cor[0] = corAtual[0];
            objeto.cor[1] = corAtual[1];
            objeto.cor[2] = corAtual[2];
            objeto.escala[0] = scaleX;
            objeto.escala[1] = scaleY;
            objeto.escala[2] = scaleZ;
            objeto.vertices = vertices;
            objeto.verticesIt = verticesIt;
            objeto.faceInt = faceInt;
            objeto.faceIntIt = faceIntIt;
            objeto.FV = FV;
            objeto.FVIt = FVIt;
            elementosArquivos[qtdElementosArquivos] = objeto;
            qtdElementosArquivos++;
            break;
        case 1: //Exportar
            cout << "Nome do arquivo (.obj) de saida: ";
            cin >> nomeArq;
            gravarArquivo(nomeArq);
            break;

    }
    glutPostRedisplay();
}


// Gerenciamento do menu principal

void MenuPrincipal(int op) {
}

void MenuTela(int op) {
    switch (op) {
        case 0:
            qtdElementosNativos = 0;
            qtdElementosArquivos = 0;
            break;
    }
    glutPostRedisplay();
}

void MenuRemoverElemento(int op) {
}

void MenuTelaBackground(int op) {
    switch (op) {
        case 0:
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            break;
        case 1:
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            break;
        case 2:
            glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
            break;
        case 3:
            glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
            break;
        case 4:
            glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
            break;
    }
    glutPostRedisplay();
}

void MenuVisualizacao(int op) {
    switch (op) {
        case 0:
            visualizacaoOrtografica = false;
            break;
        case 1:
            visualizacaoOrtografica = true;
            break;
    }
    visualization();
    glutPostRedisplay();
}

// Criação do Menu

void CriaMenu() {
    int menu, submenuCor, submenuEscala, submenuArquivo, submenuAddElemento, submenuTela, submenuTelaBackground, submenuRemoverElemento, submenuVisualizacao;

    submenuTelaBackground = glutCreateMenu(MenuTelaBackground);
    glutAddMenuEntry("Preto", 0);
    glutAddMenuEntry("Branco", 1);
    glutAddMenuEntry("Vermelho", 2);
    glutAddMenuEntry("Verde", 3);
    glutAddMenuEntry("Azul", 4);

    submenuTela = glutCreateMenu(MenuTela);
    glutAddSubMenu("Background", submenuTelaBackground);
    glutAddMenuEntry("Limpar Tela", 0);

    submenuVisualizacao = glutCreateMenu(MenuVisualizacao);
    glutAddMenuEntry("Perspectiva", 0);
    glutAddMenuEntry("Ortografica", 1);

    submenuCor = glutCreateMenu(MenuCor);
    glutAddMenuEntry("Preto", 0);
    glutAddMenuEntry("Branco", 1);
    glutAddMenuEntry("Vermelho", 2);
    glutAddMenuEntry("Verde", 3);
    glutAddMenuEntry("Azul", 4);

    submenuAddElemento = glutCreateMenu(MenuAddElemento);
    glutAddMenuEntry("TEAPOT", 0);
    glutAddMenuEntry("CUBE", 1);
    glutAddMenuEntry("SPHERE", 2);
    glutAddMenuEntry("CONE", 3);
    glutAddMenuEntry("TORUS", 4);
    glutAddMenuEntry("ICOSAHEDRON", 5);
    glutAddMenuEntry("OCTAHEDRON", 6);
    glutAddMenuEntry("TETRAHEDRON", 7);
    glutAddMenuEntry("DODECAHEDRON", 8);

    submenuEscala = glutCreateMenu(MenuEscala);
    glutAddMenuEntry("X", 0);
    glutAddMenuEntry("Y", 1);
    glutAddMenuEntry("Z", 2);
    glutAddMenuEntry("X,Y,Z", 3);

    submenuArquivo = glutCreateMenu(MenuArquivo);
    glutAddMenuEntry("Importar", 0);
    glutAddMenuEntry("Exportar", 1);


    if (qtdElementosNativos > 0) {
        submenuRemoverElemento = glutCreateMenu(MenuRemoverElemento);
        for (int i = 0; i < qtdElementosNativos; i++) {
            glutAddMenuEntry("" + (i + 1), i);
        }
    }

    menu = glutCreateMenu(MenuPrincipal);
    glutAddSubMenu("Cor", submenuCor);
    glutAddSubMenu("Adicionar Elemento", submenuAddElemento);
    glutAddSubMenu("Configurar Tela", submenuTela);
    glutAddSubMenu("Escala", submenuEscala);
    glutAddSubMenu("Visualizacao", submenuVisualizacao);
    glutAddSubMenu("Arquivo", submenuArquivo);
    if (qtdElementosNativos > 0) {
        glutAddSubMenu("Remover Elemento", submenuRemoverElemento);
    }

    glutAttachMenu(1);
}

void apresentarInformacoes() {
    cout << endl;
    cout << "======== INFORMACOES DO PROGRAMA ========" << endl;
    cout << "= TECLA =            FUNCAO             =" << endl;
    cout << "=---------------------------------------=" << endl;
    cout << "=  x/X  = Rotacao em torno do eixo X    =" << endl;
    cout << "=  y/Y  = Rotacao em torno do eixo Y    =" << endl;
    cout << "=  z/Z  = Rotacao em torno do eixo Z    =" << endl;
    cout << "=  i/F1 = Visualizacao das informacoes  =" << endl;
    cout << "= Pg Up = Zoon in                       =" << endl;
    cout << "= Pg Dn = Zoon out                      =" << endl;
    cout << "=========================================" << endl << endl;
}

GLvoid CalculateVectorNormal(Vertice fVert1, Vertice fVert2,
                             Vertice fVert3, GLfloat *fNormalX,
                             GLfloat *fNormalY, GLfloat *fNormalZ)
    {
    GLfloat Qx, Qy, Qz, Px, Py, Pz;

   Qx = fVert2.x-fVert1.x;
   Qy = fVert2.y-fVert1.y;
   Qz = fVert2.z-fVert1.z;
   Px = fVert3.x-fVert1.x;
   Py = fVert3.y-fVert1.y;
   Pz = fVert3.z-fVert1.z;

   *fNormalX = Py*Qz - Pz*Qy;
   *fNormalY = Pz*Qx - Px*Qz;
   *fNormalZ = Px*Qy - Py*Qx;

   }

// função para desenhar uma malha de triângulos 3D

void drawTriangleMesh() {
    GLfloat fNormalX;
    GLfloat fNormalY;
    GLfloat fNormalZ;

    glBegin(GL_POINTS);

    // Passar por todos os vértices para plotá-los
    for (int i = 0; i < qtdElementosArquivos; i++) {
        //std::list<Vertice>::iterator it = elementosArquivos[0].vertices.begin();
        //auto it1 = std::next(elementosArquivos[0].vertices.begin(), 0);
        //auto it2 = std::next(elementosArquivos[0].vertices.begin(), 1);
        //auto it3 = std::next(elementosArquivos[0].vertices.begin(), 2);

        //Calcula o vetor normal
        glColor3f(elementosArquivos[i].cor[0], elementosArquivos[i].cor[1], elementosArquivos[i].cor[2]);
        for (elementosArquivos[i].verticesIt = elementosArquivos[i].vertices.begin(); elementosArquivos[i].verticesIt != elementosArquivos[i].vertices.end(); elementosArquivos[i].verticesIt++) {
            glVertex3f(elementosArquivos[i].verticesIt->x*elementosArquivos[i].escala[0], elementosArquivos[i].verticesIt->y*elementosArquivos[i].escala[1], elementosArquivos[i].verticesIt->z*elementosArquivos[i].escala[2]);
            //CalculateVectorNormal(*it1, *it2, *it3, &fNormalX, &fNormalY, &fNormalZ);
            //glNormal3f(fNormalX, fNormalY, fNormalZ);
        }
    }

    for (int i = 0; i < qtdElementosArquivos; i++) {
        glColor3f(elementosArquivos[i].cor[0], elementosArquivos[i].cor[1], elementosArquivos[i].cor[2]);
        std::list<FaceVertice>::iterator j;
        std::list<FaceVertice> aux = elementosArquivos[i].FV;
        for (j = aux.begin(); j != aux.end(); j++) {
            //glVertex3f(j->v1.x*elementosArquivos[i].escala[0], j->v1.y*elementosArquivos[i].escala[1], j->v1.z*elementosArquivos[i].escala[2]);
            //glVertex3f(j->v2.x*elementosArquivos[i].escala[0], j->v2.y*elementosArquivos[i].escala[1], j->v2.z*elementosArquivos[i].escala[2]);
            //glVertex3f(j->v3.x*elementosArquivos[i].escala[0], j->v3.y*elementosArquivos[i].escala[1], j->v3.z*elementosArquivos[i].escala[2]);
        }
    }
    glBegin(GL_TRIANGLES);
    glEnd(); // Fim do desenho
}

// função para desenhar objetos

void draw() {

    for (int i = 0; i < qtdElementosNativos; i++) {
        glColor3f(elementosNativos[i].cor[0], elementosNativos[i].cor[1], elementosNativos[i].cor[2]);
        switch (elementosNativos[i].id) {
            case TEAPOT:
                glScalef(elementosNativos[i].escala[0], elementosNativos[i].escala[1], elementosNativos[i].escala[2]);
                glutSolidTeapot(50.0f);
                break;

            case CUBE:
                glScalef(elementosNativos[i].escala[0], elementosNativos[i].escala[1], elementosNativos[i].escala[2]);
                glutSolidCube(50.0f);
                break;

            case SPHERE:
                glScalef(elementosNativos[i].escala[0], elementosNativos[i].escala[1], elementosNativos[i].escala[2]);
                glutSolidSphere(50.0, 50.0, 50.0);
                break;

            case CONE:
                glScalef(elementosNativos[i].escala[0], elementosNativos[i].escala[1], elementosNativos[i].escala[2]);
                glutSolidCone(50.0, 50.0, 50.0, 50.0);
                break;

            case TORUS:
                glScalef(elementosNativos[i].escala[0], elementosNativos[i].escala[1], elementosNativos[i].escala[2]);
                glutSolidTorus(10.0, 50.0, 50.0, 50.0);
                break;

            case ICOSAHEDRON:
                glScalef(elementosNativos[i].escala[0], elementosNativos[i].escala[1], elementosNativos[i].escala[2]);
                glutSolidIcosahedron();
                break;

            case OCTAHEDRON:
                glScalef(elementosNativos[i].escala[0], elementosNativos[i].escala[1], elementosNativos[i].escala[2]);
                glutSolidOctahedron();
                break;

            case TETRAHEDRON:
                glScalef(elementosNativos[i].escala[0], elementosNativos[i].escala[1], elementosNativos[i].escala[2]);
                glutSolidTetrahedron();
                break;

            case DODECAHEDRON:
                glScalef(elementosNativos[i].escala[0], elementosNativos[i].escala[1], elementosNativos[i].escala[2]);
                glutSolidDodecahedron();
                break;

            default:
                break;
        }
    }

    drawTriangleMesh();
}

// função callback chamada para fazer o desenho

void display(void) {
    // limpa a janela e o depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();

    // cria matrizes de transformação
    glRotatef((GLfloat) rotationX, 1.0, 0.0, 0.0);
    glRotatef((GLfloat) rotationY, 0.0, 1.0, 0.0);
    glRotatef((GLfloat) rotationZ, 0.0, 0.0, 1.0);

    glTranslatef((GLfloat) translationX, 0.0, 0.0);
    glTranslatef(0.0, (GLfloat) translationY, 0.0);

    glTranslatef((GLfloat) translationX, 0.0, 0.0);

    // desenha o objeto
    draw();

    glPopMatrix();

    glutSwapBuffers();
}

// inicializa parâmetros de rendering

void init(void) {
    GLfloat luz_ambiente[4] = {0.2, 0.2, 0.2, 1.0};
    GLfloat luz_difusa[4] = {0.7, 0.7, 0.7, 1.0}; // "cor"
    GLfloat luz_especular[4] = {1.0, 1.0, 1.0, 1.0}; // "brilho"
    GLfloat posicao_luz[4] = {0.0, 50.0, 50.0, 1.0};

    // capacidade de brilho do material
    GLfloat especularidade[4] = {1.0, 1.0, 1.0, 1.0};
    GLint espec_material = 60;

    // especifica que a cor de fundo da janela será preta
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // habilita o modelo de colorização de Gouraud
    glShadeModel(GL_SMOOTH);

    // define a refletância do material
    glMaterialfv(GL_FRONT, GL_SPECULAR, especularidade);

    // define a concentração do brilho
    glMateriali(GL_FRONT, GL_SHININESS, espec_material);

    // ativa o uso da luz ambiente
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luz_ambiente);

    // define os parâmetros da luz de número 0
    glLightfv(GL_LIGHT0, GL_AMBIENT, luz_ambiente);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luz_difusa);
    glLightfv(GL_LIGHT0, GL_SPECULAR, luz_especular);
    glLightfv(GL_LIGHT0, GL_POSITION, posicao_luz);

    // habilita a definição da cor do material a partir da cor corrente
    glEnable(GL_COLOR_MATERIAL);

    // habilita o uso de iluminação
    glEnable(GL_LIGHTING);

    // habilita a luz de número 0
    glEnable(GL_LIGHT0);

    // habilita o depth-buffering
    glEnable(GL_DEPTH_TEST);

    // habilita normalização dos vetores após as transformações
    glEnable(GL_RESCALE_NORMAL);

    angle = 45;

}

// função usada para especificar o volume de visualização

void visualization(void) {
    // especifica sistema de coordenadas de projeção
    glMatrixMode(GL_PROJECTION);

    // inicializa sistema de coordenadas de projeção
    glLoadIdentity();

    // especifica se a projeção é perspectiva ou ortográfica
    if (visualizacaoOrtografica) { // Ortográfica
        glOrtho(-200.0, 200.0, -200.0, 200.0, -200.0, 200.0);
    } else { // Perspectiva
        gluPerspective(angle, f_aspect, 0.4, 500);
        gluLookAt(0, 80, 200, 0, 0, 0, 0, 1, 0);
    }

    // especifica sistema de coordenadas do modelo
    glMatrixMode(GL_MODELVIEW);

    // inicializa sistema de coordenadas do modelo
    glLoadIdentity();
}

// função callback chamada quando o tamanho da janela é alterado

void reshape(GLsizei w, GLsizei h) {
    // para previnir uma divisão por zero
    if (h == 0) h = 1;

    // especifica o tamanho da viewport
    glViewport(0, 0, w, h);

    // calcula a correção de aspecto
    f_aspect = (GLfloat) w / (GLfloat) h;

    // especifica volume da visualização
    visualization();

    // exibe objetos na tela
    glutPostRedisplay();

}

// função callback chamada para gerenciar eventos do mouse

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON)
        if (state == GLUT_DOWN) { // Zoom-in
            if (angle >= 10) angle -= 5;
        }
    if (button == GLUT_RIGHT_BUTTON)
        if (state == GLUT_DOWN) { // Zoom-out
            if (angle <= 130) angle += 5;
        }

    // especifica volume da visualização
    visualization();

    // exibe objetos na tela
    glutPostRedisplay();
}

// teclado

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
            // rotação em torno do eixo X
        case 'x':
            rotationX = (rotationX + 5) % 360;
            break;

        case 'X':
            rotationX = (rotationX - 5) % 360;
            break;

            // rotação em torno do eixo Y
        case 'y':
            rotationY = (rotationY + 5) % 360;
            break;

        case 'Y':
            rotationY = (rotationY - 5) % 360;
            break;

            // rotação em torno do eixo Z
        case 'z':
            rotationZ = (rotationZ + 5) % 360;
            break;

        case 'Z':
            rotationZ = (rotationZ - 5) % 360;
            break;

        case 'i':
            apresentarInformacoes();
            break;

            // fecha a janela
        case 27:
            exit(0);
            break;

        default:
            break;
    }

    // especifica volume da visualização
    visualization();

    // exibe objetos na tela
    glutPostRedisplay();
}

// teclas especiais

void specialkey(int key, int x, int y) {
    switch (key) {
            // translação na direção do eixo X
        case GLUT_KEY_RIGHT:
            translationX = (translationX + 5);
            break;

        case GLUT_KEY_LEFT:
            translationX = (translationX - 5);
            break;

            // rotação na direção do eixo Y
        case GLUT_KEY_UP:
            translationY = (translationY + 5);
            break;

        case GLUT_KEY_DOWN:
            translationY = (translationY - 5);
            break;

        case 1: // F1
            apresentarInformacoes();
            break;

        case 104: // Page Up
            if (angle >= 10) angle -= 5;
            break;

        case 105: // Page Down
            if (angle <= 130) angle += 5;
            break;

        default:
            break;
    }

    // especifica volume da visualização
    visualization();

    // exibe objetos na tela
    glutPostRedisplay();
}

// programa principal

int main(int argc, char** argv) {

    // INICIAR VISUALIZAÇÃO
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Visualizacao 3D");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialkey);

    CriaMenu();
    apresentarInformacoes();

    glutMainLoop();

    return 0;
}
