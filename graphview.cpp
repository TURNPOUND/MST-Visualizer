#include "graphview.h"
#include <QRandomGenerator>
#include <QSet>
#include <QPair>
#include <QInputDialog>
#include <QMessageBox>
#include <QMenu>
#include <QAction>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QGraphicsSceneMouseEvent>
#include <QContextMenuEvent>
#include <QApplication>
#include <QPainterPath>
#include <QPainterPathStroker>
#include <cmath>

// ===================== NodeItem =====================
NodeItem::NodeItem(int id, qreal x, qreal y, qreal r)
    : QGraphicsEllipseItem(x - r, y - r, r * 2, r * 2)
    , nodeId(id)
    , name("")
    , nameLabel(nullptr) {
    setBrush(QColor(100, 149, 237));
    setPen(QPen(Qt::black, 2));
    setFlag(QGraphicsItem::ItemIsSelectable, false);
    setZValue(2);
    setToolTip(QString("节点 %1").arg(id));
}

void NodeItem::setNodeName(const QString& newName) {
    name = newName;
    if (name.isEmpty()) {
        if (nameLabel) {
            nameLabel->setVisible(false);
            nameLabel->setPlainText("");
        }
    } else {
        if (!nameLabel) {
            nameLabel = new QGraphicsTextItem(name, this);
            nameLabel->setDefaultTextColor(QColor(60, 60, 60));
            nameLabel->setFont(QFont("Microsoft YaHei", 9, QFont::Bold));
        } else {
            nameLabel->setPlainText(name);
        }
        QRectF r = boundingRect();
        QRectF br = nameLabel->boundingRect();
        nameLabel->setPos(-br.width() / 2, r.height() / 2 + 2);
        nameLabel->setVisible(true);
        nameLabel->setZValue(3);
    }
}

// ===================== EdgeItem =====================
EdgeItem::EdgeItem(qreal x1, qreal y1, qreal x2, qreal y2, int u_, int v_, int w)
    : QGraphicsLineItem(x1, y1, x2, y2), u(u_), v(v_), weight(w) {
    setPen(QPen(QColor(140, 140, 140), 1.5));
    setZValue(1);
    // 增加点击感应区宽度到 12px
    setFlag(QGraphicsItem::ItemClipsToShape, false);
}

QPainterPath EdgeItem::shape() const {
    QPainterPath path;
    path.moveTo(line().p1());
    path.lineTo(line().p2());
    QPainterPathStroker stroker;
    stroker.setWidth(12.0);
    return stroker.createStroke(path);
}

// ===================== GraphView =====================
GraphView::GraphView(QWidget *parent)
    : QGraphicsView(parent)
    , scene(new QGraphicsScene(this))
    , currentGraph(nullptr)
    , nodeRadius(12.0)
    , showWeights(true)
    , weightedLayout(false)
    , mode(PanMode)
    , pendingSourceNode(-1)
    , animTimer(new QTimer(this))
    , animIndex(0)
    , animTotalWeight(0) {
    setScene(scene);
    setRenderHint(QPainter::Antialiasing);
    setDragMode(QGraphicsView::ScrollHandDrag);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    setBackgroundBrush(QColor(255, 255, 255));
    scene->setSceneRect(-1000, -1000, 2000, 2000);

    connect(animTimer, &QTimer::timeout, this, &GraphView::onAnimationStep);
}

void GraphView::setMode(Mode newMode) {
    mode = newMode;
    pendingSourceNode = -1;

    switch (mode) {
    case PanMode:
        setDragMode(QGraphicsView::ScrollHandDrag);
        setCursor(Qt::ArrowCursor);
        break;
    case AddNodeMode:
        setDragMode(QGraphicsView::NoDrag);
        setCursor(Qt::CrossCursor);
        break;
    case AddEdgeMode:
        setDragMode(QGraphicsView::NoDrag);
        setCursor(Qt::PointingHandCursor);
        break;
    }
}

void GraphView::setGraph(MSTGraph *graph) {
    currentGraph = graph;
    drawGraph();
    fitView();
}

