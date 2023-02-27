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

#ifndef GUI_DTO_ANYVALUE_ITEM_UTILS_H_
#define GUI_DTO_ANYVALUE_ITEM_UTILS_H_

//! Collection of sup::gui::AnyValueItem related utility functions.

namespace sup::gui
{

class AnyValueItem;

//! Update the data stored in \it target from the data of \it source.
//! It is assumed that both items represent scalars, and the scalar types are the same.
void UpdateAnyValueItemScalarData(const AnyValueItem& source, AnyValueItem& target);

//! Update the data stored in leaves of \it target from the data stored in leaves of \it source.
//! The layout of two items should be the same.
void UpdateAnyValueItemData(const AnyValueItem& source, AnyValueItem& target);

}  // namespace sup::gui

#endif  // GUI_DTO_ANYVALUE_ITEM_UTILS_H_
