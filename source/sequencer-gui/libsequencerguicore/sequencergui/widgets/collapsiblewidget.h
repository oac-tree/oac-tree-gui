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

#ifndef SEQUENCERGUI_WIDGETS_COLLAPSIBLEWIDGET_H
#define SEQUENCERGUI_WIDGETS_COLLAPSIBLEWIDGET_H

#include <QWidget>

class QSplitter;
class QToolBar;

namespace sequencergui
{

class CollapsibleToolBar;

class CollapsibleWidget : public QWidget
{
  Q_OBJECT

public:
  explicit CollapsibleWidget(QWidget* context, QWidget* parent = nullptr);
  ~CollapsibleWidget() override;

  void AddToSplitter(QSplitter* splitter);

  CollapsibleToolBar* GetToolBar();

  QToolBar* toolBar();

  void SetText(const QString& text);

private:
  CollapsibleToolBar* m_tool_bar{nullptr};  // ToolBar intended to go to splitter separately
};

void AddToSplitter(QSplitter* splitter, QWidget* context);

}  // namespace sequencergui

#endif  // SEQUENCERGUI_WIDGETS_COLLAPSIBLEWIDGET_H
