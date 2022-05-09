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

#include "sequencergui/model/applicationmodels.h"

#include "sequencergui/model/sequencermodel.h"
#include "sequencergui/model/jobmodel.h"

namespace sequencergui
{

ApplicationModels::ApplicationModels()
{
  m_sequencer_model = std::make_unique<SequencerModel>();
  m_job_model = std::make_unique<JobModel>();
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

}  // namespace sequencergui
