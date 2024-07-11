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

#ifndef SEQUENCERGUI_COMPONENTS_APP_CONSTANTS_H_
#define SEQUENCERGUI_COMPONENTS_APP_CONSTANTS_H_

#include <sup/gui/app/app_context.h>

#include <QString>

namespace sequencergui::app::constants
{

const QString kValidateProcedureCommandId = "Validate procedure";
const QString kExportXmlCommandId = "Export XML";

}  // namespace sequencergui::app::constants

#endif  // SEQUENCERGUI_COMPONENTS_APP_CONSTANTS_H_
