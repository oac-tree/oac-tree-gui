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

#ifndef SEQUENCERGUI_MODEL_PROCEDURE_PREAMBLE_ITEMS_H_
#define SEQUENCERGUI_MODEL_PROCEDURE_PREAMBLE_ITEMS_H_

#include <mvvm/model/compound_item.h>

namespace sequencergui
{

//! Represents TypeRegistrationInfo.

class TypeRegistrationInfoItem : public mvvm::CompoundItem
{
public:
  static inline const std::string Type = "TypeRegistrationInfo";

  using CompoundItem::CompoundItem;
  TypeRegistrationInfoItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;
};

//! Represents ProcedurePreamble.

class ProcedurePreambleItem : public mvvm::CompoundItem
{
public:
  static inline const std::string Type = "ProcedurePreamble";

  using CompoundItem::CompoundItem;
  ProcedurePreambleItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MODEL_PROCEDURE_PREAMBLE_ITEMS_H_
