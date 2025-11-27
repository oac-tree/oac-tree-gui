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

#include "job_item.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/model/procedure_item.h>

#include <mvvm/standarditems/linked_item.h>

namespace oac_tree_gui
{

namespace
{
constexpr auto kLink = "kLink";
constexpr auto kExpandedProcedure = "kExpandedProcedure";
}  // namespace

JobItem::JobItem(const std::string& type) : CompoundItem(type)
{
  (void)AddProperty(itemconstants::kStatus, std::string())
      .SetDisplayName("Status")
      .SetEditable(false);
  (void)AddProperty<mvvm::LinkedItem>(kLink).SetDisplayName("Link");
  (void)AddProperty(itemconstants::kTickTimeout, itemconstants::kDefaultTickTimeoutMsec)
      .SetDisplayName("Tick timeout");

  RegisterTag(mvvm::TagInfo(kExpandedProcedure, 0, 1, {mvvm::GetTypeName<ProcedureItem>()}),
              /*as_default*/ true);

  (void)SetFlag(mvvm::Appearance::kEditableDisplayName, true);
}

std::unique_ptr<mvvm::SessionItem> JobItem::Clone() const
{
  // This base is not intended to be used directly
  throw NotImplementedException("Clone for JobItem is not implemented");
}

RunnerStatus JobItem::GetStatus() const
{
  return GetRunnerStatus(Property<std::string>(itemconstants::kStatus));
}

void JobItem::SetStatus(RunnerStatus status)
{
  // internally we store status as a string so all views can show it
  (void)SetProperty(itemconstants::kStatus, ToString(status));
}

int JobItem::GetTickTimeout() const
{
  return Property<int>(itemconstants::kTickTimeout);
}

void JobItem::SetTickTimeout(int msec)
{
  (void)SetProperty(itemconstants::kTickTimeout, msec);
}

void JobItem::SetProcedure(const ProcedureItem* item)
{
  GetItem<mvvm::LinkedItem>(kLink)->SetLink(item);
}

ProcedureItem* JobItem::GetProcedure() const
{
  return GetItem<mvvm::LinkedItem>(kLink)->Get<ProcedureItem>();
}

ProcedureItem* JobItem::GetExpandedProcedure()
{
  return GetItem<ProcedureItem>(kExpandedProcedure);
}

}  // namespace oac_tree_gui
