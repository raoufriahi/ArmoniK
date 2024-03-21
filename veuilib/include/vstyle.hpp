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
#ifndef __V_STYLE_HPP__
#define __V_STYLE_HPP__
#include "utility.hpp"
#include "debug.hpp"
#include "videotype.hpp"
#include <QWidget>
#include "server/factory.hpp"
#include <QThread>
#include <QProxyStyle>
#include <QStyleOption>

using namespace UtilityLib;

class VE_LIBRARY_API VStyle : public QProxyStyle
{
public:
	VStyle(QStyle *style = 0);

	int pixelMetric(PixelMetric metric, const QStyleOption* option = 0,
	const QWidget* widget = 0) const;
	void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = 0) const;


public slots:
	QIcon standardIcon(StandardPixmap standardIcon, const QStyleOption* option = 0,
		const QWidget* widget = 0) const;
};

#endif /* __V_V_IPC_MGR_HPP__ */
