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
#ifndef __VSC_InfoOverlay_H__
#define __VSC_InfoOverlay_H__

#include <QWidget>
#include <QPainter>
#include <QPen>
#include <QDebug>

class VSCInfoOverlay : public QWidget
{
    Q_OBJECT
public:
    VSCInfoOverlay(QWidget *parent);
    void setText(QString text);
    void updateSize();

protected:
    void paintEvent(QPaintEvent *event);
    QString currentText;
};

#endif // __VSC_InfoOverlay_H__
