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

#include "instruction_editor_viewmodel.h"

#include "drag_and_drop_helper.h"

#include <sequencergui/model/sequencer_item_helper.h>
#include <sequencergui/model/standard_instruction_items.h>

#include <mvvm/factories/viewmodel_controller_factory.h>
#include <mvvm/interfaces/row_strategy_interface.h>
#include <mvvm/viewmodel/standard_children_strategies.h>
#include <mvvm/viewmodel/viewitem_factory.h>
#include <mvvm/viewmodelbase/viewitem.h>

#include <QDebug>
#include <QMimeData>

namespace sequencergui
{

class InstructionEditorRowStrategy : public mvvm::RowStrategyInterface
{
public:
  QStringList GetHorizontalHeaderLabels() const override
  {
    static QStringList result = {"Type", "Name"};
    return result;
  }

  std::vector<std::unique_ptr<mvvm::ViewItem>> ConstructRow(mvvm::SessionItem *item) override
  {
    std::vector<std::unique_ptr<mvvm::ViewItem>> result;

    if (!item)
    {
      return result;
    }

    result.emplace_back(mvvm::CreateDisplayNameViewItem(item));

    if (auto instruction = dynamic_cast<InstructionItem *>(item); instruction)
    {
      result.emplace_back(mvvm::CreateDataViewItem(GetNameItem(*instruction)));
    }
    return result;
  }
};

InstructionEditorViewModel::InstructionEditorViewModel(mvvm::SessionModelInterface *model,
                                                       QObject *parent)
    : ViewModel(parent)
{
  SetController(
      mvvm::factory::CreateController<mvvm::TopItemsStrategy, InstructionEditorRowStrategy>(model,
                                                                                            this));
}

Qt::ItemFlags InstructionEditorViewModel::flags(const QModelIndex &index) const
{
  auto default_flags = ViewModel::flags(index);
  if (index.isValid())
  {
    return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | default_flags;
  }

  return Qt::ItemIsDropEnabled | default_flags;
}

QMimeData *InstructionEditorViewModel::mimeData(const QModelIndexList &index_list) const
{
  qDebug() << "mimeData" << index_list;

  // ownership will be taken by QDrag operation
  return CreateInstructionMoveMimeData(index_list).release();
}

Qt::DropActions InstructionEditorViewModel::supportedDragActions() const
{
  return Qt::MoveAction;
}

Qt::DropActions InstructionEditorViewModel::supportedDropActions() const
{
  return Qt::MoveAction;
}

bool InstructionEditorViewModel::canDropMimeData(const QMimeData *data, Qt::DropAction, int row, int column, const QModelIndex &parent) const
{
  qDebug() << "canDropMimeData" << data << row << column << parent;
  return false;
}

bool InstructionEditorViewModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
  qDebug() << "dropMimeData" << data << action << row << column << parent;
  return false;

}

}  // namespace sequencergui
