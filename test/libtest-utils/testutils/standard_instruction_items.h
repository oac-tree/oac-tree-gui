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

//! Represent Increment instruction.
class IncrementItem : public sequencergui::UniversalInstructionItem
{
public:
  static inline const std::string Type = sequencergui::domainconstants::kIncrementInstructionType;

  using UniversalInstructionItem::UniversalInstructionItem;
  IncrementItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;

  std::string GetVariableName() const;

  void SetVariableName(const std::string& value);
};

//! Represent Input instruction.
class InputItem : public sequencergui::UniversalInstructionItem
{
public:
  static inline const std::string Type = sequencergui::domainconstants::kInputInstructionType;

  using UniversalInstructionItem::UniversalInstructionItem;
  InputItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;

  std::string GetTargetVariableName() const;

  void SetTargetVariableName(const std::string& value);

  std::string GetDescription() const;

  void SetDescription(const std::string& value);
};

//! Represent Inverter instruction.
class InverterItem : public sequencergui::UniversalInstructionItem
{
public:
  static inline const std::string Type = sequencergui::domainconstants::kInverterInstructionType;

  using UniversalInstructionItem::UniversalInstructionItem;
  InverterItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;
};

//! Represent Listen instruction.
class ListenItem : public sequencergui::UniversalInstructionItem
{
public:
  static inline const std::string Type = sequencergui::domainconstants::kListenInstructionType;

  using UniversalInstructionItem::UniversalInstructionItem;
  ListenItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;

  bool IsForceSuccess() const;
  void SetForceSuccess(bool value);

  std::string GetVarNames() const;
  void SetVarNames(const std::string& value);
};

}  // namespace testutils

#endif  // LIBTEST_UTILS_TESTUTILS_EQUALITY_INSTRUCTION_ITEMS_H_