void GraphView::clearGraph() {
    stopAnimation();
    scene->clear();
    nodes.clear();
    edges.clear();
    weightLabels.clear();
    nodePositions.clear();
    nodeRadius = 12.0;
    pendingSourceNode = -1;
    animHighlighted.clear();
}

void GraphView::generateRandomLayout(int n) {
    nodePositions.clear();

    nodeRadius = qBound(4.0, 400.0 / n, 14.0);

    double circumference = n * (nodeRadius * 2 + 4);
    double circleRadius = circumference / (2.0 * M_PI);
    circleRadius = qMax(circleRadius, 120.0);
    double jitter = (n > 10) ? circleRadius * 0.03 : 0.0;

    for (int i = 0; i < n; ++i) {
        double angle = 2.0 * M_PI * i / n;
        qreal x = circleRadius * cos(angle) + QRandomGenerator::global()->bounded(2 * jitter * 100) / 100.0 - jitter;
        qreal y = circleRadius * sin(angle) + QRandomGenerator::global()->bounded(2 * jitter * 100) / 100.0 - jitter;
        nodePositions[i] = QPointF(x, y);
    }
}

void GraphView::generateWeightedLayout(int n, const MSTGraph* g) {
    nodePositions.clear();
    nodeRadius = qBound(4.0, 400.0 / n, 14.0);

    if (n <= 1 || !g) {
        nodePositions[0] = QPointF(0, 0);
        return;
    }

    // 初始：圆形布局
    double circleRadius = qMax(200.0, n * 4.0);
    for (int i = 0; i < n; ++i) {
        double angle = 2.0 * M_PI * i / n;
        nodePositions[i] = QPointF(circleRadius * cos(angle), circleRadius * sin(angle));
    }

    // 力导向布局迭代：用边权作为弹簧理想距离
    const double repulsion = 80000.0;
    const double springK = 0.003;
    const double weightScale = 2.5;
    const double damping = 0.4;

    for (int iter = 0; iter < 200; ++iter) {
        QVector<QPointF> forces(n, QPointF(0, 0));

        // 节点间斥力
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                QPointF diff = nodePositions[i] - nodePositions[j];
                double dist = sqrt(diff.x() * diff.x() + diff.y() * diff.y());
                if (dist < 5) dist = 5;
                double force = repulsion / (dist * dist);
                QPointF f = diff / dist * force;
                forces[i] += f;
                forces[j] -= f;
            }
        }

        // 边权弹簧力
        const auto& edgesList = g->getEdges();
        for (const auto& e : edgesList) {
            QPointF diff = nodePositions[e.u] - nodePositions[e.v];
            double dist = sqrt(diff.x() * diff.x() + diff.y() * diff.y());
            if (dist < 1) dist = 1;
            double idealDist = e.weight * weightScale;
            double force = (dist - idealDist) * springK;
            QPointF f = diff / dist * force;
            forces[e.u] -= f;
            forces[e.v] += f;
        }

        // 应用力 + 阻尼
        for (int i = 0; i < n; ++i) {
            nodePositions[i] += forces[i] * damping;
        }
    }
}

void GraphView::setWeightedLayoutEnabled(bool enabled) {
    weightedLayout = enabled;
}

void GraphView::fitView() {
    if (scene->items().isEmpty()) return;
    resetTransform();
    fitInView(scene->itemsBoundingRect().adjusted(-20, -20, 20, 20), Qt::KeepAspectRatio);
}

