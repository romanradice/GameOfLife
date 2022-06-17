#include "mylife.h"
#include "ui_mylife.h"

#include <QtWidgets>
#include <QVector>

mylife::mylife(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::mylife)
{
    ui->setupUi(this);
    windowsize();
    setup_size();
    timer = new QTimer(this);  //SETUP timer
    qRegisterMetaTypeStreamOperators<QVector<QVector<int>>>("<QVector<QVector<int>>>"); //FOR open grid
    settings = new QSettings(pathfile,QSettings::IniFormat); // create qsettings
    connect(); //QOBJECT::CONNECT GROUPS FUNCTIONS
    affichage_fichier(); //afficher les modèles
    label_update();
}

mylife::~mylife()
{
    delete ui;
}

//CONNECT
/*------------------------------------------CONNECT------------------------------------------*/
void mylife::connect(){
    QObject::connect(ui->tablife,SIGNAL(cellEntered(int,int)),this,SLOT(mouse_enter_grid(int,int)));
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(timer_game()));
    QObject::connect(ui->Start,SIGNAL(clicked()),this,SLOT(timer_start()));//Start Timer
    QObject::connect(ui->Stop,SIGNAL(clicked()),this,SLOT(timer_stop()));//Stop Timer
    QObject::connect(ui->Vitesse,SIGNAL(valueChanged(int)),this,SLOT(timer_speed(int)));
    QObject::connect(ui->dimension_grille,SIGNAL(valueChanged(int)),this,SLOT(change_grid_size(int)));

    QObject::connect(ui->load_list,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(list_item_clicked_load(QListWidgetItem*)));
    QObject::connect(ui->load_list,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(double_item_clicked_remove(QListWidgetItem*)));

    QObject::connect(ui->actionEnregistrer,SIGNAL(triggered()),this,SLOT(act_save()));
    QObject::connect(ui->actionNouveau,SIGNAL(triggered()),this,SLOT(new_grid()));
    QObject::connect(ui->actionQuitter,SIGNAL(triggered()),qApp,SLOT(quit()));
}
/*------------------------------------------CONNECT------------------------------------------*/

/*------------------------------------------TIMER------------------------------------------*/
//TIMER
void mylife::timer_speed(int speed){
    speedtimer = speed;
    timer->setInterval(speed);
}
void mylife::timer_start(){
    timer->start(speedtimer);
    ui->tablife->setShowGrid(false);//HIDE grid
    in_game = true;
}
//Clicked in button
void mylife::timer_stop(){
    timer->stop();
    generation = 0;//Set gen at 0;
    ui->tablife->setShowGrid(true);//SHOW grid
    in_game = false;
}
void mylife::timer_game(){
    stape_life();
    generation++;// nb_generation
    label_update();//Update label
}
/*------------------------------------------TIMER------------------------------------------*/

/*------------------------------------------SAVE------------------------------------------*/
//SAUVEGARDE
//SLOT
void mylife::act_save(){
    name_file = QInputDialog::getText(this,"Enregistrer fichier","Nom fichier :");
    save_tab();
    affichage_fichier();//update qlistwidget
}

void mylife::save_tab(){
    settings->beginGroup(name_file);
    settings->setValue("tablive",QVariant::fromValue<QVector<QVector<int>>>(tablive));
    settings->setValue("size",size);
    settings->setValue("population",population);
    settings->endGroup();
}
/*------------------------------------------SAVE------------------------------------------*/

/*------------------------------------------LOAD------------------------------------------*/
//SLOTS FUNCTION
//Use when clicked in one item of qlistwidget
//load grid
void mylife::list_item_clicked_load(QListWidgetItem * item){
    name_file = item->text();
    load_tab();
    setup_size();
    for (int x = 0;x < size;x++){
        for (int y = 0;y < size;y++){
            modify_color_grid(y,x); //update color of grid
        }
    }
    label_update();
    ui->dimension_grille->setValue(size);
}
void mylife::load_tab(){
    settings->beginGroup(name_file);
    tablive = settings->value("tablive").value<QVector<QVector<int>>>();
    size = settings->value("size").toInt();
    population = settings->value("population").toInt();
    settings->endGroup();
}
/*------------------------------------------LOAD------------------------------------------*/

