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
#ifndef __VSC_IMAGEVIEWER_H__
#define __VSC_IMAGEVIEWER_H__

#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QImageReader>
#include <QColor>
#include <QPalette>
#include <QTimer>
#include <QDebug>
#include <QErrorMessage>
#include <vector>
#include "utility.hpp"
#include "vscmapoverlay.h"
#include "vscinfooverlay.h"
#include "vsccontrolsoverlay.h"
#include <time.h>

class VSCImageViewerPrivate;

class VSCImageViewer : public QWidget
{
    Q_OBJECT

public:
    VSCImageViewer();
    VSCImageViewer(QWidget* parent);
    VSCImageViewer(QWidget* parent, QImage& image);
    ~VSCImageViewer();
    void setImage(QImage& image);
    bool getImageSize(s32 &w, s32 &h, s32 &wScale, s32 &hScale);
    VSCControlsOverlay* getControls();
    bool isDisplaying();


signals:
    void sendDoubleClick();
    void imageChanged();

public slots:
    void slotFitNormal();
    void slotFitWidth();
    void slotFitAll();
    void slotZoomIn();
    void slotZoomOut();
    void slotSetInfoString(QString);
    void slotShowInfo(bool);
    void slotShowControls(bool);

private slots:
    void onAnimation();

protected:
    virtual void paintEvent(QPaintEvent* event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void resizeEvent(QResizeEvent* event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);


private:
    void fitDefault();
    void fitHorizontal();
    void fitVertical();
    void fitOriginal();
    void fitWidth();
    void fitAll();
    void setScale(float scale);
    void centerImage();
    VSCImageViewerPrivate* d;
};

#endif // __VSC_IMAGEVIEWER_H__