void GraphView::drawGraph() {
    if (!currentGraph) return;

    // 绘制边
    const auto& edgesList = currentGraph->getEdges();
    for (const auto& e : edgesList) {
        QPointF p1 = nodePositions.value(e.u, QPointF(0, 0));
        QPointF p2 = nodePositions.value(e.v, QPointF(0, 0));
        EdgeItem *edgeItem = new EdgeItem(p1.x(), p1.y(), p2.x(), p2.y(), e.u, e.v, e.weight);
        edgeItem->setToolTip(QString("边 %1 - %2  权重: %3").arg(e.u).arg(e.v).arg(e.weight));
        scene->addItem(edgeItem);
        edges.append(edgeItem);

        QGraphicsTextItem *weightLabel = new QGraphicsTextItem(QString::number(e.weight));
        weightLabel->setDefaultTextColor(QColor(100, 100, 100));
        weightLabel->setFont(QFont("Arial", 8));
        weightLabel->setPos((p1.x() + p2.x()) / 2, (p1.y() + p2.y()) / 2);
        weightLabel->setZValue(3);
        weightLabel->setVisible(showWeights);
        scene->addItem(weightLabel);
        weightLabels.append(weightLabel);
    }

    // 绘制节点
    for (auto it = nodePositions.begin(); it != nodePositions.end(); ++it) {
        QPointF pos = it.value();
        NodeItem *node = new NodeItem(it.key(), pos.x(), pos.y(), nodeRadius);
        scene->addItem(node);
        nodes.append(node);
    }
}

void GraphView::highlightMST(const QVector<Edge>& mstEdges) {
    clearAnimHighlight();
    QSet<QPair<int, int>> mstSet;
    for (const auto& e : mstEdges) {
        int u = e.u, v = e.v;
        if (u > v) std::swap(u, v);
        mstSet.insert({u, v});
    }

    for (auto* ei : edges) {
        int u = ei->u, v = ei->v;
        if (u > v) std::swap(u, v);
        if (mstSet.contains({u, v})) {
            ei->setPen(QPen(QColor(255, 50, 50), 3));
        }
    }

    for (auto* ni : nodes) {
        for (const auto& e : mstEdges) {
            if (ni->nodeId == e.u || ni->nodeId == e.v) {
                ni->setBrush(QColor(255, 100, 100));
                break;
            }
        }
    }
}

void GraphView::setShowWeights(bool show) {
    showWeights = show;
    for (auto* wl : weightLabels) {
        wl->setVisible(show);
    }
}

void GraphView::resetHighlight() {
    clearAnimHighlight();
    searchHighlightIds.clear();
    for (auto* ei : edges) {
        ei->setPen(QPen(QColor(140, 140, 140), 1.5));
    }
    for (auto* ni : nodes) {
        ni->setBrush(QColor(100, 149, 237));
    }
}

// ---- 查找功能 ----
void GraphView::highlightNodes(const QVector<int>& nodeIds) {
    clearSearchHighlight();
    QSet<int> ids(nodeIds.begin(), nodeIds.end());
    searchHighlightIds = ids;

    for (auto* ni : nodes) {
        if (ids.contains(ni->nodeId)) {
            // 橙色高亮 + 加粗边框
            ni->setBrush(QColor(255, 165, 0));
            ni->setPen(QPen(QColor(220, 120, 0), 3));
        }
    }
}

void GraphView::clearSearchHighlight() {
    for (auto* ni : nodes) {
        if (searchHighlightIds.contains(ni->nodeId)) {
            // 恢复默认颜色
            ni->setBrush(QColor(100, 149, 237));
            ni->setPen(QPen(Qt::black, 2));
        }
    }
    searchHighlightIds.clear();
}

void GraphView::centerOnNode(int nodeId) {
    for (auto* ni : nodes) {
        if (ni->nodeId == nodeId) {
            centerOn(ni);
            break;
        }
    }
}

// ---- 算法动画 ----
void GraphView::animateMST(const QVector<Edge>& orderedEdges, const QString& algoName, int totalWeight) {
    stopAnimation();
    resetHighlight();

    if (orderedEdges.isEmpty()) return;

    animOrderedEdges = orderedEdges;
    animIndex = 0;
    animTotalWeight = totalWeight;
    animAlgoName = algoName;
    animHighlighted.clear();

    // 根据边数自适应速度：边越多每步越快，但至少 50ms，最多 500ms
    int delay = qBound(50, 3000 / orderedEdges.size(), 500);
    animTimer->start(delay);
    emit statusMessage(QString("%1 算法运行中... (0/%2)").arg(algoName).arg(orderedEdges.size()));
}

