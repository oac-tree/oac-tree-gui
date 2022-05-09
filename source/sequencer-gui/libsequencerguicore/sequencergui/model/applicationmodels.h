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

#ifndef SEQUENCERGUI_MODEL_APPLICATIONMODELS_H
#define SEQUENCERGUI_MODEL_APPLICATIONMODELS_H

#include <memory>

namespace mvvm
{
class ItemPool;
}

namespace sequencergui
{

class SequencerModel;
class JobModel;

//! Central place to store all application models.
//! All models are using the same memory pool to allow cross-model item search.

class ApplicationModels
{
public:
  ApplicationModels();
  ~ApplicationModels();

  SequencerModel* GetSequencerModel();

  JobModel* GetJobModel();

private:
  std::shared_ptr<mvvm::ItemPool> m_item_pool;
  std::unique_ptr<SequencerModel> m_sequencer_model;
  std::unique_ptr<JobModel> m_job_model;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MODEL_APPLICATIONMODELS_H
