/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_MAINWINDOW_SPLASH_SCREEN_H_
#define SEQUENCERGUI_MAINWINDOW_SPLASH_SCREEN_H_

#include <QSplashScreen>

namespace sequencergui
{

/**
 * @brief The SplashScreen class is shown on main window startup.
 */
class SplashScreen : public QSplashScreen
{
  Q_OBJECT

public:
  explicit SplashScreen();

  void Start(int show_during);

protected:
  void drawContents(QPainter* painter) override;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MAINWINDOW_SPLASH_SCREEN_H_
