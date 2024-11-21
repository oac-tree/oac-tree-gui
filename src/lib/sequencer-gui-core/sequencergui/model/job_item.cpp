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

#include "job_item.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/model/item_constants.h>
#include <sequencergui/model/procedure_item.h>

#include <mvvm/standarditems/linked_item.h>

namespace sequencergui
{

static inline const std::string kLink = "kLink";
static inline const std::string kExpandedProcedure = "kExpandedProcedure";

JobItem::JobItem(const std::string &type) : CompoundItem(type)
{
  AddProperty(itemconstants::kStatus, std::string()).SetDisplayName("Status").SetEditable(false);
  AddProperty<mvvm::LinkedItem>(kLink).SetDisplayName("Link");
  AddProperty(itemconstants::kTickTimeout, itemconstants::kDefaultTickTimeoutMsec)
      .SetDisplayName("Tick timeout");

  RegisterTag(mvvm::TagInfo(kExpandedProcedure, 0, 1, {ProcedureItem::Type}), /*as_default*/ true);

  SetFlag(mvvm::Appearance::kEditableDisplayName, true);
}

std::unique_ptr<mvvm::SessionItem> JobItem::Clone(bool make_unique_id) const
{
  // This base is not intended to be used directly
  throw NotImplementedException("Clone for JobItem is not implemented");
}

std::string JobItem::GetStatus() const
{
  return Property<std::string>(itemconstants::kStatus);
}

void JobItem::SetStatus(const std::string &status)
{
  SetProperty(itemconstants::kStatus, status);
}

int JobItem::GetTickTimeout() const
{
  return Property<int>(itemconstants::kTickTimeout);
}

void JobItem::SetTickTimeout(int msec)
{
  SetProperty(itemconstants::kTickTimeout, msec);
}

void JobItem::SetProcedure(const ProcedureItem *item)
{
  GetItem<mvvm::LinkedItem>(kLink)->SetLink(item);
}

ProcedureItem *JobItem::GetProcedure() const
{
  return GetItem<mvvm::LinkedItem>(kLink)->Get<ProcedureItem>();
}

ProcedureItem *JobItem::GetExpandedProcedure()
{
  return GetItem<ProcedureItem>(kExpandedProcedure);
}

}  // namespace sequencergui
