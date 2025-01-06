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

#ifndef SEQUENCERGUI_COMPONENTS_CUSTOM_META_TYPES_H_
#define SEQUENCERGUI_COMPONENTS_CUSTOM_META_TYPES_H_

//! Registration of custom types in Qt meta type system to provide support in signal/slot and
//! QVariant.

#include <sequencergui/jobsystem/log_event.h>

#include <QMetaType>
#include <QList>

namespace sequencergui
{

void RegisterCustomMetaTypes();

}  // namespace sequencergui

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
Q_DECLARE_METATYPE(sequencergui::LogEvent)
Q_DECLARE_METATYPE(QList<int>)
#endif

#endif  // SEQUENCERGUI_COMPONENTS_CUSTOM_META_TYPES_H_
