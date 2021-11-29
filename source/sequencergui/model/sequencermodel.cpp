/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#include "sequencergui/model/sequencermodel.h"

#include "sequencergui/model/sequenceritems.h"

#include "mvvm/viewmodel/applicationmodel.h"
#include "mvvm/standarditems/containeritem.h"

namespace sequi
{
SequencerModel::SequencerModel() : ModelView::ApplicationModel("SequencerModel")
{
  RegisterItem<CopyItem>();
  RegisterItem<FallbackItem>();
  RegisterItem<InputItem>();
  RegisterItem<InverterItem>();
  RegisterItem<OutputItem>();
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
