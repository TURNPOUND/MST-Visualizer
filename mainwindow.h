#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QElapsedTimer>
#include <QButtonGroup>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include "mstalgorithms.h"
#include "graphview.h"
#include "benchmarkdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnGenerate_clicked();
    void on_btnPrim_clicked();
    void on_btnKruskal_clicked();
    void on_btnBenchmark_clicked();
    void on_btnClear_clicked();
    void on_btnResetView_clicked();
    void on_btnExportResult_clicked();
    void on_btnImportGraph_clicked();
    void on_btnExportGraph_clicked();
    void on_btnHelp_clicked();
    void on_btnFind_clicked();
    void on_btnClearFind_clicked();

    void onModeChanged(int id);

private:
    Ui::MainWindow *ui;
    MSTGraph* graph;
    GraphView* graphView;
    BenchmarkDialog* benchmarkDialog;
    QButtonGroup* modeGroup;

    // 查找功能 UI
    QComboBox* comboSearchMode;
    QLineEdit* editSearchText;
    QPushButton* btnFind;
    QPushButton* btnClearFind;

    void displayResult(const QString& algo, const QPair<int, QVector<Edge>>& result, qint64 elapsed);
    void refreshGraphView();
};

#endif
