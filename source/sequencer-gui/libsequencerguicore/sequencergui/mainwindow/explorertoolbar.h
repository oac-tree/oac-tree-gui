/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_MAINWINDOW_EXPLORERTOOLBAR_H
#define SEQUENCERGUI_MAINWINDOW_EXPLORERTOOLBAR_H

#include <QToolBar>

class QToolBar;
class QLabel;
class QAction;

namespace sequencergui
{
//! Instrument tool bar with menu selector.

class ExplorerToolBar : public QToolBar
{
  Q_OBJECT

public:
  ExplorerToolBar(QWidget* parent = nullptr);

  void setText(const QString& text);

  void AddDotsMenu();

  void InsertElement(QWidget* widget);

private:
  void insertStrech();

  QLabel* m_label{nullptr};
  QAction* m_spacer_action{nullptr};
};

}  // namespace sequi

#endif  // SEQUENCERGUI_MAINWINDOW_EXPLORERTOOLBAR_H
