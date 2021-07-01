#include <QMouseEvent>

#include "clickableview.h"

ClickableView::ClickableView(QWidget *parent):QGraphicsView(parent) {}

void ClickableView::mousePressEvent(QMouseEvent *event) {

    QPointF temp = this->mapToScene(QPoint(event->pos().x(), event->pos().y()));
    QPoint clicked_coor = temp.toPoint();


    int x_position = clicked_coor.x();
    int y_position = clicked_coor.y();

    int new_row = floor(y_position / 45);
    int new_col = floor(x_position / 45);

    emit mouseClicked(new_row, new_col);
}