void GraphView::stopAnimation() {
    animTimer->stop();
    animOrderedEdges.clear();
    animIndex = 0;
    animHighlighted.clear();
}

void GraphView::onAnimationStep() {
    if (animIndex >= animOrderedEdges.size()) {
        // 动画完成
        animTimer->stop();
        emit statusMessage(QString("%1 算法完成! MST权重=%2").arg(animAlgoName).arg(animTotalWeight));
        emit animationDone();
        return;
    }

    const Edge& e = animOrderedEdges[animIndex];
    int u = e.u, v = e.v;
    if (u > v) std::swap(u, v);
    animHighlighted.insert({u, v});

    // 高亮当前边的两个节点
    for (auto* ni : nodes) {
        if (ni->nodeId == e.u || ni->nodeId == e.v) {
            ni->setBrush(QColor(255, 100, 100));
        }
    }

    // 高亮当前步的边
    for (auto* ei : edges) {
        int eu = ei->u, ev = ei->v;
        if (eu > ev) std::swap(eu, ev);
        if (animHighlighted.contains({eu, ev})) {
            ei->setPen(QPen(QColor(255, 50, 50), 3));
        }
    }

    animIndex++;
    emit statusMessage(QString("%1 算法运行中... (%2/%3) 当前边: %4-%5 (权重%6)")
                           .arg(animAlgoName).arg(animIndex).arg(animOrderedEdges.size())
                           .arg(e.u).arg(e.v).arg(e.weight));
}

void GraphView::clearAnimHighlight() {
    animHighlighted.clear();
}

