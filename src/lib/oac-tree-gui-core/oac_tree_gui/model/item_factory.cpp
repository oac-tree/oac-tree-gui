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

#include "item_factory.h"

#include <oac_tree_gui/model/sequencer_item_includes.h>

#include <mvvm/model/item_catalogue.h>

namespace oac_tree_gui
{

namespace
{

/**
 * @brief Creates catalogue where VariableItem factory functions are registered against "Type" name
 * strings known by the sequencer.
 * @return
 */
std::unique_ptr<mvvm::ItemCatalogue<VariableItem>> CreateVariableItemCatalogue()
{
  auto result = std::make_unique<mvvm::ItemCatalogue<VariableItem>>();

  result->RegisterItem<LocalVariableItem>();
  result->RegisterItem<FileVariableItem>();
  result->RegisterItem<ChannelAccessVariableItem>();
  result->RegisterItem<PvAccessClientVariableItem>();
  result->RegisterItem<PvAccessServerVariableItem>();

  return result;
}

/**
 * @brief Creates catalogue where InstructionItem factory functions are registered against "Type"
 * name strings known by the sequencer.
 *
 * These are custom classes representing domain types, which were created for convenience of GUI
 * implementation. If there is no custom class for some domain instruction type, then
 * CreateInstructionItem factory function creates UniversalInstructionItem with domain instruction
 * type.
 */
std::unique_ptr<mvvm::ItemCatalogue<InstructionItem>> CreateInstructionItemCatalogue()
{
  auto result = std::make_unique<mvvm::ItemCatalogue<InstructionItem>>();

  result->RegisterItem<IncludeItem>();
  result->RegisterItem<ParallelSequenceItem>();
  result->RegisterItem<RepeatItem>();
  result->RegisterItem<SequenceItem>();
  result->RegisterItem<WaitItem>();

  // instructions from sequencer-plugin-epics
  result->RegisterItem<ChannelAccessReadInstructionItem>();
  result->RegisterItem<ChannelAccessWriteInstructionItem>();
  result->RegisterItem<PvAccessReadInstructionItem>();
  result->RegisterItem<PvAccessWriteInstructionItem>();
  result->RegisterItem<RPCClientInstruction>();
  result->RegisterItem<LogInstructionItem>();

  return result;
}

}  // namespace

std::unique_ptr<VariableItem> CreateVariableItem(const std::string& domain_type)
{
  static const auto catalogue = CreateVariableItemCatalogue();

  // We are first trying to create item from catalogue, and only if it fails we create
  // UniversalVariableItem with domain type as a property.
  if (!catalogue->IsRegistered(domain_type))
  {
    auto result = std::make_unique<UniversalVariableItem>();
    result->SetDomainType(domain_type);
    return result;
  }

  return catalogue->Create(domain_type);
}

std::unique_ptr<InstructionItem> CreateInstructionItem(const std::string& domain_type)
{
  static const auto catalogue = CreateInstructionItemCatalogue();

  // We are first trying to create item from catalogue, and only if it fails we create
  // UniversalVariableItem with domain type as a property.
  if (!catalogue->IsRegistered(domain_type))
  {
    auto result = std::make_unique<UniversalInstructionItem>();
    result->SetDomainType(domain_type);
    return result;
  }

  return catalogue->Create(domain_type);
}

}  // namespace oac_tree_gui
