/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/model/sequencer_model.h"

#include "sequencergui/model/procedure_item.h"
#include "sequencergui/model/sequencer_utils.h"

#include "mvvm/interfaces/item_manager_interface.h"
#include "mvvm/model/application_model.h"
#include "mvvm/standarditems/container_item.h"

namespace sequencergui
{
SequencerModel::SequencerModel() : SequencerModel(CreateSequencerItemManager()) {}

SequencerModel::SequencerModel(std::unique_ptr<mvvm::ItemManagerInterface> manager)
    : mvvm::ApplicationModel("SequencerModel", std::move(manager))

{
  SetupModel();
}

mvvm::ContainerItem *SequencerModel::GetProcedureContainer() const
{
  return m_procedure_container;
}

std::vector<ProcedureItem *> SequencerModel::GetProcedures() const
{
  return GetProcedureContainer()->GetItems<ProcedureItem>(mvvm::ContainerItem::kChildren);
}

//! Creates initial content of the model.

void SequencerModel::SetupModel()
{
  m_procedure_container = InsertItem<mvvm::ContainerItem>();
  m_procedure_container->SetDisplayName("Procedure container");
}

}  // namespace sequencergui
