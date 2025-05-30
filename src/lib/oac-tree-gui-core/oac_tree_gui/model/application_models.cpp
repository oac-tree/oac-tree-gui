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

#include "application_models.h"

#include <oac_tree_gui/model/job_model.h>
#include <oac_tree_gui/model/sequencer_item_helper.h>
#include <oac_tree_gui/model/sequencer_model.h>

#include <mvvm/model/item_pool.h>

namespace oac_tree_gui
{

ApplicationModels::ApplicationModels() : ApplicationModels(mvvm::ProjectContext{}) {}

ApplicationModels::ApplicationModels(const mvvm::ProjectContext &context) : AppProject(context)
{
  auto sequenser_model_factory_func = [this]()
  {
    // Recreating item pool which will be used for both models. Here we rely on the fact, that
    // this lambda will be called first.
    m_item_pool = std::make_shared<mvvm::ItemPool>();
    auto result = std::make_unique<SequencerModel>(m_item_pool);
    result->CreateUntitledProcedure();
    return result;
  };
  m_sequencer_model_index = RegisterModel(sequenser_model_factory_func);

  auto job_model_factory_func = [this]() { return std::make_unique<JobModel>(m_item_pool); };
  m_job_model_index = RegisterModel(job_model_factory_func);
}

ApplicationModels::~ApplicationModels() = default;

SequencerModel *ApplicationModels::GetSequencerModel()
{
  return GetModelCount() > m_sequencer_model_index
             ? GetModel<SequencerModel>(m_sequencer_model_index)
             : nullptr;
}

JobModel *ApplicationModels::GetJobModel()
{
  return GetModelCount() > m_job_model_index ? GetModel<JobModel>(m_job_model_index) : nullptr;
}

std::shared_ptr<mvvm::ItemPool> ApplicationModels::GetItemPool()
{
  return m_item_pool;
}

}  // namespace oac_tree_gui
