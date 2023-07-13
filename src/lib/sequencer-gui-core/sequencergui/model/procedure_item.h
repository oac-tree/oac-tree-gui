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

#ifndef SEQUENCERGUI_MODEL_PROCEDURE_ITEM_H_
#define SEQUENCERGUI_MODEL_PROCEDURE_ITEM_H_

#include <mvvm/model/compound_item.h>

namespace sequencergui
{
class WorkspaceItem;
class InstructionContainerItem;
class ProcedurePreambleItem;

//! Represents Procedure.
class ProcedureItem : public mvvm::CompoundItem
{
public:
  static inline const std::string Type = "Procedure";

  using CompoundItem::CompoundItem;
  ProcedureItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;

  std::string GetName() const;

  void SetName(const std::string& value);

  std::string GetDescription() const;

  void SetDescription(const std::string& value);

  std::string GetFileName() const;

  void SetFileName(const std::string& value);

  InstructionContainerItem* GetInstructionContainer() const;
  WorkspaceItem* GetWorkspace() const;

  ProcedurePreambleItem* GetPreambleItem() const;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MODEL_PROCEDURE_ITEM_H_
