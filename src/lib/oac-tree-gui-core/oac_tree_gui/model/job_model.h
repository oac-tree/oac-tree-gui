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

#ifndef OAC_TREE_GUI_MODEL_JOB_MODEL_H_
#define OAC_TREE_GUI_MODEL_JOB_MODEL_H_

#include <mvvm/model/application_model.h>

namespace oac_tree_gui
{

//! The model to hold running procedures.

class JobModel : public mvvm::ApplicationModel
{
public:
  explicit JobModel(std::shared_ptr<mvvm::ItemPool> pool = {});
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_MODEL_JOB_MODEL_H_
