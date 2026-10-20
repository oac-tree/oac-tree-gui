/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "register_items.h"

#include <oac_tree_gui/model/sequencer_item_includes.h>

#include <mvvm/model/item_factory.h>

namespace oac_tree_gui
{

void RegisterSessionItems()
{
  // instructions
  (void)mvvm::RegisterGlobalItem<oac_tree_gui::InstructionItem>();

  // variables
  (void)mvvm::RegisterGlobalItem<oac_tree_gui::FileVariableItem>();
  (void)mvvm::RegisterGlobalItem<oac_tree_gui::LocalVariableItem>();
  (void)mvvm::RegisterGlobalItem<oac_tree_gui::UniversalVariableItem>();
  (void)mvvm::RegisterGlobalItem<oac_tree_gui::VariableInfoItem>();

  // variables sequencer-plugin-epics
  (void)mvvm::RegisterGlobalItem<oac_tree_gui::ChannelAccessVariableItem>();
  (void)mvvm::RegisterGlobalItem<oac_tree_gui::PvAccessClientVariableItem>();
  (void)mvvm::RegisterGlobalItem<oac_tree_gui::PvAccessServerVariableItem>();

  // other items
  (void)mvvm::RegisterGlobalItem<oac_tree_gui::AttributeItem>();
  (void)mvvm::RegisterGlobalItem<oac_tree_gui::InstructionContainerItem>();
  (void)mvvm::RegisterGlobalItem<oac_tree_gui::ProcedureItem>();
  (void)mvvm::RegisterGlobalItem<oac_tree_gui::WorkspaceItem>();
  (void)mvvm::RegisterGlobalItem<oac_tree_gui::ProcedurePreambleItem>();
  (void)mvvm::RegisterGlobalItem<oac_tree_gui::TypeRegistrationItem>();

  (void)mvvm::RegisterGlobalItem<oac_tree_gui::LocalJobItem>();
  (void)mvvm::RegisterGlobalItem<oac_tree_gui::ImportedJobItem>();
  (void)mvvm::RegisterGlobalItem<oac_tree_gui::RemoteJobItem>();
  (void)mvvm::RegisterGlobalItem<oac_tree_gui::FileBasedJobItem>();

  (void)mvvm::RegisterGlobalItem<oac_tree_gui::PluginSettingsItem>();
  (void)mvvm::RegisterGlobalItem<oac_tree_gui::TextEditItem>();

  (void)mvvm::RegisterGlobalItem<oac_tree_gui::EPICSServerInfoItem>();
  (void)mvvm::RegisterGlobalItem<oac_tree_gui::WebSocketsServerInfoItem>();
  (void)mvvm::RegisterGlobalItem<oac_tree_gui::ServerGroupItem>();
}

}  // namespace oac_tree_gui
