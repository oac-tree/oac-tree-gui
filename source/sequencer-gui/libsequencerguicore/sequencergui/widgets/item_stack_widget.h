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

#ifndef SEQUENCERGUI_WIDGETS_ITEM_STACK_WIDGET_H_
#define SEQUENCERGUI_WIDGETS_ITEM_STACK_WIDGET_H_

#include <QWidget>
#include <QList>

class QStackedWidget;
class QToolBar;
class QAction;

namespace sequencergui
{

//!

class ItemStackWidget : public QWidget
{
  Q_OBJECT

public:
  explicit ItemStackWidget(QWidget* parent = nullptr);

  void AddWidget(QWidget* widget, QToolBar* toolbar, bool toolbar_is_always_visible = false);

private:
  struct GuestToolBarData
  {
    QAction* action{nullptr}; //! action to hide guest toolbar
    bool is_always_visible{false};
  };

  QStackedWidget* m_stacked_widget{nullptr};
  QToolBar* m_toolbar{nullptr};
  QList<GuestToolBarData> m_toolbar_data;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_WIDGETS_ITEM_STACK_WIDGET_H_
