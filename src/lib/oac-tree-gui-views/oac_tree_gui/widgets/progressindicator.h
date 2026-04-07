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

// Copyright (C) 2025 Jarek Kobus
// Copyright (C) 2025 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause
//
// Copy of progress indicator from Qt's TaskTree demo in examples/widgets/demo/progressindicator.

#ifndef OAC_TREE_GUI_WIDGETS_PROGRESS_INDICATOR_H_
#define OAC_TREE_GUI_WIDGETS_PROGRESS_INDICATOR_H_

#include <QObject>

namespace oac_tree_gui
{

enum class IndicatorType : int
{
  kHidden,
  kStatic,
  kAnimated
};

class ProgressIndicatorWidget;

class ProgressIndicator : public QObject
{
public:
  explicit ProgressIndicator(QWidget* parent_widget = nullptr);

  void SetIndicatorType(IndicatorType indicator_type);

private:
  ProgressIndicatorWidget* m_widget{nullptr};
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_WIDGETS_PROGRESS_INDICATOR_H_
