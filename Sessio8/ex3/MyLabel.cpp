#include "MyLabel.h"
#include <iostream>

const std::string LINE1 = "Cotxes: ";
const std::string LINE2 = "\nMotos: ";
const std::string LINE3 = "\nCamions: ";
const std::string LINE4 = "\nPreu: ";

MyLabel::MyLabel(QWidget* parent) : QLabel(parent) {
    ncotxes = nmotos = ncamions = 0;
    obreTanca(false);
}

void MyLabel::addCamio() {
    if (not obert) return;
    ncamions++;
    setText(get_line());
}
void MyLabel::addMoto() {
    if (not obert) return;
    nmotos++;
    setText(get_line());
}

void MyLabel::addCotxe() {
    if (not obert) return;
    ncotxes++;
    setText(get_line());
}

void MyLabel::obreTanca(bool ob) {
    obert = ob;
    if (obert) setStyleSheet("background-color : rgba(0, 255, 0, 0.5);");
    else setStyleSheet("background-color : rgba(255, 0, 0, 0.5);");
    setText(get_line());
}

int MyLabel::calc_preu() {
    return ncotxes + nmotos + 4*ncamions;
}

QString MyLabel::get_line() {
    std::string str = LINE1 + std::to_string(ncotxes) + 
           LINE2 + std::to_string(nmotos) +
           LINE3 + std::to_string(ncamions) +
           LINE4 + std::to_string(calc_preu());
    return QString::fromStdString(str);
}
