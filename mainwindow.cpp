#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "texteditor.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->centralWidget);
    estConnSB(currentTab); //establish init connection
    sz = 0; ctr = 0; strt = 0; fin = 0;
    tabLocationRegister.clear();
    tabLocationRegister.push_back("untitled");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::estConnSB(int i)
{
    connect(ui->tabWidget->widget(i)->findChild<QTextEdit *>(), &QTextEdit::cursorPositionChanged, this, &MainWindow::showCursorPos);
    connect(ui->tabWidget->widget(i)->findChild<QTextEdit *>(), &QTextEdit::textChanged, this, &MainWindow::updateTextInfo);
    showCursorPos();
}

void MainWindow::severConnSB(int i)
{
    disconnect(ui->tabWidget->widget(i)->findChild<QTextEdit *>(), &QTextEdit::cursorPositionChanged, this, &MainWindow::showCursorPos);
    disconnect(ui->tabWidget->widget(i)->findChild<QTextEdit *>(), &QTextEdit::textChanged, this, &MainWindow::updateTextInfo);
    ui->statusBar->clearMessage();
}

void MainWindow::wordCount()
{
    QString txt = ui->tabWidget->widget(currentTab)->findChild<QTextEdit *>()->toPlainText();
    int len = txt.length(), i;
    if (len == 0)
        sz = ctr = strt = fin = 0;
    if (len - sz == 1)
    {
        if (len == 1 && !txt.at(0).isSpace())
            ctr++;

        if (len > 2){
            strt = len - 1;
            fin = len;
            if (txt.at(strt-1).isSpace() && !txt.at(fin-1).isSpace())
                ctr++;
        }
    }
    else if (len - sz > 1){
        ctr = 0;
        if (!txt.at(0).isSpace())
            ctr++;
        for (i = 1; i < len; i++)
            if (!txt.at(i).isSpace() && txt.at(i-1).isSpace())
                ctr++;
    }
}

void MainWindow::updateTextInfo()
{
    if (ui->tabWidget->widget(currentTab)->findChild<QTextEdit *>()->textCursor().blockNumber() == 0 && ui->tabWidget->widget(currentTab)->findChild<QTextEdit *>()->textCursor().columnNumber() == 0)
    {
        ui->tabWidget->widget(currentTab)->findChild<QTextEdit *>()->setTextColor(QColor(238,238,238));
        ui->tabWidget->widget(currentTab)->findChild<QTextEdit *>()->setFontPointSize(10);
    }
    wordCount();
    if (ctr == 1)
        ui->statusBar->showMessage(ui->statusBar->currentMessage() + QString(" | %1 Word").arg(ctr));
    else
        ui->statusBar->showMessage(ui->statusBar->currentMessage() + QString(" | %1 Words").arg(ctr));
}

void MainWindow::showCursorPos()
{
    int line = ui->tabWidget->widget(currentTab)->findChild<QTextEdit *>()->textCursor().blockNumber()+1;
    int pos = ui->tabWidget->widget(currentTab)->findChild<QTextEdit *>()->textCursor().columnNumber()+1;
    ui->statusBar->showMessage(QString("Ln %1, Col %2").arg(line).arg(pos));
}

void MainWindow::updateWinName(){
    if(currentFile.isEmpty())
        setWindowTitle("GTSA");
    else
        setWindowTitle(currentFile+" - GTSA");
}

void MainWindow::on_findNext_clicked()
{
    QString word = ui->lineEdit->text();
    ui->tabWidget->widget(currentTab)->findChild<QTextEdit *>()->find(word, QTextDocument::FindWholeWords);
    QTextCursor textCursor = ui->tabWidget->widget(currentTab)->findChild<QTextEdit *>()->textCursor();
    textCursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor, 1);
}

void MainWindow::on_actionNew_triggered()
{
    currentFile.clear();
    tabLocationRegister.push_back("untitled");
    updateWinName();
    ui->tabWidget->addTab(new textEditor(), QString("untitled"));
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);
}

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open the file"), slash = "/";
    QFile f(fileName);
    if (!f.open(QIODevice::ReadOnly | QFile::Text)){
        f.close();
        return;
    }
    if (tabLocationRegister.empty())
        on_actionNew_triggered();
    currentFile = fileName;
    tabLocationRegister[currentTab] = currentFile;
    ui->tabWidget->setTabText(currentTab, fileName.mid(fileName.lastIndexOf(slash)+1, -1));
    updateWinName();
    QTextStream in(&f);
    QString text = in.readAll();
    ui->tabWidget->widget(currentTab)->findChild<QTextEdit *>()->setText(text);
    f.close();
}

void MainWindow::on_actionSave_triggered()
{
    QString fileName, slash = "/";
    // If we don't have a filename from before, get one.
    if (currentFile.isEmpty()) {
        fileName = QFileDialog::getSaveFileName(this, "Save");
        currentFile = fileName;
        tabLocationRegister[currentTab] = currentFile;
    } else {
        fileName = currentFile;
    }
    QFile f(fileName);
    if (!f.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot save file: " + f.errorString());
        return;
    }
    updateWinName();
    ui->tabWidget->setTabText(currentTab, fileName.mid(fileName.lastIndexOf(slash)+1, -1));
    QTextStream out(&f);
    QString text = ui->tabWidget->widget(currentTab)->findChild<QTextEdit *>()->toPlainText();
    out << text;
    f.close();
}

