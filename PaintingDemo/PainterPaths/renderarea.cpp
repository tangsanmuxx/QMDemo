#include "renderarea.h"
#include <QDebug>
#include <qpainter.h>

RenderArea::RenderArea(QWidget *parent) : QWidget(parent)
{
    // 设置背景填充色
    this->setBackgroundRole(QPalette::Base);
    this->setAutoFillBackground(true);
    initPath();
}

void RenderArea::setFill(bool flag)
{
    m_isFill = flag;
}


void RenderArea::initPath()
{

    /****************** 第一行 ******************/
    // 多边形
    QPolygonF playgonf;
    playgonf.append(QPointF(20, 20));
    playgonf.append(QPointF(40, 0));
    playgonf.append(QPointF(100, 40));
    playgonf.append(QPointF(80, 80));
    playgonf.append(QPointF(40, 60));
    m_path.addPolygon(playgonf);

    // 椭圆（圆形）
    m_path.addEllipse(150, 10, 100, 100);

    // 矩形
    m_path.addRect(300, 10, 100, 60);
    // 圆形区域
    m_path.addRegion(QRegion(450, 10, 40, 50, QRegion::Ellipse));

    /****************** 第二行 ******************/
    // 圆角矩形
    m_path.addRoundRect(QRect(20, 150, 100, 100), 20, 20);

    // 添加绘制字体
    QFont font;
    font.setPixelSize(50);
    font.setBold(true);
    m_path.addText(QPoint(150, 200), font, "字体");
    // 在字体绘制起点绘制一条直线
    m_path.moveTo(QPoint(150, 200));
    m_path.lineTo(QPoint(250, 200));

    QRectF rectf(300,150,100,100);
    m_path.addRect(rectf);       // 绘制一个矩形，便于观察arcMoveTo函数效果
    //【类似moveTo函数功能】将画笔位置移动到矩形rectf中30度位置，0度为3点钟方向，正数代表逆时针，负数代表顺时针
    m_path.arcMoveTo(rectf, 30);
    QPointF pointf = m_path.currentPosition();  // 记录移动后的画笔位置
    m_path.arcTo(rectf, 90, 180); // 在rectf矩形中从90度位置开始画一个弧长为180度的弧线，画出来的弧线的起点再和arcMoveTo所移动的点相连接
    m_path.addEllipse(pointf.x() - 5, pointf.y() - 5, 10, 10);  // 绘制arcMoveToy移动后的画笔绘制

    // 添加子路径
    QPainterPath  path;
    path.moveTo(450, 150);
    path.lineTo(500, 200);
    path.lineTo(460, 220);
//    path.closeSubpath();      // 如果不关闭路径则path为一个折线，关闭路径后为一个三角形
#if 1
    m_path.addPath(path);
#else
    m_path.moveTo(550, 150);
    m_path.lineTo(570, 200);
    m_path.connectPath(path);  // connectPath方法会将m_path的画笔位置(最后一个点)和path的画笔位置(第一个点)连接起来
#endif

    /****************** 第三行 ******************/
    // 三次贝塞尔曲线
    m_path.moveTo(20, 300);
    m_path.cubicTo(QPointF(130, 300), QPointF(50, 350), QPointF(100, 400));

    // 两个路径填充区域相交位置
    QPainterPath path1;
    path1.addRect(150, 300, 100, 100);
    QPainterPath path2;
    path2.addEllipse(200, 280, 80, 80);
    m_path.addPath(path1.intersected(path2));   // 返回path1路径填充区域与path2路径填充区域相交的路径

    // 二次贝塞尔曲线
    m_path.moveTo(300, 300);
    m_path.quadTo(QPointF(400, 300), QPointF(360, 400));

    qDebug() << "绘制路径中元素的个数："         << m_path.elementCount();
    qDebug() << "获取第0个元素："              << m_path.elementAt(0);            // 相当于获取playgonf.append(QPointF(20, 20));这一个点
    qDebug() << "画家填充规则："               << m_path.fillRule();
    qDebug() << "传入矩形是否与路径m_path相交：" << m_path.intersects(QRect(150, 10, 100, 100));
    qDebug() << "传入路径是否与路径m_path相交：" << m_path.intersects(path1);
    qDebug() << "路径中是否没有元素："          << m_path.isEmpty();
    qDebug() << "当前路径的长度："             << path1.length();
    qDebug() << "指定长度len占整个路径百分比："  << path1.percentAtLength(100);
    qDebug() << "路径path1中25%位置的点："     << path1.pointAtPercent(0.25);
}


void RenderArea::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);  // 设置抗锯齿
//    painter.setBrush(Qt::cyan);                     // 设置笔刷

    if(!m_isFill)
    {
        painter.drawPath(m_path);
    }
    else
    {
        painter.fillPath(m_path, Qt::blue);
    }
//    painter.drawRect(m_path.boundingRect());     // 将绘制路径m_path的边界矩形返回并绘制，便于观察
    painter.drawRect(m_path.controlPointRect());   // 功能与boundingRect()相似
}
