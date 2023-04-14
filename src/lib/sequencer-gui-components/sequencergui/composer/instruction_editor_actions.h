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

#include <sequencergui/composer/composer_context.h>

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

//! Collection of actions

class InsrtuctionEditorActions : public QObject
{
  Q_OBJECT

public:
  explicit InsrtuctionEditorActions(SequencerModel* model = nullptr, QObject* parent = nullptr);
  ~InsrtuctionEditorActions() override;

  void SetModel(SequencerModel* model);

  void SetContext(ComposerContext context);

  void SetMessageHandler(std::unique_ptr<sup::gui::MessageHandlerInterface> message_handler);

  void OnInsertInstructionAfterRequest(const QString& item_type);

  void OnInsertInstructionIntoRequest(const QString& item_type);

  void OnRemoveInstructionRequest();

  void OnInsertVariableAfterRequest(const QString& item_type);

  void OnRemoveVariableRequest();

private:
  ::mvvm::SessionItem* InsertItem(const std::string& item_type, ::mvvm::SessionItem* parent,
                                  const ::mvvm::TagIndex& index);
  void ValidatePrecoditions();

  SequencerModel* m_model{nullptr};
  ComposerContext m_context;
  std::unique_ptr<sup::gui::MessageHandlerInterface> m_message_handler;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_COMPOSER_INSTRUCTION_EDITOR_ACTIONS_H_
