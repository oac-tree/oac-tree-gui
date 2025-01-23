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

#ifndef SEQUENCERGUI_MODEL_APPLICATION_MODELS_H_
#define SEQUENCERGUI_MODEL_APPLICATION_MODELS_H_

#include <mvvm/project/app_project.h>

#include <memory>

namespace mvvm
{
class ItemPool;
class ISessionModel;
class ProjectContext;
}  // namespace mvvm

namespace sequencergui
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
  static inline const std::string kApplicationType = "oac-tree GUI";

  ApplicationModels();
  explicit ApplicationModels(const mvvm::ProjectContext& context);
  ~ApplicationModels() override;

  SequencerModel* GetSequencerModel();

  JobModel* GetJobModel();

  std::shared_ptr<mvvm::ItemPool> GetItemPool();

private:
  std::size_t m_sequencer_model_index{0};
  std::size_t m_job_model_index{0};
  std::shared_ptr<mvvm::ItemPool> m_item_pool;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MODEL_APPLICATION_MODELS_H_
