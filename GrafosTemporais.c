#include <iostream>
#include <map>
#include <unordered_map>
#include <cmath>
#include <vector>
#include <stack>
#include <functional>
#include <algorithm>
#include <queue>
#include <limits>
#include <climits>

static const long long DIST_MAX = LLONG_MAX;

class Vertice;

class Aresta {
public:
    Vertice* v1;
    Vertice* v2;
    bool perc;
    long long ano;
    long long tempo;
    long long custo;

    Aresta() : v1(nullptr), v2(nullptr), perc(false), ano(0), tempo(0), custo(0) {}

    Aresta(Vertice* _v1, Vertice* _v2, long long _ano, long long _tempo, long long _custo)
        : v1(_v1), v2(_v2), ano(_ano), tempo(_tempo), custo(_custo), perc(false) {}
};

class Vertice {
public:
    long long visitado;
    long long dist;
    long long id;
    Vertice* pai;
    std::vector<Aresta*> listArestas;

    Vertice(long long _id) : id(_id), dist(DIST_MAX), pai(nullptr), visitado(false) {}

    struct ComparaDist {
        bool operator()(const Vertice* v1, const Vertice* v2) const {
            return v1->dist > v2->dist;
        }
    };
};

class Disjunto {
public:
    std::vector<int> raiz;
    std::vector<int> ranq;

    Disjunto(int n) : raiz(n), ranq(n, 0) {
        for (int i = 0; i < n; ++i) {
            raiz[i] = i;
        }
    }

    int encontrar(int u) {
        if (raiz[u] != u) {
            raiz[u] = encontrar(raiz[u]);
        }
        return raiz[u];
    }

    void une(int x, int y) {
        x = encontrar(x);
        y = encontrar(y);

        if (ranq[x] > ranq[y]) {
            raiz[y] = x;
        } else {
            raiz[x] = y;
            if (ranq[x] == ranq[y]) {
                ranq[y]++;
            }
        }
    }
};

const int MAX_VERTICES = 10000;
Aresta* matrizArestas[MAX_VERTICES][MAX_VERTICES] = {nullptr}; // Matriz para armazenar arestas

void addAresta(Aresta* aresta) {
    int id1 = aresta->v1->id;
    int id2 = aresta->v2->id;
    matrizArestas[id1][id2] = aresta;
    matrizArestas[id2][id1] = aresta; // Se o grafo for não direcionado
}

void Custo_Min(std::vector<Vertice*>& vertices, std::vector<Aresta*> arestas, std::vector<Aresta*>& AGM) {
    std::sort(arestas.begin(), arestas.end(), [](const Aresta* a, const Aresta* b) {
        return a->custo < b->custo;
    });

    Disjunto conjuntoDisjunto(vertices.size());

    for (const Aresta* aresta : arestas) {
        int idVerticeU = aresta->v1->id;
        int idVerticeV = aresta->v2->id;

        int raizU = conjuntoDisjunto.encontrar(idVerticeU);
        int raizV = conjuntoDisjunto.encontrar(idVerticeV);

        if (raizU != raizV) {
            AGM.push_back(const_cast<Aresta*>(aresta));
            conjuntoDisjunto.une(raizU, raizV);
        }
    }
}

void ano_K(std::vector<Vertice*>& vertices, std::vector<Aresta*> arestas, std::vector<Aresta*>& AGM) {
    // Cria uma cópia das arestas e as ordena com base no ano
    std::vector<Aresta*> arestasPorAno = arestas;
    std::sort(arestasPorAno.begin(), arestasPorAno.end(), [](const Aresta* a, const Aresta* b) {
        return a->ano < b->ano;
    });

    // Inicializa a estrutura de conjuntos disjuntos para o gerenciamento de componentes
    Disjunto estruturaDisjunto(vertices.size());

    // Itera sobre as arestas ordenadas
    for (const Aresta* aresta : arestasPorAno) {
        int idVerticeA = aresta->v1->id;
        int idVerticeB = aresta->v2->id;

        int raizA = estruturaDisjunto.encontrar(idVerticeA);
        int raizB = estruturaDisjunto.encontrar(idVerticeB);

        // Se os vértices têm raízes diferentes, adiciona a aresta à árvore geradora mínima e une os conjuntos
        if (raizA != raizB) {
            AGM.push_back(const_cast<Aresta*>(aresta));
            estruturaDisjunto.une(raizA, raizB);
        }
    }
}

