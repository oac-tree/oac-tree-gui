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

#ifndef SEQUENCERGUI_MODEL_EQUALITY_INSTRUCTION_ITEMS_H_
#define SEQUENCERGUI_MODEL_EQUALITY_INSTRUCTION_ITEMS_H_

//! Collection of classes to represent Sequencer instructions.

#include <sequencergui/model/instruction_item.h>

namespace sequencergui
{

//! Represent Equals instruction.
class EqualsItem : public InstructionItem
{
public:
  static inline const std::string Type = "Equals";

  using InstructionItem::InstructionItem;
  EqualsItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;

  std::string GetDomainType() const override;

  std::string GetLeftHandSide() const;
  void SetLeftHandSide(const std::string& value);

  std::string GetRightHandSide() const;
  void SetRightHandSide(const std::string& value);

private:
  void InitFromDomainImpl(const instruction_t* instruction) override;
  void SetupDomainImpl(instruction_t* instruction) const override;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MODEL_EQUALITY_INSTRUCTION_ITEMS_H_
