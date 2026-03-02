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

#include "instruction_editor_drop_handler.h"

#include <oac_tree_gui/components/drag_and_drop_helper.h>
#include <oac_tree_gui/core/exceptions.h>

#include <mvvm/model/validate_utils.h>
#include <mvvm/providers/viewmodel_utils.h>

#include <QMimeData>
#include <QModelIndex>

namespace oac_tree_gui
{

InstructionEditorDropHandler::InstructionEditorDropHandler(
    const container_callback_t& container_callback)
    : m_container_callback(container_callback)
{
  if (!m_container_callback)
  {
    throw RuntimeException("Callback to retrieve current instruction container is not defined");
  }
}

InstructionEditorDropHandler::~InstructionEditorDropHandler() = default;

bool InstructionEditorDropHandler::CanDropMimeData(const QMimeData* data, Qt::DropAction action,
                                                   int32_t drop_row_indicator,
                                                   const QModelIndex& parent)
{
  if (data == nullptr)
  {
    return false;
  }

  auto parent_item = GetParentItem(parent);

  if (CanDropNewType(*data, action, drop_row_indicator, *parent_item))
  {
    return true;
  }

  if (CanDropInstructionEditorMimeData(*data, action, drop_row_indicator, *parent_item))
  {
    return true;
  }

  if (CanDropInstructionCopyMimeData(*data, action, drop_row_indicator, *parent_item))
  {
    return true;
  }

  return false;
}

bool InstructionEditorDropHandler::DropMimeData(const QMimeData* data, Qt::DropAction action,
                                                int32_t drop_row_indicator,
                                                const QModelIndex& parent)
{
  if (data == nullptr)
  {
    return false;
  }

  auto parent_item = GetParentItem(parent);

  // processing new instruction, if we can, and be done with it
  if (HandleDropNewType(*data, action, drop_row_indicator, *parent_item))
  {
    return true;
  }

  // processing move of existing instruction, if we can
  if (HandleDropInstructionEditorMimeData(*data, action, drop_row_indicator, *parent_item))
  {
    return true;
  }

  // processing copy on instruction copy-object
  if (HandleDropInstructionCopyMimeData(*data, action, drop_row_indicator, *parent_item))
  {
    return true;
  }

  return false;
}

mvvm::SessionItem* InstructionEditorDropHandler::GetParentItem(const QModelIndex& index)
{
  auto parent_item = mvvm::utils::ItemFromIndex(index);
  if (parent_item == nullptr)
  {
    parent_item = m_container_callback();
  }
  return parent_item;
}

}  // namespace oac_tree_gui
