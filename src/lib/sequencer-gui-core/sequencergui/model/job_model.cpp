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

#include "job_model.h"

#include <sequencergui/model/sequencer_item_helper.h>

namespace sequencergui
{

JobModel::JobModel(std::shared_ptr<mvvm::ItemPool> pool)
    : mvvm::ApplicationModel("JobModel", std::move(pool))
{
}

}  // namespace sequencergui
