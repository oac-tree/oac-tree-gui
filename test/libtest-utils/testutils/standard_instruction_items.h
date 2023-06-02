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

#ifndef LIBTEST_UTILS_TESTUTILS_EQUALITY_INSTRUCTION_ITEMS_H_
#define LIBTEST_UTILS_TESTUTILS_EQUALITY_INSTRUCTION_ITEMS_H_

//! Collection of classes to represent Sequencer instructions.

#include <sequencergui/domain/domain_constants.h>
#include <sequencergui/model/universal_instruction_item.h>

namespace testutils
{

//! Represent Choice instruction.
class ChoiceItem : public sequencergui::UniversalInstructionItem
{
public:
  static inline const std::string Type = sequencergui::domainconstants::kChoiceInstructionType;

  using UniversalInstructionItem::UniversalInstructionItem;
  ChoiceItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;

  std::string GetVariableName() const;

  void SetVariableName(const std::string& value);
};

}  // namespace testutils

#endif  // LIBTEST_UTILS_TESTUTILS_EQUALITY_INSTRUCTION_ITEMS_H_
