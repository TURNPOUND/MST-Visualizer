#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "graphgenerator.h"
#include <QMessageBox>
#include <QElapsedTimer>
#include <QFileDialog>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QTextBrowser>
#include <QSplitter>
#include <QRegularExpression>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , graph(nullptr)
    , benchmarkDialog(new BenchmarkDialog(this))
    , modeGroup(new QButtonGroup(this)) {
    ui->setupUi(this);

    // ---- 右侧窗格可拖拽分割 ----
    QSplitter* rightSplitter = new QSplitter(Qt::Vertical, this);
    rightSplitter->addWidget(ui->groupBox_4);  // 图可视化
    rightSplitter->addWidget(ui->groupBox_5);  // 结果输出
    rightSplitter->addWidget(ui->groupBox_6);  // 运行日志
    rightSplitter->setStretchFactor(0, 3);
    rightSplitter->setStretchFactor(1, 1);
    rightSplitter->setStretchFactor(2, 1);
    // 替换 layout 中的原有项
    QLayoutItem* item;
    while ((item = ui->verticalLayout->takeAt(0)) != nullptr) {
        delete item;
    }
    ui->verticalLayout->addWidget(rightSplitter);

    graphView = new GraphView(this);
    ui->layoutGraph->addWidget(graphView);

    // ---- 非 auto-connect 的连接 ----
    // (on_btn*_clicked 由 setupUi 自动连接，无需手动 connect)
    connect(ui->checkShowWeight, &QCheckBox::toggled, graphView, &GraphView::setShowWeights);
    connect(ui->checkWeightedLayout, &QCheckBox::toggled, this, [this](bool checked) {
        graphView->setWeightedLayoutEnabled(checked);
        if (graph && graph->getNodeCount() > 0) {
            refreshGraphView();
        }
    });

    // ---- 查找功能区域 ----
    QGroupBox* findGroup = new QGroupBox("查找节点", this);
    QVBoxLayout* findOuterLayout = new QVBoxLayout(findGroup);

    QHBoxLayout* findModeLayout = new QHBoxLayout();
    comboSearchMode = new QComboBox();
    comboSearchMode->addItem("按序号查找");
    comboSearchMode->addItem("按名称查找");
    findModeLayout->addWidget(comboSearchMode);

    editSearchText = new QLineEdit();
    editSearchText->setPlaceholderText("输入序号或名称...");
    findModeLayout->addWidget(editSearchText);
    findOuterLayout->addLayout(findModeLayout);

    QHBoxLayout* findBtnLayout = new QHBoxLayout();
    btnFind = new QPushButton("🔍 查找");
    btnClearFind = new QPushButton("✕ 清除高亮");
    btnClearFind->setStyleSheet("background: #e67e22;");
    findBtnLayout->addWidget(btnFind);
    findBtnLayout->addWidget(btnClearFind);
    findOuterLayout->addLayout(findBtnLayout);

    // 插入到交互组和算法执行组之间（第二个 item 之后第 5 个位置）
    // 顺序: 图生成(0), btnGenerate(1), btnImportGraph(2), btnExportGraph(3), 交互(4), 查找→在这里
    QVBoxLayout* leftLayout = qobject_cast<QVBoxLayout*>(ui->groupBox->layout());
    if (leftLayout) {
        // 查找 groupBox_3 (算法执行) 的位置
        for (int i = 0; i < leftLayout->count(); ++i) {
            auto* w = leftLayout->itemAt(i)->widget();
            if (w == ui->groupBox_3) {
                leftLayout->insertWidget(i, findGroup);
                break;
            }
        }
    }

    connect(btnFind, &QPushButton::clicked, this, &MainWindow::on_btnFind_clicked);
    connect(btnClearFind, &QPushButton::clicked, this, &MainWindow::on_btnClearFind_clicked);
    connect(editSearchText, &QLineEdit::returnPressed, this, &MainWindow::on_btnFind_clicked);

    // ---- 交互模式按钮组 ----
    modeGroup->addButton(ui->btnModePan, GraphView::PanMode);
    modeGroup->addButton(ui->btnModeAddNode, GraphView::AddNodeMode);
    modeGroup->addButton(ui->btnModeAddEdge, GraphView::AddEdgeMode);
    connect(modeGroup, &QButtonGroup::idClicked, this, &MainWindow::onModeChanged);

    // ---- GraphView 信号 ----
    connect(graphView, &GraphView::statusMessage, this, [this](const QString& msg) {
        ui->statusbar->showMessage(msg);
    });
    connect(graphView, &GraphView::logMessage, this, [this](const QString& msg) {
        ui->textLog->appendPlainText(msg);
    });
    connect(graphView, &GraphView::graphChanged, this, [this]() {
        // 连续编辑模式：不自动切回浏览
    });

    setWindowTitle("最小生成树算法可视化 - 卓软2501 黄仁海");
    ui->statusbar->showMessage("就绪 - 请先生成图或导入图");

    // ---- 结果/日志区域可折叠 ----
    ui->groupBox_5->setCheckable(true);
    ui->groupBox_5->setChecked(true);
    ui->groupBox_6->setCheckable(true);
    ui->groupBox_6->setChecked(true);

    // ---- 全局 UI 美化 ----
    setStyleSheet(R"(
/* 全局字体 */
QWidget {
    font-family: "Microsoft YaHei", "Segoe UI", sans-serif;
    font-size: 13px;
}

/* 主窗口背景 */
QMainWindow, QMainWindow > QWidget {
    background: #c0c0c0;
}

/* GroupBox */
QGroupBox {
    font-weight: bold;
    color: #2c3e50;
    border: 1px solid #a0a0a0;
    border-radius: 6px;
    margin-top: 10px;
    padding: 12px 8px 8px 8px;
    background: #d5d5d5;
}
QGroupBox::title {
    subcontrol-origin: margin;
    subcontrol-position: top left;
    padding: 2px 10px;
    background: #3498db;
    color: white;
    border-radius: 4px;
    font-size: 12px;
}

/* QPushButton */
QPushButton {
    background: #3498db;
    color: white;
    border: none;
    border-radius: 4px;
    padding: 6px 12px;
    font-weight: bold;
    min-height: 24px;
}
QPushButton:hover {
    background: #2980b9;
}
QPushButton:pressed {
    background: #1a6fa0;
}
QPushButton:checked {
    background: #27ae60;
}
QPushButton:checked:hover {
    background: #219a52;
}

/* 特殊按钮保留蓝色（生成、导入、算法等）不再覆盖 */
QPushButton#btnGenerate,
QPushButton#btnPrim,
QPushButton#btnKruskal,
QPushButton#btnBenchmark,
QPushButton#btnImportGraph,
QPushButton#btnExportGraph,
QPushButton#btnExportResult,
QPushButton#btnClear {
    background: #3498db;
}
QPushButton#btnGenerate:hover,
QPushButton#btnPrim:hover,
QPushButton#btnKruskal:hover,
QPushButton#btnBenchmark:hover,
QPushButton#btnImportGraph:hover,
QPushButton#btnExportGraph:hover,
QPushButton#btnExportResult:hover,
QPushButton#btnClear:hover {
    background: #2980b9;
}

