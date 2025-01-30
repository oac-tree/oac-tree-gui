/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef OAC_TREE_GUI_MODEL_INSTRUCTION_ITEM_H_
#define OAC_TREE_GUI_MODEL_INSTRUCTION_ITEM_H_

#include <oac_tree_gui/domain/sequencer_types_fwd.h>

#include <mvvm/model/compound_item.h>

namespace oac_tree_gui
{
//! Base class for all instruction items.
class InstructionItem : public mvvm::CompoundItem
{
public:
  explicit InstructionItem(const std::string& item_type);

  std::unique_ptr<SessionItem> Clone() const override;

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

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_MODEL_INSTRUCTION_ITEM_H_
