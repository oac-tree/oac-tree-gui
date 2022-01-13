/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef WIDGETBASEDTEST_H
#define WIDGETBASEDTEST_H

#include <gtest/gtest.h>

class QApplication;

//! Convenience class to setup QApplication for tests involving QWidget creation.

class WidgetBasedTest : public ::testing::Test
{
public:
  static void SetUpTestSuite();

  static void TearDownTestSuite();

protected:
  static QApplication* m_app;
};

#endif  //  WIDGETBASEDTEST_H
