#ifndef DOCUMENT_PREVIEWER_H
#define DOCUMENT_PREVIEWER_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class document_previewer; }
QT_END_NAMESPACE

class document_previewer : public QWidget
{
    Q_OBJECT

public:
    document_previewer(QWidget *parent = nullptr);
    ~document_previewer();

private:
    Ui::document_previewer *ui;
};
#endif // DOCUMENT_PREVIEWER_H
