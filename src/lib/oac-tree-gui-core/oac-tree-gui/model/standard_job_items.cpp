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

#include "standard_job_items.h"

#include <oac-tree-gui/core/exceptions.h>
#include <oac-tree-gui/model/procedure_item.h>

#include <mvvm/model/item_utils.h>

namespace oac_tree_gui
{

namespace
{
const std::string kServerName = "kServerName";
const std::string kJobIndex = "kJobIndex";
}  // namespace

// ------------------------------------------------------------------------------------------------
// LocalJobItem
// ------------------------------------------------------------------------------------------------

LocalJobItem::LocalJobItem() : JobItem(GetStaticType()) {}

std::string LocalJobItem::GetStaticType()
{
  return "LocalJob";
}

std::unique_ptr<mvvm::SessionItem> LocalJobItem::Clone() const
{
  return std::make_unique<LocalJobItem>(*this);
}

// ------------------------------------------------------------------------------------------------
// ImportedJobItem
// ------------------------------------------------------------------------------------------------

ImportedJobItem::ImportedJobItem() : JobItem(GetStaticType())
{
  RegisterTag(mvvm::TagInfo(kImportedProcedure, 0, 1, {ProcedureItem::GetStaticType()}));
}

std::string ImportedJobItem::GetStaticType()
{
  return "ImportedJob";
}

std::unique_ptr<mvvm::SessionItem> ImportedJobItem::Clone() const
{
  return std::make_unique<ImportedJobItem>(*this);
}

// ------------------------------------------------------------------------------------------------
// RemoteJobItem
// ------------------------------------------------------------------------------------------------

RemoteJobItem::RemoteJobItem() : JobItem(GetStaticType())
{
  AddProperty(kJobIndex, std::size_t(0)).SetDisplayName("Job index").SetToolTip("Remote job index");

  AddProperty(kServerName, std::string())
      .SetDisplayName("Server name")
      .SetToolTip("Remote server name");
}

std::string RemoteJobItem::GetStaticType()
{
  return "RemoteJob";
}

std::unique_ptr<mvvm::SessionItem> RemoteJobItem::Clone() const
{
  return std::make_unique<RemoteJobItem>(*this);
}

std::size_t RemoteJobItem::GetRemoteJobIndex() const
{
  return Property<std::size_t>(kJobIndex);
}

void RemoteJobItem::SetRemoteJobIndex(std::size_t value)
{
  SetProperty(kJobIndex, value);
}

std::string RemoteJobItem::GetServerName() const
{
  return Property<std::string>(kServerName);
}

void RemoteJobItem::SetServerName(const std::string &name)
{
  SetProperty(kServerName, name);
}

std::vector<std::string> GetJobItemTypes()
{
  return {LocalJobItem::GetStaticType(), ImportedJobItem::GetStaticType(),
          RemoteJobItem::GetStaticType()};
}

// ------------------------------------------------------------------------------------------------
// Factory methods
// ------------------------------------------------------------------------------------------------

std::unique_ptr<JobItem> CreateLocalJobItem(ProcedureItem *procedure, int tick_timeout_msec)
{
  if (!procedure)
  {
    throw RuntimeException("Procedure is not initialized");
  }
  auto result = std::make_unique<LocalJobItem>();
  result->SetProcedure(procedure);
  result->SetDisplayName(procedure->GetDisplayName());
  result->SetTickTimeout(tick_timeout_msec);
  return result;
}

std::unique_ptr<JobItem> CreateImportedJobItem(std::unique_ptr<ProcedureItem> procedure,
                                               int tick_timeout_msec)
{
  if (!procedure)
  {
    throw RuntimeException("Procedure is not initialized");
  }

  auto result = std::make_unique<ImportedJobItem>();
  result->SetProcedure(procedure.get());
  result->SetDisplayName(procedure->GetDisplayName());
  result->SetTickTimeout(tick_timeout_msec);

  // inserting imported procedure into own container and thus taking an ownership
  mvvm::utils::InsertItem(std::move(procedure), result.get(),
                          mvvm::TagIndex::Append(ImportedJobItem::kImportedProcedure));
  return result;
}

std::unique_ptr<RemoteJobItem> CreateRemoteJobItem(const std::string &server_name,
                                                   std::size_t job_index)
{
  auto result = std::make_unique<RemoteJobItem>();
  result->SetServerName(server_name);
  result->SetRemoteJobIndex(job_index);
  result->SetDisplayName(server_name + "_" + std::to_string(job_index));
  return result;
}

}  // namespace oac_tree_gui
