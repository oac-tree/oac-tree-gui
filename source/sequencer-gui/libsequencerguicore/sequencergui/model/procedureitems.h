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

#ifndef SEQUENCERGUI_MODEL_PROCEDUREITEMS_H
#define SEQUENCERGUI_MODEL_PROCEDUREITEMS_H

//! @file procedureitems.h
//! Collection of classes defining main procedure related UI items.

#include "sequencergui/model/instructionitem.h"
#include "sequencergui/model/standardinstructionitems.h"

#include "mvvm/model/compounditem.h"

#include <string>

namespace sequencergui
{
class VariableItem;

//! Represents Instructions.
class InstructionContainerItem : public mvvm::CompoundItem
{
public:
  static inline const std::string Type = "InstructionContainer";
  InstructionContainerItem();

  std::vector<InstructionItem*> GetInstructions() const;
};

//! Represents a Workspace.

class WorkspaceItem : public mvvm::CompoundItem
{
public:
  static inline const std::string Type = "Workspace";
  WorkspaceItem();

  std::vector<VariableItem*> GetVariables() const;
};

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

//! Represents job.
class JobItem : public mvvm::CompoundItem
{
public:
  static inline const std::string Type = "Job";
  JobItem();

  std::string GetStatus() const;

  void SetStatus(const std::string& status);
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MODEL_PROCEDUREITEMS_H
