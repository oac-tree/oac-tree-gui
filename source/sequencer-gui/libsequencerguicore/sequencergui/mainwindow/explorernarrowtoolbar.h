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

#ifndef SEQUENCERGUI_MAINWINDOW_EXPLORERNARROWTOOLBAR_H
#define SEQUENCERGUI_MAINWINDOW_EXPLORERNARROWTOOLBAR_H

#include <QFrame>
#include <QList>

class QToolBar;
class QLabel;
class QToolButton;
class QAction;

namespace sequencergui
{

//! Instrument tool bar with possibility to collapse/expand attached widget.
//! Toolbar buttons are shown only when expanded.
//! Looks like this: > LABEL BUTTON BUTTON BUTTON

//! This toolbar is intended to function in splitters together with CollapsibleWidgets.

class ExplorerNarrowToolBar : public QFrame
{
  Q_OBJECT

public:
  ExplorerNarrowToolBar(QWidget* parent = nullptr);

  void SetText(const QString& text);

  void SetControlledWidget(QWidget* widget);

  void AddWidget(QWidget* widget);

private:
  void UpdateToolBar();
  void UpdateIcon();
  void InsertStrech();

  QToolBar* m_tool_bar{nullptr};
  QToolButton* m_expand_button{nullptr};
  QLabel* m_label{nullptr};
  bool m_expanded{true};
  QWidget* m_controlled_widget{nullptr};
  QList<QAction*> m_toolbar_actions;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MAINWINDOW_EXPLORERNARROWTOOLBAR_H
