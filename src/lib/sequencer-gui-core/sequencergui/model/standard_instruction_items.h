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

#ifndef SEQUENCERGUI_MODEL_STANDARD_INSTRUCTION_ITEMS_H_
#define SEQUENCERGUI_MODEL_STANDARD_INSTRUCTION_ITEMS_H_

//! Collection of classes to represent Sequencer instructions.

#include <sequencergui/domain/domain_constants.h>
#include <sequencergui/model/universal_instruction_item.h>

namespace sequencergui
{

//! Represent IncludeItem instruction.
class IncludeItem : public UniversalInstructionItem
{
public:
  static inline const std::string Type = sequencergui::domainconstants::kIncludeInstructionType;

  using UniversalInstructionItem::UniversalInstructionItem;
  IncludeItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;

  std::string GetFileName() const;

  void SetFileName(const std::string& value);

  std::string GetPath() const;

  void SetPath(const std::string& value);
};

//! Represent Output instruction.
class OutputItem : public UniversalInstructionItem
{
public:
  static inline const std::string Type = sequencergui::domainconstants::kOutputInstructionType;

  using UniversalInstructionItem::UniversalInstructionItem;
  OutputItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;

  std::string GetSourceVariableName() const;

  void SetSourceVariableName(const std::string& value);

  std::string GetDescription() const;

  void SetDescription(const std::string& value);
};

//! Represent ParallelSequence instruction.
class ParallelSequenceItem : public UniversalInstructionItem
{
public:
  static inline const std::string Type = sequencergui::domainconstants::kParallelInstructionType;

  using UniversalInstructionItem::UniversalInstructionItem;
  ParallelSequenceItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;

  int GetSuccessThreshold() const;

  void SetSuccessThreshold(int value);

  int GetFailureThreshold() const;

  void SetFailureThreshold(int value);
};

//! Represent Repeat instruction.
class RepeatItem : public UniversalInstructionItem
{
public:
  static inline const std::string Type = sequencergui::domainconstants::kRepeatInstructionType;

  using UniversalInstructionItem::UniversalInstructionItem;
  RepeatItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;

  int GetRepeatCount() const;

  void SetRepeatCount(int value);
};

//! Represent Sequencer instruction.
class SequenceItem : public UniversalInstructionItem
{
public:
  static inline const std::string Type = sequencergui::domainconstants::kSequenceInstructionType;

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;

  using UniversalInstructionItem::UniversalInstructionItem;
  SequenceItem();
};

//! Represent UserChoice instruction.
class UserChoiceItem : public UniversalInstructionItem
{
public:
  static inline const std::string Type = sequencergui::domainconstants::kUserChoiceInstructionType;

  using UniversalInstructionItem::UniversalInstructionItem;
  UserChoiceItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;

  std::string GetDescription() const;

  void SetDescription(const std::string& value);
};

//! Represent ResetVariable instruction.
class VariableResetItem : public UniversalInstructionItem
{
public:
  static inline const std::string Type =
      sequencergui::domainconstants::kVariableResetInstructionType;

  using UniversalInstructionItem::UniversalInstructionItem;
  VariableResetItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;

  std::string GetVariableName() const;

  void SetVariableName(const std::string& value);
};

//! Represent Wait instruction.
class WaitItem : public UniversalInstructionItem
{
public:
  static inline const std::string Type = sequencergui::domainconstants::kWaitInstructionType;

  using UniversalInstructionItem::UniversalInstructionItem;
  WaitItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;

  void SetTimeout(double value);

  double GetTimeout() const;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MODEL_STANDARD_INSTRUCTION_ITEMS_H_
