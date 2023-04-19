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

#include <sequencergui/model/instruction_item.h>

namespace sequencergui
{

//! Represent Choice instruction.
class ChoiceItem : public InstructionItem
{
public:
  static inline const std::string Type = "Choice";

  using InstructionItem::InstructionItem;
  ChoiceItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;

  std::string GetDomainType() const override;

  std::string GetVariableName() const;

  void SetVariableName(const std::string& value);

private:
  void InitFromDomainImpl(const instruction_t* instruction) override;
  void SetupDomainImpl(instruction_t* instruction) const override;
};

//! Represent Condition instruction.
class ConditionItem : public InstructionItem
{
public:
  static inline const std::string Type = "Condition";

  using InstructionItem::InstructionItem;
  ConditionItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;

  std::string GetDomainType() const override;

  std::string GetVariableName() const;

  void SetVariableName(const std::string& value);

private:
  void InitFromDomainImpl(const instruction_t* instruction) override;
  void SetupDomainImpl(instruction_t* instruction) const override;
};

//! Represent Copy instruction.
class CopyItem : public InstructionItem
{
public:
  static inline const std::string Type = "Copy";

  using InstructionItem::InstructionItem;
  CopyItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;

  std::string GetDomainType() const override;

  std::string GetInput() const;

  void SetInput(const std::string& value);

  std::string GetOutput() const;

  void SetOutput(const std::string& value);

private:
  void InitFromDomainImpl(const instruction_t* instruction) override;
  void SetupDomainImpl(instruction_t* instruction) const override;
};

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

//! Represent Fallback instruction.
class FallbackItem : public InstructionItem
{
public:
  static inline const std::string Type = "Fallback";

  using InstructionItem::InstructionItem;
  FallbackItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;

  std::string GetDomainType() const override;

private:
  void InitFromDomainImpl(const instruction_t* instruction) override;
  void SetupDomainImpl(instruction_t* instruction) const override;
};

//! Represent ForceSuccess instruction.
class ForceSuccessItem : public InstructionItem
{
public:
  static inline const std::string Type = "ForceSuccess";

  using InstructionItem::InstructionItem;
  ForceSuccessItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;

  std::string GetDomainType() const override;

private:
  void InitFromDomainImpl(const instruction_t* instruction) override;
  void SetupDomainImpl(instruction_t* instruction) const override;
};

//! Represent IncludeItem instruction.
class IncludeItem : public InstructionItem
{
public:
  static inline const std::string Type = "Include";

  using InstructionItem::InstructionItem;
  IncludeItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;

  std::string GetDomainType() const override;

  std::string GetFileName() const;

  void SetFileName(const std::string& value);

  std::string GetPath() const;

  void SetPath(const std::string& value);

private:
  void InitFromDomainImpl(const instruction_t* instruction) override;
  void SetupDomainImpl(instruction_t* instruction) const override;
};

//! Represent Input instruction.
class InputItem : public InstructionItem
{
public:
  static inline const std::string Type = "Input";

  using InstructionItem::InstructionItem;
  InputItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;

  std::string GetDomainType() const override;

  std::string GetTargetVariableName() const;

  void SetTargetVariableName(const std::string& value);

  std::string GetDescription() const;

  void SetDescription(const std::string& value);

private:
  void InitFromDomainImpl(const instruction_t* instruction) override;
  void SetupDomainImpl(instruction_t* instruction) const override;
};

//! Represent Inverter instruction.
class InverterItem : public InstructionItem
{
public:
  static inline const std::string Type = "Inverter";

  using InstructionItem::InstructionItem;
  InverterItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;

  std::string GetDomainType() const override;

private:
  void InitFromDomainImpl(const instruction_t* instruction) override;
  void SetupDomainImpl(instruction_t* instruction) const override;
};

//! Represent Listen instruction.
class ListenItem : public InstructionItem
{
public:
  static inline const std::string Type = "Listen";

  using InstructionItem::InstructionItem;
  ListenItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;

  std::string GetDomainType() const override;

  bool IsForceSuccess() const;
  void SetForceSuccess(bool value);

  std::string GetVarNames() const;
  void SetVarNames(const std::string& value);

private:
  void InitFromDomainImpl(const instruction_t* instruction) override;
  void SetupDomainImpl(instruction_t* instruction) const override;
};

//! Represent Message instruction.
class MessageItem : public InstructionItem
{
public:
  static inline const std::string Type = "Message";

