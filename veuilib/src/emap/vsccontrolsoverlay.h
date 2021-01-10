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
#ifndef __VSC_ControlsOverlay_H__
#define __VSC_ControlsOverlay_H__

#include <QWidget>
#include <QPainter>
#include <QPen>
#include <QDebug>
#include <QMouseEvent>

class VSCControlsOverlay : public QWidget
{
    Q_OBJECT
public:
    explicit VSCControlsOverlay(QWidget *parent = 0);
    void updateSize();

protected:
    void paintEvent(QPaintEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    QPen pen;
    
signals:
    void exitClicked();
    void exitFullscreenClicked();
    void minimizeClicked();
    
public slots:
};

#endif // __VSC_ControlsOverlay_H__