void dijkstra(Vertice* src) {
    src->dist = 0;
    std::priority_queue<Vertice*, std::vector<Vertice*>, Vertice::ComparaDist> prioritaria;
    prioritaria.push(src);

    while (!prioritaria.empty()) {
        Vertice* top = prioritaria.top();
        prioritaria.pop();
        top->visitado = true;

        for (Aresta* aresta : top->listArestas) {
            Vertice* s = aresta->v2;
            if (top->dist + aresta->tempo < s->dist) {
                s->dist = top->dist + aresta->tempo;
                s->pai = top;
                prioritaria.push(s);
            }
        }
    }
}

int main() {
    // Inicializa vetores para vértices e arestas
    std::vector<Vertice*> listaVertices;
    std::vector<Aresta*> listaArestas;

    // Leitura da quantidade de vértices e arestas
    long long numVertices, numArestas;
    std::cin >> numVertices >> numArestas;

    // Reserva espaço para os vértices
    listaVertices.reserve(numVertices);

    // Criação e adição dos vértices
    for (long long i = 0; i < numVertices; ++i) {
        listaVertices.push_back(new Vertice(i));
    }

    // Leitura das arestas e construção do grafo
    for (long long i = 0; i < numArestas; ++i) {
        long long v1, v2, ano, tempo, custo;
        std::cin >> v1 >> v2 >> ano >> tempo >> custo;

        // Criação das arestas e adição aos vértices
        Aresta* aresta = new Aresta(listaVertices[v1 - 1], listaVertices[v2 - 1], ano, tempo, custo);
        listaVertices[v1 - 1]->listArestas.push_back(aresta);
        listaVertices[v2 - 1]->listArestas.push_back(new Aresta(listaVertices[v2 - 1], listaVertices[v1 - 1], ano, tempo, custo));
        addAresta(aresta);

        listaArestas.push_back(aresta);
    }

    // Executa o algoritmo de Dijkstra
    dijkstra(listaVertices[0]);

    // Imprime as distâncias dos vértices
    for (const Vertice* vertice : listaVertices) {
        std::cout << vertice->dist << '\n';
    }

    // Reconstroi o caminho mínimo e calcula o ano máximo
    std::vector<Aresta*> caminhoMinimoArestas;
    for (Vertice* verticeAtual : listaVertices) {
        Vertice* vertice = verticeAtual;
        while (vertice != listaVertices[0]) {
            for (const Aresta* aresta : vertice->pai->listArestas) {
                if (aresta->v2 == vertice) {
                    caminhoMinimoArestas.push_back(const_cast<Aresta*>(aresta));
                }
            }
            vertice = vertice->pai;
        }
    }

    long long anoMaximo = 0;
    for (const Aresta* aresta : caminhoMinimoArestas) {
        anoMaximo = std::max(anoMaximo, aresta->ano);
    }
    std::cout << anoMaximo << '\n';

    // Resetando os atributos dos vértices e arestas
    for (Vertice* vertice : listaVertices) {
        vertice->dist = DIST_MAX;
        vertice->pai = nullptr;
        vertice->visitado = false;
    }
    for (Aresta* aresta : listaArestas) {
        aresta->perc = false;
    }

    // Calcula a Árvore Geradora Mínima usando o algoritmo de Kruskal
    std::vector<Aresta*> AGM_Kruskal;
    ano_K(listaVertices, listaArestas, AGM_Kruskal);

    // Calcula o ano máximo na Árvore Geradora Mínima de Kruskal
    long long anoMaximoKruskal = 0;
    for (const Aresta* aresta : AGM_Kruskal) {
        anoMaximoKruskal = std::max(anoMaximoKruskal, aresta->ano);
    }
    std::cout << anoMaximoKruskal << '\n';

    // Resetando os atributos novamente
    for (Vertice* vertice : listaVertices) {
        vertice->dist = DIST_MAX;
        vertice->pai = nullptr;
        vertice->visitado = false;
    }
    for (Aresta* aresta : listaArestas) {
        aresta->perc = false;
    }

    // Calcula a Árvore Geradora Mínima usando o algoritmo de Custo Mínimo
    std::vector<Aresta*> AGM_CustoMinimo;
    Custo_Min(listaVertices, listaArestas, AGM_CustoMinimo);

    // Calcula o custo total da Árvore Geradora Mínima de Custo Mínimo
    long long custoTotal = 0;
    for (const Aresta* aresta : AGM_CustoMinimo) {
        custoTotal += aresta->custo;
    }
    std::cout << custoTotal << '\n';

        for (Vertice* vertice : listaVertices) {
        delete vertice;
    }
    for (Aresta* aresta : listaArestas) {
        delete aresta;
    }

    return 0;
}