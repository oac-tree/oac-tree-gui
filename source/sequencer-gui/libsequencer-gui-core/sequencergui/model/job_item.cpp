/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/model/job_item.h"

#include <sequencergui/model/item_constants.h>
#include <sequencergui/model/procedure_item.h>

#include <mvvm/standarditems/linked_item.h>

namespace sequencergui
{

static inline const std::string kLink = "kLink";
static inline const std::string kExpandedProcedure = "kExpandedProcedure";

JobItem::JobItem() : CompoundItem(Type)
{
  AddProperty(itemconstants::kStatus, std::string())
      ->SetDisplayName("Status")
      ->SetEditable(false);
  AddProperty<mvvm::LinkedItem>(kLink)->SetDisplayName("Link");

  RegisterTag(mvvm::TagInfo(kExpandedProcedure, 0, 1, {ProcedureItem::Type}), /*as_default*/ true);
}

std::string JobItem::GetStatus() const
{
  return Property<std::string>(itemconstants::kStatus);
}

void JobItem::SetStatus(const std::string &status)
{
  SetProperty(itemconstants::kStatus, status);
}

//! Sets link to track given item.
void JobItem::SetProcedure(const ProcedureItem *item)
{
  GetItem<mvvm::LinkedItem>(kLink)->SetLink(item);
}

//! Returns procedure linked to this JobItem.
ProcedureItem *JobItem::GetProcedure() const
{
  return GetItem<mvvm::LinkedItem>(kLink)->Get<ProcedureItem>();
}

//! Returns procedure
ProcedureItem *JobItem::GetExpandedProcedure()
{
  return GetItem<ProcedureItem>(kExpandedProcedure);
}

}  // namespace sequencergui
