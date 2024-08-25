#include "widget.h"
#include <QPaintEvent>
#include <QPainter> //绘图用
#include <QPixmap>  //存储图片用
#include <QCursor>  //光标
#include "iostream"
#include <QMetaEnum>
#include <QDebug>

Widget::Widget(QWidget *parent)
        : QWidget(parent),
          timer(new QTimer(this)),
          menu(new QMenu(this)){
    this->setWindowFlag(Qt::FramelessWindowHint);  //设置窗口标志,窗口标志用于控制窗口的外观和行为 //去除窗口边框
    this->setAttribute(Qt::WA_TranslucentBackground);  //设置窗口背景透明
    this->installEventFilter(new DragFilter);
    //想要实现窗口拖动的话，需要借助事件过滤器
    loadRoleActRes();
    initMenu();
    showAnimation(nxd_bhc);
    connect(timer, &QTimer::timeout, [this]() {
        static int index = 0;
        auto paths = this->action_map.value(this->cur_role_act);
        this->cur_role_pix = paths[index++ % paths.size()];
        this->update();  //update()内部会调用paintEvent()绘制
        //std::cout<<index<<std::endl;
    });  // [] 表示捕获列表

}

Widget::~Widget() {}

//绘制
void Widget::paintEvent(QPaintEvent *event) {

    QPainter painter(this);
    //painter.drawRect(100,100,200,200);
    QPixmap pix;
//    pix.load(this->cur_role_pix.toLocalFile());  //将 QUrl 对象转换为本地文件路径
//    painter.drawPixmap(0, 0, pix);
    if (!pix.load(this->cur_role_pix.toLocalFile())) {
        qDebug() << "Failed to load image from:" << this->cur_role_pix.toLocalFile();  // 调试输出
    } else {
        painter.drawPixmap(0, 0, pix);
    }
}

void Widget::contextMenuEvent(QContextMenuEvent *event) {
    //popup: 这是 QMenu 的一个方法，用于弹出菜单并显示在指定的位置
    //QCursor::pos(): 返回当前鼠标光标的位置。使用这个位置作为 popup 方法的参数，可以将菜单弹出在鼠标当前的位置。
    this->menu->popup(QCursor::pos());
}

void Widget::loadRoleActRes() {
    //[this]：捕获列表，表示 lambda 表达式可以访问类的成员函数和变量。
    auto addRes = [this](RoleAct k, QString path, int count) {
        QList<QUrl> paths;
        char buf[260];
        for (int i = 0; i < count; ++i) {
            memset(buf, 0, sizeof(buf));
            sprintf(buf, path.toStdString().c_str(), i);
            paths.append(QUrl::fromLocalFile((buf)));  //将本地文件路径转换为 QUrl 对象
        }
        action_map.insert(k, paths);
    };
    addRes(nxd_b, ":/nxd_b/img/nxd_b/b_%d.png", 25);
    addRes(nxd_bhc, ":/nxd_bhc/img/nxd_bhc/bhc_%d.png", 25);
    addRes(nxd_walk, ":/nxd_walk/img/nxd_walk/walk_%d.png", 25);
}

void Widget::showAnimation(RoleAct k) {
    timer->stop();
    this->cur_role_act = k;
    timer->start(60);
}

void Widget::initMenu() {
    //menu->addAction("nxd_b") 和 menu.addAction("nxd_b") 的区别在于 menu 是指针还是对象,  ->指针
    menu->addAction("nxd_b");
    menu->addAction("nxd_bhc");
    menu->addAction("nxd_walk");

    QAction *act = new QAction("Hide");
    connect(act, &QAction::triggered, [this]() {
        this->setVisible(false);
    });
    menu->addAction(act);

    connect(menu, &QMenu::triggered, this, &Widget::onMenuTriggered);
}

void Widget::onMenuTriggered(QAction *action) {
    QMetaEnum metaEnum = QMetaEnum::fromType<RoleAct>();  //字符串和枚举值可以通过这个互相转换了
    bool ok;
    int roleValue = metaEnum.keyToValue(action->text().toStdString().c_str(), &ok);
    if (ok)
    {
        showAnimation(static_cast<RoleAct> (roleValue));
    }}