/* 模式按钮组：选中色不同 */
QPushButton#btnModePan:checked {
    background: #3498db;
}
QPushButton#btnModeAddNode:checked {
    background: #e67e22;
}
QPushButton#btnModeAddEdge:checked {
    background: #9b59b6;
}

/* 使用教程按钮 */
QPushButton#btnHelp {
    background: #2ecc71;
}
QPushButton#btnHelp:hover {
    background: #27ae60;
}

/* 清空按钮 */
QPushButton#btnClear {
    background: #e74c3c;
}
QPushButton#btnClear:hover {
    background: #c0392b;
}

/* CheckBox */
QCheckBox {
    spacing: 6px;
    color: #2c3e50;
}
QCheckBox::indicator {
    width: 16px;
    height: 16px;
    border: 2px solid #bdc3c7;
    border-radius: 3px;
    background: white;
}
QCheckBox::indicator:checked {
    background: #3498db;
    border-color: #3498db;
}

/* SpinBox / DoubleSpinBox */
QSpinBox, QDoubleSpinBox {
    border: 1px solid #a0a0a0;
    border-radius: 3px;
    padding: 2px 6px;
    background: #c8c8c8;
    color: #2c3e50;
    min-height: 20px;
}

/* PlainTextEdit */
QPlainTextEdit {
    border: 1px solid #dcdde1;
    border-radius: 4px;
    background: #fafafa;
    color: #2c3e50;
    font-family: "Consolas", "Courier New", monospace;
    font-size: 12px;
}

