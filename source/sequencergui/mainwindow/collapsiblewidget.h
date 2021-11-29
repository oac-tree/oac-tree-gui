/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_MAINWINDOW_COLLAPSIBLEWIDGET_H
#define SEQUENCERGUI_MAINWINDOW_COLLAPSIBLEWIDGET_H

#include <QWidget>

class QSplitter;

namespace sequi
{

class ExplorerNarrowToolBar;

class CollapsibleWidget : public QWidget
{
  Q_OBJECT

public:
  CollapsibleWidget(QWidget* parent = nullptr);
  ~CollapsibleWidget();

  void AddToSplitter(QSplitter* splitter);

  ExplorerNarrowToolBar* GetToolBar();

  void SetText(const QString& text);

private:
  ExplorerNarrowToolBar* m_tool_bar{nullptr}; // ToolBar intended to go to splitter separately
};

}  // namespace sequi

#endif  // SEQUENCERGUI_MAINWINDOW_COLLAPSIBLEWIDGET_H
