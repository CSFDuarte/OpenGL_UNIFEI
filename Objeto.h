#include <iostream>
#include <list>

using namespace std;

struct ObjetoNativo {
    int id, translationX, translationY, rotationX, rotationY, rotationZ;
    float cor[3], escala[3];
};

struct ObjetoArquivo {
    float cor[3], escala[3];
    list<Vertice> vertices;
    list<Vertice>::iterator verticesIt;
    list<FaceInt> faceInt;
    list<FaceInt>::iterator faceIntIt;
    list<FaceVertice> FV;
    list<FaceVertice>::iterator FVIt;
};