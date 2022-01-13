/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Creation, execution and monitoring of the Sequencer procedures
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

#include "mvvm/standarditems/containeritem.h"
#include "mvvm/viewmodel/applicationmodel.h"

namespace sequencergui
{
SequencerModel::SequencerModel() : ModelView::ApplicationModel("SequencerModel")
{
  RegisterItem<ChannelAccessVariableItem>();
  RegisterItem<ConditionItem>();
  RegisterItem<CopyItem>();
  RegisterItem<EqualsItem>();
  RegisterItem<FallbackItem>();
  RegisterItem<FileVariableItem>();
  RegisterItem<ForceSuccessItem>();
  RegisterItem<IncludeItem>();
  RegisterItem<InputItem>();
  RegisterItem<InstructionContainerItem>();
  RegisterItem<InverterItem>();
  RegisterItem<LocalVariableItem>();
  RegisterItem<MessageItem>();
  RegisterItem<OutputItem>();
  RegisterItem<PVClientVariableItem>();
  RegisterItem<ParallelSequenceItem>();
  RegisterItem<ProcedureItem>();
  RegisterItem<RepeatItem>();
  RegisterItem<SequenceItem>();
  RegisterItem<UnknownInstructionItem>();
  RegisterItem<UnknownVariableItem>();
  RegisterItem<UserChoiceItem>();
  RegisterItem<WaitItem>();
  RegisterItem<WorkspaceItem>();

  SetupModel();
}

//! Returns InstructionItem corresponding to the given identifier.
InstructionItem *SequencerModel::GetInstruction(const std::string &identifier) const
{
  return dynamic_cast<InstructionItem *>(FindItem(identifier));
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
