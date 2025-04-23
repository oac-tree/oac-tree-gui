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

#include "job_model.h"

#include <oac_tree_gui/model/sequencer_item_helper.h>

namespace oac_tree_gui
{

JobModel::JobModel(std::shared_ptr<mvvm::ItemPool> pool)
    : mvvm::ApplicationModel("JobModel", std::move(pool))
{
}

}  // namespace oac_tree_gui
