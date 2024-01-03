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

#ifndef SEQUENCERGUI_VIEWMODEL_VIEWMODEL_HELPER_H_
#define SEQUENCERGUI_VIEWMODEL_VIEWMODEL_HELPER_H_

//! @file
//! Helper functions for view models.

#include <functional>

class QAbstractItemModel;
class QModelIndex;

namespace sequencergui
{

/**
 * @brief Iterates viewmodel and calls user function on every index.
 *
 * @details The method goes through column=0 only.
 */
void IterateFirstColumn(const QAbstractItemModel& viewmodel, const QModelIndex& parent,
                        const std::function<const void(const QModelIndex& child)>& func);

}  // namespace sequencergui

#endif  // SEQUENCERGUI_VIEWMODEL_VIEWMODEL_HELPER_H_
