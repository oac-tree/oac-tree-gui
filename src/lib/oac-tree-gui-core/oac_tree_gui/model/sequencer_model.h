/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef OAC_TREE_GUI_MODEL_SEQUENCER_MODEL_H_
#define OAC_TREE_GUI_MODEL_SEQUENCER_MODEL_H_

#include <mvvm/model/application_model.h>

namespace mvvm
{
class ContainerItem;
}

namespace oac_tree_gui
{
class ProcedureItem;

/**
 * @brief The SequencerModel class is a model containing a collection of sequencer procedures.
 */
class SequencerModel : public mvvm::ApplicationModel
{
public:
  explicit SequencerModel(std::shared_ptr<mvvm::ItemPool> pool = {});

  /**
   * @brief Returns container with procedures.
   */
  mvvm::ContainerItem* GetProcedureContainer() const;

  /**
   * @brief Returns list of all procedures in the procedure container.
   */
  std::vector<ProcedureItem*> GetProcedures() const;

  /**
   * @brief Creates untitled proceduer.
   */
  void CreateUntitledProcedure();

  void Clear() override;

private:
  void PopulateModel();
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_MODEL_SEQUENCER_MODEL_H_
