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

#include "sequencergui/model/sequencermodel.h"

#include "sequencergui/model/sequenceritems.h"

#include "mvvm/model/applicationmodel.h"
#include "mvvm/model/itemmanager.h"
#include "mvvm/standarditems/containeritem.h"

namespace sequencergui
{
SequencerModel::SequencerModel() : mvvm::ApplicationModel("SequencerModel")
{
  // instructions
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
  RegisterItem<ListenItem>();
  RegisterItem<MessageItem>();
  RegisterItem<OutputItem>();
  RegisterItem<ParallelSequenceItem>();
  RegisterItem<ProcedureItem>();
  RegisterItem<RepeatItem>();
  RegisterItem<SequenceItem>();
  RegisterItem<UnknownInstructionItem>();
  RegisterItem<UserChoiceItem>();
  RegisterItem<WaitItem>();
  RegisterItem<WorkspaceItem>();

  // variables
  RegisterItem<ChannelAccessVariableItem>();
  RegisterItem<LocalVariableItem>();
  RegisterItem<PVClientVariableItem>();
  RegisterItem<UnknownVariableItem>();

  SetupModel();
}

//! Returns InstructionItem corresponding to the given identifier.
InstructionItem *SequencerModel::GetInstruction(const std::string &identifier) const
{
  return dynamic_cast<InstructionItem *>(FindItem(identifier));
}

mvvm::ContainerItem *SequencerModel::GetProcedureContainer() const
{
  return m_procedure_container;
}

//! Creates initial content of the model.

void SequencerModel::SetupModel()
{
  m_procedure_container = InsertItem<mvvm::ContainerItem>();
  m_procedure_container->SetDisplayName("Procedure container");
}

}  // namespace sequencergui
