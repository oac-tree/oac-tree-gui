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
