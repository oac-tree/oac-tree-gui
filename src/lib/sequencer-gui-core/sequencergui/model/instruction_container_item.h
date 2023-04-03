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

#ifndef SEQUENCERGUI_MODEL_INSTRUCTION_CONTAINER_ITEM_H_
#define SEQUENCERGUI_MODEL_INSTRUCTION_CONTAINER_ITEM_H_

#include <mvvm/model/compound_item.h>

namespace sequencergui
{
class InstructionItem;

//! Represents Instructions.
class InstructionContainerItem : public mvvm::CompoundItem
{
public:
  static inline const std::string Type = "InstructionContainer";

  using CompoundItem::CompoundItem;
  InstructionContainerItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;

  std::vector<InstructionItem*> GetInstructions() const;

  int GetInstructionCount() const;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MODEL_INSTRUCTION_CONTAINER_ITEM_H_
