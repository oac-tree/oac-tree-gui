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

#include "procedure_preamble_items.h"

namespace sequencergui
{

// ----------------------------------------------------------------------------
// TypeRegistrationInfoItem
// ----------------------------------------------------------------------------

TypeRegistrationInfoItem::TypeRegistrationInfoItem() : CompoundItem(Type) {}

std::unique_ptr<mvvm::SessionItem> TypeRegistrationInfoItem::Clone(bool make_unique_id) const
{
  return std::make_unique<TypeRegistrationInfoItem>(*this, make_unique_id);
}

// ----------------------------------------------------------------------------
// ProcedurePreambleItem
// ----------------------------------------------------------------------------

ProcedurePreambleItem::ProcedurePreambleItem() : CompoundItem(Type) {}

std::unique_ptr<mvvm::SessionItem> ProcedurePreambleItem::Clone(bool make_unique_id) const
{
  return std::make_unique<ProcedurePreambleItem>(*this, make_unique_id);
}

}  // namespace sequencergui
