#ifndef MSTALGORITHMS_H
#define MSTALGORITHMS_H

#include <QVector>
#include <QPair>
#include <tuple>
#include <queue>
#include <functional>

struct Edge {
    int u, v;
    int weight;
    Edge(int u_ = 0, int v_ = 0, int w_ = 0) : u(u_), v(v_), weight(w_) {}
    bool operator<(const Edge& other) const { return weight < other.weight; }
};

class UnionFind {
private:
    QVector<int> parent;
    QVector<int> rankArr;
public:
    explicit UnionFind(int n);
    int find(int x);
    bool unite(int x, int y);
};

class MSTGraph {
private:
    int n;
    QVector<QVector<QPair<int, int>>> adj;
    QVector<Edge> edges;
public:
    explicit MSTGraph(int n_);
    void addEdge(int u, int v, int w);
    bool updateEdgeWeight(int u, int v, int newWeight);
    int getNodeCount() const { return n; }
    int getEdgeCount() const { return edges.size(); }
    const QVector<Edge>& getEdges() const { return edges; }
    const QVector<QVector<QPair<int, int>>>& getAdj() const { return adj; }
    int addNode();
    QPair<int, QVector<Edge>> primMST();
    QPair<int, QVector<Edge>> kruskalMST();
    QVector<Edge> primMSTOrdered();
    QVector<Edge> kruskalMSTOrdered();
};

#endif