/* 标签 */
QLabel {
    color: #2c3e50;
}

/* 状态栏 */
QStatusBar {
    background: #ecf0f1;
    color: #7f8c8d;
    border-top: 1px solid #dcdde1;
    font-size: 12px;
}
)");
}

// ---- 交互模式切换 ----
void MainWindow::onModeChanged(int id) {
    if (!graph) {
        QMessageBox::warning(this, "警告", "请先生成图！");
        ui->btnModePan->setChecked(true);
        return;
    }
    graphView->setMode(static_cast<GraphView::Mode>(id));
    graphView->resetHighlight();

    switch (id) {
    case GraphView::PanMode:
        ui->statusbar->showMessage("浏览模式 - 拖拽平移，滚轮缩放");
        break;
    case GraphView::AddNodeMode:
        ui->statusbar->showMessage("加点模式 - 点击空白处添加新节点");
        break;
    case GraphView::AddEdgeMode:
        ui->statusbar->showMessage("加边模式 - 依次点击两个节点添加边");
        break;
    }
}

// ---- 图生成 ----
void MainWindow::on_btnGenerate_clicked() {
    int n = ui->spinNodeCount->value();
    double density = ui->doubleSpinDensity->value();

    if (n < 2) {
        QMessageBox::warning(this, "警告", "节点数至少为2！");
        return;
    }

    delete graph;
    graph = GraphGenerator::generateConnectedGraph(n, density);

    refreshGraphView();

    ui->btnModePan->setChecked(true);
    graphView->setMode(GraphView::PanMode);
    ui->statusbar->showMessage(
        QString("已生成随机连通图: %1 节点, %2 条边, 密度=%3%")
            .arg(n).arg(graph->getEdgeCount()).arg(density * 100));
}

// ---- 算法执行（带动画）----
static void runAlgorithmWithAnimation(QPlainTextEdit* textResult, GraphView* graphView,
                                       MSTGraph* graph, const QString& algoName,
                                       const QPair<int, QVector<Edge>>& result, qint64 elapsed) {
    Q_UNUSED(elapsed)
    // 显示结果文本
    double ms = elapsed / 1e6;
    double us = elapsed / 1e3;
    QString text = QString("\n========== %1算法结果 ==========").arg(algoName);
    text += QString("\n总权重: %1").arg(result.first);
    text += QString("\n边数: %1 / %2")
                .arg(result.second.size()).arg(graph->getNodeCount() - 1);
    text += QString("\n耗时: %1 μs (%2 ms)")
                .arg(us, 0, 'f', 2).arg(ms, 0, 'f', 4);
    text += QString("\n------------------------------------\n");
    textResult->appendPlainText(text);

    // 边数较多时跳过动画，直接高亮结果
    int edgeCount = result.second.size();
    if (edgeCount > 30) {
        graphView->highlightMST(result.second);
        return;
    }

    // 启动动画：获取按顺序添加的边
    if (algoName == "Prim") {
        auto ordered = graph->primMSTOrdered();
        graphView->animateMST(ordered, "Prim", result.first);
    } else {
        auto ordered = graph->kruskalMSTOrdered();
        graphView->animateMST(ordered, "Kruskal", result.first);
    }
}