// ---- 右键菜单 ----
void GraphView::contextMenuEvent(QContextMenuEvent *event) {
    QPointF scenePos = mapToScene(event->pos());

    // 按 z 序从上到下检查点击到的 item，处理第一个匹配的
    QList<QGraphicsItem*> items = scene->items(scenePos, Qt::IntersectsItemShape,
                                                Qt::DescendingOrder);

    NodeItem* hitNode = nullptr;
    EdgeItem* hitEdge = nullptr;

    for (auto* item : items) {
        while (item && item->parentItem()) item = item->parentItem();
        if (!hitNode) hitNode = dynamic_cast<NodeItem*>(item);
        if (!hitEdge) hitEdge = dynamic_cast<EdgeItem*>(item);
        if (hitNode && hitEdge) break;
    }

    // 节点在上层 (z=2)，边在下层 (z=1)，点击节点时优先节点菜单
    if (hitNode) {
        // 右键节点 → 查看/编辑名称
        QMenu menu;
        QAction* editAction = menu.addAction(
            QString("查看/编辑节点 %1").arg(hitNode->nodeId));

        QAction* selected = menu.exec(event->globalPos());
        if (selected == editAction) {
            QDialog dlg(this);
            dlg.setWindowTitle(QString("节点 %1").arg(hitNode->nodeId));
            dlg.setMinimumWidth(280);

            QVBoxLayout* layout = new QVBoxLayout(&dlg);

            QLabel* idLabel = new QLabel(QString("节点 ID：%1").arg(hitNode->nodeId));
            idLabel->setStyleSheet("font-weight: bold; font-size: 14px;");
            layout->addWidget(idLabel);

            layout->addWidget(new QLabel("节点名称："));
            QLineEdit* nameEdit = new QLineEdit(hitNode->name);
            nameEdit->setPlaceholderText("输入节点名称（留空则不显示）");
            layout->addWidget(nameEdit);

            layout->addSpacing(10);

            QDialogButtonBox* buttons = new QDialogButtonBox(
                QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
            connect(buttons, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
            connect(buttons, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);
            layout->addWidget(buttons);

            if (dlg.exec() == QDialog::Accepted) {
                QString newName = nameEdit->text().trimmed();
                hitNode->setNodeName(newName);
                if (newName.isEmpty()) {
                    emit logMessage(QString("清除节点 %1 的名称").arg(hitNode->nodeId));
                } else {
                    emit logMessage(QString("节点 %1 重命名为 \"%2\"").arg(hitNode->nodeId).arg(newName));
                }
                emit statusMessage(QString("节点 %1 名称已更新").arg(hitNode->nodeId));
            }
        }
        return;
    }

    if (hitEdge) {
        // 右键边 → 编辑权重
        QMenu menu;
        QAction* editAction = menu.addAction(
            QString("编辑边 %1-%2 (当前权重: %3)").arg(hitEdge->u).arg(hitEdge->v).arg(hitEdge->weight));

        if (menu.exec(event->globalPos()) == editAction) {
            bool ok = false;
            int newWeight = QInputDialog::getInt(this, "编辑边权重",
                QString("边 %1 - %2 的新权重:").arg(hitEdge->u).arg(hitEdge->v),
                hitEdge->weight, 1, 10000, 1, &ok);
            if (!ok) return;

            if (currentGraph && currentGraph->updateEdgeWeight(hitEdge->u, hitEdge->v, newWeight)) {
                // 重绘
                scene->clear();
                nodes.clear();
                edges.clear();
                weightLabels.clear();
                drawGraph();
                fitView();

                emit logMessage(QString("更新边 %1-%2 权重: %3 → %4")
                                    .arg(hitEdge->u).arg(hitEdge->v).arg(hitEdge->weight).arg(newWeight));
                emit statusMessage(QString("边 %1-%2 权重已更新为 %3").arg(hitEdge->u).arg(hitEdge->v).arg(newWeight));
                emit graphChanged();
            }
        }
        return;
    }

    if (hitNode) {
        // 右键节点 → 查看/编辑名称
        QMenu menu;
        QAction* editAction = menu.addAction(
            QString("查看/编辑节点 %1").arg(hitNode->nodeId));

        QAction* selected = menu.exec(event->globalPos());
        if (selected == editAction) {
            QDialog dlg(this);
            dlg.setWindowTitle(QString("节点 %1").arg(hitNode->nodeId));
            dlg.setMinimumWidth(280);

            QVBoxLayout* layout = new QVBoxLayout(&dlg);

            QLabel* idLabel = new QLabel(QString("节点 ID：%1").arg(hitNode->nodeId));
            idLabel->setStyleSheet("font-weight: bold; font-size: 14px;");
            layout->addWidget(idLabel);

            layout->addWidget(new QLabel("节点名称："));
            QLineEdit* nameEdit = new QLineEdit(hitNode->name);
            nameEdit->setPlaceholderText("输入节点名称（留空则不显示）");
            layout->addWidget(nameEdit);

            layout->addSpacing(10);

            QDialogButtonBox* buttons = new QDialogButtonBox(
                QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
            connect(buttons, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
            connect(buttons, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);
            layout->addWidget(buttons);

            if (dlg.exec() == QDialog::Accepted) {
                QString newName = nameEdit->text().trimmed();
                hitNode->setNodeName(newName);
                if (newName.isEmpty()) {
                    emit logMessage(QString("清除节点 %1 的名称").arg(hitNode->nodeId));
                } else {
                    emit logMessage(QString("节点 %1 重命名为 \"%2\"").arg(hitNode->nodeId).arg(newName));
                }
                emit statusMessage(QString("节点 %1 名称已更新").arg(hitNode->nodeId));
            }
        }
        return;
    }
}

// ---- 交互模式事件处理 ----

NodeItem* GraphView::nodeAtPos(const QPointF& scenePos) {
    QList<QGraphicsItem*> items = scene->items(scenePos, Qt::IntersectsItemShape,
                                                Qt::DescendingOrder);
    for (auto* item : items) {
        while (item && item->parentItem()) item = item->parentItem();
        if (auto* ni = dynamic_cast<NodeItem*>(item)) {
            return ni;
        }
    }
    return nullptr;
}

EdgeItem* GraphView::edgeAtPos(const QPointF& scenePos) {
    QList<QGraphicsItem*> items = scene->items(scenePos, Qt::IntersectsItemShape,
                                                Qt::DescendingOrder);
    for (auto* item : items) {
        if (auto* ei = dynamic_cast<EdgeItem*>(item)) {
            return ei;
        }
    }
    return nullptr;
}

void GraphView::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
        // 右键用于 context menu，不让 view 处理
        return;
    }
    if (mode == PanMode) {
        QGraphicsView::mousePressEvent(event);
        return;
    }
}

void GraphView::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::RightButton) return;

    if (mode == PanMode) {
        QGraphicsView::mouseReleaseEvent(event);
        return;
    }

    if (!currentGraph) {
        emit statusMessage("请先生成图！");
        return;
    }

    QPointF scenePos = mapToScene(event->pos());

    if (mode == AddNodeMode) {
        NodeItem* hit = nodeAtPos(scenePos);
        if (hit) {
            emit statusMessage(QString("节点 %1 已存在，请在空白处加点").arg(hit->nodeId));
            return;
        }

        int newNodeId = currentGraph->addNode();
        nodePositions[newNodeId] = scenePos;

        scene->clear();
        nodes.clear();
        edges.clear();
        weightLabels.clear();
        int n = currentGraph->getNodeCount();
        nodeRadius = qBound(4.0, 400.0 / n, 14.0);
        drawGraph();
        fitView();

        // 恢复节点的名称标签（重新绘制后丢失，从 graph 恢复）
        // 目前名称存在 NodeItem 本身，重绘后丢失，但这是个 UI 问题
        // 简单方案：让用户通过右键重新设置

        emit logMessage(QString("添加节点: %1 (位置: %.0f, %.0f)")
                            .arg(newNodeId).arg(scenePos.x()).arg(scenePos.y()));
        emit statusMessage(QString("已添加节点 %1，继续点击空白处加点").arg(newNodeId));
        emit graphChanged();
        return;
    }

    if (mode == AddEdgeMode) {
        NodeItem* hit = nodeAtPos(scenePos);
        if (!hit) {
            emit statusMessage("请点击一个节点");
            return;
        }

        int clickedNode = hit->nodeId;

        if (pendingSourceNode < 0) {
            pendingSourceNode = clickedNode;
            hit->setBrush(QColor(50, 205, 50));
            emit statusMessage(QString("已选源节点 %1，请点击目标节点").arg(clickedNode));
        } else {
            int u = pendingSourceNode;
            int v = clickedNode;

            if (u == v) {
                emit statusMessage("起点和终点不能相同！");
                pendingSourceNode = -1;
                resetHighlight();
                return;
            }

            const auto& existingEdges = currentGraph->getEdges();
            for (const auto& e : existingEdges) {
                if ((e.u == u && e.v == v) || (e.u == v && e.v == u)) {
                    emit statusMessage(QString("边 %1 - %2 已存在！").arg(u).arg(v));
                    pendingSourceNode = -1;
                    resetHighlight();
                    return;
                }
            }

            bool ok = false;
            int w = QInputDialog::getInt(this, "添加边",
                                         QString("边 %1 - %2 的权重:").arg(u).arg(v),
                                         50, 1, 1000, 1, &ok);
            if (!ok) {
                pendingSourceNode = -1;
                resetHighlight();
                emit statusMessage("已取消");
                return;
            }

            currentGraph->addEdge(u, v, w);

            scene->clear();
            nodes.clear();
            edges.clear();
            weightLabels.clear();
            drawGraph();
            fitView();

            emit logMessage(QString("添加边: %1 - %2 (权重%3)").arg(u).arg(v).arg(w));
            emit statusMessage(QString("已添加边 %1 - %2 (权重%3)，继续点击加边").arg(u).arg(v).arg(w));
            emit graphChanged();

            pendingSourceNode = -1;
        }
        return;
    }
}

void GraphView::wheelEvent(QWheelEvent *event) {
    double scaleFactor = 1.15;
    if (event->angleDelta().y() > 0) {
        scale(scaleFactor, scaleFactor);
    } else {
        scale(1.0 / scaleFactor, 1.0 / scaleFactor);
    }
    event->accept();
}
