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

#ifndef SEQUENCERGUI_MODEL_ATTRIBUTE_ITEM_H_
#define SEQUENCERGUI_MODEL_ATTRIBUTE_ITEM_H_

#include <mvvm/model/compound_item.h>

namespace sequencergui
{

/**
 * @brief The AttributeItem class represents an attribute on board of sequencer's instructions and
 * variables.
 *
 * @details It carries information regaring AttributeDefinition and attribute value. Used as
 * property item on board of UniversalInstructionItem and UniversalVariableItem.
 */

class AttributeItem : public mvvm::CompoundItem
{
public:
  static inline const std::string Type = "Attribute";

  using CompoundItem::CompoundItem;
  AttributeItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MODEL_ATTRIBUTE_ITEM_H_