void MainWindow::on_btnPrim_clicked() {
    if (!graph) {
        QMessageBox::warning(this, "警告", "请先生成图！");
        return;
    }

    graphView->resetHighlight();

    QElapsedTimer timer;
    timer.start();
    auto result = graph->primMST();
    qint64 elapsed = timer.nsecsElapsed();

    runAlgorithmWithAnimation(ui->textResult, graphView, graph, "Prim", result, elapsed);
}

void MainWindow::on_btnKruskal_clicked() {
    if (!graph) {
        QMessageBox::warning(this, "警告", "请先生成图！");
        return;
    }

    graphView->resetHighlight();

    QElapsedTimer timer;
    timer.start();
    auto result = graph->kruskalMST();
    qint64 elapsed = timer.nsecsElapsed();

    runAlgorithmWithAnimation(ui->textResult, graphView, graph, "Kruskal", result, elapsed);
}

void MainWindow::displayResult(const QString& algo,
                               const QPair<int, QVector<Edge>>& result,
                               qint64 elapsed) {
    double ms = elapsed / 1e6;
    double us = elapsed / 1e3;

    QString text = QString("\n========== %1算法结果 ==========").arg(algo);
    text += QString("\n总权重: %1").arg(result.first);
    text += QString("\n边数: %1 / %2")
                .arg(result.second.size()).arg(graph->getNodeCount() - 1);
    text += QString("\n耗时: %1 μs (%2 ms)")
                .arg(us, 0, 'f', 2).arg(ms, 0, 'f', 4);
    text += QString("\n------------------------------------\n");

    ui->textResult->appendPlainText(text);
}

void MainWindow::on_btnBenchmark_clicked() {
    benchmarkDialog->clearResults();
    benchmarkDialog->show();

    QVector<int> sizes = {100, 500, 1000, 2000, 5000};

    for (int i = 0; i < sizes.size(); ++i) {
        int n = sizes[i];
        double d = (n >= 2000) ? 0.1 : 0.3;

        MSTGraph* g = GraphGenerator::generateConnectedGraph(n, d);
        int m = g->getEdgeCount();

        QElapsedTimer t1;
        t1.start();
        auto r1 = g->primMST();
        double primTime = t1.nsecsElapsed() / 1e6;

        QElapsedTimer t2;
        t2.start();
        auto r2 = g->kruskalMST();
        double kruskalTime = t2.nsecsElapsed() / 1e6;

        int memoryKB = (int)((n * n * d * 16 + n * 20) / 1024.0);

        benchmarkDialog->addResult(n, m, primTime, kruskalTime, r1.first, memoryKB);

        delete g;
    }
}

void MainWindow::on_btnClear_clicked() {
    graphView->clearGraph();
    delete graph;
    graph = nullptr;
    ui->textResult->clear();
    ui->textLog->clear();
    ui->btnModePan->setChecked(true);
    graphView->setMode(GraphView::PanMode);
    ui->statusbar->showMessage("已清空");
}

void MainWindow::on_btnResetView_clicked() {
    graphView->fitView();
}

void MainWindow::on_btnExportResult_clicked() {
    QString filename = QFileDialog::getSaveFileName(
        this, "导出结果", "mst_result.txt", "文本文件 (*.txt)");
    if (filename.isEmpty()) return;

    QFile file(filename);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        stream << ui->textResult->toPlainText();
        file.close();
        QMessageBox::information(this, "成功", "结果已导出");
    }
}

