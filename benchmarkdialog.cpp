#include "benchmarkdialog.h"
#include "ui_benchmarkdialog.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

BenchmarkDialog::BenchmarkDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BenchmarkDialog) {
    ui->setupUi(this);
    setWindowTitle("批量性能测试结果");
    ui->tableResults->setColumnCount(6);
    ui->tableResults->setHorizontalHeaderLabels(
        QStringList{"节点数", "边数", "Prim(ms)", "Kruskal(ms)", "MST权重", "内存(KB)"}
        );
    ui->tableResults->horizontalHeader()->setStretchLastSection(true);
    connect(ui->btnExport, &QPushButton::clicked, this, &BenchmarkDialog::on_btnExport_clicked);
}

BenchmarkDialog::~BenchmarkDialog() {
    delete ui;
}

void BenchmarkDialog::addResult(int n, int m, double primTime, double kruskalTime, int mstWeight, int memoryKB) {
    int row = ui->tableResults->rowCount();
    ui->tableResults->insertRow(row);
    ui->tableResults->setItem(row, 0, new QTableWidgetItem(QString::number(n)));
    ui->tableResults->setItem(row, 1, new QTableWidgetItem(QString::number(m)));
    ui->tableResults->setItem(row, 2, new QTableWidgetItem(QString::number(primTime, 'f', 4)));
    ui->tableResults->setItem(row, 3, new QTableWidgetItem(QString::number(kruskalTime, 'f', 4)));
    ui->tableResults->setItem(row, 4, new QTableWidgetItem(QString::number(mstWeight)));
    ui->tableResults->setItem(row, 5, new QTableWidgetItem(QString::number(memoryKB)));
}

void BenchmarkDialog::clearResults() {
    ui->tableResults->setRowCount(0);
}

void BenchmarkDialog::on_btnExport_clicked() {
    QString filename = QFileDialog::getSaveFileName(this, "导出CSV", "benchmark_results.csv", "CSV文件 (*.csv)");
    if (filename.isEmpty()) return;
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "错误", "无法保存文件");
        return;
    }
    QTextStream stream(&file);
    stream << "节点数,边数,Prim(ms),Kruskal(ms),MST权重,内存(KB)\n";
    for (int row = 0; row < ui->tableResults->rowCount(); ++row) {
        for (int col = 0; col < 6; ++col) {
            stream << ui->tableResults->item(row, col)->text();
            if (col < 5) stream << ",";
        }
        stream << "\n";
    }
    file.close();
    QMessageBox::information(this, "成功", "结果已导出到:\n" + filename);
}