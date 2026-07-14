#include "mstalgorithms.h"
#include <QSet>
#include <algorithm>

UnionFind::UnionFind(int n) {
    parent.resize(n);
    rankArr.resize(n, 0);
    for (int i = 0; i < n; ++i) parent[i] = i;
}

int UnionFind::find(int x) {
    if (parent[x] != x) parent[x] = find(parent[x]);
    return parent[x];
}

bool UnionFind::unite(int x, int y) {
    int px = find(x), py = find(y);
    if (px == py) return false;
    if (rankArr[px] < rankArr[py]) { int tmp = px; px = py; py = tmp; }
    parent[py] = px;
    if (rankArr[px] == rankArr[py]) rankArr[px]++;
    return true;
}

MSTGraph::MSTGraph(int n_) : n(n_) { adj.resize(n); }

void MSTGraph::addEdge(int u, int v, int w) {
    int maxNode = qMax(u, v);
    while (adj.size() <= maxNode) adj.append(QVector<QPair<int, int>>());
    if (maxNode >= n) n = maxNode + 1;

    adj[u].append(qMakePair(v, w));
    adj[v].append(qMakePair(u, w));
    edges.append(Edge(u, v, w));
}

bool MSTGraph::updateEdgeWeight(int u, int v, int newWeight) {
    // 更新 edges 列表
    bool found = false;
    for (auto& e : edges) {
        if ((e.u == u && e.v == v) || (e.u == v && e.v == u)) {
            e.weight = newWeight;
            found = true;
            break;
        }
    }
    if (!found) return false;

    // 更新邻接表
    for (auto& p : adj[u]) {
        if (p.first == v) { p.second = newWeight; break; }
    }
    for (auto& p : adj[v]) {
        if (p.first == u) { p.second = newWeight; break; }
    }
    return true;
}

int MSTGraph::addNode() {
    adj.append(QVector<QPair<int, int>>());
    n++;
    return n - 1;
}

QPair<int, QVector<Edge>> MSTGraph::primMST() {
    QVector<bool> visited(n, false);
    std::priority_queue<
        std::tuple<int, int, int>,
        QVector<std::tuple<int, int, int>>,
        std::greater<std::tuple<int, int, int>>
        > heap;
    QVector<Edge> mstEdges;
    int totalWeight = 0;
    visited[0] = true;
    for (int i = 0; i < adj[0].size(); ++i) {
        int v = adj[0][i].first;
        int w = adj[0][i].second;
        heap.emplace(w, 0, v);
    }
    while (!heap.empty() && mstEdges.size() < n - 1) {
        auto [w, u, v] = heap.top();
        heap.pop();
        if (visited[v]) continue;
        visited[v] = true;
        mstEdges.append(Edge(u, v, w));
        totalWeight += w;
        for (int i = 0; i < adj[v].size(); ++i) {
            int nv = adj[v][i].first;
            int nw = adj[v][i].second;
            if (!visited[nv]) heap.emplace(nw, v, nv);
        }
    }
    return qMakePair(totalWeight, mstEdges);
}

QPair<int, QVector<Edge>> MSTGraph::kruskalMST() {
    UnionFind uf(n);
    QVector<Edge> sortedEdges = edges;
    std::sort(sortedEdges.begin(), sortedEdges.end());
    QVector<Edge> mstEdges;
    int totalWeight = 0;
    for (int i = 0; i < sortedEdges.size(); ++i) {
        const Edge& e = sortedEdges[i];
        if (uf.unite(e.u, e.v)) {
            mstEdges.append(e);
            totalWeight += e.weight;
            if (mstEdges.size() == n - 1) break;
        }
    }
    return qMakePair(totalWeight, mstEdges);
}

QVector<Edge> MSTGraph::primMSTOrdered() {
    QVector<bool> visited(n, false);
    std::priority_queue<
        std::tuple<int, int, int>,
        QVector<std::tuple<int, int, int>>,
        std::greater<std::tuple<int, int, int>>
        > heap;
    QVector<Edge> orderedEdges;
    visited[0] = true;
    for (int i = 0; i < adj[0].size(); ++i) {
        int v = adj[0][i].first;
        int w = adj[0][i].second;
        heap.emplace(w, 0, v);
    }
    while (!heap.empty() && orderedEdges.size() < n - 1) {
        auto [w, u, v] = heap.top();
        heap.pop();
        if (visited[v]) continue;
        visited[v] = true;
        orderedEdges.append(Edge(u, v, w));
        for (int i = 0; i < adj[v].size(); ++i) {
            int nv = adj[v][i].first;
            int nw = adj[v][i].second;
            if (!visited[nv]) heap.emplace(nw, v, nv);
        }
    }
    return orderedEdges;
}

QVector<Edge> MSTGraph::kruskalMSTOrdered() {
    UnionFind uf(n);
    QVector<Edge> sortedEdges = edges;
    std::sort(sortedEdges.begin(), sortedEdges.end());
    QVector<Edge> orderedEdges;
    for (int i = 0; i < sortedEdges.size(); ++i) {
        const Edge& e = sortedEdges[i];
        if (uf.unite(e.u, e.v)) {
            orderedEdges.append(e);
            if (orderedEdges.size() == n - 1) break;
        }
    }
    return orderedEdges;
}
