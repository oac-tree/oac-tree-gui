/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "procedure_item_transform_helper.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/instruction_item.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/procedure_preamble_items.h>
#include <sequencergui/transform/instruction_item_transform_helper.h>
#include <sequencergui/transform/variable_item_transform_helper.h>

#include <sup/dto/anytype_registry.h>
#include <sup/sequencer/instruction.h>
#include <sup/sequencer/procedure.h>
#include <sup/sequencer/procedure_preamble.h>

namespace sequencergui
{

namespace
{

/**
 * @brief Creates Registry containing type defined in a preamble.
 *
 * We can't use registry on board of procedure itself, because it hasn't been setup  yet.
 */
std::unique_ptr<sup::dto::AnyTypeRegistry> CreateRegistry(const procedure_t& procedure)
{
  auto result = std::make_unique<sup::dto::AnyTypeRegistry>();

  for (const auto& type_registration : procedure.GetPreamble().GetTypeRegistrations())
  {
    auto anytype = ParseTypeRegistrationInfo(type_registration, procedure.GetFilename(), *result);
    result->RegisterType(anytype);
  }

  return result;
}

/**
 * @brief Populates empty instruction container with instructions from the domain procedure.
 */
void PopulateInstructionContainerItem(const procedure_t& procedure,
                                      InstructionContainerItem& container)
{
  for (auto instruction : procedure.GetTopInstructions())
  {
    auto instruction_tree = CreateInstructionItemTree(*instruction);
    container.InsertItem(std::move(instruction_tree.root), mvvm::TagIndex::Append());
  }
}

/**
 * @brief Populates empty procedure item.
 */
void PopulateProcedureItem(const procedure_t& procedure, ProcedureItem& procedure_item)
{
  auto instruction_container = procedure_item.GetInstructionContainer();
  PopulateInstructionContainerItem(procedure, *instruction_container);

  PopulateProcedurePreambleItem(procedure.GetPreamble(), *procedure_item.GetPreambleItem());

  auto registry = CreateRegistry(procedure);

  PopulateWorkspaceItem(procedure.GetWorkspace(), registry.get(), procedure_item.GetWorkspace());
}

}  // namespace

void PopulateProcedurePreambleItem(const preamble_t& preamble, ProcedurePreambleItem& item)
{
  if (!item.GetPluginPaths().empty() || !item.GetTypeRegistrations().empty())
  {
    throw LogicErrorException("ProcedurePreambleItem must be empty");
  }

  for (const auto& str : preamble.GetPluginPaths())
  {
    item.AddPluginPath(str);
  }

  for (const auto& info : preamble.GetTypeRegistrations())
  {
    item.AddTypeRegistration(static_cast<int>(info.GetRegistrationMode()), info.GetString());
  }
}

std::unique_ptr<ProcedureItem> CreateProcedureItem(const procedure_t& procedure)
{
  auto result = std::make_unique<ProcedureItem>();
  PopulateProcedureItem(procedure, *result);
  return result;
}

}  // namespace sequencergui
