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

#ifndef SEQUENCERGUI_WIDGETS_ITEM_STACK_WIDGET_H_
#define SEQUENCERGUI_WIDGETS_ITEM_STACK_WIDGET_H_

#include <QList>
#include <QWidget>
#include <memory>

class QStackedWidget;
class QAction;
class QToolBar;
class QMenu;

namespace sequencergui
{

class PanelToolBar;

//! Stack of widgets with wide menubar and dots-menu at the right corner.

class ItemStackWidget : public QWidget
{
  Q_OBJECT

public:
  explicit ItemStackWidget(QWidget* parent = nullptr);
  ~ItemStackWidget() override;

  void AddWidget(QWidget* widget, std::unique_ptr<QToolBar> toolbar = {},
                 bool toolbar_is_always_visible = false);

  void AddWidget(QWidget* widget, const QList<QAction*>& actions,
                 bool toolbar_is_always_visible = false);

  void SetCurrentIndex(int index);

private:
  void AddMenuEntry(QWidget* widget);
  void AddGuestToolBar(std::unique_ptr<QToolBar> toolbar = {},
                       bool is_always_visible = false);

  void AddGuestActions(const QList<QAction*>& actions, bool is_always_visible = false);

  void UpdateToolBarVisibility();

  struct GuestToolBarData
  {
    QList<QAction*> actions;  //! actions to hide guest toolbar
    bool is_always_visible{false};
  };

  QStackedWidget* m_stacked_widget{nullptr};
  QList<GuestToolBarData> m_toolbar_data;
  std::unique_ptr<QMenu> m_widget_selection_menu;
  PanelToolBar* m_main_toolbar{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_WIDGETS_ITEM_STACK_WIDGET_H_
