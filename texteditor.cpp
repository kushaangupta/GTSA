#include "texteditor.h"
#include "ui_texteditor.h"

textEditor::textEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::textEditor)
{
    ui->setupUi(this);
}

textEditor::~textEditor()
{
    delete ui;
}

