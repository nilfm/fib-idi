#include "MyLabel.h"
#include <iostream>

MyLabel::MyLabel(QWidget* parent) : QLabel(parent) {
    pass1 = pass2 = "";
    color = 0;
}

void MyLabel::updatePass1(QString str) {
    pass1 = str;
    calcColor();
}

void MyLabel::updatePass2(QString str) {
    pass2 = str;
    calcColor();
}

void MyLabel::displayText() {
    setText(obtainText());
}

void MyLabel::changeColor() {
    if (color == 0) setStyleSheet("background-color : red;");
    else if (color == 1) setStyleSheet("background-color : yellow");
    else setStyleSheet("background-color : green"); 
}

void MyLabel::calcColor() {
    if (pass1 != pass2) color = 0;
    else if (pass1.size() >= 6) color = 2;
    else color = 1;
}

QString MyLabel::obtainText() {
    if (color == 0) return "Contrassenya no coincident";
    if (color == 1) return "Contrassenya de menys de 6 caràcters";
    return "Contrassenya vàlida";
}
