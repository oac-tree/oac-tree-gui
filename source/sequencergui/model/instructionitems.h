/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_MODEL_INSTRUCTIONITEMS_H
#define SEQUENCERGUI_MODEL_INSTRUCTIONITEMS_H

//! @file instructionitems.h
//! Collection of classes to represent Sequencer instructions.

#include "sequencergui/model/sequencer_types.h"

#include "mvvm/model/compounditem.h"

namespace sequi
{
//! Base class for all instruction items.
class InstructionItem : public ModelView::CompoundItem
{
public:
  InstructionItem(const std::string& item_type);

  //! Init given item from the domain instruction.
  void InitFromDomain(const instruction_t* instruction);

  //! Creates domain instruction corresponding to given item.
  virtual std::unique_ptr<instruction_t> CreateDomainInstruction() const;

  std::vector<InstructionItem*> GetInstructions() const;

  bool IsRoot() const;

  void SetIsRootFlag(bool value);

  void SetStatus(const std::string& status);

  std::string GetStatus() const;

  double GetX() const;

  void SetX(double value);

  double GetY() const;

  void SetY(double value);

  SessionItem* GetStatusItem() const;

  SessionItem* GetNameItem() const;

private:
  virtual void InitFromDomainImpl(const instruction_t* instruction) = 0;
};

//! Represent Condition instruction.
class ConditionItem : public InstructionItem
{
public:
  static inline const std::string Type = "Condition";
  ConditionItem();

  std::unique_ptr<instruction_t> CreateDomainInstruction() const override;

  std::string GetVariableName() const;

  void SetVariableName(const std::string &value);

private:
  void InitFromDomainImpl(const instruction_t* instruction) override;
};

//! Represent Copy instruction.
class CopyItem : public InstructionItem
{
public:
  static inline const std::string Type = "Copy";
  CopyItem();

  std::unique_ptr<instruction_t> CreateDomainInstruction() const override;

  std::string GetInput() const;

  void SetInput(const std::string& value);

  std::string GetOutput() const;

  void SetOutput(const std::string& value);

private:
  void InitFromDomainImpl(const instruction_t* instruction) override;
};

//! Represent Fallback instruction.
class FallbackItem : public InstructionItem
{
public:
  static inline const std::string Type = "Fallback";
  FallbackItem();

  std::unique_ptr<instruction_t> CreateDomainInstruction() const override;

private:
  virtual void InitFromDomainImpl(const instruction_t* instruction) override;
};

//! Represent IncludeItem instruction.
class IncludeItem : public InstructionItem
{
public:
  static inline const std::string Type = "Include";
  IncludeItem();

  std::unique_ptr<instruction_t> CreateDomainInstruction() const override;

  std::string GetFileName() const;

  void SetFileName(const std::string& value);

  std::string GetPath() const;

  void SetPath(const std::string& value);

private:
  void InitFromDomainImpl(const instruction_t* instruction);
};

//! Represent Input instruction.
class InputItem : public InstructionItem
{
public:
  static inline const std::string Type = "Input";
  InputItem();

  std::unique_ptr<instruction_t> CreateDomainInstruction() const override;

  std::string GetTargetVariableName() const;

  void SetTargetVariableName(const std::string& value);

  std::string GetDescription() const;

  void SetDescription(const std::string& value);

private:
  void InitFromDomainImpl(const instruction_t* instruction) override;
};

//! Represent Inverter instruction.
class InverterItem : public InstructionItem
{
public:
  static inline const std::string Type = "Inverter";
  InverterItem();

  std::unique_ptr<instruction_t> CreateDomainInstruction() const override;

private:
  void InitFromDomainImpl(const instruction_t* instruction) override;
};

//! Represent Output instruction.
class OutputItem : public InstructionItem
{
public:
  static inline const std::string Type = "Output";
  OutputItem();

  std::unique_ptr<instruction_t> CreateDomainInstruction() const override;

  std::string GetSourceVariableName() const;

  void SetSourceVariableName(const std::string& value);

  std::string GetDescription() const;

  void SetDescription(const std::string& value);

private:
  void InitFromDomainImpl(const instruction_t* instruction) override;
};

//! Represent ParallelSequence instruction.
class ParallelSequenceItem : public InstructionItem
{
public:
  static inline const std::string Type = "ParallelSequence";
  ParallelSequenceItem();

  std::unique_ptr<instruction_t> CreateDomainInstruction() const override;

  int GetSuccessThreshold() const;

  void SetSuccessThreshold(int value);

  int GetFailureThreshold() const;

  void SetFailureThreshold(int value);

private:
  void InitFromDomainImpl(const instruction_t* instruction) override;
};

//! Represent Repeat instruction.
class RepeatItem : public InstructionItem
{
public:
  static inline const std::string Type = "Repeat";
  RepeatItem();

  std::unique_ptr<instruction_t> CreateDomainInstruction() const override;

  int GetRepeatCount() const;

  void SetRepeatCount(int value);

private:
  void InitFromDomainImpl(const instruction_t* instruction) override;
};

//! Represent Sequencer instruction.
class SequenceItem : public InstructionItem
{
public:
  static inline const std::string Type = "Sequence";
  SequenceItem();

  std::unique_ptr<instruction_t> CreateDomainInstruction() const override;

private:
  void InitFromDomainImpl(const instruction_t* instruction) override;
};

//! Represent Wait instruction.
class WaitItem : public InstructionItem
{
public:
  static inline const std::string Type = "Wait";
  WaitItem();

  std::unique_ptr<instruction_t> CreateDomainInstruction() const override;

  void SetTimeout(double value);

  double GetTimeout() const;

private:
  void InitFromDomainImpl(const instruction_t* instruction) override;
};

}  // namespace sequi

#endif  // SEQUENCERGUI_MODEL_INSTRUCTIONITEMS_H
