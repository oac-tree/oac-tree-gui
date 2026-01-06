/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef OAC_TREE_GUI_MAINWINDOW_SPLASH_SCREEN_H_
#define OAC_TREE_GUI_MAINWINDOW_SPLASH_SCREEN_H_

#include <QSplashScreen>

namespace oac_tree_gui
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

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_MAINWINDOW_SPLASH_SCREEN_H_
