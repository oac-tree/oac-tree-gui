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

#include "instruction_toolkit_viewmodel.h"

#include <sequencergui/domain/domain_utils.h>

#include <memory>

namespace
{

std::unique_ptr<QStandardItem> CreateItem(const std::string& name)
{
  auto result = std::make_unique<QStandardItem>(QString::fromStdString(name));
  result->setEditable(false);
  return result;
}

}  // namespace

namespace sequencergui
{

InstructionToolKitViewModel::InstructionToolKitViewModel(QObject* parent)
    : QStandardItemModel(parent)
{
  PopulateModel();
}

void InstructionToolKitViewModel::PopulateModel()
{
  auto parent_item = invisibleRootItem();

  for (const auto& name : GetDomainInstructionNames())
  {
    parent_item->appendRow(CreateItem(name).release());
  }
}

}  // namespace sequencergui
