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

#include <oac_tree_gui/core/exceptions.h>

#include <QDebug>
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
  const QStringList formats = data ? data->formats() : QStringList();
  qDebug() << "InstructionEditorViewModel::canDropMimeData:" << data << formats
           << ", action:" << action << ", row:" << drop_row_indicator << ", parent:" << parent;
  return false;
}

bool InstructionEditorDropHandler::DropMimeData(const QMimeData* data, Qt::DropAction action,
                                                int32_t drop_row_indicator,
                                                const QModelIndex& parent)
{
  (void)data;
  (void)action;
  (void)drop_row_indicator;
  (void)parent;
  return false;
}

}  // namespace oac_tree_gui
