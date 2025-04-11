# Soluções para Problemas em Grafos

Este repositório contém duas implementações em C++ abordando problemas clássicos de grafos com diferentes enfoques:

- **Identificação de Faces em Grafos Planares**  
  Implementa a identificação e ordenação de faces em um grafo bidimensional. Essa solução utiliza conceitos de geometria computacional e manipulação de estruturas de dados (pontos, vértices e arestas) para reconstruir ciclos (faces) do grafo, ordenando as arestas em sentido anti-horário.

- **Análise de Grafos Temporais**  
  Foca na resolução de problemas de conectividade em redes com atributos temporais e de custo. O código integra algoritmos clássicos, como Dijkstra (para caminhos mínimos) e variações do algoritmo de Kruskal (para obtenção de árvores geradoras mínimas), permitindo responder questões como:
  - Determinar a menor distância (em termos de tempo) a partir de um vértice de origem;
  - Identificar o primeiro ano em que todas as localidades ficam acessíveis;
  - Calcular o menor custo para garantir a conectividade total da rede.

---

## Descrição dos Problemas e Metodologia

### Grafos Planares

#### Problema
O objetivo é identificar as faces (regiões ou ciclos) de um grafo planar composto por vértices com coordenadas no plano e suas respectivas conexões. A aplicação é especialmente útil em áreas como computação gráfica, SIG e redes de transporte.

#### Metodologia e Implementação
- **Estruturas de Dados:**
  - **Ponto:** Representa as coordenadas (x, y) de cada vértice.
  - **Vértice:** Armazena um identificador único, a coordenada associada e uma lista de arestas adjacentes.
  - **Aresta:** Conecta dois vértices e possui um atributo para marcar se já foi visitada durante a identificação dos ciclos.
- **Funções Principais:**
  - **inclinacaoRelativa:** Calcula a orientação entre dois pontos usando a função `atan2`, fundamental para ordenar as arestas.
  - **compararInclinacao:** Compara a orientação entre duas arestas em relação a um referencial fixo.
  - **ordenarAntiHorario:** Organiza as arestas de um vértice em ordem anti-horária, permitindo a correta identificação das faces.
  - **Reconstrução das Faces:** O algoritmo percorre as arestas não visitadas, formando ciclos que representam cada face do grafo.
- **Complexidade:**  
  O custo envolve a leitura e construção do grafo (O(n + m)), a ordenação local das arestas (dependente do grau dos vértices) e a identificação dos ciclos (O(m)). No pior caso, o algoritmo opera em O(m log d), onde _d_ é o grau de um vértice.

---

### Grafos Temporais

#### Problema
O desafio é analisar uma rede de transporte temporal, onde cada aresta possui atributos como ano, tempo de travessia e custo. A solução busca:
- Calcular os caminhos mínimos em termos de tempo a partir de um ponto de origem (usando Dijkstra);
- Determinar o primeiro ano em que todas as localidades estão conectadas (usando uma variação do algoritmo de Kruskal);
- Obter o menor custo para garantir a conectividade total da rede (usando um algoritmo de custo mínimo baseado em Kruskal).

#### Metodologia e Implementação
- **Estruturas de Dados:**
  - **Vértice:** Armazena identificador, distância, vértice pai, e uma lista de arestas conectadas. Atributos auxiliares (como visitado) são usados para o processamento pelo algoritmo.
  - **Aresta:** Registra o ano, tempo e custo além de referenciar os dois vértices que conecta.
  - **Disjunto (Union-Find):** Gerencia a união e a busca de conjuntos de vértices, evitando ciclos durante a construção da árvore geradora mínima.
- **Algoritmos Empregados:**
  - **Dijkstra:** Encontra o caminho mínimo a partir de um vértice de origem utilizando uma fila de prioridades (min-heap), atualizando as distâncias e reconstruindo o caminho para análise do ano máximo envolvido.
  - **Kruskal e Variações:**  
    - **Kruskal padrão:** Ordena as arestas pelo custo para construir a árvore geradora mínima.
    - **Kruskal Modificado:** Ordena as arestas com base no ano para determinar o primeiro momento em que toda a rede se torna conectada.
- **Integração:**  
  A combinação dos algoritmos permite uma análise completa da rede, respondendo à conectividade temporal, ao custo e às rotas mínimas, aspectos críticos para a modelagem de redes de transporte.

---

## Instruções para Compilação e Execução

Ambos os códigos foram escritos em C++ e podem ser compilados e executados em sistemas que possuam um compilador compatível (como o `g++`).

### Pré-requisitos
- Compilador C++ (ex.: `g++`)
- Ambiente Linux, Windows ou macOS com suporte para linha de comando

### Compilando os Códigos

Abra o terminal ou prompt de comando e navegue até o diretório do repositório. Em seguida, utilize os seguintes comandos para compilar:

#### Grafos Planares
O programa espera a entrada com a seguinte estrutura:

Número de vértices e número de conexões.

Para cada vértice: coordenadas (x e y), grau (número de adjacências) e os identificadores dos vértices adjacentes.

Após processar a entrada, o programa exibirá o número de faces identificadas, seguido da lista de vértices que compõem cada face.

Comando para compilar:
g++ GrafosPlanares.c -o grafos_planares

### Grafos Temporais

O programa espera os dados na seguinte ordem:

Quantidade de vértices e arestas.

Para cada aresta: identificadores dos vértices conectados, ano, tempo de travessia e custo.

O programa realiza as seguintes operações:

Executa o algoritmo de Dijkstra para calcular as menores distâncias (em tempo) a partir do vértice de origem e exibe essas distâncias.

Reconstrói o caminho mínimo para determinar o maior ano envolvido nesse percurso.

Calcula, utilizando variações do algoritmo de Kruskal, tanto o primeiro ano em que a rede se torna totalmente conectada quanto o custo total mínimo para essa conectividade.

Os resultados são impressos na saída padrão conforme a execução dos algoritmos.



