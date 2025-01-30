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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef OAC_TREE_GUI_COMPONENTS_CUSTOM_META_TYPES_H_
#define OAC_TREE_GUI_COMPONENTS_CUSTOM_META_TYPES_H_

//! Registration of custom types in Qt meta type system to provide support in signal/slot and
//! QVariant.

#include <oac_tree_gui/jobsystem/log_event.h>

#include <QList>
#include <QMetaType>

namespace oac_tree_gui
{

void RegisterCustomMetaTypes();

}  // namespace oac_tree_gui

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
Q_DECLARE_METATYPE(oac_tree_gui::LogEvent)
Q_DECLARE_METATYPE(QList<int>)
#endif  // OAC_TREE_GUI_COMPONENTS_CUSTOM_META_TYPES_H_

#endif  // OAC_TREE_GUI_COMPONENTS_CUSTOM_META_TYPES_H_
