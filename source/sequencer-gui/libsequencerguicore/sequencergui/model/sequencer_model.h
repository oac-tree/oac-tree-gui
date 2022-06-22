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

#ifndef SEQUENCERGUI_MODEL_SEQUENCER_MODEL_H_
#define SEQUENCERGUI_MODEL_SEQUENCER_MODEL_H_

#include <mvvm/model/application_model.h>

namespace mvvm
{
class ContainerItem;
}

namespace sequencergui
{
class ProcedureItem;

//! The model containing a collection of sequencer procedures.
//! Used during procedure assembly.

class SequencerModel : public mvvm::ApplicationModel
{
public:
  SequencerModel();
  explicit SequencerModel(std::unique_ptr<mvvm::ItemManagerInterface> manager);

  mvvm::ContainerItem* GetProcedureContainer() const;

  std::vector<ProcedureItem*> GetProcedures() const;

private:
  void SetupModel();

  mvvm::ContainerItem* m_procedure_container{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MODEL_SEQUENCER_MODEL_H_
