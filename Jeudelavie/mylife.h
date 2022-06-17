#ifndef MYLIFE_H
#define MYLIFE_H

#include <QMainWindow>
#include <QVector>
#include <QSettings>
#include <QListWidget>
#include <QTimer>
#include <QMenu>
#include <QEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class mylife; }
QT_END_NAMESPACE

class mylife : public QMainWindow
{
    Q_OBJECT

public slots:
    //Mouse detect grid
    void mouse_enter_grid(int y,int x);

    //Timer
    void timer_start();
    void timer_stop();
    void timer_game();
    void timer_speed(int speed);

    //Spinbox dimension grid
    void change_grid_size(int newsize);

    //save && load
    void act_save();
    void list_item_clicked_load(QListWidgetItem * item);
    void double_item_clicked_remove(QListWidgetItem * item);

    //QACTION
    //list_item_clicked_load
    void new_grid();

public:
    mylife(QWidget *parent = nullptr);
    ~mylife();

    //INIATIALISATION
    void setup_size();

    //RESIZE GRID AND TABLE
    void setup_item_grid();
    void setup_size_tab_bool();
    void setup_grid_size();

    void windowsize();
    //grid clicked
    bool eventFilter(QObject * obj, QEvent * event);

    //GAME
    void modify_color_grid(int y,int x);
    void stape_life();

    //Label update
    void label_update();

    //Sauvegarde
    void save_tab();
    void load_tab();

    //QLIST
    void affichage_fichier();

    //Connect
    void connect();


private:
     Ui::mylife *ui;
     QVector<QVector<int>> tablive;
     QTimer *timer;
     //Sauvegarde
     QString name_file = "";
     QSettings *settings = 0;
     QString pathfile = "modele.ini";
     //OPTIONS
     int speedtimer = 500;
     int size = 20;
     bool in_game = false;
     int heigtsizescreen = 0;
     //mouse
     int x_case = 0;
     int y_case = 0;
     bool rightpress = false;
     bool leftpress = false;
     //LABEL variables
     int population = 0;
     int nb_cellules = 400;
     int generation = 0;


};
#endif // MYLIFE_H
