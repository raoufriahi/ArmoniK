/*
 * Copyright (c) 2017-2018 Heimdall
 *
 * The computer program contained herein contains proprietary
 * information which is the property of Heimdall.
 * The program may be used and/or copied only with the written
 * permission of Heimdall or in accordance with the
 * terms and conditions stipulated in the agreement/contract under
 * which the programs have been supplied.
 */
#ifndef __VSC_MAPOVERLAY_H__
#define __VSC_MAPOVERLAY_H__

#include <QWidget>
#include <QPainter>
#include <QPen>
#include <QDebug>
#include <QPaintEvent>

class VSCMapOverlayPrivate;

class VSCMapOverlay : public QWidget
{
    Q_OBJECT
public:
    explicit VSCMapOverlay(QWidget *parent = 0);
    void updateMap(const QSize& windowRect, const QRect& drawingRect);
    void updatePosition();

protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    
private:
    VSCMapOverlayPrivate* d;
};

#endif // __VSC_VSCMapOverlay_H__
