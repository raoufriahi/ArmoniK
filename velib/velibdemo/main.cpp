/*
 * Copyright (c) 2017-2023 Heimdall
 *
 * The computer program contained herein contains proprietary
 * information which is the property of Heimdall.
 * The program may be used and/or copied only with the written
 * permission of Heimdall or in accordance with the
 * terms and conditions stipulated in the agreement/contract under
 * which the programs have been supplied.
 */
#include "velibdemo.h"
#include <QtWidgets/QApplication>
#include "debug.hpp"

int main(int argc, char *argv[])
{
	Debug::init(9100, "testlog");
	QApplication a(argc, argv);
	velibdemo w;
	w.show();
	return a.exec();
}
