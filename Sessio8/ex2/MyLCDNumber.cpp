#include "MyLCDNumber.h"

MyLCDNumber::MyLCDNumber(QWidget* parent) : QLCDNumber(parent) {

}

void MyLCDNumber::color(int x) {
    if (x == 0) setStyleSheet("color: green;");
    else if (x%2 != 0) setStyleSheet("color: red;");
    else setStyleSheet("color: blue;");
}

void MyLCDNumber::set_zero() {
    setStyleSheet("color: green;");
    display(0);
}
