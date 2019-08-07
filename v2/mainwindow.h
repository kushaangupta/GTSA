#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QFileDialog>
#include <QKeyEvent>
#include <QVector>

#include <QtCharts/QChartView>
#include <QtCharts/QLegend>
#include <QtCharts/QLineSeries>
#include <QtCharts/QCategoryAxis>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_findNext_clicked();

    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_As_triggered();

    void on_actionPrint_triggered();

    void on_actionExit_triggered();

    void on_actionCut_triggered();

    void on_actionCopy_triggered();

    void on_actionPaste_triggered();

    void on_actionUndo_triggered();

    void on_actionRedo_triggered();

    void on_actionAbout_GTSA_triggered();

    void on_lineEdit_returnPressed();

    void on_tabWidget_tabCloseRequested(int index);

    void on_actionSave_triggered();

    void on_actionClose_Tab_triggered();

    void on_tabWidget_currentChanged(int index);

    void on_actionAnalyze_triggered();

    void on_closeFind_clicked();

private:
    Ui::MainWindow *ui;
    QString currentFile = "";
    QVector<QString> tabLocationRegister;
    int currentTab = 0;
    bool recentlyClosed = false;
    int sz, ctr, strt, fin;
    QString SENTIMENTAL_ANALYSIS_SCRIPT = "\"C:\\Users\\Kushaan\ Gupta\\Documents\\GTSA\\res\\scripts\\sentimentalAnalysis.py\"";
    void wordCount();
    void updateWinName();
    void showCursorPos();
    void updateTextInfo();
    void estConnSB(int);
    void severConnSB(int);
};

#endif // MAINWINDOW_H
