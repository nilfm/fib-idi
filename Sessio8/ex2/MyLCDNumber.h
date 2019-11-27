#include <QLCDNumber>

class MyLCDNumber : public QLCDNumber {
    Q_OBJECT

public:
    MyLCDNumber(QWidget* parent);
public slots:
    void color(int x);
    void set_zero();
};
