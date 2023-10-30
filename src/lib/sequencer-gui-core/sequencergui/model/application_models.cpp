/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
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

#include <mvvm/interfaces/item_manager_interface.h>
#include <mvvm/model/item_pool.h>

namespace sequencergui
{

ApplicationModels::ApplicationModels()
    : m_item_pool(std::make_shared<mvvm::ItemPool>())
    , m_sequencer_model(std::make_unique<SequencerModel>(CreateSequencerItemManager(m_item_pool)))
    , m_job_model(std::make_unique<JobModel>(CreateSequencerItemManager(m_item_pool)))
{
}

ApplicationModels::~ApplicationModels() = default;

SequencerModel *ApplicationModels::GetSequencerModel()
{
  return m_sequencer_model.get();
}

JobModel *ApplicationModels::GetJobModel()
{
  return m_job_model.get();
}

std::vector<mvvm::ApplicationModel *> ApplicationModels::GetModels() const
{
  return {m_sequencer_model.get(), m_job_model.get()};
}

}  // namespace sequencergui
