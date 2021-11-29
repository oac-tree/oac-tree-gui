/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#ifndef SEQUENCERGUI_MODEL_SEQUENCERMODEL_H
#define SEQUENCERGUI_MODEL_SEQUENCERMODEL_H

#include "mvvm/viewmodel/applicationmodel.h"

namespace ModelView
{
class ContainerItem;
}

namespace sequi
{
class InstructionItem;

//! The model containing pulse schedule definition.
class SequencerModel : public ModelView::ApplicationModel
{
public:
  SequencerModel();

  InstructionItem* GetInstruction(const std::string& identifier) const;

  ModelView::ContainerItem* GetProcedureContainer() const;

private:
  void SetupModel();

  ModelView::ContainerItem* m_procedure_container{nullptr};
};

}  // namespace sequi

#endif // SEQUENCERGUI_MODEL_SEQUENCERMODEL_H
