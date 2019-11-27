#include "MyLabel.h"

MyLabel::MyLabel(QWidget* parent) : QLabel(parent) {

}

void MyLabel::retalla(int x) {
    QString str = text();
    str.truncate(x);
    setText(str);
}
