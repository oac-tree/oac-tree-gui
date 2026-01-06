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

#ifndef OAC_TREE_GUI_MAINWINDOW_ABOUT_APPLICATION_DIALOG_H_
#define OAC_TREE_GUI_MAINWINDOW_ABOUT_APPLICATION_DIALOG_H_

#include <QDialog>
#include <memory>

class QBoxLayout;

namespace oac_tree_gui
{

/**
 * @brief The AboutApplicationDialog class is a dialog that pops-up on Help/About menu entry.
 */
class AboutApplicationDialog : public QDialog
{
  Q_OBJECT

public:
  AboutApplicationDialog(QWidget* parent_widget = nullptr);

private:
  std::unique_ptr<QBoxLayout> CreateLogoLayout();
  std::unique_ptr<QBoxLayout> CreateTextLayout();
  std::unique_ptr<QBoxLayout> CreateButtonLayout();
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_MAINWINDOW_ABOUT_APPLICATION_DIALOG_H_
