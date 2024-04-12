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

class ToolKitViewModel;
class InsructionToolKitTreeView;

/**
 * @brief The InstructionItemPanel class is a tree with instruction type names grouped according to
 * their plugin origin.
 *
 * It is located on the left-hand-side of SequencerComposerView.
 */
class InstructionItemPanel : public QWidget
{
  Q_OBJECT

public:
  explicit InstructionItemPanel(QWidget* parent = nullptr);

signals:
  void InstructionDoubleClicked(const QString& name);

private:
  ToolKitViewModel* m_instruction_toolkit_viewmodel{nullptr};
  InsructionToolKitTreeView* m_tree_view{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_COMPOSER_INSTRUCTION_ITEM_PANEL_H_
