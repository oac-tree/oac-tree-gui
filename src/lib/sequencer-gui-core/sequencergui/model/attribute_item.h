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

#ifndef SEQUENCERGUI_MODEL_ATTRIBUTE_ITEM_H_
#define SEQUENCERGUI_MODEL_ATTRIBUTE_ITEM_H_

#include <sup/gui/model/anyvalue_item.h>

namespace sup::gui
{
class AnyValueItem;
}

namespace sequencergui
{

/**
 * @brief The AttributeItem class represents an attribute on board of sequencer's instructions and
 * variables.
 *
 * @details It carries information regaring AttributeDefinition and attribute value. Used as
 * property item on board of UniversalInstructionItem and UniversalVariableItem.
 */

using AttributeItem = sup::gui::AnyValueScalarItem;

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MODEL_ATTRIBUTE_ITEM_H_
