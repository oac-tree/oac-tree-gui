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

#ifndef SEQUENCERGUI_MODEL_INSTRUCTIONITEM_H
#define SEQUENCERGUI_MODEL_INSTRUCTIONITEM_H

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
  std::unique_ptr<instruction_t> CreateDomainInstruction() const;

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

  virtual std::string GetDomainType() const = 0;

private:
  virtual void InitFromDomainImpl(const instruction_t* instruction) = 0;
  virtual void SetupDomainImpl(instruction_t* instruction) const = 0;
};

}  // namespace sequi

#endif  // SEQUENCERGUI_MODEL_INSTRUCTIONITEM_H
