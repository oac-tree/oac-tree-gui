/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#ifndef OAC_TREE_GUI_MODEL_PROJECT_MODEL_H_
#define OAC_TREE_GUI_MODEL_PROJECT_MODEL_H_

#include <mvvm/model/application_model.h>

namespace oac_tree_gui
{

/**
 * @brief The ProjectModel class contains widget settings specific to the given project.
 *
 * It is a part of the XML project file, along with SequencerModel and JobModel.
 *
 * The ProjectModel is intended for any configuration that has to be restored after the user
 * opens the saved project. For example, it can be the configuration of procedure editors, where
 * various procedures are reopened in the correct places, as it was the last time the user saved the
 * project.
 */
class ProjectModel : public mvvm::ApplicationModel
{
public:
  explicit ProjectModel(std::shared_ptr<mvvm::ItemPool> pool = {});
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_MODEL_PROJECT_MODEL_H_