/*------------------------------------------REMOVE------------------------------------------*/
void mylife::double_item_clicked_remove(QListWidgetItem * item){
    int reponse = QMessageBox::question(this, "Supprimer modèle", "Voulez-vous vraiment supprimer le modèle ?", QMessageBox::Yes | QMessageBox::No);
    if (reponse == QMessageBox::Yes){
        name_file = item->text();
        settings->remove(name_file);
        affichage_fichier();//update qlistwidget
    }
}
/*------------------------------------------REMOVE------------------------------------------*/

/*------------------------------------------VALUE AND COLOR WHEN CLICKED GRID------------------------------------------*/
bool mylife::eventFilter(QObject * obj, QEvent* event){
    QMouseEvent *mev = static_cast<QMouseEvent*>(event);
    if(event->type() == QEvent::MouseButtonPress && (obj == ui->tablife || obj == ui->tablife->viewport()))
    {
        if(mev->button() == Qt::LeftButton)leftpress = true;
        else if(mev->button() == Qt::RightButton)rightpress = true;

    }
    if(event->type() == QEvent::MouseButtonRelease && (obj == ui->tablife || obj == ui->tablife->viewport()))
    {
        if(mev->button() == Qt::LeftButton)leftpress = false;
        else if(mev->button() == Qt::RightButton)rightpress = false;
    }
    if(rightpress && !in_game){
        if(tablive[x_case][y_case] != 0)population--;//label pop --
        tablive[x_case][y_case] = 0;
        modify_color_grid(y_case,x_case);
        label_update();

     }
    if(leftpress && !in_game){
        if(tablive[x_case][y_case] != 1)population++;//LABEL pop ++
        tablive[x_case][y_case] = 1;
        modify_color_grid(y_case,x_case);
        label_update();

     }
    return false;
}
//when enter in cell of the qtablewidget (tablife)
void mylife::mouse_enter_grid(int y,int x){
    x_case = x;
    y_case = y;
}
//USE WHEN CLICKED AND IN THE INITIALISATION OF THE GRID ITEM
//Color item case
void mylife::modify_color_grid(int y,int x){
    if (tablive[x][y] == 0)ui->tablife->item(y,x)->setBackground(Qt::black);
    else if (tablive[x][y] == 1)ui->tablife->item(y,x)->setBackground(Qt::red);
}
/*------------------------------------------VALUE AND COLOR WHEN CLICKED GRID------------------------------------------*/

/*start------------------------------------------RESIZE------------------------------------------*/
void mylife::setup_size(){
    nb_cellules = size*size;
    setup_size_tab_bool();
    setup_grid_size();
    setup_item_grid();
}
//RESIZE
//use when change value of dimension spinbox
//set new size to tablive,ui->tablife
void mylife::change_grid_size(int newsize){
    size = newsize;
    setup_size();
    label_update();
}
//RESIZE
//Use in initialisation and for resize
//Resize the tablive
void mylife::setup_size_tab_bool(){
    tablive.resize(size);
    for (int i = 0;i < size;i++){
        tablive[i].resize(size);
    }
}
//Use in initialisation and for resize
//Resize the grid and set size
void mylife::setup_grid_size(){
    ui->tablife->setMouseTracking(true);
    ui->tablife->viewport()->setMouseTracking(true);
    ui->tablife->installEventFilter(this);
    ui->tablife->viewport()->installEventFilter(this);

    ui->tablife->setColumnCount(size); //set size grid column
    ui->tablife->setRowCount(size);    //set size grid row

    ui->tablife->horizontalHeader()->setDefaultSectionSize((heigtsizescreen-80)/size); //x = 500,y = 500
    ui->tablife->verticalHeader()->setDefaultSectionSize((heigtsizescreen-80)/size); //x = 500,y = 500
}

void mylife::windowsize(){
    QSize size = qApp->screens()[0]->size();
    heigtsizescreen = size.height()-100;

    setFixedSize(heigtsizescreen+400,heigtsizescreen);
    ui->tablife->setGeometry(200,10,heigtsizescreen-80,heigtsizescreen-80);
    ui->frame->setGeometry(200+heigtsizescreen-80+60,10,120,heigtsizescreen-80);
    ui->load_list->setGeometry(0,20,170,(heigtsizescreen-80));
}
/*end------------------------------------------RESIZE------------------------------------------*/

