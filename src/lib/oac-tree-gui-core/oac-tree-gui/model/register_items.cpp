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

#include "register_items.h"

#include <mvvm/model/item_catalogue.h>
#include <mvvm/model/item_factory.h>

#include <oac-tree-gui/model/sequencer_item_includes.h>

namespace sequencergui
{

void RegisterSequencerItems()
{
  // instructions
  (void)mvvm::RegisterGlobalItem<sequencergui::IncludeItem>();
  (void)mvvm::RegisterGlobalItem<sequencergui::ParallelSequenceItem>();
  (void)mvvm::RegisterGlobalItem<sequencergui::RepeatItem>();
  (void)mvvm::RegisterGlobalItem<sequencergui::SequenceItem>();
  (void)mvvm::RegisterGlobalItem<sequencergui::WaitItem>();
  (void)mvvm::RegisterGlobalItem<sequencergui::UniversalInstructionItem>();

  // sequencer-plugin-epics instructions
  (void)mvvm::RegisterGlobalItem<sequencergui::ChannelAccessReadInstructionItem>();
  (void)mvvm::RegisterGlobalItem<sequencergui::ChannelAccessWriteInstructionItem>();
  (void)mvvm::RegisterGlobalItem<sequencergui::PvAccessReadInstructionItem>();
  (void)mvvm::RegisterGlobalItem<sequencergui::PvAccessWriteInstructionItem>();
  (void)mvvm::RegisterGlobalItem<sequencergui::RPCClientInstruction>();
  (void)mvvm::RegisterGlobalItem<sequencergui::SystemCallInstructionItem>();
  (void)mvvm::RegisterGlobalItem<sequencergui::LogInstructionItem>();

  // variables
  (void)mvvm::RegisterGlobalItem<sequencergui::FileVariableItem>();
  (void)mvvm::RegisterGlobalItem<sequencergui::LocalVariableItem>();
  (void)mvvm::RegisterGlobalItem<sequencergui::UniversalVariableItem>();

  // variables sequencer-plugin-epics
  (void)mvvm::RegisterGlobalItem<sequencergui::ChannelAccessVariableItem>();
  (void)mvvm::RegisterGlobalItem<sequencergui::PvAccessClientVariableItem>();
  (void)mvvm::RegisterGlobalItem<sequencergui::PvAccessServerVariableItem>();

  // other items
  (void)mvvm::RegisterGlobalItem<sequencergui::InstructionContainerItem>();
  (void)mvvm::RegisterGlobalItem<sequencergui::ProcedureItem>();
  (void)mvvm::RegisterGlobalItem<sequencergui::WorkspaceItem>();
  (void)mvvm::RegisterGlobalItem<sequencergui::ProcedurePreambleItem>();
  (void)mvvm::RegisterGlobalItem<sequencergui::TypeRegistrationItem>();
  (void)mvvm::RegisterGlobalItem<sequencergui::ProcedurePreambleItem>();

  (void)mvvm::RegisterGlobalItem<sequencergui::LocalJobItem>();
  (void)mvvm::RegisterGlobalItem<sequencergui::ImportedJobItem>();
  (void)mvvm::RegisterGlobalItem<sequencergui::RemoteJobItem>();
}

}  // namespace sequencergui
