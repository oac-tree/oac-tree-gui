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

#ifndef SEQUENCERGUI_MODEL_INSTRUCTION_ITEM_H_
#define SEQUENCERGUI_MODEL_INSTRUCTION_ITEM_H_

#include <sequencergui/domain/sequencer_types_fwd.h>

#include <mvvm/model/compound_item.h>

namespace sequencergui
{
//! Base class for all instruction items.
class InstructionItem : public mvvm::CompoundItem
{
public:
  using CompoundItem::CompoundItem;
  explicit InstructionItem(const std::string& item_type);

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;

  virtual std::string GetDomainType() const = 0;

  //! Init given item from the domain instruction.
  void InitFromDomain(const instruction_t* instruction);

  //! Creates domain instruction corresponding to given item.
  std::unique_ptr<instruction_t> CreateDomainInstruction() const;

  std::vector<InstructionItem*> GetInstructions() const;

  bool IsRoot() const;

  void SetIsRootFlag(bool value);

  std::string GetName() const;

  void SetName(const std::string& name);

  std::string GetStatus() const;

  void SetStatus(const std::string& status);

  double GetX() const;

  void SetX(double value);

  double GetY() const;

  void SetY(double value);

protected:
  void RegisterCommonProperties();

private:
  virtual void InitFromDomainImpl(const instruction_t* instruction) = 0;
  virtual void SetupDomainImpl(instruction_t* instruction) const = 0;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MODEL_INSTRUCTION_ITEM_H_
