/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_MODEL_PROCEDUREITEM_H
#define SEQUENCERGUI_MODEL_PROCEDUREITEM_H

#include "mvvm/model/compounditem.h"

namespace sequencergui
{
class WorkspaceItem;
class InstructionContainerItem;

//! Represents Procedure.
class ProcedureItem : public mvvm::CompoundItem
{
public:
  static inline const std::string Type = "Procedure";
  ProcedureItem();

  std::string GetStatus() const;

  void SetStatus(const std::string& status);

  InstructionContainerItem* GetInstructionContainer() const;
  WorkspaceItem* GetWorkspace() const;
};
}  // namespace sequencergui

#endif  // SEQUENCERGUI_MODEL_PROCEDUREITEMS_H
