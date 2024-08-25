#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMap> //对
#include <QList>
#include <QUrl>
#include <QTimer>
#include <QMouseEvent>
#include <QContextMenuEvent>
#include <QMenu>
#include <QAction>

//在 C++ 中，当你在类中声明一个虚函数时，编译器需要知道该函数所使用的参数的类型。
class QPaintEvent;
//你必须确保 QPaintEvent 类型在你的代码中是可见的。否则，编译器将无法正确解析 QPaintEvent，从而导致编译错误。
/*可以通过以下两种方式之一来确保 QPaintEvent 类型是可见的：
 * 1.前向声明 (Forward Declaration): class QPaintEvent;
 * 2.包含头文件: #include <QPaintEvent>
 * */

namespace Act {
    Q_NAMESPACE
    enum RoleAct {
        nxd_b,
        nxd_bhc,
        nxd_walk
    };

    Q_ENUM_NS(RoleAct)
    //注册之后就可以实现枚举转字符串，字符串转枚举
}

using namespace Act;

class Widget : public QWidget {
Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);

    ~Widget();

protected:  //访问修饰符
    void paintEvent(QPaintEvent *event) override;  //重写paintEvent虚函数
    //paintEvent 是 QWidget 类中的一个虚函数。
    //当你重写 paintEvent 函数时，你实际上是在自定义如何在窗口部件上绘制内容。
   //contextMenuEvent 是 Qt 的事件处理函数，用于处理右键点击（上下文菜单）事件
    void contextMenuEvent(QContextMenuEvent *event) override;
private:
    void loadRoleActRes();
    void initMenu();

private:
    QMap<RoleAct, QList<QUrl>> action_map;
    QTimer *timer;
    RoleAct cur_role_act;  //当前动作
    QUrl cur_role_pix;  //当前动作的帧
    QMenu* menu;
public:
    void showAnimation(RoleAct k);
//槽函数: 是 Qt 中的一种特殊成员函数，用于响应信号（signals）。当信号发射时，连接到这个信号的槽函数将被自动调用。
public slots:  //public slots: 是 Qt 中的一个访问修饰符，用于声明槽（slots）函数。槽函数是 Qt 的信号与槽机制中的一部分，它们用于响应信号（signals）
    void onMenuTriggered(QAction* action);
};


class DragFilter : public QObject {
    //继承之后，重写Event函数
public:
    bool eventFilter(QObject *obj, QEvent *event) override  //处理事件过滤器的事件
    {
        //使用 dynamic_cast 操作符来将 obj 指针动态地转换为 QWidget 指针
        //dynamic_cast 是 C++ 的一个操作符，用于在运行时进行安全的类型转换
        /*eventFilter 函数必须返回一个 bool 类型的值来指示事件是否被处理
         * return true: 表示事件已经被完全处理
         * return false: 表示事件没有被处理，事件将继续传递给目标对象的默认处理函数或其他事件过滤器。
        */
        auto w = dynamic_cast<QWidget *>(obj);
        if (!w)
            return false;
        if (event->type() == QEvent::MouseButtonPress) {
            auto e = dynamic_cast<QMouseEvent *>(event);
            if (e)
            {
                pos=e->pos();
            }
        }
        else if (event->type() == QEvent::MouseMove) {
            auto e = dynamic_cast<QMouseEvent *>(event);
            if (e)
            {
                //buttons() 是 QMouseEvent 提供的一个成员函数，用于返回当前事件中哪些鼠标按钮被按下
                //& 是按位与（bitwise AND）操作符。它用于检查 buttons() 返回值中是否包含了 LeftButton 的标志。
                if (e->buttons()&Qt::MouseButton::LeftButton)
                {
                    w->move(e->globalPosition().toPoint()-pos);
                }
            }
        }
        //事件可以继续传递给其他事件过滤器或默认处理函数，确保事件链的完整性
        return QObject::eventFilter(obj,event);
    }

private:
    QPoint pos;
};

#endif // WIDGET_H