// ---- 文件 I/O ----
void MainWindow::on_btnImportGraph_clicked() {
    QString filename = QFileDialog::getOpenFileName(
        this, "导入图", "", "文本文件 (*.txt *.csv);;所有文件 (*)");
    if (filename.isEmpty()) return;

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "错误", "无法打开文件");
        return;
    }

    QVector<std::tuple<int, int, int>> parsedEdges;
    int maxNode = -1;
    int lineNum = 0;
    QTextStream in(&file);

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        lineNum++;
        if (line.isEmpty() || line.startsWith('#')) continue;

        QStringList parts = line.split(QRegularExpression("[\\s,]+"), Qt::SkipEmptyParts);
        if (parts.size() < 3) {
            QMessageBox::warning(this, "格式错误",
                QString("第 %1 行格式错误，应为: u v w").arg(lineNum));
            file.close();
            return;
        }

        bool ok1, ok2, ok3;
        int u = parts[0].toInt(&ok1);
        int v = parts[1].toInt(&ok2);
        int w = parts[2].toInt(&ok3);

        if (!ok1 || !ok2 || !ok3 || u < 0 || v < 0 || w <= 0) {
            QMessageBox::warning(this, "格式错误",
                QString("第 %1 行数据无效（u, v >= 0, w > 0）").arg(lineNum));
            file.close();
            return;
        }

        parsedEdges.append({u, v, w});
        maxNode = qMax(maxNode, qMax(u, v));
    }
    file.close();

    if (parsedEdges.isEmpty()) {
        QMessageBox::warning(this, "错误", "文件中没有有效的边数据");
        return;
    }

    int n = maxNode + 1;
    delete graph;
    graph = new MSTGraph(n);
    for (const auto& [u, v, w] : parsedEdges) {
        graph->addEdge(u, v, w);
    }

    refreshGraphView();

    ui->btnModePan->setChecked(true);
    graphView->setMode(GraphView::PanMode);
    ui->textLog->appendPlainText(
        QString("导入图: %1 节点, %2 条边 (文件: %3)")
            .arg(n).arg(graph->getEdgeCount()).arg(QFileInfo(filename).fileName()));
    ui->statusbar->showMessage(
        QString("已导入图: %1 节点, %2 条边").arg(n).arg(graph->getEdgeCount()));
}

void MainWindow::on_btnExportGraph_clicked() {
    if (!graph || graph->getEdgeCount() == 0) {
        QMessageBox::warning(this, "警告", "没有可供导出的图！");
        return;
    }

    QString filename = QFileDialog::getSaveFileName(
        this, "导出图", "graph.txt", "文本文件 (*.txt *.csv);;所有文件 (*)");
    if (filename.isEmpty()) return;

    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "错误", "无法保存文件");
        return;
    }

    QTextStream out(&file);
    out << "# MST-Visualizer 图数据\n";
    out << "# 格式: u v w  (节点编号从0开始，权重为正整数)\n";
    out << "# 节点数: " << graph->getNodeCount()
        << "  边数: " << graph->getEdgeCount() << "\n\n";

    const auto& edgesList = graph->getEdges();
    for (const auto& e : edgesList) {
        out << e.u << " " << e.v << " " << e.weight << "\n";
    }

    file.close();
    ui->statusbar->showMessage(
        QString("已导出图到: %1").arg(QFileInfo(filename).fileName()));
    QMessageBox::information(this, "成功",
        QString("图已导出到:\n%1").arg(filename));
}

