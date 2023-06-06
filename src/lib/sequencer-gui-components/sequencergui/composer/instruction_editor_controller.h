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

#ifndef SEQUENCERGUI_COMPOSER_INSTRUCTION_EDITOR_CONTROLLER_H_
#define SEQUENCERGUI_COMPOSER_INSTRUCTION_EDITOR_CONTROLLER_H_

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

//! The InstructionEditorHandler class implements logic to add/remove instructions in
//! InstructionContainerItem.

class InstructionEditorController : public QObject
{
  Q_OBJECT

public:
  explicit InstructionEditorController(InstructionEditorContext context, QObject* parent = nullptr);
  ~InstructionEditorController() override;

  void OnInsertInstructionAfterRequest(const QString& item_type);

  void OnInsertInstructionIntoRequest(const QString& item_type);

  void OnRemoveInstructionRequest();

private:
  mvvm::SessionModelInterface* GetModel() const;
  InstructionContainerItem* GetInstructionContainer() const;
  void SendMessage(const std::string& text, const std::string& informative = {},
                   const std::string& details = {});

  ::mvvm::SessionItem* InsertItem(const std::string& item_type, ::mvvm::SessionItem* parent,
                                  const ::mvvm::TagIndex& index);

  InstructionEditorContext m_context;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_COMPOSER_INSTRUCTION_EDITOR_CONTROLLER_H_
