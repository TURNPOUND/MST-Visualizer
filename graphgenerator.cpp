#include "graphgenerator.h"
#include <QRandomGenerator>
#include <QSet>

MSTGraph* GraphGenerator::generateConnectedGraph(int n, double density) {
    MSTGraph* g = new MSTGraph(n);

    // 步骤1: 生成随机生成树保证连通性
    QVector<int> nodes(n);
    for (int i = 0; i < n; ++i) nodes[i] = i;

    // 随机打乱节点顺序
    for (int i = n - 1; i > 0; --i) {
        int j = QRandomGenerator::global()->bounded(i + 1);
        std::swap(nodes[i], nodes[j]);
    }

    // 构建生成树
    for (int i = 1; i < n; ++i) {
        int parentIdx = QRandomGenerator::global()->bounded(i);
        int w = QRandomGenerator::global()->bounded(1, 101);
        g->addEdge(nodes[parentIdx], nodes[i], w);
    }

    // 步骤2: 按密度添加额外边
    int maxEdges = n * (n - 1) / 2;
    int targetEdges = qMax(n - 1, (int)(maxEdges * density));
    QSet<QPair<int, int>> added;

    // 记录已有边
    for (const auto& e : g->getEdges()) {
        int u = e.u, v = e.v;
        if (u > v) std::swap(u, v);
        added.insert({u, v});
    }

    while (g->getEdgeCount() < targetEdges) {
        int u = QRandomGenerator::global()->bounded(n);
        int v = QRandomGenerator::global()->bounded(n);

        if (u == v) continue;
        if (u > v) std::swap(u, v);

        if (added.contains({u, v})) continue;

        added.insert({u, v});
        int w = QRandomGenerator::global()->bounded(1, 101);
        g->addEdge(u, v, w);
    }

    return g;
}