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

#ifndef SEQUENCERGUI_MODEL_SETTINGS_MODEL_H_
#define SEQUENCERGUI_MODEL_SETTINGS_MODEL_H_

#include <mvvm/model/application_model.h>

namespace sequencergui
{

/**
 * @brief Contains common application settings.
 *
 * It provides a collection of editable setting items, which is written in persistent storage thanks
 * to QSettings machinery.
 */
class SettingsModel : public mvvm::ApplicationModel
{
public:
  SettingsModel();
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MODEL_SETTINGS_MODEL_H_
