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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "standard_job_items.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/model/procedure_item.h>

#include <mvvm/utils/file_utils.h>

namespace oac_tree_gui
{

namespace
{
constexpr auto kServerName = "kServerName";
constexpr auto kJobIndex = "kJobIndex";
constexpr auto kFileName = "kFileName";
}  // namespace

// ------------------------------------------------------------------------------------------------
// LocalJobItem
// ------------------------------------------------------------------------------------------------

LocalJobItem::LocalJobItem() : JobItem(mvvm::GetTypeName<LocalJobItem>()) {}

std::unique_ptr<mvvm::SessionItem> LocalJobItem::Clone() const
{
  return std::make_unique<LocalJobItem>(*this);
}

// ------------------------------------------------------------------------------------------------
// ImportedJobItem
// ------------------------------------------------------------------------------------------------

ImportedJobItem::ImportedJobItem() : JobItem(mvvm::GetTypeName<ImportedJobItem>())
{
  RegisterTag(
      mvvm::TagInfo(std::string(kImportedProcedure), 0, 1, {mvvm::GetTypeName<ProcedureItem>()}));
}

std::unique_ptr<mvvm::SessionItem> ImportedJobItem::Clone() const
{
  return std::make_unique<ImportedJobItem>(*this);
}

// ------------------------------------------------------------------------------------------------
// RemoteJobItem
// ------------------------------------------------------------------------------------------------

RemoteJobItem::RemoteJobItem() : JobItem(mvvm::GetTypeName<RemoteJobItem>())
{
  (void)AddProperty(kJobIndex, std::size_t(0))
      .SetDisplayName("Job index")
      .SetToolTip("Remote job index");

  (void)AddProperty(kServerName, std::string())
      .SetDisplayName("Server name")
      .SetToolTip("Remote server name");
}

std::unique_ptr<mvvm::SessionItem> RemoteJobItem::Clone() const
{
  return std::make_unique<RemoteJobItem>(*this);
}

std::size_t RemoteJobItem::GetRemoteJobIndex() const
{
  return Property<std::uint64_t>(kJobIndex);
}

void RemoteJobItem::SetRemoteJobIndex(std::size_t value)
{
  (void)SetProperty(kJobIndex, value);
}

std::string RemoteJobItem::GetServerName() const
{
  return Property<std::string>(kServerName);
}

void RemoteJobItem::SetServerName(const std::string& name)
{
  (void)SetProperty(kServerName, name);
}

// ------------------------------------------------------------------------------------------------
// FileBasedJobItem
// ------------------------------------------------------------------------------------------------

FileBasedJobItem::FileBasedJobItem() : JobItem(mvvm::GetTypeName<FileBasedJobItem>())
{
  (void)AddProperty(kFileName, std::string()).SetDisplayName("File name");
}

std::unique_ptr<mvvm::SessionItem> FileBasedJobItem::Clone() const
{
  return std::make_unique<FileBasedJobItem>(*this);
}

std::string FileBasedJobItem::GetFileName() const
{
  return Property<std::string>(kFileName);
}

void FileBasedJobItem::SetFileName(const std::string& value)
{
  (void)SetProperty(kFileName, value);
}

std::vector<std::string> GetJobItemTypes()
{
  return {mvvm::GetTypeName<LocalJobItem>(), mvvm::GetTypeName<ImportedJobItem>(),
          mvvm::GetTypeName<RemoteJobItem>(), mvvm::GetTypeName<FileBasedJobItem>()};
}

// ------------------------------------------------------------------------------------------------
// Factory methods
// ------------------------------------------------------------------------------------------------

std::unique_ptr<JobItem> CreateLocalJobItem(ProcedureItem* procedure,
                                            std::chrono::milliseconds tick_timeout)
{
  if (procedure == nullptr)
  {
    throw RuntimeException("Procedure is not initialized");
  }
  auto result = std::make_unique<LocalJobItem>();
  result->SetProcedure(procedure);
  (void)result->SetDisplayName(procedure->GetDisplayName());
  result->SetTickTimeout(tick_timeout);
  return result;
}

std::unique_ptr<JobItem> CreateImportedJobItem(std::unique_ptr<ProcedureItem> procedure,
                                               std::chrono::milliseconds tick_timeout)
{
  if (!procedure)
  {
    throw RuntimeException("Procedure is not initialized");
  }

  auto result = std::make_unique<ImportedJobItem>();
  result->SetProcedure(procedure.get());
  (void)result->SetDisplayName(procedure->GetDisplayName());
  result->SetTickTimeout(tick_timeout);

  // inserting imported procedure into own container and thus taking an ownership
  (void)result->InsertItem(
      std::move(procedure),
      mvvm::TagIndex::Append(std::string(ImportedJobItem::kImportedProcedure)));
  return result;
}

std::unique_ptr<RemoteJobItem> CreateRemoteJobItem(const std::string& server_name,
                                                   std::size_t job_index)
{
  auto result = std::make_unique<RemoteJobItem>();
  result->SetServerName(server_name);
  result->SetRemoteJobIndex(job_index);
  (void)result->SetDisplayName(server_name + "_" + std::to_string(job_index));
  return result;
}

std::unique_ptr<JobItem> CreateFileBasedJobItem(const std::string& file_name,
                                                std::chrono::milliseconds tick_timeout)
{
  auto result = std::make_unique<FileBasedJobItem>();
  result->SetFileName(file_name);
  result->SetTickTimeout(tick_timeout);
  auto procedure_name = mvvm::utils::GetPathStem(file_name);
  (void)result->SetDisplayName(procedure_name);

  return result;
}

}  // namespace oac_tree_gui
