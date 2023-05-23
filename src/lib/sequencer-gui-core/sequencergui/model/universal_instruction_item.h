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

#ifndef SEQUENCERGUI_MODEL_UNIVERSAL_INSTRUCTION_ITEM_H_
#define SEQUENCERGUI_MODEL_UNIVERSAL_INSTRUCTION_ITEM_H_

//! Collection of classes to represent Sequencer instructions.

#include <sequencergui/model/instruction_item.h>

namespace sequencergui
{

/**
 * @brief The UniversalInstructionItem class represents domain Instruction.
 */

class UniversalInstructionItem : public InstructionItem
{
public:
  static inline const std::string Type = "UniversalInstruction";

  using InstructionItem::InstructionItem;
  UniversalInstructionItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;

  std::string GetDomainType() const override;

private:
  void InitFromDomainImpl(const instruction_t* instruction) override;
  void SetupDomainImpl(instruction_t* instruction) const override;

  std::string m_domain_type;  //!< Domain typename.
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MODEL_UNIVERSAL_INSTRUCTION_ITEM_H_
