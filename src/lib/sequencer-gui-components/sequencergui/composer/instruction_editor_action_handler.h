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

#ifndef SEQUENCERGUI_COMPOSER_INSTRUCTION_EDITOR_ACTION_HANDLER_H_
#define SEQUENCERGUI_COMPOSER_INSTRUCTION_EDITOR_ACTION_HANDLER_H_

#include <sequencergui/composer/instruction_editor_context.h>

#include <QObject>
#include <memory>

namespace mvvm
{

class SessionItem;
class TagIndex;
class SessionModelInterface;
}  // namespace mvvm

namespace sequencergui
{
class SequencerModel;
class InstructionContainerItem;

//! The InstructionEditorActionHandler class implements logic to add/remove instructions in
//! InstructionContainerItem.

class InstructionEditorActionHandler : public QObject
{
  Q_OBJECT

public:
  explicit InstructionEditorActionHandler(InstructionEditorContext context,
                                          QObject* parent = nullptr);
  ~InstructionEditorActionHandler() override;

  void OnInsertInstructionAfterRequest(const QString& item_type);

  void OnInsertInstructionIntoRequest(const QString& item_type);

  void OnRemoveInstructionRequest();

  void OnMoveUpRequest();

  void OnMoveDownRequest();

signals:
  void SelectItemRequest(mvvm::SessionItem* item);

private:
  InstructionItem* GetSelectedInstruction();

  mvvm::SessionModelInterface* GetModel() const;
  InstructionContainerItem* GetInstructionContainer() const;
  void SendMessage(const std::string& text, const std::string& informative = {},
                   const std::string& details = {});

  ::mvvm::SessionItem* InsertItem(const std::string& item_type, ::mvvm::SessionItem* parent,
                                  const ::mvvm::TagIndex& index);

  InstructionEditorContext m_context;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_COMPOSER_INSTRUCTION_EDITOR_ACTION_HANDLER_H_
