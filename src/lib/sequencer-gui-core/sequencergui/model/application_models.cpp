/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "application_models.h"

#include <sequencergui/model/job_model.h>
#include <sequencergui/model/sequencer_item_helper.h>
#include <sequencergui/model/sequencer_model.h>

#include <mvvm/model/item_pool.h>

namespace sequencergui
{

namespace
{

/**
 * @brief Creates context to pass to AbstractProject.
 */
mvvm::ProjectContext CreateContext(ApplicationModels::callback_t modified_callback,
                                   ApplicationModels::callback_t loaded_callback)
{
  return {std::move(modified_callback), std::move(loaded_callback),
          ApplicationModels::kApplicationType};
}

}  // namespace

ApplicationModels::ApplicationModels() : ApplicationModels({}, {}) {}

ApplicationModels::ApplicationModels(callback_t modified_callback, callback_t loaded_callback)
    : AppProject(CreateContext(modified_callback, loaded_callback))
{
  auto sequenser_model_factory_func = [this]()
  {
    // Recreating item pool which will be used for both models. Here we rely on the fact, that
    // this lambda will be called first.
    m_item_pool = std::make_shared<mvvm::ItemPool>();
    auto result = std::make_unique<SequencerProjectModel>(m_item_pool);
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

}  // namespace sequencergui
