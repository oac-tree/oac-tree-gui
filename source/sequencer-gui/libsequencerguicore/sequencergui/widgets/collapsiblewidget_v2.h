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

#ifndef SEQUENCERGUI_WIDGETS_COLLAPSIBLEWIDGET_V2_H
#define SEQUENCERGUI_WIDGETS_COLLAPSIBLEWIDGET_V2_H

#include <QWidget>

class QSplitter;

namespace sequencergui
{

class CollapsibleToolBarV2;

class CollapsibleWidgetV2 : public QWidget
{
  Q_OBJECT

public:
  explicit CollapsibleWidgetV2(QWidget* context, QWidget* parent = nullptr);
  ~CollapsibleWidgetV2() override;

  void AddToSplitter(QSplitter* splitter);

  CollapsibleToolBarV2* GetToolBar();

  void SetText(const QString& text);

private:
  CollapsibleToolBarV2* m_tool_bar{nullptr};  // ToolBar intended to go to splitter separately
};

void AddToSplitter(QWidget* context, QSplitter* splitter);

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MAINWINDOW_COLLAPSIBLEWIDGET_H