// ---- 使用教程 ----
// ---- 查找功能 ----
void MainWindow::on_btnFind_clicked() {
    if (!graph || graph->getNodeCount() == 0) {
        QMessageBox::warning(this, "警告", "请先生成图或导入图！");
        return;
    }

    QString keyword = editSearchText->text().trimmed();
    if (keyword.isEmpty()) {
        QMessageBox::information(this, "提示", "请输入要查找的内容");
        return;
    }

    int mode = comboSearchMode->currentIndex();
    QVector<int> foundIds;

    if (mode == 0) {
        // ---- 按序号查找 ----
        bool ok = false;
        int targetId = keyword.toInt(&ok);
        if (!ok || targetId < 0) {
            QMessageBox::warning(this, "输入错误", "请输入有效的节点序号（非负整数）");
            return;
        }
        if (targetId >= graph->getNodeCount()) {
            QMessageBox::information(this, "未找到",
                QString("节点 %1 不存在，当前节点范围: 0 ~ %2")
                    .arg(targetId).arg(graph->getNodeCount() - 1));
            return;
        }
        foundIds.append(targetId);
        ui->textLog->appendPlainText(
            QString("查找节点 %1 → 找到").arg(targetId));
    } else {
        // ---- 按名称查找（模糊匹配）----
        for (auto* ni : graphView->getNodes()) {
            if (ni->name.contains(keyword, Qt::CaseInsensitive)) {
                foundIds.append(ni->nodeId);
            }
        }
        if (foundIds.isEmpty()) {
            ui->textLog->appendPlainText(
                QString("查找名称包含 \"%1\" 的节点 → 未找到").arg(keyword));
            QMessageBox::information(this, "未找到",
                QString("未找到名称包含 \"%1\" 的节点").arg(keyword));
            return;
        }
        std::sort(foundIds.begin(), foundIds.end());
        ui->textLog->appendPlainText(
            QString("查找名称包含 \"%1\" 的节点 → 找到 %2 个")
                .arg(keyword).arg(foundIds.size()));
    }

    // 高亮找到的节点，居中到第一个
    graphView->highlightNodes(foundIds);
    graphView->centerOnNode(foundIds.first());

    // 构建状态消息
    QString resultMsg;
    if (foundIds.size() == 1) {
        int id = foundIds.first();
        QString nodeName;
        for (auto* ni : graphView->getNodes()) {
            if (ni->nodeId == id) {
                nodeName = ni->name;
                break;
            }
        }
        if (!nodeName.isEmpty()) {
            ui->statusbar->showMessage(
                QString("找到节点 %1 (名称: %2)").arg(id).arg(nodeName));
        } else {
            ui->statusbar->showMessage(
                QString("找到节点 %1").arg(id));
        }
    } else {
        ui->statusbar->showMessage(
            QString("找到 %1 个节点，已高亮显示").arg(foundIds.size()));
    }
}

void MainWindow::on_btnClearFind_clicked() {
    graphView->clearSearchHighlight();
    ui->statusbar->showMessage("已清除查找高亮");
    ui->textLog->appendPlainText("清除查找高亮");
}

