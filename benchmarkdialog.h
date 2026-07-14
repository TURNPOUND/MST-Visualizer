#ifndef BENCHMARKDIALOG_H
#define BENCHMARKDIALOG_H

#include <QDialog>

namespace Ui {
class BenchmarkDialog;
}

class BenchmarkDialog : public QDialog {
    Q_OBJECT
public:
    explicit BenchmarkDialog(QWidget *parent = nullptr);
    ~BenchmarkDialog();
    void addResult(int n, int m, double primTime, double kruskalTime, int mstWeight, int memoryKB);
    void clearResults();

private slots:
    void on_btnExport_clicked();

private:
    Ui::BenchmarkDialog *ui;
};

#endif