  using InstructionItem::InstructionItem;
  MessageItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;

  std::string GetDomainType() const override;

  std::string GetText() const;

  void SetText(const std::string& value);

private:
  void InitFromDomainImpl(const instruction_t* instruction) override;
  void SetupDomainImpl(instruction_t* instruction) const override;
};

//! Represent Output instruction.
class OutputItem : public InstructionItem
{
public:
  static inline const std::string Type = "Output";

  using InstructionItem::InstructionItem;
  OutputItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;

  std::string GetDomainType() const override;

  std::string GetSourceVariableName() const;

  void SetSourceVariableName(const std::string& value);

  std::string GetDescription() const;

  void SetDescription(const std::string& value);

private:
  void InitFromDomainImpl(const instruction_t* instruction) override;
  void SetupDomainImpl(instruction_t* instruction) const override;
};

//! Represent ParallelSequence instruction.
class ParallelSequenceItem : public InstructionItem
{
public:
  static inline const std::string Type = "ParallelSequence";

  using InstructionItem::InstructionItem;
  ParallelSequenceItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;

  std::string GetDomainType() const override;

  int GetSuccessThreshold() const;

  void SetSuccessThreshold(int value);

  int GetFailureThreshold() const;

  void SetFailureThreshold(int value);

private:
  void InitFromDomainImpl(const instruction_t* instruction) override;
  void SetupDomainImpl(instruction_t* instruction) const override;
};

//! Represent Repeat instruction.
class RepeatItem : public InstructionItem
{
public:
  static inline const std::string Type = "Repeat";

  using InstructionItem::InstructionItem;
  RepeatItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;

  std::string GetDomainType() const override;

  int GetRepeatCount() const;

  void SetRepeatCount(int value);

private:
  void InitFromDomainImpl(const instruction_t* instruction) override;
  void SetupDomainImpl(instruction_t* instruction) const override;
};

//! Represent Sequencer instruction.
class SequenceItem : public InstructionItem
{
public:
  static inline const std::string Type = "Sequence";

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;

  using InstructionItem::InstructionItem;
  SequenceItem();

  std::string GetDomainType() const override;

private:
  void InitFromDomainImpl(const instruction_t* instruction) override;
  void SetupDomainImpl(instruction_t* instruction) const override;
};

//! Represent UserChoice instruction.
class UserChoiceItem : public InstructionItem
{
public:
  static inline const std::string Type = "UserChoice";

  using InstructionItem::InstructionItem;
  UserChoiceItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;

  std::string GetDomainType() const override;

  std::string GetDescription() const;

  void SetDescription(const std::string& value);

private:
  void InitFromDomainImpl(const instruction_t* instruction) override;
  void SetupDomainImpl(instruction_t* instruction) const override;
};

//! Represent ResetVariable instruction.
class VariableResetItem : public InstructionItem
{
public:
  static inline const std::string Type = "ResetVariable";

  using InstructionItem::InstructionItem;
  VariableResetItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;

  std::string GetDomainType() const override;

  std::string GetVariableName() const;

  void SetVariableName(const std::string& value);

private:
  void InitFromDomainImpl(const instruction_t* instruction) override;
  void SetupDomainImpl(instruction_t* instruction) const override;
};

//! Represent Wait instruction.
class WaitItem : public InstructionItem
{
public:
  static inline const std::string Type = "Wait";

  using InstructionItem::InstructionItem;
  WaitItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;

  std::string GetDomainType() const override;

  void SetTimeout(double value);

  double GetTimeout() const;

private:
  void InitFromDomainImpl(const instruction_t* instruction) override;
  void SetupDomainImpl(instruction_t* instruction) const override;
};

//! Represents unknown instruction.
class UnknownInstructionItem : public InstructionItem
{
public:
  static inline const std::string Type = "UnknownInstruction";

  using InstructionItem::InstructionItem;
  UnknownInstructionItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;

  std::string GetDomainType() const override;

private:
  void InitFromDomainImpl(const instruction_t* instruction) override;
  void SetupDomainImpl(instruction_t* instruction) const override;

  std::string m_domain_name;
  std::vector<std::string> m_domain_attributes;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MODEL_STANDARD_INSTRUCTION_ITEMS_H_
