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

#include "sequencergui/model/jobmodel.h"

#include "sequencergui/model/sequencerutils.h"

#include "mvvm/interfaces/item_manager_interface.h"

namespace sequencergui
{

JobModel::JobModel() : JobModel(CreateSequencerItemManager()) {}

JobModel::JobModel(std::unique_ptr<mvvm::ItemManagerInterface> manager)
    : mvvm::ApplicationModel("JobModel", std::move(manager))
{
}

}  // namespace sequencergui