/*------------------------------------------QTABLEWIDGET------------------------------------------*/
//Use in the initialisation and for resize
// Create new item for each case in the grid
void mylife::setup_item_grid(){
    for(int x =0;x < ui->tablife->columnCount();x++){
        for(int y =0;y < ui->tablife->rowCount();y++){
            ui->tablife->setItem(y,x,new QTableWidgetItem);//create a new item for each case
            modify_color_grid(y,x);//Color case of each item

            //ui->tablife->item(x,y)->setText(0); //later maybe
        }
    }
}
/*------------------------------------------QTABLEWIDGET------------------------------------------*/

/*------------------------------------------LABEL------------------------------------------*/
//LAbel functions
void mylife::label_update(){
    ui->gen_label->setNum(generation);
    ui->pop_label->setNum(population);
    ui->nb_cellules_label->setNum(nb_cellules);
}
/*------------------------------------------LABEL------------------------------------------*/

/*------------------------------------------QLISTWIDGET------------------------------------------*/
//QLIST
void mylife::affichage_fichier(){
    ui->load_list->clear();
    QStringList groups = settings->childGroups();
    for (int i = 0;i < groups.size();i++){
        QListWidgetItem *item = new QListWidgetItem();
        //item->setIcon();
        item->setText(groups[i]);
        ui->load_list->addItem(item);
    }
}
/*------------------------------------------QLISTWIDGET------------------------------------------*/

/*------------------------------------------CELL DIED OR ALIVE------------------------------------------*/
//GAME FUNCTION
//Use for detect if a cell is dead or alive
//Activate by a timer
void mylife::stape_life(){
    int alive = 0;
    QVector<QVector<int>> tablivet1 = tablive;
    for (int x = 0; x < ui->tablife->columnCount();x++){
        for(int y = 0;y < ui->tablife->rowCount();y++){
            alive = 0;
            if (x != 0){                                //detect if case in the left is alive
                if (tablive[x-1][y] == 1)alive++;
            }
            if (x != ui->tablife->columnCount()-1){       //detect if case in the right is alive
                if (tablive[x+1][y] == 1)alive++;
            }
            if (y != 0){                                //detect if case in the top is alive
                if (tablive[x][y-1] == 1)alive++;
            }
            if (y != ui->tablife->rowCount()-1){          //detect if case in the down is alive
                if (tablive[x][y+1] == 1)alive++;
            }
            if (x != 0 && y != 0){                      //detect if case in the left-top is alive
                if (tablive[x-1][y-1] == 1)alive++;
            }
            if (x != 0 && (y != ui->tablife->rowCount()-1)){   //detect if case in the left-down is alive
                if (tablive[x-1][y+1] == 1)alive++;
            }
            if (x != ui->tablife->columnCount()-1 && y != 0){ //detect if case in the right-top is alive
                if (tablive[x+1][y-1] == 1)alive++;
            }
            if (x != ui->tablife->columnCount()-1 && y != ui->tablife->rowCount()-1){   //detect if case in the right_down is alive
                if (tablive[x+1][y+1] == 1)alive++;
            }

            //died or lived
            // 0 = dead
            // 1 = alived
            if (tablive[x][y] == 0){
                if (alive == 3){
                    tablivet1[x][y] = 1;
                    population++;
                }
            }
            if (tablive[x][y] == 1){
                if (alive != 2 && alive != 3){
                    tablivet1[x][y] = 0;
                    population--;
                }
            }
            modify_color_grid(y,x); //update color item's grid
        }
    }
    tablive = tablivet1; 
}
/*------------------------------------------CELL DIED OR ALIVE------------------------------------------*/

/*------------------------------------------QACTION------------------------------------------*/
void mylife::new_grid(){
    population = 0;
    for (int x = 0;x < size;x++){
        for (int y = 0;y < size;y++){
            tablive[x][y] = 0;
            modify_color_grid(y,x);
        }
    }
    label_update();
}

/*------------------------------------------QACTION------------------------------------------*/
