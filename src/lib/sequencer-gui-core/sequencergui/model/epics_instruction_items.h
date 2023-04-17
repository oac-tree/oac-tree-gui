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

#ifndef SEQUENCERGUI_MODEL_EPICS_INSTRUCTION_ITEMS_H_
#define SEQUENCERGUI_MODEL_EPICS_INSTRUCTION_ITEMS_H_

//! Collection of classes to represent Sequencer instructions from sup-sequencer-plugin-epics.

#include <sequencergui/model/instruction_item.h>

namespace sequencergui
{

//! Represents ChannelAccessReadInstruction.
class ChannelAccessReadInstructionItem : public InstructionItem
{
public:
  static inline const std::string Type = "ChannelAccessRead";

  using InstructionItem::InstructionItem;
  ChannelAccessReadInstructionItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;

  std::string GetDomainType() const override;

private:
  void InitFromDomainImpl(const instruction_t* instruction) override;
  void SetupDomainImpl(instruction_t* instruction) const override;
};

//! Represents ChannelAccessWriteInstruction.
class ChannelAccessWriteInstructionItem : public InstructionItem
{
public:
  static inline const std::string Type = "ChannelAccessWrite";

  using InstructionItem::InstructionItem;
  ChannelAccessWriteInstructionItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;

  std::string GetDomainType() const override;

private:
  void InitFromDomainImpl(const instruction_t* instruction) override;
  void SetupDomainImpl(instruction_t* instruction) const override;
};

//! Represents PVAccessReadInstruction.
class PVAccessReadInstructionItem : public InstructionItem
{
public:
  static inline const std::string Type = "ChannelAccessRead";

  using InstructionItem::InstructionItem;
  PVAccessReadInstructionItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;

  std::string GetDomainType() const override;

private:
  void InitFromDomainImpl(const instruction_t* instruction) override;
  void SetupDomainImpl(instruction_t* instruction) const override;
};

//! Represents PVAccessWriteInstruction.
class PVAccessWriteInstructionItem : public InstructionItem
{
public:
  static inline const std::string Type = "ChannelAccessRead";

  using InstructionItem::InstructionItem;
  PVAccessWriteInstructionItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;

  std::string GetDomainType() const override;

private:
  void InitFromDomainImpl(const instruction_t* instruction) override;
  void SetupDomainImpl(instruction_t* instruction) const override;
};

//! Represents RPCClientInstruction.
class RPCClientInstruction : public InstructionItem
{
public:
  static inline const std::string Type = "RPCClient";

  using InstructionItem::InstructionItem;
  RPCClientInstruction();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;

  std::string GetDomainType() const override;

private:
  void InitFromDomainImpl(const instruction_t* instruction) override;
  void SetupDomainImpl(instruction_t* instruction) const override;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MODEL_EPICS_INSTRUCTION_ITEMS_H_
