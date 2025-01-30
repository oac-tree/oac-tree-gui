/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_MODEL_OAC_TREE_MODEL_H_
#define SEQUENCERGUI_MODEL_OAC_TREE_MODEL_H_

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

/**
 * @brief The SequencerProjectModel class is an extension of original SequencerModel which creates
 * untitled procedure each time when cleared.
 *
 * This is the only way with current design of AbstractProject
 * - to create untitled procedure on each "new project" activity
 * - have the document in unmodified state
 *
 * FIXME remove this class after AbstractProject refactoring
 */
class SequencerProjectModel : public SequencerModel
{
public:
  explicit SequencerProjectModel(std::shared_ptr<mvvm::ItemPool> pool = {});

  void Clear() override;
};

}  // namespace oac_tree_gui

#endif  // SEQUENCERGUI_MODEL_OAC_TREE_MODEL_H_
