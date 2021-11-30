/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/model/sequencermodel.h"

#include "sequencergui/model/sequenceritems.h"

#include "mvvm/viewmodel/applicationmodel.h"
#include "mvvm/standarditems/containeritem.h"

namespace sequi
{
SequencerModel::SequencerModel() : ModelView::ApplicationModel("SequencerModel")
{
  RegisterItem<ConditionItem>();
  RegisterItem<CopyItem>();
  RegisterItem<FallbackItem>();
  RegisterItem<IncludeItem>();
  RegisterItem<InputItem>();
  RegisterItem<InverterItem>();
  RegisterItem<OutputItem>();
  RegisterItem<ParallelSequenceItem>();
  RegisterItem<RepeatItem>();
  RegisterItem<SequenceItem>();
  RegisterItem<WaitItem>();

  RegisterItem<InstructionContainerItem>();
  RegisterItem<LocalVariableItem>();
  RegisterItem<ProcedureItem>();
  RegisterItem<UnknownInstructionItem>();
  RegisterItem<WorkspaceItem>();

  SetupModel();
}

//! Returns InstructionItem corresponding to the given identifier.
InstructionItem *SequencerModel::GetInstruction(const std::string &identifier) const
{
  auto result = dynamic_cast<InstructionItem *>(FindItem(identifier));
  if (!result)
  {
    throw std::runtime_error("Error in SequencerModel: can't find instruction");
  }
  return result;
}

ModelView::ContainerItem *SequencerModel::GetProcedureContainer() const
{
  return m_procedure_container;
}

//! Creates initial content of the model.

void SequencerModel::SetupModel()
{
  m_procedure_container = InsertItem<ModelView::ContainerItem>();
  m_procedure_container->SetDisplayName("Procedure container");
}

}  // namespace sequi
