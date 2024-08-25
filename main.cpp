#include "widget.h"

#include <QApplication>
#include <QSystemTrayIcon>  //系统菜单托盘


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;

    QSystemTrayIcon systemTrayIcon(QIcon("://img/WK.jpg"),&w);
    QMenu menu;
    auto showAct=new QAction("show",&systemTrayIcon);
    auto exitAct=new QAction("exit",&systemTrayIcon);

    QObject::connect(showAct,&QAction::triggered,[&](){
        w.setVisible(true);
    });
    QObject::connect(exitAct,&QAction::triggered,[&](){
        QApplication::quit();
    });
    menu.addAction(showAct);
    menu.addAction(exitAct);
    systemTrayIcon.setContextMenu(&menu);
    systemTrayIcon.show();
    w.show();
    return a.exec();
}
