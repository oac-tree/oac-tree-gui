// Copyright (C) 2025 Jarek Kobus
// Copyright (C) 2025 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef PROGRESSINDICATOR_H
#define PROGRESSINDICATOR_H

#include <QObject>

namespace oac_tree_gui
{

enum class IndicatorType : int
{
  kHidden,
  kStatic,
  kAnimated
};

class ProgressIndicator : public QObject
{
public:
  explicit ProgressIndicator(QWidget* parent = nullptr);

  void SetIndicatorType(IndicatorType indicator_type);

  // void show();
  // void hide();

private:
  class ProgressIndicatorWidget* m_widget = nullptr;
};

}  // namespace oac_tree_gui

#endif  // PROGRESSINDICATOR_H