void MainWindow::on_btnHelp_clicked() {
    QDialog dlg(this);
    dlg.setWindowTitle("📖 使用教程 - 最小生成树算法可视化");
    dlg.resize(560, 520);

    QVBoxLayout* layout = new QVBoxLayout(&dlg);

    QTextBrowser* browser = new QTextBrowser();
    browser->setOpenExternalLinks(true);
    browser->setHtml(R"(
<style>
h2 { color: #2c6b9e; border-bottom: 1px solid #ccc; padding-bottom: 4px; }
h3 { color: #3a8bc2; }
code { background: #f0f0f0; padding: 1px 4px; border-radius: 3px; font-size: 12px; }
ul { margin-left: 16px; }
</style>

<h2>📋 基本操作</h2>

<h3>生成图</h3>
<ul>
  <li>设置 <b>节点数</b> 和 <b>边密度</b>，点击 <b>「生成随机图」</b></li>
  <li>密度 0.3 = 30% 的边被随机添加</li>
  <li>密度越大图越稠密，算法耗时越长</li>
</ul>

<h3>导入/导出图</h3>
<ul>
  <li><b>「导入图」</b>：从文件读取图数据，格式：每行 <code>u v w</code>（以空格或逗号分隔）</li>
  <li><b>「导出图」</b>：将当前图保存为文本文件，可重新导入</li>
  <li>支持 <code>#</code> 注释行</li>
</ul>

<h2>🔍 查找节点</h2>
<ul>
  <li><b>按序号查找</b>：输入节点编号（如 <code>5</code>），找到并高亮对应节点</li>
  <li><b>按名称查找</b>：输入节点名称关键词，模糊匹配所有包含该名称的节点</li>
  <li>找到的节点以 <span style='color:#ffa500;'>橙色高亮</span> 显示，视图自动居中到第一个结果</li>
  <li>点击 <b>「清除高亮」</b> 恢复节点颜色</li>
</ul>

<h2>🖱 交互编辑</h2>

<h3>浏览模式（默认）</h3>
<ul>
  <li>拖拽平移、滚轮缩放可视化区域</li>
</ul>

<h3>加点模式</h3>
<ul>
  <li>点击 <b>「加点」</b> 进入连续加点模式</li>
  <li>在可视化区 <b>点击空白处</b> 添加新节点</li>
  <li>节点会出现在点击位置</li>
  <li>点 <b>「浏览」</b> 退出</li>
</ul>

<h3>加边模式</h3>
<ul>
  <li>点击 <b>「加边」</b> 进入连续加边模式</li>
  <li><b>依次点击两个节点</b>，弹出对话框输入权重</li>
  <li>点 <b>「浏览」</b> 退出</li>
</ul>

<h3>右键编辑</h3>
<ul>
  <li><b>右键节点</b> → 查看/编辑节点 ID 和名称</li>
  <li>设置名称后，名称会显示在节点下方</li>
  <li><b>右键边</b> → 编辑边权重</li>
</ul>

<h2>🧮 算法执行</h2>

<h3>Prim / Kruskal 算法</h3>
<ul>
  <li>点击 <b>「Prim算法」</b> 或 <b>「Kruskal算法」</b> 执行最小生成树计算</li>
  <li>边数 ≤ 30 时：<b>动画演示</b> 逐条高亮添加的边</li>
  <li>边数 > 30 时：直接显示最终结果</li>
  <li>MST 边以 <span style='color:red;'>红色高亮</span> 显示</li>
  <li>结果文本显示在右侧「结果输出」区域</li>
</ul>

<h3>批量性能测试</h3>
<ul>
  <li>自动对 100~5000 节点规模进行性能对比</li>
  <li>显示 Prim 与 Kruskal 的运行时间对比</li>
  <li>支持导出 CSV</li>
</ul>

<h2>⚙️ 显示选项</h2>
<ul>
  <li><b>「显示边权重」</b>：切换边的权重数字显示</li>
  <li><b>「距离等比边权」</b>：启用力导向布局，节点距离正比于边权</li>
</ul>

<h2>📄 导出结果</h2>
<ul>
  <li><b>「导出结果」</b>：将右侧结果文本保存到文件</li>
  <li><b>「重置视图」</b>：恢复可视化区域的缩放和位置</li>
  <li><b>「清空」</b>：清除图数据、结果和日志</li>
</ul>

<hr>
<p style='color:gray; text-align:center;'>最小生成树算法可视化 &copy; 卓软2501 黄仁海</p>
)");

    layout->addWidget(browser);

    QDialogButtonBox* buttons = new QDialogButtonBox(QDialogButtonBox::Close);
    connect(buttons, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);
    layout->addWidget(buttons);

    dlg.exec();
}

// ---- 辅助 ----
void MainWindow::refreshGraphView() {
    graphView->clearGraph();
    if (ui->checkWeightedLayout->isChecked()) {
        graphView->generateWeightedLayout(graph->getNodeCount(), graph);
    } else {
        graphView->generateRandomLayout(graph->getNodeCount());
    }
    graphView->setGraph(graph);
}

MainWindow::~MainWindow() {
    delete ui;
    delete graph;
}
