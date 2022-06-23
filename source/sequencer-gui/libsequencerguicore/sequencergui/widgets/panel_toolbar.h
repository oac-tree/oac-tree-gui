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

#ifndef SEQUENCERGUI_WIDGETS_PANEL_TOOLBAR_H_
#define SEQUENCERGUI_WIDGETS_PANEL_TOOLBAR_H_

#include <QToolBar>

class QToolBar;
class QLabel;
class QAction;
class QToolButton;
class QMenu;

namespace sequencergui
{
//! Instrument tool bar with menu selector.

class PanelToolBar : public QToolBar
{
  Q_OBJECT

public:
  explicit PanelToolBar(QWidget* parent = nullptr);

  void setText(const QString& text);

  void AddDotsMenu();

  QAction* InsertElement(QWidget* widget);

  void SetDotsMenu(QMenu* dots_menu);

  QAction* AppendSeparator();

private:
  void InsertStrech();

  QLabel* m_label{nullptr};
  QAction* m_spacer_action{nullptr};
  QToolButton* m_dots_button{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_WIDGETS_PANEL_TOOLBAR_H_
