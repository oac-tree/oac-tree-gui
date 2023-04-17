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

#ifndef SEQUENCERGUI_COMPOSER_INSTRUCTION_EDITOR_ACTIONS_H_
#define SEQUENCERGUI_COMPOSER_INSTRUCTION_EDITOR_ACTIONS_H_

#include <sequencergui/composer/instruction_editor_context.h>

#include <QObject>
#include <memory>

namespace mvvm
{
class SessionItem;
class TagIndex;
}  // namespace mvvm

namespace sup::gui
{
class MessageHandlerInterface;
}

namespace sequencergui
{
class SequencerModel;

//! The InstructionEditorActions class implements actions to add/remove instructions in
//! InstructionContainerItem.

class InstructionEditorActions : public QObject
{
  Q_OBJECT

public:
  explicit InstructionEditorActions(SequencerModel* model = nullptr, QObject* parent = nullptr);
  ~InstructionEditorActions() override;

  void SetModel(SequencerModel* model);

  void SetContext(InstructionEditorContext context);

  void SetMessageHandler(std::unique_ptr<sup::gui::MessageHandlerInterface> message_handler);

  void OnInsertInstructionAfterRequest(const QString& item_type);

  void OnInsertInstructionIntoRequest(const QString& item_type);

  void OnRemoveInstructionRequest();

private:
  ::mvvm::SessionItem* InsertItem(const std::string& item_type, ::mvvm::SessionItem* parent,
                                  const ::mvvm::TagIndex& index);
  void ValidatePrecoditions();

  SequencerModel* m_model{nullptr};
  InstructionEditorContext m_context;
  std::unique_ptr<sup::gui::MessageHandlerInterface> m_message_handler;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_COMPOSER_INSTRUCTION_EDITOR_ACTIONS_H_