void MainWindow::on_actionSave_As_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save As..."), slash = "/";
    QFile f(fileName);
    if(!f.open(QFile::WriteOnly | QFile::Text)){
        QMessageBox::warning(this, "Warning", "Cannot save file: " + f.errorString());
        return;
    }
    currentFile = fileName;
    tabLocationRegister[currentTab] = currentFile;
    updateWinName();
    ui->tabWidget->setTabText(currentTab, fileName.mid(fileName.lastIndexOf(slash)+1, -1));
    QTextStream out(&f);
    QString text = ui->tabWidget->widget(currentTab)->findChild<QTextEdit *>()->toPlainText();
    out << text;
    f.close();
}

void MainWindow::on_actionPrint_triggered()
{
    QPrinter printDev;
    QPrintDialog dialog(&printDev, this);
    if (dialog.exec() == QDialog::Rejected){
        return;
    }
    ui->textEdit->print(&printDev);
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionCut_triggered()
{
    ui->tabWidget->widget(currentTab)->findChild<QTextEdit *>()->cut();
}

void MainWindow::on_actionCopy_triggered()
{
    ui->tabWidget->widget(currentTab)->findChild<QTextEdit *>()->copy();
}

void MainWindow::on_actionPaste_triggered()
{
    ui->tabWidget->widget(currentTab)->findChild<QTextEdit *>()->paste();
}

void MainWindow::on_actionUndo_triggered()
{
    ui->tabWidget->widget(currentTab)->findChild<QTextEdit *>()->undo();
}

void MainWindow::on_actionRedo_triggered()
{
    ui->tabWidget->widget(currentTab)->findChild<QTextEdit *>()->redo();
}

void MainWindow::on_actionAbout_GTSA_triggered()
{
    QMessageBox about;
    about.setText("Graphical Text Sentiment Analyzer");
    about.setInformativeText("Text Editor + Sentimental Analyzer\nDeveloped by: kushaangupta");
    about.setIconPixmap(QPixmap(":res/gtsaLogo.png").scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    about.setStandardButtons(QMessageBox::Close);
    about.show();
    about.exec();
}

void MainWindow::on_lineEdit_returnPressed()
{
    on_findNext_clicked();
}

void MainWindow::on_tabWidget_tabCloseRequested(int i)
{
    recentlyClosed = true;
    tabLocationRegister.removeAt(i);
    ui->tabWidget->removeTab(i);
}

void MainWindow::on_actionClose_Tab_triggered()
{
    on_tabWidget_tabCloseRequested(currentTab);
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    //qInfo("%i", currentTab);
    if (currentTab >= 0 && !recentlyClosed)
        severConnSB(currentTab);
    recentlyClosed = false;
    currentTab = index;
    if (currentTab >= 0)
        estConnSB(index);
    else
        ui->statusBar->clearMessage();
    //qInfo("%i", currentTab);
    sz = 0; ctr = 0; strt = 0; fin = 0;
    if (currentTab == -1)
        currentFile.clear();
    else
        currentFile = tabLocationRegister[currentTab];
    //qDebug() << "File: " << currentFile;
    updateWinName();
}

void MainWindow::on_actionAnalyze_triggered()
{
    on_actionSave_triggered();
    QString qs = "python " + SENTIMENTAL_ANALYSIS_SCRIPT + " " + tabLocationRegister[currentTab]; //+ " >> txt.txt"; [redirect the output]

    FILE *in = popen(qPrintable(qs), "r"); //file ptr
    qs = "";
    char buff[255];
    while(fgets(buff, 255, in))
        qs = qs + buff;
    fclose(in);
    //qInfo("%s", qPrintable(qs));
    int numSentences = qs.count("\n"), len = qs.length();
    QVector<double> y;
    ui->statusBar->showMessage(ui->statusBar->currentMessage() + QString(" | %1 Sentences").arg(numSentences));
    for(int i = 0, j = 0; j < len; j++)
        if (qs.at(j) == '\n'){
            y.push_back(qs.mid(i, j-i).toDouble());
            i = j+1;
        }

    QLineSeries *series = new QLineSeries();
    for (int i = 0; i < numSentences; i++)
        series->append(i, y[i]);
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Sentimental Analysis");
    chart->setAnimationOptions(QChart::NoAnimation);
    series->setUseOpenGL(true);
    chart->legend()->setVisible(false);
    //chart->legend()->setAlignment(Qt::AlignBottom);
    QValueAxis *axisX = new QValueAxis(), *axisY = new QValueAxis();
    //axisX->setTickCount(numSentences);

    axisY->setRange(-100, 100);
    axisY->setTickCount(41);

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisX);
    series->attachAxis(axisY);

    QChartView *chartView = new QChartView(chart);
    //chartView->setRenderHint(QPainter::Antialiasing);
    chartView->resize(800, 600);
    chartView->show();
    QPixmap p = chartView->grab();
    p.save("test.png", "PNG");
}

void MainWindow::on_closeFind_clicked()
{
    //closefindbar
}
