#include <iostream>
#include <list>
#include <cstring>

using namespace std;


class Vertice {
public:
    float x, y, z;
};

class FaceInt {
public:
    int x, y, z;
};

class FaceVertice {
public:
    FaceInt v, u, n;
};

list<Vertice> vertices;
list<Vertice>::iterator verticesIt;
list<FaceInt> faceInt;
list<FaceInt>::iterator faceIntIt;
list<FaceVertice> FV;
list<FaceVertice>::iterator FVIt;

void lerArquivo(char nome[40]) {
    FILE *entrada = NULL;
    Vertice v;
    FaceInt f;
    FaceVertice fv;
    int cont = 1, face, contAux;
    float p1, p2, p3, num;
    char palavra[2], fim, buf[50];
    bool achou1, achou2, achou3;

    if ((entrada = fopen(nome, "r")) == NULL) {
        cout << "Arquivo de entrada \"" << nome << "\" não pode ser aberto!" << endl;
        return;
    } else {
        cout << "Arquivo de entrada \"" << nome << "\" aberto com sucesso!" << endl;
    }

    while (fgets(palavra, 2, entrada) != NULL && !feof(entrada)) {
        if (palavra[0] == 'v') {
            fscanf(entrada, "%f %f %f", &v.x, &v.y, &v.z);
            vertices.push_back(v);
            //cout << "v " << v.x << " " << v.y << " " << v.z << endl;
        } else if (palavra[0] == 'f') {
            std::string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf(entrada, "%d/%d/%d %d/%d/%d %d/%d/%d", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );

            fv.v.x = vertexIndex[0];
            fv.v.y = vertexIndex[1];
            fv.v.z = vertexIndex[2];

            fv.u.x = uvIndex[0];
            fv.u.y = uvIndex[1];
            fv.u.z = uvIndex[2];

            fv.n.x = normalIndex[0];
            fv.n.y = normalIndex[1];
            fv.n.z = normalIndex[2];

            FV.push_back(fv);
            cout << "f " << fv.v.x << " " << fv.v.y << " " << fv.v.z << endl;
        }
    }

    cout << "Fim da leitura do arquivo \"" << nome << "\"!" << endl;

    fclose(entrada);
}

void gravarArquivo(char nome[40]) {
    FILE *saida = NULL;

    if ((saida = fopen(nome, "w")) == NULL) {
        cout << "Arquivo de saída não pode ser aberto!" << endl;
    } else {
        cout << "Arquivo de saída aberto com sucesso!" << endl;
    }

    for (verticesIt = vertices.begin(); verticesIt != vertices.end(); verticesIt++) {
        fprintf(saida, "v %f %f %f\n", verticesIt->x, verticesIt->y, verticesIt->z);
    }

    fclose(saida);
    cout << "Fim da gravação!" << endl;
}
