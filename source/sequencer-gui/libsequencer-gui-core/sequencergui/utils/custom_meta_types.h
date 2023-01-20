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

#ifndef SEQUENCERGUI_UTILS_CUSTOM_META_TYPES_H_
#define SEQUENCERGUI_UTILS_CUSTOM_META_TYPES_H_

//! @file custom_meta_types.h
//! Registration of custom types in Qt meta type system to provide support in signal/slot and
//! QVariant.

#include <sequencergui/jobsystem/log_event.h>

#include <QMetaType>

Q_DECLARE_METATYPE(sequencergui::LogEvent)

namespace sequencergui
{

void RegisterCustomMetaTypes();

}  // namespace sequencergui

#endif  // SEQUENCERGUI_UTILS_ALIGN_UTILS_H_
