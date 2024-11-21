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

#include "standard_job_items.h"

#include <sequencergui/model/procedure_item.h>

#include <mvvm/model/item_utils.h>

namespace sequencergui
{

namespace
{
const std::string kServerName = "kServerName";
const std::string kJobIndex = "kJobIndex";
}  // namespace

// ------------------------------------------------------------------------------------------------
// LocalJobItem
// ------------------------------------------------------------------------------------------------

LocalJobItem::LocalJobItem() : JobItem(Type) {}

std::unique_ptr<mvvm::SessionItem> LocalJobItem::Clone(bool make_unique_id) const
{
  return std::make_unique<LocalJobItem>(*this, make_unique_id);
}

// ------------------------------------------------------------------------------------------------
// ImportedJobItem
// ------------------------------------------------------------------------------------------------

ImportedJobItem::ImportedJobItem() : JobItem(Type)
{
  RegisterTag(mvvm::TagInfo(kImportedProcedure, 0, 1, {ProcedureItem::Type}));
}

std::unique_ptr<mvvm::SessionItem> ImportedJobItem::Clone(bool make_unique_id) const
{
  return std::make_unique<ImportedJobItem>(*this, make_unique_id);
}

// ------------------------------------------------------------------------------------------------
// RemoteJobItem
// ------------------------------------------------------------------------------------------------

RemoteJobItem::RemoteJobItem() : JobItem(Type)
{
  AddProperty(kJobIndex, size_t(0)).SetDisplayName("Job index").SetToolTip("Remote job index");

  AddProperty(kServerName, std::string())
      .SetDisplayName("Server name")
      .SetToolTip("Remote server name");
}

std::unique_ptr<mvvm::SessionItem> RemoteJobItem::Clone(bool make_unique_id) const
{
  return std::make_unique<RemoteJobItem>(*this, make_unique_id);
}

size_t RemoteJobItem::GetRemoteJobIndex() const
{
  return Property<size_t>(kJobIndex);
}

void RemoteJobItem::SetRemoteJobIndex(size_t value)
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

// ------------------------------------------------------------------------------------------------
// Factory methods
// ------------------------------------------------------------------------------------------------

std::unique_ptr<JobItem> CreateLocalJobItem(ProcedureItem *procedure)
{
  auto result = std::make_unique<LocalJobItem>();
  result->SetProcedure(procedure);
  return result;
}

std::unique_ptr<JobItem> CreateImportedJobItem(std::unique_ptr<ProcedureItem> procedure)
{
  auto result = std::make_unique<ImportedJobItem>();
  auto procedure_ptr = procedure.get();

  // we insert imported procedure in dedicated place, and link to it
  mvvm::utils::InsertItem(std::move(procedure), result.get(),
                          mvvm::TagIndex::Append(ImportedJobItem::kImportedProcedure));
  result->SetProcedure(procedure_ptr);  // set link

  return result;
}

std::unique_ptr<RemoteJobItem> CreateRemoteJobItem(const std::string &server_name, size_t job_index)
{
  auto result = std::make_unique<RemoteJobItem>();
  result->SetServerName(server_name);
  result->SetRemoteJobIndex(job_index);
  return result;
}

}  // namespace sequencergui
