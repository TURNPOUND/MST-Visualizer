#ifndef GRAPHVIEW_H
#define GRAPHVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QTimer>
#include <QVector>
#include <QMap>
#include <QPointF>
#include "mstalgorithms.h"

class NodeItem : public QGraphicsEllipseItem {
public:
    int nodeId;
    QString name;
    QGraphicsTextItem* nameLabel;

    NodeItem(int id, qreal x, qreal y, qreal r);
    void setNodeName(const QString& newName);
};

class EdgeItem : public QGraphicsLineItem {
public:
    int u, v;
    int weight;
    EdgeItem(qreal x1, qreal y1, qreal x2, qreal y2, int u_, int v_, int w);
    QPainterPath shape() const override;
};

class GraphView : public QGraphicsView {
    Q_OBJECT
public:
    enum Mode { PanMode, AddNodeMode, AddEdgeMode };

    explicit GraphView(QWidget *parent = nullptr);

    void setGraph(MSTGraph* graph);
    void clearGraph();
    void highlightMST(const QVector<Edge>& mstEdges);
    void resetHighlight();
    void generateRandomLayout(int n);
    void generateWeightedLayout(int n, const MSTGraph* g);
    void fitView();
    void setShowWeights(bool show);
    void setWeightedLayoutEnabled(bool enabled);
    void setMode(Mode newMode);
    Mode currentMode() const { return mode; }

    // 查找功能
    void highlightNodes(const QVector<int>& nodeIds);
    void clearSearchHighlight();
    void centerOnNode(int nodeId);
    const QVector<NodeItem*>& getNodes() const { return nodes; }

    // 算法动画
    void animateMST(const QVector<Edge>& orderedEdges, const QString& algoName, int totalWeight);
    void stopAnimation();

signals:
    void nodeSelected(int nodeId);
    void statusMessage(const QString& msg);
    void logMessage(const QString& msg);
    void graphChanged();
    void animationDone();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    QGraphicsScene* scene;
    MSTGraph* currentGraph;
    QVector<NodeItem*> nodes;
    QVector<EdgeItem*> edges;
    QVector<QGraphicsTextItem*> weightLabels;
    QMap<int, QPointF> nodePositions;
    qreal nodeRadius;
    bool showWeights;
    bool weightedLayout;
    Mode mode;
    int pendingSourceNode;

    // 查找高亮
    QSet<int> searchHighlightIds;

    // 动画状态
    QTimer* animTimer;
    QVector<Edge> animOrderedEdges;
    int animIndex;
    int animTotalWeight;
    QString animAlgoName;
    QSet<QPair<int, int>> animHighlighted;

    void drawGraph();
    NodeItem* nodeAtPos(const QPointF& scenePos);
    EdgeItem* edgeAtPos(const QPointF& scenePos);
    void onAnimationStep();
    void clearAnimHighlight();
};

#endif
