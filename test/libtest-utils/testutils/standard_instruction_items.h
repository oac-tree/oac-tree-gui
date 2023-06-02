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

//! Represent Condition instruction.
class ConditionItem : public sequencergui::UniversalInstructionItem
{
public:
  static inline const std::string Type = sequencergui::domainconstants::kConditionInstructionType;

  using UniversalInstructionItem::UniversalInstructionItem;
  ConditionItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;

  std::string GetVariableName() const;

  void SetVariableName(const std::string& value);
};

//! Represent Copy instruction.
class CopyItem : public sequencergui::UniversalInstructionItem
{
public:
  static inline const std::string Type = sequencergui::domainconstants::kCopyInstructionType;

  using UniversalInstructionItem::UniversalInstructionItem;
  CopyItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;

  std::string GetDomainType() const override;

  std::string GetInput() const;

  void SetInput(const std::string& value);

  std::string GetOutput() const;

  void SetOutput(const std::string& value);
};

//! Represent Decrement instruction.
class DecrementItem : public sequencergui::UniversalInstructionItem
{
public:
  static inline const std::string Type = sequencergui::domainconstants::kDecrementInstructionType;

  using UniversalInstructionItem::UniversalInstructionItem;
  DecrementItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;

  std::string GetDomainType() const override;

  std::string GetVariableName() const;

  void SetVariableName(const std::string& value);
};

//! Represent Fallback instruction.
class FallbackItem : public sequencergui::UniversalInstructionItem
{
public:
  static inline const std::string Type = sequencergui::domainconstants::kFallbackInstructionType;

  using UniversalInstructionItem::UniversalInstructionItem;
  FallbackItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;
};

//! Represent ForceSuccess instruction.
class ForceSuccessItem : public sequencergui::UniversalInstructionItem
{
public:
  static inline const std::string Type =
      sequencergui::domainconstants::kForceSuccessInstructionType;

  using UniversalInstructionItem::UniversalInstructionItem;
  ForceSuccessItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;
};

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
