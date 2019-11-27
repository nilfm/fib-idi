#include <QLabel>

class MyLabel : public QLabel {
    Q_OBJECT

public:
    MyLabel(QWidget* parent);
public slots:
    void addCamio();
    void addMoto();
    void addCotxe();
    void obreTanca(bool);
private:
    int ncotxes, nmotos, ncamions;
    bool obert;
    int calc_preu();
    QString get_line();
};
