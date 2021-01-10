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
#include "vscinfooverlay.h"

VSCInfoOverlay::VSCInfoOverlay(QWidget *parent) :QWidget(parent) {
    setPalette(Qt::transparent);
    setAttribute(Qt::WA_TransparentForMouseEvents);
    currentText = "No file opened.";
    this->setFixedHeight(30);
    updateSize();
}

void VSCInfoOverlay::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QFont font;
    font.setPixelSize(13);
    painter.setFont(font);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(Qt::green));
    painter.drawText(QRect(10,5,950,16),currentText);
}

void VSCInfoOverlay::setText(QString text) {
    currentText = text;
    this->update();
}

void VSCInfoOverlay::updateSize() {
    this->setMinimumWidth(this->parentWidget()->size().rwidth()-60);
}



