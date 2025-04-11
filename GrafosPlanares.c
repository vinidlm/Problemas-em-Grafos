#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

const double PI = 3.14159265358979323846;

class Ponto {
    public:
        double x, y;

        Ponto() {
            this->x = 0;
            this->y = 0;
        }

        Ponto(double _x, double _y) {
            this->x = _x;
            this->y = _y;
        }

        Ponto operator-(const Ponto& outro) const {
            return Ponto(x - outro.x, y - outro.y);
        }
};

class Aresta;

class Vertice {
    public:
        int id;
        Ponto coordenada;

        std::vector<Aresta*> lista_arestas;

        Vertice() {
            this->id = 0;
            this->coordenada = Ponto();
        }

        Vertice(int _id, double _x, double _y) {
            this->id = _id;
            this->coordenada = Ponto(_x, _y);
        }
};

class Aresta {
    public:
        Vertice* vertice1;
        Vertice* vertice2;
        bool visitada;

        Aresta() {
            this->vertice1 = nullptr;
            this->vertice2 = nullptr;
            this->visitada = false;
        }

        Aresta(Vertice* _vertice1, Vertice* _vertice2) {
            this->vertice1 = _vertice1;
            this->vertice2 = _vertice2;
            this->visitada = false;
        }
};

double inclinacaoRelativa(const Ponto& p, const Ponto& q) {
    return atan2(q.y - p.y, q.x - p.x);
}

bool compararInclinacao(const Ponto& a, const Ponto& b, const Ponto& A, const Ponto& B) {
    double anguloA = inclinacaoRelativa(B, A);
    double angulo1 = inclinacaoRelativa(B, a) - anguloA;
    double angulo2 = inclinacaoRelativa(B, b) - anguloA;

    if (angulo1 < 0) angulo1 += 2 * PI;
    if (angulo2 < 0) angulo2 += 2 * PI;

    return angulo1 < angulo2;
}

void ordenarAntiHorario(Vertice* A, Vertice* B) {
    std::vector<std::pair<Ponto, Aresta*>> pontosEArestas;

    for (int i = 0; i < B->lista_arestas.size(); i++) {
        pontosEArestas.push_back(std::make_pair(B->lista_arestas[i]->vertice2->coordenada, B->lista_arestas[i]));
    }

    std::sort(pontosEArestas.begin(), pontosEArestas.end(), [&](const std::pair<Ponto, Aresta*>& a, const std::pair<Ponto, Aresta*>& b) {
        return compararInclinacao(a.first, b.first, A->coordenada, B->coordenada);
    });

    for (int i = 0; i < B->lista_arestas.size(); i++) {
        B->lista_arestas[i] = pontosEArestas[i].second;
    }
}

int main() {
    std::vector<Vertice*> vertices;
    std::vector<std::vector<int>> listasAdjacencia;

    int n, m;
    std::cin >> n >> m;
    listasAdjacencia.resize(n);
    for (int i = 0; i < n; i++) {
        double x, y;
        int grau;
        std::cin >> x >> y >> grau;
        vertices.push_back(new Vertice(i + 1, x, y));

        for (int j = 0; j < grau; j++) {
            int adjacente;
            std::cin >> adjacente;
            listasAdjacencia[i].push_back(adjacente - 1);
        }
    }

    std::vector<Aresta*> arestas;

    for (int i = 0; i < vertices.size(); i++) {
        for (int j = 0; j < listasAdjacencia[i].size(); j++) {
            Aresta* nova = (new Aresta(vertices[i], vertices[listasAdjacencia[i][j]]));
            arestas.push_back(nova);
            vertices[i]->lista_arestas.push_back(nova);
        }
    }

    std::vector<std::vector<Vertice*>> faces;

    Aresta* inicial = arestas[0];

    while (inicial->visitada != true) {
        Aresta* atual = inicial;
        std::vector<Vertice*> face;

        face.push_back(inicial->vertice1);
        while (atual->visitada != true) {
            face.push_back(atual->vertice2);
            atual->visitada = true;

            ordenarAntiHorario(atual->vertice1, atual->vertice2);

            for (int j = 0; j < atual->vertice2->lista_arestas.size(); j++) {
                if (atual->vertice2->lista_arestas[j]->vertice2 != atual->vertice1) {
                    atual = atual->vertice2->lista_arestas[j];
                    break;
                } else if (atual->vertice2->lista_arestas.size() < 2) {
                    atual = atual->vertice2->lista_arestas[j];
                    break;
                }
            }
        }

        faces.push_back(face);

        for (Aresta* aresta : arestas) {
            if (!aresta->visitada) {
                inicial = aresta;
                break;
            }
        }
    }

    if (faces.size() > 0) {
        std::cout << faces.size() << '\n';
        for (int i = 0; i < faces.size(); i++) {
            if (faces[i].size() > 0) {
                std::cout << faces[i].size() << ' ';
                for (int j = 0; j < faces[i].size(); j++) {
                    std::cout << faces[i][j]->id << " ";
                }
                std::cout << std::endl;
            }
        }
    }

    for (int i = 0; i < arestas.size(); i++) {
        delete arestas[i];
    }

    for (int i = 0; i < vertices.size(); i++) {
        delete vertices[i];
    }

    return 0;
}