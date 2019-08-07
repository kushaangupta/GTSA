#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <QWidget>

namespace Ui {
class textEditor;
}

class textEditor : public QWidget
{
    Q_OBJECT

public:
    explicit textEditor(QWidget *parent = nullptr);
    ~textEditor();

private:
    Ui::textEditor *ui;
};

#endif // TEXTEDITOR_H
