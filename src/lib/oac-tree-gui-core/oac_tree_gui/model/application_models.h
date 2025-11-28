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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef OAC_TREE_GUI_MODEL_APPLICATION_MODELS_H_
#define OAC_TREE_GUI_MODEL_APPLICATION_MODELS_H_

#include <mvvm/project/app_project.h>

#include <memory>
#include <string_view>

namespace mvvm
{
class ItemPool;
class ISessionModel;
class ProjectContext;
}  // namespace mvvm

namespace oac_tree_gui
{

class SequencerModel;
class JobModel;

/**
 * @brief The ApplicationModels class stores all application models.
 *
 * All models are using the same memory pool to allow cross-model item search.
 */
class ApplicationModels : public mvvm::AppProject
{
public:
  static constexpr std::string_view kApplicationType = "oac-tree GUI";

  ApplicationModels();
  explicit ApplicationModels(const mvvm::ProjectContext& context);
  ~ApplicationModels() override;

  ApplicationModels(const ApplicationModels&) = delete;
  ApplicationModels& operator=(const ApplicationModels&) = delete;
  ApplicationModels(ApplicationModels&&) = delete;
  ApplicationModels& operator=(ApplicationModels&&) = delete;

  SequencerModel* GetSequencerModel();

  JobModel* GetJobModel();

  std::shared_ptr<mvvm::ItemPool> GetItemPool();

private:
  std::size_t m_sequencer_model_index{0};
  std::size_t m_job_model_index{0};
  std::shared_ptr<mvvm::ItemPool> m_item_pool;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_MODEL_APPLICATION_MODELS_H_
