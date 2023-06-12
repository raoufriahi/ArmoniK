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
#ifndef VELIBDEMO_H
#define VELIBDEMO_H

#include <QtWidgets/QMainWindow>
#include <QThread>
#include "ui_velibdemo.h"
#include "vplay/vplay.hpp"

/**
 * \class velibdemo
 * \brief A demonstration class for the VeLib application.
 *
 * This class inherits from QMainWindow and QThread, and provides functionality for running the VeLib demo.
 */
class velibdemo : public QMainWindow, public QThread
{
    Q_OBJECT

public:
    /**
     * \brief Constructs a velibdemo object.
     * \param parent The parent widget (default = nullptr).
     */
    velibdemo(QWidget *parent = nullptr);

    /**
     * \brief Destroys the velibdemo object.
     */
    ~velibdemo();

public:
    /**
     * \brief The entry point for the thread.
     *
     * This function is called when the thread starts running and performs the main functionality of the demo.
     */
    void run();

private:
    Ui::velibdemoClass ui; /**< The user interface for the velibdemo class. */

    VPlay m_play; /**< An instance of the VPlay class used for playing audio. */
};



#endif // VELIBDEMO_H
