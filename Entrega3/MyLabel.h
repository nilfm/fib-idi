#include <QLabel>

class MyLabel : public QLabel {
    Q_OBJECT

public:
    MyLabel(QWidget* parent);
public slots:
    void updatePass1(QString);
    void updatePass2(QString);
    void emitSignal();
signals:
	void changeText(QString);
private:
    QString pass1, pass2;
    QString text;
    int color; //0 - red, 1 - yellow, 2 - green
    void calcColor();
    QString obtainText();
    void changeColor();
};
