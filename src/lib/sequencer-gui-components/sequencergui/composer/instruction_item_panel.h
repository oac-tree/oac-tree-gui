/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_COMPOSER_INSTRUCTION_ITEM_PANEL_H_
#define SEQUENCERGUI_COMPOSER_INSTRUCTION_ITEM_PANEL_H_

#include <QWidget>

namespace sequencergui
{

class ItemListWidget;

//! Collapsible list with instructions available to drag and drop on graphics scene.

class InstructionItemPanel : public QWidget
{
  Q_OBJECT

public:
  explicit InstructionItemPanel(QWidget* parent = nullptr);

signals:
  void InstructionDoubleClicked(const QString& name);

private:
  ItemListWidget* m_list_widget{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_COMPOSER_INSTRUCTION_ITEM_PANEL_H_
